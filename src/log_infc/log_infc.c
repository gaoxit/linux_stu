#include <unistd.h>
#include <stdarg.h>
#include "log_infc.h"
#include "errno.h"


static void tlog_handle_real_log(log_msg_t *msg);
static void record_enqueue(int8_t type, log_record_t *record, bool force_sync);

log_mgr_t log_mgr;

void tlog_handle_log_msg(char* data, uint32_t(len));

task_callback_ret_t tlog_thread(task_callback_para_t arg)
{
    return NULL;
}

size_t tlog_strcpy(size_t cur_len, char *dst, const char *src)
{
    const char *src_old = src;

    while (*src != 0)
    {
        /* make sure destination has enough space */
        if (cur_len++ < RECORD_DATA_MAX_LEN)
        {
            *dst++ = *src++;
        }
        else
        {
            break;
        }
    }
    return src - src_old;
}

void tlog_get_line_info(long line, char *cur_line_info)
{
    snprintf(cur_line_info, LOG_LINE_INFO_LEN, "%05ld", line);
}

// void tlog_get_time(char *format_time_buf, uint16_t buf_len)
// {
//     cp56time2a_t cur_time;
//     get_time_in_cp56time2a(&cur_time);
//     cp56time2a_to_str(&cur_time, format_time_buf, buf_len);
// }


int32_t plm_tlog(const char *tag, uint32_t level, const char *func,
            uint64_t line, const char *format, ...)
{
    uint16_t          log_len = 0;
    uint16_t          record_len;
    uint16_t          data_len;
    int8_t            pid = 0;
    va_list           args;
    int32_t          fmt_result;
    
    uint32_t        log_flag = LOG_RECORD_FLAG;
    char            log_buf[LOG_MSG_MAX_LEN] = {'\0'};
    char            time_buf[LOG_TIME_INFO_LEN] = {'\0'};
    char            cur_line_info[LOG_LINE_INFO_LEN] = { 0 };

    memset(log_buf, 0, LOG_MSG_MAX_LEN);

    log_buf[log_len++]  = MSG_HEAD_FLAG;
    log_len += 2;

    log_buf[log_len++]  = 1;
    log_buf[log_len++]  = LOG_DATA_MSG;
    log_buf[log_len++]  = pid;
    log_buf[log_len++]  = level;
    memcpy(log_buf+log_len, &log_flag, 4);
    log_len += 4;
    log_buf[log_len++]  = pid;
    log_len += 2;

    if (tag != NULL)
    {
        log_len += tlog_strcpy(log_len, log_buf + log_len, tag);
    }
    else
    {
        return RES_OK;
    }
    printf("log_len_line_90 = %d\n",log_len);

    log_len += tlog_strcpy(log_len, log_buf + log_len, "#");

    if (0 != strncmp(tag, TLOG_TAG_PARA, strlen(TLOG_TAG_PARA)))
    {
        // log_len += tlog_strcpy(log_len, log_buf + log_len, level_output_info[level]);
        log_len += tlog_strcpy(log_len, log_buf + log_len, "debug");
        log_len += tlog_strcpy(log_len, log_buf + log_len, " [");
        // tlog_get_time(time_buf, sizeof(time_buf));
        // log_len += tlog_strcpy(log_len, log_buf + log_len, time_buf);
        log_len += tlog_strcpy(log_len, log_buf + log_len, " ");
        log_len += tlog_strcpy(log_len, log_buf + log_len, func);
        log_len += tlog_strcpy(log_len, log_buf + log_len, ":");
        tlog_get_line_info(line, cur_line_info);
        log_len += tlog_strcpy(log_len, log_buf + log_len, cur_line_info);

        log_len += tlog_strcpy(log_len, log_buf + log_len, "] ");
    }

    /**
     * @brief vsnprintf
     * @param str
     * @param size
     * @param format
     * @param ap
     * @return
     * tips: if return value >= size, that means the final str has been truncated.
     * and the real copy len is the less value between size-1 and strlen(src_str)
     * int vsnprintf(char *str, size_t size, const char *format, va_list ap);
     */
    va_start(args, format);

    fmt_result = vsnprintf(log_buf + log_len,
                           LOG_MSG_MAX_LEN - log_len,
                           format, args);
    va_end(args);

    if ((fmt_result > -1) &&
        (fmt_result <= (int)(LOG_MSG_MAX_LEN - log_len - 4)))
    {
        log_len += fmt_result;
    }
    else
    {
        log_len = LOG_MSG_MAX_LEN - 4;
    }
    /* fill in the msg data length */
    data_len    = log_len - LOG_MSG_DATA_START_POS + 3; //为最后回车预留1
    memcpy(log_buf+LOG_MSG_DATA_LEN_POS, &data_len, sizeof(data_len));

    /* fill in the record length */
    record_len  = log_len - LOG_MSG_RECORD_START_POS + 3;//为最后回车预留1

    memcpy(log_buf+LOG_MSG_RECORD_LEN_POS, &record_len, sizeof(record_len));



    /* fill in the msg tailer */
    log_buf[log_len++]    = '\r';
    log_buf[log_len++]    = '\n';
    log_buf[log_len++]    = '\0';
    log_buf[log_len++]    = MSG_TAIL_FLAG;

    tlog_handle_log_msg(&log_buf[LOG_MSG_DATA_START_POS], log_len-5);

    /*do not FREE resources if enqueue succeeds*/
    return RES_OK;
}

//内部区分日志信息/报文信息
void tlog_handle_log_msg(char* data, uint32_t(len))
{
    uint32_t i = 0;
    log_msg_t msg;
    memset(&msg, 0, sizeof(log_msg_t));
    record_header_t *record_header;

    msg.header.type     = data[i++];
    msg.header.pid      = data[i++];
    msg.header.level    = data[i++];

    record_header = &(msg.record.header);
    memcpy(&(record_header->type), &data[i], sizeof(record_header->type));
    i += sizeof(record_header->type);

    record_header->pid = data[i++];

    memcpy(&(record_header->len), &data[i], sizeof(record_header->pid));
    i += sizeof(record_header->len);

    memcpy(msg.record.data, &data[i], (len - i));

    // if(msg.header.type == LOG_DATA_MSG)
    {
        tlog_handle_real_log(&msg);
    }
}

//函数作用：查找#号是否存在
static void tlog_handle_real_log(log_msg_t *msg)
{
    log_record_t        *record = &(msg->record);
    log_msg_header_t    *header = &(msg->header);
    char *substr;
    substr = strchr(record->data, '#');     //查找record->data中#第一次出现的位置，并返回其指针；失败则返回NULL
    if (substr == NULL)
    {
        printf("substr == NULL\n");
        return;
    }

    record_enqueue(header->level, record, false);   //在此只需要header的level值和record数据
}

static uint16_t tlog_fill_record_buf(log_record_t *record, uint8_t *buf)
{
    uint16_t len = 0;

    memcpy(&buf[len], record->data, record->header.len);
    len += record->header.len;

    return len;
}

#if 1
static void tlog_dequeue();

static void record_enqueue(int8_t type, log_record_t *record, bool force_sync)
{
    uint8_t buf[RECORD_MAX_LEN];
    uint16_t len = tlog_fill_record_buf(record, buf);   //将record的data数据拷贝到buf里
    uint8_t* formt_log = buf;

    printf("cur_in_record_enqueue_func\n");
    // if(type == LOG_DATA_MSG)
    {
        log_buf_t *cache = log_mgr.log_cache;

        if (cache->tail + len >= (cache->size * 1024))
        {
            return;
        }

        /* 报文长度大于cache size不存储 */
        if(len > (cache->size * 1024))
        {
            printf("return 1\n");
            fprintf(stderr, "record len > cache size, len=%u, cache->size =%u\n", len,cache->size * 1024);
            return;
        }

        memcpy(cache->buf+cache->tail, buf, len);
        cache->tail += len;

        if (force_sync == true)
        {
            return;
            tlog_dequeue();
        }
    }
}
#endif

/*MGR收到APP发来的日志信息后，不是立即写入NAND，而是先缓存到log_cache中，
 *此举是为了减少对NAND的擦写次数，延长NAND寿命，
 *log_cache即将存满；写入周期时间到；系统掉电forcesync
 *只有以上三个条件任何一个满足时，才会执行一次写操作，清空log_cache
 *UART/USB/NET/SD_CARD不需要log_cache延寿，直接写
 */

ssize_t plm_tlog_writen(int fd, const void *vptr, size_t n);

static void tlog_dequeue()
{
    TEXT file_path[MAX_FILE_PATH_LEN];
    log_buf_t *cache = log_mgr.log_cache;
    log_file_t *logfile = &(log_mgr.dev->log_file);
    int32_t ret;
#if CACHE_SYNC_DEBUG
    clock_t t;
    double time_taken;
#endif

    sprintf(file_path, "%s/terminal.log", "/mnt/e/1Code/my_code/linux_stu");

    int32_t detectFileLen = lcp_get_file_size(file_path);

    if(detectFileLen != -1 && logfile->len != (uint32_t)detectFileLen)
    {
        logfile->len = detectFileLen;
    }

    if(detectFileLen == -1)
    {
        close(logfile->fd);
        logfile->fd = open(file_path,  TLOG_FILE_OPEN_FLAGS, 0666);
        if(-1 == logfile->fd)
        {
            fprintf(stderr, "Open  %s failed. errno[%d]\n", file_path, errno);
            return;
        }
        logfile->len = 0;
    }

    if (logfile->len + cache->tail > (uint32_t)(logfile->size * 1024))
    {
        close(logfile->fd);
        // rotate(file_path, logfile->count);   //文件循环存储，先注释掉0913
        logfile->fd = open(file_path,  TLOG_FILE_OPEN_FLAGS, 0666);
        if(-1 == logfile->fd)
        {
            fprintf(stderr, "Open  %s failed. errno[%d]\n", file_path, errno);
            return;
        }
        logfile->len = 0;
    }

    ret = plm_tlog_writen(logfile->fd, cache->buf, cache->tail);
    if (ret >= 0)
    {
        logfile->len += cache->tail;
        cache->tail = 0;
        cache->flush_time = time(NULL);
        fsync(logfile->fd);
    }
}

ssize_t plm_tlog_writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            /*写的过程中遇到了中断，不认为是错误，重写即可*/
            if (nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;        /* and call write() again */
            }
            else
            {
                return (-1);         /* error */
            }
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return (n);
}
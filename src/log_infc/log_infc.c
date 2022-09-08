#include "log_infc.h"


void tlog_handle_log_msg(char* data, uint32_t(len));

task_callback_ret_t tlog_thread(task_callback_para_t arg)
{


    return NULL;
}

int32_t plm_tlog(const char* tag, uint32_t level, const char* func, uint64_t line, const char* format, ...)
{
    char log_buff[LOG_MSG_MAX_LEN] = {"hello, my name is daming"};
    
    tlog_handle_log_msg(log_buff, (uint32_t)strlen(log_buff));

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

    if(msg.header.type == LOG_DATA_MSG)
    {
        // tlog_handle_real_log(&msg);
    }
}



//tlog任务

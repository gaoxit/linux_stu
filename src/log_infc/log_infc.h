//log功能设计到存取写文件，需要用到file_infc中的一些功能

#ifndef __LOG_INFC__
#define __LOG_INFC__

#include <fcntl.h>
#include "log_export.h"


#define MAX_TAG_LEN             32
#define MAX_TAG_COUNT           16
#define LOG_QUEUE_MAX_LEN       1024

#define LOG_MSG_MAX_LEN         256
#define RECORD_DATA_MAX_LEN     250
// #define RECORD_MAX_LEN          2048
#define MAX_FILE_PATH_LEN       128

#define TLOG_FILE_OPEN_FLAGS    (O_WRONLY | O_APPEND | O_CREAT)

#define LOG_RECORD_FLAG             0x22222222
#define LOG_TIME_INFO_LEN               32
#define LOG_LINE_INFO_LEN               10

/*0xF9 + 数据域长度(2 bytes) + 控制码(1 bytes) + 数据域 + 0x6E*/
#define LOG_MSG_MAX_LEN             256
#define RECORD_MAX_LEN              (sizeof(log_record_t))


#define MSG_HEAD_FLAG (0xF9)
#define MSG_TAIL_FLAG (0x6E)
#define TLOG_MAX_LOG_LEVEL          6

#define TLOG_TAG_PARA               "PARA"

#define LOG_MSG_DATA_LEN_POS            1
#define LOG_MSG_DATA_START_POS          4
#define LOG_MSG_RECORD_LEN_POS          12
#define LOG_MSG_RECORD_START_POS        14

#define TLOG_DEFAULT_SYNC_PERIOD        1
#define TLOG_DEFAULT_CACHE_SIZE         1024



// const char  level_output_info[TLOG_MAX_LOG_LEVEL][6] =
// {
//     {"DEBUG"}, {"INFO"}, {"WARN"},
//     {"ERROR"}, {"FATAL"}, {"OFF"}
// };


typedef struct 
{
    int8_t          level;
    int8_t          port;
} log_property_t;

typedef struct 
{
    uint8_t*        buf;
    uint32_t        size;
    uint32_t        tail;
    int32_t         flush_period;
    time_t          flush_time;
} log_buf_t;

typedef struct 
{
    char name[MAX_TAG_LEN];
    int8_t      pid;
    int8_t      level;
} log_tag_t;

typedef struct 
{
    int32_t fd;
    uint32_t    len;
    uint16_t    size;
    uint16_t    count;
    int8_t*     level;
    char        file_name[MAX_TAG_LEN];
} log_file_t;

typedef struct 
{
    log_file_t          log_file;
    log_file_t          frame_file;
} log_dev_t;

typedef struct 
{
    char*               buf;
    uint32_t            len;
} log_raw_t;

typedef struct 
{
    mutex_lock_t        lock;
    mutex_cond_t        r_cond;
    mutex_cond_t        w_cond;
    int32_t             r_waiting;
    int32_t             w_waiting;
    int32_t             head;
    int32_t             tail;
    int32_t             used;
    const log_raw_t*    quene[LOG_QUEUE_MAX_LEN];
} log_msgq_t;


typedef struct 
{
    int8_t          pid;
    uint8_t         init_complete;
    int32_t         run_in_simulator;
    char*           log_path;
    log_property_t* property;
    log_buf_t*      log_cache;
    log_buf_t*      frame_cache;
    log_tag_t*      tags;
    log_dev_t*      dev;
    log_msgq_t*     qu;
} log_mgr_t;


typedef struct record_header_s
{
    uint32_t        type;
    int8_t          pid;
    uint16_t        len;
} record_header_t;

typedef struct
{
    record_header_t header;
    char            data[RECORD_DATA_MAX_LEN];
} log_record_t;

typedef struct
{
    int8_t          type;
    int8_t          pid;
    int8_t          level;
} log_msg_header_t;

typedef struct
{
    log_msg_header_t        header;
    log_record_t            record;
} log_msg_t;

typedef enum
{
    LOG_MGR_MSG = 0,
    LOG_DATA_MSG,
    LOG_FRM_MSG,
} app_msg_t;

/*0xF9 + 数据域长度(2 bytes) + 控制码(1 bytes) + 【数据域】 + 0x6E*/
//【数据域】：pid(1) + level(1) + log_flag(4) + pid(1) + 2bit + tag + # + 日志级别 + [...




#endif  //__LOG_INFC__
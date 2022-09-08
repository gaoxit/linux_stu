//log功能设计到存取写文件，需要用到file_infc中的一些功能

#ifndef __LOG_INFC__
#define __LOG_INFC__

#include "log_export.h"



#define MAX_TAG_LEN             32
#define MAX_TAG_COUNT           16
#define LOG_QUEUE_MAX_LEN       1024

#define LOG_MSG_MAX_LEN         256
#define RECORD_DATA_MAX_LEN         250



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
    uint16_t    conut;
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

#endif  //__LOG_INFC__
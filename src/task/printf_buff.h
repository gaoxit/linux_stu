#ifndef __BUFF_PRINTF__
#define __BUFF_PRINTF__

#include "task_export.h"

#define MAX_DEBUG_ITEM_TEXT_LEN1 2048
#define DEBU_MAX1 2048

typedef struct
{
    uint32_t time_tick[DEBU_MAX1];

    uint8_t text[DEBU_MAX1][MAX_DEBUG_ITEM_TEXT_LEN1];
    uint16_t len[DEBU_MAX1];

    uint16_t read_pos;
    uint16_t write_pos;
} debug_log_buf_t; //debug 缓存



void write_debug_buf(uint8_t *text, uint16_t len);
void report_debug_buf(void);

#endif
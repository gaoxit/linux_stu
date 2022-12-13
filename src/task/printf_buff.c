#include "printf_buff.h"



static debug_log_buf_t g_debug_vol_cur_buf;


void write_debug_buf(uint8_t *text, uint16_t len)
{
    if (len >= MAX_DEBUG_ITEM_TEXT_LEN1)
    {
        return;
    }

    strncpy((char *)g_debug_vol_cur_buf.text[g_debug_vol_cur_buf.write_pos], (char *)text, len);
    g_debug_vol_cur_buf.len[g_debug_vol_cur_buf.write_pos] = len;

    g_debug_vol_cur_buf.write_pos++;
    if (g_debug_vol_cur_buf.write_pos >= DEBU_MAX1)
    {
        g_debug_vol_cur_buf.write_pos = 0;
    }
}

// extern serial_port_t g_debug_port;
void report_debug_buf(void)
{
    while ((g_debug_vol_cur_buf.read_pos != g_debug_vol_cur_buf.write_pos) && (g_debug_vol_cur_buf.write_pos != DEBU_MAX1))
    {
        uint8_t buf[MAX_DEBUG_ITEM_TEXT_LEN1 + 2] = {0};

        uint8_t debug_len = g_debug_vol_cur_buf.len[g_debug_vol_cur_buf.read_pos];

        memcpy(buf, g_debug_vol_cur_buf.text[g_debug_vol_cur_buf.read_pos], debug_len);

        buf[sizeof(buf) - 1] = '\0';

        printf("buf=%s",buf);
        // // plm_tlog_warn(TLOG_TAG_PROTECT, "%s", buf);
        // serial_port_write_bytes(&g_debug_port, buf, 0, debug_len);
        // _trace_debug_protect("serial_port_write_bytes debug debug_len:%d", debug_len);

        g_debug_vol_cur_buf.read_pos++;
        if (g_debug_vol_cur_buf.read_pos >= DEBU_MAX1)
        {
            g_debug_vol_cur_buf.read_pos = 0;
        }
    }
    
}
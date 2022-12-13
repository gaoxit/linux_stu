#include "task_manage.h"
#include "hal_task.h"
#include "hal_timer.h"
#include "utils_export.h"
#include "log_export.h"
#include "printf_buff.h"

static debug_log_buf_t g_debug_vol_cur_buf;

#if 0
void write_debug_buf(uint8_t *text, uint16_t len)
{
    
    if (len >= MAX_DEBUG_ITEM_TEXT_LEN1)
    {
        return;
    }

    strncpy((char *)g_debug_vol_cur_buf.text[g_debug_vol_cur_buf.write_pos], (char *)text, len);
    // printf("text: %s\n",g_debug_vol_cur_buf.text[1]);
    // printf("len : %d\n",len);

    g_debug_vol_cur_buf.len[g_debug_vol_cur_buf.write_pos] = len;
    // printf("g_debug_vol_cur_buf.len[W %d]: %d\n",g_debug_vol_cur_buf.write_pos, g_debug_vol_cur_buf.len[g_debug_vol_cur_buf.write_pos]);

    g_debug_vol_cur_buf.write_pos++;
        // printf("write_pos1: %d\n",g_debug_vol_cur_buf.write_pos);

    if (g_debug_vol_cur_buf.write_pos >= DEBU_MAX1)
    {
        printf("write_pos2: %d\n",g_debug_vol_cur_buf.write_pos);
        g_debug_vol_cur_buf.write_pos = 0;
    }
}

// extern serial_port_t g_debug_port;
void report_debug_buf(void)
{
    while ((g_debug_vol_cur_buf.read_pos != g_debug_vol_cur_buf.write_pos) && (g_debug_vol_cur_buf.write_pos != DEBU_MAX1))
    {
        uint8_t buf[MAX_DEBUG_ITEM_TEXT_LEN1 + 2] = {0};

        // printf("R: %d W: %d\n",g_debug_vol_cur_buf.read_pos,g_debug_vol_cur_buf.write_pos);

        uint8_t debug_len = g_debug_vol_cur_buf.len[g_debug_vol_cur_buf.read_pos];     //原来这里是uint8_t，传进来的长度是1510，发生截断后的值是230，导致打印不全

        memcpy(buf, g_debug_vol_cur_buf.text[g_debug_vol_cur_buf.read_pos], debug_len);

        buf[sizeof(buf) - 1] = '\0';

        printf("debug_len = %d\n",debug_len);
        printf("buf = %s\n",buf);

        g_debug_vol_cur_buf.read_pos++;
        if (g_debug_vol_cur_buf.read_pos >= DEBU_MAX1)
        {
            g_debug_vol_cur_buf.read_pos = 0;
        }
    }
    
}
#endif





#define STACK_SIZE_1K 1024
#define STACK_SIZE_1M (STACK_SIZE_1K * 1024)
#define STACK_SIZE_2M (STACK_SIZE_1M * 2)


static uint8_t stack_task_cycle1[STACK_SIZE_2M];
static uint8_t stack_task_cycle2[STACK_SIZE_2M];
static uint8_t stack_task_tlog[STACK_SIZE_2M];



task_callback_ret_t task_cycle1(task_callback_para_t arg);
task_callback_ret_t task_cycle2(task_callback_para_t arg);


static task_t g_task_list[] = 
{
    {
        .task_id = APP_TASK_CYCLE1,
        .task_status = TASK_NOT_CREATE,
        .task_name = "cycle1",
        .task_stack = stack_task_cycle1,
        .task_priority = 0,
        .task_stack_size = sizeof(stack_task_cycle1),
        .task_callback = task_cycle1,
        .task_callback_para = 0,
        .task_init = NULL,
    },
    {
        .task_id = APP_TASK_CYCLE2,
        .task_status = TASK_NOT_CREATE,
        .task_name = "cycle2",
        .task_stack = stack_task_cycle2,
        .task_priority = 1,
        .task_stack_size = sizeof(stack_task_cycle2),
        .task_callback = task_cycle2,
        .task_callback_para = 0,
        .task_init = NULL,
    },
    {
        .task_id = APP_TASK_TLOG,
        .task_status = TASK_NOT_CREATE,
        .task_name = "tlog_thread",
        .task_stack = stack_task_tlog,
        .task_priority = 1,
        .task_stack_size = sizeof(stack_task_tlog),
        .task_callback = tlog_thread,
        .task_callback_para = 0,
        .task_init = NULL,
    },
};



task_callback_ret_t task_cycle1(task_callback_para_t arg)
{
    standard_time_t now_time;
    
    while(1)
    {

        hal_get_time_in_standard(&now_time);
#if 1
        printf("当前在task1任务中--当前时间是：20%d年%d月%d日 %d:%d:%d,%d\n",
        now_time.year,
        now_time.month,
        now_time.mday,
        now_time.hour,
        now_time.min,
        now_time.s,
        now_time.ms);
#endif

        //测试拼接打印
#define TEST_STR_COMBIME 1
#if TEST_STR_COMBIME

#define CLOSE_LOOP_DATA_NUM 10

static int16_t g_data_full_flag = false;
static int16_t cnt = 0;
        uint32_t tmp[10 * 1024] = {0};
        uint16_t pos = 0;

        int16_t voltage[2][CLOSE_LOOP_DATA_NUM] = {0};

        if(g_data_full_flag == false)
        {
            for(int i1 = 0; i1 < CLOSE_LOOP_DATA_NUM; i1++)
            {
                voltage[0][i1] = (cnt * 2 + 5);
                voltage[1][i1] = (cnt * 2 + 3);

                // printf("voltage_0_[%d] = %d\n", i1, voltage[0][i1]);         //单个打印，一次打印一个
                cnt +=3;
                if(i1 == (CLOSE_LOOP_DATA_NUM - 1))
                {
                    cnt = cnt + 5;
                    g_data_full_flag = true;
                }

            }
        }


#if 0

        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "index 0: ");

        for (uint16_t i = 0; i < CLOSE_LOOP_DATA_NUM; i++)
        {
            pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%d ", voltage[0][i]);
        }

        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "\r\nindex 1: ");
        for (uint16_t i = 0; i < CLOSE_LOOP_DATA_NUM; i++)
        {
            pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%d ", voltage[1][i]);
        }
#endif
        printf("--------if  begin--------\n");

        if(g_data_full_flag == true)
        {
            printf("--------voltage[0] begin--------\n");
            for (uint16_t i = 0; i < CLOSE_LOOP_DATA_NUM; i++)
            {
                printf("voltage_0_[%d] = %d\n", i, voltage[0][i]);         //单个打印，一次打印一个
                hal_sleep_in_ms(100);
            }
            printf("--------voltage[0] end--------\n");
#if 1
            printf("--------voltage[1] begin--------\n");
            for (uint16_t i = 0; i < CLOSE_LOOP_DATA_NUM; i++)
            {
                printf("voltage_1_[%d] = %d\n",i, voltage[1][i]);         //单个打印，一次打印一个
                hal_sleep_in_ms(100);
            }
            printf("--------voltage[1] end--------\n");
#endif
            g_data_full_flag = false;
        }


        // write_debug_buf(tmp, pos);





#endif
        plm_tlog_error("RUN_TIME", "heart beats: 20%d年%d月%d日 %d:%d:%d,%d", 
        now_time.year,
        now_time.month,
        now_time.mday,
        now_time.hour,
        now_time.min,
        now_time.s,
        now_time.ms);  

        // hal_sleep_in_s(1);
    }
}


task_callback_ret_t task_cycle2(task_callback_para_t arg)
{
    int num = 0;
    while(1)
    {
        // printf("当前在task2任务中....(%d)..\n", num++);
        // report_debug_buf();
        hal_sleep_in_s(2);
    }
}


void task_create(void)
{
    uint8_t ret = 0;
    for(uint8_t task_index = 0; task_index < COUNT(g_task_list); task_index++)
    {
        ret = hal_task_create(&g_task_list[task_index]);
        if(ret != 0)
        {
            printf("Create %s failure......\n", g_task_list[task_index].task_name);
        }
    }
}

// void task_start(void)
// {
//     uint8_t ret = 0;
//     for(uint8_t task_index = 0; task_index < COUNT(g_task_list); task_index++)
//     {
//         ret = hal_task_start(&g_task_list[task_index]);
//         if(ret != 0)
//         {
//             //
//         }
//     }
// }




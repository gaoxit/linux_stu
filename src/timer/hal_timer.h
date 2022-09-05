#ifndef __HAL_TIME__
#define __HAL_TIME__

#include <unistd.h>
#include <time.h>
#include <sys/time.h>   //gettimeofday等相关函数所需要包含的头文件
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef union
{
    uint8_t value[7];
    struct
    {
        uint8_t ms_low;  //毫秒-低字节
        uint8_t ms_high; //毫秒-高字节(2字节0~59999毫秒,即0.000~59.999秒)

        uint8_t min : 6;  //分钟(0~59)
        uint8_t res1 : 1; //保留
        uint8_t iv : 1;   //有效(=0)\无效(=1)

        uint8_t hour : 5; //小时(0~23)
        uint8_t res2 : 2; //保留
        uint8_t su : 1;   //标准时间(=0)\夏季时间(=1)

        uint8_t mday : 5; //日(1~31)
        uint8_t wday : 3; //星期(1~7)

        uint8_t month : 4; //月(1~12)
        uint8_t res3 : 4;  //保留

        uint8_t year : 7; //年(0~99,以2000年为基准计算年份)
        uint8_t res4 : 1; //保留
    };
} cp56time2a_t;

typedef struct
{
    uint8_t ms;         //毫秒
    uint8_t s;          //秒
    uint8_t min;        //分钟(0~59)
    uint8_t hour;       //小时(0~23)
    uint8_t mday;       //日(1~31)
    uint8_t wday;       //星期(1~7)
    uint8_t month;      //月(1~12)
    uint8_t year;       //年(0~99,以1900年为基准计算年份)
} standard_time_t;


void hal_sleep_in_s(uint32_t s);

/**
 * @brief 获取系统启动以来的相对时间，单位ms
 * @note
 * @param void
 * @return ms结果
 */
uint64_t hal_get_system_tick_in_ms(void);

/**
 * @brief 获取自UTC以来的绝对时间，单位ms
 * @note 自1970-01-01 00:00:00 +0000 (UTC)以来的秒数和微秒数。
 * @param void
 * @return ms结果
 */
uint64_t hal_get_time_in_ms(void);

/**
 * @brief 获取当前时间，以cp56格式显示
 * @note 
 * @param void
 * @return 1获取成功；0获取失败
 */
bool hal_get_time_in_cp56time2a(cp56time2a_t *cur_time_in_cp56time2a);

/**
 * @brief 获取当前时间，以年月日时分秒的格式显示
 * @note 
 * @param void
 * @return 1获取成功；0获取失败
 */
bool hal_get_time_in_standard(standard_time_t *standard_time);









#endif //__HAL_TIME__
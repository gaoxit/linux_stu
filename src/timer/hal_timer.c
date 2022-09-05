#include "hal_timer.h"

void hal_sleep_in_ms(uint32_t ms)
{
    usleep(ms * 1000);  //usleep是休眠us，因此休眠ms需要将传进去的参数乘1000
}

void hal_sleep_in_s(uint32_t s)
{
    usleep(s);
}

uint64_t hal_get_system_tick_in_ms(void)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    return(((uint64_t)now.tv_sec * 1000) + (now.tv_nsec / 1000000));
}

uint64_t hal_get_time_in_ms(void)
{
    struct timeval now;

    int ret = gettimeofday(&now, NULL);
    if(ret == 0)
    {
        return ((uint64_t)(now.tv_sec * 1000) + (now.tv_usec / 1000));
    }
    else
    {
        return 0;
    }
}

bool hal_get_time_in_cp56time2a(cp56time2a_t *cur_time_in_cp56time2a)
{
    if (cur_time_in_cp56time2a == NULL)
    {
        return false;
    }
    memset(cur_time_in_cp56time2a->value, 0x00, sizeof(cp56time2a_t));

    struct timeval time_in_timeval;
    int ret = gettimeofday(&time_in_timeval, NULL);
    if (ret == 0)
    {
        struct tm time_in_tm;
        localtime_r(&time_in_timeval.tv_sec, &time_in_tm);

        uint16_t ms_tmp = time_in_tm.tm_sec * 1000 + time_in_timeval.tv_usec / 1000;
        cur_time_in_cp56time2a->ms_low = ms_tmp % 0x100;
        cur_time_in_cp56time2a->ms_high = ms_tmp / 0x100;
        cur_time_in_cp56time2a->min = time_in_tm.tm_min;
        cur_time_in_cp56time2a->hour = time_in_tm.tm_hour;
        cur_time_in_cp56time2a->mday = time_in_tm.tm_mday;
        cur_time_in_cp56time2a->month = time_in_tm.tm_mon + 1;      //当前月份相对于1月份增加的月份。例如当前是9月份，其值是8，所以需要进行加1处理。
        cur_time_in_cp56time2a->year = time_in_tm.tm_year - 100;    //相对于1900年增加的年数。例如当前是2022年，tm_year的值为122，所以需要进行减100处理。
        cur_time_in_cp56time2a->wday = ((time_in_tm.tm_wday == 0) ? 7 : time_in_tm.tm_wday);

        return true;
    }
    else
    {
        return false;
    }
}


bool hal_get_time_in_standard(standard_time_t *standard_time)
{
    if (standard_time == NULL)
    {
        return false;
    }
    memset(standard_time, 0x00, sizeof(standard_time_t));

    struct timeval time_in_timeval;
    int ret = gettimeofday(&time_in_timeval, NULL);
    if (ret == 0)
    {
        struct tm time_in_tm;
        localtime_r(&time_in_timeval.tv_sec, &time_in_tm);

        // uint16_t ms_tmp = time_in_tm.tm_sec * 1000 + time_in_timeval.tv_usec / 1000;
        standard_time->ms = time_in_timeval.tv_usec / 1000;
        standard_time->s = time_in_tm.tm_sec;
        standard_time->min = time_in_tm.tm_min;
        standard_time->hour = time_in_tm.tm_hour;
        standard_time->mday = time_in_tm.tm_mday;
        standard_time->month = time_in_tm.tm_mon + 1;      //当前月份相对于1月份增加的月份。例如当前是9月份，其值是8，所以需要进行加1处理。
        standard_time->year = time_in_tm.tm_year - 100;    //相对于1900年增加的年数。例如当前是2022年，tm_year的值为122，所以需要进行减100处理。
        standard_time->wday = ((time_in_tm.tm_wday == 0) ? 7 : time_in_tm.tm_wday);

        return true;
    }
    else
    {
        return false;
    }
}
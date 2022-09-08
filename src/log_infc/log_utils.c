#include "log_utils.h"


int lcp_get_file_size(const char *path)
{
    int filesize  = -1;
    struct stat statbuff;

    if(stat(path, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }
    return filesize;
}


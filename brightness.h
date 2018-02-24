/*
 * This library assumes a linux system where the brightness files are located at
 * /sys/class/backligt/intel_backlight/
 * Here it expects to find the files brightness and max_brightness.
 */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static char* maxFileName = "/sys/class/backlight/intel_backlight/max_brightness";
static char* valFileName = "/sys/class/backlight/intel_backlight/brightness";

#define BUF_SIZE 4

/* Returns the current brightness value. Returns -1 on failure */
int
brt_val()
{
        int  fd;
        char buf[BUF_SIZE];

        if ( (fd = open(valFileName, O_RDONLY)) == -1 )
                return -1;

        if ( read(fd, (void*)buf, BUF_SIZE) == -1 )
                return -1;

        if ( close(fd) )
                return -1;

        return atoi(buf);
}

/* Returns the maximum brightness value. Returns -1 on failure */
int
brt_max()
{
        int  fd;
        char buf[BUF_SIZE];

        if ( (fd = open(maxFileName, O_RDONLY)) == -1 )
                return -1;

        if ( read(fd, (void*)buf, BUF_SIZE) == -1 )
                return -1;

        if ( close(fd) )
                return -1;

        return atoi(buf);
}

/* Sets the current brightness value to val. Returns 0 on success */
int
brt_set(int val)
{
        int  fd;
        char buf[BUF_SIZE];

        if ( snprintf(buf, BUF_SIZE, "%d", val) == -1 )
                return -1;

        if ( (fd = open(valFileName, O_WRONLY)) == -1 )
                return -1;

        if ( write(fd, buf, BUF_SIZE) == -1 )
                return -1;

        return close(fd);
}

/* Returns the current brightness as a percent ranging from 0.0 to 1.0.
   Return -1 on failure */
double
brt_valp()
{
        int val, max;

        val = brt_val();
        max = brt_max();

        return (double)val / (double)max;
}

/* Sets the current brightness value to the percentage value val.
   Returns 0 on success. */
int
brt_setp(double val)
{
        int  fd, ival, max;
        char buf[BUF_SIZE];

        max  = brt_max();
        ival = (int)((double)max * val);

        if ( snprintf(buf, BUF_SIZE, "%d", ival) == -1 )
                return -1;

        if ( (fd = open(valFileName, O_WRONLY)) == -1 )
                return -1;

        if ( write(fd, buf, BUF_SIZE) == -1 )
                return -1;

        return close(fd);
} /* brt_setp */

#undef BUF_SIZE

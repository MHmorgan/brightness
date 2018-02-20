#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char* maxFileName = "/sys/class/backlight/intel_backlight/max_brightness";
char* valFileName = "/sys/class/backlight/intel_backlight/brightness";

#define eq_dec(str) !strncmp(str, "dec", 3)
#define eq_inc(str) !strncmp(str, "inc", 3)

/* convenience function calling perror and exit */
void fatal(char*);

/* Printing msg if not NULL. Prints usage message with name if not NULL. */
void print_and_exit(char const* msg, char const* name);

/* Reads from file and expects it to contain only one number,
   which it returns. Return -1 if failed. */
int read_val(char const*);

/* Writes number to file. Returns 0 if successfull. */
int write_val(char const*, int);

int
main(int argc, char const* argv[])
{
        /* the actual minimum value is always 0. min_val is just a value chosen
           based on personal preferences. */
        int val, diff, max_val, min_val = 100;

        if ( geteuid() )
                print_and_exit("Must run as root!", NULL);
        if ( argc < 2 )
                print_and_exit(NULL, argv[0]);

        diff = ( argc >= 3 ) ? atoi(argv[2]) : 100;

        if ( eq_dec(argv[1]) )
                diff *= -1;
        else if ( !eq_inc(argv[1]) )
                print_and_exit("Unknown argument.", argv[0]);

        if ( (max_val = read_val(maxFileName)) == -1 )
                fatal("failed reading max brightness value");
        if ( (val = read_val(valFileName)) == -1 )
                fatal("failed reading brightness value");

        val += diff;
        if ( val < min_val ) val = min_val;
        if ( val > max_val ) val = max_val;

        printf("val %d diff %d min %d max %d\n", val, diff, min_val, max_val);

        if ( write_val(valFileName, val) )
                fatal("failed writing new brightness value");
        return 0;
} /* main */

#define BUF_SIZE 4

int
write_val(char const* fname, int val)
{
        int  fd;
        char buf[BUF_SIZE];

        if ( snprintf(buf, BUF_SIZE, "%d", val) == -1 )
                return -1;

        if ( (fd = open(fname, O_WRONLY)) == -1 )
                return -1;

        if ( write(fd, buf, BUF_SIZE) == -1 )
                return -1;

        return close(fd);
}

int
read_val(char const* fname)
{
        int  fd;
        char buf[BUF_SIZE];

        if ( (fd = open(fname, O_RDONLY)) == -1 )
                return -1;

        if ( read(fd, (void*)buf, BUF_SIZE) == -1 )
                return -1;

        if ( close(fd) )
                return -1;

        return atoi(buf);
}

#undef BUF_SIZE

void
print_and_exit(char const* msg, char const* name)
{
        if ( msg != NULL )
                printf("%s\n", msg);
        if ( name != NULL )
                printf("Usage\n\n\t%s <inc|dec> [value]\n\n", name);
        exit(0);
}

void
fatal(char* msg)
{
        perror(msg);
        exit(errno);
}
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEBUG

/* convenience function calling perror and exit */
void fatal(char*);

/* Printing msg if not NULL. Prints usage message with name if not NULL. */
void print_and_exit(char const* msg, char const* name);

/* Reads from file and expects it to contain only one number,
   which it returns. Return -1 if failed. */
int read_val(char const*);

/* Writes number to file. Returns 0 if successfull. */
int write_val(char const*, int);

char* maxFileName = "/sys/class/backlight/intel_backlight/max_brightness";
char* valFileName = "/sys/class/backlight/intel_backlight/brightness";

#define is_inc(str) !strncmp(str, "inc", 3)
#define is_dec(str) !strncmp(str, "dec", 3)
#define is_set(str) !strncmp(str, "set", 3)
#define is_val(str) !strncmp(str, "val", 3)
#define is_max(str) !strncmp(str, "max", 3)

int
main(int argc, char const* argv[])
{
        /* the actual minimum value is always 0. min_val is just a value chosen
           based on personal preferences. */
        int diff, val, max_val, min_val = 100;

        if ( geteuid() )
                print_and_exit("Must run as root!", NULL);
        if ( argc < 2 )
                print_and_exit(NULL, argv[0]);

        if ( (max_val = read_val(maxFileName)) == -1 )
                fatal("failed reading max brightness value");
        if ( (val = read_val(valFileName)) == -1 )
                fatal("failed reading brightness value");

        diff = ( argc >= 3 ) ? atoi(argv[2]) : 20 * (val / 100);

        if ( is_val(argv[1]) ) {
                printf("%d\n", val);
                exit(EXIT_SUCCESS);
        } else if ( is_max(argv[1]) ) {
                printf("%d\n", max_val);
                exit(EXIT_SUCCESS);
        } else if ( is_set(argv[1]) && argc >= 3 ) {
                val = atoi(argv[2]);
        } else if ( is_dec(argv[1]) ) {
                val -= diff;
        } else if ( is_inc(argv[1]) ) {
                val += diff;
        } else {
                print_and_exit("Invalid input.", argv[0]);
        }

        if ( val < min_val ) val = min_val;
        if ( val > max_val ) val = max_val;

        #ifdef DEBUG
        printf("diff %d val %d max_val %d min_val %d\n", diff, val, max_val, min_val);
        #endif

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
                printf("Usage\n\n"
                       "\t%s <inc|dec> [VALUE]\n"
                       "\t%s <val|max>\n"
                       "\t%s set VALUE\n"
                       "\n"
                       "Commands:\n\n"
                       "\tinc\tincrease brightness\n"
                       "\tdec\tdecrease brightness\n"
                       "\tset\tset brightness value\n"
                       "\tval\tprint current brightness value\n"
                       "\tmax\tprint max brightness value\n"
                       "\n", name, name, name);
        exit(0);
} /* print_and_exit */

void
fatal(char* msg)
{
        perror(msg);
        exit(errno);
}

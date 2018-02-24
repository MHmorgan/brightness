# Brightness
A program to increase and decrease brightness on linux.  

## Program
Try to use Xrandr or xbacklight etc. first if you just want to change the brightness.

###### To use without sudo:
```Bash
sudo chown root brightness
sudo chmod 4755 brightness
```
or
```Bash
make install
```

## C API

Include brightness.h to use these functions.

```C
/* Returns the current brightness value. Returns -1 on failure */
int brt_val();

/* Returns the maximum brightness value. Returns -1 on failure */
int brt_max();

/* Sets the current brightness value to val. Returns 0 on success */
int brt_set(int val);

/* Returns the current brightness as a percent ranging from 0.0 to 1.0.
   Return -1 on failure */
double brt_valp();

/* Sets the current brightness value to the percentage value val.
   Returns 0 on success. */
int brt_setp(double val);
```

## Changelog
###### v1.1
 - Default increase and decrease values vary with the current brightness level to make the changes smoother.

###### v2.0
 - Added header with C library

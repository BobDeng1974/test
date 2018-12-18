#ifndef CAMERATESTER__H__
#define CAMERATESTER__H__

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

#include "interact.h"

#include "water_mark.h"
#include "add_water.h"
#include "convert.h"

#define vfe_camera 0
#define vin_camera 1
#define usb_camera 2

#define ALL_TYPE 0
#define BMP_TYPE 1
#define YUV_TYPE 2

#define true 1
#define false 0

#define N_WIN_SIZES 20

#define ALIGN_16B(x) (((x) + (15)) & ~(15))

extern int camera_dbg_en;

//for internel debug
#define camera_dbg(x,arg...) do{ \
                                if(camera_dbg_en) \
                                    printf("[CAMERATESTER_DEBUG]"x,##arg); \
                            }while(0)

//print when error happens
#define camera_err(x,arg...) do{ \
                                printf("\033[1m\033[;31m[CAMERATESTER_ERR]"x"\033[0m",##arg); \
								fflush(stdout); \
                            }while(0)

#define camera_warn(x,arg...) printf("[CAMERATESTER_WARN]"x,##arg)
//print unconditional, for important info
#define camera_print(x,arg...) printf("[CAMERATESTER]"x,##arg)

#define camera_prompt(x,arg...) do{ \
                                printf("\033[1m\033[;32m[CAMERATESTER]"x"\033[0m",##arg); \
								fflush(stdout); \
                            }while(0)

struct buffer
{
    void *start[3];
    size_t length[3];
};

struct v4l2_frmsize
{
    unsigned int width;
    unsigned int height;
};

struct vfe_format
{
    unsigned char name[32];
    unsigned int fourcc;
	ConverFunc ToRGB24Func;
    struct v4l2_frmsize size[N_WIN_SIZES];
};

typedef struct camera_hal
{
    int videofd;
    int camera_type;
    int errorNum;
    int photo_num;
    char save_path[64];
    int nplanes;
    struct buffer *buffers;
    int buf_count;
    unsigned int win_width;
    unsigned int win_height;
    unsigned int pixelformat;
	ConverFunc ToRGB24;
    int use_wm;
    WaterMarkInfo WM_info;
}camera_handle;

#endif

#include "cameratester.h"

int camera_dbg_en = 0;

#define OtherTestItem   (0)

#define MAX_SKIP_NUM 5

static struct vfe_format fmt_fourcc[] = {
    {
        .name = "YUV422P",
        .fourcc = V4L2_PIX_FMT_YUV422P,
        .ToRGB24Func = YUV422PToRGB24,
    },
    {
        .name = "YUV420",
        .fourcc = V4L2_PIX_FMT_YUV420,
        .ToRGB24Func = YUV420ToRGB24,
    },
    {
        .name = "YVU420",
        .fourcc = V4L2_PIX_FMT_YVU420,
        .ToRGB24Func = YVU420ToRGB24,
    },
    {
        .name = "NV16",
        .fourcc = V4L2_PIX_FMT_NV16,
        .ToRGB24Func = NV16ToRGB24,
    },
    {
        .name = "NV12",
        .fourcc = V4L2_PIX_FMT_NV12,
        .ToRGB24Func = NV12ToRGB24,
    },
    {
        .name = "NV61",
        .fourcc = V4L2_PIX_FMT_NV61,
        .ToRGB24Func = NV61ToRGB24,
    },
    {
        .name = "NV21",
        .fourcc = V4L2_PIX_FMT_NV21,
        .ToRGB24Func = NV21ToRGB24,
    },
    {
        .name = "HM12",
        .fourcc = V4L2_PIX_FMT_HM12,
        .ToRGB24Func = NULL,
    },
    {
        .name = "YUYV",
        .fourcc = V4L2_PIX_FMT_YUYV,
        .ToRGB24Func = YUYVToRGB24,
    },
    {
        .name = "YVYU",
        .fourcc = V4L2_PIX_FMT_YVYU,
        .ToRGB24Func = YVYUToRGB24,
    },
    {
        .name = "UYVY",
        .fourcc = V4L2_PIX_FMT_UYVY,
        .ToRGB24Func = UYVYToRGB24,
    },
    {
        .name = "VYUY",
        .fourcc = V4L2_PIX_FMT_VYUY,
        .ToRGB24Func = VYUYToRGB24,
    },
    {
        .name = "BA81",
        .fourcc = V4L2_PIX_FMT_SBGGR8,
        .ToRGB24Func = NULL,
    },
    {
        .name = "GBRG",
        .fourcc = V4L2_PIX_FMT_SGBRG8,
        .ToRGB24Func = NULL,
    },
    {
        .name = "GRBG",
        .fourcc = V4L2_PIX_FMT_SGRBG8,
        .ToRGB24Func = NULL,
    },
    {
        .name = "RGGB",
        .fourcc = V4L2_PIX_FMT_SRGGB8,
        .ToRGB24Func = NULL,
    },
    {
        .name = "BG10",
        .fourcc = V4L2_PIX_FMT_SBGGR10,
        .ToRGB24Func = NULL,
    },
    {
        .name = "GB10",
        .fourcc = V4L2_PIX_FMT_SGBRG10,
        .ToRGB24Func = NULL,
    },
    {
        .name = "BA10",
        .fourcc = V4L2_PIX_FMT_SGRBG10,
        .ToRGB24Func = NULL,
    },
    {
        .name = "RG10",
        .fourcc = V4L2_PIX_FMT_SRGGB10,
        .ToRGB24Func = NULL,
    },
    {
        .name = "BG12",
        .fourcc = V4L2_PIX_FMT_SBGGR12,
        .ToRGB24Func = NULL,
    },
    {
        .name = "GB12",
        .fourcc = V4L2_PIX_FMT_SGBRG12,
        .ToRGB24Func = NULL,
    },
    {
        .name = "BA12",
        .fourcc = V4L2_PIX_FMT_SGRBG12,
        .ToRGB24Func = NULL,
    },
    {
        .name = "RG12",
        .fourcc = V4L2_PIX_FMT_SRGGB12,
        .ToRGB24Func = NULL,
    },
    {
        .name = "MJPEG",
        .fourcc = V4L2_PIX_FMT_MJPEG,
        .ToRGB24Func = NULL,
    },
    {
        .name = "H264",
        .fourcc = V4L2_PIX_FMT_H264,
        .ToRGB24Func = NULL,
    },
};

#define FOURCC_NUM  (sizeof(fmt_fourcc)/sizeof(fmt_fourcc[0]))

#define NUM_SUPPORT_FMT 30
static unsigned int formatNum = 0;
static struct vfe_format support_fmt[NUM_SUPPORT_FMT];
static struct vfe_format sensor_formats[NUM_SUPPORT_FMT];

char *get_format_name(unsigned int format)
{
    if(format == V4L2_PIX_FMT_YUV422P)
        return "YUV422P";
    else if(format == V4L2_PIX_FMT_YUV420)
        return "YUV420";
    else if(format == V4L2_PIX_FMT_YVU420)
        return "YVU420";
    else if(format == V4L2_PIX_FMT_NV16)
        return "NV16";
    else if(format == V4L2_PIX_FMT_NV12)
        return "NV12";
    else if(format == V4L2_PIX_FMT_NV61)
        return "NV61";
    else if(format == V4L2_PIX_FMT_NV21)
        return "NV21";
    else if(format == V4L2_PIX_FMT_HM12)
        return "MB YUV420";
    else if(format == V4L2_PIX_FMT_YUYV)
        return "YUYV";
    else if(format == V4L2_PIX_FMT_YVYU)
        return "YVYU";
    else if(format == V4L2_PIX_FMT_UYVY)
        return "UYVY";
    else if(format == V4L2_PIX_FMT_VYUY)
        return "VYUY";
    else if(format == V4L2_PIX_FMT_SBGGR8)
        return "BA81";
    else if(format == V4L2_PIX_FMT_SGBRG8)
        return "GBRG";
    else if(format == V4L2_PIX_FMT_SGRBG8)
        return "GRBG";
    else if(format == V4L2_PIX_FMT_SRGGB8)
        return "RGGB";
    else if(format == V4L2_PIX_FMT_SBGGR10)
        return "BG10";
    else if(format == V4L2_PIX_FMT_SGBRG10)
        return "GB10";
    else if(format == V4L2_PIX_FMT_SGRBG10)
        return "BA10";
    else if(format == V4L2_PIX_FMT_SRGGB10)
        return "RG10";
    else if(format == V4L2_PIX_FMT_SBGGR12)
        return "BG12";
    else if(format == V4L2_PIX_FMT_SGBRG12)
        return "GB12";
    else if(format == V4L2_PIX_FMT_SGRBG12)
        return "BA12";
    else if(format == V4L2_PIX_FMT_SRGGB12)
        return "RG12";
    else if(format == V4L2_PIX_FMT_MJPEG)
        return "MJPEG";
    else if(format == V4L2_PIX_FMT_H264)
        return "H264";
    else
        return NULL;
}

int return_format(char *short_name)
{
    if (strcmp(short_name, "YUV422P") == 0)
        return V4L2_PIX_FMT_YUV422P;
    else if (strcmp(short_name, "YUV420") == 0)
        return V4L2_PIX_FMT_YUV420;
    else if (strcmp(short_name, "YVU420") == 0)
        return V4L2_PIX_FMT_YVU420;
    else if (strcmp(short_name, "NV16") == 0)
        return V4L2_PIX_FMT_NV16;
    else if (strcmp(short_name, "NV12") == 0)
        return V4L2_PIX_FMT_NV12;
    else if (strcmp(short_name, "NV61") == 0)
        return V4L2_PIX_FMT_NV61;
    else if (strcmp(short_name, "NV21") == 0)
        return V4L2_PIX_FMT_NV21;
    else if (strcmp(short_name, "YUYV") == 0)
        return V4L2_PIX_FMT_YUYV;
    else if (strcmp(short_name, "YVYU") == 0)
        return V4L2_PIX_FMT_YVYU;
    else if (strcmp(short_name, "UYVY") == 0)
        return V4L2_PIX_FMT_UYVY;
    else if (strcmp(short_name, "VYUY") == 0)
        return V4L2_PIX_FMT_VYUY;
    else if (strcmp(short_name, "MJPEG") == 0)
        return V4L2_PIX_FMT_MJPEG;
    else if (strcmp(short_name, "H264") == 0)
        return V4L2_PIX_FMT_H264;
    else
        return -1;
}


static int save_frame_to_file(void *str,void *start,int length)
{
    FILE *fp = NULL;

    fp = fopen(str, "wb+"); //save more frames
    if(!fp){
        camera_err(" Open %s error\n", (char *)str);
        return -1;
    }

    if(fwrite(start, length, 1, fp)){
        fclose(fp);
        return 0;
    }else{
        camera_err(" Write file fail (%s)\n",strerror(errno));
        fclose(fp);
        return -1;
    }

    return 0;
}

static int detectformat(camera_handle *camera)
{
    int i;

    camera_print("**********************************************************\n");
    camera_print(" Using format parameters %s.\n", get_format_name(camera->pixelformat));
    for(i=0; sensor_formats[i].fourcc!=0; i++)
        if(camera->pixelformat == sensor_formats[i].fourcc)
            break;

    /* find support pixelformat */
    if(sensor_formats[i].fourcc == 0){
        camera_err(" sensor not support %s\n", get_format_name(camera->pixelformat));
        camera_print(" Use support for the first format and resolution\n");
        camera->pixelformat = sensor_formats[0].fourcc;
        camera->win_width = sensor_formats[0].size[0].width;
        camera->win_height = sensor_formats[0].size[0].height;

    }else{
        int j,sizeindex;

        for(j=0; sensor_formats[j].fourcc!=0; j++)
            if(camera->pixelformat == sensor_formats[j].fourcc)
                break;

        for(sizeindex=0;sensor_formats[j].size[sizeindex].width!=0; sizeindex++)
            if(camera->win_width == sensor_formats[j].size[sizeindex].width && camera->win_height == sensor_formats[j].size[sizeindex].height)
                break;

        if(sensor_formats[j].size[sizeindex].width == 0){
            camera_err(" sensor not support %u * %u\n", camera->win_width, camera->win_height);
            camera_print(" use support for the first resolution\n");

            if(sensor_formats[j].size[0].width != 0 && sensor_formats[j].size[0].height != 0){
                camera->win_width = sensor_formats[j].size[0].width;
                camera->win_height = sensor_formats[j].size[0].height;
            }else{
                /* mandatory settings */
                camera->win_width = 640;
                camera->win_height = 480;
            }
        }
    }

    /* find convert function */
    for(i=0; i<FOURCC_NUM; i++)
    {
        if(camera->pixelformat == fmt_fourcc[i].fourcc){
            camera->ToRGB24 = fmt_fourcc[i].ToRGB24Func;
            break;
        }
    }
    if(i >= FOURCC_NUM)
        camera->ToRGB24 = NULL;

    camera_print(" camera pixelformat %s\n",get_format_name(camera->pixelformat));
    camera_print(" Resolution size : %u * %u\n",camera->win_width,camera->win_height);
}

static int setformat(camera_handle *camera)
{
    struct v4l2_format fmt;

    /* set the data format */
    memset(&fmt, 0, sizeof(struct v4l2_format));

    if(camera->camera_type == vin_camera){
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        fmt.fmt.pix_mp.width = camera->win_width;
        fmt.fmt.pix_mp.height = camera->win_height;
        fmt.fmt.pix_mp.pixelformat = camera->pixelformat;
        fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;
    }else{
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = camera->win_width;
        fmt.fmt.pix.height = camera->win_height;
        fmt.fmt.pix.pixelformat = camera->pixelformat;
        fmt.fmt.pix.field = V4L2_FIELD_NONE;
    }

    if (ioctl(camera->videofd, VIDIOC_S_FMT, &fmt) < 0){
        camera_err(" setting the data format failed!\n");
        camera->errorNum++;
        return -1;
    }

    if(camera->camera_type == vin_camera){
        if(camera->win_width != fmt.fmt.pix_mp.width || camera->win_height != fmt.fmt.pix_mp.height)
            camera_err(" does not support %u * %u\n", camera->win_width, camera->win_height);

        camera->win_width = fmt.fmt.pix_mp.width;
        camera->win_height = fmt.fmt.pix_mp.height;
        camera_print(" VIDIOC_S_FMT succeed\n");
        camera_print(" fmt.type = %d\n",fmt.type);
        camera_print(" fmt.fmt.pix.width = %u\n",fmt.fmt.pix_mp.width);
        camera_print(" fmt.fmt.pix.height = %u\n",fmt.fmt.pix_mp.height);
        camera_print(" fmt.fmt.pix.pixelformat = %s\n",get_format_name(fmt.fmt.pix_mp.pixelformat));
        camera_print(" fmt.fmt.pix.field = %d\n",fmt.fmt.pix_mp.field);

        if (ioctl(camera->videofd, VIDIOC_G_FMT, &fmt) < 0){
            camera_err(" get the data format failed!\n");
            camera->errorNum++;
        }
        camera->nplanes = fmt.fmt.pix_mp.num_planes;
    }else{
        if(camera->win_width != fmt.fmt.pix.width || camera->win_height != fmt.fmt.pix.height)
            camera_err(" does not support %u * %u\n", camera->win_width, camera->win_height);

        camera->win_width = fmt.fmt.pix.width;
        camera->win_height = fmt.fmt.pix.height;
        camera_print(" VIDIOC_S_FMT succeed\n");
        camera_print(" fmt.type = %d\n",fmt.type);
        camera_print(" fmt.fmt.pix.width = %u\n",fmt.fmt.pix.width);
        camera_print(" fmt.fmt.pix.height = %u\n",fmt.fmt.pix.height);
        camera_print(" fmt.fmt.pix.pixelformat = %s\n",get_format_name(fmt.fmt.pix.pixelformat));
        camera_print(" fmt.fmt.pix.field = %d\n",fmt.fmt.pix.field);
    }

    return 0;

}

static int requestbuf (camera_handle *camera)
{
    int n_buffers;
    struct v4l2_requestbuffers req;  /* Initiate Memory Mapping or User Pointer I/O */
    struct v4l2_buffer buf;          /* Query the status of a buffer */

    /* Initiate Memory Mapping or User Pointer I/O */
    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    req.count = 3;
    if(camera->camera_type == vin_camera)
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if(ioctl(camera->videofd, VIDIOC_REQBUFS, &req) < 0){
        camera_err(" VIDIOC_REQBUFS failed\n");
        if(camera->errorNum > 0)
            camera_err(" camera demo error number:%d\n",camera->errorNum);
        return -1;
    }

    /* Query the status of a buffers */
    camera->buf_count = req.count;
    camera_dbg(" reqbuf number is %d\n",camera->buf_count);

    camera->buffers = calloc(req.count, sizeof(struct buffer));
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        if(camera->camera_type == vin_camera)
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        else
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        if(camera->camera_type == vin_camera){
            buf.length = camera->nplanes;
            buf.m.planes =  (struct v4l2_plane *)calloc(buf.length, sizeof(struct v4l2_plane));
        }

        if (ioctl(camera->videofd, VIDIOC_QUERYBUF, &buf) == -1) {
            camera_err(" VIDIOC_QUERYBUF error\n");
            if(camera->errorNum > 0)
                camera_err(" camera demo error number:%d\n",camera->errorNum);

            if(camera->camera_type == vin_camera)
                free(buf.m.planes);
            free(camera->buffers);
            return -1;
        }

        if(camera->camera_type == vin_camera){
            for(int i = 0; i < camera->nplanes; i++){
                camera->buffers[n_buffers].length[i] = buf.m.planes[i].length;
                camera->buffers[n_buffers].start[i] = mmap(NULL , buf.m.planes[i].length,
                                                   PROT_READ | PROT_WRITE, \
                                                   MAP_SHARED , camera->videofd, \
                                                   buf.m.planes[i].m.mem_offset);

                camera_dbg(" map buffer index: %d, mem: %p, len: %x, offset: %x\n",
                   n_buffers,camera->buffers[n_buffers].start[i],buf.m.planes[i].length,
                   buf.m.planes[i].m.mem_offset);
            }
            free(buf.m.planes);
        }else{
            camera->buffers[n_buffers].length[0] = buf.length;
            camera->buffers[n_buffers].start[0] = mmap(NULL , buf.length,
                                                PROT_READ | PROT_WRITE, \
                                                MAP_SHARED , camera->videofd, \
                                                buf.m.offset);

            camera_dbg(" map buffer index: %d, mem: %p, len: %x, offset: %x\n", \
                    n_buffers,camera->buffers[n_buffers].start[0],buf.length,buf.m.offset);
        }
    }

    /* Exchange a buffer with the driver */
    for(n_buffers = 0; n_buffers < req.count; n_buffers++) {
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        if(camera->camera_type == vin_camera)
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        else
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory= V4L2_MEMORY_MMAP;
        buf.index= n_buffers;
        if(camera->camera_type == vin_camera){
            buf.length = camera->nplanes;
            buf.m.planes =  (struct v4l2_plane *)calloc(buf.length, sizeof(struct v4l2_plane));
        }

        if (ioctl(camera->videofd, VIDIOC_QBUF, &buf) == -1) {
            camera_err(" VIDIOC_QBUF error\n");
            if(camera->errorNum > 0)
                camera_err(" camera demo error number:%d\n",camera->errorNum);

            if(camera->camera_type == vin_camera)
                free(buf.m.planes);
            free(camera->buffers);
            return -1;
        }

        if(camera->camera_type == vin_camera)
            free(buf.m.planes);
    }

    return 0;
}

static int releasebuf (camera_handle *camera)
{
    int i;

    /* munmap camera->buffers */
    /* munmap camera->buffers */
    if(camera->camera_type == vin_camera){
        for (int i = 0; i < camera->buf_count; ++i)
            for (int j = 0; j < camera->nplanes; j++)
                 munmap(camera->buffers[i].start[j], camera->buffers[i].length[j]);
    }else{
        for(int i=0; i<camera->buf_count; i++)
            munmap(camera->buffers[i].start[0],camera->buffers[i].length[0]);
    }

    /* free camera->buffers */
    free(camera->buffers);
}

static int capture_photo(camera_handle *camera, char *savename)
{
    enum v4l2_buf_type type;
    struct timeval tv;
    fd_set fds;
    struct v4l2_buffer buf;
    int ret = 0;
    int np = 0;
    int skip = 0;
    long long timestamp_now, timestamp_save;
    int i;
    char source_data_path[64];
    char bmp_data_path[64];
    FILE *fp = NULL;
    int width,height;

    /* streamon */
    if(camera->camera_type == vin_camera)
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(camera->videofd, VIDIOC_STREAMON, &type) == -1) {
        camera_err(" VIDIOC_STREAMON error! %s\n",strerror(errno));
        if(camera->errorNum > 0)
            camera_err(" camera demo error number:%d\n",camera->errorNum);
        return -1;
    }else{
        camera_print(" stream on succeed\n");
    }

    FD_ZERO(&fds);
    FD_SET(camera->videofd, &fds);

    memset(&buf, 0, sizeof(struct v4l2_buffer));
    if(camera->camera_type == vin_camera)
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if(camera->camera_type == vin_camera){
        buf.length = camera->nplanes;
        buf.m.planes = (struct v4l2_plane *)calloc(camera->nplanes, sizeof(struct v4l2_plane));
    }

    while (np < camera->photo_num)
    {
        camera_print(" capture num is [%d]\n",np);
        /* wait for sensor capture data */
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        ret = select(camera->videofd+1,&fds,NULL,NULL,&tv);
        if (ret == -1){
            camera_err(" select error\n");
            continue;
        }else if (ret == 0){
            camera_err(" select timeout,end capture thread!\n");
            if(camera->errorNum > 0)
                camera_err(" camera demo error number:%d\n",camera->errorNum);
            ret = -1;
            goto STREAMOFF;
        }

        /* dqbuf */
        ret = ioctl(camera->videofd, VIDIOC_DQBUF, &buf);
        if (ret != 0){
            camera_err("****DQBUF FAIL*****\n");
            ret = -1;
            goto STREAMOFF;
        }

        if(skip >= MAX_SKIP_NUM){
            timestamp_now = buf.timestamp.tv_sec*1000 + buf.timestamp.tv_usec/1000;
            if (np == 0)
                timestamp_save = timestamp_now;

            /* save photos at intervals */
            if((skip - MAX_SKIP_NUM)%2 == 0){
                camera_dbg(" the interval of two frames is %lld ms\n", timestamp_now - timestamp_save);
                timestamp_save = timestamp_now;

                /* check the data buf for byte alignment */
                if(camera->pixelformat == V4L2_PIX_FMT_YUV422P || camera->pixelformat == V4L2_PIX_FMT_YUYV
                    || camera->pixelformat == V4L2_PIX_FMT_YVYU || camera->pixelformat == V4L2_PIX_FMT_UYVY
                    || camera->pixelformat == V4L2_PIX_FMT_VYUY){
                    if(camera->win_width*camera->win_height*2 < buf.bytesused){
                        width = ALIGN_16B(camera->win_width);
                        height = ALIGN_16B(camera->win_height);
                    }else{
                        width = camera->win_width;
                        height = camera->win_height;
                    }
                }else if(camera->pixelformat == V4L2_PIX_FMT_YUV420 || camera->pixelformat == V4L2_PIX_FMT_YVU420
                    || camera->pixelformat == V4L2_PIX_FMT_NV16 || camera->pixelformat == V4L2_PIX_FMT_NV61
                    || camera->pixelformat == V4L2_PIX_FMT_NV12 || camera->pixelformat == V4L2_PIX_FMT_NV21){
                    if(camera->win_width*camera->win_height*1.5 < buf.bytesused){
                        width = ALIGN_16B(camera->win_width);
                        height = ALIGN_16B(camera->win_height);
                    }else{
                        width = camera->win_width;
                        height = camera->win_height;
                    }
                }else{
                    width = camera->win_width;
                    height = camera->win_height;
                }

                if(camera->pixelformat != V4L2_PIX_FMT_MJPEG && camera->pixelformat != V4L2_PIX_FMT_H264){
                    /* add water mark */
                    if(camera->use_wm)
                        AddWM(&camera->WM_info, width, height, camera->buffers[buf.index].start[0],
                                camera->buffers[buf.index].start[0]+width*height, camera->win_width-470, camera->win_height-40, NULL);

                    sprintf(bmp_data_path,"%s/%s%s%s_%d%s",camera->save_path, "bmp_", savename, get_format_name(camera->pixelformat), np+1,".bmp");
                    YUVToBMP(bmp_data_path,camera->buffers[buf.index].start[0], camera->ToRGB24, width, height);
                }else{ /* MJPEG or H264 */
                    sprintf(source_data_path, "%s/%s%d.%s", camera->save_path, "source_data", np+1, get_format_name(camera->pixelformat));
                    save_frame_to_file(source_data_path, camera->buffers[buf.index].start[0], camera->buffers[buf.index].length[0]);
                }

                camera_print(" The path to data saving is %s.\n", bmp_data_path);

                np++;
            }
        }

        /* qbuf */
        if (ioctl(camera->videofd, VIDIOC_QBUF, &buf) != 0){
            camera_err("*****QBUF FAIL*****\n");
            ret = -1;
            goto STREAMOFF;
        }

        skip++;
    }

STREAMOFF:
    if(camera->camera_type == vin_camera)
        free(buf.m.planes);

    /* streamoff */
    if(camera->camera_type == vin_camera)
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(camera->videofd, VIDIOC_STREAMOFF, &type) == -1) {
        camera_err(" VIDIOC_STREAMOFF error! %s\n",strerror(errno));
        if(camera->errorNum > 0)
            camera_err(" camera demo error number:%d\n",camera->errorNum);
        return -1;
    }

    return ret;
}

static int watermarkthread(camera_handle *camera)
{
    int ret = -1;
    char *photoname = "water";

	camera_prompt("<<<<<<<< camera add water mark test >>>>>>>>\n");

    if(WMInit(&camera->WM_info, "/etc/res/wm_540p_") < 0){
        camera_err(" water mark init error\n");
        camera->errorNum++;
    }

    camera->use_wm = 1;
    camera->photo_num = 1;
    camera->win_width = 640;
    camera->win_height = 480;
    camera->pixelformat = V4L2_PIX_FMT_NV21;

	detectformat(camera);
    setformat(camera);
    ret = requestbuf(camera);
    if(ret < 0){
        camera_err(" camera add water mark request buf error\n");
        return -1;
    }

    ret = capture_photo(camera, photoname);
    if(ret < 0)
        camera_err(" add water mark thread capture thread failed\n");

    releasebuf(camera);

    camera->use_wm = 0;
    camera->photo_num = 0;

    return ret;
}

static int shootthread(camera_handle *camera)
{
    int ret = -1;
    char *photoname = "shoot";

	camera_prompt("<<<<<<<< camera shoot test >>>>>>>>\n");

    camera->use_wm = 0;
    camera->photo_num = 2;
    camera->win_width = 640;
    camera->win_height = 480;
    camera->pixelformat = V4L2_PIX_FMT_NV21;

	detectformat(camera);
    setformat(camera);
    ret = requestbuf(camera);
    if(ret < 0){
        camera_err(" camera shoot request buf error\n");
        return -1;
    }

    ret = capture_photo(camera, photoname);
    if(ret < 0)
        camera_err(" shoot thread capture thread failed\n");

    releasebuf(camera);

    camera->use_wm = 0;
    camera->photo_num = 0;

    return ret;
}

/* support maximum and minimum resolution */
static int resolutionthread(camera_handle *camera)
{
    int ret = -1;
    int format_index = 0, size_index = 0;
    int min_index = 0, max_index = 0;
    char photoname[64];

    camera->use_wm = 0;
    camera->photo_num = 1;

	camera_prompt("<<<<<<<< camera resolution test >>>>>>>>\n");

    while(format_index < formatNum){
        if(sensor_formats[format_index].fourcc == camera->pixelformat)
            break;

        format_index++;
    }
    if(format_index >= formatNum){
        camera_err(" format that is not supported\n");
        camera->pixelformat = sensor_formats[0].fourcc;
    }

    while(sensor_formats[format_index].size[size_index].width != 0){
        if(sensor_formats[format_index].size[min_index].width >= sensor_formats[format_index].size[size_index].width
            || sensor_formats[format_index].size[min_index].height >= sensor_formats[format_index].size[size_index].height){
            min_index = size_index;
        }

        if(sensor_formats[format_index].size[max_index].width <= sensor_formats[format_index].size[size_index].width
            || sensor_formats[format_index].size[max_index].height <= sensor_formats[format_index].size[size_index].height){
            max_index = size_index;
        }

        size_index++;
    }

    /* min */
    camera->win_width = sensor_formats[format_index].size[min_index].width;
    camera->win_height = sensor_formats[format_index].size[min_index].height;
    memset(photoname, 0, sizeof(photoname));
    sprintf(photoname, "size%u_%u", camera->win_width, camera->win_height);
	camera_print(" camera minimum resolution test\n");

	detectformat(camera);
    setformat(camera);
    ret = requestbuf(camera);
    if(ret < 0){
        camera_err(" camera minimum resolution request buf error\n");
        goto MAXTEST;
    }

    ret = capture_photo(camera, photoname);
    if(ret < 0)
        camera_err(" minimum resolution thread capture thread failed\n");

    releasebuf(camera);

    /* max */
MAXTEST:
	if(min_index == max_index){
		camera_prompt(" the maximum minimum resolution is the same\n");
		return ret;
	}

    camera->win_width = sensor_formats[format_index].size[max_index].width;
    camera->win_height = sensor_formats[format_index].size[max_index].height;
    memset(photoname, 0, sizeof(photoname));
    sprintf(photoname, "size%u_%u", camera->win_width, camera->win_height);
	camera_print(" camera maximum resolution test\n");

	detectformat(camera);
    setformat(camera);
    ret = requestbuf(camera);
    if(ret < 0){
        camera_err(" camera maximum resolution request buf error\n");
        return -1;
    }

    ret = capture_photo(camera, photoname);
    if(ret < 0)
        camera_err(" maximum resolution thread capture thread failed\n");

    releasebuf(camera);

    camera->use_wm = 0;
    camera->photo_num = 0;

    return ret;
}

int main(int argc,char *argv[])
{
    camera_handle camera;
    struct v4l2_capability cap;      /* Query device capabilities */
    struct v4l2_fmtdesc fmtdesc;     /* Enumerate image formats */
    struct v4l2_frmsizeenum frmsize; /* Enumerate frame sizes */
    struct v4l2_format fmt;          /* try a format */
    struct v4l2_input inp;           /* select the current video input */
    struct v4l2_streamparm parms;    /* set streaming parameters */
    char bmp_data_path[64];
    int ret = -1;
	int results = 0;

    int index = 0;

    /* default settings */
    memset(&camera, 0, sizeof(camera_handle));
    camera.errorNum = 0;
    camera.win_width = 640;
    camera.win_height = 480;
    camera.photo_num = 1;
    camera.pixelformat = V4L2_PIX_FMT_NV21;
    memcpy(camera.save_path,"/tmp",sizeof("/tmp"));

    camera_print("**********************************************************\n");
    camera_print("*                                                        *\n");
    camera_print("*              this is camera test.                      *\n");
    camera_print("*                                                        *\n");
    camera_print("**********************************************************\n");

    //memset(camera.save_path, 0, sizeof(camera.save_path));
    //task("please enter the path to save the photo, for example:/mnt/SDCARD",
    //    camera.save_path, sizeof(camera.save_path));

    /* 1.open /dev/video0 node */
    camera_print("**********************************************************\n");
    camera_print(" open /dev/voide0!\n");
    camera_print("**********************************************************\n");

    camera.videofd = open("/dev/video0", O_RDWR, 0);
    if(camera.videofd < 0){
        camera_err(" open /dev/video0 fail!!!\n");

        return -1;
    }

    /* 2.Query device capabilities */
    if(ioctl(camera.videofd,VIDIOC_QUERYCAP,&cap) < 0){
        camera_err(" Query device capabilities fail!!!\n");

        camera.errorNum++;
    }else{
        camera_dbg(" Querey device capabilities succeed\n");
        camera_dbg(" cap.driver=%s\n",cap.driver);
        camera_dbg(" cap.card=%s\n",cap.card);
        camera_dbg(" cap.bus_info=%s\n",cap.bus_info);
        camera_dbg(" cap.version=%d\n",cap.version);
        camera_dbg(" cap.capabilities=%d\n",cap.capabilities);
    }

    if(strcmp(cap.driver,"sunxi-vfe") == 0){
        camera.camera_type = vfe_camera;
    }else if(strcmp(cap.driver,"sunxi-vin") == 0){
            camera.camera_type = vin_camera;
    }else if(strcmp(cap.driver,"uvcvideo") == 0){
        camera.camera_type = usb_camera;
    }else{
        camera_err(" unknow camera type is %s\n",cap.driver);
        camera.errorNum++;
    }

    if((cap.capabilities & (V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_VIDEO_CAPTURE_MPLANE)) <= 0){
        camera_err(" The device is not supports the Video Capture interface!!!\n");
        camera.errorNum++;
    }

    camera_print("**********************************************************\n");
    camera_print(" The path to data saving is %s.\n",camera.save_path);

    /* 3.select the current video input */
    inp.index = 0;
    inp.type = V4L2_INPUT_TYPE_CAMERA;
    if(ioctl(camera.videofd,VIDIOC_S_INPUT,&inp) < 0){
        camera_err(" VIDIOC_S_INPUT failed! s_input: %d\n",inp.index);
        camera.errorNum++;
    }

    /* 4.Enumerate image formats */
    camera_dbg("**********************************************************\n");
    camera_dbg(" enumerate image formats\n");
    memset(support_fmt, 0, sizeof(support_fmt));
    fmtdesc.index = 0;
    if(camera.camera_type == vin_camera)
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    index = 0;
    while(ioctl(camera.videofd,VIDIOC_ENUM_FMT,&fmtdesc) == 0)
    {
        /* save image formats */
        if(get_format_name(fmtdesc.pixelformat) != NULL){
            memcpy(support_fmt[index].name, fmtdesc.description, 32);
            support_fmt[index].fourcc = fmtdesc.pixelformat;
            camera_dbg(" format index = %d, name = %s\n", index, get_format_name(fmtdesc.pixelformat));
            index++;
        }
        fmtdesc.index++;
    }

    /* 5.try all format,here resolution is not important */
    int enumFmtIndex = 0;
    formatNum = 0;
    memset(&fmt,0,sizeof(fmt));
    if(camera.camera_type == vin_camera)
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = camera.win_width;
    fmt.fmt.pix.height = camera.win_height;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    memset(sensor_formats,0,sizeof(sensor_formats));

    camera_dbg("*********************************************************\n");
    camera_dbg(" The sensor supports the following formats :\n");
    while(support_fmt[enumFmtIndex].fourcc != 0)
    {
        fmt.fmt.pix.pixelformat = support_fmt[enumFmtIndex].fourcc;

        if(ioctl(camera.videofd,VIDIOC_TRY_FMT,&fmt) == 0)
        {
            sensor_formats[formatNum].fourcc = fmt.fmt.pix.pixelformat;
            camera_dbg(" Index %u : %s.\n",formatNum,get_format_name(sensor_formats[formatNum].fourcc));
            formatNum++;
        }
        enumFmtIndex++;
    }

    /* 6.Enumerate frame sizes */
    enumFmtIndex = 0;

    while(sensor_formats[enumFmtIndex].fourcc != 0)
    {
        memset(&frmsize,0,sizeof(frmsize));
        frmsize.index = 0;
        frmsize.pixel_format = sensor_formats[enumFmtIndex].fourcc;
        camera_dbg("**********************************************************\n");
        camera_dbg(" The %s supports the following resolutions:\n",get_format_name(sensor_formats[enumFmtIndex].fourcc));
        while(ioctl(camera.videofd,VIDIOC_ENUM_FRAMESIZES,&frmsize) == 0)
        {
            if(camera.camera_type == vin_camera){
                sensor_formats[enumFmtIndex].size[frmsize.index].width = frmsize.stepwise.max_width;
                sensor_formats[enumFmtIndex].size[frmsize.index].height = frmsize.stepwise.max_height;

                camera_dbg(" Index %d : %u * %u\n",frmsize.index,frmsize.stepwise.max_width,frmsize.stepwise.max_height);
            }else{
                sensor_formats[enumFmtIndex].size[frmsize.index].width = frmsize.discrete.width;
                sensor_formats[enumFmtIndex].size[frmsize.index].height = frmsize.discrete.height;

                camera_dbg(" Index %d : %u * %u\n",frmsize.index,frmsize.discrete.width,frmsize.discrete.height);
            }

            frmsize.index++;
            if(frmsize.index > N_WIN_SIZES-1)
            {
                camera_err(" Beyond the maximum queryable range, please modify N_WIN_SIZES.\n");
                break;
            }
        }

        /* next frame */
        enumFmtIndex++;
    }

    /* 7.set streaming parameters */
    if(camera.camera_type == vin_camera)
        parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parms.parm.capture.timeperframe.numerator = 1;
    parms.parm.capture.timeperframe.denominator = 30;
    if(ioctl(camera.videofd,VIDIOC_S_PARM,&parms) < 0){
        camera_err(" Setting streaming parameters failed!\n");
        camera.errorNum++;
    }

    /* 8.get streaming parameters */
    memset(&parms,0,sizeof(struct v4l2_streamparm));
    if(camera.camera_type == vin_camera)
        parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    else
        parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(camera.videofd,VIDIOC_G_PARM,&parms) < 0){
        camera_err(" Get streaming parameters failed!\n");
    }else{
        camera_print(" Camera capture framerate is %u/%u\n",
                parms.parm.capture.timeperframe.denominator, \
                parms.parm.capture.timeperframe.numerator);
    }

    /* capture thread */
    ret = watermarkthread(&camera);
    if(ret < 0){
        camera_err(" camera add water mark thread error\n");
        results--;
    }

#if OtherTestItem
    ret = shootthread(&camera);
    if(ret < 0){
        camera_err(" camera shoot thread error\n");
        results--;
    }

    ret = resolutionthread(&camera);
    if(ret < 0){
        camera_err(" camera change resolution thread error\n");
        results--;
    }
#endif

    /* image upload check */
    memset(bmp_data_path, 0, sizeof(bmp_data_path));
    sprintf(bmp_data_path, "%s/%s", camera.save_path, "bmp_water");
    ret = tshowimg(bmp_data_path, "Named bmp_water*.bmp The picture was watermarked?");
    if(ret == 1){
        camera_err(" camera add water mark error\n");
        results--;
    }else
        camera_print(" camera add water mark ok\n");

#if OtherTestItem
    memset(bmp_data_path, 0, sizeof(bmp_data_path));
    sprintf(bmp_data_path, "%s/%s", camera.save_path, "bmp_shoot");
    ret = tshowimg(bmp_data_path, "Named bmp_shoot*.bmp image is a continuous shooting?");
    if(ret == 1){
        camera_err(" camera shoot error\n");
        results--;
    }else
        camera_print(" camera shoot ok\n");

    memset(bmp_data_path, 0, sizeof(bmp_data_path));
    sprintf(bmp_data_path, "%s/%s", camera.save_path, "bmp_size");
    ret = tshowimg(bmp_data_path, "Named bmp_width_height.bmp Is the image shown clearly and correctly?");
    if(ret == 1){
        camera_err(" camera change resolution error\n");
        results--;
    }else
        camera_print(" camera change resolution ok\n");
#endif

	close(camera.videofd);
    return results;
}

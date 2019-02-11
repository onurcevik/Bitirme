#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <memory>


struct buffer {
      void   *data;
      size_t  size;
};

struct RGBImage {
      unsigned char   *data; // RGB888 <=> RGB24
      size_t          width;
      size_t          height;
      size_t          size; // width * height * 3
};

class Camera
{
public:
    Camera(const std::string& device = "/dev/video0",
           int width = 640,
           int height = 480);
    ~Camera();
    const RGBImage& frame(int timeout = 1);
private:
    void init_mmap();

    void open_device();
    void close_device();

    void init_device();
    void uninit_device();

    void start_capturing();
    void stop_capturing();

    bool read_frame();

    std::string device;
    int fd;

    RGBImage rgb_frame;

    struct buffer          *buffers;
    unsigned int     n_buffers;

    size_t xres, yres;
    size_t stride;

    bool force_format = true;
};

#endif // CAMERA_H

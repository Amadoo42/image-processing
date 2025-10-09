#pragma once
#include "Filter.h"
#include "ResizeFilter.h"

class FrameFilter : public Filter {
private:
    Image frame_image;
public:
    FrameFilter(const Image &frame_img) : frame_image(frame_img) {}

    void apply(Image &image) override {
        Image frame_image_resized = frame_image;
        ResizeFilter resizeFrame(image.width, image.height);
        resizeFrame.apply(frame_image_resized);
        int border_size = min(image.width, image.height) / 20;
        Image newImage = frame_image_resized;
        int inner_width = image.width - 2 * border_size;
        int inner_height = image.height - 2 * border_size;
        ResizeFilter resize(inner_width, inner_height);
        resize.apply(image);
        for(int x = 0; x < inner_width; ++x) {
            for(int y = 0; y < inner_height; ++y) {
                newImage(x + border_size, y + border_size, 0) = image(x, y, 0);
                newImage(x + border_size, y + border_size, 1) = image(x, y, 1);
                newImage(x + border_size, y + border_size, 2) = image(x, y, 2);
            }
        }
        image = newImage;
    }      
};
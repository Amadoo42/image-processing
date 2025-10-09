#pragma once
#include "Image_Class.h"

class Filter {
    public:
        void blackandwhite(Image &image);
        void darken(Image &image);
        void lighten(Image &image);
        void frame(Image &image, const Image &frame_image);
        void gray(Image &image);
        void h_flip(Image &image);
        void v_flip(Image &image);
        void invert(Image &image);
        void merge(Image &image, Image &merge_image);
        void resize(Image &image, int newWidth, int newHeight);
        void resize(Image &image, double ratioX, double ratioY);
        void rotate(Image &image, int degrees);
        void blur(Image &image, int kernelSize = 7, double sigma = 1.0);
        void crop(Image &image, int x, int y, int w, int h);
        void edge(Image &image);
        void purple(Image &image);
        void infrared(Image &image);
        void wave(Image &image);
};
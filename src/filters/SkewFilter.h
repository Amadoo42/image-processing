#pragma once
#include "Filter.h"

class SkewFilter : public Filter {
private:
    double k = 0.0;

    double bilinearInterpolate(const Image &image, double x, double y, int channel) {
        int x1 = x;
        int y1 = y;
        int x2 = min(x1 + 1, image.width - 1);
        int y2 = min(y1 + 1, image.height - 1);

        double dx = x - x1;
        double dy = y - y1;

        return (1 - dx) * (1 - dy) * image(x1, y1, channel)
               + dx * (1 - dy) * image(x2, y1, channel)
               + (1 - dx) * dy * image(x1, y2, channel)
               + dx * dy * image(x2, y2, channel);
    }

public:    
    SkewFilter(double deg) {
        k = tan(deg * M_PI / 180.0);
    }

    void apply(Image &image) override {
        int W = image.width;
        int H = image.height;
        int extra = fabs(k) * H;
        int newWidth = W + extra;
        int newHeight = H;
        Image newImage(newWidth, newHeight);

        int xOffset = (k < 0) ? extra : 0;

        for(int i = 0; i < newWidth; ++i) {
            for(int j = 0; j < newHeight; ++j) {
                for(int c = 0; c < 3; ++c) newImage(i, j, c) = 0; 
            }
        }

        for(int x = 0; x < newWidth; ++x) {
            for(int y = 0; y < newHeight; ++y) {
                double srcX = (x - xOffset) - k * (H - y);
                double srcY = y;

                if(srcX < 0 || srcX >= W || srcY < 0 || srcY >= H) continue;

                double r = bilinearInterpolate(image, srcX, srcY, 0);
                double g = bilinearInterpolate(image, srcX, srcY, 1);
                double b = bilinearInterpolate(image, srcX, srcY, 2);

                newImage(x, y, 0) = r;
                newImage(x, y, 1) = g;
                newImage(x, y, 2) = b;
            }
        }

        image = newImage;
    }      
};
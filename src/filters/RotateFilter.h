#pragma once
#include "Filter.h"

class RotateFilter : public Filter {
private:
    int degrees;

public:
    RotateFilter(int deg) : degrees(deg) {}

    void apply(Image &image) override {
        int numRotations = degrees / 90;
        for(int k = 0; k < numRotations; ++k) {
            int newWidth = image.height;
            int newHeight = image.width;
            Image newImage(newWidth, newHeight);
            for(int j = 0; j < newWidth; ++j) {
                for(int i = 0; i < newHeight; ++i) {
                    newImage(newWidth - j - 1, i, 0) = image(i, j, 0);
                    newImage(newWidth - j - 1, i, 1) = image(i, j, 1);
                    newImage(newWidth - j - 1, i, 2) = image(i, j, 2);
                }
            }
            image = newImage;
        }
    }      
};
#pragma once
#include "Filter.h"

class ContrastFilter : public Filter {
private:
    double contrastFactor;

public:
    ContrastFilter(double factor = 2) : contrastFactor(factor) {}

    void apply(Image &image) override {
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                image(x, y, 0) = clamp((int)(128 + (image(x, y, 0) - 128) * contrastFactor), 0, 255);
                image(x, y, 1) = clamp((int)(128 + (image(x, y, 1) - 128) * contrastFactor), 0, 255);
                image(x, y, 2) = clamp((int)(128 + (image(x, y, 2) - 128) * contrastFactor), 0, 255);
            }
        }
    }
};
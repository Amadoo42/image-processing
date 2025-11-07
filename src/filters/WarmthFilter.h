#pragma once
#include "Filter.h"

class WarmthFilter : public Filter {
private:
    double warmthFactor;

public:
    WarmthFilter(double factor = 2.5) : warmthFactor(factor) {}

    void apply(Image &image) override {
        // We basically increase the red and green channels while decreasing the blue channel
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                double rMul = 1.0 + warmthFactor * 0.18;
                double gMul = 1.0 + warmthFactor * 0.12;
                double bMul = 1.0 - warmthFactor * 0.06;

                image(x, y, 0) = std::clamp((int)(image(x, y, 0) * rMul), 0, 255);
                image(x, y, 1) = std::clamp((int)(image(x, y, 1) * gMul), 0, 255);
                image(x, y, 2) = std::clamp((int)(image(x, y, 2) * bMul), 0, 255);
            }
        }
    }
};
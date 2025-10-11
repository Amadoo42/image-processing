#pragma once
#include "Filter.h"

class SaturationFilter : public Filter {
private:
    double saturationFactor;

public:
    SaturationFilter(double factor = 2) : saturationFactor(factor) {}

    void apply(Image &image) override {
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                double gray = 0.299 * image(x, y, 0) + 0.587 * image(x, y, 1) + 0.114 * image(x, y, 2);
                image(x, y, 0) = std::clamp((int)(gray + (image(x, y, 0) - gray) * saturationFactor), 0, 255);
                image(x, y, 1) = std::clamp((int)(gray + (image(x, y, 1) - gray) * saturationFactor), 0, 255);
                image(x, y, 2) = std::clamp((int)(gray + (image(x, y, 2) - gray) * saturationFactor), 0, 255);
            }
        }
    }
};
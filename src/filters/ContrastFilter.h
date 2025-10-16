#pragma once
#include "Filter.h"

class ContrastFilter : public Filter {
private:
    double contrastFactor;

public:
    ContrastFilter(double factor = 1.0) : contrastFactor(factor < 0.0 ? 0.0 : factor) {}

    void apply(Image &image) override {
        // Clamp at runtime for safety
        if (contrastFactor < 0.0) contrastFactor = 0.0;
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                image(x, y, 0) = std::clamp((int)(128 + (image(x, y, 0) - 128) * contrastFactor), 0, 255);
                image(x, y, 1) = std::clamp((int)(128 + (image(x, y, 1) - 128) * contrastFactor), 0, 255);
                image(x, y, 2) = std::clamp((int)(128 + (image(x, y, 2) - 128) * contrastFactor), 0, 255);
            }
        }
    }
};
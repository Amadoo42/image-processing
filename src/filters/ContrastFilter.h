#pragma once
#include "Filter.h"

class ContrastFilter : public Filter {
private:
    double contrastFactor;

public:
    ContrastFilter(double factor = 1.0f) : contrastFactor(factor) {}
    // Contrast basically stretches the difference from the midpoint (128)

    void apply(Image &image) override {
        // Just to make sure we don't invert colors
        contrastFactor = std::max(contrastFactor, 0.0);
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                image(x, y, 0) = std::clamp((int)(128 + (image(x, y, 0) - 128) * contrastFactor), 0, 255);
                image(x, y, 1) = std::clamp((int)(128 + (image(x, y, 1) - 128) * contrastFactor), 0, 255);
                image(x, y, 2) = std::clamp((int)(128 + (image(x, y, 2) - 128) * contrastFactor), 0, 255);
            }
        }
    }
};
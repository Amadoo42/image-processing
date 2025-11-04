#pragma once
#include "Filter.h"

class VigentteFilter : public Filter {
private:
    double vigentteFactor;

public:
    VigentteFilter(double factor = 1.5) : vigentteFactor(factor) {}
    void apply(Image &image) override {
        // We will darken pixels based on the distance from the center
        int centerX = image.width / 2;
        int centerY = image.height / 2;
        // We will use the maximum distance from the center to a corner for normalization
        double maxDistance = sqrt(centerX * centerX + centerY * centerY);

        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                double dx = x - centerX;
                double dy = y - centerY;
                double distance = sqrt(dx * dx + dy * dy);
                // The factor will depend on the percentage distance from the center
                // If distance is closer to maxDistance, the factor will be lower
                double factor = 1.0 - (distance / maxDistance) * vigentteFactor;

                factor = std::max(0.0, std::min(1.0, factor));

                for(int c = 0; c < 3; ++c) image(x, y, c) = std::clamp((int)(image(x, y, c) * factor), 0, 255);
            }
        }
    }
};
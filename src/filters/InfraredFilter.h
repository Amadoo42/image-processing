#pragma once
#include "Filter.h"

class InfraredFilter : public Filter {
public:    
    void apply(Image &image) override {
        // For infrared effect, we map brightness to a red hue
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                double brightness = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);
                // Alpha determines the intensity of the infrared effect
                double alpha = (brightness / 255.0);
                // Basically dark areas of the image (low brightness) become white, and bright areas (high brightness) become red
                double r = 255.0 * alpha + (1 - alpha) * 255.0;
                double g = 0.0 * alpha + (1 - alpha) * 255.0;
                double b = 0.0 * alpha + (1 - alpha) * 255.0;

                image(i, j, 0) = std::clamp(r, 0.0, 255.0);
                image(i, j, 1) = std::clamp(g, 0.0, 255.0);
                image(i, j, 2) = std::clamp(b, 0.0, 255.0);
            }
        }
    }
};
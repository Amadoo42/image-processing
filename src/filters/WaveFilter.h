#pragma once
#include "Filter.h"

class WaveFilter : public Filter {
private:
    float amplitude, wavelength;

public:
    WaveFilter(float a, float w): amplitude(a), wavelength(w) {}
    void apply(Image &image) override {
        // We basically shift pixels horizontally based on a sine wave function
        Image newImage(image);
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                int srci = std::clamp(static_cast<int>(i + amplitude * sin(2 * M_PI * j / wavelength)), 0, image.width - 1);
                int srcj = std::clamp(j, 0, image.height - 1);

                for(int c = 0; c < 3; c++)
                    newImage(i, j, c) = image(srci, srcj, c);
            }
        }
        image = newImage;
    }      
};
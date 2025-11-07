#pragma once
#include "Filter.h"

class DarkenFilter : public Filter {
private:
    float darkenFactor;

public:
    DarkenFilter(float factor = 1.0f) : darkenFactor(factor) {}
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    image(i, j, k) = std::clamp(image(i, j, k) * darkenFactor, 0.0f, 255.0f);
                }
            }
        }
    }      
};
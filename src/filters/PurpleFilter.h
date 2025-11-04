#pragma once
#include "Filter.h"

class PurpleFilter : public Filter {
private:
    float purpleFactor;

public:
    PurpleFilter(float factor): purpleFactor(factor) {}
    void apply(Image &image) override {
        // We simply increase the red and blue channels while decreasing the green channel
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){                
                int newR = std::clamp(image(i, j, 0) + 40 * purpleFactor, 0.0f, 255.0f);
                int newG = std::clamp(image(i, j, 1) - 40 * purpleFactor, 0.0f, 255.0f);
                int newB = std::clamp(image(i, j, 2) + 40 * purpleFactor, 0.0f, 255.0f);
                
                image(i, j, 0) = newR;
                image(i, j, 1) = newG;
                image(i, j, 2) = newB;
            }
        }
    }
};
#pragma once
#include "Filter.h"

class PurpleFilter : public Filter {
private:
float percent;
public:
    PurpleFilter(float p): percent(p) {}    
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){                
                int newR = image(i, j, 0) + 40 * percent;
                int newG = image(i, j, 1) - 40 * percent;
                int newB = image(i, j, 2) + 40 * percent;

                if(newR < 0) newR = 0;
                if(newR > 255) newR = 255;
                
                if(newG < 0) newG = 0;
                if(newG > 255) newG = 255;
                
                if(newB < 0) newB = 0;
                if(newB > 255) newB = 255;
                
                image(i, j, 0) = newR;
                image(i, j, 1) = newG;
                image(i, j, 2) = newB;
            }
        }
    }
};
#pragma once
#include "Filter.h"

class Purple : public Filter {
public:
    Purple() : Filter("Purple") {}
    
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                double brightness = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);
                double alpha = 1.0 - (brightness / 255.0);

                image(i, j, 0) = (1 - alpha) * image(i, j, 0) + alpha * 128;
                image(i, j, 1) = (1 - alpha) * image(i, j, 1) + alpha * 0;
                image(i, j, 2) = (1 - alpha) * image(i, j, 2) + alpha * 128;

                if(image(i, j, 0) < 0) image(i, j, 0) = 0;
                if(image(i, j, 0) > 255) image(i, j, 0) = 255;
                
                if(image(i, j, 1) < 0) image(i, j, 1) = 0;
                if(image(i, j, 1) > 255) image(i, j, 1) = 255;
                
                if(image(i, j, 2) < 0) image(i, j, 2) = 0;
                if(image(i, j, 2) > 255) image(i, j, 2) = 255;
            }
        }
    }      
};
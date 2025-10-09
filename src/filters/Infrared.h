#pragma once
#include "Filter.h"

class Infrared : public Filter {
public:    
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                double brightness = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);
                double alpha = (brightness / 255.0);

                image(i, j, 0) = 255 * alpha + (1 - alpha) * 255;  
                image(i, j, 1) = 0 * alpha + (1 - alpha) * 255;   
                image(i, j, 2) = 0 * alpha + (1 - alpha) * 255;

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
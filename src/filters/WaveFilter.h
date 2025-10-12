#pragma once
#include "Filter.h"

class WaveFilter : public Filter {
private:
float amplitude, wavelength;
public:
    WaveFilter(float a, float w): amplitude(a), wavelength(w) {}
    void apply(Image &image) override {
        Image newImage(image);
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                int srci = i + amplitude * sin(2 * M_PI * j / wavelength);
                int srcj = j;


                if(srci >= image.width)srci = image.width - 1;
                if(srci < 0)srci = 0;

                if(srcj >= image.height)srcj = image.height - 1;
                if(srcj < 0)srcj = 0;

                for(int c = 0; c < 3; c++)
                    newImage(i, j, c) = image(srci, srcj, c);
            }
        }
        image = newImage;
    }      
};
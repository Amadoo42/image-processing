#pragma once
#include "Filter.h"
#include "GrayscaleFilter.h"
#include "BlurFilter.h"
#include <vector>

// References: https://en.wikipedia.org/wiki/Sobel_operator
class OutlineFilter : public Filter {
public:
    void apply(Image &image) override {
        GrayscaleFilter gray;
        BlurFilter blur;
        gray.apply(image);
        blur.apply(image);
        int w = image.width, h = image.height;
        std::vector <std::vector <int>> z(w, std::vector <int>(h, 0));
        double threshold = 0.2;
        int mx = 0;
        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                int gx = -1 * image(std::clamp(i - 1, 0, w - 1), std::clamp(j - 1, 0, h - 1), 0) 
                    + image(std::clamp(i + 1, 0, w - 1), std::clamp(j - 1, 0, h - 1), 0)
                    + -2 * image(std::clamp(i - 1, 0, w - 1), std::clamp(j, 0, h - 1), 0) 
                    + 2 * image(std::clamp(i + 1, 0, w - 1), std::clamp(j, 0, h - 1), 0)
                    + -1 * image(std::clamp(i - 1, 0, w - 1), std::clamp(j + 1, 0, h - 1), 0) 
                    + image(std::clamp(i + 1, 0, w - 1), std::clamp(j + 1, 0, h - 1), 0);
                    
                int gy = -1 * image(std::clamp(i - 1, 0, w - 1), std::clamp(j - 1, 0, h - 1), 0) 
                    + -2 * image(std::clamp(i, 0, w - 1), std::clamp(j - 1, 0, h - 1), 0) 
                    - image(std::clamp(i + 1, 0, w - 1), std::clamp(j - 1, 0, h - 1), 0)
                    + 1 * image(std::clamp(i - 1, 0, w - 1), std::clamp(j + 1, 0, h - 1), 0) 
                    + 2 * image(std::clamp(i, 0, w - 1), std::clamp(j + 1, 0, h - 1), 0) 
                    + image(std::clamp(i + 1, 0, w - 1), std::clamp(j + 1, 0, h - 1), 0);

                int g = sqrt(gx * gx + gy * gy);
                z[i][j] = g;
                mx = std::max(mx, g);
            }
        }
        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                int g = z[i][j];
                int val = 0;
                double f = g;
                f /= mx;
                if(f < threshold) val = 255;
                for(int k = 0; k < 3; k++) image(i, j, k) = val;
            }
        }
    }      
};
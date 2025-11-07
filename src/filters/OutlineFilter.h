#pragma once
#include "Filter.h"
#include "GrayscaleFilter.h"
#include "BlurFilter.h"
#include <vector>

// References: https://en.wikipedia.org/wiki/Sobel_operator
class OutlineFilter : public Filter {
public:
    void apply(Image &image) override {
        // We first convert the image to grayscale and then apply a blur to reduce noise
        GrayscaleFilter gray;
        BlurFilter blur;
        gray.apply(image);
        blur.apply(image);
        
        const int w = image.width;
        const int h = image.height;

        std::vector <std::vector <int>> z(w, std::vector <int>(h, 0));
        double threshold = 0.2;
        int mx = 0;

        auto getGray = [&](int x, int y) -> int {
            x = std::clamp(x, 0, w - 1);
            y = std::clamp(y, 0, h - 1);
            return image(x, y, 0);
        };

        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                int gx =
                    -1 * getGray(i - 1, j - 1) + 1 * getGray(i + 1, j - 1) +
                    -2 * getGray(i - 1, j    ) + 2 * getGray(i + 1, j    ) +
                    -1 * getGray(i - 1, j + 1) + 1 * getGray(i + 1, j + 1);

                int gy =
                    -1 * getGray(i - 1, j - 1) + -2 * getGray(i,     j - 1) + -1 * getGray(i + 1, j - 1) +
                     1 * getGray(i - 1, j + 1) +  2 * getGray(i,     j + 1) +  1 * getGray(i + 1, j + 1);


                int g = static_cast<int>(std::sqrt(static_cast<double>(gx * gx + gy * gy)));
                z[i][j] = g;
                mx = std::max(mx, g);
            }
        }

        mx = std::max(mx, 1); // To avoid division by zero

        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                int g = z[i][j];
                double normalized = static_cast<double>(g) / static_cast<double>(mx);
                int val = (normalized < threshold) ? 255 : 0; // white for non-edges, black for edges
                for (int k = 0; k < 3; ++k) image(i, j, k) = val;
            }
        }
    }      
};
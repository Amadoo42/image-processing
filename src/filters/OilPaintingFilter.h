#pragma once
#include "Filter.h"
#include <vector>
#include <algorithm>

class OilPaintingFilter : public Filter {
private:
    int radius, intensity;

public:
    OilPaintingFilter(int rad = 5, int _intensity = 20): radius(rad), intensity(_intensity) {}

    void apply(Image &image) override {
        Image newImage(image.width, image.height);
        for(int i = 0; i < image.width; ++i) {
            for(int j = 0; j < image.height; ++j) {

                int i0 = std::max(0, i - radius);
                int i1 = std::min(image.width - 1, i + radius);
                int j0 = std::max(0, j - radius);
                int j1 = std::min(image.height - 1, j + radius);

                std::vector <int> freq(intensity + 1, 0);
                std::vector <int> avgR(intensity + 1, 0);
                std::vector <int> avgG(intensity + 1, 0);
                std::vector <int> avgB(intensity + 1, 0);

                // We find the most frequent intensity in the neighborhood
                int mx = 0, idx = 0;
                for(int x = i0; x <= i1; x++) {
                    for(int y = j0; y <= j1; y++) {
                        double r = image(x, y, 0);
                        double g = image(x, y, 1);
                        double b = image(x, y, 2);

                        int curIntensity = static_cast<int>(((r + g + b) / 3.0) * intensity / 255.0);

                        freq[curIntensity]++;
                        avgR[curIntensity] += static_cast<int>(r);
                        avgG[curIntensity] += static_cast<int>(g);
                        avgB[curIntensity] += static_cast<int>(b);

                        if (freq[curIntensity] > mx) {
                            mx = freq[curIntensity];
                            idx = curIntensity;
                        }
                    }
                }
                newImage(i, j, 0) = avgR[idx] / mx;
                newImage(i, j, 1) = avgG[idx] / mx;
                newImage(i, j, 2) = avgB[idx] / mx;
            }
        }
        image = newImage;
    }      
};
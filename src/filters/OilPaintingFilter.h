#pragma once
#include "Filter.h"
using namespace std;

class OilPaintingFilter : public Filter {
public:
    void apply(Image &image) override {
        int radius = 5, intensity = 20;
        Image img(image.width, image.height);
        for(int i = 0; i < image.width; ++i) {
            for(int j = 0; j < image.height; ++j) {
                int i0, i1, j0, j1;
                i0 = max(0, i - radius), i1 = min(image.width - 1, i + radius);
                j0 = max(0, j - radius), j1 = min(image.height - 1, j + radius);
                int freq[intensity + 1] = {};
                int avgR[intensity + 1] = {};
                int avgG[intensity + 1] = {};
                int avgB[intensity + 1] = {};
                int mx = 0, idx = 0;
                for(int x = i0; x <= i1; x++) {
                    for(int y = j0; y <= j1; y++) {
                        int sum = image(x, y, 0) + image(x, y, 1) + image(x, y, 2);
                        sum *= intensity;
                        double z = sum;
                        z /= 3;
                        z /= 255;
                        int Z = z;
                        freq[Z]++;
                        avgR[Z] += image(x, y, 0);
                        avgG[Z] += image(x, y, 1);
                        avgB[Z] += image(x, y, 2);
                        if(freq[Z] > mx) {
                            mx = freq[Z];
                            idx = Z;
                        }
                    }
                }
                img(i, j, 0) = avgR[idx] / mx;
                img(i, j, 1) = avgG[idx] / mx;
                img(i, j, 2) = avgB[idx] / mx;
            }
        }
        image = img;
    }      
};
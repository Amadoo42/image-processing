#pragma once
#include "Filter.h"

class BlurFilter : public Filter {
private:
    int kernelSize;
    double sigma;

    // Reference: https://en.wikipedia.org/wiki/Gaussian_blur, https://www.geeksforgeeks.org/machine-learning/gaussian-kernel/
    vector <double> generateGaussianKernel() {
        vector <double> kernel(kernelSize);
        int half = kernelSize / 2;
        double sum = 0.0;

        for(int x = -half; x <= half; ++x) {
            kernel[x + half] = exp(-(x * x) / (2 * sigma * sigma)); 
            sum += kernel[x + half];
        }

        for(auto &i : kernel) i /= sum;

        return kernel;
    }

public:
    BlurFilter(int kSize = 13, double sig = 2.0) : kernelSize(kSize), sigma(sig) {}

    void apply(Image &image) override {
        int W = image.width, H = image.height;
        vector <double> kernel = generateGaussianKernel();

        Image temp(W, H);
        int half = kernelSize / 2;

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int k = -half; k <= half; ++k) {
                        int nx = clamp(x + k, 0, W - 1);
                        weightedSum += image(nx, y, c) * kernel[k + half];
                    }
                    temp(x, y, c) = round(weightedSum);
                }
            }
        }

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int k = -half; k <= half; ++k) {
                        int ny = clamp(y + k, 0, H - 1);
                        weightedSum += temp(x, ny, c) * kernel[k + half];
                    }
                    image(x, y, c) = round(weightedSum);
                }
            }
        }
    }
};
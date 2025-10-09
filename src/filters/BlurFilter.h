#pragma once
#include "Filter.h"
#include <vector>
using namespace std;

class BlurFilter : public Filter {
private:
    int kernelSize;
    double sigma;
    // Reference: https://en.wikipedia.org/wiki/Gaussian_blur, https://www.geeksforgeeks.org/machine-learning/gaussian-kernel/
    vector <vector <double>> generate2DGaussianKernel() {
        vector <vector <double>> kernel(kernelSize, vector <double>(kernelSize));

        int half = kernelSize / 2;
        double sigma2 = 2 * sigma * sigma, sum = 0.0;
        for(int x = -half; x <= half; ++x) {
            for(int y = -half; y <= half; ++y) { 
                kernel[x + half][y + half] = exp(-(x * x + y * y) / (sigma2)) / (M_PI * sigma2);
                sum += kernel[x + half][y + half];
            }
        }

        for(auto &i : kernel)
            for(auto &j : i)
                j /= sum;

        return kernel;
    }   

public:
    BlurFilter(int kernel = 7, double sig = 1.0) : kernelSize(kernel), sigma(sig) {}

    void apply(Image &image) override {
        int W = image.width, H = image.height;
        vector <vector <double>> kernel = generate2DGaussianKernel();

        Image newImage(W, H);
        int half = kernelSize / 2;
        
        for(int y = 0; y < H; ++y) {
            for(int x = 0; x < W; ++x) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int ky = -half; ky <= half; ++ky) {
                        for(int kx = -half; kx <= half; ++kx) {
                            int nx = min(max(x + kx, 0), W - 1);
                            int ny = min(max(y + ky, 0), H - 1);
                            weightedSum += image(nx, ny, c) * kernel[kx + half][ky + half];
                        }
                    }
                    newImage(x, y, c) = round(weightedSum);
                }
            }
        }
        
        image = newImage;
    }      
};
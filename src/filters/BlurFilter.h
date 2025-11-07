#pragma once
#include "Filter.h"
#include "ResizeFilter.h"
#include <algorithm>
#include <vector>

// Gaussian blur with separable kernel
class BlurFilter : public Filter {
private:
    int kernelSize;
    double sigma;

    std::vector <double> generateGaussianKernel() {
        std::vector <double> kernel(kernelSize);
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
    // We will blur using Gaussian Blur with the separable kernel method for optimizing
    // Kernel size determines how many neighboring pixels around the central pixel are included in the blur calculation
    // Sigma determines the spread or width of the Gaussian kernel, which weights neighboring pixels around a central pixel

    void apply(Image &image) override {
        int ogW = image.width, ogH = image.height;
        long long totalPixels = (long long)ogW * ogH;

        const long long MIN_PIXELS = 1000000;
        const long long MAX_PIXELS = 8000000;
        const double MIN_DOWNSAMPLE_FACTOR = 1.0;
        const double MAX_DOWNSAMPLE_FACTOR = 4.0;

        double downsampleFactor = MIN_DOWNSAMPLE_FACTOR;

        if(totalPixels > MIN_PIXELS) {
            if(totalPixels >= MAX_PIXELS) downsampleFactor = MAX_DOWNSAMPLE_FACTOR;
            else {
                // We use linear interpolation here to tell me how much I should downsample by
                // alpha is the interpolation factor which tells me how far is totalPixels between MIN_PIXELS and MAX_PIXELS
                double alpha = (double)(totalPixels - MIN_PIXELS) / (MAX_PIXELS - MIN_PIXELS);
                downsampleFactor = (1.0 - alpha) * MIN_DOWNSAMPLE_FACTOR + alpha * MAX_DOWNSAMPLE_FACTOR;
            }
        }

        int newW = std::max(1, (int)round(ogW / downsampleFactor));
        int newH = std::max(1, (int)round(ogH / downsampleFactor));

        if(newW == ogW || newH == ogH) {
            int W = image.width, H = image.height;
            std::vector <double> kernel = generateGaussianKernel();

            Image temp(W, H);
            int half = kernelSize / 2;

            for(int x = 0; x < W; ++x) {
                for(int y = 0; y < H; ++y) {
                    for(int c = 0; c < 3; ++c) {
                        double weightedSum = 0.0;
                        for(int k = -half; k <= half; ++k) {
                            int nx = std::clamp(x + k, 0, W - 1);
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
                            int ny = std::clamp(y + k, 0, H - 1);
                            weightedSum += temp(x, ny, c) * kernel[k + half];
                        }
                        image(x, y, c) = round(weightedSum);
                    }
                }
            }
            return;
        }

        // We've changed this to make it a little faster, where we first downsample the image to 25% of the resolution
        // Then blur it then upsample it again. This significantly improves speed while not losing too much accuracy.
        Image downsampledImage = image;
        ResizeFilter downsample(newW, newH);
        downsample.apply(downsampledImage);
        
        int W = downsampledImage.width, H = downsampledImage.height;
        std::vector <double> kernel = generateGaussianKernel();
        Image temp(W, H);
        int half = kernelSize / 2;

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int k = -half; k <= half; ++k) {
                        int nx = std::clamp(x + k, 0, W - 1);
                        weightedSum += downsampledImage(nx, y, c) * kernel[k + half];
                    }
                    temp(x, y, c) = round(weightedSum);
                }
            }
        }

        Image upsampledImage(W, H);

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int k = -half; k <= half; ++k) {
                        int ny = std::clamp(y + k, 0, H - 1);
                        weightedSum += temp(x, ny, c) * kernel[k + half];
                    }
                    upsampledImage(x, y, c) = round(weightedSum);
                }
            }
        }

        ResizeFilter upsample(ogW, ogH);
        upsample.apply(upsampledImage);

        image = upsampledImage;
    }
};
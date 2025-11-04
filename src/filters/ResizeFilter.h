#pragma once
#include "Filter.h"
#include <algorithm>
#include <cmath>

class ResizeFilter : public Filter {
private:
    int newWidth, newHeight;
    double ratioX, ratioY;
    bool useRatio = false;

    // References: https://en.wikipedia.org/wiki/Bilinear_interpolation
    double sampleBilinear(const Image &image, int dstX, int dstY, int dstW, int dstH, int c) const {
        const int srcW = std::max(1, image.width);
        const int srcH = std::max(1, image.height);

        const double srcX = (dstW > 1)
            ? (static_cast<double>(dstX) * (srcW - 1)) / static_cast<double>(dstW - 1)
            : 0.0;
        const double srcY = (dstH > 1)
            ? (static_cast<double>(dstY) * (srcH - 1)) / static_cast<double>(dstH - 1)
            : 0.0;

        const int x1 = static_cast<int>(std::floor(srcX));
        const int y1 = static_cast<int>(std::floor(srcY));
        const int x2 = std::min(srcW - 1, x1 + 1);
        const int y2 = std::min(srcH - 1, y1 + 1);

        const double fx = srcX - x1;
        const double fy = srcY - y1;

        const double w11 = (1.0 - fx) * (1.0 - fy);
        const double w21 = fx * (1.0 - fy);
        const double w12 = (1.0 - fx) * fy;
        const double w22 = fx * fy;

        const double p11 = image(x1, y1, c);
        const double p21 = image(x2, y1, c);
        const double p12 = image(x1, y2, c);
        const double p22 = image(x2, y2, c);

        return p11 * w11 + p21 * w21 + p12 * w12 + p22 * w22;
    }

public:
    ResizeFilter(int nW, int nH) : newWidth(nW), newHeight(nH), useRatio(false) {}
    ResizeFilter(double rX, double rY) : ratioX(rX), ratioY(rY), useRatio(true) {}

    void apply(Image &image) override {
        if (useRatio) {
            newWidth = static_cast<int>(std::round(static_cast<double>(image.width) * ratioX));
            newHeight = static_cast<int>(std::round(static_cast<double>(image.height) * ratioY));
        }

        newWidth = std::max(1, newWidth);
        newHeight = std::max(1, newHeight);

        if (newWidth == image.width && newHeight == image.height) {
            return;
        }

        Image newImage(newWidth, newHeight);
        for (int x = 0; x < newWidth; ++x) {
            for (int y = 0; y < newHeight; ++y) {
                const double r = sampleBilinear(image, x, y, newWidth, newHeight, 0);
                const double g = sampleBilinear(image, x, y, newWidth, newHeight, 1);
                const double b = sampleBilinear(image, x, y, newWidth, newHeight, 2);

                newImage(x, y, 0) = std::clamp(r, 0.0, 255.0);
                newImage(x, y, 1) = std::clamp(g, 0.0, 255.0);
                newImage(x, y, 2) = std::clamp(b, 0.0, 255.0);
            }
        }
        image = newImage;
    }      
};
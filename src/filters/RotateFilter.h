#pragma once
#include "Filter.h"
#include <cmath>
#include <algorithm>

class RotateFilter : public Filter {
private:
    double angleDegrees;
    bool expandCanvas;

    //safe getPixel, if the pixel outside the original image it returns 0 (black)
    double getPixel(const Image &src, int x, int y, int c) {
        if (x < 0 || x >= src.width || y < 0 || y >= src.height) return 0.0;
        return src(x, y, c);
    }

    // References: https://en.wikipedia.org/wiki/Bilinear_interpolation
    double sampleBilinear(const Image &src, double x, double y, int c) {
        int x1 = std::floor(x);
        int y1 = std::floor(y);
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        double fx = x - x1;
        double fy = y - y1;

        double p11 = getPixel(src, x1, y1, c);
        double p21 = getPixel(src, x2, y1, c);
        double p12 = getPixel(src, x1, y2, c);
        double p22 = getPixel(src, x2, y2, c);

        double w11 = (1.0 - fx) * (1.0 - fy);
        double w21 = fx * (1.0 - fy);
        double w12 = (1.0 - fx) * fy;
        double w22 = fx * fy;

        return p11 * w11 + p21 * w21 + p12 * w12 + p22 * w22;
    }

public:
    RotateFilter(double degrees, bool expand = true)
        : angleDegrees(degrees), expandCanvas(expand) {}

    void apply(Image &image) override {
        //initialize the angle in radian with its cosine and sine values
        const double PI = 3.14159265358979323846;
        const double radians = angleDegrees * PI / 180.0;
        const double c = std::cos(radians);
        const double s = std::sin(radians);


        // Source center
        const double centerXS = (image.width  - 1.0) * 0.5;
        const double centerYS = (image.height - 1.0) * 0.5;

        int newWidth = image.width;
        int newHeight = image.height;
        if (expandCanvas) {
            const double abs_c = std::fabs(c);
            const double abs_s = std::fabs(s);
            //standard formula to calculate axis-aligned bounding box (AABB)
            //in short calculated the bounds of a rotated rectangle
            newWidth = std::max(1, std::ceil(image.width  * abs_c + image.height * abs_s));
            newHeight = std::max(1, std::ceil(image.width  * abs_s + image.height * abs_c));
        }

        Image output(newWidth, newHeight);

        //destenation center
        const double centerXD = (newWidth  - 1.0) * 0.5;
        const double centerYD = (newHeight  - 1.0) * 0.5;

        for (int y = 0; y < outH; ++y) {
            for (int x = 0; x < outW; ++x) {
                // Destination pixel relative to its center
                const double dx = x - centerXD;
                const double dy = y - centerYD;

                //multiplying by rotation matrix
                // [ cos sin] [dx]
                // [-sin cos] [dy]
                const double sXRel =  c * dx + s * dy;
                const double sYRel = -s * dx + c * dy;

                const double sX = sXRel + centerXS;
                const double sY = sYRel + centerYS;

                // Bilinear sample with zero padding outside image bounds
                for (int ch = 0; ch < 3; ++ch) {
                    const double v = sampleBilinear(image, sX, sY, ch);
                    output(x, y, ch) = std::clamp(v, 0, 255);
                }
            }
        }

        image = output;
    }
};
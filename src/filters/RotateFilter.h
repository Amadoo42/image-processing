#pragma once
#include "Filter.h"
#include <cmath>
#include <algorithm>

// Arbitrary-angle rotation using bilinear interpolation.
// By default expands the canvas to fit the rotated image.
class RotateFilter : public Filter {
private:
    double angleDegrees;
    bool expandCanvas;

    static inline unsigned char samplePixelSafe(const Image &src, int x, int y, int c) {
        if (x < 0 || x >= src.width || y < 0 || y >= src.height) return 0;
        return src(x, y, c);
    }

    static inline double sampleBilinearZero(const Image &src, double x, double y, int c) {
        // Bilinear sample with zero-padding outside image bounds
        int x1 = static_cast<int>(std::floor(x));
        int y1 = static_cast<int>(std::floor(y));
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        double fx = x - x1;
        double fy = y - y1;

        double p11 = samplePixelSafe(src, x1, y1, c);
        double p21 = samplePixelSafe(src, x2, y1, c);
        double p12 = samplePixelSafe(src, x1, y2, c);
        double p22 = samplePixelSafe(src, x2, y2, c);

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
        constexpr double PI = 3.14159265358979323846;
        const double radians = angleDegrees * PI / 180.0;
        const double c = std::cos(radians);
        const double s = std::sin(radians);


        // Source center
        const double cxS = (static_cast<double>(image.width)  - 1.0) * 0.5;
        const double cyS = (static_cast<double>(image.height) - 1.0) * 0.5;

        int outW = image.width;
        int outH = image.height;
        if (expandCanvas) {
            const double abs_c = std::fabs(c);
            const double abs_s = std::fabs(s);
            outW = std::max(1, static_cast<int>(std::ceil(image.width  * abs_c + image.height * abs_s)));
            outH = std::max(1, static_cast<int>(std::ceil(image.width  * abs_s + image.height * abs_c)));
        }

        Image output(outW, outH);
        const double cxD = (static_cast<double>(outW)  - 1.0) * 0.5;
        const double cyD = (static_cast<double>(outH)  - 1.0) * 0.5;

        for (int y = 0; y < outH; ++y) {
            for (int x = 0; x < outW; ++x) {
                // Destination pixel relative to its center
                const double dx = static_cast<double>(x) - cxD;
                const double dy = static_cast<double>(y) - cyD;

                // Inverse rotate to find source position
                const double sxRel =  c * dx + s * dy;
                const double syRel = -s * dx + c * dy;

                const double sx = sxRel + cxS;
                const double sy = syRel + cyS;

                // Bilinear sample with zero padding outside
                for (int ch = 0; ch < 3; ++ch) {
                    const double v = sampleBilinearZero(image, sx, sy, ch);
                    output(x, y, ch) = static_cast<unsigned char>(std::clamp(static_cast<int>(std::lround(v)), 0, 255));
                }
            }
        }

        image = output;
    }
};
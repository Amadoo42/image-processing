#pragma once
#include "Filter.h"

class GrayscaleFilter : public Filter {
public:
    // Legacy: full-frame grayscale
    void apply(Image &image) override {
        // Use new API with a full mask for consistency
        Image fullMask(image.width, image.height);
        for (int y = 0; y < image.height; ++y)
            for (int x = 0; x < image.width; ++x)
                for (int c = 0; c < 3; ++c) fullMask(x, y, c) = 255;
        Image out = apply((const Image&)image, (const Image&)fullMask);
        image = out;
    }

    // New: masked grayscale with per-pixel blending
    Image apply(const Image &original, const Image &mask) override {
        Image result = original; // start from original
        int w = original.width, h = original.height;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                // mask is stored as grayscale in all channels (due to 3-channel Image class)
                unsigned char m = (mask.width == w && mask.height == h) ? mask(x, y, 0) : 0;
                if (m == 0) continue;
                float a = (float)m / 255.0f;
                int r = original(x, y, 0);
                int g = original(x, y, 1);
                int b = original(x, y, 2);
                int gray = (int)(0.2126f * r + 0.7152f * g + 0.0722f * b + 0.5f);
                // Blend each channel toward gray by mask alpha
                auto blend = [&](int src, int tgt){ float out = (1.0f - a) * src + a * tgt; if (out < 0) out = 0; if (out > 255) out = 255; return (unsigned char)(out + 0.5f); };
                result(x, y, 0) = blend(r, gray);
                result(x, y, 1) = blend(g, gray);
                result(x, y, 2) = blend(b, gray);
            }
        }
        return result;
    }
};
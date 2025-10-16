#pragma once

#include "../../external/Image_Class.h"

// Base filter interface with backward-compatible and new masked API.
class Filter {
public:
    // Legacy in-place API (still used widely across the codebase)
    virtual void apply(Image &image) = 0;

    // New API: return a processed image using a selection mask (0..255).
    // Default fallback ignores mask and calls legacy API on a copy.
    virtual Image apply(const Image &original, const Image &mask) {
        Image result = original;
        apply(result);
        return result;
    }

    virtual ~Filter() = default;
};
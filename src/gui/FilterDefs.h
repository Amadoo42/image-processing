#pragma once

#include <string>

// Enumerates available filters and operations
enum class FilterType {
    None = 0,
    Grayscale,
    Invert,
    Blur,
    BlackAndWhite,
    Crop,
    Resize,
    Brightness,
    Frame,
    HorizontalFlip,
    VerticalFlip,
    Flip,
    Merge,
    Rotate,
    Outline,
    Purple,
    Infrared,
    Wave,
    OilPainting,
    Retro,
    Saturation,
    Contrast,
    Skew,
    Vignette,
    Warmth,
};

inline const char* filterTypeName(FilterType t) {
    switch (t) {
        case FilterType::None: return "None";
        case FilterType::Grayscale: return "Grayscale";
        case FilterType::Invert: return "Invert";
        case FilterType::Blur: return "Blur";
        case FilterType::BlackAndWhite: return "Black & White";
        case FilterType::Crop: return "Crop";
        case FilterType::Resize: return "Resize";
        case FilterType::Brightness: return "Brightness";
        case FilterType::Frame: return "Frame";
        case FilterType::HorizontalFlip: return "Horizontal Flip";
        case FilterType::VerticalFlip: return "Vertical Flip";
        case FilterType::Flip: return "Flip";
        case FilterType::Merge: return "Merge";
        case FilterType::Rotate: return "Rotate";
        case FilterType::Outline: return "Outline";
        case FilterType::Purple: return "Purple";
        case FilterType::Infrared: return "Infrared";
        case FilterType::Wave: return "Wave";
        case FilterType::OilPainting: return "Oil Painting";
        case FilterType::Retro: return "Retro";
        case FilterType::Saturation: return "Saturation";
        case FilterType::Contrast: return "Contrast";
        case FilterType::Skew: return "Skew";
        case FilterType::Vignette: return "Vignette";
        case FilterType::Warmth: return "Warmth";
    }
    return "Unknown";
}

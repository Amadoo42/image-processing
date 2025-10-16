#pragma once

#include <string>
#include "../../external/Image_Class.h"

struct Layer {
    Image activeImage;
    bool isVisible = true;
    float opacity = 1.0f; // 0..1
    std::string name;
    bool isSelected = false; // for GUI highlighting

    Layer() = default;
    explicit Layer(const Image &img, const std::string &layerName = "Layer")
        : activeImage(img), isVisible(true), opacity(1.0f), name(layerName), isSelected(false) {}
};

#pragma once

#include "../../external/Image_Class.h"
#include <string>
#include <memory>

enum class BlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    SoftLight,
    HardLight,
    ColorDodge,
    ColorBurn,
    Darken,
    Lighten,
    Difference,
    Exclusion
};

class Layer {
private:
    std::string name;
    Image image;
    float opacity;
    bool visible;
    bool locked;
    BlendMode blendMode;
    int zIndex; // For layer ordering

public:
    Layer(const std::string& layerName, int width, int height);
    Layer(const std::string& layerName, const Image& img);
    Layer(const Layer& other);
    Layer& operator=(const Layer& other);
    ~Layer() = default;

    // Getters
    const std::string& getName() const { return name; }
    const Image& getImage() const { return image; }
    Image& getImage() { return image; }
    float getOpacity() const { return opacity; }
    bool isVisible() const { return visible; }
    bool isLocked() const { return locked; }
    BlendMode getBlendMode() const { return blendMode; }
    int getZIndex() const { return zIndex; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setOpacity(float newOpacity) { opacity = std::clamp(newOpacity, 0.0f, 1.0f); }
    void setVisible(bool newVisible) { visible = newVisible; }
    void setLocked(bool newLocked) { locked = newLocked; }
    void setBlendMode(BlendMode newBlendMode) { blendMode = newBlendMode; }
    void setZIndex(int newZIndex) { zIndex = newZIndex; }

    // Layer operations
    void clear();
    void fill(unsigned char r, unsigned char g, unsigned char b);
    void resize(int newWidth, int newHeight);
    void crop(int x, int y, int width, int height);
    
    // Blend two layers together
    static Image blendLayers(const Layer& bottom, const Layer& top);
    
    // Apply blend mode to two images
    static Image applyBlendMode(const Image& bottom, const Image& top, BlendMode mode, float opacity);
};
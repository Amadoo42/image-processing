#include "Layer.h"
#include <algorithm>
#include <cmath>

Layer::Layer(const std::string& layerName, int width, int height) 
    : name(layerName), image(width, height), opacity(1.0f), visible(true), 
      locked(false), blendMode(BlendMode::Normal), zIndex(0) {
    // Fill with transparent (black) initially
    fill(0, 0, 0);
}

Layer::Layer(const std::string& layerName, const Image& img) 
    : name(layerName), image(img), opacity(1.0f), visible(true), 
      locked(false), blendMode(BlendMode::Normal), zIndex(0) {
}

Layer::Layer(const Layer& other) 
    : name(other.name), image(other.image), opacity(other.opacity), 
      visible(other.visible), locked(other.locked), blendMode(other.blendMode), 
      zIndex(other.zIndex) {
}

Layer& Layer::operator=(const Layer& other) {
    if (this != &other) {
        name = other.name;
        image = other.image;
        opacity = other.opacity;
        visible = other.visible;
        locked = other.locked;
        blendMode = other.blendMode;
        zIndex = other.zIndex;
    }
    return *this;
}

void Layer::clear() {
    if (!locked) {
        fill(0, 0, 0);
    }
}

void Layer::fill(unsigned char r, unsigned char g, unsigned char b) {
    if (locked) return;
    
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            image.setPixel(x, y, 0, r);
            image.setPixel(x, y, 1, g);
            image.setPixel(x, y, 2, b);
        }
    }
}

void Layer::resize(int newWidth, int newHeight) {
    if (locked) return;
    
    // Create new image with new dimensions
    Image newImage(newWidth, newHeight);
    
    // Simple nearest neighbor scaling
    float scaleX = (float)image.width / newWidth;
    float scaleY = (float)image.height / newHeight;
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = (int)(x * scaleX);
            int srcY = (int)(y * scaleY);
            
            // Clamp to source image bounds
            srcX = std::min(srcX, image.width - 1);
            srcY = std::min(srcY, image.height - 1);
            
            for (int c = 0; c < 3; c++) {
                newImage.setPixel(x, y, c, image.getPixel(srcX, srcY, c));
            }
        }
    }
    
    image = newImage;
}

void Layer::crop(int x, int y, int width, int height) {
    if (locked) return;
    
    // Clamp crop area to image bounds
    x = std::max(0, std::min(x, image.width));
    y = std::max(0, std::min(y, image.height));
    width = std::min(width, image.width - x);
    height = std::min(height, image.height - y);
    
    if (width <= 0 || height <= 0) return;
    
    Image croppedImage(width, height);
    
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            for (int c = 0; c < 3; c++) {
                croppedImage.setPixel(dx, dy, c, image.getPixel(x + dx, y + dy, c));
            }
        }
    }
    
    image = croppedImage;
}

Image Layer::blendLayers(const Layer& bottom, const Layer& top) {
    if (!top.isVisible()) {
        return bottom.getImage();
    }
    
    return applyBlendMode(bottom.getImage(), top.getImage(), top.getBlendMode(), top.getOpacity());
}

Image Layer::applyBlendMode(const Image& bottom, const Image& top, BlendMode mode, float opacity) {
    int width = std::min(bottom.width, top.width);
    int height = std::min(bottom.height, top.height);
    
    Image result(width, height);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get pixel values (normalized to 0-1)
            float br = bottom.getPixel(x, y, 0) / 255.0f;
            float bg = bottom.getPixel(x, y, 1) / 255.0f;
            float bb = bottom.getPixel(x, y, 2) / 255.0f;
            
            float tr = top.getPixel(x, y, 0) / 255.0f;
            float tg = top.getPixel(x, y, 1) / 255.0f;
            float tb = top.getPixel(x, y, 2) / 255.0f;
            
            float fr, fg, fb;
            
            switch (mode) {
                case BlendMode::Normal:
                    fr = tr; fg = tg; fb = tb;
                    break;
                case BlendMode::Multiply:
                    fr = br * tr; fg = bg * tg; fb = bb * tb;
                    break;
                case BlendMode::Screen:
                    fr = 1.0f - (1.0f - br) * (1.0f - tr);
                    fg = 1.0f - (1.0f - bg) * (1.0f - tg);
                    fb = 1.0f - (1.0f - bb) * (1.0f - tb);
                    break;
                case BlendMode::Overlay:
                    fr = (br < 0.5f) ? 2.0f * br * tr : 1.0f - 2.0f * (1.0f - br) * (1.0f - tr);
                    fg = (bg < 0.5f) ? 2.0f * bg * tg : 1.0f - 2.0f * (1.0f - bg) * (1.0f - tg);
                    fb = (bb < 0.5f) ? 2.0f * bb * tb : 1.0f - 2.0f * (1.0f - bb) * (1.0f - tb);
                    break;
                case BlendMode::SoftLight:
                    fr = (tr < 0.5f) ? 2.0f * br * tr + br * br * (1.0f - 2.0f * tr) : 2.0f * br * (1.0f - tr) + std::sqrt(br) * (2.0f * tr - 1.0f);
                    fg = (tg < 0.5f) ? 2.0f * bg * tg + bg * bg * (1.0f - 2.0f * tg) : 2.0f * bg * (1.0f - tg) + std::sqrt(bg) * (2.0f * tg - 1.0f);
                    fb = (tb < 0.5f) ? 2.0f * bb * tb + bb * bb * (1.0f - 2.0f * tb) : 2.0f * bb * (1.0f - tb) + std::sqrt(bb) * (2.0f * tb - 1.0f);
                    break;
                case BlendMode::HardLight:
                    fr = (tr < 0.5f) ? 2.0f * br * tr : 1.0f - 2.0f * (1.0f - br) * (1.0f - tr);
                    fg = (tg < 0.5f) ? 2.0f * bg * tg : 1.0f - 2.0f * (1.0f - bg) * (1.0f - tg);
                    fb = (tb < 0.5f) ? 2.0f * bb * tb : 1.0f - 2.0f * (1.0f - bb) * (1.0f - tb);
                    break;
                case BlendMode::ColorDodge:
                    fr = (tr == 1.0f) ? 1.0f : std::min(1.0f, br / (1.0f - tr));
                    fg = (tg == 1.0f) ? 1.0f : std::min(1.0f, bg / (1.0f - tg));
                    fb = (tb == 1.0f) ? 1.0f : std::min(1.0f, bb / (1.0f - tb));
                    break;
                case BlendMode::ColorBurn:
                    fr = (tr == 0.0f) ? 0.0f : std::max(0.0f, 1.0f - (1.0f - br) / tr);
                    fg = (tg == 0.0f) ? 0.0f : std::max(0.0f, 1.0f - (1.0f - bg) / tg);
                    fb = (tb == 0.0f) ? 0.0f : std::max(0.0f, 1.0f - (1.0f - bb) / tb);
                    break;
                case BlendMode::Darken:
                    fr = std::min(br, tr); fg = std::min(bg, tg); fb = std::min(bb, tb);
                    break;
                case BlendMode::Lighten:
                    fr = std::max(br, tr); fg = std::max(bg, tg); fb = std::max(bb, tb);
                    break;
                case BlendMode::Difference:
                    fr = std::abs(br - tr); fg = std::abs(bg - tg); fb = std::abs(bb - tb);
                    break;
                case BlendMode::Exclusion:
                    fr = br + tr - 2.0f * br * tr;
                    fg = bg + tg - 2.0f * bg * tg;
                    fb = bb + tb - 2.0f * bb * tb;
                    break;
            }
            
            // Apply opacity
            fr = br + (fr - br) * opacity;
            fg = bg + (fg - bg) * opacity;
            fb = bb + (fb - bb) * opacity;
            
            // Clamp and convert back to 0-255
            result.setPixel(x, y, 0, (unsigned char)std::clamp(fr * 255.0f, 0.0f, 255.0f));
            result.setPixel(x, y, 1, (unsigned char)std::clamp(fg * 255.0f, 0.0f, 255.0f));
            result.setPixel(x, y, 2, (unsigned char)std::clamp(fb * 255.0f, 0.0f, 255.0f));
        }
    }
    
    return result;
}
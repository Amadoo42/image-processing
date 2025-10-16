#pragma once

#include <vector>
#include <functional>

class SelectionMask {
private:
    std::vector<bool> mask;
    int width;
    int height;
    
public:
    SelectionMask(int w, int h);
    SelectionMask(int w, int h, bool initialValue);
    SelectionMask(const SelectionMask& other);
    SelectionMask& operator=(const SelectionMask& other);
    ~SelectionMask() = default;
    
    // Basic operations
    void clear();
    void fill(bool value);
    void invert();
    
    // Selection operations
    void select(int x, int y);
    void deselect(int x, int y);
    void toggle(int x, int y);
    bool isSelected(int x, int y) const;
    
    // Area operations
    void selectRectangle(int x1, int y1, int x2, int y2);
    void selectCircle(int centerX, int centerY, int radius);
    void selectLasso(const std::vector<std::pair<int, int>>& points);
    void selectMagicWand(int x, int y, int tolerance, const class Image& image);
    
    // Boolean operations
    void unionWith(const SelectionMask& other);
    void intersectWith(const SelectionMask& other);
    void subtract(const SelectionMask& other);
    
    // Transformations
    void resize(int newWidth, int newHeight);
    void translate(int dx, int dy);
    void rotate(float angle, int centerX, int centerY);
    void scale(float scaleX, float scaleY);
    
    // Utility functions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getSelectedPixelCount() const;
    bool isEmpty() const;
    bool isFull() const;
    
    // Bounding box
    void getBoundingBox(int& minX, int& minY, int& maxX, int& maxY) const;
    
    // Iteration support
    void forEachSelected(std::function<void(int x, int y)> callback) const;
    void forEachPixel(std::function<void(int x, int y, bool selected)> callback) const;
    
    // Serialization
    std::vector<bool> getMaskData() const { return mask; }
    void setMaskData(const std::vector<bool>& data, int w, int h);
};
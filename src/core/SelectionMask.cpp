#include "SelectionMask.h"
#include "../../external/Image_Class.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <set>

SelectionMask::SelectionMask(int w, int h) : width(w), height(h) {
    mask.resize(w * h, false);
}

SelectionMask::SelectionMask(int w, int h, bool initialValue) : width(w), height(h) {
    mask.resize(w * h, initialValue);
}

SelectionMask::SelectionMask(const SelectionMask& other) 
    : mask(other.mask), width(other.width), height(other.height) {
}

SelectionMask& SelectionMask::operator=(const SelectionMask& other) {
    if (this != &other) {
        mask = other.mask;
        width = other.width;
        height = other.height;
    }
    return *this;
}

void SelectionMask::clear() {
    std::fill(mask.begin(), mask.end(), false);
}

void SelectionMask::fill(bool value) {
    std::fill(mask.begin(), mask.end(), value);
}

void SelectionMask::invert() {
    for (bool& pixel : mask) {
        pixel = !pixel;
    }
}

void SelectionMask::select(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        mask[y * width + x] = true;
    }
}

void SelectionMask::deselect(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        mask[y * width + x] = false;
    }
}

void SelectionMask::toggle(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        mask[y * width + x] = !mask[y * width + x];
    }
}

bool SelectionMask::isSelected(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return mask[y * width + x];
}

void SelectionMask::selectRectangle(int x1, int y1, int x2, int y2) {
    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);
    int minY = std::min(y1, y2);
    int maxY = std::max(y1, y2);
    
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            select(x, y);
        }
    }
}

void SelectionMask::selectCircle(int centerX, int centerY, int radius) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = x - centerX;
            int dy = y - centerY;
            if (dx * dx + dy * dy <= radius * radius) {
                select(x, y);
            }
        }
    }
}

void SelectionMask::selectLasso(const std::vector<std::pair<int, int>>& points) {
    if (points.size() < 3) return;
    
    // Use ray casting algorithm for point-in-polygon test
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool inside = false;
            
            for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
                int xi = points[i].first;
                int yi = points[i].second;
                int xj = points[j].first;
                int yj = points[j].second;
                
                if (((yi > y) != (yj > y)) && 
                    (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
                    inside = !inside;
                }
            }
            
            if (inside) {
                select(x, y);
            }
        }
    }
}

void SelectionMask::selectMagicWand(int x, int y, int tolerance, const Image& image) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    if (x >= image.width || y >= image.height) return;
    
    // Get the reference color
    unsigned char refR = image.getPixel(x, y, 0);
    unsigned char refG = image.getPixel(x, y, 1);
    unsigned char refB = image.getPixel(x, y, 2);
    
    // Flood fill algorithm
    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, int>> visited;
    
    queue.push({x, y});
    visited.insert({x, y});
    
    while (!queue.empty()) {
        auto [currentX, currentY] = queue.front();
        queue.pop();
        
        // Check if this pixel should be selected
        if (currentX >= 0 && currentX < image.width && 
            currentY >= 0 && currentY < image.height) {
            
            unsigned char r = image.getPixel(currentX, currentY, 0);
            unsigned char g = image.getPixel(currentX, currentY, 1);
            unsigned char b = image.getPixel(currentX, currentY, 2);
            
            // Calculate color distance
            int distance = std::abs(r - refR) + std::abs(g - refG) + std::abs(b - refB);
            
            if (distance <= tolerance) {
                select(currentX, currentY);
                
                // Add neighbors to queue
                std::vector<std::pair<int, int>> neighbors = {
                    {currentX + 1, currentY},
                    {currentX - 1, currentY},
                    {currentX, currentY + 1},
                    {currentX, currentY - 1}
                };
                
                for (const auto& neighbor : neighbors) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        queue.push(neighbor);
                    }
                }
            }
        }
    }
}

void SelectionMask::unionWith(const SelectionMask& other) {
    int minWidth = std::min(width, other.width);
    int minHeight = std::min(height, other.height);
    
    for (int y = 0; y < minHeight; y++) {
        for (int x = 0; x < minWidth; x++) {
            if (other.isSelected(x, y)) {
                select(x, y);
            }
        }
    }
}

void SelectionMask::intersectWith(const SelectionMask& other) {
    int minWidth = std::min(width, other.width);
    int minHeight = std::min(height, other.height);
    
    for (int y = 0; y < minHeight; y++) {
        for (int x = 0; x < minWidth; x++) {
            if (!other.isSelected(x, y)) {
                deselect(x, y);
            }
        }
    }
}

void SelectionMask::subtract(const SelectionMask& other) {
    int minWidth = std::min(width, other.width);
    int minHeight = std::min(height, other.height);
    
    for (int y = 0; y < minHeight; y++) {
        for (int x = 0; x < minWidth; x++) {
            if (other.isSelected(x, y)) {
                deselect(x, y);
            }
        }
    }
}

void SelectionMask::resize(int newWidth, int newHeight) {
    if (newWidth == width && newHeight == height) return;
    
    std::vector<bool> newMask(newWidth * newHeight, false);
    
    float scaleX = (float)width / newWidth;
    float scaleY = (float)height / newHeight;
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = (int)(x * scaleX);
            int srcY = (int)(y * scaleY);
            
            if (srcX >= 0 && srcX < width && srcY >= 0 && srcY < height) {
                newMask[y * newWidth + x] = mask[srcY * width + srcX];
            }
        }
    }
    
    mask = std::move(newMask);
    width = newWidth;
    height = newHeight;
}

void SelectionMask::translate(int dx, int dy) {
    std::vector<bool> newMask(width * height, false);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int newX = x - dx;
            int newY = y - dy;
            
            if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                newMask[newY * width + newX] = mask[y * width + x];
            }
        }
    }
    
    mask = std::move(newMask);
}

void SelectionMask::rotate(float angle, int centerX, int centerY) {
    std::vector<bool> newMask(width * height, false);
    
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mask[y * width + x]) {
                // Translate to origin
                float relX = x - centerX;
                float relY = y - centerY;
                
                // Rotate
                float newX = relX * cosA - relY * sinA;
                float newY = relX * sinA + relY * cosA;
                
                // Translate back
                int finalX = (int)(newX + centerX);
                int finalY = (int)(newY + centerY);
                
                if (finalX >= 0 && finalX < width && finalY >= 0 && finalY < height) {
                    newMask[finalY * width + finalX] = true;
                }
            }
        }
    }
    
    mask = std::move(newMask);
}

void SelectionMask::scale(float scaleX, float scaleY) {
    int newWidth = (int)(width * scaleX);
    int newHeight = (int)(height * scaleY);
    
    if (newWidth <= 0 || newHeight <= 0) return;
    
    std::vector<bool> newMask(newWidth * newHeight, false);
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = (int)(x / scaleX);
            int srcY = (int)(y / scaleY);
            
            if (srcX >= 0 && srcX < width && srcY >= 0 && srcY < height) {
                newMask[y * newWidth + x] = mask[srcY * width + srcX];
            }
        }
    }
    
    mask = std::move(newMask);
    width = newWidth;
    height = newHeight;
}

int SelectionMask::getSelectedPixelCount() const {
    int count = 0;
    for (bool pixel : mask) {
        if (pixel) count++;
    }
    return count;
}

bool SelectionMask::isEmpty() const {
    return getSelectedPixelCount() == 0;
}

bool SelectionMask::isFull() const {
    return getSelectedPixelCount() == width * height;
}

void SelectionMask::getBoundingBox(int& minX, int& minY, int& maxX, int& maxY) const {
    minX = width;
    minY = height;
    maxX = -1;
    maxY = -1;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mask[y * width + x]) {
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
            }
        }
    }
    
    if (maxX == -1) {
        minX = minY = maxX = maxY = 0;
    }
}

void SelectionMask::forEachSelected(std::function<void(int x, int y)> callback) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mask[y * width + x]) {
                callback(x, y);
            }
        }
    }
}

void SelectionMask::forEachPixel(std::function<void(int x, int y, bool selected)> callback) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            callback(x, y, mask[y * width + x]);
        }
    }
}

void SelectionMask::setMaskData(const std::vector<bool>& data, int w, int h) {
    if (data.size() != w * h) return;
    
    mask = data;
    width = w;
    height = h;
}
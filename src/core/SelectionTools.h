#pragma once

#include "SelectionMask.h"
#include "../../external/Image_Class.h"
#include <vector>
#include <memory>

enum class SelectionTool {
    None,
    Rectangle,
    Lasso,
    MagicWand,
    Ellipse,
    Polygon
};

struct SelectionState {
    SelectionMask mask;
    bool isActive;
    int startX, startY;
    int currentX, currentY;
    std::vector<std::pair<int, int>> lassoPoints;
    SelectionTool currentTool;
    int tolerance; // For magic wand
    bool isDrawing;
    
    SelectionState(int width, int height) 
        : mask(width, height), isActive(false), startX(0), startY(0), 
          currentX(0), currentY(0), currentTool(SelectionTool::None), 
          tolerance(30), isDrawing(false) {}
};

class SelectionTools {
private:
    std::unique_ptr<SelectionState> state;
    int canvasWidth;
    int canvasHeight;
    
public:
    SelectionTools(int width, int height);
    ~SelectionTools() = default;
    
    // Tool management
    void setTool(SelectionTool tool);
    SelectionTool getCurrentTool() const { return state->currentTool; }
    
    // Selection state
    bool hasSelection() const { return !state->mask.isEmpty(); }
    const SelectionMask& getSelection() const { return state->mask; }
    void clearSelection();
    void invertSelection();
    
    // Mouse interaction
    void onMouseDown(int x, int y, const Image& image);
    void onMouseMove(int x, int y, const Image& image);
    void onMouseUp(int x, int y, const Image& image);
    
    // Tool parameters
    void setTolerance(int tolerance) { state->tolerance = tolerance; }
    int getTolerance() const { return state->tolerance; }
    
    // Selection operations
    void selectAll();
    void selectNone();
    void growSelection(int pixels);
    void shrinkSelection(int pixels);
    void featherSelection(int pixels);
    
    // Boolean operations
    void addToSelection(const SelectionMask& other);
    void subtractFromSelection(const SelectionMask& other);
    void intersectWithSelection(const SelectionMask& other);
    
    // Transformations
    void moveSelection(int dx, int dy);
    void rotateSelection(float angle, int centerX, int centerY);
    void scaleSelection(float scaleX, float scaleY);
    
    // Utility
    void resizeCanvas(int newWidth, int newHeight);
    bool isPointInSelection(int x, int y) const;
    void getBoundingBox(int& minX, int& minY, int& maxX, int& maxY) const;
    
    // Drawing helpers
    void drawSelectionOutline(class ImDrawList* drawList, float zoom, float offsetX, float offsetY) const;
    void drawToolPreview(class ImDrawList* drawList, float zoom, float offsetX, float offsetY) const;
    
private:
    void updateRectangleSelection();
    void updateLassoSelection();
    void updateMagicWandSelection(const Image& image);
    void updateEllipseSelection();
    void updatePolygonSelection();
    
    // Helper functions
    void addLassoPoint(int x, int y);
    void finalizeLassoSelection();
    void finalizePolygonSelection();
    
    // Drawing utilities
    void drawRectangleOutline(class ImDrawList* drawList, int x1, int y1, int x2, int y2, 
                             float zoom, float offsetX, float offsetY, ImU32 color) const;
    void drawLassoOutline(class ImDrawList* drawList, const std::vector<std::pair<int, int>>& points,
                         float zoom, float offsetX, float offsetY, ImU32 color) const;
    void drawEllipseOutline(class ImDrawList* drawList, int centerX, int centerY, int radiusX, int radiusY,
                           float zoom, float offsetX, float offsetY, ImU32 color) const;
};
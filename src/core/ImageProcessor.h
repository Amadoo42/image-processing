// This will basically manage the current image and provide undo/redo functionality
#pragma once

#include "../external/Image_Class.h"
#include "../filters/Filter.h"
#include <vector>
#include <string>


class ImageProcessor {
private:
    GLuint currentTextureID;
    Image currentImage;
    std::vector <Image> undoHistory;
    std::vector <Image> redoHistory;
    int historySize;

    void pushUndo() {
        if((int)undoHistory.size() >= historySize) undoHistory.erase(undoHistory.begin());
        undoHistory.push_back(currentImage);
    }

    void pushRedo() {
        if((int)redoHistory.size() >= historySize) redoHistory.erase(redoHistory.begin());
        redoHistory.push_back(currentImage);
    }

public:
    ImageProcessor(int hSize = 10) : historySize(hSize) {};

    void loadImage(const std::string &filename) {
        Image newImage(filename);
        currentImage = newImage;
        undoHistory.clear();
        redoHistory.clear();
    }

    bool saveImage(const std::string &filename) {
        return currentImage.saveImage(filename); 
    }

    void applyFilter(Filter &filter) {
        pushUndo();
        redoHistory.clear();
        filter.apply(currentImage);
        //textureNeedsUpdate = true;
    } 

    // Apply a filter without affecting undo/redo history.
    // Intended for live previews; caller should manage resetting or committing.
    void applyFilterNoHistory(Filter &filter) {
        filter.apply(currentImage);
    }

    bool undo() {
        if(undoHistory.empty()) return false;
        pushRedo();
        currentImage = undoHistory.back();
        undoHistory.pop_back();
        return true;
    }

    bool redo() {
        if(redoHistory.empty()) return false;
        pushUndo();
        currentImage = redoHistory.back();
        redoHistory.pop_back();
        return true;
    }

    const Image& getCurrentImage() const { return currentImage; }
    void setImage(Image &newImage) { currentImage = newImage; }

    const GLuint& getTextureID() const { return currentTextureID; }

    // Expose read-only accessors for GUI visual history rendering
    const std::vector<Image>& getUndoHistory() const { return undoHistory; }
    const std::vector<Image>& getRedoHistory() const { return redoHistory; }
};
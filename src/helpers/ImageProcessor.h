// This will basically manage the current image and provide undo/redo functionality

#pragma once
#include "Image_Class.h"
#include "../filters/Filter.h"
#include <vector>
using namespace std;

class ImageProcessor {
private:
    Image currentImage;
    vector <Image> undoHistory;
    vector <Image> redoHistory;
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

    void loadImage(const string &filename) {
        Image newImage(filename);
        currentImage = newImage;
        undoHistory.clear();
        redoHistory.clear();
    }

    bool saveImage(const string &filename) {
        return currentImage.saveImage(filename); 
    }

    void applyFilter(Filter &filter) {
        pushUndo();
        redoHistory.clear();
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
};
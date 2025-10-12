#pragma once

#include "../core/ImageProcessor.h"

class FilterParameters {
public:
    FilterParameters(ImageProcessor &proc);

    void applyGrayscale();
    void applyInvert();
    void applyBlur();
    void applyBlackAndWhite();
    void applyCrop();
    void applyResize();
    void applyDarken();
    void applyLighten();
    void applyFrame();
    void applyHorizontalFlip();
    void applyVerticalFlip();
    void applyMerge();
    void applyRotate();
    void applyOutline();
    void applyPurple();
    void applyInfrared();
    void applyWave();
    void applyOilPainting();
    void applyContrast();
    void applyRetro();
    void applySaturation();
    void applySkew();
    void applyVignette();
    void applyWarmth();

private:
    ImageProcessor &processor;    
};
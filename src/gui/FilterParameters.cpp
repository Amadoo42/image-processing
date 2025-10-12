#include "FilterParameters.h"

#include "../filters/GrayscaleFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/DarkenFilter.h"
#include "../filters/LightenFilter.h"
#include "../filters/FrameFilter.h"
#include "../filters/HorizontalFlipFilter.h"
#include "../filters/VerticalFlipFilter.h"
#include "../filters/MergeFilter.h"
#include "../filters/RotateFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/PurpleFilter.h"
#include "../filters/InfraredFilter.h"
#include "../filters/WaveFilter.h"
#include "../filters/OilPaintingFilter.h"
#include "../filters/ContrastFilter.h"
#include "../filters/RetroFilter.h"
#include "../filters/SaturationFilter.h"
#include "../filters/SkewFilter.h"
#include "../filters/VigentteFilter.h"
#include "../filters/WarmthFilter.h"

FilterParameters::FilterParameters(ImageProcessor &proc) : processor(proc) {}

void FilterParameters::applyGrayscale() {
    GrayscaleFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Grayscale Filter\n";
}

void FilterParameters::applyInvert() {
    InvertFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Invert Filter\n";
}

void FilterParameters::applyBlur() {
    BlurFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Blur Filter\n";
}

void FilterParameters::applyBlackAndWhite() {
    BlackAndWhiteFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Black and White Filter\n";
}

void FilterParameters::applyCrop() {
    CropFilter filter(50, 50, 200, 200); 
    processor.applyFilter(filter);
    std::cout << "Applied Crop Filter\n";
}

void FilterParameters::applyResize() {
    ResizeFilter filter(400, 400); 
    processor.applyFilter(filter);
    std::cout << "Applied Resize Filter\n";
}

void FilterParameters::applyDarken() {
    DarkenFilter filter; 
    processor.applyFilter(filter);
    std::cout << "Applied Darken Filter\n";
}

void FilterParameters::applyLighten() {
    LightenFilter filter; 
    processor.applyFilter(filter);
    std::cout << "Applied Lighten Filter\n";
}

void FilterParameters::applyFrame() {
    Image frame_image("../assets/fancy.png");
    FrameFilter filter(frame_image);
    processor.applyFilter(filter);
    std::cout << "Applied Frame Filter\n";
}

void FilterParameters::applyHorizontalFlip() {
    HorizontalFlipFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Horizontal Flip Filter\n";
}

void FilterParameters::applyVerticalFlip() {
    VerticalFlipFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Vertical Flip Filter\n";
}

void FilterParameters::applyMerge() {
    Image merge_image("../assets/SampleImages/luffy.jpg");
    MergeFilter filter(merge_image); 
    processor.applyFilter(filter);
    std::cout << "Applied Merge Filter\n";
}

void FilterParameters::applyRotate() {
    RotateFilter filter(90); 
    processor.applyFilter(filter);
    std::cout << "Applied Rotate Filter\n";
}

void FilterParameters::applyOutline() {
    OutlineFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Outline Filter\n";
}

void FilterParameters::applyPurple() {
    PurpleFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Purple Filter\n";
}

void FilterParameters::applyInfrared() {
    InfraredFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Infrared Filter\n";
}

void FilterParameters::applyWave() {
    WaveFilter filter; 
    processor.applyFilter(filter);
    std::cout << "Applied Wave Filter\n";
}

void FilterParameters::applyOilPainting() {
    OilPaintingFilter filter; 
    processor.applyFilter(filter);
    std::cout << "Applied Oil Painting Filter\n";
}

void FilterParameters::applyContrast() {
    ContrastFilter filter(1.5); 
    processor.applyFilter(filter);
    std::cout << "Applied Contrast Filter\n";
}

void FilterParameters::applyRetro() {
    RetroFilter filter;
    processor.applyFilter(filter);
    std::cout << "Applied Retro Filter\n";
}

void FilterParameters::applySaturation() {
    SaturationFilter filter(1.5); 
    processor.applyFilter(filter);
    std::cout << "Applied Saturation Filter\n";
}

void FilterParameters::applySkew() {
    SkewFilter filter(40); 
    processor.applyFilter(filter);
    std::cout << "Applied Skew Filter\n";
}

void FilterParameters::applyVignette() {
    VigentteFilter filter(0.5); 
    processor.applyFilter(filter);
    std::cout << "Applied Vignette Filter\n";
}

void FilterParameters::applyWarmth() {
    WarmthFilter filter(1.5); 
    processor.applyFilter(filter);
    std::cout << "Applied Warmth Filter\n";
}

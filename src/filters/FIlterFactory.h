#pragma once

#include "Filter.h"
#include "BlackAndWhiteFilter.h"
#include "BlurFilter.h"
#include "CropFilter.h"
#include "DarkenFilter.h"
#include "LightenFilter.h"
#include "FrameFilter.h"
#include "GrayscaleFilter.h"
#include "HorizontalFlipFilter.h"
#include "VerticalFlipFilter.h"
#include "MergeFilter.h"
#include "ResizeFilter.h"
#include "RotateFilter.h"
#include "InvertFilter.h"
#include "OutlineFilter.h"

#include <memory>
#include <functional>
#include <unordered_map>
#include <string>
using namespace std;

class FilterFactory {
private:
    unordered_map <string, function <unique_ptr <IFilter>()>> creators;

    FilterFactory() {
        registerFilter("Black & White", []() { return make_unique <BlackAndWhiteFilter>(); });
        registerFilter("Blur", []() { return make_unique <BlurFilter>(); });
        registerFilter("Crop", []() { return make_unique <CropFilter>(); });
        registerFilter("Darken", []() { return make_unique <DarkenFilter>(); });
        registerFilter("Lighten", []() { return make_unique <LightenFilter>(); });
        registerFilter("Frame", []() { return make_unique <FrameFilter>(); });
        registerFilter("Grayscale", []() { return make_unique <GrayscaleFilter>(); });
        registerFilter("Horizontal Flip", []() { return make_unique <HorizontalFlipFilter>(); });
        registerFilter("Vertical Flip", []() { return make_unique <VerticalFlipFilter>(); });
        registerFilter("Merge", []() { return make_unique <MergeFilter>(); });
        registerFilter("Resize", []() { return make_unique <ResizeFilter>(); });
        registerFilter("Rotate", []() { return make_unique <RotateFilter>(); });
        registerFilter("Invert", []() { return make_unique <InvertFilter>(); });
        registerFilter("Outline", []() { return make_unique <OutlineFilter>(); });
    }

public:
    void registerFilter(const string &name, function <unique_ptr <IFilter>()> creator) {
        creators[name] = creator;
    }
};
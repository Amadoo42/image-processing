#pragma once

// This is a pure virtual base class for image filters
class Filter {
public:
    virtual void apply(Image &image) = 0;
    // We assign a virtual destructor here to make sure that derived classes are properly cleaned up
    virtual ~Filter() = default;
};
#pragma once

class Filter {
public:
    virtual void apply(Image &image) = 0;
    virtual ~Filter() = default;
};
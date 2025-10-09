#pragma once

// References: https://www.geeksforgeeks.org/cpp/virtual-function-cpp/
class Filter {
public:
    virtual void apply(Image &image) = 0;
    virtual ~Filter() = default;
};
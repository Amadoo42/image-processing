#pragma once

// References: https://www.geeksforgeeks.org/cpp/virtual-function-cpp/
class Filter {
private:
    string name;
    
public:
    virtual void apply(Image &image) = 0;
    virtual ~Filter() = default;
    Filter(const string &filterName) : name(filterName) {}
};
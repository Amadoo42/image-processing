#pragma once
#include "Image_Class.h"
#include <string>
using namespace std;

// References: https://www.geeksforgeeks.org/cpp/virtual-function-cpp/
class IFilter {
public:
    virtual void apply(Image &image) = 0;
    virtual ~IFilter() = default;
    virtual string getName() = 0;
};

class Filter : public IFilter {
protected:
    string name;
    
public:
    Filter(const string& filterName) : name(filterName) {}
    string getName() override { return name; }
};
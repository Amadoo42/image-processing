#include <bits/stdc++.h>
using namespace std;

#include "Image_Class.h"

// References: https://www.geeksforgeeks.org/cpp/virtual-function-cpp/
class Filter {
public:
    virtual void apply(Image &image) = 0;
    virtual ~Filter() = default;
};

// This will basically manage the current image and provide undo/redo functionality
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

class BlackAndWhiteFilter : public Filter {
public:    
    void apply(Image &image) override {
        int sum = 0;
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                int gray = 0.2126 * image(i, j, 0) + 0.7152 * image(i, j, 1) + 0.0722 * image(i, j, 2);

                for(int c = 0; c < 3; c++) image(i, j, c) = gray;

                sum += gray;
            }
        }

        int threshold = sum / (image.width * image.height);

        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                if(image(i, j, 0) >= threshold) {
                    image(i, j, 0) = 255;
                    image(i, j, 1) = 255;
                    image(i, j, 2) = 255;
                }
                else {
                    image(i, j, 0) = 0;
                    image(i, j, 1) = 0;
                    image(i, j, 2) = 0;
                }
            }
        }
    }      
};

class BlurFilter : public Filter {
private:
    int kernelSize;
    double sigma;

    // Reference: https://en.wikipedia.org/wiki/Gaussian_blur, https://www.geeksforgeeks.org/machine-learning/gaussian-kernel/
    vector <double> generateGaussianKernel() {
        vector <double> kernel(kernelSize);
        int half = kernelSize / 2;
        double sum = 0.0;

        for(int x = -half; x <= half; ++x) {
            kernel[x + half] = exp(-(x * x) / (2 * sigma * sigma)); 
            sum += kernel[x + half];
        }

        for(auto &i : kernel) i /= sum;

        return kernel;
    }

public:
    BlurFilter(int kSize = 13, double sig = 2.0) : kernelSize(kSize), sigma(sig) {}

    void apply(Image &image) override {
        int W = image.width, H = image.height;
        vector <double> kernel = generateGaussianKernel();

        Image temp(W, H);
        int half = kernelSize / 2;

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int k = -half; k <= half; ++k) {
                        int nx = clamp(x + k, 0, W - 1);
                        weightedSum += image(nx, y, c) * kernel[k + half];
                    }
                    temp(x, y, c) = round(weightedSum);
                }
            }
        }

        for(int x = 0; x < W; ++x) {
            for(int y = 0; y < H; ++y) {
                for(int c = 0; c < 3; ++c) {
                    double weightedSum = 0.0;
                    for(int k = -half; k <= half; ++k) {
                        int ny = clamp(y + k, 0, H - 1);
                        weightedSum += temp(x, ny, c) * kernel[k + half];
                    }
                    image(x, y, c) = round(weightedSum);
                }
            }
        }
    }
};

class CropFilter : public Filter {
private:
    int X, Y, W, H;
public:
    CropFilter(int x, int y, int w, int h) : X(x), Y(y), W(w), H(h) {}

    void apply(Image &image) override {
        Image newImage(W, H);
        for(int i = X; i < X + W; i++) {
            for(int j = Y; j < Y + H; j++) {
                newImage(i - X, j - Y, 0) = image(i, j, 0);
                newImage(i - X, j - Y, 1) = image(i, j, 1);
                newImage(i - X, j - Y, 2) = image(i, j, 2);
            }
        }
        image = newImage;
    }      
};

class ResizeFilter : public Filter {
private:
    int newWidth, newHeight;
    double ratioX, ratioY;
    bool useRatio = false;

    // References: https://en.wikipedia.org/wiki/Bilinear_interpolation
    double color(Image &image,int i, int j, int newWidth, int newHeight, int c){
        double srci= i * image.width / newWidth;
        double srcj= j * image.height / newHeight;
                
        int x1 = srci, y1 = srcj, x2 = min(image.width - 1, x1 + 1), y2 = min(image.height - 1, y1 + 1);

        return (x2 - srci) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x1,y1,c)
                        + (srci - x1) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x2, y1, c)
                        + (x2 - srci) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x1, y2, c)
                        + (srci - x1) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x2, y2, c);
    }

public:
    ResizeFilter(int nW, int nH) : newWidth(nW), newHeight(nH), useRatio(false) {}
    ResizeFilter(double rX, double rY) : ratioX(rX), ratioY(rY), useRatio(true) {}

    void apply(Image &image) override {
        if(useRatio) {
            newWidth = image.width * ratioX;
            newHeight = image.height * ratioY;
        }
        Image newImage(newWidth, newHeight);
        for(int i = 0; i < newWidth; i++){
            for(int j=0; j < newHeight; j++){

                double r = color(image, i, j, newWidth, newHeight, 0);
                double g = color(image, i, j, newWidth, newHeight, 1);
                double b = color(image, i, j, newWidth, newHeight, 2);

                newImage(i, j, 0) = r;
                newImage(i, j, 1) = g;
                newImage(i, j, 2) = b;
            }
        }
        image = newImage;
    }      
};

class DarkenFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    image(i, j, k) = (image(i, j, k) + 0) / 2;
                }
            }
        }
    }      
};

class LightenFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    image(i, j, k) = (image(i, j, k) + 255) / 2;
                }
            }
        }
    }      
};

class FrameFilter : public Filter {
private:
    Image frame_image;
public:
    FrameFilter(const Image &frame_img) : frame_image(frame_img) {}

    void apply(Image &image) override {
        Image frame_image_resized = frame_image;
        ResizeFilter resizeFrame(image.width, image.height);
        resizeFrame.apply(frame_image_resized);
        int border_size = min(image.width, image.height) / 20;
        Image newImage = frame_image_resized;
        int inner_width = image.width - 2 * border_size;
        int inner_height = image.height - 2 * border_size;
        ResizeFilter resize(inner_width, inner_height);
        resize.apply(image);
        for(int x = 0; x < inner_width; ++x) {
            for(int y = 0; y < inner_height; ++y) {
                newImage(x + border_size, y + border_size, 0) = image(x, y, 0);
                newImage(x + border_size, y + border_size, 1) = image(x, y, 1);
                newImage(x + border_size, y + border_size, 2) = image(x, y, 2);
            }
        }
        image = newImage;
    }      
};

class GrayscaleFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                int gray = 0.2126 * image(i, j, 0) + 0.7152 * image(i, j, 1) + 0.0722 * image(i, j, 2);
                for(int c = 0; c < 3; c++) image(i, j, c) = gray;
            }
        }
    }      
};

class HorizontalFlipFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width / 2; i++) {
            for(int j = 0; j < image.height; j++) {
                swap(image(i, j, 0), image(image.width - i - 1, j, 0));
                swap(image(i, j, 1), image(image.width - i - 1, j, 1));
                swap(image(i, j, 2), image(image.width - i - 1, j, 2));   
            }
        }
    }      
};

class VerticalFlipFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int j = 0; j < image.height / 2; j++){
            for(int i = 0; i < image.width; i++){
                swap(image(i, j, 0), image(i, image.height - j - 1, 0));
                swap(image(i, j, 1), image(i, image.height - j - 1, 1));
                swap(image(i, j, 2), image(i, image.height - j - 1, 2));   
            }
        }
    }      
};

class MergeFilter : public Filter {
private:
    Image other_image;
    
public:
    MergeFilter(Image &oth) : other_image(oth) {}

    void apply(Image &image) override {
        // Assuming the images are the same size
        int x = min(image.width, other_image.width);
        int y = min(image.height, other_image.height);
        if(x != image.width || x != other_image.width || y != image.height || y != other_image.height) {
            // #TODO: Handle this elsewhere?
            cout << "Images are not the same size.\n";
            cout << "1. Resize the smaller image to the size of the larger one.\n";
            cout << "2. Crop the larger image to the size of the smaller image.\n";
            while(true) {
                int x; cin >> x;
                int w1, h1, w2, h2;
                w1 = image.width, h1 = image.height;
                w2 = other_image.width, h2 = other_image.height;
                bool z = (w2 * h2 < w1 * h1); // z = 0 means first image is smaller
                if(x == 1) {
                    ResizeFilter resize(max(w1, w2), max(h1, h2));
                    resize.apply(image);
                    resize.apply(other_image);
                    break;
                }
                else if(x == 2) {
                    CropFilter crop(0, 0, min(w1, w2), min(h1, h2));
                    crop.apply(image);
                    crop.apply(other_image);
                    break;
                }
                else cout << "invalid input please try again (input 1 or 2) \n";
            }
        }
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    image(i, j, k) = (image(i, j, k) + other_image(i, j, k)) / 2;
                }
            }
        }
    }      
};

class RotateFilter : public Filter {
private:
    int degrees;

public:
    RotateFilter(int deg) : degrees(deg) {}

    void apply(Image &image) override {
        int numRotations = degrees / 90;
        for(int k = 0; k < numRotations; ++k) {
            int newWidth = image.height;
            int newHeight = image.width;
            Image newImage(newWidth, newHeight);
            for(int j = 0; j < newWidth; ++j) {
                for(int i = 0; i < newHeight; ++i) {
                    newImage(newWidth - j - 1, i, 0) = image(i, j, 0);
                    newImage(newWidth - j - 1, i, 1) = image(i, j, 1);
                    newImage(newWidth - j - 1, i, 2) = image(i, j, 2);
                }
            }
            image = newImage;
        }
    }      
};

class InvertFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; ++i) {
            for(int j = 0; j < image.height; ++j) {
                image(i, j, 0) = 255 - image(i, j, 0);
                image(i, j, 1) = 255 - image(i, j, 1);
                image(i, j, 2) = 255 - image(i, j, 2);
            }
        }
    }      
};

// References: https://en.wikipedia.org/wiki/Sobel_operator
class OutlineFilter : public Filter {
public:
    void apply(Image &image) override {
        GrayscaleFilter gray;
        BlurFilter blur;
        gray.apply(image);
        blur.apply(image);
        int w = image.width, h = image.height;
        vector <vector <int>> z(w, vector <int>(h, 0));
        double threshold = 0.2;
        int mx = 0;
        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                int gx = -1 * image(clamp(i - 1, 0, w - 1), clamp(j - 1, 0, h - 1), 0) 
                    + image(clamp(i + 1, 0, w - 1), clamp(j - 1, 0, h - 1), 0)
                    + -2 * image(clamp(i - 1, 0, w - 1), clamp(j, 0, h - 1), 0) 
                    + 2 * image(clamp(i + 1, 0, w - 1), clamp(j, 0, h - 1), 0)
                    + -1 * image(clamp(i - 1, 0, w - 1), clamp(j - 1, 0, h - 1), 0) 
                    + image(clamp(i + 1, 0, w - 1), clamp(j + 1, 0, h - 1), 0);
                    
                int gy = -1 * image(clamp(i - 1, 0, w - 1), clamp(j - 1, 0, h - 1), 0) 
                    + -2 * image(clamp(i, 0, w - 1), clamp(j - 1, 0, h - 1), 0) 
                    - image(clamp(i + 1, 0, w - 1), clamp(j - 1, 0, h - 1), 0)
                    + 1 * image(clamp(i - 1, 0, w - 1), clamp(j + 1, 0, h - 1), 0) 
                    + 2 * image(clamp(i, 0, w - 1), clamp(j + 1, 0, h - 1), 0) 
                    + image(clamp(i + 1, 0, w - 1), clamp(j + 1, 0, h - 1), 0);

                int g = sqrt(gx * gx + gy * gy);
                z[i][j] = g;
                mx = max(mx, g);
            }
        }
        for(int i = 0; i < w; i++) {
            for(int j = 0; j < h; j++) {
                int g = z[i][j];
                int val = 0;
                double f = g;
                f /= mx;
                if(f < threshold) val = 255;
                for(int k = 0; k < 3; k++) image(i, j, k) = val;
            }
        }
    }      
};

class PurpleFilter : public Filter {
public:    
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){                
                int newR = image(i, j, 0) + 40;
                int newG = image(i, j, 1) - 40;
                int newB = image(i, j, 2) + 40;

                if(newR < 0) newR = 0;
                if(newR > 255) newR = 255;
                
                if(newG < 0) newG = 0;
                if(newG > 255) newG = 255;
                
                if(newB < 0) newB = 0;
                if(newB > 255) newB = 255;
                
                image(i, j, 0) = newR;
                image(i, j, 1) = newG;
                image(i, j, 2) = newB;
            }
        }
    }
};

class InfraredFilter : public Filter {
public:    
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                double brightness = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);
                double alpha = (brightness / 255.0);

                image(i, j, 0) = 255 * alpha + (1 - alpha) * 255;  
                image(i, j, 1) = 0 * alpha + (1 - alpha) * 255;   
                image(i, j, 2) = 0 * alpha + (1 - alpha) * 255;

                if(image(i, j, 0) < 0) image(i, j, 0) = 0;
                if(image(i, j, 0) > 255) image(i, j, 0) = 255;
                
                if(image(i, j, 1) < 0) image(i, j, 1) = 0;
                if(image(i, j, 1) > 255) image(i, j, 1) = 255;
                
                if(image(i, j, 2) < 0) image(i, j, 2) = 0;
                if(image(i, j, 2) > 255) image(i, j, 2) = 255;
            }
        }
    }      
};

class WaveFilter : public Filter {
public:    
    void apply(Image &image) override {
        Image newImage(image);
        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                int srci = i + 10.0 * sin(2 * M_PI * j / 80.0);
                int srcj = j;


                if(srci >= image.width)srci = image.width - 1;
                if(srci < 0)srci = 0;

                if(srcj >= image.height)srcj = image.height - 1;
                if(srcj < 0)srcj = 0;

                for(int c = 0; c < 3; c++)
                    newImage(i, j, c) = image(srci, srcj, c);
            }
        }
        image = newImage;
    }      
};

class OilPaintingFilter : public Filter {
public:
    void apply(Image &image) override {
        int radius = 5, intensity = 20;
        Image img(image.width, image.height);
        for(int i = 0; i < image.width; ++i) {
            for(int j = 0; j < image.height; ++j) {
                int i0, i1, j0, j1;
                i0 = max(0, i - radius), i1 = min(image.width - 1, i + radius);
                j0 = max(0, j - radius), j1 = min(image.height - 1, j + radius);
                int freq[intensity + 1] = {};
                int avgR[intensity + 1] = {};
                int avgG[intensity + 1] = {};
                int avgB[intensity + 1] = {};
                int mx = 0, idx = 0;
                for(int x = i0; x <= i1; x++) {
                    for(int y = j0; y <= j1; y++) {
                        int sum = image(x, y, 0) + image(x, y, 1) + image(x, y, 2);
                        sum *= intensity;
                        double z = sum;
                        z /= 3;
                        z /= 255;
                        int Z = z;
                        freq[Z]++;
                        avgR[Z] += image(x, y, 0);
                        avgG[Z] += image(x, y, 1);
                        avgB[Z] += image(x, y, 2);
                        if(freq[Z] > mx) {
                            mx = freq[Z];
                            idx = Z;
                        }
                    }
                }
                img(i, j, 0) = avgR[idx] / mx;
                img(i, j, 1) = avgG[idx] / mx;
                img(i, j, 2) = avgB[idx] / mx;
            }
        }
        image = img;
    }      
};

class RetroFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                if(j % 4 > 1) {
                    image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 0;
                }
            }
        }
    }      
};

class SkewFilter : public Filter {
private:
    double k = 0.0;

    // References: https://en.wikipedia.org/wiki/Bilinear_interpolation
    double bilinearInterpolate(const Image &image, double x, double y, int channel) {
        int x1 = x;
        int y1 = y;
        int x2 = min(x1 + 1, image.width - 1);
        int y2 = min(y1 + 1, image.height - 1);

        double dx = x - x1;
        double dy = y - y1;

        return (1 - dx) * (1 - dy) * image(x1, y1, channel)
               + dx * (1 - dy) * image(x2, y1, channel)
               + (1 - dx) * dy * image(x1, y2, channel)
               + dx * dy * image(x2, y2, channel);
    }

public:    
    SkewFilter(double deg) {
        k = tan(deg * M_PI / 180.0);
    }

    void apply(Image &image) override {
        int W = image.width;
        int H = image.height;
        int extra = fabs(k) * H;
        int newWidth = W + extra;
        int newHeight = H;
        Image newImage(newWidth, newHeight);

        int xOffset = (k < 0) ? extra : 0;

        for(int i = 0; i < newWidth; ++i) {
            for(int j = 0; j < newHeight; ++j) {
                for(int c = 0; c < 3; ++c) newImage(i, j, c) = 0; 
            }
        }

        for(int x = 0; x < newWidth; ++x) {
            for(int y = 0; y < newHeight; ++y) {
                double srcX = (x - xOffset) - k * (H - y);
                double srcY = y;

                if(srcX < 0 || srcX >= W || srcY < 0 || srcY >= H) continue;

                double r = bilinearInterpolate(image, srcX, srcY, 0);
                double g = bilinearInterpolate(image, srcX, srcY, 1);
                double b = bilinearInterpolate(image, srcX, srcY, 2);

                newImage(x, y, 0) = r;
                newImage(x, y, 1) = g;
                newImage(x, y, 2) = b;
            }
        }

        image = newImage;
    }      
};

class WarmthFilter : public Filter {
private:
    double warmthFactor;

public:
    WarmthFilter(double factor = 2.5) : warmthFactor(factor) {}

    void apply(Image &image) override {
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                double rMul = 1.0 + warmthFactor * 0.18;
                double gMul = 1.0 + warmthFactor * 0.12;
                double bMul = 1.0 - warmthFactor * 0.06;

                image(x, y, 0) = clamp((int)(image(x, y, 0) * rMul), 0, 255);
                image(x, y, 1) = clamp((int)(image(x, y, 1) * gMul), 0, 255);
                image(x, y, 2) = clamp((int)(image(x, y, 2) * bMul), 0, 255);
            }
        }
    }
};

class SaturationFilter : public Filter {
private:
    double saturationFactor;

public:
    SaturationFilter(double factor = 2) : saturationFactor(factor) {}

    void apply(Image &image) override {
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                double gray = 0.299 * image(x, y, 0) + 0.587 * image(x, y, 1) + 0.114 * image(x, y, 2);
                image(x, y, 0) = clamp((int)(gray + (image(x, y, 0) - gray) * saturationFactor), 0, 255);
                image(x, y, 1) = clamp((int)(gray + (image(x, y, 1) - gray) * saturationFactor), 0, 255);
                image(x, y, 2) = clamp((int)(gray + (image(x, y, 2) - gray) * saturationFactor), 0, 255);
            }
        }
    }
};

class ContrastFilter : public Filter {
private:
    double contrastFactor;

public:
    ContrastFilter(double factor = 2) : contrastFactor(factor) {}

    void apply(Image &image) override {
        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                image(x, y, 0) = clamp((int)(128 + (image(x, y, 0) - 128) * contrastFactor), 0, 255);
                image(x, y, 1) = clamp((int)(128 + (image(x, y, 1) - 128) * contrastFactor), 0, 255);
                image(x, y, 2) = clamp((int)(128 + (image(x, y, 2) - 128) * contrastFactor), 0, 255);
            }
        }
    }
};

class VigentteFilter : public Filter {
private:
    double vigentteFactor;

public:
    VigentteFilter(double factor = 1.5) : vigentteFactor(factor) {}
    void apply(Image &image) override {
        int centerX = image.width / 2;
        int centerY = image.height / 2;
        double maxDistance = sqrt(centerX * centerX + centerY * centerY);

        for(int x = 0; x < image.width; ++x) {
            for(int y = 0; y < image.height; ++y) {
                double dx = x - centerX;
                double dy = y - centerY;
                double distance = sqrt(dx * dx + dy * dy);
                double factor = 1.0 - (distance / maxDistance) * vigentteFactor;

                factor = max(0.0, min(1.0, factor));

                for(int c = 0; c < 3; ++c) image(x, y, c) = clamp((int)(image(x, y, c) * factor), 0, 255);
            }
        }
    }
};

class Menu {
private:
    ImageProcessor processor;
    string filename;

    void displayMainMenu() {
        cout << "\n=== Image Processing Menu ===" << endl;
        cout << "1. Load a New Image" << endl;
        cout << "2. Save Current Image" << endl;
        cout << "3. Apply Filter" << endl;
        cout << "4. Undo" << endl;
        cout << "5. Redo" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
    }

    void displayFilterMenu() {
        cout << "\n=== Select Filter ===" << endl;
        cout << "1. Black & White" << endl;
        cout << "2. Darken & Lighten" << endl;
        cout << "3. Add Frame" << endl;
        cout << "4. Grayscale" << endl;
        cout << "5. Flip" << endl;
        cout << "6. Invert" << endl;
        cout << "7. Merge" << endl;
        cout << "8. Resize" << endl;
        cout << "9. Rotate" << endl;
        cout << "10. Blur" << endl;
        cout << "11. Crop" << endl;
        cout << "12. Outline" << endl;
        cout << "13. Purple" << endl;
        cout << "14. Infrared" << endl;
        cout << "15. Wave" << endl;
        cout << "16. Oil Painting" << endl;
        cout << "17. Retro TV" << endl;
        cout << "18. Skew" << endl;
        cout << "19. Warmth" << endl;
        cout << "20. Saturation" << endl;
        cout << "21. Contrast" << endl;
        cout << "22. Vigentte" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Choice: ";
    }

    void handleLoadImage() {
        cout << "Enter image filename: ";
        cin >> filename;
        processor.loadImage(filename);
        cout << "Image loaded successfully!\n";
    }

    void handleSaveImage() {
        int choice;
        while(choice != 1 && choice != 2) {
            cout << "Do you want to overwrite the image?\n";
            cout << "1. Yes\n2. No\nChoice: ";
            cin >> choice;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1 or 2.\n";
                continue;
            }
            if(choice == 1 || choice == 2) break;
            cout << "Invalid choice. Please enter 1 or 2.\n";
        }

        if(choice == 1) {
            processor.saveImage(filename);
            cout << "Image saved successfully!\n";
        }

        else {
            cout << "Enter the filename to save the current image (with extension .jpg, .bmp, .png): ";
            cin >> filename;
            if (processor.saveImage(filename)) cout << "Image saved successfully!\n";
            else cout << "Failed to save image. Please check the filename and try again.\n";
        }
    }

    void applyBlackAndWhite() {
        BlackAndWhiteFilter filter;
        processor.applyFilter(filter);
        cout << "Black & White Filter applied successfully!\n";
    }

    void applyDarkenAndLighten() { 
        int choice;
        while(choice != 1 && choice != 2){
            cout << "1. Darken\n2. Lighten\nChoice: ";
            cin >> choice;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1 or 2.\n";
                continue;
            }
            if(choice == 1 || choice == 2) break;
            cout << "Invalid input. Please enter 1 or 2.\n";
        }
        if(choice == 1){
            DarkenFilter filter;
            processor.applyFilter(filter);
            cout << "Darken Filter applied successfully!\n";
        }
        else if(choice == 2){
            LightenFilter filter;
            processor.applyFilter(filter);
            cout << "Lighten Filter applied successfully!\n";
        }
    }

    void applyFrame() { 
        string frame_image_directory;
        int choice;

        while(choice != 1 && choice != 2){
            cout << "1. Choose a frame\n2. Load your own frame\nChoice: ";
            cin >> choice;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1 or 2.\n";
                continue;
            }
            if(choice == 1 || choice == 2) break;
            cout << "Invalid input. Please enter 1 or 2.\n";
        }
        if(choice == 1){
            int subChoice;
            while(subChoice != 1 && subChoice != 2){
                cout << "1. Normal frame\n2. Fancy frame!\nChoice: ";
                cin >> subChoice;
                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    continue;
                }
                if(subChoice == 1 || subChoice == 2) break;
                cout << "Invalid input. Please enter 1 or 2.\n";
            }
            if(subChoice == 1) frame_image_directory = "../assets/frames/normal.jpg"; 
            else frame_image_directory = "../assets/frames/fancy.jpg"; 
        }
        else {
            cout << "Enter the filename of the frame image: ";
            cin >> frame_image_directory;
        }
        Image frame_image(frame_image_directory);
        FrameFilter filter(frame_image);
        processor.applyFilter(filter);
        cout << "Frame Filter applied successfully!\n";
    }

    void applyGrayscale() {
        GrayscaleFilter filter;
        processor.applyFilter(filter);
        cout << "Grayscale Filter applied successfully!\n";
    }
    
    void applyFlip() { 
        int choice;
        while(choice != 1 && choice != 2){
            cout << "1. Horizontal Flip\n2. Vertical Flip\nChoice: ";
            cin >> choice;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1 or 2.\n";
                continue;
            }
            if(choice == 1 || choice == 2) break;
            cout << "Invalid input. Please enter 1 or 2.\n";
        }
        if(choice == 1){
            HorizontalFlipFilter filter;
            processor.applyFilter(filter);
            cout << "Flip Filter applied successfully!\n";
        }
        else if(choice == 2){
            VerticalFlipFilter filter;
            processor.applyFilter(filter);
            cout << "Flip Filter applied successfully!\n";
        }
    }

    void applyInvert() {
        InvertFilter filter;
        processor.applyFilter(filter);
        cout << "Invert Filter applied successfully!\n";
    }

    void applyMerge() { 
        string merge_image_directory;
        cout << "Enter the filename of the image to merge with: ";  
        cin >> merge_image_directory;
        Image merge_image(merge_image_directory);
        MergeFilter filter(merge_image);
        processor.applyFilter(filter);
        cout << "Merge Filter applied successfully!\n";
    }

    void applyResize() { 
        int choice;
        while(choice != 1 && choice != 2){
            cout << "1. By Ratio\n2. By Dimensions\nChoice: ";
            cin >> choice;
            if(cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1 or 2.\n";
                continue;
            }
            if(choice == 1 || choice == 2) break;
            cout << "Invalid input. Please enter 1 or 2.\n";
        }
        if(choice == 1) {
            double ratioX, ratioY;
            cout << "Enter the ratio for width (e.g., 0.5 for half): ";
            cin >> ratioX;
            cout << "Enter the ratio for height: ";
            cin >> ratioY;
            ResizeFilter filter(ratioX, ratioY);
            processor.applyFilter(filter);
            cout << "Filter applied successfully!\n";
        }
        else if(choice == 2) {
            int newWidth, newHeight;
            cout << "Enter the new width: ";
            cin >> newWidth;
            cout << "Enter the new height: ";
            cin >> newHeight;
            ResizeFilter filter(newWidth, newHeight);
            processor.applyFilter(filter);
            cout << "Resize Filter applied successfully!\n";
        }
    }

    void applyRotate() { 
        int angle;
        cout << "Enter rotation angle (90, 180, 270): ";
        cin >> angle;
        while(angle != 90 && angle != 180 && angle != 270){
            cout << "Invalid angle. Please try again\n";
            cout << "Enter rotation angle (90, 180, 270): ";
            cin >> angle;
        }
        RotateFilter filter(angle);
        processor.applyFilter(filter);
        cout << "Rotate Filter applied successfully!\n";
    }

    void applyBlur() {
        BlurFilter filter;
        processor.applyFilter(filter);
        cout << "Blur Filter applied successfully!\n";
    }

    void applyCrop() { 
        int x1, y1, w, h;
        const Image &image = processor.getCurrentImage();
        cout << "Enter the top-left x and y coordinates, width and height for cropping: ";
        cin >> x1 >> y1 >> w >> h; 
        if(x1 < 0 || y1 < 0 || w < 0 || h < 0 || x1 + w >= image.width || y1 + h >= image.height){
            cout << "Invalid input. Returning to main menu\n";
        }
        else{
            CropFilter filter(x1, y1, w, h);
            processor.applyFilter(filter);
            cout << "Crop Filter applied successfully!\n";
        } 
    }

    void applyOutline() {
        OutlineFilter filter;
        processor.applyFilter(filter);
        cout << "Outline Filter applied successfully!\n";
    }

    void applyPurple() {
        PurpleFilter filter;
        processor.applyFilter(filter);
        cout << "Purple filter applied successfully!\n";
    }

    void applyInfrared() {
        InfraredFilter filter;
        processor.applyFilter(filter);
        cout << "Infrared filter applied successfully!\n";
    }

    void applyWave() {
        WaveFilter filter;
        processor.applyFilter(filter);
        cout << "Wave filter applied successfully!\n";
    }

    void applyOil() {
        OilPaintingFilter filter;
        processor.applyFilter(filter);
        cout << "Oil filter applied successfully!\n";
    }

    void applyRetro() {
        RetroFilter filter;
        processor.applyFilter(filter);
        cout << "Retro TV filter applied successfully!\n";
    }

    void applySkew() { 
        int degrees;
        cout << "Enter skew angle in degrees (positive for right skew, negative for left skew): ";
        cin >> degrees;
        while(degrees < -89 || degrees > 89) {
            cout << "Invalid angle. Enter an angle between -89 and 89\n";
            cin >> degrees;
        }
        SkewFilter filter(degrees);
        processor.applyFilter(filter);
        cout << "Skew filter applied successfully!\n";
    }

    void applyWarmth() {
        double warmth;
        cout << "Enter warmth factor (positive to increase warmth, negative to decrease): ";
        cin >> warmth;
        WarmthFilter filter(warmth);
        processor.applyFilter(filter);
        cout << "Warmth filter applied successfully!\n";
    }

    void applySaturation() { 
        double saturation;
        cout << "Enter saturation factor (greater than 1 to increase, between 0 and 1 to decrease): ";
        cin >> saturation;
        while(saturation < 0) {
            cout << "Invalid factor. Please enter a value greater than 0\n";
            cin >> saturation;
        }
        SaturationFilter filter(saturation);
        processor.applyFilter(filter);
        cout << "Saturation filter applied successfully!\n";
    }

    void applyContrast() { 
        double contrast;
        cout << "Enter contrast factor (greater than 1 to increase, between 0 and 1 to decrease): ";
        cin >> contrast;
        while(contrast < 0) {
            cout << "Invalid factor. Please enter a value greater than 0\n";
            cin >> contrast;
        }
        ContrastFilter filter(contrast);
        processor.applyFilter(filter);
        cout << "Contrast filter applied successfully!\n";
    }

    void applyVigentte() { 
        double vigentte;
        cout << "Enter vigentte factor (positive to increase effect): ";
        cin >> vigentte;
        while(vigentte < 0) {
            cout << "Invalid factor. Please enter a value greater than 0\n";
            cin >> vigentte;
        }
        VigentteFilter filter(vigentte);
        processor.applyFilter(filter);
        cout << "Vigentte filter applied successfully!\n";
    }

    void handleApplyFilter() {
        int choice;
        displayFilterMenu();
        cin >> choice;
        switch(choice) {
            case 1: applyBlackAndWhite(); break;
            case 2: applyDarkenAndLighten(); break;
            case 3: applyFrame(); break;
            case 4: applyGrayscale(); break;
            case 5: applyFlip(); break;
            case 6: applyInvert(); break;
            case 7: applyMerge(); break;
            case 8: applyResize(); break;
            case 9: applyRotate(); break;
            case 10: applyBlur(); break;
            case 11: applyCrop(); break;
            case 12: applyOutline(); break;
            case 13: applyPurple(); break;
            case 14: applyInfrared(); break;
            case 15: applyWave(); break;
            case 16: applyOil(); break;
            case 17: applyRetro(); break;
            case 18: applySkew(); break;
            case 19: applyWarmth(); break;
            case 20: applySaturation(); break;
            case 21: applyContrast(); break;
            case 22: applyVigentte(); break;
            case 0: return; // Back to main menu
            default: cout << "Invalid choice. Returning to main menu." << endl; break;
        }
    }

public:
    void run() {
        int choice;
        while(true) {
            displayMainMenu();
            cin >> choice;
            switch(choice) {
                case 1: handleLoadImage(); break;
                case 2: handleSaveImage(); break;
                case 3: handleApplyFilter(); break;
                case 4: 
                    if(processor.undo()) cout << "Undo successful!\n";
                    else cout << "Nothing to undo.\n";
                    break;
                case 5: 
                    if(processor.redo()) cout << "Redo successful!\n";
                    else cout << "Nothing to redo.\n";
                    break;
                case 0: 
                    cout << "Exiting the program. Goodbye!" << endl;
                    return;
                default: cout << "Invalid choice. Please try again." << endl; break;
            }
        }
    }

};

int main() {
    Menu menu;
    menu.run();

    return 0;
}
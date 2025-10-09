#include "apply_filter.h"

void Filter::blackandwhite(Image &image) {
    int sum=0;
    for(int i=0; i < image.width; i++){
        for(int j=0; j < image.height; j++){
            int gry = 0.2126 * image(i,j,0) + 0.7152 * image(i,j,1) + 0.0722 * image(i,j,2);

            for(int c = 0; c < 3; c++)image(i,j,c) = gry;
            
            sum += gry;
        }
    }

    int threshold = sum / (image.width * image.height);

    for(int i=0; i < image.width; i++){
        for(int j=0; j < image.height; j++){
            if(image(i,j,0) >= threshold){
                image(i,j,0)=255;
                image(i,j,1)=255;
                image(i,j,2)=255;
            }
            else{
                image(i,j,0)=0;
                image(i,j,1)=0;
                image(i,j,2)=0;
            }
        }
    }
}

// Reference: https://en.wikipedia.org/wiki/Gaussian_blur, https://www.geeksforgeeks.org/machine-learning/gaussian-kernel/
vector <vector <double>> generate2DGaussianKernel(int kernelSize, double sigma) {
    vector <vector <double>> kernel(kernelSize, vector <double>(kernelSize));

    int half = kernelSize / 2;
    double sigma2 = 2 * sigma * sigma, sum = 0.0;
    for(int x = -half; x <= half; ++x) {
        for(int y = -half; y <= half; ++y) { 
            kernel[x + half][y + half] = exp(-(x * x + y * y) / (sigma2)) / (M_PI * sigma2);
            sum += kernel[x + half][y + half];
        }
    }

    for(auto &i : kernel)
        for(auto &j : i)
            j /= sum;

    return kernel;
}   

void Filter::blur(Image &image, int kernelSize, double sigma) {
    int W = image.width, H = image.height;
    vector <vector <double>> kernel = generate2DGaussianKernel(kernelSize, sigma);

    Image newImage(W, H);
    int half = kernelSize / 2;
    
    for(int y = 0; y < H; ++y) {
        for(int x = 0; x < W; ++x) {
            for(int c = 0; c < 3; ++c) {
                double weightedSum = 0.0;
                for(int ky = -half; ky <= half; ++ky) {
                    for(int kx = -half; kx <= half; ++kx) {
                        int nx = min(max(x + kx, 0), W - 1);
                        int ny = min(max(y + ky, 0), H - 1);
                        weightedSum += image(nx, ny, c) * kernel[kx + half][ky + half];
                    }
                }
                newImage(x, y, c) = round(weightedSum);
            }
        }
    }
    
    image = newImage;
}

void Filter::crop(Image &image,int x,int y,int w,int h){
    Image newImage(w,h);
    for(int i=x;i<x+w;i++){
        for(int j=y;j<y+h;j++){
            newImage(i-x,j-y,0)=image(i,j,0);
            newImage(i-x,j-y,1)=image(i,j,1);
            newImage(i-x,j-y,2)=image(i,j,2);
        }
    }
    image=newImage;
}

void Filter::darken(Image & a) {
    for(int i = 0; i < a.width; i++) {
        for(int j = 0; j < a.height; j++) {
            for(int k = 0; k < 3; k++) {
                a(i, j, k) = (a(i, j, k) + 0) / 2;
            }
        }
    }
}

void Filter::lighten(Image & a) {
    for(int i = 0; i < a.width; i++) {
        for(int j = 0; j < a.height; j++) {
            for(int k = 0; k < 3; k++) {
                a(i, j, k) = (a(i, j, k) + 255) / 2;
            }
        }
    }
}


int X, Y;
int getx(int x) {
    if(x < 0) return 0;
    if(x >= X) return X - 1;
    return x;
}
int gety(int y) {
    if(y < 0) return 0;
    if(y >= Y) return Y - 1;
    return y;
}
vector<vector<int>> z;// array too big will give us RTE
void Filter::edge(Image & a) {
    gray(a);
    blur(a);
    // return;
    int w = a.width, h = a.height;
    X = w, Y = h;
    // Image b(w, h);
    z.resize(w, vector<int>(h,0));
    double threshold = 0.2;
    int mx = 0;
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            // -1 0 1
            // -2 0 2
            // -1 0 1
            int gx = -1 * a(getx(i - 1), gety(j - 1), 0) + a(getx(i + 1), gety(j - 1), 0)
                   + -2 * a(getx(i - 1), gety(j), 0) + 2 * a(getx(i + 1), gety(j), 0)
                   + -1 * a(getx(i - 1), gety(j - 1), 0) + a(getx(i + 1), gety(j + 1), 0);
            // -1 -2 -1
            // 0 0 0
            // 1 2 1
            int gy =  -1 * a(getx(i - 1), gety(j - 1), 0) + -2 * a(getx(i), gety(j - 1), 0) - a(getx(i + 1), gety(j - 1), 0)
                   +   1 * a(getx(i - 1), gety(j + 1), 0) +  2 * a(getx(i), gety(j + 1), 0) + a(getx(i + 1), gety(j + 1), 0);
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
            for(int k = 0; k < 3; k++)a(i, j, k) = val;
        }
    }
   
}

void Filter::frame(Image &image, const Image &frame_image) {
    Image frame_image_resized = frame_image;
    resize(frame_image_resized, image.width, image.height);
    int border_size = min(image.width, image.height) / 20;
    Image newImage = frame_image_resized;
    int inner_width = image.width - 2 * border_size;
    int inner_height = image.height - 2 * border_size;
    resize(image, inner_width, inner_height);
    for(int x = 0; x < inner_width; ++x) {
        for(int y = 0; y < inner_height; ++y) {
            newImage(x + border_size, y + border_size, 0) = image(x, y, 0);
            newImage(x + border_size, y + border_size, 1) = image(x, y, 1);
            newImage(x + border_size, y + border_size, 2) = image(x, y, 2);
        }
    }
    image = newImage;
}

void Filter::gray(Image & image) {
    for(int i=0; i < image.width; i++){
        for(int j=0; j < image.height; j++){
            int gry = 0.2126 * image(i,j,0) + 0.7152 * image(i,j,1) + 0.0722 * image(i,j,2);
            for(int c = 0; c < 3; c++)image(i,j,c) = gry;
        }
    }
}

void Filter::h_flip(Image &image){
    for(int i=0;i<image.width/2;i++){
        for(int j=0;j<image.height;j++){
            swap(image(i,j,0),image(image.width-i-1,j,0));
            swap(image(i,j,1),image(image.width-i-1,j,1));
            swap(image(i,j,2),image(image.width-i-1,j,2));   
        }
    }
}

void Filter::invert(Image &image) {
    for(int i = 0; i < image.width; ++i) {
        for(int j = 0; j < image.height; ++j) {
            image(i, j, 0) = 255 - image(i, j, 0);
            image(i, j, 1) = 255 - image(i, j, 1);
            image(i, j, 2) = 255 - image(i, j, 2);
        }
    }
}

void Filter::merge(Image & a, Image & b) {
    // assuming same size
    int x = min(a.width, b.width);// rte safe-guard
    int y = min(a.height, b.height);
    if(x != a.width || x != b.width || y != a.height || y != b.height) {
        cout << "images are not the same size \n";
        cout << "1. resize the smaller image to the size of the larger one \n";
        cout << "2. crop the larger image to the size of the smaller image \n";
        while(true) {

            int x; cin >> x;
            int w1, h1, w2, h2;
            w1 = a.width, h1 = a.height;
            w2 = b.width, h2 = b.height;
            bool z = 0; // z = 0 means first image is smaller
            if(w2 * h2 < w1 * h1) {
                z = 1;
            }
            if(x == 1) {
                resize(a, max(w1, w2), max(h1, h2));
                resize(b, max(w1, w2), max(h1, h2));
                break;
            }else if(x == 2){
                crop(a, 0, 0, min(w1, w2), min(h1, h2));
                crop(b, 0, 0, min(w1, w2), min(h1, h2));
                break;
            }else {
                cout << "invalid input please try again (input 1 or 2) \n";
            }
        }
    }
    for(int i = 0; i < a.width; i++) {
        for(int j = 0; j < a.height; j++) {
            for(int k = 0; k < 3; k++) {
                a(i, j, k) = (a(i, j, k) + b(i, j, k)) / 2;
            }
        }
    }
}

double color(Image &image,int i, int j, int newWidth, int newHeight, int c){
    double srci= i * image.width / newWidth;
    double srcj= j * image.height / newHeight;
            
    int x1 = srci, y1 = srcj, x2 = min(image.width-1,x1+1), y2 = min(image.height-1,y1+1);

    return (x2 - srci) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x1,y1,c)
                    + (srci - x1) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x2,y1,c)
                    + (x2 - srci) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x1,y2,c)
                    + (srci - x1) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x2,y2,c);
}

void Filter::resize(Image &image,int newWidth, int newHeight){
    Image newImage(newWidth,newHeight);
    for(int i=0; i < newWidth; i++){
        for(int j=0; j < newHeight; j++){

            double r = color(image, i, j, newWidth, newHeight, 0);
            double g = color(image, i, j, newWidth, newHeight, 1);
            double b = color(image, i, j, newWidth, newHeight, 2);

            newImage(i,j,0) = r;
            newImage(i,j,1) = g;
            newImage(i,j,2) = b;
        }
    }
    image = newImage;
}

void Filter::resize(Image &image,double ratioX, double ratioY){
    int newWidth = image.width * ratioX;
    int newHeight = image.height * ratioY;
    resize(image, newWidth, newHeight);
}

void Filter::rotate(Image &image, int degrees) {
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

void Filter::v_flip(Image &image){
    for(int j=0;j<image.height/2;j++){
        for(int i=0;i<image.width;i++){
            swap(image(i,j,0),image(i,image.height-j-1,0));
            swap(image(i,j,1),image(i,image.height-j-1,1));
            swap(image(i,j,2),image(i,image.height-j-1,2));   
        }
    }
}

void Filter::purple(Image &image){
    for(int i = 0; i < image.width; i++){
        for(int j = 0; j < image.height; j++){
            double brightness = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);
            double alpha = 1.0 - (brightness / 255.0);

            image(i, j, 0) = (1 - alpha) * image(i, j, 0) + alpha * 128;
            image(i, j, 1) = (1 - alpha) * image(i, j, 1) + alpha * 0;
            image(i, j, 2) = (1 - alpha) * image(i, j, 2) + alpha * 128;

            if(image(i, j, 0) < 0) image(i, j, 0) = 0;
            if(image(i, j, 0) > 255) image(i, j, 0) = 255;
            
            if(image(i, j, 1) < 0) image(i, j, 1) = 0;
            if(image(i, j, 1) > 255) image(i, j, 1) = 255;
            
            if(image(i, j, 2) < 0) image(i, j, 2) = 0;
            if(image(i, j, 2) > 255) image(i, j, 2) = 255;
            
        }
    }
}

void Filter::infrared(Image &image){
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

void Filter::wave(Image &image){ 
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


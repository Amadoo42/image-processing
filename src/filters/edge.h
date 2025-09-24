#include "blur.h"
#include "gray.h"

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
void edge(Image & a) {
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
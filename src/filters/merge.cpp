#include "../helpers/apply_filter.h"

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
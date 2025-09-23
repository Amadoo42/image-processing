void Crop(Image &image,int x,int y,int w,int h){
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
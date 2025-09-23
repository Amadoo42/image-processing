void Resize(Image &image,int newWidth, int newHeight){
    Image newImage(newWidth,newHeight);
    for(int i=0;i<newWidth;i++){
        for(int j=0;j<newHeight;j++){
            int srci= i * image.width / newWidth;
            int srcj= j * image.height / newHeight;
            newImage(i,j,0)=image(srci,srcj,0);
            newImage(i,j,1)=image(srci,srcj,1);
            newImage(i,j,2)=image(srci,srcj,2);
        }
    }
    image=newImage;
}
void Resize(Image &image,double ratioX, double ratioY){
    int newWidth=image.width * ratioX;
    int newHeight=image.height * ratioY;
    Image newImage(newWidth,newHeight);
    for(int i=0;i<newWidth;i++){
        for(int j=0;j<newHeight;j++){
            int srci= i * image.width / newWidth;
            int srcj= j * image.height / newHeight;
            newImage(i,j,0)=image(srci,srcj,0);
            newImage(i,j,1)=image(srci,srcj,1);
            newImage(i,j,2)=image(srci,srcj,2);
        }
    }
    image=newImage;
}
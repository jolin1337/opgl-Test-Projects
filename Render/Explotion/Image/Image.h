#ifndef __IMG_DATA__
#define __IMG_DATA__
#include <stdlib.h>
#include <stdio.h>
#include <string>

// jpeg decode
#include <jpeglib.h>
#include <png.h>
#include <setjmp.h>

#define JPEG_TYPE 1
#define PNG_TYPE 2
#define UNKOWN_TYPE 43

class Image{
	unsigned char *pxls;
	int width, height
	    , type;
	char *name;
	bool loadFailed;
	unsigned char color,
		      depth;
public:
	Image(){loadFailed = true;}
	Image(char *Name){
		name = Name;
		loadFailed = true;
		readImage();
	}
	Image(std::string Name){
		name = (char *)Name.c_str(); 
		loadFailed = true;
		readImage();

	}
	Image(const Image& i){ 
		width = i.width;
		height = i.height;
		name = i.name;
		loadFailed = i.loadFailed;
		if(loadFailed) return;
		pxls = new unsigned char[width*height*4];	
		for(int x = 0; x < width; x++)
			for(int y = 0; y < height; y++)
				pxls[y*width + x] = i.pxls[y*width + x];
	}
	Image & operator=(const Image &i){
		if(!loadFailed) delete []pxls;
		width = i.width;
		height = i.height;
		name = i.name;
		loadFailed = i.loadFailed;
		if(loadFailed) return *this;
		pxls = new unsigned char[width*height*4];	
		for(int x = 0; x < width; x++)
			for(int y = 0; y < height; y++)
				pxls[y*width + x] = i.pxls[y*width + x];
		return *this;
	}
	~Image(){
		deleteImage();
	}
	void deleteImage(){
		if(!loadFailed)
			delete []pxls;	
		loadFailed = true;
	}
	bool readImage();
	bool readImage(std::string src){
		name = (char *)src.c_str(); 
		loadFailed = true;
		return readImage();
	}
	bool readJPEG();
	bool readPNG();
	bool isPngFile(char *fileName);

	int getWidth() const {
		return width;
	}
	int getHeight() const {
		return height;
	}
	unsigned char getBlueColor(int x,int y){
		if(isInBounds(x,y))
			return pxls[4*y*width + x*4];
		else
			return 0;
	}
	unsigned char getGreenColor(int x,int y){
		if(isInBounds(x,y))
			return pxls[4*y*width + x*4+1];
		else
			return 0;
	}
	unsigned char getRedColor(int x,int y){
		if(isInBounds(x,y))
			return pxls[4*y*width + x*4+2];
		else
			return 0;
	}
	unsigned char getAlphaColor(int x,int y){
		if(isInBounds(x,y))
			return pxls[4*y*width + x*4+3];
		else
			return 0;
	}
	bool isInBounds(int x, int y){
		return 4*y*width + 4*x < width*height*4 - 4;
	}
	bool isOpen(){
		return !loadFailed;
	}
	int getType(){
		return type;
	}

	/** USE at own risk! **/
	unsigned char *getPixlArray(){
		return pxls;
	}
};

#endif

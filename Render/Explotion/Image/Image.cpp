#include "Image.h"

bool Image::readImage(){
	if(!loadFailed)
		delete []pxls;
	loadFailed = true;

	/*FILE *infile = fopen(name, "rb");
	if(!infile){
		loadFailed = true;
		fprintf(stderr, "Can't open %s\n", name);
		return true;
	}

	unsigned char header[8];
	fread(header, 1, 8, infile);
	fclose(infile);
	if(!png_sig_cmp(header, 0, 8))
		return readPNG();
	else //if(!jpeg_sig_cmp(header, 0, 8))
		return readJPEG();
	*/
	//if(readPNG())
		readJPEG();
}
bool Image::isPngFile(char *fileName){
	png_byte buf[4];
	FILE *fp;
	if((fp = fopen(fileName, "rb")) != NULL)
		return 0;
	if(fread(buf, 1, 4, fp) != 4)
		return 0;
	fclose(fp);
	return (png_check_sig(buf, 4));
}
bool Image::readPNG(){
	/*fread(header, 0, 8, infile);
	if(png_sig_cmp(header, 0, 4)){*/
	if(!isPngFile(name)){
		loadFailed = true;
		fprintf(stderr, "Can't recognize the format of the image %s\n", name);
		return true;
	}

	int row_stride;
	unsigned char header[4],
		      *pDummy, *pTest;
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	
	FILE *infile;
	if(!(infile = fopen(name, "rb"))){
		loadFailed = true;
		fprintf(stderr, "Can't open %s\n", name);
		return true;
	}

	
	//							erpt, erfn, warnfn
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		fclose(infile);
		loadFailed = true;
		fprintf(stderr, "Structure of png %s unkown.\n", name);
		return true;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(infile);
		loadFailed = true;
		fprintf(stderr, "Structure of png %s unkown.\n", name);
		return true;
	}
	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(infile);
		loadFailed = true;
		fprintf(stderr, "Error during initial io from: %s.\n", name);
		return true;
	}

	png_init_io(png_ptr, infile);
	png_set_sig_bytes(png_ptr, 0);

	png_read_info(png_ptr, info_ptr);

	//png_getIHDR(png_ptr, info_ptr, &width, &height, &depth, &color, &number_of_passes, NULL, NULL);
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color = png_get_color_type(png_ptr, info_ptr);
	depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);

	png_set_strip_16(png_ptr);	// strip 16bit images to 8 bit
	png_set_strip_alpha(png_ptr);   // strip alpha to not combine with background (not recomended)
	png_set_packing(png_ptr); 	// separate depths (usefull for grayscale images
	png_set_packswap(png_ptr);	// change order of packed pixels to lsb first

	if(color == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);
	if(color == PNG_COLOR_TYPE_GRAY && depth < 8)
		png_set_expand(png_ptr);
	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);
	
	png_read_update_info(png_ptr, info_ptr);

	if(setjmp(png_jmpbuf(png_ptr))){
		loadFailed = true;
		fprintf(stderr, "Error during initial io from: %s. \n", name);
		return true;
	}


	png_bytep row_pointers[height];
	pDummy = pTest = new unsigned char[width*height*4];
	if(!pDummy){
		loadFailed = true;
		printf("NO MEM FOR CONVERTION!\n");
		return true;
	}
	loadFailed = false;

	for(int i=0;i<height;i++)
		row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
	png_read_image(png_ptr, row_pointers);
	fclose(infile);
	
	for(int y=0;y<height;y++){
		png_byte *row = row_pointers[y];
		for(int x=0;x < width*4;x++)
			pDummy[y*width + x] = row[x];
	}
	for(int i=0; i < height; i++)
		free(row_pointers[i]);
	free(row_pointers);
	return false;
}
bool Image::readJPEG(){
	FILE *infile = fopen(name, "rb");
	if(infile < 0){
		loadFailed = true;
		fprintf(stderr, "Can't open %s\n", name);
		return true;
	}
	
	unsigned char a,r,g,b,
		      *pDummy, *pTest;
	JSAMPARRAY pJpegBuffer;
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	(void) jpeg_read_header(&cinfo, TRUE);
	(void) jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;
	height = cinfo.output_height;

	pDummy = pTest = new unsigned char[width*height*4];

	row_stride = width * cinfo.output_components;
	pJpegBuffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);	// wtf!?!?
	while(cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
		for(int x = 0; x < width; x++){
			a = 255;
			r = pJpegBuffer[0][cinfo.output_components*x+0];
			if(cinfo.output_components > 2){
				g = pJpegBuffer[0][cinfo.output_components*x+1];
				b = pJpegBuffer[0][cinfo.output_components*x+2];
			}
			else{
				g = b = r;
			}
			*(pDummy++) = b;
			*(pDummy++) = g;
			*(pDummy++) = r;
			*(pDummy++) = a;
		}
	}

	fclose(infile);
	(void)jpeg_finish_decompress(&cinfo);
	pxls = pTest;
	loadFailed = false;
	jpeg_destroy_decompress(&cinfo);
	/*BMap = (int*)pTest;
	Height = height;
	Width = width;
	Depht = 32;
	*/
	return false;
}

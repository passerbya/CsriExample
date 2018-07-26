// note: csri output is only BGR, no alpha channel.
// alpha effect is only for background

#include "csri/csri.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef USE_RWIMG
#include "writeimage.h"
#endif

typedef struct image_s {
	int width, height, stride;
	unsigned char *buffer;      // RGB24
} image_t;

static void flip(image_t * frame)
{

	//unsigned short bpp = Data->Info.bmiHeader.biBitCount;
	unsigned int width = abs(frame->width);
	unsigned int height = abs(frame->height);
	unsigned int stride = abs(frame->stride);

	unsigned char* buffer = (unsigned char*)malloc(stride);

	unsigned char *cur = frame->buffer;

	for (unsigned int i = 0; i <= height / 2; i++)
	{
		memcpy(buffer, cur + i * stride, stride);
		memcpy(cur + i * stride, cur + (height - 1 - i)*stride, stride);
		memcpy(cur + (height - 1 - i)*stride, buffer, stride);
	}
	free(buffer);
}

void WriteBMP(const char* filename, image_t *img)
{
	// flip(img);
	int w = img->width;
	int h = img->height;
	int l = (w * 3 + 3) / 4 * 4;
	int bmi[] = { l*h + 54,0,54,40,w,h,1 | 3 * 8 << 16,0,l*h,0,0,100,0 };
	FILE *fp = fopen(filename, "wb");
	fprintf(fp, "BM");
	fwrite(&bmi, 52, 1, fp);
	fwrite(img->buffer, 1, l*h, fp);
	fclose(fp);
}

void list_render(csri_rend* cur)
{
	do
	{
		puts(csri_renderer_info(cur)->name);
		cur = csri_renderer_next(cur);
	}
	while(cur);
}

int main(int argc, const char* argv[])
{
	if(argc < 2)
	{
		printf("usage: %s file.ass",argv[0]);
		return -1;
	}
	const char *filename = argv[1];

	int width = 1280;
	int height = 720;
	// 2.0s
	double frame_time = 2.0;
	
	int byte_size = width * height * 4;

	csri_inst * csriInstance = NULL;
	csri_rend * csriRender = csri_renderer_default();

	csriInstance = csri_open_file(csriRender, filename, NULL);
	
	if(!csriInstance)
	{
		puts("csri_open_file error.");
		return -1;
	}

	struct csri_frame current_frame;
	memset(&current_frame, 0, sizeof(current_frame));
	current_frame.pixfmt = CSRI_F_BGR_;
	
	unsigned char* canvas = NULL;
	canvas = malloc(byte_size);
	
	//background black
	memset(canvas, 0, byte_size);

#ifdef USE_RWIMG
	current_frame.planes[0] = canvas;
	current_frame.strides[0] = width * 4;
#else
	// BMP need flip
	current_frame.planes[0] = canvas + (height - 1) * width * 4;
	current_frame.strides[0] = -(signed)width * 4;
#endif
	struct csri_fmt fmt = { current_frame.pixfmt , width, height };
	int state = csri_request_fmt(csriInstance, &fmt);
	csri_render(csriInstance, &current_frame, frame_time);

	for (int i = 0; i < byte_size; i += 4)
	{
		// B
		// canvas[i + 0] = canvas[i + 0];
		// G
		// canvas[i + 1] = canvas[i + 1];
		// R 
		// canvas[i + 2] = canvas[i + 2];
		// A
		canvas[i + 3] = 255;
	}

	image_t *img = malloc(sizeof(image_t));
	img->width = width;
	img->height = height;
	img->stride = width * 3;
	img->buffer = (unsigned char *)calloc(1, height * width * 4);
	
#ifdef USE_RWIMG
	// BGR_ => RGB
	for (int index = 0; index < height*width; ++index)
	{
		img->buffer[index * 3] = canvas[index * 4 + 2];
		img->buffer[index * 3 + 1] = canvas[index * 4 + 1];
		img->buffer[index * 3 + 2] = canvas[index * 4 + 0];
	}
	write_image("test.png", width, height, img->buffer, 3, width * 3, IMAGE_FORMAT_AUTO);
	free(img->buffer);
#else
	// BGR_ => BGR
	for (int index = 0; index < height*width; ++index)
	{
		img->buffer[index * 3] = canvas[index * 4 + 0];
		img->buffer[index * 3 + 1] = canvas[index * 4 + 1];
		img->buffer[index * 3 + 2] = canvas[index * 4 + 2];
	}
	WriteBMP("test.bmp", img);
	free(img->buffer);
#endif
	//
	list_render(csriRender);
	csri_close(csriInstance);
}
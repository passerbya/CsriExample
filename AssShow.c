/*
* FileName: AssShow.c
* Usage:    set PATH=bin;%PATH% && tcc -Isrc -Lbin -lcsri -luser32 -lgdi32 -lshell32 -run AssShow.c
* cl /Isrc AssShow.c user32.lib gdi32.lib shell32.lib lib\libcsri.lib
*/

#include <windows.h>
#include <stdlib.h>
#include <shellapi.h>
//

#include "csri/csri.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef USE_RWIMG
#include "writeimage.h"
#endif

csri_inst *csriInstance = NULL;
csri_rend *csriRender = NULL;

// const int width = 640;
// const int height = 360;
// 2.0s
double frame_time = 0;
int fps = 10;
struct csri_frame current_frame;

void renderSubtitle(unsigned char *buffer, int width, int height)
{
	if (!csriInstance)
	{
		puts("csri_open_file error.");
		return ;
	}
	
	const int byte_size = width * height * 4;

	unsigned char *canvas = NULL;
	canvas = malloc(byte_size);

	// background black
	// memset(canvas, 0, byte_size);
	// gen chess block
	int block_size = 32;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int index = y * width * 4 + x * 4;
			int r, g, b;
			if (((y / block_size) & 1) != ((x / block_size) & 1))
			{
				r = 192;
				g = 192;
				b = 192;
			}
			else
			{
				r = 128;
				g = 128;
				b = 128;
			}
			canvas[index + 0] = b;
			canvas[index + 1] = g;
			canvas[index + 2] = r;
			canvas[index + 3] = 128;
		}
	}
	
	// BMP need flip
	current_frame.planes[0] = canvas + (height - 1) * width * 4;
	current_frame.strides[0] = -(signed)width * 4;
	struct csri_fmt fmt = {current_frame.pixfmt, width, height};
	int state = csri_request_fmt(csriInstance, &fmt);

	csri_render(csriInstance, &current_frame, frame_time);

	for (int i = 0; i < byte_size; i += 4)
	{
		// A
		canvas[i + 3] = 255;
	}
	// BGR_ => BGR
	for (int index = 0; index < height * width; ++index)
	{
		buffer[index * 3] = canvas[index * 4 + 0];
		buffer[index * 3 + 1] = canvas[index * 4 + 1];
		buffer[index * 3 + 2] = canvas[index * 4 + 2];
	}
	free(canvas);
}
//
typedef unsigned char byte;

typedef struct
{
	int Width;
	int Height;
	byte *Data;
} Image;

void OnDropFiles(HWND hwnd, HDROP hDropInfo)
{
	UINT nFileCount = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH] = "";
	DWORD dwAttribute;

	// 获取拖拽进来文件和文件夹
	for (UINT i = 0; i < nFileCount; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
		dwAttribute = GetFileAttributes(szFileName);

		// 是否为文件夹
		if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY)
		{
			// SetCurrentDirectory(szFileName);
			// EnumerateFiles();
		}
		else
		{
			KillTimer(hwnd, 1);
			frame_time = 0;
			// 文件可以直接进行播放
			csriInstance = csri_open_file(csriRender, szFileName, NULL);
			
			memset(&current_frame, 0, sizeof(current_frame));
			current_frame.pixfmt = CSRI_F_BGR_;
			// cout << szFileName << endl;
			// MessageBox(0, szFileName, "", MB_OK);
			SetTimer(hwnd, 1, 1000/fps, 0);
		}
	}

	DragFinish(hDropInfo);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	HDC hdc;
	RECT rect;
	Image img;
	BITMAPINFO bmi;
	PAINTSTRUCT ps;
	int iRowLength;
	//
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	//
	switch (msg)
	{
	case WM_CREATE:
	{
		csriInstance = NULL;
		csriRender = csri_renderer_default();
	}
	case WM_DROPFILES:
		OnDropFiles(hwnd, (HDROP)w);
		break;
	case WM_DESTROY:
		PostQuitMessage(1);
		break;
	case WM_KEYDOWN:
		if(w == VK_LEFT)
		{
			frame_time -= 10.0;
		}
		else if(w == VK_RIGHT)
		{
			frame_time += 10.0;
		}	
		break;
	case WM_TIMER: 
		hdc = GetDC(hwnd);
		//
		frame_time += 1.0/fps;
		// puts("paint");
		//
		GetClientRect(hwnd, &rect);
		img.Width = rect.right - rect.left;
		iRowLength = ((img.Width * 8 * 3 + 31) & ~31) >> 3;
		img.Height = rect.bottom - rect.top;
		img.Data = (byte *)malloc(iRowLength * img.Height);
		renderSubtitle(img.Data,img.Width,img.Height);
		bmi.bmiHeader.biWidth = img.Width;
		bmi.bmiHeader.biHeight = img.Height;
		SetDIBitsToDevice(hdc, 0, 0, img.Width, img.Height,
						  0, 0, 0, img.Height, img.Data, &bmi, DIB_RGB_COLORS);
	    free(img.Data);
		char buffer[BUFSIZ];
		sprintf(buffer,"%.3fs",frame_time);
		SetWindowText(hwnd,buffer);
		break;
	default:
		return DefWindowProc(hwnd, msg, w, l);
	}
	return 0;
}

BOOL CenterWindow(HWND hwnd)
{
    HWND hwndParent;
    RECT rect, rectP;
    int width, height;      
    int screenwidth, screenheight;
    int x, y;
 
    //make the window relative to its parent
    hwndParent = GetDesktopWindow();    
     
    GetWindowRect(hwnd, &rect);
    GetWindowRect(hwndParent, &rectP);
     
    width  = rect.right  - rect.left;
    height = rect.bottom - rect.top;    
     
    x = ((rectP.right-rectP.left) -  width) / 2 + rectP.left;
    y = ((rectP.bottom-rectP.top) - height) / 2 + rectP.top;    
 
    screenwidth  = GetSystemMetrics(SM_CXSCREEN);
    screenheight = GetSystemMetrics(SM_CYSCREEN);
 
    //make sure that the dialog box never moves outside of//the screen
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x + width  > screenwidth)  x = screenwidth  - width;
    if(y + height > screenheight) y = screenheight - height;    
 
    MoveWindow(hwnd, x, y, width, height, FALSE);
        return TRUE;
}

int main(int argc, char *argv[])
{
	static TCHAR szAppName[] = TEXT("RandColor");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	int iCmdShow = 1;
	HINSTANCE hInstance = NULL;
	//
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_ACCEPTFILES,
						  szAppName, TEXT("AssShow--Drag ass file to this window, Left-10s, Right+10s"), WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
						  CW_USEDEFAULT, CW_USEDEFAULT, 1280*0.75, 720*0.75,
						  NULL, NULL, hInstance, NULL);
	//
	CenterWindow(hwnd);
	//The message loop.
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

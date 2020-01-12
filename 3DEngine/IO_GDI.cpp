#include "IO.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;
using namespace std;

static ULONG_PTR gdiplusToken;
static CLSID bmpClassID;

void IO::InitGdiPlus() {

	cout << "Initializing GDI" << std::endl;

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UINT  num;
	UINT  size;

	GetImageEncodersSize(&num, &size);
	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, L"image/bmp") == 0) {
			bmpClassID = pImageCodecInfo[j].Clsid;
		}
	}

	free(pImageCodecInfo);

}

void IO::CreateBMPFrom(string& badImage) {

	int dotIndex = badImage.find(".");
	string filetype = badImage.substr(dotIndex, badImage.size());

	string filename = badImage.substr(0, dotIndex);

	wstring wfilepath(badImage.begin(), badImage.end());
	wstring wnewFileName(filename.begin(), filename.end());
	wnewFileName += L".bmp";

	Image* image = new Image(wfilepath.c_str());

	Gdiplus::EncoderParameters params;
	ULONG colorDeph = 32;
	params.Count = 1;
	params.Parameter[0].Guid = EncoderColorDepth;
	params.Parameter[0].Type = EncoderParameterValueTypeLong;
	params.Parameter[0].NumberOfValues = 1;
	params.Parameter[0].Value = &colorDeph;

	Status a = image->Save(wnewFileName.c_str(), &bmpClassID, &params);

	delete image;

	badImage = filename + ".bmp";

	cout << "Creating Image: " << badImage << std::endl;

}

void IO::UnInitGdiPlus() {
	GdiplusShutdown(gdiplusToken);
}
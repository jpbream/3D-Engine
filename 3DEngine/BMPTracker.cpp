#include "BMPTracker.h"
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <iostream>
#include <cstdio>

using namespace Gdiplus;
using namespace std;

vector<string> createdFiles;
ULONG_PTR gdiplusToken;
CLSID bmpClassID;

void BMPTracker::InitGDIPlus() {

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

void BMPTracker::CreateBMPFrom(string& badImage) {

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
	createdFiles.push_back(string(badImage));

	cout << "Creating Image: " << badImage << std::endl;

}

void BMPTracker::UnInit() {
	GdiplusShutdown(gdiplusToken);

	for (int i = 0; i < createdFiles.size(); ++i) {
		cout << "Deleting File: " << createdFiles[i] << std::endl;
		remove(createdFiles[i].c_str());
	}
	createdFiles.clear();

}


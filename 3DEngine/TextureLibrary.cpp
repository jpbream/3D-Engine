#include "TextureLibrary.h"
#include "Texture.h"
#include <iostream>
#include <atlstr.h>
#include <atlimage.h>
#include "BMPTracker.h"

namespace TextureLibrary {

	void InitNewTexture(string filename, Texture* tex) {

		string filetype = filename.substr(filename.find("."), filename.size());
		if (filetype != ".bmp") {

			BMPTracker::CreateBMPFrom(filename);

		}

		filenames.push_back(filename);
		tex->ID = filenames.size() - 1;

	}

	void Load(Texture* tex) {

		if (!(tex->IsNull()) && tex->ID < filenames.size()) {

			if (tex->texture != nullptr) {
				return;
			}

			CImage cimg;
			cimg.Load(filenames[tex->ID].c_str());

			tex->width = cimg.GetWidth();
			tex->height = cimg.GetHeight();

			int pitch = cimg.GetPitch();
			if (pitch < 0) {
				tex->bottomUp = true;
				pitch *= -1;
			}
			else {
				tex->bottomUp = false;
			}
			tex->pitch = pitch;
			tex->bpp = cimg.GetBPP();

			int size = tex->height * pitch;
			unsigned char* start = nullptr;

			if (tex->bottomUp) {
				start = (unsigned char*)cimg.GetPixelAddress(0, tex->height - 1);
			}
			else {
				start = (unsigned char*)cimg.GetPixelAddress(0, 0);
			}

			std::cout << "Allocating " << size / 1000000.0 << " megabytes for texture" << std::endl;
			tex->texture = (unsigned char*)malloc(size);
			memcpy(tex->texture, start, size);

			cimg.Destroy();
			
		}
	}

	void Unload(Texture* tex) {
		
		if (tex->texture != nullptr) {
			free(tex->texture);
			tex->texture = nullptr;
		}
	}

}
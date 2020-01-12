#include "JoeyRender.h"
#include <iostream>
#include "Window.h"
#include "Time.h"
#include "Surface.h"
#include "Display.h"
#include "Mat.h"
#include "Model.h"
#include "Entity.h"
#include "Renderer.h"
#include "IO.h"
#include "Texture.h"
#include "CubeMap.h"
#include "PointLight.h"

int main(int argc, char* args[]) {

	double t;
	
	JoeyRenderInit();

	int num = SDL_GetNumDisplayModes(0);
	std::cout << num << " display modes\n";

	bool diagnostics = false;

	double fraction = 1;

	int width = Display::GetWeightedWidth(fraction);
	int height = Display::GetWeightedHeight(fraction);
	
	std::cout << "Window Size: " << width << " " << height << std::endl;

	Window window("Joey Render", 0, 60, width, height, false);
	window.ShowImage("LoadScreen.bmp");

	Surface surface(width, height);
	Renderer renderer(&surface);
	renderer.ambientIntensity = 1;

	Model cowModel = IO::LoadObj("Cow2.obj");
	Model planeModel = IO::LoadObj("Plane.obj");
	Model cubeMapModel = IO::LoadObj("CubeMap.obj");

	Entity basicCow(&cowModel);
	Entity shinyCow(&cowModel);

	Entity rockPlane(&planeModel);
	Entity texturePlane(&planeModel);

	CubeMap cubeMap(&cubeMapModel);

	shinyCow.cubeMap = &cubeMap;
	shinyCow.shiny = true;
	
	basicCow.position.z = -10;
	basicCow.position.x = -6;

	shinyCow.position.z = -10;
	shinyCow.position.x = 6;

	texturePlane.position.z = -40;
	texturePlane.rotation.z = 180;

	rockPlane.rotation.x = -90;
	rockPlane.position.y = -4;

	DirectionalLight light;
	light.color = Color(1, 1, 1);
	light.rotation.x = -30;
	renderer.directionalLights.push_back(&light);

	Camera camera;
	camera.fov = 110;
	renderer.camera = &camera;

	camera.position.x = 8;
	camera.rotation.y = -30;
	camera.position.y = 7;
	camera.rotation.x = -45;

	basicCow.ApplyTransform();
	shinyCow.ApplyTransform();
	rockPlane.ApplyTransform();
	cubeMap.ApplyTransform();

	renderer.DrawEntity(&cubeMap, nullptr, nullptr);
	renderer.DrawEntity(&basicCow, nullptr, nullptr);
	renderer.DrawEntity(&shinyCow, nullptr, nullptr);
	renderer.DrawEntity(&rockPlane, nullptr, nullptr);
	
	renderer.EndDraw();

	Texture dynamicTexture(surface.GetWidth(), surface.GetHeight());
	surface.CopyToTexture(&dynamicTexture, 0, 0);

	surface.ClearBuffer();

	camera.position.x = 0;
	camera.position.y = 3;
	camera.rotation.x = -10;
	camera.rotation.y = 0;

	texturePlane.ApplyTransform();

	renderer.DrawEntity(&cubeMap, nullptr, nullptr);
	renderer.DrawEntity(&basicCow, nullptr, nullptr);
	renderer.DrawEntity(&shinyCow, nullptr, nullptr);
	renderer.DrawEntity(&rockPlane, nullptr, nullptr);

	planeModel.materials[0].diffuseTexture.MoveFrom(dynamicTexture);
	planeModel.materials[0].normalMap.Free();
	renderer.DrawEntity(&texturePlane, nullptr, nullptr);

	renderer.EndDraw();

	surface.CopyToWindow(&window);
	window.Update();
	//surface.SaveToFile("final.bmp");

	surface.ClearBuffer();

	SDL_Event e;
	bool quit = false;

	int x, y;

	while (!quit) {

		while (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					break;
				}
			}
		}

		SDL_GetMouseState(&x, &y);

	}

	JoeyRenderQuit();
	return 0;
}
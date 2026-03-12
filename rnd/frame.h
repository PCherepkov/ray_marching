#pragma once

#include <windows.h>
#include <thread>

#include "../def.h"
#include "scene.h"
#include "camera.h"


class frame
{
public:
	HBITMAP hDib;
	int W = 0, H = 0;
	DWORD* Image;
	BITMAPINFOHEADER bmih;
	scene Sc;
	camera Cam;

	frame()
	{
	}

	frame(int W, int H)
	{
		// two spheres
		// sphere* Sph1 = new sphere(1.0, vec3(-1, 0, 0));
		// sphere* Sph2 = new sphere(1.0, vec3(1, 0, 0));
		// Sc.AddShape((shape*)Sph1);
		// Sc.AddShape((shape*)Sph2);
		// Sph1->Color = color(204, 255, 102); // color(255, 102, 102);
		// Sph2->Color = color(204, 255, 102);

		// dumpling
		torus* Torus1 = new torus(vec2(1.2, 0.15), vec3(0, 1, 0));
		sphere* Sph1 = new sphere(1.0, vec3(0, 1, 0));
		Sc.AddShape((shape*)Sph1);
		Sc.AddShape((shape*)Torus1);
		Sph1->Color = color(255, 102, 102);
		Torus1->Color = color(204, 255, 102);

		// droplet
		// sphere* Sph = new sphere(1.0, vec3(0, 1, 0));
		// Sph->Color = color(255, 102, 102);
		// plane* Plane = new plane(vec3(0, 1, 0), vec3(0));
		// Plane->Color = color(204, 255, 102);
		// Sc.AddShape((shape*)Sph);
		// Sc.AddShape((shape*)Plane);

		vec3 Pos = vec3(4, 8, -8);
		Cam.SetProj(W, H).SetView(Pos,
								  vec3(0, 0, 0),
								  vec3(0, 1, 0));

		Image = (DWORD*)malloc(sizeof(DWORD*) * W * H);
		memset(Image, 0, sizeof(DWORD*) * W * H);
	}

	void PutPixel(int X, int Y, DWORD Color)
	{
		if (X < W && X > -1 && Y < H && Y > -1)
			Image[Y * W + X] = Color;
	}

	void PutPixel(int X, int Y, color Color)
	{
		if (X < W && X > -1 && Y < H && Y > -1)
			Image[Y * W + X] = Color.DwordColor();
	}

	void PutPixel(int X, int Y, byte r, byte g, byte b, byte a)
	{
		DWORD Color = (((((r << 8) + g) << 8) + b) << 8) + a;
		if (X < W && X > -1 && Y < H && Y > -1)
			Image[Y * W + X] = Color;
	}

	~frame()
	{
		free(Image);
	}

	static void ThreadRender(frame* F, int ind, int num)
	{
		camera Cam = F->Cam;
		int W = F->W, H = F->H;

		for (int y = 0; y < H; y++)
			for (int x = ind; x < W; x += num)
			{
				color ResColor = F->Sc.March(Cam.MakeRay(x, y, 0.25, 0.25));
				ResColor = ResColor + (F->Sc.March(Cam.MakeRay(x, y, 0.75, 0.25)));
				ResColor = ResColor + (F->Sc.March(Cam.MakeRay(x, y, 0.25, 0.75)));
				ResColor = ResColor + (F->Sc.March(Cam.MakeRay(x, y, 0.75, 0.75)));
				ResColor = ResColor / 4.f;

				F->PutPixel(x, y, ResColor.clamp());
			}
	}

	void Render(void)
	{
		int n = std::thread::hardware_concurrency();
		std::vector<std::thread> Ths(n);

		// move around
		Sc.objects[0]->pos.y = sin(clock() / 1000.f);
		// Sc.objects[1]->pos.x = -sin(clock() / 1000.f) * 1.5;

		for (int i = 0; i < n; i++)
			Ths[i] = std::thread(ThreadRender, this, i, n);

		for (int i = 0; i < n; i++)
			Ths[i].join();
	}
};
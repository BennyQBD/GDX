#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <string>
#include "math3d.h"

namespace Display
{
    bool Init(int width = 800, int height = 600, const std::string& title = "3D Engine");
    void Destroy();

    void SetMousePos(const Vector2f& pos);
	void SetTitle(const std::string& title);
	//void SetIcon(const std::string& fileName);
	void SetClearColor(float r, float g, float b, float a);

	void ShowMouse(bool show);
	void SwapBuffers();
	void ClearScreenAndDepth();
	void ClearScreen();
	void ClearDepth();

	void Error(const std::string& message);

	Vector2f GetSize();
	Vector2f GetCenter();
	//bool IsResized();
};

#endif // DISPLAY_H_INCLUDED

#pragma once
#include "../include/Core.h"
#include "../include/Colors.h"

typedef unsigned char Color;

class Paint : public ConsoleApp {
private:
	CHAR_INFO* canvas;
	Vec2 canvasSize;
	SMALL_RECT canvasRegion;

	bool updateCanvas = false;

	wchar_t glyph;
	Color fgColor;
	Color bgColor;

	enum class TOOL {
		Pencil, Line, Rectangle
	} tool;

	const int maxNumOfColors = 16;

	bool resetClickPos = true;
	Vec2 clickPos;
	Vec2 lastPos;

public:
	Paint();
	~Paint();


private:
	void OnCreate() {
		canvas = new CHAR_INFO[GetScreenWidth() * GetScreenHeight()];
		canvasSize = Vec2(GetScreenWidth(), GetScreenHeight());
		canvasRegion = { 0, 0, (short)canvasSize.x - 1, (short)canvasSize.y - 1 };
		ClearCanvas();
		updateCanvas = true;
	}

	void OnUpdate() {
		DrawCanvas();

		if (KeyPressed(VK_LBUTTON)) {
			if (resetClickPos) {
				clickPos = GetMousePosition();
				resetClickPos = false;
			}
			lastPos = GetMousePosition();

			if (tool == TOOL::Pencil)
				Draw(GetMousePosition(), glyph, fgColor | bgColor);
			if (tool == TOOL::Line)
				GhostLine(clickPos, lastPos, NULL, fgColor | bgColor);
			if (tool == TOOL::Rectangle)
				GhostRectangle(clickPos, lastPos, NULL, fgColor | bgColor);
		}
		if (KeyReleased(VK_LBUTTON))
			if (!resetClickPos) {
				if (tool == TOOL::Line)
					Line(clickPos, lastPos, NULL, fgColor | bgColor);
				if (tool == TOOL::Rectangle)
					Rectangle(clickPos, lastPos, NULL, fgColor | bgColor);
				resetClickPos = true;
				DrawCanvas();
			}

		if (KeyReleased('E'))
			SwitchFGColor();
		if (KeyReleased('Q'))
			SwitchBGColor();

		if (KeyReleased('P'))
			tool = TOOL::Pencil;
		if (KeyReleased('L'))
			tool = TOOL::Line;
		if (KeyReleased('R'))
			tool = TOOL::Rectangle;

		if (KeyPressed('C'))
			ClearCanvas();
	}

private:
	void DrawCanvas();

	void ChangeGlyph(wchar_t);
	void ChangeFGColor(Color);
	void ChangeBGColor(Color);

	void SwitchFGColor();
	void SwitchBGColor();

	bool inCanvas(int x, int y) const;
	bool inCanvas(Vec2) const;

	void Draw(int x, int y, wchar_t, Color);
	void Draw(Vec2, wchar_t, Color);
	void Line(Vec2, Vec2, wchar_t, Color);
	void GhostLine(Vec2, Vec2, wchar_t, Color);
	void Rectangle(Vec2, Vec2, wchar_t, Color);
	void GhostRectangle(Vec2, Vec2, wchar_t, Color);

	void ClearCanvas();
};
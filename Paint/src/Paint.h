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
		Pencil, Line, Rectangle, Ellipse, Bucket
	} tool;

	const int maxNumOfColors = 16;

	bool resetFirstClickPos = true;
	Vec2 firstClickPos;
	Vec2 lastClickPos;

	struct Panel {
		CHAR_INFO* buffer;
		Vec2 size;
		SMALL_RECT region;

		bool update = false;

		Color color;
		const std::wstring previewText = L"PREVIEW:";
		Vec2 previewTextPos;
		const std::wstring fgColorText = L"FG:";
		Vec2 fgColorTextPos;
		const std::wstring bgColorText = L"BG:";
		Vec2 bgColorTextPos;
		const std::wstring toolText = L"TOOL:";
		std::wstring toolName;
		Vec2 toolTextPos;

		Panel();
		~Panel();

		void DrawPanel(wchar_t glyph, Color fgColor, Color bgColor, TOOL tool);
	} panel;

public:
	Paint();
	~Paint();


private:
	void OnCreate() {
		// Canvas init
		canvas = new CHAR_INFO[GetScreenWidth() * GetScreenHeight()];
		canvasSize = Vec2(GetScreenWidth(), GetScreenHeight() - 1);
		canvasRegion = { 0, 0, (short)GetScreenWidth() - 1, (short)GetScreenHeight() - 2 };
		ClearCanvas();
		updateCanvas = true;

		// Panel init
		panel.buffer = new CHAR_INFO[GetScreenWidth() * 1];
		panel.size = Vec2(GetScreenWidth(), 1);
		panel.region = { 0, (short)GetScreenHeight() - 1, (short)GetScreenWidth() - 1, (short)GetScreenHeight() - 1 };
		
		panel.color = 0x0070;
		panel.previewTextPos = { 2, 0 };
		panel.fgColorTextPos = { 19, 0 };
		panel.bgColorTextPos = { 13, 0 };
		panel.toolTextPos = { 25, 0 };

		panel.DrawPanel(glyph, fgColor, bgColor, tool);
		panel.update = true;
	}

	void OnUpdate() {
		DrawVisualElements();

		if (KeyPressed(VK_LBUTTON)) {
			if (resetFirstClickPos) {
				firstClickPos = GetMousePosition();
				resetFirstClickPos = false;
			}
			if (inCanvas(GetMousePosition())) {
				lastClickPos = GetMousePosition();

				if (tool == TOOL::Pencil)
					Draw(GetMousePosition(), glyph, fgColor | bgColor);
				if (tool == TOOL::Line)
					GhostLine(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
				if (tool == TOOL::Rectangle)
					GhostRectangle(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
				if (tool == TOOL::Ellipse)
					GhostEllipse(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
				if (tool == TOOL::Bucket)
					Bucket(GetMousePosition(), glyph, fgColor | bgColor);
			}
			else
				resetFirstClickPos = true;
		}
		if (KeyReleased(VK_LBUTTON))
			if (!resetFirstClickPos) {
				if (tool == TOOL::Line)
					Line(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
				if (tool == TOOL::Rectangle)
					Rectangle(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
				if (tool == TOOL::Ellipse)
					Ellipse(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
				resetFirstClickPos = true;
				DrawVisualElements();
			}

		if (KeyReleased(VK_OEM_6))
			NextFGColor();
		if (KeyReleased(VK_OEM_4))
			PreviousFGColor();
		if (KeyReleased(VK_OEM_7))
			NextBGColor();
		if (KeyReleased(VK_OEM_1))
			PreviousBGColor();

		if (KeyReleased('P'))
			ChangeTool(TOOL::Pencil);
		if (KeyReleased('L'))
			ChangeTool(TOOL::Line);
		if (KeyReleased('R'))
			ChangeTool(TOOL::Rectangle);
		if (KeyReleased('E'))
			ChangeTool(TOOL::Ellipse);
		if (KeyReleased('B'))
			ChangeTool(TOOL::Bucket);

		if (KeyPressed('C'))
			ClearCanvas();

		if (KeyReleased('Q'))
			Close();
	}

private:
	void DrawVisualElements();

	void ChangeGlyph(wchar_t);
	void ChangeFGColor(Color);
	void ChangeBGColor(Color);
	void ChangeTool(TOOL);

	void NextFGColor();
	void PreviousFGColor();
	void NextBGColor();
	void PreviousBGColor();

	bool inCanvas(int x, int y) const;
	bool inCanvas(Vec2) const;

	void Draw(int x, int y, wchar_t, Color);
	void Draw(Vec2, wchar_t, Color);
	void Line(Vec2, Vec2, wchar_t, Color);
	void GhostLine(Vec2, Vec2, wchar_t, Color);
	void Rectangle(Vec2, Vec2, wchar_t, Color);
	void GhostRectangle(Vec2, Vec2, wchar_t, Color);
	void Ellipse(Vec2, Vec2, wchar_t, Color);
	void GhostEllipse(Vec2, Vec2, wchar_t, Color);
	void Bucket(Vec2, wchar_t, Color);

	void ClearCanvas();
};
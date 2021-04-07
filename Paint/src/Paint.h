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

		if (KeyPressed('C'))
			ClearCanvas();
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

	void ClearCanvas();
};
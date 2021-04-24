#pragma once
#include "../include/Core.h"
#include "Canvas.h"
#include "Panels.h"

class Paint : public ConsoleApp {
	std::string imageName;
	const std::string savePath = "images/";
	const std::string imageExtension = ".uimg";

	Canvas canvas;

	ImagePanel imagePanel;
	InfoPanel infoPanel;
	ToolPanel toolPanel;
	GlyphPanel glyphPanel;
	PalettePanel palettePanel;

	InputPanel NewImageNamePanel;
	InputPanel SavedImageNamePanel;

	Glyph glyph;
	Color fgColor;
	Color bgColor;

	TOOL tool;

	Vec2 mousePosition;
	Vec2 originPoint;
	bool resetOriginPoint;

public:
	bool updateVisualElements;

public:
	Paint();

private:
	void OnCreate();
	void OnUpdate();

	void UpdateVisualElements();
	void DrawBlueprints();
	void CheckMouseInput();
	void CheckKeyboardInput();

public:
	void New();
	void Save();
	void Load(std::string image);

	void Undo();
	void Redo();

	void ChangeGlyph(Glyph glyph);
	void ChangeFGColor(Color color);
	void ChangeBGColor(Color color);
	void ChangeTool(TOOL tool);

	void ChangeImageName(std::string name);

	void NextFGColor();
	void PreviousFGColor();
	void NextBGColor();
	void PreviousBGColor();

	void ClearCanvas();
};
























///* ####### APP ####### */
//class s : public ConsoleApp {
//private:
//	bool resetFirstClickPos = true;
//	Vec2 firstClickPos;
//	Vec2 lastClickPos;
//
//
//private:
//	void OnCreate() {
//		// Panel init
//		/*imagePanel(L"IMAGE", Vec2(11, 9), { (short)GetScreenWidth() - 11, 0, (short)GetScreenWidth() - 1, 8 }, 0x0000 | 0x0070);*/
//		infoPanel.Init(L"INFO", Vec2(11, 8), { (short)GetScreenWidth() - 11, 9, (short)GetScreenWidth() - 1, 16 }, 0x0000 | 0x0070);
//		toolPanel.Init(L"TOOLS", Vec2(11, 17), { (short)GetScreenWidth() - 11, 17, (short)GetScreenWidth() - 1, 33 }, 0x0000 | 0x0070, tool);
//		/*panel3.Init(L"GLYPHS", Vec2(11, 20), { (short)GetScreenWidth() - 11, 34, (short)GetScreenWidth() - 1, 53 }, 0x0000 | 0x0070);*/
//		palettePanel.Init(L"PALETTE", Vec2(11, 6), { (short)GetScreenWidth() - 11, 54, (short)GetScreenWidth() - 1, 59 }, 0x0000 | 0x0070, fgColor, bgColor);
//
//		infoPanel.ChangeInfo(glyph, fgColor, bgColor);
//	}
//
//	void OnUpdate() {
//		if (KeyPressed(VK_LBUTTON)) {
//			if (resetFirstClickPos) {
//				firstClickPos = GetMousePosition();
//				resetFirstClickPos = false;
//			}
//			if (inCanvas(GetMousePosition())) {
//				lastClickPos = GetMousePosition();
//
//				if (tool == TOOL::Pencil)
//					Draw(GetMousePosition(), glyph, fgColor | bgColor);
//				if (tool == TOOL::Line)
//					GhostLine(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
//				if (tool == TOOL::Rectangle)
//					GhostRectangle(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
//				if (tool == TOOL::Ellipse)
//					GhostEllipse(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
//				if (tool == TOOL::Bucket)
//					Bucket(GetMousePosition(), glyph, fgColor | bgColor);
//			}
//			else
//				resetFirstClickPos = true;
//		}
//		if (KeyReleased(VK_LBUTTON)) {
//			if (!resetFirstClickPos) {
//				if (tool == TOOL::Line)
//					Line(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
//				if (tool == TOOL::Rectangle)
//					Rectangle(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
//				if (tool == TOOL::Ellipse)
//					Ellipse(firstClickPos, lastClickPos, glyph, fgColor | bgColor);
//				resetFirstClickPos = true;
//				DrawVisualElements();
//			}
//
//			if (palettePanel.inBounds(GetMousePosition())) {
//				palettePanel.SelectColor(GetMousePosition());
//				ChangeFGColor(palettePanel.selectedFGColor);
//				ChangeBGColor(palettePanel.selectedBGColor);
//			}
//			if (toolPanel.inBounds(GetMousePosition())) {
//				toolPanel.SelectTool(GetMousePosition());
//				ChangeTool(toolPanel.selectedTool);
//			}
//		}
//
//	}
//};
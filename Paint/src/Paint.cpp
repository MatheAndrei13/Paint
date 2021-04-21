#include "Paint.h"
#include <fstream>
#include <iomanip>

Paint::Paint() {
	glyph = EMPTY_GLYPH;
	fgColor = 0x0000;
	bgColor = 0x0000;
	tool = TOOL::Pencil;

	resetOriginPoint = true;
}

void Paint::OnCreate() {
	// Initialize Canvas
	canvas.Init(Vec2(GetScreenWidth() - 11, GetScreenHeight()), Rect(0, 0, GetScreenWidth() - 12, GetScreenHeight()));

	// Initialize Panels
	imagePanel.Init(L"IMAGE", Vec2(11, 9), Rect(GetScreenWidth() - 11, 0, GetScreenWidth() - 1, 8), 0x0070);
	infoPanel.Init(L"INFO", Vec2(11, 8), Rect(GetScreenWidth() - 11, 9, GetScreenWidth() - 1, 16), 0x0070, glyph, fgColor, bgColor);
	toolPanel.Init(L"TOOLS", Vec2(11, 17), Rect(GetScreenWidth() - 11, 17, GetScreenWidth() - 1, 33), 0x0070, tool);
	glyphPanel.Init(L"GLYPHS", Vec2(11, 20), Rect(GetScreenWidth() - 11, 34, GetScreenWidth() - 1, 53), 0x0070, glyph);
	palettePanel.Init(L"PALETTE", Vec2(11, 6), Rect(GetScreenWidth() - 11, 54, GetScreenWidth() - 1, 59), 0x0070, fgColor, bgColor);
}

void Paint::OnUpdate() {
	// Update Canvas & Panels
	UpdateVisualElements();

	// Check Inputs
	CheckMouseInput();
	CheckKeyboardInput();
}

void Paint::UpdateVisualElements() {
	// Update Canvas
	canvas.Update(*this);

	// Update Panels
	imagePanel.Update(*this);
	infoPanel.Update(*this);
	toolPanel.Update(*this);
	glyphPanel.Update(*this);
	palettePanel.Update(*this);
}

void Paint::CheckMouseInput() {
	// Get Current Mouse Position
	Vec2 mousePosition = GetMousePosition();

	if (KeyPressed(VK_LBUTTON)) { // Left Click
		// Draw according to the Selected Tool
		if (canvas.inBounds(mousePosition)) {
			switch (tool) {
			case TOOL::Pencil:
				canvas.Draw(mousePosition, glyph, fgColor | bgColor);
				break;

			case TOOL::Line:
				if (resetOriginPoint) {
					// Mouse Position when Left Click (for Blueprint Tools)
					originPoint = mousePosition;
					resetOriginPoint = false;
				}
				canvas.LineBlueprint(originPoint, mousePosition, glyph, fgColor | bgColor, *this);
				break;

			case TOOL::Rectangle:
				if (resetOriginPoint) {
					// Mouse Position when Left Click (for Blueprint Tools)
					originPoint = mousePosition;
					resetOriginPoint = false;
				}
				canvas.RectangleBlueprint(originPoint, mousePosition, glyph, fgColor | bgColor, *this);
				break;

			case TOOL::Ellipse:
				if (resetOriginPoint) {
					// Mouse Position when Left Click (for Blueprint Tools)
					originPoint = mousePosition;
					resetOriginPoint = false;
				}
				canvas.EllipseBlueprint(originPoint, mousePosition, glyph, fgColor | bgColor, *this);
				break;

			case TOOL::Bucket:
				canvas.Bucket(mousePosition, glyph, fgColor | bgColor);
				break;

			case TOOL::Picker:
				canvas.Picker(*this, mousePosition);
				break;

			default:
				break;
			}
		}
		else
			resetOriginPoint = true;
	}

	if (KeyReleased(VK_LBUTTON)) { // Left Click
		// Draw Blueprints for Line, Rectangle or Ellipse Tool
		if (!resetOriginPoint) {
			switch (tool) {
			case TOOL::Line:
				canvas.Line(originPoint, mousePosition, glyph, fgColor | bgColor);
				break;

			case TOOL::Rectangle:
				canvas.Rectangle(originPoint, mousePosition, glyph, fgColor | bgColor);
				break;

			case TOOL::Ellipse:
				canvas.Ellipse(originPoint, mousePosition, glyph, fgColor | bgColor);
				break;

			default:
				break;
			}

			canvas.Update(*this);

			resetOriginPoint = true;
		}

		// Press Button from Image Panel
		if (imagePanel.inBounds(mousePosition))
			imagePanel.PressButton(*this, mousePosition);

		// Select Tool from Tool Panel
		if (toolPanel.inBounds(mousePosition)) {
			toolPanel.SelectTool(*this, mousePosition);
			toolPanel.PressButton(*this, mousePosition);
		}

		// Select Glyph from Glyph Panel & Select Page
		if (glyphPanel.inBounds(mousePosition)) {
			glyphPanel.SelectGlyph(*this, mousePosition);
			glyphPanel.SelectPage(mousePosition, glyph);
		}

		// Select Color from Palette
		if (palettePanel.inBounds(mousePosition))
			palettePanel.SelectColor(*this, mousePosition);
	}
}

void Paint::CheckKeyboardInput() {
	// Clear Canvas
	if (KeyReleased('C'))
		ClearCanvas();

	// Change Tools
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

	// Change Colors
	if (KeyReleased(VK_OEM_6)) // Key: ]
		NextFGColor();
	if (KeyReleased(VK_OEM_4)) // Key: [
		PreviousFGColor();
	if (KeyReleased(VK_OEM_7)) // Key: '
		NextBGColor();
	if (KeyReleased(VK_OEM_1)) // Key: ;
		PreviousBGColor();

	// Change Font Size
	if (KeyReleased(VK_OEM_PLUS) || KeyReleased(VK_ADD))          // Key: +
		ChangeFontSize(GetFontSize().x + 1, GetFontSize().y + 1);
	if (KeyReleased(VK_OEM_MINUS) || KeyReleased(VK_SUBTRACT))    // Key: -
		ChangeFontSize(GetFontSize().x - 1, GetFontSize().y - 1);

	// Quit App
	if (KeyReleased('Q'))
		Close();
}

void Paint::ChangeGlyph(Glyph glyph) {
	this->glyph = glyph;

	infoPanel.ChangeGlyph(glyph);
	glyphPanel.ChangeGlyph(glyph);
}

void Paint::ChangeFGColor(Color color) {
	fgColor = color;

	infoPanel.ChangeFGColor(color);
	palettePanel.ChangeFGColor(fgColor);
}

void Paint::ChangeBGColor(Color color) {
	bgColor = color;

	infoPanel.ChangeBGColor(color);
	palettePanel.ChangeBGColor(bgColor);
}

void Paint::ChangeTool(TOOL tool) {
	this->tool = tool;

	toolPanel.ChangeTool(tool);
}

void Paint::NextFGColor() {
	if (fgColor == 0x000F)
		fgColor = 0x0000;
	else
		fgColor++;

	palettePanel.ChangeFGColor(fgColor);
}

void Paint::PreviousFGColor() {
	if (fgColor == 0x0000)
		fgColor = 0x000F;
	else
		fgColor--;

	palettePanel.ChangeFGColor(fgColor);
}

void Paint::NextBGColor() {
	if (bgColor == 0x00F0)
		bgColor = 0x0000;
	else
		bgColor += 0x0010;

	palettePanel.ChangeBGColor(bgColor);
}

void Paint::PreviousBGColor() {
	if (bgColor == 0x0000)
		bgColor = 0x00F0;
	else
		bgColor -= 0x0010;

	palettePanel.ChangeBGColor(bgColor);
}

void Paint::ClearCanvas() {
	canvas.Clear();
}
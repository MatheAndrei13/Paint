#include "Paint.h"
#include <fstream>
#include <iomanip>

Paint::Paint() {
	imageName = "";

	glyph = EMPTY_GLYPH;
	fgColor = 0x0000;
	bgColor = 0x0000;
	tool = TOOL::Pencil;

	updateVisualElements = false;

	resetOriginPoint = true;
}

void Paint::OnCreate() {
	// Initialize Canvas
	canvas.Init(Vec2(GetScreenWidth() - 11, GetScreenHeight()), Rect(0, 0, GetScreenWidth() - 12, GetScreenHeight()));

	// Initialize Panels
	imagePanel.Init(L"IMAGE", Vec2(11, 7), Rect(GetScreenWidth() - 11, 0, GetScreenWidth() - 1, 6), 0x0070);
	infoPanel.Init(L"INFO", Vec2(11, 9), Rect(GetScreenWidth() - 11, 7, GetScreenWidth() - 1, 15), 0x0070, glyph, fgColor, bgColor, imageName);
	toolPanel.Init(L"TOOLS", Vec2(11, 18), Rect(GetScreenWidth() - 11, 16, GetScreenWidth() - 1, 33), 0x0070, tool);
	glyphPanel.Init(L"GLYPHS", Vec2(11, 20), Rect(GetScreenWidth() - 11, 34, GetScreenWidth() - 1, 53), 0x0070, glyph);
	palettePanel.Init(L"PALETTE", Vec2(11, 6), Rect(GetScreenWidth() - 11, 54, GetScreenWidth() - 1, 59), 0x0070, fgColor, bgColor);

	// Initialize Shortcuts Panel
	shortcutsPanel.Init(L"SHORTCUTS", Vec2(30, 30), Rect(GetScreenWidth() - 42, 0, GetScreenWidth() - 12, 29), 0x0070);

	// Initialize Input Panels
	NewImageNamePanel.Init(L"IMAGE NAME", Vec2(14, 3), Rect((GetScreenWidth() - 11) / 2 - 7, GetScreenHeight() / 2 - 3, (GetScreenWidth() - 11) / 2 + 6, GetScreenHeight() / 2 - 1), 0x0070);
	SavedImageNamePanel.Init(L"IMAGE NAME", Vec2(14, 3), Rect((GetScreenWidth() - 11) / 2 - 7, GetScreenHeight() / 2 - 3, (GetScreenWidth() - 11) / 2 + 6, GetScreenHeight() / 2 - 1), 0x0070);
}

void Paint::OnUpdate() {
	// Freeze Elements if Input Panel exists
	if (NewImageNamePanel.active) {
		NewImageNamePanel.Update(*this);
		NewImageNamePanel.CheckInput(*this);
		if (NewImageNamePanel.value != "") {
			ChangeImageName(NewImageNamePanel.value);
			Save();
		}
		return;
	}
	if (SavedImageNamePanel.active) {
		SavedImageNamePanel.Update(*this);
		SavedImageNamePanel.CheckInput(*this);
		if (SavedImageNamePanel.value != "") {
			Load(SavedImageNamePanel.value);
			SavedImageNamePanel.value = "";
		}
		return;
	}

	// Freeze Elements if Shortcuts Panel is visible
	if (shortcutsPanel.active) {
		shortcutsPanel.Update(*this);
		if (KeyReleased(VK_ESCAPE))
			shortcutsPanel.Hide(*this);
		return;
	}

	// Check Inputs
	CheckMouseInput();
	CheckKeyboardInput();

	// Update Canvas & Panels
	UpdateVisualElements();

	// Draw Blueprints if necessary
	DrawBlueprints();
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
	mousePosition = GetMousePosition();

	// Reset originPoint (for Blueprint Tools)
	if (resetOriginPoint)
		originPoint = mousePosition;

	if (KeyPressed(VK_LBUTTON)) { // Left Click
		if (canvas.inBounds(mousePosition)) {
			resetOriginPoint = false;

			switch (tool) {
			case TOOL::Pencil:
				canvas.Draw(mousePosition, glyph, fgColor | bgColor);
				break;

			case TOOL::Eraser:
				canvas.Draw(mousePosition, EMPTY_GLYPH, 0x0000);
				break;

			case TOOL::Bucket:
				canvas.Bucket(mousePosition, glyph, fgColor | bgColor);
				break;

			case TOOL::Picker:
				canvas.Picker(mousePosition, *this);
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

			case TOOL::Copy:
				canvas.Copy(originPoint, mousePosition);
				break;

			case TOOL::Paste:
				canvas.Paste(mousePosition, *this);
				break;

			default:
				break;
			}
			resetOriginPoint = true;
		}

		// Update Timeline
		canvas.AddTimePoint();

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

void Paint::DrawBlueprints() {
	if (canvas.inBounds(mousePosition)) {
		if (tool == TOOL::Paste)
			canvas.PasteBlueprint(mousePosition, *this);

		if (KeyPressed(VK_LBUTTON)) { // Left Click
			switch (tool) {
			case TOOL::Line:
				canvas.LineBlueprint(originPoint, mousePosition, glyph, fgColor | bgColor, *this);
				break;

			case TOOL::Rectangle:
				canvas.RectangleBlueprint(originPoint, mousePosition, glyph, fgColor | bgColor, *this);
				break;

			case TOOL::Ellipse:
				canvas.EllipseBlueprint(originPoint, mousePosition, glyph, fgColor | bgColor, *this);
				break;

			case TOOL::Copy:
				canvas.Selection(originPoint, mousePosition, *this);
				break;

			default:
				break;
			}
		}
	}
}

void Paint::CheckKeyboardInput() {
	// New Image
	if (KeyPressed(VK_CONTROL) && KeyReleased('N'))
		New();

	// Save Image
	if (KeyPressed(VK_CONTROL) && KeyReleased('S'))
		Save();

	// Load Image
	if (KeyPressed(VK_CONTROL) && KeyReleased('O'))
		Load("");

	// Close App
	if (KeyPressed(VK_CONTROL) && KeyReleased('Q'))
		Close();

	// Change Font Size
	if (KeyReleased(VK_OEM_PLUS) || KeyReleased(VK_ADD))          // Key: +
		ChangeFontSize(GetFontSize().x + 1, GetFontSize().y + 1);
	if (KeyReleased(VK_OEM_MINUS) || KeyReleased(VK_SUBTRACT))    // Key: -
		ChangeFontSize(GetFontSize().x - 1, GetFontSize().y - 1);

	// Switch Colors
	if (KeyReleased(VK_UP))
		NextFGColor();
	if (KeyReleased(VK_DOWN))
		PreviousFGColor();
	if (KeyReleased(VK_RIGHT))
		NextBGColor();
	if (KeyReleased(VK_LEFT))
		PreviousBGColor();

	// Undo
	if (KeyPressed(VK_CONTROL) && KeyReleased('Z'))
		Undo();

	// Redo
	if (KeyPressed(VK_CONTROL) && KeyReleased('Y'))
		Redo();

	// Change Tools
	if (KeyReleased('P'))
		ChangeTool(TOOL::Pencil);
	if (KeyReleased('E'))
		ChangeTool(TOOL::Eraser);
	if (KeyReleased('L'))
		ChangeTool(TOOL::Line);
	if (KeyReleased('R'))
		ChangeTool(TOOL::Rectangle);
	if (KeyReleased('O'))
		ChangeTool(TOOL::Ellipse);
	if (KeyReleased('B'))
		ChangeTool(TOOL::Bucket);
	if (KeyReleased(VK_LSHIFT))
		ChangeTool(TOOL::Picker);

	// Copy
	if (KeyPressed(VK_CONTROL) && KeyReleased('C'))
		ChangeTool(TOOL::Copy);

	// Paste
	if (KeyPressed(VK_CONTROL) && KeyReleased('V'))
		ChangeTool(TOOL::Paste);

	// Clear Canvas
	if (KeyPressed(VK_CONTROL) && KeyReleased('X'))
		ClearCanvas();
}

void Paint::New() {
	ChangeImageName("");
	canvas.Clear();
	canvas.ResetTimeline();
}

void Paint::Save() {
	// Name Image if it doesn't have one
	if (imageName == "") {
		NewImageNamePanel.Enable();
		return;
	}

	// Save Image
	std::string imagePath = savePath + imageName + imageExtension;

	std::ofstream fout(imagePath);

	// Return if file failed
	if (!fout.is_open())
		return;

	canvas.Save(fout);

	fout.close();
}

void Paint::Load(std::string imageName) {
	// Select Saved Image
	if (SavedImageNamePanel.value == "") {
		SavedImageNamePanel.Enable();
		return;
	}
		
	std::string imagePath = savePath + imageName + imageExtension;

	std::ifstream fin(imagePath);

	// Return if file failed
	if (!fin.is_open())
		return;

	ChangeImageName(imageName);

	canvas.Load(fin);

	fin.close();
}

void Paint::Shortcuts() {
	if (!shortcutsPanel.active)
		shortcutsPanel.Show();
	else
		shortcutsPanel.Hide(*this);
}

void Paint::Undo() {
	canvas.Undo();
}

void Paint::Redo() {
	canvas.Redo();
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

void Paint::ChangeImageName(std::string name) {
	imageName = name;

	infoPanel.ChangeImageName(imageName);
}

void Paint::NextFGColor() {
	if (fgColor == 0x000F)
		fgColor = 0x0000;
	else
		fgColor++;

	infoPanel.ChangeFGColor(fgColor);
	palettePanel.ChangeFGColor(fgColor);
}

void Paint::PreviousFGColor() {
	if (fgColor == 0x0000)
		fgColor = 0x000F;
	else
		fgColor--;

	infoPanel.ChangeFGColor(fgColor);
	palettePanel.ChangeFGColor(fgColor);
}

void Paint::NextBGColor() {
	if (bgColor == 0x00F0)
		bgColor = 0x0000;
	else
		bgColor += 0x0010;

	infoPanel.ChangeBGColor(bgColor);
	palettePanel.ChangeBGColor(bgColor);
}

void Paint::PreviousBGColor() {
	if (bgColor == 0x0000)
		bgColor = 0x00F0;
	else
		bgColor -= 0x0010;

	infoPanel.ChangeBGColor(bgColor);
	palettePanel.ChangeBGColor(bgColor);
}

void Paint::ClearCanvas() {
	canvas.Clear();

	// Update Timeline
	canvas.AddTimePoint();
}
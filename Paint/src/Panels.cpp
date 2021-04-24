#include "Panels.h"
#include "Paint.h"
#include <fstream>
#include <iomanip>

/* ####### IMAGE PANEL ####### */

ImagePanel::ImagePanel() {
	color = 0x0000;
}

bool ImagePanel::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void ImagePanel::Init(std::wstring name, Vec2 size, Rect region, Color color) {
	texture.Init(size, region);

	this->name = name;
	this->color = color;

	// Draw Panel Frame
	// Center
	for (short x = 1; x < size.x - 1; ++x)
		for (short y = 1; y < size.y - 1; ++y)
			texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, color);

	// Edges
	for (short x = 1; x < size.x - 1; ++x) {
		texture.SetPixel(Vec2(x, 0), L'\u2500', color);
		texture.SetPixel(Vec2(x, size.y - 1), L'\u2500', color);
	}

	for (short y = 1; y < size.y - 1; ++y) {
		texture.SetPixel(Vec2(0, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x - 1, y), L'\u2502', color);
	}

	// Corners
	texture.SetPixel(Vec2(0, 0), L'\u250C', color);
	texture.SetPixel(Vec2(size.x - 1, 0), L'\u2510', color);
	texture.SetPixel(Vec2(0, size.y - 1), L'\u2514', color);
	texture.SetPixel(Vec2(size.x - 1, size.y - 1), L'\u2518', color);

	// Write Panel Name
	short nameLength = (short)name.length();
	short namePosition = size.x / 2 - nameLength / 2;
	for (short i = 0; i < (short)nameLength; ++i)
		texture.SetPixel(Vec2(namePosition + i, 0), name.at(i), color);

	texture.SetPixel(Vec2(namePosition - 1, 0), L'\u2524', color);
	texture.SetPixel(Vec2(namePosition + nameLength, 0), L'\u251C', color);

	// Initialize Labels
	newButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 1, texture.region.right - 1, texture.region.top + 1));
	resizeButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 2, texture.region.right - 1, texture.region.top + 2));
	saveButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 3, texture.region.right - 1, texture.region.top + 3));
	loadButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 4, texture.region.right - 1, texture.region.top + 4));
	exitButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 6, texture.region.right - 1, texture.region.top + 6));

	// Draw Labels
	DrawButtons();
}

void ImagePanel::Update(Paint& app) {
	if (texture.update || app.updateVisualElements) {
		app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });
		texture.update = false;
		app.updateVisualElements = false;
	}

	if (newButton.update || app.updateVisualElements) {
		app.WriteToBuffer(newButton.buffer, { newButton.size.x, newButton.size.y }, { newButton.region.left, newButton.region.top, newButton.region.right, newButton.region.bottom });
		newButton.update = false;
		app.updateVisualElements = false;
	}
	if (resizeButton.update || app.updateVisualElements) {
		app.WriteToBuffer(resizeButton.buffer, { resizeButton.size.x, resizeButton.size.y }, { resizeButton.region.left, resizeButton.region.top, resizeButton.region.right, resizeButton.region.bottom });
		resizeButton.update = false;
		app.updateVisualElements = false;
	}
	if (saveButton.update || app.updateVisualElements) {
		app.WriteToBuffer(saveButton.buffer, { saveButton.size.x, saveButton.size.y }, { saveButton.region.left, saveButton.region.top, saveButton.region.right, saveButton.region.bottom });
		saveButton.update = false;
		app.updateVisualElements = false;
	}
	if (loadButton.update || app.updateVisualElements) {
		app.WriteToBuffer(loadButton.buffer, { loadButton.size.x, loadButton.size.y }, { loadButton.region.left, loadButton.region.top, loadButton.region.right, loadButton.region.bottom });
		loadButton.update = false;
		app.updateVisualElements = false;
	}
	if (exitButton.update || app.updateVisualElements) {
		app.WriteToBuffer(exitButton.buffer, { exitButton.size.x, exitButton.size.y }, { exitButton.region.left, exitButton.region.top, exitButton.region.right, exitButton.region.bottom });
		exitButton.update = false;
		app.updateVisualElements = false;
	}
}

void ImagePanel::DrawButtons() {
	std::wstring newText = L"New";
	std::wstring resizeText = L"Resize";
	std::wstring saveText = L"Save";
	std::wstring loadText = L"Load";
	std::wstring exitText = L"Exit";

	// New Button (Text + Background)
	for (short i = 0; i < (short)newText.length(); ++i)
		newButton.SetPixel(Vec2(i, 0), newText.at(i), color);
	for (short i = (short)newText.length(); i < newButton.size.x; ++i)
		newButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Resize Button (Text + Background)
	for (short i = 0; i < (short)resizeText.length(); ++i)
		resizeButton.SetPixel(Vec2(i, 0), resizeText.at(i), color);
	for (short i = (short)resizeText.length(); i < resizeButton.size.x; ++i)
		resizeButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Save Button (Text + Background)
	for (short i = 0; i < (short)saveText.length(); ++i)
		saveButton.SetPixel(Vec2(i, 0), saveText.at(i), color);
	for (short i = (short)saveText.length(); i < saveButton.size.x; ++i)
		saveButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Load Button (Text + Background)
	for (short i = 0; i < (short)loadText.length(); ++i)
		loadButton.SetPixel(Vec2(i, 0), loadText.at(i), color);
	for (short i = (short)loadText.length(); i < loadButton.size.x; ++i)
		loadButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Exit Button (Text + Background)
	for (short i = 0; i < (short)exitText.length(); ++i)
		exitButton.SetPixel(Vec2(i, 0), exitText.at(i), color);
	for (short i = (short)exitText.length(); i < exitButton.size.x; ++i)
		exitButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
}

void ImagePanel::PressButton(Paint& app, Vec2 mousePosition) {
	if (newButton.inBounds(mousePosition))
		app.New();
	else if (saveButton.inBounds(mousePosition))
		app.Save();
	else if (loadButton.inBounds(mousePosition))
		app.Load("");
	else if (exitButton.inBounds(mousePosition))
		app.Close();
}




/* ####### INFO PANEL ####### */

InfoPanel::InfoPanel() {
	color = 0x0000;
}

bool InfoPanel::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void InfoPanel::Init(std::wstring name, Vec2 size, Rect region, Color color, Glyph glyph, Color fgColor, Color bgColor, std::string imageName) {
	texture.Init(size, region);

	this->name = name;
	this->color = color;

	// Draw Panel Frame
	// Center
	for (short x = 1; x < size.x - 1; ++x)
		for (short y = 1; y < size.y - 1; ++y)
			texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, color);

	// Edges
	for (short x = 1; x < size.x - 1; ++x) {
		texture.SetPixel(Vec2(x, 0), L'\u2500', color);
		texture.SetPixel(Vec2(x, size.y - 1), L'\u2500', color);
	}

	for (short y = 1; y < size.y - 1; ++y) {
		texture.SetPixel(Vec2(0, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x - 1, y), L'\u2502', color);
	}

	// Corners
	texture.SetPixel(Vec2(0, 0), L'\u250C', color);
	texture.SetPixel(Vec2(size.x - 1, 0), L'\u2510', color);
	texture.SetPixel(Vec2(0, size.y - 1), L'\u2514', color);
	texture.SetPixel(Vec2(size.x - 1, size.y - 1), L'\u2518', color);

	// Write Panel Name
	short nameLength = (short)name.length();
	short namePosition = size.x / 2 - nameLength / 2;
	for (short i = 0; i < (short)nameLength; ++i)
		texture.SetPixel(Vec2(namePosition + i, 0), name.at(i), color);

	texture.SetPixel(Vec2(namePosition - 1, 0), L'\u2524', color);
	texture.SetPixel(Vec2(namePosition + nameLength, 0), L'\u251C', color);

	// Initialize Labels
	previewLabel.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 1, texture.region.right - 1, texture.region.top + 1));
	glyphLabel.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 2, texture.region.right - 1, texture.region.top + 2));
	fgColorLabel.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 3, texture.region.right - 1, texture.region.top + 3));
	bgColorLabel.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 4, texture.region.right - 1, texture.region.top + 4));
	imageLabel.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 6, texture.region.right - 1, texture.region.top + 6));
	imageNameLabel.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 7, texture.region.right - 1, texture.region.top + 7));

	// Draw Labels
	DrawLabels();

	// Set Image Name
	ChangeImageName(imageName);

	// Set Preview, Glyph and Colors
	ChangeGlyph(glyph);
	ChangeFGColor(fgColor);
	ChangeBGColor(bgColor);
}

void InfoPanel::Update(Paint& app) {
	if (texture.update || app.updateVisualElements) {
		app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });
		texture.update = false;
		app.updateVisualElements = false;
	}

	if (previewLabel.update || app.updateVisualElements) {
		app.WriteToBuffer(previewLabel.buffer, { previewLabel.size.x, previewLabel.size.y }, { previewLabel.region.left, previewLabel.region.top, previewLabel.region.right, previewLabel.region.bottom });
		previewLabel.update = false;
		app.updateVisualElements = false;
	}
	if (glyphLabel.update || app.updateVisualElements) {
		app.WriteToBuffer(glyphLabel.buffer, { glyphLabel.size.x, glyphLabel.size.y }, { glyphLabel.region.left, glyphLabel.region.top, glyphLabel.region.right, glyphLabel.region.bottom });
		glyphLabel.update = false;
		app.updateVisualElements = false;
	}
	if (fgColorLabel.update || app.updateVisualElements) {
		app.WriteToBuffer(fgColorLabel.buffer, { fgColorLabel.size.x, fgColorLabel.size.y }, { fgColorLabel.region.left, fgColorLabel.region.top, fgColorLabel.region.right, fgColorLabel.region.bottom });
		fgColorLabel.update = false;
		app.updateVisualElements = false;
	}
	if (bgColorLabel.update || app.updateVisualElements) {
		app.WriteToBuffer(bgColorLabel.buffer, { bgColorLabel.size.x, bgColorLabel.size.y }, { bgColorLabel.region.left, bgColorLabel.region.top, bgColorLabel.region.right, bgColorLabel.region.bottom });
		bgColorLabel.update = false;
		app.updateVisualElements = false;
	}
	if (imageLabel.update || app.updateVisualElements) {
		app.WriteToBuffer(imageLabel.buffer, { imageLabel.size.x, imageLabel.size.y }, { imageLabel.region.left, imageLabel.region.top, imageLabel.region.right, imageLabel.region.bottom });
		imageLabel.update = false;
		app.updateVisualElements = false;
	}
	if (imageNameLabel.update || app.updateVisualElements) {
		app.WriteToBuffer(imageNameLabel.buffer, { imageNameLabel.size.x, imageNameLabel.size.y }, { imageNameLabel.region.left, imageNameLabel.region.top, imageNameLabel.region.right, imageNameLabel.region.bottom });
		imageNameLabel.update = false;
		app.updateVisualElements = false;
	}
}

void InfoPanel::DrawLabels() {
	std::wstring previewText = L"Preview";
	std::wstring glyphText = L"Glyph";
	std::wstring fgColorText = L"Fore";
	std::wstring bgColorText = L"Back";
	std::wstring imageText = L"  Image";

	// Preview Label (Text + Background)
	for (short i = 0; i < (short)previewText.length(); ++i)
		previewLabel.SetPixel(Vec2(i, 0), previewText.at(i), color);
	for (short i = (short)previewText.length(); i < previewLabel.size.x; ++i)
		previewLabel.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Glyph Label (Text + Background)
	for (short i = 0; i < (short)glyphText.length(); ++i)
		glyphLabel.SetPixel(Vec2(i, 0), glyphText.at(i), color);
	for (short i = (short)glyphText.length(); i < glyphLabel.size.x; ++i)
		glyphLabel.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Foreground Color Label (Text + Background)
	for (short i = 0; i < (short)fgColorText.length(); ++i)
		fgColorLabel.SetPixel(Vec2(i, 0), fgColorText.at(i), color);
	for (short i = (short)fgColorText.length(); i < fgColorLabel.size.x; ++i)
		fgColorLabel.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Background Color Label (Text + Background)
	for (short i = 0; i < (short)bgColorText.length(); ++i)
		bgColorLabel.SetPixel(Vec2(i, 0), bgColorText.at(i), color);
	for (short i = (short)bgColorText.length(); i < bgColorLabel.size.x; ++i)
		bgColorLabel.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Image Label (Text + Background)
	for (short i = 0; i < (short)imageText.length(); ++i)
		imageLabel.SetPixel(Vec2(i, 0), imageText.at(i), color);
	for (short i = (short)imageText.length(); i < imageLabel.size.x; ++i)
		imageLabel.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
}

void InfoPanel::ChangeGlyph(Glyph glyph) {
	glyphLabel.SetPixel(Vec2(glyphLabel.size.x - 1, 0), glyph, color);

	previewLabel.SetPixel(Vec2(previewLabel.size.x - 1, 0), glyph , (Color)previewLabel.GetPixel(Vec2(previewLabel.size.x - 1, 0)).Attributes);
}

void InfoPanel::ChangeFGColor(Color color) {
	fgColorLabel.SetPixel(Vec2(fgColorLabel.size.x - 1, 0), EMPTY_GLYPH, color << 4);

	previewLabel.SetPixel(Vec2(previewLabel.size.x - 1, 0), previewLabel.GetPixel(Vec2(previewLabel.size.x - 1, 0)).Char.UnicodeChar, color | bgColorLabel.GetPixel(Vec2(previewLabel.size.x - 1, 0)).Attributes);
}

void InfoPanel::ChangeBGColor(Color color) {
	bgColorLabel.SetPixel(Vec2(bgColorLabel.size.x - 1, 0), EMPTY_GLYPH, color);

	previewLabel.SetPixel(Vec2(previewLabel.size.x - 1, 0), previewLabel.GetPixel(Vec2(previewLabel.size.x - 1, 0)).Char.UnicodeChar, (fgColorLabel.GetPixel(Vec2(previewLabel.size.x - 1, 0)).Attributes >> 4) | color);
}

void InfoPanel::ChangeImageName(std::string imageName) {
	for (short i = 0; i < (short)imageName.length(); ++i)
		imageNameLabel.SetPixel(Vec2(i, 0), imageName.at(i), 0x000F);
	for (short i = (short)imageName.length(); i < imageNameLabel.size.x; ++i)
		imageNameLabel.SetPixel(Vec2(i, 0), EMPTY_GLYPH, 0x000F);
}




/* ####### TOOL PANEL ####### */

ToolPanel::ToolPanel() {
	color = 0x0000;
	selectedTool = TOOL::Pencil;
}

bool ToolPanel::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void ToolPanel::Init(std::wstring name, Vec2 size, Rect region, Color color, TOOL tool) {
	texture.Init(size, region);

	this->name = name;
	this->color = color;

	// Draw Panel Frame
	// Center
	for (short x = 1; x < size.x - 1; ++x)
		for (short y = 1; y < size.y - 1; ++y)
			texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, color);

	// Edges
	for (short x = 1; x < size.x - 1; ++x) {
		texture.SetPixel(Vec2(x, 0), L'\u2500', color);
		texture.SetPixel(Vec2(x, size.y - 1), L'\u2500', color);
	}

	for (short y = 1; y < size.y - 1; ++y) {
		texture.SetPixel(Vec2(0, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x - 1, y), L'\u2502', color);
	}

	// Corners
	texture.SetPixel(Vec2(0, 0), L'\u250C', color);
	texture.SetPixel(Vec2(size.x - 1, 0), L'\u2510', color);
	texture.SetPixel(Vec2(0, size.y - 1), L'\u2514', color);
	texture.SetPixel(Vec2(size.x - 1, size.y - 1), L'\u2518', color);

	// Write Panel Name
	short nameLength = (short)name.length();
	short namePosition = size.x / 2 - nameLength / 2;
	for (short i = 0; i < (short)nameLength; ++i)
		texture.SetPixel(Vec2(namePosition + i, 0), name.at(i), color);

	texture.SetPixel(Vec2(namePosition - 1, 0), L'\u2524', color);
	texture.SetPixel(Vec2(namePosition + nameLength, 0), L'\u251C', color);

	// Initialize Buttons
	pencilButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 1, texture.region.right - 1, texture.region.top + 1));
	lineButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 2, texture.region.right - 1, texture.region.top + 2));
	rectangleButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 3, texture.region.right - 1, texture.region.top + 3));
	ellipseButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 4, texture.region.right - 1, texture.region.top + 4));
	bucketButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 5, texture.region.right - 1, texture.region.top + 5));
	pickerButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 7, texture.region.right - 1, texture.region.top + 7));
	copyButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 9, texture.region.right - 1, texture.region.top + 9));
	pasteButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 10, texture.region.right - 1, texture.region.top + 10));
	undoButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 12, texture.region.right - 1, texture.region.top + 12));
	redoButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 13, texture.region.right - 1, texture.region.top + 13));
	clearButton.Init(Vec2(9, 1), Rect(texture.region.left + 1, texture.region.top + 15, texture.region.right - 1, texture.region.top + 15));

	// Draw Buttons
	DrawButtons();

	// Set Tool
	ChangeTool(tool);
}

void ToolPanel::Update(Paint& app) {
	if (texture.update || app.updateVisualElements) {
		app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });
		texture.update = false;
		app.updateVisualElements = false;
	}

	if (pencilButton.update || app.updateVisualElements) {
		app.WriteToBuffer(pencilButton.buffer, { pencilButton.size.x, pencilButton.size.y }, { pencilButton.region.left, pencilButton.region.top, pencilButton.region.right, pencilButton.region.bottom });
		pencilButton.update = false;
		app.updateVisualElements = false;
	}
	if (lineButton.update || app.updateVisualElements) {
		app.WriteToBuffer(lineButton.buffer, { lineButton.size.x, lineButton.size.y }, { lineButton.region.left, lineButton.region.top, lineButton.region.right, lineButton.region.bottom });
		lineButton.update = false;
		app.updateVisualElements = false;
	}
	if (rectangleButton.update || app.updateVisualElements) {
		app.WriteToBuffer(rectangleButton.buffer, { rectangleButton.size.x, rectangleButton.size.y }, { rectangleButton.region.left, rectangleButton.region.top, rectangleButton.region.right, rectangleButton.region.bottom });
		rectangleButton.update = false;
		app.updateVisualElements = false;
	}
	if (ellipseButton.update || app.updateVisualElements) {
		app.WriteToBuffer(ellipseButton.buffer, { ellipseButton.size.x, ellipseButton.size.y }, { ellipseButton.region.left, ellipseButton.region.top, ellipseButton.region.right, ellipseButton.region.bottom });
		ellipseButton.update = false;
		app.updateVisualElements = false;
	}
	if (bucketButton.update || app.updateVisualElements) {
		app.WriteToBuffer(bucketButton.buffer, { bucketButton.size.x, bucketButton.size.y }, { bucketButton.region.left, bucketButton.region.top, bucketButton.region.right, bucketButton.region.bottom });
		bucketButton.update = false;
		app.updateVisualElements = false;
	}
	if (pickerButton.update || app.updateVisualElements) {
		app.WriteToBuffer(pickerButton.buffer, { pickerButton.size.x, pickerButton.size.y }, { pickerButton.region.left, pickerButton.region.top, pickerButton.region.right, pickerButton.region.bottom });
		pickerButton.update = false;
		app.updateVisualElements = false;
	}
	if (copyButton.update || app.updateVisualElements) {
		app.WriteToBuffer(copyButton.buffer, { copyButton.size.x, copyButton.size.y }, { copyButton.region.left, copyButton.region.top, copyButton.region.right, copyButton.region.bottom });
		copyButton.update = false;
		app.updateVisualElements = false;
	}
	if (pasteButton.update || app.updateVisualElements) {
		app.WriteToBuffer(pasteButton.buffer, { pasteButton.size.x, pasteButton.size.y }, { pasteButton.region.left, pasteButton.region.top, pasteButton.region.right, pasteButton.region.bottom });
		pasteButton.update = false;
		app.updateVisualElements = false;
	}
	if (undoButton.update || app.updateVisualElements) {
		app.WriteToBuffer(undoButton.buffer, { undoButton.size.x, undoButton.size.y }, { undoButton.region.left, undoButton.region.top, undoButton.region.right, undoButton.region.bottom });
		undoButton.update = false;
		app.updateVisualElements = false;
	}
	if (redoButton.update || app.updateVisualElements) {
		app.WriteToBuffer(redoButton.buffer, { redoButton.size.x, redoButton.size.y }, { redoButton.region.left, redoButton.region.top, redoButton.region.right, redoButton.region.bottom });
		redoButton.update = false;
		app.updateVisualElements = false;
	}
	if (clearButton.update || app.updateVisualElements) {
		app.WriteToBuffer(clearButton.buffer, { clearButton.size.x, clearButton.size.y }, { clearButton.region.left, clearButton.region.top, clearButton.region.right, clearButton.region.bottom });
		clearButton.update = false;
		app.updateVisualElements = false;
	}
}

void ToolPanel::DrawButtons() {
	std::wstring pencilText = L"Pencil";
	std::wstring lineText = L"Line";
	std::wstring rectangleText = L"Rect";
	std::wstring ellipseText = L"Ellipse";
	std::wstring bucketText = L"Bucket";
	std::wstring pickerText = L"Picker";
	std::wstring copyText = L"Copy";
	std::wstring pasteText = L"Paste";
	std::wstring undoText = L"Undo";
	std::wstring redoText = L"Redo";
	std::wstring clearText = L"Clear";

	// Pencil Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)pencilText.length(); ++i)
		pencilButton.SetPixel(Vec2(i, 0), pencilText.at(i), color);
	for (short i = (short)pencilText.length(); i < pencilButton.size.x - 1; ++i)
		pencilButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	pencilButton.SetPixel(Vec2(pencilButton.size.x - 1, 0), L'\u25a1', color);

	// Line Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)lineText.length(); ++i)
		lineButton.SetPixel(Vec2(i, 0), lineText.at(i), color);
	for (short i = (short)lineText.length(); i < lineButton.size.x - 1; ++i)
		lineButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	lineButton.SetPixel(Vec2(lineButton.size.x - 1, 0), L'\u25a1', color);

	// Rectangle Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)rectangleText.length(); ++i)
		rectangleButton.SetPixel(Vec2(i, 0), rectangleText.at(i), color);
	for (short i = (short)rectangleText.length(); i < rectangleButton.size.x - 1; ++i)
		rectangleButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	rectangleButton.SetPixel(Vec2(rectangleButton.size.x - 1, 0), L'\u25a1', color);

	// Ellipse Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)ellipseText.length(); ++i)
		ellipseButton.SetPixel(Vec2(i, 0), ellipseText.at(i), color);
	for (short i = (short)ellipseText.length(); i < ellipseButton.size.x - 1; ++i)
		ellipseButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	ellipseButton.SetPixel(Vec2(ellipseButton.size.x - 1, 0), L'\u25a1', color);

	// Bucket Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)bucketText.length(); ++i)
		bucketButton.SetPixel(Vec2(i, 0), bucketText.at(i), color);
	for (short i = (short)bucketText.length(); i < bucketButton.size.x - 1; ++i)
		bucketButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	bucketButton.SetPixel(Vec2(bucketButton.size.x - 1, 0), L'\u25a1', color);

	// Picker Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)pickerText.length(); ++i)
		pickerButton.SetPixel(Vec2(i, 0), pickerText.at(i), color);
	for (short i = (short)pickerText.length(); i < pickerButton.size.x - 1; ++i)
		pickerButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	pickerButton.SetPixel(Vec2(pickerButton.size.x - 1, 0), L'\u25a1', color);

	// Copy Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)copyText.length(); ++i)
		copyButton.SetPixel(Vec2(i, 0), copyText.at(i), color);
	for (short i = (short)copyText.length(); i < copyButton.size.x - 1; ++i)
		copyButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	copyButton.SetPixel(Vec2(copyButton.size.x - 1, 0), L'\u25a1', color);

	// Paste Button (Text + Background + Tick Button)
	for (short i = 0; i < (short)pasteText.length(); ++i)
		pasteButton.SetPixel(Vec2(i, 0), pasteText.at(i), color);
	for (short i = (short)pasteText.length(); i < pasteButton.size.x - 1; ++i)
		pasteButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
	pasteButton.SetPixel(Vec2(pasteButton.size.x - 1, 0), L'\u25a1', color);

	// Undo Button (Text + Background)
	for (short i = 0; i < (short)undoText.length(); ++i)
		undoButton.SetPixel(Vec2(i, 0), undoText.at(i), color);
	for (short i = (short)undoText.length(); i < undoButton.size.x; ++i)
		undoButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Redo Button (Text + Background)
	for (short i = 0; i < (short)redoText.length(); ++i)
		redoButton.SetPixel(Vec2(i, 0), redoText.at(i), color);
	for (short i = (short)redoText.length(); i < redoButton.size.x; ++i)
		redoButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);

	// Clear Button (Text + Background)
	for (short i = 0; i < (short)clearText.length(); ++i)
		clearButton.SetPixel(Vec2(i, 0), clearText.at(i), color);
	for (short i = (short)clearText.length(); i < clearButton.size.x; ++i)
		clearButton.SetPixel(Vec2(i, 0), EMPTY_GLYPH, color);
}

void ToolPanel::ChangeTool(TOOL tool) {
	switch (selectedTool) {
	case TOOL::Pencil:
		pencilButton.SetPixel(Vec2(pencilButton.size.x - 1, 0), L'\u25a1', color);
		break;

	case TOOL::Line:
		lineButton.SetPixel(Vec2(lineButton.size.x - 1, 0), L'\u25a1', color);
		break;

	case TOOL::Rectangle:
		rectangleButton.SetPixel(Vec2(rectangleButton.size.x - 1, 0), L'\u25a1', color);
		break;

	case TOOL::Ellipse:
		ellipseButton.SetPixel(Vec2(ellipseButton.size.x - 1, 0), L'\u25a1', color);
		break;

	case TOOL::Bucket:
		bucketButton.SetPixel(Vec2(bucketButton.size.x - 1, 0), L'\u25a1', color);
		break;

	case TOOL::Picker:
		pickerButton.SetPixel(Vec2(pickerButton.size.x - 1, 0), L'\u25a1', color);
		break;

	default:
		break;
	}

	switch (tool) {
	case TOOL::Pencil:
		pencilButton.SetPixel(Vec2(pencilButton.size.x - 1, 0), L'\u25a0', color);
		break;

	case TOOL::Line:
		lineButton.SetPixel(Vec2(lineButton.size.x - 1, 0), L'\u25a0', color);
		break;

	case TOOL::Rectangle:
		rectangleButton.SetPixel(Vec2(rectangleButton.size.x - 1, 0), L'\u25a0', color);
		break;

	case TOOL::Ellipse:
		ellipseButton.SetPixel(Vec2(ellipseButton.size.x - 1, 0), L'\u25a0', color);
		break;

	case TOOL::Bucket:
		bucketButton.SetPixel(Vec2(bucketButton.size.x - 1, 0), L'\u25a0', color);
		break;

	case TOOL::Picker:
		pickerButton.SetPixel(Vec2(pickerButton.size.x - 1, 0), L'\u25a0', color);
		break;

	default:
		break;
	}

	selectedTool = tool;
}

void ToolPanel::SelectTool(Paint& app, Vec2 mousePosition) {
	if (pencilButton.inBounds(mousePosition))
		app.ChangeTool(TOOL::Pencil);
	else if (lineButton.inBounds(mousePosition))
		app.ChangeTool(TOOL::Line);
	else if (rectangleButton.inBounds(mousePosition))
		app.ChangeTool(TOOL::Rectangle);
	else if (ellipseButton.inBounds(mousePosition))
		app.ChangeTool(TOOL::Ellipse);
	else if (bucketButton.inBounds(mousePosition))
		app.ChangeTool(TOOL::Bucket);
	else if (pickerButton.inBounds(mousePosition))
		app.ChangeTool(TOOL::Picker);
}

void ToolPanel::PressButton(Paint& app, Vec2 mousePosition) {
	if (undoButton.inBounds(mousePosition))
		app.Undo();
	else if (redoButton.inBounds(mousePosition))
		app.Redo();
	else if (clearButton.inBounds(mousePosition))
		app.ClearCanvas();
}




/* ####### GLYPH PANEL ####### */

GlyphPanel::GlyphPanel() {
	color = 0x0000;
}

bool GlyphPanel::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void GlyphPanel::Init(std::wstring name, Vec2 size, Rect region, Color color, Glyph glyph) {
	texture.Init(size, region);

	this->name = name;
	this->color = color;

	// Draw Panel Frame
	// Edges
	for (short x = 1; x < size.x - 1; ++x) {
		texture.SetPixel(Vec2(x, 0), L'\u2500', color);
		texture.SetPixel(Vec2(x, size.y - 1), L'\u2500', color);
	}

	for (short y = 1; y < size.y - 1; ++y) {
		texture.SetPixel(Vec2(0, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x - 1, y), L'\u2502', color);
	}

	// Corners
	texture.SetPixel(Vec2(0, 0), L'\u250C', color);
	texture.SetPixel(Vec2(size.x - 1, 0), L'\u2510', color);
	texture.SetPixel(Vec2(0, size.y - 1), L'\u2514', color);
	texture.SetPixel(Vec2(size.x - 1, size.y - 1), L'\u2518', color);

	// Pages
	texture.SetPixel(Vec2(size.x / 2 - 1, size.y - 1), L'<', color);
	texture.SetPixel(Vec2(size.x / 2 + 1, size.y - 1), L'>', color);

	// Write Panel Name
	short nameLength = (short)name.length();
	short namePosition = size.x / 2 - nameLength / 2;
	for (short i = 0; i < (short)nameLength; ++i)
		texture.SetPixel(Vec2(namePosition + i, 0), name.at(i), color);

	texture.SetPixel(Vec2(namePosition - 1, 0), L'\u2524', color);
	texture.SetPixel(Vec2(namePosition + nameLength, 0), L'\u251C', color);

	// Draw Glyphs
	DrawGlyphs(1);

	// Set Glyph
	ChangeGlyph(glyph);
}

void GlyphPanel::Update(Paint& app) {
	if (!texture.update && !app.updateVisualElements)
		return;

	app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });

	texture.update = false;
	app.updateVisualElements = false;
}

void GlyphPanel::DrawGlyphs(short pageNumber) {
	std::ifstream fin;

	if (pageNumber == 1) {
		fin.open("resources/glyphs01.in");
		texture.SetPixel(Vec2(texture.size.x / 2, texture.size.y - 1), L'1', color);
	}
	else {
		fin.open("resources/glyphs02.in");
		texture.SetPixel(Vec2(texture.size.x / 2, texture.size.y - 1), L'2', color);
	}

	// Return if file failed
	if (!fin.is_open())
		return;

	fin >> std::setbase(16);

	for (short y = 1; y < texture.size.y - 1; ++y) {
		for (short x = 1; x < texture.size.x - 1; ++x) {
			int glyph;
			fin >> glyph;

			texture.SetPixel(Vec2(x, y), (Glyph)glyph, color);
		}
	}

	fin.close();
}

void GlyphPanel::ChangeGlyph(Glyph glyph) {
	for (short x = 1; x < texture.size.x - 1; ++x)
		for (short y = 1; y < texture.size.y - 1; ++y) {
			if (texture.GetPixel(Vec2(x, y)).Attributes == (0x000E | color))
				texture.SetPixel(Vec2(x, y), texture.GetPixel(Vec2(x, y)).Char.UnicodeChar, color);
			if (texture.GetPixel(Vec2(x, y)).Char.UnicodeChar == glyph)
				texture.SetPixel(Vec2(x, y), glyph, 0x000E | color);
		}
}

void GlyphPanel::SelectGlyph(Paint& app, Vec2 mousePosition) {
	mousePosition = mousePosition - Vec2(texture.region.left, texture.region.top); // Relative to the Panel

	if ((mousePosition.x >= 1 && mousePosition.y >= 1) && (mousePosition.x <= texture.size.x - 2 && mousePosition.y <= texture.size.y - 2))
		app.ChangeGlyph(texture.GetPixel(mousePosition).Char.UnicodeChar);
}

void GlyphPanel::SelectPage(Vec2 mousePosition, Glyph glyph) {
	mousePosition = mousePosition - Vec2(texture.region.left, texture.region.top); // Relative to the Panel

	if (mousePosition == Vec2(texture.size.x / 2 - 1, texture.size.y - 1))
		DrawGlyphs(1);
	if (mousePosition == Vec2(texture.size.x / 2 + 1, texture.size.y - 1))
		DrawGlyphs(2);

	ChangeGlyph(glyph);
}




/* ####### PALETTE PANEL ####### */

PalettePanel::PalettePanel() {
	color = 0x0000;
}

bool PalettePanel::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void PalettePanel::Init(std::wstring name, Vec2 size, Rect region, Color color, Color fgColor, Color bgColor) {
	texture.Init(size, region);

	this->name = name;
	this->color = color;

	// Draw Panel Frame
	// Center
	DrawPalette();

	// Edges
	for (short x = 1; x < size.x - 1; ++x) {
		texture.SetPixel(Vec2(x, 0), L'\u2500', color);
		texture.SetPixel(Vec2(x, size.y - 1), L'\u2500', color);
	}

	for (short y = 1; y < size.y - 1; ++y) {
		texture.SetPixel(Vec2(0, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x - 1, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x / 2, y), L'\u2502', color); // Middle Line
	}

	texture.SetPixel(Vec2(size.x / 2, 0), L'\u2534', color);

	// Corners
	texture.SetPixel(Vec2(0, 0), L'\u250C', color);
	texture.SetPixel(Vec2(size.x - 1, 0), L'\u2510', color);
	texture.SetPixel(Vec2(0, size.y - 1), L'\u2514', color);
	texture.SetPixel(Vec2(size.x - 1, size.y - 1), L'\u2518', color);


	// Write Panel Name
	short nameLength = (short)name.length();
	short namePosition = size.x / 2 - nameLength / 2;
	for (short i = 0; i < (short)nameLength; ++i)
		texture.SetPixel(Vec2(namePosition + i, 0), name.at(i), color);

	texture.SetPixel(Vec2(namePosition - 1, 0), L'\u2524', color);
	texture.SetPixel(Vec2(namePosition + nameLength, 0), L'\u251C', color);

	// Set Colors
	ChangeFGColor(fgColor);
	ChangeBGColor(bgColor);
}

void PalettePanel::Update(Paint& app) {
	if (!texture.update && !app.updateVisualElements)
		return;

	app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });

	texture.update = false;
	app.updateVisualElements = false;
}

void PalettePanel::DrawPalette() {
	std::ifstream fin("resources/palette.in");

	// Return if file failed
	if (!fin.is_open())
		return;

	fin >> std::setbase(16);

	for (short y = 1; y < texture.size.y - 1; ++y)
		for (short x = 1; x < texture.size.x / 2; ++x) {
			short color;
			fin >> color;

			texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, (Color)color);
			texture.SetPixel(Vec2(texture.size.x / 2 + x, y), EMPTY_GLYPH, (Color)color);
		}

	fin.close();
}

void PalettePanel::ChangeFGColor(Color color) {
	color = color << 4;

	for (short x = 1; x < texture.size.x / 2; ++x)
		for (short y = 1; y < texture.size.y - 1; ++y) {
			if (texture.GetPixel(Vec2(x, y)).Char.UnicodeChar == L'f')
				texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, (Color)texture.GetPixel(Vec2(x, y)).Attributes);
			if ((texture.GetPixel(Vec2(x, y)).Attributes >> 4) << 4 == color)
				texture.SetPixel(Vec2(x, y), L'f', (Color)texture.GetPixel(Vec2(x, y)).Attributes);
		}
}

void PalettePanel::ChangeBGColor(Color color) {
	for (short x = texture.size.x / 2 + 1; x < texture.size.x - 1; ++x)
		for (short y = 1; y < texture.size.y - 1; ++y) {
			if (texture.GetPixel(Vec2(x, y)).Char.UnicodeChar == L'b')
				texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, (Color)texture.GetPixel(Vec2(x, y)).Attributes);
			if ((texture.GetPixel(Vec2(x, y)).Attributes >> 4) << 4 == color)
				texture.SetPixel(Vec2(x, y), L'b', (Color)texture.GetPixel(Vec2(x, y)).Attributes);
		}
}

void PalettePanel::SelectColor(Paint& app, Vec2 mousePosition) {
	mousePosition = mousePosition - Vec2(texture.region.left, texture.region.top); // Relative to the Panel

	if ((mousePosition.x >= 1 && mousePosition.y >= 1) && (mousePosition.x <= texture.size.x / 2 - 1 && mousePosition.y <= texture.size.y - 2))
		app.ChangeFGColor(texture.GetPixel(mousePosition).Attributes >> 4);
	if ((mousePosition.x >= texture.size.x / 2 + 1 && mousePosition.y >= 1) && (mousePosition.x <= texture.size.x - 2 && mousePosition.y <= texture.size.y - 2))
		app.ChangeBGColor((texture.GetPixel(mousePosition).Attributes >> 4) << 4);
}




/* ####### INPUT FIELD ####### */
InputField::InputField() {
	value = "";
	color = 0x0000;
	cursorGlyph = 0x25AC;
	cursorPosition = 0;
	maxNumOfCharacters = 100;
}

void InputField::Init(Vec2 size, Rect region, Color color) {
	texture.Init(size, region);

	this->color = color;

	for (short x = 0; x < size.x; ++x)
		for (short y = 0; y < size.y; ++y)
			texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, color);

	texture.SetPixel(Vec2(cursorPosition, 0), cursorGlyph, color);
}

void InputField::Update(Paint& app) {
	if (!texture.update && !app.updateVisualElements)
		return;

	app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });

	texture.update = false;
}

void InputField::Reset() {
	texture.Clear();
	value = "";
	cursorPosition = 0;
	texture.SetPixel(Vec2(cursorPosition, 0), cursorGlyph, color);
}

void InputField::CheckInput(Paint& app) {
	char typedCharacter = NULL;

	// Check for Pressed Number Keys
	for (char c = '0'; c <= '9'; ++c)
		if (app.KeyReleased(c) || app.KeyReleased(c + 0x30))
			typedCharacter = c;

	// Check for Pressed Letter Keys
	for (char c = 'A'; c <= 'Z'; ++c)
		if (app.KeyReleased(c))
			typedCharacter = (app.KeyPressed(VK_LSHIFT) ^ app.CapsLock()) ? c : c + 32;

	// Check for Space Key
	if (app.KeyReleased(VK_SPACE) && value != "")
		typedCharacter = ' ';

	// Check for Backspace Key
	if (app.KeyReleased(VK_BACK))
		typedCharacter = -1;

	// Update Textbox
	if (typedCharacter != NULL) {
		// Update Value
		if (typedCharacter != -1 && cursorPosition < maxNumOfCharacters) {
			value = value + typedCharacter;
			cursorPosition++;
		}
		else if (typedCharacter == -1 && !value.empty()) {
			value.pop_back();
			texture.SetPixel(Vec2(cursorPosition, 0), EMPTY_GLYPH, color);
			cursorPosition--;
		}

		// Show Value
		short indexStart = (cursorPosition < texture.size.x) ? 0 : cursorPosition - texture.size.x + 1;
		for (short i = 0; i + indexStart < (short)value.length(); ++i)
			texture.SetPixel(Vec2(i, 0), value.at(i + indexStart), color);
		texture.SetPixel(Vec2(cursorPosition, 0), cursorGlyph, color);
	}
}




/* ####### TYPE NAME PANEL ####### */

InputPanel::InputPanel() {
	color = 0x0000;
	active = false;
}

bool InputPanel::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void InputPanel::Init(std::wstring name, Vec2 size, Rect region, Color color) {
	texture.Init(size, region);

	this->name = name;
	this->color = color;

	// Draw Panel Frame
	// Center
	for (short x = 1; x < size.x - 1; ++x)
		for (short y = 1; y < size.y - 1; ++y)
			texture.SetPixel(Vec2(x, y), EMPTY_GLYPH, color);

	// Edges
	for (short x = 1; x < size.x - 1; ++x) {
		texture.SetPixel(Vec2(x, 0), L'\u2500', color);
		texture.SetPixel(Vec2(x, size.y - 1), L'\u2500', color);
	}

	for (short y = 1; y < size.y - 1; ++y) {
		texture.SetPixel(Vec2(0, y), L'\u2502', color);
		texture.SetPixel(Vec2(size.x - 1, y), L'\u2502', color);
	}

	texture.SetPixel(Vec2(size.x / 2, 0), L'\u2534', color);

	// Corners
	texture.SetPixel(Vec2(0, 0), L'\u250C', color);
	texture.SetPixel(Vec2(size.x - 1, 0), L'\u2510', color);
	texture.SetPixel(Vec2(0, size.y - 1), L'\u2514', color);
	texture.SetPixel(Vec2(size.x - 1, size.y - 1), L'\u2518', color);


	// Write Panel Name
	short nameLength = (short)name.length();
	short namePosition = size.x / 2 - nameLength / 2;
	for (short i = 0; i < (short)nameLength; ++i)
		texture.SetPixel(Vec2(namePosition + i, 0), name.at(i), color);

	texture.SetPixel(Vec2(namePosition - 1, 0), L'\u2524', color);
	texture.SetPixel(Vec2(namePosition + nameLength, 0), L'\u251C', color);

	// Initialize Textbox
	inputField.Init(Vec2(12, 1), Rect(texture.region.left + 1, texture.region.top + 1, texture.region.right - 1, texture.region.top + 1), 0x000F);
}

void InputPanel::Update(Paint& app) {
	if (texture.update || app.updateVisualElements) {
		app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });
		texture.update = false;
		app.updateVisualElements = false;
	}

	inputField.Update(app);
}

void InputPanel::Enable() {
	inputField.Reset();
	texture.update = true;
	active = true;
}

void InputPanel::Disable() {
	active = false;
}

void InputPanel::CheckInput(Paint& app) {
	// Check for Input
	inputField.CheckInput(app);

	// Set Value or Close Panel
	if (app.KeyReleased(VK_RETURN)) {
		value = inputField.value;
		Disable();
		app.updateVisualElements = true;
	}

	if (app.KeyReleased(VK_ESCAPE)) {
		Disable();
		app.updateVisualElements = true;
	}
}
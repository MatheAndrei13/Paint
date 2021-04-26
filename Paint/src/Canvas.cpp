#include "Canvas.h"
#include "Paint.h"
#include <queue>
#include <iomanip>

Canvas::Canvas() {
	updateTimeline = false;
}

bool Canvas::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void Canvas::Init(Vec2 size, Rect region) {
	texture.Init(size, region);
}

void Canvas::Update(Paint& app) {
	if (!texture.update && !app.updateVisualElements)
		return;

	app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });

	texture.update = false;
	app.updateVisualElements = false;
}

void Canvas::Save(std::ofstream& fout) {
	fout << std::setbase(16);

	for (short y = 0; y < texture.size.y; ++y) {
		for (short x = 0; x < texture.size.x; ++x)
			fout << texture.GetPixel(Vec2(x, y)).Char.UnicodeChar << " " << texture.GetPixel(Vec2(x, y)).Attributes << " ";
		fout << "\n";
	}
}

void Canvas::Load(std::ifstream& fin) {
	fin >> std::setbase(16);

	for (short y = 0; y < texture.size.y; ++y)
		for (short x = 0; x < texture.size.x; ++x) {
			int glyph;
			short color;

			fin >> glyph >> color;

			texture.SetPixel(Vec2(x, y), (Glyph)glyph, (Color)color);
		}
}

void Canvas::UpdateTimeline() {
	if (!updateTimeline)
		return;

	if (!timeline.empty() && currentTimePoint != timeline.end() - 1)
			timeline.erase(currentTimePoint + 1, timeline.end());

	if (timeline.size() == maxNumOfTimePoints)
		timeline.pop_front();

	timeline.push_back(tempTimePoint);
	currentTimePoint = timeline.end() - 1;

	tempTimePoint.oldState.clear();
	tempTimePoint.newState.clear();

	updateTimeline = false;
}

void Canvas::Undo() {
	if (timeline.empty())
		return;

	for (std::set<Pixel>::iterator pixel = currentTimePoint->oldState.begin(); pixel != currentTimePoint->oldState.end(); pixel++)
		texture.SetPixel(pixel->position, (Glyph)pixel->data.Char.UnicodeChar, (Color)pixel->data.Attributes);

	if (currentTimePoint != timeline.begin())
		currentTimePoint--;
}

void Canvas::Redo() {
	if (timeline.empty())
		return;

	if (currentTimePoint != timeline.end() - 1)
		currentTimePoint++;

	for (std::set<Pixel>::iterator pixel = currentTimePoint->newState.begin(); pixel != currentTimePoint->newState.end(); pixel++)
		texture.SetPixel(pixel->position, (Glyph)pixel->data.Char.UnicodeChar, (Color)pixel->data.Attributes);
}

void Canvas::Draw(Vec2 vec2, Glyph glyph, Color color) {
	Pixel pixel;
	pixel.position = vec2;
	pixel.data = texture.GetPixel(vec2);

	tempTimePoint.oldState.insert(pixel);

	texture.SetPixel(vec2, glyph, color);

	pixel.data = { glyph, color };

	tempTimePoint.newState.insert(pixel);

	updateTimeline = true;
}

void Canvas::Line(Vec2 start, Vec2 end, Glyph glyph, Color color) {
	short dx = abs(end.x - start.x);
	short dy = -abs(end.y - start.y);

	short x_step = (start.x < end.x) ? 1 : -1;
	short y_step = (start.y < end.y) ? 1 : -1;

	Vec2 currentPos = Vec2(start.x, start.y);

	short err = dx + dy;
	while (true) {
		Draw(currentPos, glyph, color);
		if (currentPos == end)
			break;
		short e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			currentPos.x += x_step;
		}
		if (e2 <= dx) {
			err += dx;
			currentPos.y += y_step;
		}
	}
}

void Canvas::LineBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app) {
	short dx = abs(end.x - start.x);
	short dy = -abs(end.y - start.y);

	short x_step = (start.x < end.x) ? 1 : -1;
	short y_step = (start.y < end.y) ? 1 : -1;

	Vec2 currentPos = Vec2(start.x, start.y);

	short err = dx + dy;
	while (true) {
		if (inBounds(currentPos))
			app.SetPixel(currentPos, glyph, color);
		if (currentPos == end)
			break;
		short e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			currentPos.x += x_step;
		}
		if (e2 <= dx) {
			err += dx;
			currentPos.y += y_step;
		}
	}

	texture.update = true;
}

void Canvas::Rectangle(Vec2 start, Vec2 end, Glyph glyph, Color color) {
	if (start.x <= end.x && start.y > end.y) {
		short aux = start.y;
		start.y = end.y;
		end.y = aux;
	}
	else if (start.x > end.x && start.y <= end.y) {
		short aux = start.x;
		start.x = end.x;
		end.x = aux;
	}
	else if (start.x > end.x && start.y > end.y) {
		Vec2 aux = start;
		start = end;
		end = aux;
	}

	for (short x = start.x; x <= end.x; ++x) {
		Draw(Vec2(x, start.y), glyph, color);
		Draw(Vec2(x, end.y), glyph, color);
	}
	for (short y = start.y + 1; y < end.y; ++y) {
		Draw(Vec2(start.x, y), glyph, color);
		Draw(Vec2(end.x, y), glyph, color);
	}
}

void Canvas::RectangleBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app) {
	if (start.x <= end.x && start.y > end.y) {
		short aux = start.y;
		start.y = end.y;
		end.y = aux;
	}
	else if (start.x > end.x && start.y <= end.y) {
		short aux = start.x;
		start.x = end.x;
		end.x = aux;
	}
	else if (start.x > end.x && start.y > end.y) {
		Vec2 aux = start;
		start = end;
		end = aux;
	}

	for (short x = start.x; x <= end.x; ++x) {
		if (inBounds(Vec2(x, start.y)))
			app.SetPixel(Vec2(x, start.y), glyph, color);
		if (inBounds(Vec2(x, end.y)))
			app.SetPixel(Vec2(x, end.y), glyph, color);
	}
	for (short y = start.y + 1; y < end.y; ++y) {
		if (inBounds(Vec2(start.x, y)))
			app.SetPixel(Vec2(start.x, y), glyph, color);
		if (inBounds(Vec2(end.x, y)))
			app.SetPixel(Vec2(end.x, y), glyph, color);
	}

	texture.update = true;
}

void Canvas::Ellipse(Vec2 start, Vec2 end, Glyph glyph, Color color) {
	int a = abs(end.x - start.x), b = abs(end.y - start.y), b1 = b & 1;
	long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a;
	long err = dx + dy + b1 * a * a, e2;

	if (start.x > end.x) {
		start.x = end.x;
		end.x += a;
	}
	if (start.y > end.y)
		start.y = end.y;

	start.y += (b + 1) / 2;
	end.y = start.y - b1;

	a *= 8 * a;
	b1 = 8 * b * b;

	do {
		Draw(Vec2(end.x, start.y), glyph, color);
		Draw(Vec2(start.x, start.y), glyph, color);
		Draw(Vec2(start.x, end.y), glyph, color);
		Draw(Vec2(end.x, end.y), glyph, color);

		e2 = 2 * err;
		if (e2 <= dy) {
			start.y++;
			end.y--;
			err += dy += a;
		}
		if (e2 >= dx || 2 * err > dy) {
			start.x++;
			end.x--;
			err += dx += b1;
		}
	} while (start.x <= end.x);

	while (start.y - end.y < b) {
		Draw(Vec2(start.x - 1, start.y), glyph, color);
		Draw(Vec2(end.x + 1, start.y), glyph, color);
		start.y++;
		Draw(Vec2(start.x - 1, end.y), glyph, color);
		Draw(Vec2(end.x + 1, end.y), glyph, color);
		end.y--;
	}
}

void Canvas::EllipseBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app) {
	int a = abs(end.x - start.x), b = abs(end.y - start.y), b1 = b & 1;
	long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a;
	long err = dx + dy + b1 * a * a, e2;

	if (start.x > end.x) {
		start.x = end.x;
		end.x += a;
	}
	if (start.y > end.y)
		start.y = end.y;

	start.y += (b + 1) / 2;
	end.y = start.y - b1;

	a *= 8 * a;
	b1 = 8 * b * b;

	do {
		if (inBounds(Vec2(end.x, start.y)))
			app.SetPixel(Vec2(end.x, start.y), glyph, color);
		if (inBounds(start))
			app.SetPixel(start, glyph, color);
		if (inBounds(Vec2(start.x, end.y)))
			app.SetPixel(Vec2(start.x, end.y), glyph, color);
		if (inBounds(end))
			app.SetPixel(end, glyph, color);

		e2 = 2 * err;
		if (e2 <= dy) {
			start.y++;
			end.y--;
			err += dy += a;
		}
		if (e2 >= dx || 2 * err > dy) {
			start.x++;
			end.x--;
			err += dx += b1;
		}
	} while (start.x <= end.x);

	while (start.y - end.y < b) {
		if (inBounds(Vec2(start.x - 1, start.y)))
			app.SetPixel(Vec2(start.x - 1, start.y), glyph, color);
		if (inBounds(Vec2(end.x + 1, start.y)))
			app.SetPixel(Vec2(end.x + 1, start.y), glyph, color);
		start.y++;
		if (inBounds(Vec2(start.x - 1, end.y)))
			app.SetPixel(Vec2(start.x - 1, end.y), glyph, color);
		if (inBounds(Vec2(end.x + 1, end.y)))
			app.SetPixel(Vec2(end.x + 1, end.y), glyph, color);
		end.y--;
	}

	texture.update = true;
}

void Canvas::Bucket(Vec2 vec2, Glyph glyph, Color color) {
	Glyph startingGlyph = texture.GetPixel(vec2).Char.UnicodeChar;
	Color startingColor = (Color)texture.GetPixel(vec2).Attributes;

	if (startingGlyph == glyph && startingColor == color)
		return;

	std::queue<Vec2> Q;

	Q.push(vec2);

	Draw(vec2, glyph, color);

	Vec2 directions[4] = { Vec2(0, -1), Vec2(1, 0), Vec2(0, 1), Vec2(-1, 0) };

	while (!Q.empty()) {
		vec2 = Q.front();

		for (short d = 0; d < 4; ++d) {
			Vec2 currentPos = vec2 + directions[d];
			if (inBounds(currentPos))
				if (texture.GetPixel(currentPos).Char.UnicodeChar == startingGlyph && texture.GetPixel(currentPos).Attributes == startingColor) {
					Draw(currentPos, glyph, color);
					Q.push(currentPos);
				}
		}

		Q.pop();
	}
}

void Canvas::Picker(Vec2 vec2, Paint& app) {
	app.ChangeGlyph(texture.GetPixel(vec2).Char.UnicodeChar);
	app.ChangeFGColor((Color)(texture.GetPixel(vec2).Attributes << 12 >> 12));
	app.ChangeBGColor((Color)(texture.GetPixel(vec2).Attributes) >> 4 << 4);
}

void Canvas::Selection(Vec2 start, Vec2 end, Paint& app) {
	if (start.x <= end.x && start.y > end.y) {
		short aux = start.y;
		start.y = end.y;
		end.y = aux;
	}
	else if (start.x > end.x && start.y <= end.y) {
		short aux = start.x;
		start.x = end.x;
		end.x = aux;
	}
	else if (start.x > end.x && start.y > end.y) {
		Vec2 aux = start;
		start = end;
		end = aux;
	}

	for (short x = start.x; x <= end.x; ++x) {
		if (inBounds(Vec2(x, start.y)))
			app.SetPixel(Vec2(x, start.y - 1), L'\u2500', 0x000F);
		if (inBounds(Vec2(x, end.y)))
			app.SetPixel(Vec2(x, end.y + 1), L'\u2500', 0x000F);
	}
	for (short y = start.y; y <= end.y; ++y) {
		if (inBounds(Vec2(start.x, y)))
			app.SetPixel(Vec2(start.x - 1, y), L'\u2502', 0x000F);
		if (inBounds(Vec2(end.x, y)))
			app.SetPixel(Vec2(end.x + 1, y), L'\u2502', 0x000F);
	}

	app.SetPixel(Vec2(start.x - 1, start.y - 1), L'\u250c', 0x000F);
	app.SetPixel(Vec2(end.x + 1, start.y - 1), L'\u2510', 0x000F);
	app.SetPixel(Vec2(start.x - 1, end.y + 1), L'\u2514', 0x000F);
	app.SetPixel(Vec2(end.x + 1, end.y + 1), L'\u2518', 0x000F);

	texture.update = true;
}

void Canvas::Copy(Vec2 start, Vec2 end) {
	if (start.x <= end.x && start.y > end.y) {
		short aux = start.y;
		start.y = end.y;
		end.y = aux;
	}
	else if (start.x > end.x && start.y <= end.y) {
		short aux = start.x;
		start.x = end.x;
		end.x = aux;
	}
	else if (start.x > end.x && start.y > end.y) {
		Vec2 aux = start;
		start = end;
		end = aux;
	}

	copyTexture.Reset();

	copyTexture.Init(Vec2(end.x - start.x + 1, end.y - start.y + 1), Rect(0, 0, 0, 0));

	for (short x = 0; x < copyTexture.size.x; ++x)
		for (short y = 0; y < copyTexture.size.y; ++y)
			copyTexture.SetPixel(Vec2(x, y), texture.GetPixel(Vec2(x + start.x, y + start.y)).Char.UnicodeChar, (Color)texture.GetPixel(Vec2(x + start.x, y + start.y)).Attributes);
}

void Canvas::Paste(Vec2 vec2, Paint& app) {
	if (copyTexture.buffer == nullptr)
		return;

	copyTexture.region = Rect(vec2.x, vec2.y, vec2.x + copyTexture.size.x - 1, vec2.y + copyTexture.size.y - 1);

	for (short x = 0; x < copyTexture.size.x; ++x)
		for (short y = 0; y < copyTexture.size.y; ++y)
			Draw(Vec2(x + copyTexture.region.left, y + copyTexture.region.top), copyTexture.GetPixel(Vec2(x, y)).Char.UnicodeChar, (Color)copyTexture.GetPixel(Vec2(x, y)).Attributes);
}

void Canvas::PasteBlueprint(Vec2 vec2, Paint& app) {
	if (copyTexture.buffer == nullptr)
		return;

	copyTexture.region = Rect(vec2.x, vec2.y, vec2.x + copyTexture.size.x - 1, vec2.y + copyTexture.size.y - 1);

	for (short x = 0; x < copyTexture.size.x; ++x)
		for (short y = 0; y < copyTexture.size.y; ++y)
			if (inBounds(Vec2(x + copyTexture.region.left, y + copyTexture.region.top)))
				app.SetPixel(Vec2(x + copyTexture.region.left, y + copyTexture.region.top), copyTexture.GetPixel(Vec2(x, y)).Char.UnicodeChar, (Color)copyTexture.GetPixel(Vec2(x, y)).Attributes);
	
	texture.update = true;
}

void Canvas::Clear() {
	for (short x = 0; x < texture.size.x; ++x)
		for (short y = 0; y < texture.size.y; ++y)
			Draw(Vec2(x, y), EMPTY_GLYPH, 0x0000);
}
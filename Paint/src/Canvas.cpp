#include "Canvas.h"
#include "Paint.h"
#include <queue>

Canvas::Canvas() {}

bool Canvas::inBounds(Vec2 vec2) const {
	return texture.inBounds(vec2);
}

void Canvas::Init(Vec2 size, Rect region) {
	texture.Init(size, region);
}

void Canvas::Update(Paint& app) {
	if (!texture.update)
		return;

	app.WriteToBuffer(texture.buffer, { texture.size.x, texture.size.y }, { texture.region.left, texture.region.top, texture.region.right, texture.region.bottom });

	texture.update = false;
}

void Canvas::Draw(Vec2 vec2, Glyph glyph, Color color) {
	texture.SetPixel(vec2, glyph, color);
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

void Canvas::Picker(Paint& app, Vec2 vec2) {
	app.ChangeGlyph(texture.GetPixel(vec2).Char.UnicodeChar);
	app.ChangeFGColor(texture.GetPixel(vec2).Attributes);
	app.ChangeBGColor(texture.GetPixel(vec2).Attributes);
}

void Canvas::Clear() {
	texture.Clear();
}
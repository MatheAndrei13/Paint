#pragma once
#include "../include/MathUtils.h"
#include "Texture.h"

class Paint;

class Canvas {
	Texture texture;

public:
	Canvas();

	bool inBounds(Vec2 vec2) const;

	void Init(Vec2 size, Rect region);
	void Update(Paint& app);

	void Draw(Vec2 vec2, Glyph glyph, Color color);

	void Line(Vec2 start, Vec2 end, Glyph glyph, Color color);
	void LineBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app);

	void Rectangle(Vec2 start, Vec2 end, Glyph glyph, Color color);
	void RectangleBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app);

	void Ellipse(Vec2 start, Vec2 end, Glyph glyph, Color color);
	void EllipseBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app);

	void Bucket(Vec2 vec2, Glyph glyph, Color color);

	void Clear();
};


#pragma once
#include "../include/MathUtils.h"
#include "Texture.h"
#include <fstream>
#include <deque>
#include <set>

class Paint;

class Canvas {
	Texture texture;

	using Pixel = struct { Vec2 position; CHAR_INFO data; };
	using TimePoint = struct { std::set<Pixel> oldState, newState; };

	std::deque<TimePoint> timeline;
	std::deque<TimePoint>::iterator currentTimePoint;
	TimePoint tempTimePoint;

	const short maxNumOfTimePoints = 20;
	bool updateTimeline;

	friend bool operator<(const Pixel& left, const Pixel& right) {
		if (left.position.y < right.position.y)
			return 1;
		if (left.position.y == right.position.y)
			if (left.position.x < right.position.x)
				return 1;
		return 0;
	}

public:
	Canvas();

	bool inBounds(Vec2 vec2) const;

	void Init(Vec2 size, Rect region);
	void Update(Paint& app);

	void Save(std::ofstream& fout);
	void Load(std::ifstream& fin);

	void UpdateTimeline();

	void Undo();
	void Redo();

	void Draw(Vec2 vec2, Glyph glyph, Color color);

	void Line(Vec2 start, Vec2 end, Glyph glyph, Color color);
	void LineBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app);

	void Rectangle(Vec2 start, Vec2 end, Glyph glyph, Color color);
	void RectangleBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app);

	void Ellipse(Vec2 start, Vec2 end, Glyph glyph, Color color);
	void EllipseBlueprint(Vec2 start, Vec2 end, Glyph glyph, Color color, Paint& app);

	void Bucket(Vec2 vec2, Glyph glyph, Color color);

	void Picker(Paint& app, Vec2 vec2);

	void Clear();
};


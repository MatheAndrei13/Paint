#pragma once

struct Vec2 {
	short x, y;

	Vec2();
	Vec2(short, short);

	bool operator==(const Vec2&) const;
	bool operator!=(const Vec2&) const;
	const Vec2 operator+(const Vec2&) const;
	const Vec2 operator-(const Vec2&) const;
	const Vec2 operator*(const short&) const;
	void operator+=(const Vec2&);
	void operator-=(const Vec2&);
	void operator*=(const short&);
};

struct Rect {
	short left, top, right, bottom;

	Rect();
	Rect(short, short, short, short);

	bool operator==(const Rect&) const;
	bool operator!=(const Rect&) const;
};
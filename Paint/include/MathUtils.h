#pragma once

struct Vec2 {
	int x, y;

	Vec2();
	Vec2(int, int);

	bool operator==(const Vec2&) const;
	bool operator!=(const Vec2&) const;
	const Vec2 operator+(const Vec2&) const;
	const Vec2 operator-(const Vec2&) const;
	const Vec2 operator*(const int&) const;
	void operator+=(const Vec2&);
	void operator-=(const Vec2&);
	void operator*=(const int&);
};
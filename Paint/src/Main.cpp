#include "Paint.h"

int main() {
	COORD appSize = { 64, 36 };
	COORD fontSize = { 10, 10 };

	Paint paint;
	paint.Create(L"Paint", appSize.X, appSize.Y, fontSize.X, fontSize.Y);
	paint.Start();
	return 0;
}
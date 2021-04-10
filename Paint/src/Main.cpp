#include "Paint.h"

int main() {
	COORD appSize = { 64, 36 };
	COORD fontSize = { 10, 10 };

	AddFontResourceA("D:/User/MyProjects/Visual Studio Projects/Paint/Paint/fonts/DejaVuSansMono.ttf");

	Paint paint;
	paint.Create(L"Paint", appSize.X, appSize.Y, L"DejaVu Sans Mono", fontSize.X, fontSize.Y);
	paint.Start();

	while (RemoveFontResourceA("D:/User/MyProjects/Visual Studio Projects/Paint/Paint/fonts/DejaVuSansMono.ttf"));

	return 0;
}
#include "Paint.h"
#include <fstream>

int main() {
	COORD appSize = { 120, 30 };
	COORD fontSize = { 8, 16 };

	std::ifstream in("config.txt");
	in >> appSize.X >> appSize.Y >> fontSize.X >> fontSize.Y;
	in.close();

	Paint paint;
	paint.Create(L"Paint", appSize.X, appSize.Y, fontSize.X, fontSize.Y);
	paint.Start();
	return 0;
}
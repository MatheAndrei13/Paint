#pragma once
#include <Windows.h>
#include <string>
#include "MathUtils.h"

class ConsoleApp {
private:
	std::wstring appName;
	HANDLE handleIn;
	HANDLE handleOut;
	SMALL_RECT windowSize;
	COORD screenSize;
	COORD fontSize;

	CHAR_INFO* screenBuffer;
	bool updateBuffer = false;

	bool invalidConsole = true;
	bool running = false;

	struct Key {
		bool oldState = false;
		bool newState = false;
	};

	struct MouseInput {
		static constexpr short numButtons = 2;
		COORD position = { 0, 0 };
		Key buttons[numButtons];
	} mouseInput;

	struct KeyboardInput {
		static constexpr short numKeys = 256;
		Key keys[numKeys];
	} keyboardInput;

public:
	ConsoleApp();
	void Create(std::wstring appName, short screenWidth, short screenHeight, std::wstring fontFace, short fontWidth, short fontHeight);
	~ConsoleApp();

	void Start();
	void Close();

	void ChangeFontSize(short fontWidth, short fontHeight);

private:
	void Run();
	void GetInput();
	void GetMouseInput(INPUT_RECORD* inputBuffer, DWORD numEvents);
	void GetKeyboardInput();

	void ResetConsole();
	void Error(const wchar_t* message);

protected:
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;

	// GETTERS
public:
	Vec2 GetMousePosition() const;
	bool KeyPressed(const unsigned char) const;
	bool KeyReleased(const unsigned char) const;
	bool KeyHeld(const unsigned char) const;

	short GetScreenWidth() const;
	short GetScreenHeight() const;
	Vec2 GetFontSize() const;

	// TOOLS
public:
	bool OutOfBounds(short x, short y) const;

	void SetPixel(short x, short y, wchar_t glyph, unsigned char color);
	void SetPixel(Vec2 vec2, wchar_t glyph, unsigned char color);
	void Fill(short x0, short y0, short x1, short y1, wchar_t glyph, unsigned char color);
	void Fill(Vec2 vec2_0, Vec2 vec2_1, wchar_t glyph, unsigned char color);

	void WriteToBuffer(CHAR_INFO* buffer, COORD bufferSize, SMALL_RECT region);

	void ClearScreen();
};
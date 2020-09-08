#pragma once

enum class FontFeature
{
    ANTIALIASED_FONTS = 1,
    DROPSHADOW_FONTS = 2,
    OUTLINE_FONTS = 6,
};

enum class FontDrawType
{
    DEFAULT = 0,
    NONADDITIVE,
    ADDITIVE,
    TYPE_COUNT = 2,
};

enum class FontFlags : int
{
    NONE,
    ITALIC = 0x001,
    UNDERLINE = 0x002,
    STRIKEOUT = 0x004,
    SYMBOL = 0x008,
    ANTIALIAS = 0x010,
    GAUSSIANBLUR = 0x020,
    ROTARY = 0x040,
    DROPSHADOW = 0x080,
    ADDITIVE = 0x100,
    OUTLINE = 0x200,
    CUSTOM = 0x400,
    BITMAP = 0x800,
};

class ISurface
{
public:
	unsigned long SourceCreateFont()
	{
		return reinterpret_cast<unsigned long(*)(void)>(reinterpret_cast<void***>(this)[0][71])();
	}

	bool SetFont(unsigned long font, const char* fontName, int tall, int weight, int blur, int scanlines, FontFlags flags, int rangeMin = 0, int rangeMax = 0)
	{
		return reinterpret_cast<bool(__thiscall*)(void*, unsigned, const char*, int, int, int, int, FontFlags, int, int)>(reinterpret_cast<void***>(this)[0][72])(this, font, fontName, tall, weight, blur, scanlines, flags, rangeMin, rangeMax);
	}

	void DrawTextW(const wchar_t* text, int x, int y, unsigned long font, int r, int g, int b)
	{
		size_t len = 0;
		while (text[++len]);

		reinterpret_cast<void(__thiscall*)(void*, unsigned long)>(reinterpret_cast<void***>(this)[0][23])(this, font); // set text font
		reinterpret_cast<void(__thiscall*)(void*, int, int, int, int)>(reinterpret_cast<void***>(this)[0][25])(this, r, g, b, 255); // set font color
		reinterpret_cast<void(__thiscall*)(void*, int, int)>(reinterpret_cast<void***>(this)[0][26])(this, x, y); // Set Text position
		reinterpret_cast<void(__thiscall*)(void*, const wchar_t*, int, int)>(reinterpret_cast<void***>(this)[0][28])(this, text, len, 0); // draw print text
	}
};
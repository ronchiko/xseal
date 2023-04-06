#pragma once

#ifdef SEAL_WINDOWS

#include <ostream>
#include <unordered_map>

#include <Windows.h>

#include "conosle_color.inl"

static const std::unordered_map<console_color, WORD> COLOR_MAPPING{
	{ console_color::Red, FOREGROUND_INTENSITY | FOREGROUND_RED },
	{ console_color::Blue, FOREGROUND_INTENSITY | FOREGROUND_BLUE },
	{ console_color::Green, FOREGROUND_INTENSITY | FOREGROUND_GREEN },
	{ console_color::Yellow, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN }
};

struct color_context
{
	explicit color_context(console_color color)
		: m_ConsoleHandle(INVALID_HANDLE_VALUE)
		, m_LastColor(0)
	{
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		m_ConsoleHandle = console;

		CONSOLE_SCREEN_BUFFER_INFO csbiInfo{};
		GetConsoleScreenBufferInfo(m_ConsoleHandle, &csbiInfo);
		m_LastColor = csbiInfo.wAttributes;

		SetConsoleTextAttribute(m_ConsoleHandle, COLOR_MAPPING.at(color));
	}

	~color_context() {
		SetConsoleTextAttribute(m_ConsoleHandle, m_LastColor);
	}

private:
	HANDLE m_ConsoleHandle;
	WORD m_LastColor;
};

#endif

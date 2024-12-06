#pragma once
#include "pch.h"

class Logger
{
public:
	enum class Level
	{
		LVL_INFO,
		LVL_TRACE,
		LVL_WARNING,
		LVL_ERROR,
	};

public:
	static void Log(Level level, const std::string& message);
};

#define LOG_INFO(...)    Logger::Log(Logger::Level::LVL_INFO, __VA_ARGS__)
#define LOG_TRACE(...)   Logger::Log(Logger::Level::LVL_TRACE, __VA_ARGS__)
#define LOG_WARNING(...) Logger::Log(Logger::Level::LVL_WARNING, __VA_ARGS__)
#define LOG_ERROR(...)   Logger::Log(Logger::Level::LVL_ERROR, __VA_ARGS__)
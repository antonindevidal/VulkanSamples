#include "Logger.hpp"

void Logger::Log(Level level, const std::string& message)
{	
	std::stringstream buffer;

	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	
	switch (level)
	{
	case Logger::Level::LVL_INFO:
		buffer << "\033[1;32m";
		break;
	case Logger::Level::LVL_TRACE:
		buffer << "\033[1;37m";
		break;
	case Logger::Level::LVL_WARNING:
		buffer << "\033[1;33m";
		break;
	case Logger::Level::LVL_ERROR:
		buffer << "\033[1;31m";
		break;
	default:
		break;
	}

	buffer << std::put_time(&tm, "[%H:%M:%S] ");
	buffer << message;
	buffer << "\033[0m\n";

	std::cout << buffer.str();
}

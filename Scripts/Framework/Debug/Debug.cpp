#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"
#include <fstream>
#include <sstream>

bool Debug::consoleEnabled = false;
std::ofstream Debug::logFile;

void Debug::EnableConsole()
{
	if (!consoleEnabled)
	{
		AllocConsole();

		if (freopen("CONOUT$", "w", stdout) == nullptr)
		{
			std::cerr << "Failed to redirect stdout to console." << std::endl;
		}

		if (freopen("CONOUT$", "w", stderr) == nullptr)
		{
			std::cerr << "Failed to redirect stderr to console." << std::endl;
		}

		consoleEnabled = true;
		std::cout << "Console initialized." << std::endl;
	}
}

void Debug::DisableConsole()
{
	if (consoleEnabled)
	{
		FreeConsole();
		consoleEnabled = false;
	}
}

void Debug::EnsureConsole()
{
	if (!consoleEnabled)
	{
		EnableConsole();
	}
}

void Debug::OutputMessage(const std::string& message, LogLevel level)
{
	EnsureConsole();

	std::string levelStr;
	switch (level)
	{
		case LogLevel::DEBUG:  levelStr = "[DEBUG] "; break;
		case LogLevel::WARN:   levelStr = "[WARN] "; break;
		case LogLevel::ERR:    levelStr = "[ERROR] "; break;
	}

	std::string logMessage = levelStr + message;

	if (consoleEnabled)
	{
		std::cout << logMessage << std::endl;
	}

	OutputDebugStringA((logMessage + "\n").c_str());

	if (logFile.is_open())
	{
		logFile << logMessage << std::endl;
	}
}

void Debug::Log(const std::string& message, LogLevel level)
{
	OutputMessage(message, level);
}

void Debug::Log(const char* message, LogLevel level)
{
	OutputMessage(std::string(message), level);
}

void Debug::Log(int value, LogLevel level)
{
	OutputMessage(std::to_string(value), level);
}

void Debug::Log(float value, LogLevel level)
{
	OutputMessage(std::to_string(value), level);
}

void Debug::Log(const DirectX::XMFLOAT2& value, LogLevel level)
{
	std::stringstream ss;
	ss << "XMFLOAT2(" << value.x << ", " << value.y << ")";
	OutputMessage(ss.str(), level);
}

void Debug::Log(const DirectX::XMFLOAT3& value, LogLevel level)
{
	std::stringstream ss;
	ss << "XMFLOAT3(" << value.x << ", " << value.y << ", " << value.z << ")";
	OutputMessage(ss.str(), level);
}

void Debug::Log(const void* ptr, LogLevel level)
{
	std::ostringstream ss;
	ss << "Address: " << ptr;
	OutputMessage(ss.str(), level);
}

void Debug::Log(bool value, LogLevel level)
{
	OutputMessage(value ? "True" : "False", level);
}

void Debug::EnableFileLogging(const std::string& filename)
{
	logFile.open(filename, std::ios::out | std::ios::app);
	if (logFile.is_open())
	{
		Log("File logging started.", LogLevel::DEBUG);
	}
}

void Debug::DisableFileLogging()
{
	if (logFile.is_open())
	{
		Log("File logging ended.", LogLevel::DEBUG);
		logFile.close();
	}
}

void Debug::PrintHResultError(HRESULT hr)
{
	if (FAILED(hr))
	{
		std::stringstream ss;
		ss << "DirectX Error: " << std::hex << hr;
		Log(ss.str(), LogLevel::ERR);
	}
}
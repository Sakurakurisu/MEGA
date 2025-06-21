#pragma once

#include <windows.h>
#include <iostream>
#include <DirectXMath.h>

class Debug
{
public:
	enum class LogLevel {
		DEBUG, WARN, ERR
	};

	static void EnableConsole();
	static void DisableConsole();

	static void Log(const std::string& message, LogLevel level = LogLevel::DEBUG);

	static void Log(const char* message, LogLevel level = LogLevel::DEBUG);

	static void Log(int value, LogLevel level = LogLevel::DEBUG);

	static void Log(float value, LogLevel level = LogLevel::DEBUG);

	static void Log(const DirectX::XMFLOAT2& value, LogLevel level = LogLevel::DEBUG);

	static void Log(const DirectX::XMFLOAT3& value, LogLevel level = LogLevel::DEBUG);

	static void Log(const void* ptr, LogLevel level = LogLevel::DEBUG);

	static void Log(bool value, LogLevel level = LogLevel::DEBUG);

	static void EnableFileLogging(const std::string& filename);
	static void DisableFileLogging();

	static void PrintHResultError(HRESULT hr);

private:
	static bool consoleEnabled;
	static std::ofstream logFile;

	static void EnsureConsole();
	static void OutputMessage(const std::string& message, LogLevel level);
};

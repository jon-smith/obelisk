#pragma once

#include <chrono>
#include <string>
#include <thread>

namespace obelisk
{
namespace platform_utilities
{

std::wstring currentOSUsername();

void outputDebugString(const std::wstring &str);
void outputDebugString(const std::string &str);

// This is just a rough check - not a guarantee that the file can be created
bool isValidFilename(const std::wstring filename);

std::wstring getMyDocumentsPath();
std::wstring getLocalAppDataPath(const std::wstring &appName);
std::wstring getGlobalAppDataPath(const std::wstring &appName);
std::wstring getExecutablePath();
std::wstring getExecutableDirectory();

// windows only functions
namespace windows
{
bool beep(unsigned int frequencyHz, std::chrono::milliseconds time);

enum class ThreadPriority
{
	BACKGROUND,
	NORMAL,
	HIGH,
	VERY_HIGH
};
bool setThreadPriority(std::thread &thread, ThreadPriority priority);
bool setCurrentThreadPriority(ThreadPriority priority);

unsigned getCurrentProcessThreadCount();

bool delayLoadDLL(std::wstring dllname);
} // namespace windows

}; // namespace platform_utilities
}; // namespace obelisk

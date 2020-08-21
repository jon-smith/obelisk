#include "obelisk/PlatformSpecific.h"

#include <iostream>
#include <set>
#include <type_traits>

#include <NSSystemDirectories.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <sys/sysctl.h>
#include <unistd.h>

#include "obelisk/StringFuncs.h"

namespace
{

bool IsDebuggerPresent()
{
	int mib[4];
	struct kinfo_proc info;
	size_t size;

	info.kp_proc.p_flag = 0;
	mib[0] = CTL_KERN;
	mib[1] = KERN_PROC;
	mib[2] = KERN_PROC_PID;
	mib[3] = getpid();

	size = sizeof(info);
	sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);

	return ((info.kp_proc.p_flag & P_TRACED) != 0);
}

enum class DirectoryType
{
	DOCUMENT,
	APP_SUPPORT
};
enum class DirectoryDomainMask
{
	USER,
	LOCAL
};

std::string GetDirectory(DirectoryType dir, DirectoryDomainMask mask)
{

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

	auto convertDir = [dir]() -> std::optional<NSSearchPathDirectory> {
		switch (dir)
		{
		case DirectoryType::DOCUMENT:
			return NSDocumentDirectory;
		case DirectoryType::APP_SUPPORT:
			return NSApplicationSupportDirectory;
		}
		return {};
	};

	auto convertMask = [mask]() -> std::optional<NSSearchPathDomainMask> {
		switch (mask)
		{
		case DirectoryDomainMask::USER:
			return NSUserDomainMask;
		case DirectoryDomainMask::LOCAL:
			return NSLocalDomainMask;
		}
		return {};
	};

	const auto dirNS = convertDir();
	const auto maskNS = convertMask();

	if (dirNS && maskNS)
	{

		char path[PATH_MAX] = {0};
		auto state = NSStartSearchPathEnumeration(dirNS.value(), maskNS.value());
		while ((state = NSGetNextSearchPathEnumeration(state, path)))
		{
		}

#pragma clang diagnostic pop

		return path;
	}

	return {};
}

} // namespace

namespace obelisk
{
namespace platform_utilities
{

std::wstring currentOSUsername()
{
	if (const auto username = getenv("USER"))
	{
		return strToWStr(username);
	}
	return L"";
}

void outputDebugString(const std::wstring &str)
{
	if (IsDebuggerPresent())
		std::wcout << str << std::endl;
}

void outputDebugString(const std::string &str)
{
	if (IsDebuggerPresent())
		std::cout << str << std::endl;
}

bool isValidFilename(const std::wstring filename)
{
	// Make sure string and wstring are the same
	if (filename != strToWStr(wStrToStr(filename)))
		return false;

	if (filename.empty())
		return false;

	// Contains invalid characters
	if (filename.find_first_of(L"<>:\"/\\|?*") != std::wstring::npos)
		return false;

	// Ends with a fullstop
	if (*filename.crbegin() == L'.')
		return false;

	// Reserved filenames
	static const std::set<std::wstring> reservedFilenames = {L"CON",  L"PRN",  L"AUX",	L"NUL",	 L"COM1", L"COM2", L"COM3", L"COM4", L"COM5", L"COM6", L"COM7",
															 L"COM8", L"COM9", L"LPT1", L"LPT2", L"LPT3", L"LPT4", L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9"};

	if (reservedFilenames.find(filename) != reservedFilenames.end())
		return false;

	return true;
}

std::wstring getMyDocumentsPath()
{
	const auto path = GetDirectory(DirectoryType::DOCUMENT, DirectoryDomainMask::USER);

	if (!path.empty())
		return strToWStr(path);

	if (const auto homeDir = getenv("HOME"))
	{
		return strToWStr(homeDir) + L"/Documents";
	}
	return L"";
}

std::wstring getLocalAppDataPath(const std::wstring &appName)
{
	const auto path = GetDirectory(DirectoryType::APP_SUPPORT, DirectoryDomainMask::USER);

	return strToWStr(path) + L"/" + appName;
}

std::wstring getGlobalAppDataPath(const std::wstring &appName)
{

	const auto path = GetDirectory(DirectoryType::APP_SUPPORT, DirectoryDomainMask::LOCAL);

	return strToWStr(path) + L"/" + appName;
}

std::wstring getExecutablePath()
{
	char buf[PATH_MAX] = {0};
	uint32_t bufsize = PATH_MAX;
	if (!_NSGetExecutablePath(buf, &bufsize))
		puts(buf);

	return strToWStr(buf);
}

std::wstring getExecutableDirectory()
{
	const auto exePath = getExecutablePath();
	const auto finalSlash = exePath.find_last_of(L"/");
	if (finalSlash != std::string::npos)
	{
		return exePath.substr(0, finalSlash);
	}
	return exePath;
}

namespace windows
{
bool beep(unsigned int frequencyHz, std::chrono::milliseconds time)
{
	outputDebugString("beep() not supported");
	return false;
}

bool setThreadPriority(std::thread &thread, ThreadPriority priority)
{
	outputDebugString("setThreadPriority not implemented");
	return false;
}

bool setCurrentThreadPriority(ThreadPriority priority)
{
	outputDebugString("setCurrentThreadPriority not implemented");
	return false;
}

unsigned getCurrentProcessThreadCount()
{
	outputDebugString("getCurrentProcessThreadCount not implemented");
	return 0;
}

bool delayLoadDLL(std::wstring dllname)
{
	outputDebugString("Delay load not supported");
	return false;
}
} // namespace windows
} // namespace platform_utilities
} // namespace obelisk

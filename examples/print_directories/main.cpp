#include <obelisk/PlatformSpecific.h>

#include <iostream>

using namespace obelisk::platform_utilities;

int main()
{
	const auto username = currentOSUsername();
	std::wcout << L"Username: " << username << std::endl;

	const auto docsDir = getMyDocumentsPath();
	std::wcout << L"Docs: " << docsDir << std::endl;

	const auto userAppDir = getLocalAppDataPath(L"test-app");
	std::wcout << L"User App: " << userAppDir << std::endl;

	const auto systemAppDir = getGlobalAppDataPath(L"test-app");
	std::wcout << L"System App: " << systemAppDir << std::endl;

	const auto exePath = getExecutablePath();
	std::wcout << L"Exe: " << exePath << std::endl;

	const auto exeDir = getExecutableDirectory();
	std::wcout << L"Exe Path: " << exeDir << std::endl;

	return 0;
}
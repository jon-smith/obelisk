#include "stdafx.h"
#include "Logging.h"

#include <iostream>

#include "PlatformSpecific.h"
#include "FormatString.hpp"

namespace obelisk_exp
{
	void LOG(const std::string &experimentName, const std::string &logKey, const std::string &message)
	{
		// Create a simple string with all the info and write it to the console and debug console
		const auto str = obelisk::formatString("%s - %s: %s\n", experimentName.c_str(), logKey.c_str(), message.c_str());
		std::cout << str;
		obelisk::platform_utilities::outputDebugString(str);
	}
}
// ObeliskExperiments.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <map>
#include <functional>

#include "ThreadLimits.h"
#include "StringFuncs.h"

using namespace obelisk_exp;

void runExperiment(const std::string &name)
{
	auto const adjName = obelisk::toLower(name);

	static const auto experiments = std::map<std::string, std::function<void(void)>>
	{ 
		{"future", []() {multiConcurrentStdFutureTimings(2, 2, 12); }},
		{"thread", []() {multiConcurrentStdThreadTimings(2, 2, 12); }},
		{"future2", []() {multiConcurrentDualDependanceStdFutureTimings(2, 2, 8); }}
	};

	if (name == "?")
	{
		std::cout << "\nAvailable experiments:\n";
		for (auto const e : experiments)
			std::cout << e.first << "\n";

		std::cout << "\n";
	}
	else
	{
		if (auto const it = experiments.find(adjName); it != experiments.end())
		{
			it->second();
		}
		else
		{
			std::cout << "\nSelected experiment could not be found.\n";
		}
	}
}

int main()
{
	std::string s;

	do
	{
		if (!s.empty())
			runExperiment(s);

		std::cout << "\nType the name of the experiment you would like to run and click enter\n(Type '?' for a list of available experiments)\n";
		std::cin >> s;
	} while (!s.empty());

	return 0;
}


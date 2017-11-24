#pragma once

#include <string>

namespace obelisk_exp
{
	void LOG(const std::string &experimentName, const std::string &logKey, const std::string &message);

	// Helper class for convenience
	class ExperimentLogger
	{
	public:
		ExperimentLogger(const std::string &experimentName) :
			experimentName(experimentName)
		{}

		void LOG(const std::string &logKey, const std::string &message)
		{
			obelisk_exp::LOG(experimentName, logKey, message);
		}

	private:
		const std::string experimentName;
	};
}
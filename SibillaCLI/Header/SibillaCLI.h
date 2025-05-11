#pragma once

#include <SibillaCommon.h>
#include <Utils/Singleton.h>

#include <map>
#include <string>

namespace scli
{
	class SibillaCLI
	{
	public:
		SibillaCLI();
		~SibillaCLI();

		bool run();

	private:
		bool m_IsRunning = false;

		std::map<std::string, scom::Adapter *> m_Adapters;
		std::map<std::string, scom::ChatBot *> m_Bots;
	};
} // namespace scli
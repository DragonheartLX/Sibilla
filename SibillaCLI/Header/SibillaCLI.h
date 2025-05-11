#pragma once

#include <SibillaCommon.h>
#include <Utils/Singleton.h>

#include <vector>

namespace scli
{
	struct Instance
	{
		scom::AdapterInfo adapterInfo = {};
		scom::ChatBotInfo chatBotInfo = {};

		scom::Adapter *adapter		  = nullptr;
		scom::ChatBot *chatBot		  = nullptr;
	};

	class SibillaCLI
	{
	public:
		SibillaCLI();
		~SibillaCLI();

		bool run();

	private:
		bool m_IsRunning = false;

		std::vector<Instance> m_Instances;
	};
} // namespace scli
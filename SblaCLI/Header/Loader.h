#pragma once

#include <SblaCore/Config.h>
#include <SblaInterface/Plugin.h>

#include <dylib.hpp>
#include <string>

namespace sbla
{

	class Loader
	{
		using LoadPluginFunc   = void (*)(PluginInfo*);
		using LoadPlatformFunc = IPlatform* (*)(std::string, Config*);
		using LoadChatbotFunc  = IChatbot* (*)(std::string, Config*);

	public:
		Loader();
		Loader(const std::string& path, const std::string& name);
		~Loader();

		bool load();
		IPlatform* createPlatform(const std::string& name, Config config);
		IChatbot* createChatbot(const std::string& name, Config config);
		void exit();

		PluginInfo* getInfo();

	private:
		std::string m_Path;
		std::string m_Name;

		dylib* m_Lib;
		PluginInfo* m_Info;

		LoadPluginFunc m_PluginFunc;
		LoadPlatformFunc m_PlatformFunc;
		LoadChatbotFunc m_ChatbotFunc;
	};
} // namespace sbla
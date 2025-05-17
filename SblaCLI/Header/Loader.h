#pragma once

#include <SblaInterface/Plugin.h>

#include <dylib.hpp>
#include <string>

namespace sbla
{

	class Loader
	{
		using LoadPluginFunc   = void (*)(PluginInfo*);
		using LoadPlatformFunc = void (*)(std::string, IPlatform*);
		using LoadChatbotFunc  = void (*)(std::string, IChatbot*);

	public:
		Loader();
		Loader(const std::string& path, const std::string& name);
		~Loader();

		bool load();
		// void exec(InitInfo* info);
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
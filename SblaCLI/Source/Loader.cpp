#include "Loader.h"

#include <dylib.hpp>

#include "Console.h"
#include "SblaInterface/Plugin.h"

namespace sbla
{
	Loader::Loader() : m_Path(""), m_Name(""), m_Lib(nullptr), m_Info(nullptr) {};

	Loader::Loader(const std::string& path, const std::string& name) : m_Path(path), m_Name(name), m_Lib(nullptr), m_Info(nullptr) {};

	Loader::~Loader()
	{
		if (m_Info != nullptr) delete m_Info;
		if (m_Lib != nullptr) delete m_Lib;
	}

	bool Loader::load()
	{
		try
		{
			m_Lib		 = new dylib(m_Path, m_Name, dylib::no_filename_decorations);
			m_PluginFunc = m_Lib->get_function<void(PluginInfo*)>("loadPlugin");
		}
		catch (const dylib::load_error& e)
		{
			Console::warn("Load {0} error, skip.\nError: {1}", m_Name, e.what());
			return false;
		}
		catch (const dylib::symbol_error& e)
		{
			Console::warn("Get {0} loadPlugin symbol error, skip.\nError: {1}", m_Name, e.what());
			return false;
		}

		m_Info		   = new PluginInfo;
		m_Info->logger = Console::Instance().get();
		m_PluginFunc(m_Info);

		try
		{
			if (m_Info->type == "Platform" || m_Info->type == "ALL")
				m_PlatformFunc = m_Lib->get_function<IPlatform*(std::string, Config*)>("loadPlatForm");

			if (m_Info->type == "Chatbot" || m_Info->type == "ALL")
				m_ChatbotFunc = m_Lib->get_function<IChatbot*(std::string, Config*)>("loadChatbot");

			if (m_Info->type != "Platform" && m_Info->type != "Chatbot" && m_Info->type != "ALL")
			{
				Console::warn("Unknow plugin type: {0} in {1}, skip.", m_Info->type, m_Name);
				return false;
			}
		}
		catch (const dylib::symbol_error& e)
		{
			Console::warn("Get {0} createPlugin symbol error, skip.\nError: {1}", m_Name, e.what());
			return false;
		}

		return true;
	}

	IPlatform* Loader::createPlatform(const std::string& name, Config config)
	{
		IPlatform* ptr = nullptr;
		ptr			   = m_PlatformFunc(name, &config);
		return ptr;
	}

	IChatbot* Loader::createChatbot(const std::string& name, Config config)
	{
		IChatbot* ptr = nullptr;
		ptr			  = m_ChatbotFunc(name, &config);
		return ptr;
	}

	PluginInfo* Loader::getInfo() { return m_Info; }
} // namespace sbla
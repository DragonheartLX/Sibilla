#include "SibillaCLI.h"
#include "Console.h"

#include <curl/curl.h>

namespace scli
{
	SibillaCLI::SibillaCLI()
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
		Console::debug("Init libcurl");
		Console::debug("libcurl verion: {0}", curl_version());
		// scli::Console::debug("WorkspaceRoot: {0}",
		// std::filesystem::current_path().string());

		m_Bots.push_back(new ChatBot(AdapterType::OneBot));
	}

	SibillaCLI::~SibillaCLI()
	{
		for (ChatBot* ptr : m_Bots) delete ptr;

		curl_global_cleanup();
	}

	bool SibillaCLI::run()
	{
		for (ChatBot* ptr : m_Bots) ptr->run();

		while (m_IsRunning)
		{
		}

		return true;
	}
} // namespace scli
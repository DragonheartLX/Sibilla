#include "Loader.h"

// #include <SblaInterface/Exports.h>

#include <dylib.hpp>

#include "Console.h"

namespace sbla
{
	Loader::Loader() : m_Path(""), m_Name(""), m_Lib(nullptr) {};

	Loader::Loader(const std::string& path, const std::string& name) : m_Path(path), m_Name(name), m_Lib(nullptr) {};

	Loader::~Loader()
	{
		if (m_Lib != nullptr) delete m_Lib;
	}

	bool Loader::load()
	{
		try
		{
			m_Lib = new dylib(m_Path, m_Name);
			// m_Func = m_Lib->get_function<void(InitInfo*)>("init");
		}
		catch (const dylib::load_error&)
		{
			Console::warn("Load {0} error, skip.", m_Name);
			return false;
		}
		catch (const dylib::symbol_error&)
		{
			Console::warn("Get {0} init symbol error, skip.", m_Name);
			return false;
		}

		return true;
	}

	// void Loader::exec(InitInfo* info)
	// {
	// 	info->logger = Console::getInstance().get();
	// 	m_Func(info);
	// }
} // namespace sbla
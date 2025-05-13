#pragma once

#include <dylib.hpp>
#include <string>

#include "SblaInterface/Exports.h"

namespace sbla
{
	using DyLibInitFunc = void (*)(InitInfo*);

	class Loader
	{
	public:
		Loader();
		Loader(const std::string& path, const std::string& name);
		~Loader();

		bool load();
		void exec(InitInfo* info);
		void exit();

	private:
		std::string m_Path;
		std::string m_Name;

		dylib* m_Lib;
		DyLibInitFunc m_Func;
	};
} // namespace sbla
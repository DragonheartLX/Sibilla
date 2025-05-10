#pragma once
#include <memory>
#include <mutex>

namespace scli
{
	template <typename T>
	class Singleton
	{
	public:
		static bool init()
		{
			std::shared_ptr<T> ptr = getInstance();
			if (ptr == nullptr) return false;
			return true;
		};

		static std::shared_ptr<T> getInstance()
		{
			static std::once_flag s_Flag;
			std::call_once(s_Flag,
						   [&]() { s_Instance = std::shared_ptr<T>(new T); });

			return s_Instance;
			;
		};

		~Singleton() = default;

	protected:
		Singleton() = default;
		Singleton(const Singleton<T>&) = delete;
		Singleton& operator=(const Singleton<T>&) = delete;

		static std::shared_ptr<T> s_Instance;
	};

	template <typename T>
	std::shared_ptr<T> Singleton<T>::s_Instance = nullptr;
} // namespace scli
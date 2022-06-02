#pragma once
namespace dae
{
	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()		// for this to work the derived class's constructor needs to be public ??
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default;
	};
}
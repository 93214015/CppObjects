#pragma once

#include <utility>

template<typename R, typename...Ts> 
class Delegate
{};

template<typename R, typename...Ts>
class Delegate<R(Ts...)>
{

public:

	using ProxyPointer = R(*)(void*, Ts...);

	template<R(*FunctionPointer)(Ts...)>
	static R FunctionProxy(void* _Instance, Ts... Args)
	{
		return FunctionPointer(std::forward<Ts>(Args)...);
	}

	template<class T, R(T::*MethodPointer)(Ts...Args)>
	static R MethodProxy(void* Instance, Ts... Args)
	{
		return (static_cast<T*>(Instance)->*MethodPointer)(std::forward<Ts>(Args)...);
	}

	template<R(*FunctionPointer)(Ts...)>
	void Bind()
	{
		m_Proxy = &FunctionProxy<FunctionPointer>;
	}

	template<class T, R(T::*MethodPointer)(Ts...)>
	void Bind(T* _Instance)
	{
		m_Instance = _Instance;
		m_Proxy = &MethodProxy<T, MethodPointer>;
	}

	R Invoke(Ts... Args)
	{
		return m_Proxy(m_Instance, std::forward<Ts>(Args)...);
	}

private:
	void* m_Instance = nullptr;
	ProxyPointer m_Proxy = nullptr;
};

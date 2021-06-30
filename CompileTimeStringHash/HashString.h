#pragma once

#include <cstdint>
#include <string>


namespace CompileTime
{
	//
	// using Fnv1a algorithm to hash strings
	//
	template<size_t I>
	struct Hash
	{
		template<size_t N>
		constexpr static uint32_t Generate(const char(&str)[N])
		{
			return static_cast<uint32_t>(static_cast<uint64_t>(Hash<I - 1u>::Generate(str) ^ static_cast<uint32_t>(str[I - 1])) * 16777619ull);
		}
	};

	template<>
	struct Hash<0u>
	{
		template<size_t N>
		constexpr static uint32_t Generate(const char(&str)[N])
		{
			return 2166136261u;
		}
	};


	template<size_t I, size_t N>
	constexpr static uint32_t GenerateHash(const char(&str)[N])
	{
		return static_cast<uint32_t>(static_cast<uint64_t>(Hash<I - 1u>::Generate(str) ^ static_cast<uint32_t>(str[I - 1])) * 16777619ull);
	}

}

namespace RunTime
{

	inline uint32_t GenerateHash(const char* _String, size_t _StringLength)
	{
		uint64_t _HashValue = 2166136261u;

		for (size_t i = 0; i < _StringLength; ++i)
		{
			_HashValue ^= static_cast<uint64_t>(_String[i]);
			_HashValue *= 16777619ull;
		}

		return static_cast<uint32_t>(_HashValue);
	}

	inline uint32_t GenerateHash(const char* _String)
	{
		return GenerateHash(_String, strlen(_String));
	}

}


template<typename T>
struct HashHelper{};

template<>
struct HashHelper<const char*>
{
	static uint32_t Generate(const char* _String)
	{
		return RunTime::GenerateHash(_String);
	}
};

template<size_t N>
struct HashHelper<char [N]>
{
	static constexpr uint32_t Generate(const char(&_String)[N])
	{
		return CompileTime::Hash<N - 1u>::Generate(_String);
	}
};


template<typename T>
uint32_t GenerateHash(const T& _String) 
{
	return HashHelper<T>::Generate(_String);
};


struct StringHash
{
	template<typename T>
	StringHash(const T& _String)
		: m_Hash(HashHelper<T>::Generate(_String))
	{}

	uint32_t GetHash()const
	{
		return m_Hash;
	}


private:
	const uint32_t m_Hash;
};




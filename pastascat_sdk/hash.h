#pragma once

namespace pc
{
	/// <summary>
	/// Type definition for strings hashed in FNV64
	/// </summary>
	typedef unsigned long long FNV64;

	namespace utils
	{

		/// <summary>
		/// Hashes a string with FNV
		/// </summary>
		/// <typeparam name="szType">Source string type</typeparam>
		/// <param name="str">String to hash</param>
		/// <returns>Hashed result of the string</returns>
		template <typename szType>
		constexpr FNV64 cx_fnv(const szType* str)
		{
			FNV64 result = 0xcbf29ce484222325;

			while (*str)
				result = (result * 0x00000100000001B3) ^ *str++;

			return result;
		}

		/// <summary>
		/// Partially hashes a string indicated by the len parameter with FNV
		/// </summary>
		/// <typeparam name="szType">Source string type</typeparam>
		/// <param name="str">String to hash</param>
		/// <param name="len">Length to hash</param>
		/// <returns>Hashed result of the string</returns>
		template <typename szType>
		constexpr FNV64 cx_fnv(const szType* str, unsigned int len)
		{
			FNV64 result = 0xcbf29ce484222325;

			while (*str && len-- > 0)
				result = (result * 0x00000100000001B3) ^ *str++;

			return result;
		}

	}
}
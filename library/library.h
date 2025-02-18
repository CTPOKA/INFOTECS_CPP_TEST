#pragma once
#include <string>

#ifdef _WIN32
#ifdef LIBRARY_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif
#else
#define LIBRARY_API
#endif

namespace lib
{
	extern "C"
	{
		/**
		 * @brief Сортирует цифры во входной строке по убыванию и заменяет четные цифры на заданную подстроку.
		 *
		 * @param str Входная строка, содержащая только цифры (будет изменена).
		 * @param replacement Строка, на которую заменяются четные цифры.
		 */
		LIBRARY_API void sortDigitsAndReplaceEven(std::string &str, std::string replacement = "KB");

		/**
		 * @brief Вычисляет сумму всех цифр во входной строке.
		 *
		 * @param str Входная строка (должна содержать только цифры).
		 * @return int Сумма всех цифр в строке.
		 */
		LIBRARY_API int calcSumOfDigits(const std::string &str);
		/**
		 * @brief Проверяет, что длина числа больше 2 и делится ли число на 32.
		 *
		 * @param num Число в формате строки.
		 * @return true Если длина числа больше 2 и число делится на 32.
		 * @return false В противном случае.
		 */
		LIBRARY_API bool isDivisibleBy32(const std::string &str);
	}
}

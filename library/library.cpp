#include <stdexcept>
#include <cctype>
#include "library.h"

void lib::sortDigitsAndReplaceEven(std::string& str, std::string replacement) {
	u_int16_t count[__SCHAR_MAX__ + 1] = { 0 };
	int evenCount = 0;

	for (char ch : str) {
		count[ch]++;
		if (std::isdigit(ch) && (ch - '0') % 2 == 0) {
			evenCount++;
		}
	}

	str.clear();
	str.reserve(str.size() + evenCount * (replacement.size() - 1));

	for (char ch = __SCHAR_MAX__; ch >= 0; --ch) {

		if (std::isdigit(ch) && (ch - '0') % 2 == 0) {
			for (int i = 0; i < count[ch]; ++i) {
				str.append(replacement);
			}
			continue;
		}

		for (int i = 0; i < count[ch]; ++i) {

			str.push_back(ch);
		}
	}
}

int lib::calcSumOfDigits(const std::string& str) {
	int sum = 0;
	for (char ch : str) {
		sum += isdigit(ch) ? (ch - '0') : 0;
	}
	return sum;
}

bool lib::isDivisibleBy32(const std::string& num) {
	if (num.size() < 2)
		return false;
	
	std::string lastDigits = num.size() <= 5 ? num : num.substr(num.size() - 5);

    int lastNumber = std::stoi(lastDigits);

	return lastNumber % 32 == 0;
}
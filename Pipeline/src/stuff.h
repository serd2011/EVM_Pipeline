#pragma once

#include <iostream>
#include <array>

namespace stuff {

	template<typename T>
	T getNumberFromCin(const std::string& str, T min = std::numeric_limits<T>().min(), T max = std::numeric_limits<T>().max()) {
		T num = 0;
		std::cout << str;
		std::cin >> num;
		while (num < min || num > max || std::cin.fail()) {
			std::cout << "Некорректный ввод!" << std::endl << str;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			std::cin >> num;
		};
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return num;
	}

	template<typename T>
	size_t selectOption(const std::string& title, const T* options, size_t count) {
		std::cout << title << std::endl;
		for (int i = 0; i < count; i++) {
			std::cout << i + 1 << ") " << options[i] << std::endl;
		}
		return getNumberFromCin<size_t>("Ваш выбор >>> ", 1, count) - 1;
	}

	template<typename T, size_t size>
	inline size_t selectOption(const std::string& title, const std::array<T, size>& options) {
		return selectOption<T>(title, options.data(), options.size());
	}

	template<typename T, size_t size>
	inline const T& getOption(const std::string& title, const std::array<T, size>& options) {
		return options[selectOption<T>(title, options.data(), options.size())];
	}
}
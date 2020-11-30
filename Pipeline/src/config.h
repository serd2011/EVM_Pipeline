#pragma once

#include <array>

namespace config {

	constexpr std::array<double, 3> OpRegChanceOptions{ 0.8, 0.7, 0.5 };
	constexpr std::array<unsigned int, 3> memoryAccessTimeOptions{ 2, 5, 10 };
	constexpr std::array<unsigned int, 3> firstCommandTimeOptions{ 1, 2, 3 };
	constexpr std::array<unsigned int, 3> secondCommandTimeOptions{ 4, 5, 16 };
	constexpr std::array<double, 3> firstCommandChanceOptions{ 0.9, 0.7, 0.5 };

	constexpr std::array<bool, 3> isCommandTermitating{ false,false,true };

	constexpr unsigned int thirdCommandTime = 1;

	constexpr unsigned int decodingTime = 1;
	constexpr unsigned int regAccessTime = 1;

	constexpr unsigned int maxNumOfConcurMemAccess = 2;
	constexpr unsigned int maxNumOfConcurRegAccess = 1;

	constexpr unsigned int commandTypesCount = 3;


	namespace output {
		constexpr char outputFileName[] = "output.txt";
		constexpr unsigned int cellWidth = 3;
		constexpr unsigned int tickCellWidth = 3;
		constexpr unsigned int reservedSpaceForHead = 200;
	}
}

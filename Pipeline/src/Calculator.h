#pragma once

#include <random>
#include <array>
#include <map>

#include "config.h"

enum class State {
	IF,
	FOP,
	SOP,
	EX,
	RES
};

struct CommandType {
	std::bernoulli_distribution firstOpRegGenerator;
	std::bernoulli_distribution secondOpRegGenerator;
	unsigned int executionTime;
	bool isTerminating;
};

struct Command {
	CommandType& type;
	bool isFirstOpReg;
	bool isSecondOpReg;
	State state;
	unsigned int tick;
	bool isRunning;
};

using CommandTypes = std::array<CommandType, config::commandTypesCount>;
using CommandChances = std::array<double, config::commandTypesCount>;
using StateStatus = std::map<State, bool>;

class Calculator {

public:
	Calculator() = delete;
	Calculator(const CommandTypes&, const CommandChances&, unsigned int memoryAccessTime);

	void calculate(unsigned int maxCommandCount);

private:
	CommandTypes commandTypes;
	CommandChances commandChances;
	unsigned int memoryAccessTime;

	Command getRandomCommand();

	std::mt19937 gen;
	std::discrete_distribution<int> commandsDistribution;
};

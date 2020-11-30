#pragma once

#include <fstream>
#include <list>
#include <map>

#include "Calculator.h"
#include "config.h"

enum class outputSatate {
	HALT,
	IF,
	FOP_REG,
	FOP_MEM,
	SOP_REG,
	SOP_MEM,
	EX,
	RES_REG,
	RES_MEM
};

struct OutputStateDescription {
	std::string name;
	std::string description;
};

using OutputStateDescriptions = std::map<outputSatate, OutputStateDescription>;

class OutputWriter {

public:
	OutputWriter() = delete;
	OutputWriter(unsigned int reservedLines);
	~OutputWriter();

	void writeInitInfo(const std::string& title, const OutputStateDescriptions&);
	void initWriteTickInfo(const std::list<Command>&, const unsigned int& ticks, const unsigned int& commandCount, const unsigned int& memoryAccessTime);
	void writeTickInfo();
	void writeResult(const std::string&);

private:
	const bool ifFileOpened;
	std::ofstream stream;
	OutputStateDescriptions stateDescriptions{};
	unsigned int linesWriten = 0;
	const std::list<Command>* commands = nullptr;
	const unsigned int* ticks = nullptr;
	const unsigned int* commandCount = nullptr;
	const unsigned int* memoryAccessTime = nullptr;
};

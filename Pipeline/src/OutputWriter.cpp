#include "OutputWriter.h"

#include <iomanip>
#include <iostream>

OutputWriter::OutputWriter(unsigned int reservedLines) :stream(config::output::outputFileName, std::ios::trunc), ifFileOpened(stream.is_open()) {
	if (!this->ifFileOpened) {
		std::cout << "Ошибка открытия файла!" << std::endl;
		return;
	}
	std::string blankLine(config::output::reservedSpaceForHead, ' ');
	for (unsigned int i = 0; i < reservedLines; i++) {
		this->stream << blankLine << '\n';
	}
}

OutputWriter::~OutputWriter() {
	this->stream.close();
}

void OutputWriter::writeInitInfo(const std::string& title, const OutputStateDescriptions& description) {
	if (!this->ifFileOpened) return;
	this->stateDescriptions = description;
	this->stream << '\n';
	this->stream << title << '\n';;
	for (auto& description : this->stateDescriptions) {
		this->stream << std::setw(config::output::cellWidth) << description.second.name << " - " << description.second.description << '\n';
	}
	this->stream << '\n';
}

void OutputWriter::initWriteTickInfo(const std::list<Command>& commands, const unsigned int& ticks, const unsigned int& commandCount, const unsigned int& memoryAccessTime) {
	if (!this->ifFileOpened) return;
	this->commands = &commands;
	this->ticks = &ticks;
	this->commandCount = &commandCount;
	this->memoryAccessTime = &memoryAccessTime;
}

void OutputWriter::writeTickInfo() {
	if (!this->ifFileOpened) return;
	if (this->commands->empty()) return;

	this->stream << std::setw(config::output::tickCellWidth) << *this->ticks << " | " << std::string((*(this->commandCount) - this->commands->size()) * (config::output::cellWidth + 1), ' ');
	for (auto it = this->commands->begin(); it != this->commands->end(); it++) {
		this->stream << " " << std::setw(config::output::cellWidth);
		if (!(it->isRunning)) {
			this->stream << this->stateDescriptions[outputSatate::HALT].name;
			continue;
		}
		switch (it->state) {

		case State::IF:
			this->stream << this->stateDescriptions[outputSatate::IF].name;
			break;

		case State::FOP:
			if (it->isFirstOpReg) {
				this->stream << this->stateDescriptions[outputSatate::FOP_REG].name;
			} else {
				if (it->tick == *(this->memoryAccessTime) + 1) {
					this->stream << this->stateDescriptions[outputSatate::FOP_REG].name;
				} else {
					this->stream << this->stateDescriptions[outputSatate::FOP_MEM].name;
				}
			}
			break;

		case State::SOP:
			if (it->isSecondOpReg) {
				this->stream << this->stateDescriptions[outputSatate::SOP_REG].name;
			} else {
				if (it->tick == *(this->memoryAccessTime) + 1) {
					this->stream << this->stateDescriptions[outputSatate::SOP_REG].name;
				} else {
					this->stream << this->stateDescriptions[outputSatate::SOP_MEM].name;
				}
			}
			break;

		case State::EX:
			this->stream << this->stateDescriptions[outputSatate::EX].name;
			break;

		case State::RES:
			if (it->isSecondOpReg) {
				this->stream << this->stateDescriptions[outputSatate::RES_REG].name;
			} else {
				if (it->tick == *(this->memoryAccessTime) + 1) {
					this->stream << this->stateDescriptions[outputSatate::RES_REG].name;
				} else {
					this->stream << this->stateDescriptions[outputSatate::RES_MEM].name;
				}
			}
			break;
		}
	}
	this->stream << '\n';
}

void OutputWriter::writeResult(const std::string& output) {
	std::cout << output << std::endl;
	if (!this->ifFileOpened) return;
	if (this->linesWriten++ == 0) this->stream.seekp(0);
	auto pos = this->stream.tellp();
	this->stream << output;
	this->stream.seekp(config::output::reservedSpaceForHead + pos, std::ios::beg);
	this->stream << '\n';
}

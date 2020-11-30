#include "Calculator.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <list>


#include "OutputWriter.h"

// Algorithm is written a way that assumes regAccessTime == 1
// To use different Time it need's to be rewriten
static_assert(config::regAccessTime == 1, "Reg access time is inacceptable");

static const OutputStateDescriptions description{
	{outputSatate::HALT,   {" _ ","Пропуск"}},
	{outputSatate::IF,     {" IF","Получение инструкции и декодирование"}},
	{outputSatate::FOP_REG,{"FOR","Обращение к регистру для первого операнда"}},
	{outputSatate::FOP_MEM,{"FOM","Обращение к памяти для первого операнда"}},
	{outputSatate::SOP_REG,{"SOR","Обращение к регистру для второго операнда"}},
	{outputSatate::SOP_MEM,{"SOM","Обращение к памяти для второго операнда"}},
	{outputSatate::EX,     {" EX","Выполнение"}},
	{outputSatate::RES_REG,{"WBR","Обращение к регистру для записи результата"}},
	{outputSatate::RES_MEM,{"WBM","Обращение к памяти для записи результата"}}
};


Calculator::Calculator(const CommandTypes& types, const CommandChances& chances, unsigned int time) : commandTypes(types),
commandChances(chances),
memoryAccessTime(time),
gen(std::random_device()()),
commandsDistribution(this->commandChances.begin(), this->commandChances.end()) {};

void Calculator::calculate(unsigned int maxCommandCount) {

	OutputWriter output(4);

	output.writeInitInfo("Описание обозначений", description);

	bool terminated = false;
	unsigned int commandCount = 0;
	unsigned int ticks = 0;
	unsigned int ticksWithoutPipeline = 0;

	std::list<Command> commands{};

	unsigned int memoryAccess = 0;
	unsigned int nextTickMemoryAccess = 0;
	unsigned int regAccess = 0;

	StateStatus isStateRunning{ {State::IF,false}, {State::FOP,false}, {State::SOP,false}, {State::EX,false}, {State::RES,false} };

	output.initWriteTickInfo(commands, ticks, commandCount, memoryAccessTime);

	commands.push_back(getRandomCommand());
	Command& command = commands.back();

	commandCount++;
	if (commandCount >= maxCommandCount) terminated = true;
	if (command.type.isTerminating) terminated = true;

	isStateRunning[State::IF] = true;
	command.isRunning = true;
	command.state = State::IF;
	command.tick = memoryAccessTime + 1;

	// Time without Pipeline
	ticksWithoutPipeline += memoryAccessTime + config::regAccessTime * 3 + command.type.executionTime;
	if (!command.isFirstOpReg) ticksWithoutPipeline += memoryAccessTime;
	if (!command.isSecondOpReg) ticksWithoutPipeline += memoryAccessTime * 2;

	// Calculate
	do {
		ticks++;

		memoryAccess = nextTickMemoryAccess;
		nextTickMemoryAccess = 0;
		regAccess = 0;

		// Clearing states
		for (auto it = commands.begin(); it != commands.end(); it++) {
			(it->tick)--;
			if (it->tick != 0) {
				if (it->tick > 1 && (it->state != State::EX && it->state != State::IF)) nextTickMemoryAccess++;
			}
			else {
				if (it->isRunning) {
					it->isRunning = false;
					isStateRunning[it->state] = false;
				}
			}
		}

		// Changing states
		std::list<Command>::iterator deleteIt = commands.end();
		for (auto it = commands.begin(); it != commands.end(); it++) {
			if (it->tick == 0) {
				it->tick = 1;
				switch (it->state) {
				case State::IF:
					if (isStateRunning[State::FOP]) break;
					if (regAccess == config::maxNumOfConcurRegAccess) break;
					if (!it->isFirstOpReg) {
						if (nextTickMemoryAccess == config::maxNumOfConcurMemAccess) break;
						it->tick = memoryAccessTime + 1;
						nextTickMemoryAccess++;
					}
					regAccess++;
					it->state = State::FOP;
					it->isRunning = true;
					isStateRunning[State::FOP] = true;
					break;
				case State::FOP:
					if (isStateRunning[State::SOP]) break;
					if (regAccess == config::maxNumOfConcurRegAccess) break;
					if (!it->isSecondOpReg) {
						if (nextTickMemoryAccess == config::maxNumOfConcurMemAccess) break;
						it->tick = memoryAccessTime + 1;
						nextTickMemoryAccess++;
					}
					regAccess++;
					it->state = State::SOP;
					it->isRunning = true;
					isStateRunning[State::SOP] = true;
					break;
				case State::SOP:
					if (isStateRunning[State::EX]) break;
					it->state = State::EX;
					it->isRunning = true;
					it->tick = it->type.executionTime;
					isStateRunning[State::EX] = true;
					break;
				case State::EX:
					if (isStateRunning[State::RES]) break;
					if (regAccess == config::maxNumOfConcurRegAccess) break;
					if (!it->isSecondOpReg) {
						if (nextTickMemoryAccess == config::maxNumOfConcurMemAccess) break;
						it->tick = memoryAccessTime + 1;
						nextTickMemoryAccess++;
					}
					regAccess++;
					it->state = State::RES;
					it->isRunning = true;
					isStateRunning[State::RES] = true;
					break;
				case State::RES:
					deleteIt = it;
					break;
				}
			}
		}
		if (deleteIt != commands.end()) {
			commands.erase(deleteIt);
		}


		// Adding new command
		if (!terminated && !isStateRunning[State::IF] && (commands.back().state == State::FOP)) {
			commands.push_back(getRandomCommand());
			Command& command = commands.back();

			commandCount++;
			if (commandCount >= maxCommandCount) terminated = true;
			if (command.type.isTerminating) terminated = true;

			isStateRunning[State::IF] = true;
			command.isRunning = true;
			command.state = State::IF;
			command.tick = memoryAccessTime;

			// Time without Pipeline
			ticksWithoutPipeline += memoryAccessTime + config::regAccessTime * 3 + command.type.executionTime;
			if (!command.isFirstOpReg) ticksWithoutPipeline += memoryAccessTime;
			if (!command.isSecondOpReg) ticksWithoutPipeline += memoryAccessTime * 2;
		}

		output.writeTickInfo();

	} while (!commands.empty());

	ticks--;

	unsigned int increase = (unsigned int)((1 - ticks / (double)ticksWithoutPipeline) * 100.0);
	double averegeTime = ticks / (double)commandCount;

	std::cout << "=========================" << std::endl;
	output.writeResult("Выполнено команд: " + std::to_string(commandCount));
	output.writeResult("Время выполнения без применения конвейера: " + std::to_string(ticksWithoutPipeline));
	output.writeResult("Время выполнения c применением конвейера: " + std::to_string(ticks) + " (на " + std::to_string(increase) + "% быстрее)");
	output.writeResult("Среднее время выполнения команды: " + std::to_string(averegeTime));

}

Command Calculator::getRandomCommand() {
	unsigned int type = this->commandsDistribution(this->gen);
	return Command{ commandTypes[type], commandTypes[type].firstOpRegGenerator(gen), commandTypes[type].secondOpRegGenerator(gen) };
}

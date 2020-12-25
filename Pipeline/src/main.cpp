#include <iostream>
#include <locale>

#include "config.h"
#include "stuff.h"
#include "Calculator.h"

CommandChances getCommandChances();
CommandTypes getCommandTypes();

int main() {
	std::locale::global(std::locale("ru_RU.UTF-8"));

	unsigned int maxCommandCount = stuff::getNumberFromCin<int>("Введите максимальное количество команд:", 1);
	unsigned int memoryAccessTime = stuff::getOption("Время доступа к памяти:", config::memoryAccessTimeOptions);

	CommandChances commandChances = getCommandChances();
	CommandTypes commandTypes = getCommandTypes();

	Calculator calc(commandTypes, commandChances, memoryAccessTime);
	calc.calculate(maxCommandCount);

	return 0;
}

CommandChances getCommandChances() {

	// To use different command count this function need's to be rewriten
	static_assert(config::commandTypesCount == 3, "Command types count doesn't match!");

	CommandChances commandChances{};

	commandChances[0] = stuff::getOption("Вероятность того, что команда является командой первого типа:", config::firstCommandChanceOptions);
	commandChances[1] = stuff::getNumberFromCin<double>("Введите вероятность того, что команда является командой второго типа:", 0);
	commandChances[2] = 1 - commandChances[0] - commandChances[1];
	if (commandChances[2] < 0) commandChances[2] = 0;

	return commandChances;
}

CommandTypes getCommandTypes() {

	// To use different command count this function need's to be rewriten
	static_assert(config::commandTypesCount == 3, "Command types count doesn't match!");

	CommandTypes commandTypes{};

	std::cout << std::endl << "===== Первая команда =====" << std::endl << std::endl;
	commandTypes[0].firstOpRegGenerator = std::bernoulli_distribution(stuff::getOption("Вероятность использования регистровой адресации первого операнда:", config::OpRegChanceOptions));
	commandTypes[0].secondOpRegGenerator = std::bernoulli_distribution(stuff::getOption("Вероятность использования регистровой адресации второго операнда:", config::OpRegChanceOptions));
	commandTypes[0].executionTime = stuff::getOption("Время вычисления результата:", config::firstCommandTimeOptions);
	commandTypes[0].isTerminating = config::isCommandTermitating[0];

	std::cout << std::endl << "===== Вторая команда =====" << std::endl << std::endl;
	commandTypes[1].firstOpRegGenerator = std::bernoulli_distribution(stuff::getOption("Вероятность использования регистровой адресации первого операнда:", config::OpRegChanceOptions));
	commandTypes[1].secondOpRegGenerator = std::bernoulli_distribution(stuff::getOption("Вероятность использования регистровой адресации второго операнда:", config::OpRegChanceOptions));
	commandTypes[1].executionTime = stuff::getOption("Время вычисления результата:", config::secondCommandTimeOptions);
	commandTypes[1].isTerminating = config::isCommandTermitating[1];

	std::cout << std::endl << "===== Третья команда =====" << std::endl << std::endl;
	commandTypes[2].firstOpRegGenerator = std::bernoulli_distribution(stuff::getOption("Вероятность использования регистровой адресации первого операнда:", config::OpRegChanceOptions));
	commandTypes[2].secondOpRegGenerator = std::bernoulli_distribution(stuff::getOption("Вероятность использования регистровой адресации второго операнда:", config::OpRegChanceOptions));
	commandTypes[2].executionTime = config::thirdCommandTime;
	commandTypes[2].isTerminating = config::isCommandTermitating[2];

	return commandTypes;
}

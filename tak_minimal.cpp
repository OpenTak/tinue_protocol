#include "tak_engine_minimal.h"

#include <iostream>

int main()
{
	TakEngineMinimal engine;
	while (true)
	{
		std::string input;
		std::getline(std::cin, input);
		engine.processCommand(input);
	}
}
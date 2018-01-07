#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <cstdint>
#include <sstream>
#include <unordered_map>

#include "multithreaded_logger.h"

class TakEngineMinimal
{
	std::atomic_bool m_needs_stop;
	enum class Debug : uint8_t
	{
		off = 0,
		on = 1
	};
	Logging::MultithreadedLogger m_logger;
	uint8_t m_init_given : 1;
	uint8_t m_position_given : 1;

	enum class CommandIn : uint8_t
	{
		tinue,
		debug,
		position,
		setoption,
		go,
		ping,
		stop,
		quit,
		isready,

		unknown
	};
	static std::unordered_map<std::string, CommandIn> commands;

	// This function is run in another thread, and should find the best move and return it.
	// The parameters are fully customisable. This example takes only a single parameter for demonstration purposes.
	// It is the most important function of the entire program, really.
	void findBestMove_worker(int movetime);

	// These functions are self descriptive. They can all be customised by an engine.
	// Example implementations are provided for all.
	void handleTinueInit(std::istringstream& args);
	void handleDebug(std::istringstream& args);
	void handlePosition(std::istringstream& args);
	void handleSetoption(std::istringstream& args);
	void handleGo(std::istringstream& args);
	void handlePing(std::istringstream& args);
	void handleStop(std::istringstream& args);
	void handleQuit(std::istringstream& args);
	void handleIsready(std::istringstream& args);
	void handleOther(const std::string& cmd, std::istringstream & args);
public:
	// Start the engine up.
	TakEngineMinimal();

	// This decodes the command and dispatches it to the correct handle* function.
	void processCommand(std::string command);
};
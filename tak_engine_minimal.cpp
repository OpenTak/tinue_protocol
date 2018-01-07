#include "tak_engine_minimal.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std::chrono_literals;

std::unordered_map<std::string, TakEngineMinimal::CommandIn> TakEngineMinimal::commands{
	std::make_pair("tinue",CommandIn::tinue),
	std::make_pair("debug",CommandIn::debug),
	std::make_pair("position",CommandIn::position),
	std::make_pair("setoption",CommandIn::setoption),
	std::make_pair("go",CommandIn::go),
	std::make_pair("ping",CommandIn::ping),
	std::make_pair("stop",CommandIn::stop),
	std::make_pair("quit",CommandIn::quit),
	std::make_pair("isready",CommandIn::isready)
};

TakEngineMinimal::TakEngineMinimal()
	: m_needs_stop{ false }
	, m_logger{ 100ms , std::cout }
	, m_init_given{ 0 }
	, m_position_given{ 0 }
{}

// This will find the command, and handle it.
void TakEngineMinimal::processCommand(std::string message)
{
	std::istringstream cmdStream{ std::move(message) };
	std::string commandStr;
	cmdStream >> commandStr;
	const auto commandIt = commands.find(commandStr);
	const CommandIn command = commandIt != end(commands) ? commandIt->second : CommandIn::unknown;
	if (!m_init_given && (command != CommandIn::tinue))
	{
		m_logger << "ERROR! Not initialised with \"tinue\" command!\n";
		return;
	}
	switch (command)
	{
	default:
		handleOther(commandStr, cmdStream);
		break;
	case CommandIn::tinue:
		handleTinueInit(cmdStream);
		break;
	case CommandIn::debug:
		handleDebug(cmdStream);
		break;
	case CommandIn::position:
		handlePosition(cmdStream);
		break;
	case CommandIn::setoption:
		handleSetoption(cmdStream);
		break;
	case CommandIn::go:
		handleGo(cmdStream);
		break;
	case CommandIn::ping:
		handlePing(cmdStream);
		break;
	case CommandIn::stop:
		handleStop(cmdStream);
		break;
	case CommandIn::quit:
		handleQuit(cmdStream);
		break;
	case CommandIn::isready:
		handleIsready(cmdStream);
		break;
	}
	m_logger << "ready\n";
}

// KEY FUNCTION
// Find the best move, and return it. Check for "m_needs_stop" periodically.
void TakEngineMinimal::findBestMove_worker(int movetime)
{
	const std::chrono::milliseconds time_budget{ movetime };
	using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
	const time_point start = std::chrono::high_resolution_clock::now();
	time_point last_output = start;
	while (!m_needs_stop)
	{

		const time_point present = std::chrono::high_resolution_clock::now();
		const auto elapsed = present - start;
		if ((present - last_output) > std::chrono::seconds(1))
		{
			std::ostringstream msg;
			m_logger << "info Been thinking for " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << "ms\n";
			last_output = std::chrono::high_resolution_clock::now();
		}
		if (elapsed > time_budget)
			break;
	}
	m_logger << "info Wait ... I don't know how to play tak.\n" << Logging::flush() <<  "move x\n";
}

void TakEngineMinimal::handleDebug(std::istringstream& args)
{
	// This turns the debug flag off and on.
	std::string arg;
	args >> arg;
	if (arg == "on")
	{
		m_logger.setLogLevel(Debug::on);
	}
	else if (arg == "off")
	{
		m_logger.setLogLevel(Debug::off);
	}
	else
	{
		m_logger << Logging::setlevel(Debug::on) << "info Unrecognised option for 'debug'.";
	}
}

void TakEngineMinimal::handleGo(std::istringstream& args)
{
	if (!m_position_given)
	{
		m_logger << "info No position given. Cannot go.\n";
		return;
	}
	std::string arg;
	int time_ms{ 10'000 };
	do
	{
		args >> arg;
		if (arg == "movetime")
		{
			args >> time_ms;
			break;
		}
	} while (args.good());
	m_logger << Logging::setlevel(Debug::on) << "Starting to think\n";
	std::thread movefinder{ &TakEngineMinimal::findBestMove_worker, this, time_ms };
	movefinder.detach();
}

void TakEngineMinimal::handleTinueInit(std::istringstream& args)
{
	m_logger << "id name Minimally Compliant Engine Example\n" << Logging::flush() << "id ver 0.1\n";
	m_init_given = true;
}

void TakEngineMinimal::handlePosition(std::istringstream& args)
{
	std::string arg;
	args >> arg;
	if (arg.empty())
	{
		m_logger << "info 'position' command must have a position.\n";
	}
	else
	{
		m_position_given = true;
		m_logger << Logging::setlevel(Debug::on) << "info Position set.\n";
	}
}

void TakEngineMinimal::handleSetoption(std::istringstream& args)
{
	m_logger << "info No options to set\n";
}

void TakEngineMinimal::handlePing(std::istringstream& args)
{
	m_logger << "pong\n";
}

void TakEngineMinimal::handleStop(std::istringstream& args)
{
	m_logger << Logging::setlevel(Debug::on) << "info Stopping think\n";
	m_needs_stop = true;
}

void TakEngineMinimal::handleQuit(std::istringstream& args)
{
	m_logger << Logging::setlevel(Debug::on) << "info Received quit command\n";
	std::exit(0);
}

void TakEngineMinimal::handleIsready(std::istringstream& args)
{
	m_logger << "ready\n";
}

void TakEngineMinimal::handleOther(const std::string& command, std::istringstream& args)
{
	if (command.empty())
	{
		return;
	}
	m_logger << Logging::setlevel(Debug::on) << "Unrecognised command '" << command << "'\n";
	std::string newCommand;
	std::getline(args, newCommand);
	processCommand(newCommand);
}
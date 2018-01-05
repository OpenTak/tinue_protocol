# Tak INterface for Universal Engines (TINUE)^1

Version 0.1

## General notes

This specification is independant of operating system.

All communication is done via standard input and output (stdin and stdout) commands.

An engine must always be able to process input from stdin. Even while thinking.

All commands to and from the engine end with a newline. (This may vary system to system.)

An unrecognised command, or an invalid command, should be ignored.

Engines are free to implement additional commands and options to those seen here. Where a convention is strongly established, it should be considered for formalisation into the TINUE standard.

## Notations

The engine should understand ["Tak Positional System" (TPS)](https://www.reddit.com/r/Tak/wiki/tak_positional_system) and ["Portable Tak Notation" (PTN)](https://www.reddit.com/r/Tak/wiki/tak_positional_system).
As an extention to PTN, x may be used to indicate a null move.

## Commands to the engine

### tinue:

this is always the first command given to the engine. It responds with an id command, then lists all its options, then finally "ready".

###d ebug [on|off]:

Set debug on or off. When debug is on the engine 

### position [size|tps] [ptn]:

Set up a new game, with a board of the given size, or set out according to a given TPS.

The PTN is optional, and indicates moved played from the given position.

Examples:

position 5  
position [TPS "x5/x5/x5/x5/x5 1 1"]  
position 3 a1 c2

### setoption [name] [value]:

sets engine options.

### go [params]:

Start thinking about a move. Should return "ready" as soon as it can, though. This is invalid if "position" has not been set. Params are:

All params are optional.

Standard params are:

- time1 [val]: Time left for player 1, in milliseconds
- time2 [val]: Time left for player 2, in milliseconds
- inc1 [val]: Time increment for player 1, in milliseconds.
- inc2 [val]: Time increment for player 2, in milliseconds.
- control [val]: Move that next time control is on.
- depth [val]: search val plies only.
- movetime [val]: search val milliseconds only.
- stones1 [val]: total stones player 1 has left.
- stones2 [val]: total stones player 2 has left.
- matchscore1 [val]: player 1's total score in the match
- matchscore2 [val]: player 2's total score in the match
- gamesleft [val]: number of games left in the match

These are not required, and the engine is free to ignore them. It is also free to implement additional commands.

### stop

Engine should stop thinking and send its best move.

### quit

The engine should stop what it's doing and close down.

## Commands from the engine

Engines are expected to use these commands to communicate. A UI designed to work with any TAK engine must understand these commands. Engines and UIs may implement additional commands and still be compliant.

### id [name|author|ver] [value]:

This is sent in response to the "tinue" command.

id name [engine_name] is the name of the engine. (Mandatory.)
id ver [version_number] is the version number. (Mandatory.)
id author [author_name] is the name of the engine's author. (Optional.)

### option [option_name] [option_type] [option_values]

- option_name is implementation defined, and used to identify the option.

- option_type is the type of the option, and can be "check", "range", "value", "combo", or "string".

A check is a true/false checkbox.

A range is a value within a set range. The option_value lists a "min" and a "max".

A value is an unbounded value.

A combo is like a combo box, and lists possible values.

A string is an arbitrary string.

For example:

option multithreaded check (can be "true" or "false")  
option max_depth range min 1 max 100  
option search_time value  
option style combo aggressive balanced passive defensive  
option string opponent_name  

###  info [string]:

this can be used to send arbitrary data. How this is handled is up to the UI. While thinking, it is recommended that an engine regularly send info commands.

###  move [ptn] [score]:

the best move according to the engine. This is in PTN format. Returning 'x' is permitted, but strongly discouraged.

"Score" is optional, but gives the engine's evaluation of the position after playing the noted move.

###  pong:

sent in responce to a "ping" command, as soon as possible. It indicates that the engine is alive.

###  ready:

indicates that the engine is ready to receive commands.

##  Footnote:

1. The chess version is called "UCI" for "Universal Chess Interface".
The tak equivilent would be "UTI", which is unfortanately also a rather unpleasant disease.
This is the reason for the somewhat odd naming.

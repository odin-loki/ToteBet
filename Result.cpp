#include "Result.h"

Result::Result(short _winner, short _second, short _third) 
{
	winner[0] = _winner;
	winner[1] = _second;
	winner[2] = _third;
}

short * Result::GetWinners() 
{
	return winner;
}

string Result::ToString() 
{
	char delimiter = ':';
	return "Result" + delimiter + to_string(winner[0]) + delimiter + to_string(winner[1]) + delimiter + to_string(winner[2]) + '\n';
}


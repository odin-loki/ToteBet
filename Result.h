#pragma once

#include <string>
using namespace std;

class Result
{
private:
	short winner[3];
public:
	Result(short _winner, short _second, short _third);
	short* GetWinners();
	string ToString();
};


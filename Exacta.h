#pragma once

#include <string>
using namespace std;

class Exacta
{
private:
	static constexpr char code = 'E'; //Product Code
	static constexpr double multiplier = 0.18; //Mulitplier to Calc Commission.
	double sum; //Sum of Total Bet Money.
	double commission; //Commision for TabCorp.

public:
	Exacta();
	Exacta(double _sum, double _commission); //Code and multiplier have defaults in implementation.
	char GetCode();
	void SetSum(double _sum);
	double GetSum();
	void CalulateCommission();
	string GetProductName();
};


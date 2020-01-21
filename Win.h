#pragma once

#include <string>
using namespace std;

class Win
{
private:
	static constexpr char code = 'W'; //Product Code
	static constexpr double multiplier = 0.15; //Mulitplier to Calc Commission.
	double sum; //Sum of Total Bet Money.
	double commission; //Commision for TabCorp.

public:
	Win();
	Win(double _sum, double _commission); //Code and multiplier have defaults in implementation.
	char GetCode();
	double GetSum();
	void SetSum(double _sum);
	void CalulateCommission();
	string GetProductName();
};

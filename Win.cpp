#include "Win.h"


Win::Win() {};

Win::Win(double _sum, double _commission)
{
	sum = _sum;
	commission = _commission;
}

double Win::GetSum()
{
	return sum;
}

void Win::SetSum(double _sum)
{
	sum = _sum;
}

void Win::CalulateCommission() 
{
	commission = static_cast<double>(sum * multiplier);
	sum -= commission;
}

char Win::GetCode() 
{
	return code;
}

string Win::GetProductName()
{
	return "Win";
}
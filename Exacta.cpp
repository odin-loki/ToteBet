#include "Exacta.h"

Exacta::Exacta() {};

Exacta::Exacta(double _sum, double _commission)
{
	sum = _sum;
	commission = _commission;
}

void Exacta::SetSum(double _sum)
{
	sum = _sum;
}

double Exacta::GetSum()
{
	return sum;
}

void Exacta::CalulateCommission() 
{
	commission = static_cast<double>(sum * multiplier);
	sum -= commission;
}

char Exacta::GetCode() 
{
	return code;
}

string Exacta::GetProductName()
{
	return "Exacta";
}
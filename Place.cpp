#include "Place.h"

Place::Place() {};

Place::Place(double _sum, double _commission)
{
	sum = _sum;
	commission = _commission;
}

void Place::SetSum(double _sum)
{
	sum = _sum;
}

void Place::CalulateCommission() 
{
	commission = static_cast<double>(sum * multiplier);
	sum -= commission;
}

char Place::GetCode() 
{
	return code;
}

double Place::GetSum()
{
	return sum;
}

string Place::GetProductName()
{
	return "Place";
}
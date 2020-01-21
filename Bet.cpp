#include "Bet.h"

Bet::Bet(char _product, vector<short> _selection, int _stake) 
{
	product = _product;
	selection = _selection;
	stake = _stake;
}

string Bet::ToString(char _product, vector<short> _selection, float _stake) 
{
	const char delimiter = ':';
	return "Bet" + delimiter + _product + delimiter + SelectionToString(_selection) + delimiter + to_string(_stake) + '\n';
}

string Bet::SelectionToString(vector<short> _selection) 
{
	string data;
	for (auto num : _selection)
	{
		data.append(to_string(num));
		data.append(", ");
	}
	data.erase(data.size() - 1);
	return data;
}

char Bet::GetProduct() 
{
	return product;
}

int Bet::GetStake() 
{
	return stake;
}

vector<short> Bet::GetSelection() 
{
	return selection;
}

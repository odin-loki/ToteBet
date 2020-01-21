#pragma once //Guard Pragama

#include <string>
#include <vector>

using namespace std;

class Bet
{
private:
	char product;
	vector<short> selection;
	int stake;

public:
	Bet(char _product, vector<short> _selection, int _stake);
	string ToString(char _product, vector<short> _selection, float _stake);
	string SelectionToString(vector<short> _selection);
	char GetProduct();
	int GetStake();
	vector<short> GetSelection();
};


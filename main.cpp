//Std Libs
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <exception>
#include <iomanip> 

//vcpkg source files
#include <gsl-lite.hpp> //Windows Guidelines support libriarys

//Header Files
#include "Bet.h"
#include "Result.h"
#include "Exacta.h"
#include "Place.h"
#include "Win.h"


//Use Emplace_back. Constructs objects at address instead of allocating and copying.

/*
	Had to Update knowledge. Was familiar with 1998 ISO C and C++. Coding practices seem to be better with a stronger
	emphasis on security and type safetey. Optimisations are automatically handled. Had to change from using:
	for (register int i = 0; i < (int)list.size(); i++) with  a vector[i] accessor to a Modern C++ one.
	Example: for (auto item : list) and for (auto item : *list) for vectors passed via pointer.
*/

/*
	Diary:
	Day 1: 6-8 Hours Dev.
	Day 2: 6-8 Hours Dev.
	Day 3: 6-8 Hours Refactor and Mess Around, Eg. Enforce Type Safety, Modern C++, Eliminate Warnings. Add Commenting. Began to Debug.
	Monday: Had an annoying day. 3 hours in Centrelink and 3 in Family and Community Services.
*/

/*
	Day 3: Would like to make Exacta, Price and Win have a Base Class with Virtual Methods and Overloads.
*/

/*
	Default Target is x64 architecture but no architecture specific code has been used.
	My favourite is the Intel Free Software Packages for Parralell Mathematics/Analytics and Nvidia CUDA.
	Compiled with most Security Options. Code Analysis and Warnings is set to All/All Native for debug, paralell compilation set.
	Compiled and Tested with Visual Studio 2019 and ISO C++ 2017. VS C++ 14.2.
	Packages are organised on windows using vcpkg package manager.
	Cmake is used for cross platform builds.
*/

using namespace std;
using namespace gsl;

vector<string> GetData(string filename)
{
	fstream file;
	string line;
	vector<string> lines;
	file.open(filename, fstream::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (!line.empty()) //Add string to list if not empty. Input Guard.
			{
				lines.emplace_back(line);
			}
			line = ""; //Set string to Null, for getline check.
		}
	}
	else
	{
		cout << "Error. File not found." << endl;
	}
	file.close();
	return lines;
}

/* Sorts Data into two Containers which are bet and result. Strings are sorted based
on presence of Bet or Result in string. Data is ignored if it is invalid. */
void SortData(const vector<string> list, not_null<vector<string>*> bets, not_null<vector<string>*> result)
{
	for (auto item : list)
	{
		string str = item;
		if (str.find("Bet") == 0)
		{
			str.erase(0, 4); //Needs to be 4
			bets->emplace_back(str);
		}
		else
		{
			if (str.find("Result") == 0)
			{
				str.erase(0, 7); //Needs to be 7
				result->emplace_back(str);
			}
		}
	}
}

//Gets Deletes Delimiter from first line from Input.txt file
inline void DeleteDelimiter(not_null<string*> str)
{
	const unsigned int pos = narrow_cast<unsigned int>(str->find_first_of(':'));
	str->erase(0, pos);
}

//Gets Product from Input.txt file
inline char GetProduct(not_null<string*> str)
{
	const unsigned int pos = narrow_cast<unsigned int>(str->find_first_of(':'));
	const char result = str->at(0);
	str->erase(0, pos);
	return result;
}

//Gets Stake from Input.txt file
inline string GetStake(not_null<string*> str)
{
	const unsigned int pos = narrow_cast<unsigned int>(str->find_first_of(':'));
	string result = str->substr(0, pos);
	str->erase(0, pos);
	return result;
}

//Gets Substrings from a deliminated string.
vector<short> GetWinners(string str, const char deliminiter)
{
	vector<short> result;
	stringstream ss(str);

	while (ss.good())
	{
		string temp;
		getline(ss, temp, deliminiter); //Not Working.
		result.emplace_back((short)stoi(temp));
	}
	return result;
}

//Makes an Int Value from a Char. Otherwise Converting from 1 to Short returns 50 which is the ASCII value.
short MakeIntValueFromASCIIValue(char cha)
{
	return narrow_cast<short>(cha - '0');
}

//Gets Substrings from a deliminated string. Consider Making Array. ??
vector<short> GetProductSelection(not_null<string*> str, const char productCode)
{
	vector<short> result;

	if (productCode == 'W' || productCode == 'P')
	{
		result.emplace_back(MakeIntValueFromASCIIValue(str->at(1)));
		str->erase(0, 3);
	}
	else if (productCode == 'E')
	{
		result.emplace_back(MakeIntValueFromASCIIValue(str->at(1)));
		result.emplace_back(MakeIntValueFromASCIIValue(str->at(3)));
		str->erase(0, 5);
	}

	return result;
}

//Helps to create a Bet list.
vector<Bet> AssigntoBet(const vector<string> list)
{
	vector<Bet> bets;
	for (auto item : list)
	{
		string str = item;
		const char product = GetProduct(&str);
		vector<short> selection = GetProductSelection(&str, product);
		const int stake = stoi(str);
		bets.emplace_back(Bet(product, selection, stake));
	}
	return bets;
}

//Helps create a Result object list.
vector<Result> AssigntoResult(const vector<string> list)
{
	vector<Result> results;
	for (auto item : list)
	{
		string str = item;
		vector<short> selection = GetWinners(str, ':');
		results.emplace_back(Result(selection[0], selection[1], selection[2]));
	}
	return results;
}

//Tests if Bet is an Win Winner.
inline bool IsWinWinner(not_null<Bet*> bet, not_null<Result*> result)
{
	if (bet->GetSelection().data()[0] == result->GetWinners()[0]) //bet returns 50 and result returns 2. Bet returns char representation of short. bad cast??
	{
		return true;
	}
	return false;
}

//Tests if Bet is an Place Winner. Returns true for any correct choice, as order does not count.
inline bool IsPlaceWinner(not_null<Bet*> bet, not_null<Result*> result)
{
	for (size_t i = 0; i < 3; i++)
	{
		if (bet->GetSelection()[0] == result->GetWinners()[i])
		{
			return true;
		}
	}
	return false;
}

//Tests if Bet is an Exacta Winner.
inline bool IsExactaWinner(not_null<Bet*> bet, not_null<Result*> result)
{
	if (bet->GetSelection().data()[0] == result->GetWinners()[0] && bet->GetSelection().data()[1] == result->GetWinners()[1])
	{
		return true;
	}
	return false;
}

//Finds Winners and Sorts them Into Three Winners.
void FindAndSortWinners(not_null<vector<Bet>*> bets, not_null<vector<Result>*> results, not_null<vector<Bet>*> winList, not_null<vector<Bet>*> placeList, not_null<vector<Bet>*> exactaList)
{
	for (auto betItem : *bets) //Get Value from Pointer and use Iterator.
	{
		for (auto resultItem : *results)
		{
			switch (betItem.GetProduct())
			{
			case 'W':
				if (IsWinWinner(&betItem, &resultItem))
				{
					winList->emplace_back(betItem);
				}
				break;
			case 'P':
				if (IsPlaceWinner(&betItem, &resultItem))
				{
					placeList->emplace_back(betItem);
				}
				break;
			case 'E':
				if (IsExactaWinner(&betItem, &resultItem))
				{
					exactaList->emplace_back(betItem);
				}
				break;
			default:
				continue;
			}
		}
	};
}

//Calculates Betting pools and assigns to objects. Error. C++ Will not accept const, static const or static constexpr as switch constants.
void CalculateWinnings(not_null<vector<Bet>*> bets, not_null<Win*> win, not_null<Place*> place, not_null<Exacta*> exacta)
{
	int w = 0, p = 0, e = 0;
	for (auto item : *bets) //Get Value from Pointer and use Iterator.
	{
		switch (item.GetProduct())
		{
		case 'W':
			w += item.GetStake();
			break;
		case 'P':
			p += item.GetStake();
			break;
		case 'E':
			e += item.GetStake();
			break;
		default:
			continue;
		}
	}

	win->SetSum(w);
	place->SetSum(p);
	exacta->SetSum(e);
}

//Calculates Commission and updates Betting pool.
void CalculateCommission(not_null<Win*> win, not_null<Place*> place, not_null<Exacta*> exacta)
{
	win->CalulateCommission();
	place->CalulateCommission();
	exacta->CalulateCommission();
}

//Prints Exacta dividends
inline void PrintExactaDividends(string product, vector<short> places, double dividend)
{
	cout << std::fixed;
	cout << std::setprecision(2);
	cout << product + ':' + to_string(places[0]) + ',' + to_string(places[1]) + ":$" << dividend << endl;
}

//Prints dividents for Place and Win.
inline void PrintDividends(string product, int place, double dividend)
{
	cout << std::fixed;
	cout << std::setprecision(2);
	cout << product + ':' + to_string(place) + ":$" << dividend << endl;
}

//Calculate Win Dividends and write to Console.
void CalculateWinDividends(not_null<vector<Bet>*> winList, not_null<Win*> win)
{
	for (auto betItem : *winList)
	{
		const double dividend = (betItem.GetStake() / win->GetSum()) * 100;
		PrintDividends(win->GetProductName(), betItem.GetSelection()[0], dividend);
	}
}

//Calculate Place Dividends and write to Console.
void CalculatePlaceDividends(not_null<vector<Result>*> results, not_null<vector<Bet>*> betList, not_null<Place*> place)
{
	for (auto resultItem : *results)
	{
		for (auto betItem : *betList)
		{
			const double subPool = place->GetSum() / 3;
			double dividend;
			for (size_t i = 0; i < 3; i++)
			{
				if (resultItem.GetWinners()[i] == betItem.GetSelection()[0])
				{
					dividend = (betItem.GetStake() / subPool) * 100;
					PrintDividends(place->GetProductName(), betItem.GetSelection()[0], dividend);
				}
			}
		}
	}
}

//Calculate Exacta Dividends and write to Console.
void CalculateExactDividends(not_null<vector<Bet>*> betList, not_null<Exacta*> exacta)
{
	for (auto betItem : *betList)
	{
		const double dividend = (betItem.GetStake() / exacta->GetSum()) * 100;
		PrintExactaDividends(exacta->GetProductName(), betItem.GetSelection(), dividend);
	}
}

//Release Resources for First Set of Data. Delete will not work as it is not Dynamically allocated. Or it is declared as a Local Stack variable. Strange becauase Vectors are Heap variables.
inline void FreeResources(not_null<vector<string>*> betList, not_null<vector<string>*> lines, not_null<vector<string>*> result)
{
	betList->clear();
	lines->clear();
	result->clear();

	betList->shrink_to_fit();
	lines->shrink_to_fit();
	result->shrink_to_fit();
}

//Main Method.
int main(int const argc, const char* const* argv)
{
	try //One Try and Catch as Code in Methods will throw to main.
	{
		if (argc != 2)
		{
			cout << "Program needs list of bets or too many inputs.";
			//exit(1); //Program Error.
		}

		vector<string> lines = GetData(argv[1]);
		vector<string> betlist, result;

		SortData(lines, &betlist, &result);
		vector<Bet> bets = AssigntoBet(betlist);
		vector<Result> results = AssigntoResult(result);

		FreeResources(&betlist, &lines, &result);

		Win win;
		Place place;
		Exacta exacta;
		vector<Bet> winList, placeList, exactaList;

		FindAndSortWinners(&bets, &results, &winList, &placeList, &exactaList);
		CalculateWinnings(&bets, &win, &place, &exacta);
		CalculateCommission(&win, &place, &exacta);

		CalculateWinDividends(&winList, &win);
		CalculatePlaceDividends(&results, &placeList, &place);
		CalculateExactDividends(&exactaList, &exacta);

		exit(0);
	}
	catch (const exception & ex)
	{
		cout << endl << "Exception: " << ex.what() << endl;
		exit(1); //Program Error.
	}
}
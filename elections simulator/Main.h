#pragma once
#include "SaveAndLoad.h"
#include "ElectionRound.h"
#include <iostream>

class Main
{
public:

	enum class Menu1
	{
		create = 1, load, exit
	};
	enum class Menu2
	{
		addDistrict = 1, addCitizen, addParty, addCandidate, printDistricts, printCitizens, printParties, vote, election, exit, save, load
	};


	static bool loadCase(election::ElectionRound*& r1);
	static void electionmain(election::ElectionRound* r1);
	static Menu1 getOptionMenu1();
	static Menu2 getOptionMenu2();
};



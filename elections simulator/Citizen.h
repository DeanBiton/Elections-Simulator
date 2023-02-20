#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <stdexcept>

namespace election
{
	class District;
	class SaveAndLoad;
	class Citizen
	{
	private:
		int id;
		std::string name;
		int birthYear;
		District* district;
		bool voted;
		bool candidate;

	public:
		//CONSTRUCTORS AND DESTRUCTOR
		Citizen();
		Citizen(int id,const std::string& name, int birthYear, District* district);
		~Citizen();
		Citizen(const Citizen& other);

		//GETS
		int getId() const; 
		const std::string& getName() const;
		int getBirthYear() const;
		District* getDistrict() const;
		int getDistrictNum() const;
		bool isVoted() const;
		bool isCandidate() const;

		//SETS
		bool setCandidate(); //changes candidate to 1 
		bool setVoted(); //changes voted to 1 

		//OPERATORS
		friend std::ostream& operator<<(std::ostream& os, const Citizen& ciz); //printing
		void operator=(const Citizen& other);

		//SAVE
		bool save(std::ostream& outfile, SaveAndLoad* sal); 
		bool load(std::istream& infile, SaveAndLoad* sal);
	};


}
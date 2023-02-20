#pragma once
#include <iostream>
#include "District.h"
namespace election
{
	template<class T>class Rep;
	template<class T>class PointerArray;

	class Party
	{
	private:
		static int serialNumGenerator;
		int partyNum;
		std::string name;
		Citizen* president;

		Rep<District>* candidatesByDistrict;

	public:
		//CONSTRUCTOR AND DESTRUCTOR
		Party();
		Party(const std::string& name, Citizen* president);
		Party(const std::string& name, Citizen* president, PointerArray<District>* districts);
		~Party();

		//OPERATORS
		void operator=(const Party& other);
		friend std::ostream& operator<<(std::ostream& os, const Party& party);

		//GETS
		std::list<Citizen*>* getCandidatesOfDistrict(const District* district);
		const std::string& getName();
		Citizen& getPresident() const;
		int getTNum() const;
		Rep<District>* getPartyRep();
		int getSerialNumGenerator() const;

		//SETS
		bool setSerialNumGenerator(int num);

		//OTHER
		void addDistrict(District* district);
		bool addCandidate(Citizen* ciz, District* district);

		//SAVE AND LOAD
		bool save(std::ostream& outfile, SaveAndLoad* sal);
		bool load(std::istream& infile, SaveAndLoad* sal);
	};
}
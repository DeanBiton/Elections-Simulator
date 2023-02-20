#pragma once
#include "Rep.h"

namespace election
{
	class ElectionRoundMemory
	{
	private:
		DynamicArray<Citizen*> citizens;
		DynamicArray<District*> districts;
		DynamicArray<Party*> parties;

	public:
		ElectionRoundMemory();
		~ElectionRoundMemory();

		Citizen* createNewCitizen(int id, const std::string& name, int birthYear, District* district);
		District* createNewDistrict(const std::string& districtName, int representNum, int serialType);
		Party* createNewParty(const std::string& name, Citizen* president, PointerArray<District>* districts);
		void addCitizen(Citizen* ciz);
		void addDistrict(District* district);
		void addToStart(District* district);
		void addParty(Party* party);
		
	};
}
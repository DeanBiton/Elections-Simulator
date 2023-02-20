#include "ElectionRoundMemory.h"

namespace election
{
	ElectionRoundMemory::ElectionRoundMemory() : citizens(), districts(), parties() {}

	ElectionRoundMemory::~ElectionRoundMemory() 
	{
		for (auto itr : citizens)
			delete itr;

		for (auto itr : districts)
			delete itr;

		for (auto itr : parties)
			delete itr;
	}

	Citizen* ElectionRoundMemory::createNewCitizen(int id, const std::string& name, int birthYear, District* district)
	{
		Citizen* ciz = new Citizen(id, name, birthYear, district);
		citizens.push_back(ciz);
		return ciz;
	}

	District* ElectionRoundMemory::createNewDistrict(const std::string& districtName, int representNum, int serialType)
	{
		if (serialType < 0 || serialType > 1)
			throw invalid_argument("serialType not possible");

		District* dis = nullptr;
		if (serialType == 0)
			dis = new District(districtName, representNum, parties.size());
		if (serialType == 1)//divided
			dis = new DividedDistrict(districtName, representNum, parties.size());
		districts.push_back(dis);
		return dis;
	}

	Party* ElectionRoundMemory::createNewParty(const std::string& name, Citizen* president, PointerArray<District>* districts)
	{
		Party* p = new Party(name, president, districts);
		parties.push_back(p);
		return p;
	}

	void ElectionRoundMemory::addCitizen(Citizen* ciz)
	{
		citizens.push_back(ciz);
	}
	void ElectionRoundMemory::addDistrict(District* district)
	{
		districts.push_back(district);
	}
	void ElectionRoundMemory::addToStart(District* district)
	{
		districts.insert(districts.begin(), district);
	}
	void ElectionRoundMemory::addParty(Party* party)
	{
		parties.push_back(party);
	}
}
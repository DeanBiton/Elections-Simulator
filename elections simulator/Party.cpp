#include "Party.h"
#include "SaveAndLoad.h"
#include "Rep.h"
#include "PointerArray.h"

namespace election
{
	int Party::serialNumGenerator = 0;

	//CONSTRUCTOR AND DESTRUCTOR

	Party::Party() : partyNum(-1), name(), president(nullptr), candidatesByDistrict(nullptr) {}

	Party::Party(const std::string& name, Citizen* president)
		: partyNum(serialNumGenerator), name(name), president(president), candidatesByDistrict(nullptr)
	{
		if (president != nullptr)
			president->setCandidate();
		serialNumGenerator++;
	}

	Party::Party(const std::string& name, Citizen* president, PointerArray<District>* districts)
		: partyNum(serialNumGenerator), name(name), president(president)
	{
		if (name.size() == 0)
			throw std::invalid_argument("empty name");
		if (president == nullptr)
			throw std::invalid_argument("president doesnt exist");

		if (districts == nullptr)
			throw std::invalid_argument("districts doesnt exist");

		candidatesByDistrict = new Rep<District>(*districts);

		president->setCandidate();
		serialNumGenerator++;
	}

	Party::~Party()
	{
		if (candidatesByDistrict != nullptr)
			delete candidatesByDistrict;
	}

	//OPERATORS
	void Party::operator=(const Party& other)
	{
		partyNum = other.partyNum;
		name = other.name;
		president = other.president;

		candidatesByDistrict = other.candidatesByDistrict;
	}

	std::ostream& operator<<(std::ostream& os, const Party& party)
	{
		os << "Number: " << party.partyNum << ", Name: " << party.name << ", Candidate for president: " << party.president->getName() << std::endl;
		party.candidatesByDistrict->print();
		return os;
	}

	std::list<Citizen*>* Party::getCandidatesOfDistrict(const District* district)
	{
		return candidatesByDistrict->getCitizenArray(district);
	}

	const std::string& Party::getName()
	{
		return this->name;
	}

	Citizen& Party::getPresident() const
	{
		return *president;
	}

	int Party::getTNum() const
	{
		return partyNum;
	}

	Rep<District>* Party::getPartyRep()
	{
		return candidatesByDistrict;
	}

	int Party::getSerialNumGenerator() const
	{
		return serialNumGenerator;
	}

	//SETS
	bool Party::setSerialNumGenerator(int num)
	{
		serialNumGenerator = num;
		return true;
	}

	//OTHER
	void Party::addDistrict(District* district)
	{
		candidatesByDistrict->addT(district);
	}

	bool Party::addCandidate(Citizen* ciz, District* district)//add candidate to currect district
	{
		if (ciz == nullptr)
			throw std::invalid_argument("citizen doesnt exist");
		if (district == nullptr)
			throw std::invalid_argument("district doesnt exist");

			candidatesByDistrict->addCitizen(district, ciz);
	
		return true;
	}
	
	//SAVE AND LOAD
	bool Party::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, partyNum))) // saving partyNum
			return false;
		if (!(SaveAndLoad::saveToBinaryFileSTL(outfile,name))) // saving name
			return false;

		int presidentIndex;
		presidentIndex = sal->getIndex(president);

		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, presidentIndex))) // saving presidentIndex
			return false;

		char ch = 'N';

		//saving candidatesByDistrict
		if (candidatesByDistrict == nullptr)
		{
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, ch)))
				return false;
		}
		else
		{
			ch = 'F';
			int PartyRepIndex = sal->getIndex(candidatesByDistrict);
			if (PartyRepIndex == -1) // index not found
				return false;
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, ch)))
				return false;
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, PartyRepIndex))) // saving PartyRepIndex
				return false;
		}
		return true;
	}
	bool Party::load(std::istream& infile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, partyNum))) // loading partyNum
			return false;
		if (!(SaveAndLoad::loadFromBinaryFileSTL(infile,name))) // loading name
			return false;

		int presidentIndex;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, presidentIndex))) // loading presidentIndex
			return false;

		president = sal->getCitizen(presidentIndex);

		//loading candidatesByDistrict
		char f;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, f))) // loading 'N' or 'F'
			return false;

		if (f != 'N')
		{
			int PartyRepIndex;
			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, PartyRepIndex))) // loading PartyRepIndex
				return false;

			candidatesByDistrict = sal->getPartyRep(PartyRepIndex);
		}

		return true;
	}
	
}

#pragma once
#include "Citizen.h"
#include "District.h"
#include "SaveAndLoad.h"

namespace election
{

	//CONSTRUCTORS AND DESTRUCTOR
	Citizen::Citizen() : id(0), name(""), birthYear(0), district(nullptr), voted(0), candidate(0) {}

	Citizen::Citizen(int id, const std::string& name, int birthYear, District* district)
		: id(id), name(name), birthYear(birthYear), district(district), voted(0), candidate(0)
	{
		if ((id <100000000)||(id >=1000000000))
			throw std::invalid_argument("id is not 9 figures");

		if (district == nullptr)
			throw std::invalid_argument("district doesnt exist");
		if (name.length() == 0)
			throw std::invalid_argument("empty name");
	}

	Citizen::~Citizen() {}

	Citizen::Citizen(const Citizen& other)
	{
		*this = other;
	}

	//GETS
	int Citizen::getId() const
	{
		return this->id;
	}
	const std::string& Citizen::getName() const
	{
		return this->name;
	}
	int Citizen::getBirthYear() const
	{
		return this->birthYear;
	}
	District* Citizen::getDistrict() const
	{
		return district;
	}
	int Citizen::getDistrictNum() const
	{
		return (*this).district->getTNum();
	}
	bool Citizen::isVoted() const
	{
		if (voted)
			return true;

		return false;

	}
	bool Citizen::isCandidate() const
	{
		return this->candidate;
	}

	//SET
	bool Citizen::setCandidate()
	{
		candidate = 1;
		return true;
	}
	bool Citizen::setVoted()
	{
		voted = 1;
		return true;
	}

	//OPERATORS
	std::ostream& operator<<(std::ostream& os, const Citizen& ciz)
	{
		os << "Name: " << ciz.name << ", ID: " << ciz.id << ", Date of birth: " << ciz.birthYear << ", District number: " << ciz.district->getTNum();
		return os;
	}

	void Citizen::operator=(const Citizen& other)
	{
		id = other.id;
		name = other.name;
		birthYear = other.birthYear;
		district = other.district;
		voted = other.voted;
	}
	
	bool Citizen::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		char ch;
		
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, id))) // saving id
			return false;

		if (!(SaveAndLoad::saveToBinaryFileSTL(outfile, name))) // saving name
			return false;

		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, birthYear))) // saving birthYear
			return false;

		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, voted))) // saving voted
			return false;

		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, candidate))) // saving candidate
			return false;

		if (district == nullptr)
		{
			ch = 'N';
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, ch))) // saving 'N'
				return false;
		}
		else
		{
			int districtIndex = sal->getIndex(district);
			if (districtIndex == -1) // index not found
				return false;
	
			ch = 'D';
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, ch))) // saving 'D'
				return false;

			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, districtIndex))) // saving district Index in sal
				return false;

			if (!SaveAndLoad::saveDistrictType(outfile, district)) // saving district type
				return false;
		}

		return true;
	}
	
	bool Citizen::load(std::istream& infile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, id))) // loading id
			return false;

		if (!(SaveAndLoad::loadFromBinaryFileSTL(infile, name))) // loading name
			return false;

		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, birthYear))) // loading birthYear
			return false;

		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, voted))) // loading voted
			return false;

		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, candidate))) // loading candidate
			return false;


		char d;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, d))) // loading 'N' or 'D'
			return false;

		if (d != 'N')
		{
			int districtIndex;
			DistrictType districtType;

			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, districtIndex))) // loading districtIndex
				return false;
			if (!(SaveAndLoad::loadDistrictType(infile, districtType))) // loading districtType
				return false;

			district = sal->getDistrict(districtIndex, districtType);
		}

		return true;
	}
	
}
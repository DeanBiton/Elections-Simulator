#pragma once
#include "SaveAndLoad.h"
#include "PointerArray.h"

namespace election
{
	template<>
	bool PointerArray<District>::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, arr.size()))) // saving districts LogSize
			return false;

		int districtIndex;

		for (auto itr : arr)
		{
			districtIndex = sal->getIndex(itr);
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, districtIndex))) // saving districtIndex
				return false;

			if (!(SaveAndLoad::saveDistrictType(outfile, itr)))
				return false;
		}
		return true;
	}

	template<>
	bool PointerArray<Party>::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, arr.size()))) // saving partiesLogSize
			return false;

		int partyIndex;
		for (auto itr : arr)
		{
			partyIndex = sal->getIndex(itr);
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, partyIndex))) // saving partyIndex
				return false;
		}
		return true;
	}

	template<>
	bool PointerArray<std::list<Citizen*>>::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, arr.size()))) // saving listsLogSize
			return false;

		int listIndex;
		for (auto itr : arr)
		{
			listIndex = sal->getIndex(itr);
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, listIndex))) // saving listIndex
				return false;
		}
		return true;
	}

	template<>
	bool  PointerArray<District>::load(std::istream& infile, SaveAndLoad* sal)
	{
		int districtsLogsize;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, districtsLogsize))) // loading districts Logsize
			return false;

		int districtIndex;
		DistrictType type;
		for (int i = 0; i < districtsLogsize; i++)
		{
			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, districtIndex))) // loading districtIndex
				return false;
			if (!(SaveAndLoad::loadDistrictType(infile, type))) // loading district type
				return false;

			arr.insert(arr.end(), sal->getDistrict(districtIndex, type));
		}

		return true;
	}

	template<>
	bool PointerArray<Party>::load(std::istream& infile, SaveAndLoad* sal)
	{
		int partiesLogSize;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, partiesLogSize))) // loading partiesLogSize
			return false;

		int partyIndex;
		for (int i = 0; i < partiesLogSize; i++)
		{
			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, partyIndex))) // loading partyIndex
				return false;

			arr.insert(arr.end(), sal->getParty(partyIndex));
		}

		return true;
	}

	template<>
	bool PointerArray<std::list<Citizen*>>::load(std::istream& infile, SaveAndLoad* sal)
	{
		int listsLogSize;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, listsLogSize))) // loading listsLogSize
			return false;

		int listIndex;
		for (int i = 0; i < listsLogSize; i++)
		{
			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, listIndex))) // loading listIndex
				return false;

			arr.insert(arr.end(), sal->getCitizensList(listIndex));
		}

		return true;
	}
}
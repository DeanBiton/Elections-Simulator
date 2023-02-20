#pragma once
#include "ElectionRound.h"
#include "exception.h"
namespace election
{
	enum class electionRoundType { ElectionRound = 1, SimpleElectionRound = 2 };

	enum class DistrictType { District , DividedDistrict };

	class SaveAndLoad
	{
	private:
		PointerArray<Citizen> A;
		PointerArray<std::list<Citizen*>> B;
		PointerArray<Party> C;
		PointerArray<District> D;
		PointerArray<Rep<Party>> E; // DistrictRep
		PointerArray<Rep<District>> F;// PartyRep

		//function for createSAL
		template<class T>
		void getCitizensListsPointers(Rep<T>* t)
		{
			if (t != nullptr)
			{
				//getting CitizensLists pointers inside Rep to B
				for (int i = 0; i < t->getLogSize(); i++)
					if(t->getCitizenArray(i)!= nullptr)
						B.add(t->getCitizenArray(i));
			}
		}

	public:
		//CONSTRUCTOR AND DESTRUCTOR
		SaveAndLoad();
		~SaveAndLoad();

		//GETS
		int getLogSizeA() const;
		int getLogSizeB() const;
		int getLogSizeC() const;
		int getLogSizeD() const;
		int getLogSizeE() const;
		int getLogSizeF() const;

		int getIndex(Citizen* other) const;
		int getIndex(std::list<Citizen*>* other) const;
		int getIndex(Party* other) const;
		int getIndex(District* other) const;
		int getIndex(Rep<Party>* other) const;
		int getIndex(Rep<District>* other) const;

		Citizen* getCitizen(int index);
		std::list<Citizen*>* getCitizensList(int index);
		Party* getParty(int index);
		District* getDistrict(int index, DistrictType type = DistrictType::District);
		Rep<Party>* getDistrictRep(int index);
		Rep<District>* getPartyRep(int index);

		////SAVE TO BINARY FILE
		//Primitive Data Types
		template<typename T>
		static bool saveToBinaryFilePrimitive(std::ostream& outfile, const T& other)
		{
			outfile.write(reinterpret_cast<const char*>(&other), sizeof(other));
			if (outfile.fail())
				throw std::bad_alloc();

			return true;
		}

		//STL 
		static bool saveToBinaryFileSTL(std::ostream& outfile, const std::string& other);
		bool saveToBinaryFileSTL(std::ostream& outfile, const std::map<int, Citizen*>& other);
		bool saveToBinaryFileSTL(std::ostream& outfile, const std::list<Citizen*>& other);


		////LOAD FROM BINARY FILE
		//Primitive Data Types
		template<typename T>
		static bool loadFromBinaryFilePrimitive(std::istream& infile, T& other)
		{
			infile.read(reinterpret_cast<char*>(&other), sizeof(other));
			if (infile.fail())
				throw std::bad_alloc();

			return true;
		}

		//STL
		static bool loadFromBinaryFileSTL(std::istream& infile, std::string& other);
		bool loadFromBinaryFileSTL(std::istream& infile, std::map<int, Citizen*>& other);
		bool loadFromBinaryFileSTL(std::istream& infile, std::list<Citizen*>& other);

		//CreateSal
		void createSAL(ElectionRound& e1);

		//SAVE SAL
		bool saveSAL(std::ostream& outfile);

		//LOAD SAL
		bool loadSAL(std::istream& infile);

		//ElectionRound
		static ElectionRound* createElectionRound(electionRoundType type, int representNum = 0);
		static ElectionRound* createElectionRound(std::istream& infile);
		static bool saveElectionRoundType(ElectionRound* r, std::ofstream& outfile);

		//District
		static District* createDistrict(DistrictType type);
		static bool saveDistrictType(std::ostream& outfile, District* district);
		static bool loadDistrictType(std::istream& infile, DistrictType& type);
		static bool checkElectionType(int num);

	}; 
}
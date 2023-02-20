#include "SaveAndLoad.h"

using namespace std;
namespace election

{
	//CONSTRUCTOR AND DESTRUCTOR
	SaveAndLoad::SaveAndLoad() : A(), B(), C(), D(), E(), F() {}

	SaveAndLoad::~SaveAndLoad() {}

	//GETS
	int SaveAndLoad::getLogSizeA() const
	{
		return A.getLength();
	}
	int SaveAndLoad::getLogSizeB() const
	{
		return B.getLength();
	}
	int SaveAndLoad::getLogSizeC() const
	{
		return C.getLength();
	}
	int SaveAndLoad::getLogSizeD() const
	{
		return D.getLength();
	}
	int SaveAndLoad::getLogSizeE() const
	{
		return E.getLength();
	}
	int SaveAndLoad::getLogSizeF() const
	{
		return F.getLength();
	}

	int SaveAndLoad::getIndex(Citizen* other) const
	{
		for (int i = 0; i < A.getLength(); i++)
		{
			if (other == A.getTByIndex(i))
				return i;
		}
		return -1;
	}
	int SaveAndLoad::getIndex(std::list<Citizen*>* other) const
	{
		for (int i = 0; i < B.getLength(); i++)
		{
			if (other == B.getTByIndex(i))
				return i;
		}
		return -1;
	}
	int SaveAndLoad::getIndex(Party* other) const
	{
		for (int i = 0; i < C.getLength(); i++)
		{
			if (other == C.getTByIndex(i))
				return i;
		}
		return -1;
	}
	int SaveAndLoad::getIndex(District* other) const
	{
		for (int i = 0; i < D.getLength(); i++)
		{
			if (other == D.getTByIndex(i))
				return i;
		}
		return -1;
	}
	int SaveAndLoad::getIndex(Rep<Party>* other) const
	{
		for (int i = 0; i < E.getLength(); i++)
		{
			if (other == E.getTByIndex(i))
				return i;
		}
		return -1;
	}
	int SaveAndLoad::getIndex(Rep<District>* other) const
	{
		for (int i = 0; i < F.getLength(); i++)
		{
			if (other == F.getTByIndex(i))
				return i;
		}
		return -1;
	}

	Citizen* SaveAndLoad::getCitizen(int index)  
	{
		Citizen* a = nullptr;

		if (index < 0 || index >= A.getLength())//checks if A[index] exist
			return a;

		Citizen* temp = A.getTByIndex(index);
		if (temp != nullptr)
			a = temp;
		else
		{
			a = new Citizen;
			A.setPointer(index, a);
		}

		return a;
	}
	std::list<Citizen*>* SaveAndLoad::getCitizensList(int index) 
	{
		list<Citizen*>* b = nullptr;

		if (index < 0 || index >= B.getLength())//checks if B[index] exist
			return b;

		list<Citizen*>* temp = B.getTByIndex(index);
		if (temp != nullptr)
			b = temp;
		else
		{
			b = new list<Citizen*>;
			B.setPointer(index, b);
		}

		return b;
	}
	Party* SaveAndLoad::getParty(int index)
	{
		Party* c = nullptr;

		if (index < 0 || index >= C.getLength())//checks if C[index] exist
			return c;

		Party* temp = C.getTByIndex(index);
		if (temp != nullptr)
			c = temp;
		else
		{
			c = new Party;
			C.setPointer(index, c);
		}

		return c;
	}
	District* SaveAndLoad::getDistrict(int index, DistrictType type)
	{
		District* d = nullptr;

		if (index < 0 || index >= D.getLength())//checks if D[index] exist
			return d;

		District* temp = D.getTByIndex(index);
		if (temp != nullptr)
			d = temp;
		else
		{
			d = createDistrict(type);
			D.setPointer(index, d);
		}

		return d;
	}
	Rep<Party>* SaveAndLoad::getDistrictRep(int index)
	{
		Rep<Party>* e = nullptr;

		if (index < 0 || index >= E.getLength())//checks if E[index] exist
			return e;

		Rep<Party>* temp = E.getTByIndex(index);
		if (temp != nullptr)
			e = temp;
		else
		{
			e = new Rep<Party>;
			E.setPointer(index, e);
		}

		return e;
	}
	Rep<District>* SaveAndLoad::getPartyRep(int index)
	{
		Rep<District>* f = nullptr;

		if (index < 0 || index >= F.getLength())//checks if F[index] exist
			return f;

		Rep<District>* temp = F.getTByIndex(index);
		if (temp != nullptr)
			f = temp;
		else
		{
			f = new Rep<District>;
			F.setPointer(index, f);
		}

		return f;
	}

	////SAVE TO BINARY FILE
	//STL
	bool SaveAndLoad::saveToBinaryFileSTL(std::ostream& outfile, const std::string& other)
	{
		if (!saveToBinaryFilePrimitive(outfile, other.size()))
			throw exception();

		for (auto itr : other)
			if (!saveToBinaryFilePrimitive(outfile, itr))
				throw exception();
			
		return true;
	}

	bool SaveAndLoad::saveToBinaryFileSTL(std::ostream& outfile, const std::map<int, Citizen*>& other)
	{
		if (!saveToBinaryFilePrimitive(outfile, other.size()))
				throw exception();

		int index;
		Citizen* ciz;
		for (auto itr : other)
		{
			ciz = itr.second;
			index = getIndex(ciz);
			if (!saveToBinaryFilePrimitive(outfile, index))
				throw exception();
		}

		return true;
	}

	bool SaveAndLoad::saveToBinaryFileSTL(std::ostream& outfile, const std::list<Citizen*>& other)
	{
		if (!saveToBinaryFilePrimitive(outfile, other.size()))
				throw exception();

		int index;
		Citizen* ciz;
		for (auto itr : other)
		{
			ciz = itr;
			index = getIndex(ciz);
			if (!saveToBinaryFilePrimitive(outfile, index))
				throw exception();
		}

		return true;
	}
	
	////LOAD FROM BINARY FILE
	//STL
	bool SaveAndLoad::loadFromBinaryFileSTL(std::istream& infile, std::string& other)
	{
		int logSize;
		if (!loadFromBinaryFilePrimitive(infile, logSize))
				throw exception();

		char ch;
		for (int i = 0; i < logSize; i++)
		{
			if (!loadFromBinaryFilePrimitive(infile, ch))
				throw exception();

			other.push_back(ch);
		}
		return true;
	}

	bool SaveAndLoad::loadFromBinaryFileSTL(std::istream& infile, std::map<int, Citizen*>& other)
	{
		int logSize;
		if (!loadFromBinaryFilePrimitive(infile, logSize))
				throw exception();
		
		int index;
		Citizen* ciz;
		for (int i = 0; i < logSize; i++)
		{
			if (!loadFromBinaryFilePrimitive(infile, index))
				throw exception();

			ciz = getCitizen(index);
			other.insert(std::pair<int, Citizen*>(ciz->getId(), ciz));
		}

		return true;
	}

	bool SaveAndLoad::loadFromBinaryFileSTL(std::istream& infile, std::list<Citizen*>& other)
	{
		int logSize;
		if (!loadFromBinaryFilePrimitive(infile, logSize))
				throw exception();

		int index;
		Citizen* ciz;
		for (int i = 0; i < logSize; i++)
		{
			if (!loadFromBinaryFilePrimitive(infile, index))
				throw exception();

			ciz = getCitizen(index);
			other.push_back(ciz);
		}

		return true;
	}

	//CreateSal
	void SaveAndLoad::createSAL(ElectionRound& e1)
	{
		std::map<int, Citizen*>& citizens = e1.getCitizens();

		//getting all Citizens pointers to A
		for (auto itr : citizens)
		{
			A.add(itr.second);
		}

		PointerArray<District>& districts = e1.getDistricts();
		for (int i = 0; i < districts.getLength(); i++)
		{
			D.add(districts.getTByIndex(i)); //gets District pointer to D
			Rep<Party>* DistrictRep = D.getTByIndex(i)->getDistricRep();
			if (DistrictRep != nullptr)
			{
				E.add(DistrictRep);//gets DistricRep pointer to E
				getCitizensListsPointers(DistrictRep);//getting CitizensLists pointers inside DistrictRep to B
			}
		}

		PointerArray<Party>& parties = e1.getParties();
		for (int i = 0; i < parties.getLength(); i++)
		{
			C.add(parties.getTByIndex(i)); //gets Party pointer to C
			Rep<District>* PartyRep = C.getTByIndex(i)->getPartyRep();
			if (PartyRep != nullptr)
			{
				F.add(PartyRep);//gets DistricRep pointer to F
				getCitizensListsPointers(PartyRep);//getting CitizensLists pointers inside PartyRep to B
			}
		}
	}

	//SAVE SAL
	bool SaveAndLoad::saveSAL(std::ostream& outfile)
	{
		//Saving arrays logSize 
		if (!(saveToBinaryFilePrimitive(outfile, A.getLength())))
				throw exception();
		if (!(saveToBinaryFilePrimitive(outfile, B.getLength())))
				throw exception();
		if (!(saveToBinaryFilePrimitive(outfile, C.getLength())))
				throw exception();
		if (!(saveToBinaryFilePrimitive(outfile, D.getLength())))
				throw exception();
		if (!(saveToBinaryFilePrimitive(outfile, E.getLength())))
				throw exception();
		if (!(saveToBinaryFilePrimitive(outfile, F.getLength())))
				throw exception();

		//Saving arrays
		for (int i = 0; i < A.getLength(); i++)//A
		{
			if (!(A.getTByIndex(i)->save(outfile, this)))
				throw exception();
		}

		for (int i = 0; i < B.getLength(); i++)//B
		{
			if (!(saveToBinaryFileSTL(outfile,*(B.getTByIndex(i)))))
				throw exception();
		}

		for (int i = 0; i < C.getLength(); i++)//C
		{
			if (!(C.getTByIndex(i)->save(outfile, this)))
				throw exception();
		}
		if (C.getLength() > 0)// saving serialNumGenerator
		{
			int serialNumGenerator = C.getTByIndex(0)->getSerialNumGenerator();
			if (!(saveToBinaryFilePrimitive(outfile, serialNumGenerator)))
				throw exception();
		}

		for (int i = 0; i < D.getLength(); i++)//D
		{
			if (!SaveAndLoad::saveDistrictType(outfile, D.getTByIndex(i)))
				throw exception();

			if (!(D.getTByIndex(i)->save(outfile, this)))
				throw exception();
		}
		if (D.getLength() > 0)// saving districtNumgenerator
		{
			int districtNumgenerator = D.getTByIndex(0)->getDistrictNumgenerator();
			if (!(saveToBinaryFilePrimitive(outfile, districtNumgenerator)))
				throw exception();
		}

		for (int i = 0; i < E.getLength(); i++)//E
		{
			if (!(E.getTByIndex(i)->save(outfile, this)))
				throw exception();
		}

		for (int i = 0; i < F.getLength(); i++)//F
		{
			if (!(F.getTByIndex(i)->save(outfile, this)))
				throw exception();
		}

		return true;
	}

	//LOAD SAL
	bool SaveAndLoad::loadSAL(std::istream& infile)
	{
		int logSizeA, logSizeB, logSizeC, logSizeD, logSizeE, logSizeF;

		//Loading arrays logSize
		if (!(loadFromBinaryFilePrimitive(infile, logSizeA)))
				throw exception();
		if (!(loadFromBinaryFilePrimitive(infile, logSizeB)))
				throw exception();
		if (!(loadFromBinaryFilePrimitive(infile, logSizeC)))
				throw exception();
		if (!(loadFromBinaryFilePrimitive(infile, logSizeD)))
				throw exception();
		if (!(loadFromBinaryFilePrimitive(infile, logSizeE)))
				throw exception();
		if (!(loadFromBinaryFilePrimitive(infile, logSizeF)))
				throw exception();

		//Reset arrays
		for (int i = 0; i < logSizeA; i++)
			A.add(nullptr);
		for (int i = 0; i < logSizeB; i++)
			B.add(nullptr);
		for (int i = 0; i < logSizeC; i++)
			C.add(nullptr);
		for (int i = 0; i < logSizeD; i++)
			D.add(nullptr);
		for (int i = 0; i < logSizeE; i++)
			E.add(nullptr);
		for (int i = 0; i < logSizeF; i++)
			F.add(nullptr);

		//loading SAL arrays
		
		for (int i = 0; i < logSizeA; i++)//A
		{
			if (A.getTByIndex(i) == nullptr)
			{
				Citizen* ciz = new Citizen;
				A.setPointer(i, ciz);
			}
			if (!(A.getTByIndex(i)->load(infile, this)))
				throw exception();
		}

		for (int i = 0; i < logSizeB; i++)//B
		{
			if (B.getTByIndex(i) == nullptr)
			{
				std::list<Citizen*>* list = new std::list<Citizen*>;
				B.setPointer(i, list);
			}
			if (!(loadFromBinaryFileSTL(infile, *(B.getTByIndex(i)))))
				throw exception();
		}

		
		for (int i = 0; i < logSizeC; i++)//C
		{
			if (C.getTByIndex(i) == nullptr)
			{
				Party* party = new Party;
				C.setPointer(i, party);
			}
			if (!(C.getTByIndex(i)->load(infile, this)))
				throw exception();
		}
		if (logSizeC > 0)// loading serialNumGenerator
		{
			int serialNumGenerator;
			if (!(loadFromBinaryFilePrimitive(infile, serialNumGenerator)))
				throw exception();
			C.getTByIndex(0)->setSerialNumGenerator(serialNumGenerator);
		}

		for (int i = 0; i < logSizeD; i++)//D
		{
			DistrictType type;
			if (!(loadDistrictType(infile, type)))
				throw exception();

			if (D.getTByIndex(i) == nullptr)
			{
				District* district = createDistrict(type);
				D.setPointer(i, district);
			}

			if (!(D.getTByIndex(i)->load(infile, this)))
				throw exception();
		}
		if (logSizeD > 0)// loading districtNumgenerator
		{
			int districtNumgenerator;
			if (!(loadFromBinaryFilePrimitive(infile, districtNumgenerator)))
				throw exception();
			D.getTByIndex(0)->setDistrictNumgenerator(districtNumgenerator);
		}

		for (int i = 0; i < logSizeE; i++)//E
		{
			if (E.getTByIndex(i) == nullptr)
			{
				Rep<Party>* DistrictRep = new Rep<Party>;
				E.setPointer(i, DistrictRep);
			}
			if (!(E.getTByIndex(i)->load(infile, this)))
				throw exception();
		}

		for (int i = 0; i < logSizeF; i++)//F
		{
			if (F.getTByIndex(i) == nullptr)
			{
				Rep<District>* PartyRep = new Rep<District>;
				F.setPointer(i, PartyRep);
			}
			if (!(F.getTByIndex(i)->load(infile, this)))
				throw exception();
		}

		return true;
	}

	//ElectionRound
	ElectionRound* SaveAndLoad::createElectionRound(electionRoundType type, int representNum)
	{
		switch (type)
		{
		case electionRoundType::ElectionRound:
			return new ElectionRound;
			break;

		case electionRoundType::SimpleElectionRound:
			return new SimpleElectionRound(representNum);
			break;

		default:
			throw exception();
			break;
		}

		return nullptr;
	}

	ElectionRound* SaveAndLoad::createElectionRound(std::istream& infile)
	{
		electionRoundType type;
		int representNum = 0;

		infile.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (infile.fail())
				throw exception();

		if (type == electionRoundType::SimpleElectionRound)
		{
			if (!loadFromBinaryFilePrimitive(infile, representNum))
				throw exception();
		}

		return SaveAndLoad::createElectionRound(type, representNum);
	}

	bool SaveAndLoad::saveElectionRoundType(ElectionRound* r, std::ofstream& outfile)
	{
		electionRoundType type;

		if (typeid(*r) == typeid(ElectionRound))
			type = electionRoundType::ElectionRound;
		else if (typeid(*r) == typeid(SimpleElectionRound))
			type = electionRoundType::SimpleElectionRound;
		else
			return false;

		outfile.write(reinterpret_cast<const char*>(&type), sizeof(type));
		if (outfile.fail())
			return false;

		if (typeid(*r) == typeid(SimpleElectionRound))
		{
			int representNum = r->getDistricts().getTByIndex(0)->getRepresentNum();
			if (!saveToBinaryFilePrimitive(outfile, representNum))
				throw exception();
		}

		return true;
	}

	//District
	District* SaveAndLoad::createDistrict(DistrictType type)
	{
		switch (type)
		{
		case DistrictType::District:
			return new District;
			break;

		case DistrictType::DividedDistrict:
			return new DividedDistrict;
			break;

		default:
			break;
		}

		return nullptr;
	}

	bool SaveAndLoad::saveDistrictType(std::ostream& outfile, District* district)
	{
		DistrictType type;

		if (typeid(*district) == typeid(District))
			type = DistrictType::District;
		else if (typeid(*district) == typeid(DividedDistrict))
			type = DistrictType::DividedDistrict;
		else
			throw exception();

		outfile.write(reinterpret_cast<const char*>(&type), sizeof(type));
		if (outfile.fail())
			throw exception();

		return true;
	}

	bool SaveAndLoad::loadDistrictType(std::istream& infile, DistrictType& type)
	{
		infile.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (infile.fail())
			throw exception();

		return true;
	}

	bool SaveAndLoad::checkElectionType(int num)
	{
		if (num <= 2 && num >= 1)
			return true;

		return false;
	}

}


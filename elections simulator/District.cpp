#include"District.h"
#include "Rep.h"
#include "Party.h"
#include "SaveAndLoad.h"

namespace election
{
	int  District::districtNumgenerator = 0;

	//CONSTRUCTORS
	District::District() :districtNum(-1), name(), voters(), representNum(0), sumVotes(0), votePercentage(0),
		votesByParty(nullptr), logSize(0), phySize(0), candidatesByParty(nullptr) {}

	District::District(const std::string& name, int representNum, int partiesSize) : districtNum(districtNumgenerator), name(name), voters(),
		representNum(representNum), sumVotes(0), votePercentage(0), candidatesByParty(nullptr)
	{

		if (representNum <= 0)
			throw std::invalid_argument("representNum not possible");
		if (name.length() == 0)
			throw std::invalid_argument("empty name");

		districtNumgenerator++;
		if (partiesSize)
		{
			votesByParty = new int[partiesSize];
			memset(votesByParty, 0, partiesSize * (sizeof(int)));
			logSize = partiesSize;
			phySize = partiesSize;
		}
		else
		{
			votesByParty = nullptr;
			logSize = 0;
			phySize = 0;
		}
	}

	District::~District()
	{
		delete[] votesByParty;
		if (candidatesByParty != nullptr)
			delete candidatesByParty;
	}

	//GETS
	int District::getTNum() const
	{
		return districtNum;
	}

	const std::string& District::getName() const
	{
		return name;
	}

	int District::getNumberOfCitizens() const
	{
		return voters.size();
	}

	int District::getRepresentNum() const
	{
		return representNum;
	}

	int District::getSumVotes() const
	{
		return sumVotes;
	}

	int District::getVotesForParty(int partyNum)
	{
		return votesByParty[partyNum];
	}

	int District::getDistrictNumgenerator() const
	{
		return districtNumgenerator;
	}

	int* District::getVotesForPartyArray()
	{
		return votesByParty;
	}

	std::list<Citizen*>* District::getCandidatesOfParty(Party* p)
	{
		return candidatesByParty->getCitizenArray(p);
	}

	Rep<Party>* District::getDistricRep()
	{
		return candidatesByParty;
	}


	std::map<int, Citizen*>& District::getVoters()
	{
		return voters;
	}


	//SETS
	bool District::setCandidatesByParty(Rep<Party>* rep)
	{
		if (candidatesByParty != nullptr)
			delete candidatesByParty;

		candidatesByParty = rep;
		return true;

	}

	bool District::setDistrictNumgenerator(int num)
	{
		districtNumgenerator = num;
		return true;
	}

	//OPERATORS

	std::ostream& operator<<(std::ostream& os, const District& district)
	{
		district.print(os);
		return os;
	}

	void  District::print(std::ostream& os) const
	{
		os << "Number: " << districtNum << ", Name: " << name << ", Number of representatives: " << representNum << ", District Type: unified" << std::endl;
	}

	void  DividedDistrict::print(std::ostream& os) const
	{
		os << "Number: " << districtNum << ", Name: " << name << ", Number of representatives: " << representNum << ", District Type: divided" << std::endl;
	}

	void District::operator=(const District& other)
	{
		this->districtNum = other.districtNum;
		this->name = other.name;
		this->representNum = other.representNum;
		this->voters = other.voters;
		this->sumVotes = other.sumVotes;
		this->votePercentage = other.votePercentage;
		this->logSize = other.logSize;
		this->phySize = other.phySize;
		this->candidatesByParty = other.candidatesByParty;

		if (votesByParty != nullptr)
		{
			votesByParty = new int[phySize];
			for (int i = 0; i < logSize; i++)
			{
				votesByParty[i] = other.votesByParty[i];
			}
		}
	}

	//OTHER
	void District::addVoter(Citizen* ciz)
	{
		voters.insert(std::pair<int, Citizen*>(ciz->getId(), ciz));
	}

	void District::addParty()
	{
		//only enlarge array if necessery
		if (votesByParty == nullptr)
		{
			votesByParty = new int[2];
			memset(votesByParty, 0, 2 * (sizeof(int)));
			phySize = 2;
		}
		else if (logSize == phySize)//resize
		{
			int* temp = new int[phySize * 2];
			memset(temp, 0, phySize * 2 * (sizeof(int)));
			for (int i = 0; i < logSize; i++)
			{
				temp[i] = votesByParty[i];
			}
			delete[] votesByParty;
			votesByParty = temp;
			phySize = phySize * 2;
		}
		votesByParty[logSize++] = 0;
	}

	void District::addVote(int partyNum)
	{
		votesByParty[partyNum]++;
		sumVotes++;
	}

	void District::updateVotePercentage()
	{
		votePercentage = HelperFunc::getPercentage(sumVotes, voters.size());
	}
	
	//SAVE AND LOAD
	bool District::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, districtNum))) // saving districtNum
			return false;
		if (!(SaveAndLoad::saveToBinaryFileSTL(outfile, name))) // saving name
			return false;
		if (!(sal->saveToBinaryFileSTL(outfile, voters))) // saving voters
			return false;
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, representNum))) // saving representNum
			return false;
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, sumVotes))) // saving sumVotes
			return false;
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, votePercentage))) // saving votePercentage
			return false;
		if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, logSize))) // saving logSize
			return false;

		for (int i = 0; i < logSize; i++) //saving votesByParty
		{
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, votesByParty[i])))
				return false;
		}
		
		char ch = 'N';

		//saving candidatesByParty
		if (candidatesByParty == nullptr) 
		{
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, ch)))
				return false;
		}
		else
		{
			ch = 'E';
			int districtRepIndex = sal->getIndex(candidatesByParty);
			if (districtRepIndex == -1) // index not found
				return false;
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, ch)))
				return false;
			if (!(SaveAndLoad::saveToBinaryFilePrimitive(outfile, districtRepIndex))) // saving districtRepIndex
				return false;
		}

		return true;
	}
	
	bool District::load(std::istream& infile, SaveAndLoad* sal)
	{
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, districtNum))) // loading districtNum
			return false;
		if (!(SaveAndLoad::loadFromBinaryFileSTL(infile, name))) // loading name
			return false;
		if (!(sal->loadFromBinaryFileSTL(infile, voters))) // loading voters
			return false;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, representNum))) // loading representNum
			return false;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, sumVotes))) // loading sumVotes
			return false;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, votePercentage))) // loading votePercentage
			return false;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, logSize))) // loading logSize
			return false;

		phySize = logSize;
		votesByParty = new int[logSize];
		for (int i = 0; i < logSize; i++)// loading votesByParty
		{
			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, votesByParty[i])))
				return false;
		}

		//loading candidatesByParty
		char e;
		if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, e))) // loading 'N' or 'E'
			return false;

		if (e != 'N')
		{
			int districtRepIndex;
			if (!(SaveAndLoad::loadFromBinaryFilePrimitive(infile, districtRepIndex))) // loading districtIndex
				return false;

			candidatesByParty = sal->getDistrictRep(districtRepIndex);
		}

		return true;
	}
	
	//SAVE AND LOAD
	bool DividedDistrict::save(std::ostream& outfile, SaveAndLoad* sal)
	{
		return (this->District::save(outfile, sal));
	}
	bool DividedDistrict::load(std::istream& infile, SaveAndLoad* sal)
	{
		return (this->District::load(infile, sal));
	}
}
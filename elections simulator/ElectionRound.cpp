#include "ElectionRound.h"
#include "SaveAndLoad.h"
namespace election
{
	ElectionRound::ElectionRound() :districts(), parties(), citizens(), m(), year(2021), month(1), day(1){}

	ElectionRound::~ElectionRound() {}

	bool ElectionRound::addDistrict(const std::string& districtName, int representNum, int serialType)
	{

		District* dis = m.createNewDistrict(districtName, representNum, serialType);//according to serialType 
		districts.add(dis);

		if (parties.getLength())//Adding district to DistrictRep in all parties 
		{
			for (int i = 0; i < parties.getLength(); i++)
			{
				parties.getTByIndex(i)->addDistrict(dis);
			}
		}
		return true;
	}

	bool ElectionRound::addCitizen(const std::string& name, int id, int birthYear, int districtNum)
	{
		District* d = districts.getT(districtNum);

		if (year - birthYear <= 18)
			throw std::invalid_argument("age is below 18");

		Citizen* ciz = m.createNewCitizen(id, name, birthYear, d);
		std::pair<std::map<int, Citizen*>::iterator, bool> pair = citizens.insert(std::pair<int, Citizen*>(ciz->getId(), ciz));
		if (!pair.second)
		{
			delete ciz;
			throw std::invalid_argument("citizen already exist");
		}

		d->addVoter(ciz);

		return true;
	}

	bool ElectionRound::addParty(const std::string& name, int idPresident)
	{
		std::map<int,Citizen*>::iterator itr = citizens.find(idPresident); //checks if idPresident exist
	
		if (itr == citizens.end())
			throw std::invalid_argument("president doesnt exist");
	
		Citizen* president = (*itr).second;

		if (president->isCandidate()) //checks if president is already a candidate
			throw std::invalid_argument("citizen already candidate");
		
		Party* p = m.createNewParty(name, president, &districts);
		parties.add(p);

		for (int i = 0; i < districts.getLength(); i++)//Adding to votesByParty in each district another cell for the party
		{
			districts.getTByIndex(i)->addParty();
		}
		return true;
	}

	bool ElectionRound::addCandidate(int idCandidate, int partyNum, int districtNum)
	{
		Party* party = parties.getT(partyNum);
		if (party == nullptr)
			throw std::invalid_argument("party doesnt exist");
		District* district = districts.getT(districtNum);

		std::map<int, Citizen*>::iterator itr = citizens.find(idCandidate); //checks if idCandidate exist
		if (itr == citizens.end())
			throw std::invalid_argument("candidate doesnt exist");
		Citizen* candidate = (*itr).second;

		if (candidate->isCandidate())//If the citizen is already a candidate or a president
			throw std::invalid_argument("already candidate");

		//candidate->setCandidate();
		party->addCandidate(candidate, district);
		return true;
	}


	const int ElectionRound::getYear() const
	{
		return year;
	}
	const int ElectionRound::getMonth() const
	{
		return month;
	}
	const int ElectionRound::getDay() const
	{
		return day;
	}
	PointerArray<District>& ElectionRound::getDistricts()
	{
		return districts;
	}
	PointerArray<Party>& ElectionRound::getParties()
	{
		return parties;
	}
	std::map<int, Citizen*>& ElectionRound::getCitizens()
	{
		return citizens;
	}

	void ElectionRound::setDate(int _year, int _month, int _day)
	{
		year = _year;
		month = _month;
		day = _day;
	}

	void ElectionRound::printDistricts()
	{
		std::cout << "Districts:" << std::endl;
		std::cout << districts;
	}

	void ElectionRound::printCitizens()
	{
		std::cout << "Citizens:" << std::endl;
		HelperFunc::printPointersMap(citizens);
	}

	void ElectionRound::printParties()
	{
		std::cout << "Parties:" << std::endl;
		std::cout << parties;
	}

	bool ElectionRound::vote(int id, int partynum)
	{
		std::map<int, Citizen*>::iterator itr = citizens.find(id); //checks if id exist
		//checking if citizen exist, if he already voted and if the party exist.
		if (itr == citizens.end())
			throw std::invalid_argument("citizen doesnt exist");
		Citizen* ciz = (*itr).second;

		if (ciz->isVoted())
			throw std::invalid_argument("citizen already voted");
		if (!parties.checkIfExist(partynum))
			throw std::invalid_argument("party doesnt exist");

		ciz->getDistrict()->addVote(partynum);
		ciz->setVoted();
		return true;
	}

	bool ElectionRound::election()
	{
		int partiesSize = parties.getLength();
		int districtsSize = districts.getLength();
		if (districtsSize == 0 || partiesSize == 0)//Can't start election if there are no districts or no parties
			throw std::invalid_argument("elections cannot be calculated");

		bool check = true;
		int* sumOfCandidatesByParty = nullptr;
		int* sumOfVotesByParty = nullptr;

		try {
			sumOfCandidatesByParty = new int[partiesSize];
			sumOfVotesByParty = new int[partiesSize];

		}
		catch (std::bad_alloc& ex) { delete[]sumOfCandidatesByParty; delete[]sumOfVotesByParty; throw; }

		PointerArray<Citizen> presidentWonByDistrict;
		memset(sumOfCandidatesByParty, 0, partiesSize * (sizeof(int)));
		memset(sumOfVotesByParty, 0, partiesSize * (sizeof(int)));

		District* district;
		for (int i = 0; i < districtsSize && check; i++)//calculating the results in each district
		{
			district = districts.getTByIndex(i);
			try { electionDistrict(district, sumOfCandidatesByParty, sumOfVotesByParty, presidentWonByDistrict); }//return false if one of the parties doesn't have enough candidates.
			catch (std::exception& ex) { delete[]sumOfCandidatesByParty; delete[]sumOfVotesByParty; throw; }

		}
		//if (check)
		//{
		printResult(sumOfCandidatesByParty, sumOfVotesByParty, presidentWonByDistrict);
		//}
		delete[] sumOfCandidatesByParty;
		delete[] sumOfVotesByParty;
		return check;
	}

	bool ElectionRound::electionDistrict(District* district, int* sumOfCandidatesByParty, int* sumOfVotesByParty, PointerArray<Citizen>& presidentWonByDistrict)
	{
		int indexPartyNumWon;

		district->updateVotePercentage();                                                      //vote percentage
		int* representsByParty = getRepresentsByPartyForDistrict(*(district));                 //gets number of candidates for each party
		Rep<Party>* CandidatesElected = createCandidatesByParty(representsByParty, district); //create CitizenArray* of all candidates elected
		if (CandidatesElected == nullptr)
			throw std::invalid_argument("elections cannot be calculated");
		district->setCandidatesByParty(CandidatesElected);                                     //DistrictRep* placement
		indexPartyNumWon = findDistrictWinner(representsByParty);                              //finds the winner of the district
		representsToAdd(district, sumOfCandidatesByParty, indexPartyNumWon, representsByParty);               //adding representatives to the party who won
		presidentWonByDistrict.add(&(parties.getTByIndex(indexPartyNumWon)->getPresident()));

		for (int j = 0; j < parties.getLength(); j++)//sums votes for parties
		{
			sumOfVotesByParty[j] += district->getVotesForParty(j);
		}
		delete[] representsByParty;
		return true;
	}

	int* ElectionRound::getRepresentsByPartyForDistrict(District& district)
	{
		int partiesSize = parties.getLength();
		int* represents = nullptr;
		float* votesByParty = nullptr;

		try {
			represents = new int[partiesSize];
			votesByParty = new float[partiesSize];
		}
		catch (std::bad_alloc& ex) { delete[]represents; delete[]votesByParty; throw; }

		int representesLeft = district.getRepresentNum();
		float votesPerRepresent = static_cast<float>(district.getSumVotes()) / district.getRepresentNum();
		for (int i = 0; i < partiesSize; i++) //inserting represents before leftovers
		{
			votesByParty[i] = static_cast<float>(district.getVotesForParty(i));
			if (votesPerRepresent != 0)
			{
				represents[i] = static_cast<int>(votesByParty[i] / votesPerRepresent);//Adding represents
			}
			else
			{
				represents[i] = 0;
			}
			representesLeft -= represents[i];
			votesByParty[i] -= represents[i] * votesPerRepresent;
		}

		float max;
		int partyNumMax;
		while (representesLeft != 0) //leftovers
		{
			max = votesByParty[0];
			partyNumMax = 0;
			for (int i = 1; i < partiesSize; i++)//finds the next party who should get the next representative. 
			{
				if (votesByParty[i] > max)
				{
					max = votesByParty[i];
					partyNumMax = i;
				}
			}
			represents[partyNumMax]++;
			votesByParty[partyNumMax] = 0;
			representesLeft--;
		}

		delete[] votesByParty;
		return represents;
	}

	Rep<Party>* ElectionRound::createCandidatesByParty(int* representsByParty, District* district)
	{
		int partiesSize = parties.getLength();

		Rep<Party>* rep = new Rep<Party>;

		std::list<Citizen*>* temp;
		std::list<Citizen*>* arr;
		Party* party;

		int* partyOrder = nullptr;
		int k = 0;
		if (typeid(*district) == typeid(DividedDistrict)) // will get the decending order of the parties indexes by candidates number
		{
			try { partyOrder = HelperFunc::indexesOrder(representsByParty, partiesSize); }
			catch (std::bad_alloc& ex) { delete rep; throw; }

		}

		for (int i = 0; i < partiesSize; i++)
		{
			if (typeid(*district) == typeid(DividedDistrict))
			{
				if (partyOrder != nullptr)
					k = partyOrder[i];
			}
			else
				k = i;

			try { arr = new std::list<Citizen*>; }
			catch (std::bad_alloc& ex) { delete rep; delete[]partyOrder; throw; }

			party = parties.getTByIndex(k);
			rep->addT(party);
			temp = party->getCandidatesOfDistrict(district); // gets all candidates of party in the district
			if (temp != nullptr)
			{
				if (representsByParty[k] > temp->size()) // check if represents needed is bigger then the represents that exists.
				{
					delete[] partyOrder;
					delete arr;
					return nullptr;
				}

				int j = 0;
				for (auto itr : *temp)
				{				
					if (j == representsByParty[k])
						break;
					arr->push_back(itr);
					j++;
				}
				rep->setCitizenArray(party, arr);
			}
			else
			{
				if (representsByParty[k] > 0)// if there are no candidates but the party needs to have at least 1 candidate.
				{
					delete[] partyOrder;
					delete arr;
					return nullptr;
				}
				rep->setCitizenArray(party, arr);
			}
		}
		delete[] partyOrder;
		return rep;
	}

	int ElectionRound::findDistrictWinner(int* representsByParty)
	{
		int max = representsByParty[0];
		int partyNumMax = 0;
		for (int i = 1; i < parties.getLength(); i++)
		{
			if (representsByParty[i] > max)
			{
				max = representsByParty[i];
				partyNumMax = i;
			}
		}

		return partyNumMax;
	}

	void ElectionRound::representsToAdd(District* district, int* sumOfCandidatesByParty, int indexPartyNumWon, int* representsByParty)
	{
		if (typeid(*district) == typeid(District))
		{
			sumOfCandidatesByParty[indexPartyNumWon] += district->getRepresentNum();
		}
		if (typeid(*district) == typeid(DividedDistrict))
		{
			for (int i = 0; i < parties.getLength(); i++)
			{
				sumOfCandidatesByParty[i] += representsByParty[i];
			}
		}
	}

	void ElectionRound::printResult(int* sumOfCandidatesByParty, int* sumOfVotesByParty, PointerArray<Citizen>& presidentWonByDistrict)
	{
		printDate();
		printResultdistric(presidentWonByDistrict);
		printResultparty(sumOfCandidatesByParty, sumOfVotesByParty);
	}

	void ElectionRound::printDate() const
	{
		std::cout << "Date: " << day << "/" << month << "/" << year << std::endl;
	}

	void ElectionRound::printResultdistric(PointerArray<Citizen>& presidentWonByDistrict) const
	{
		District* district;
		std::cout << "/**********************************************************************************/" << std::endl;
		for (int i = 0; i < districts.getLength(); i++)//districts
		{
			district = districts.getTByIndex(i);
			printDistrictInfo(district, presidentWonByDistrict.getTByIndex(i)->getName());

			for (int j = 0; j < parties.getLength(); j++)
			{
				std::cout << "Party:" << parties.getTByIndex(j)->getName() << ", representatives: " << std::endl;
				if(district->getCandidatesOfParty(parties.getTByIndex(j)) != nullptr)
					HelperFunc::printPointersList(*district->getCandidatesOfParty(parties.getTByIndex(j)));
				std::cout << "votes: " << district->getVotesForParty(j) << ", percentage: ";
				std::cout << HelperFunc::getPercentage(district->getVotesForParty(j), district->getSumVotes()) << std::endl << std::endl;
			}
			std::cout << "percentage votes in district: ";
			std::cout << HelperFunc::getPercentage(district->getSumVotes(), district->getNumberOfCitizens()) << std::endl;
			std::cout << "/**********************************************************************************/" << std::endl;
		}
	}

	void ElectionRound::printDistrictInfo(District* district, const std::string& presidentWonByDistrict) const
	{
		std::cout << "District:" << district->getName() << ", number of represents: " << district->getRepresentNum();
		if (typeid(*district) == typeid(District))
			std::cout << ", winner:" << presidentWonByDistrict << std::endl << std::endl;
		if (typeid(*district) == typeid(DividedDistrict))
		{
			const Rep<Party>* dist = district->getDistricRep();
			const Party* p;
			const std::list<Citizen*>* arr;

			std::cout << std::endl;
			for (int i = 0; i < parties.getLength(); i++)
			{
				p = dist->getTByIndex(i);
				arr = dist->getCitizenArray(p);
				if (arr->size() != 0)
				{
					std::cout << "president: " << p->getPresident().getName() << ", number of represents: " << arr->size() << std::endl;
				}
			}
			std::cout << std::endl;
		}
	}

	void ElectionRound::printResultparty(int* sumOfCandidatesByParty, const int* sumOfVotesByParty) const
	{
		Party* party;
		int* partyPrintOrderByIndex = HelperFunc::indexesOrder(sumOfCandidatesByParty, parties.getLength()); //getting the print order of the parties.
		std::cout << "Parties:" << std::endl;
		int j;
		for (int i = 0; i < parties.getLength(); i++)
		{
			j = partyPrintOrderByIndex[i];
			party = parties.getTByIndex(j);
			std::cout << "Party:" << party->getName() << "; president: " << party->getPresident().getName() << ", number of representatives: " << sumOfCandidatesByParty[j] << ", total votes: " << sumOfVotesByParty[j] << std::endl;
			//std::cout << std::endl << std::endl;
		}
		std::cout << "/**********************************************************************************/" << std::endl;
		delete[] partyPrintOrderByIndex;
	}

	//SAVE AND LOAD
	bool ElectionRound::save(std::string fileName)
	{
		SaveAndLoad* sal = new SaveAndLoad; 
		sal->createSAL(*this); // getting data from ElectionRound to SAVEANDLOAD

		std::ofstream outfile; //opening file
		outfile.open(fileName, std::ios::binary| std::ios::trunc);
		if (!outfile.is_open())
			return false;

		if (!SaveAndLoad::saveElectionRoundType(this, outfile)) //save type to file 
			return false;

		//saving date
		if (!SaveAndLoad::saveToBinaryFilePrimitive(outfile, year))
			return false;

		if (!SaveAndLoad::saveToBinaryFilePrimitive(outfile, month))
			return false;

		if (!SaveAndLoad::saveToBinaryFilePrimitive(outfile, day))
			return false;

		bool check = sal->saveSAL(outfile); //saving data to binary file
		
		outfile.close(); //closing file
		if (outfile.is_open())
			return false;

		delete sal;
		
		return check;
	}
	
	bool ElectionRound::load(std::istream& infile)
	{
		SaveAndLoad* sal = new SaveAndLoad;

		//loading date
		if (!SaveAndLoad::loadFromBinaryFilePrimitive(infile, year))
			return false;

		if (!SaveAndLoad::loadFromBinaryFilePrimitive(infile, month))
			return false;

		if (!SaveAndLoad::loadFromBinaryFilePrimitive(infile, day))
			return false;

		sal->loadSAL(infile); //load sal from binary file
		

		//Input pointers to the arrays 
		Citizen* ciz;
		for (int i = 0; i < sal->getLogSizeA(); i++)
		{
			ciz = sal->getCitizen(i);
			citizens.insert(std::pair<int, Citizen*>(ciz->getId(), ciz));
			m.addCitizen(ciz);
		}

		Party* p;
		for (int i = 0; i < sal->getLogSizeC(); i++)
		{
			p = sal->getParty(i);
			parties.add(p);
			m.addParty(p);
		}

		District* district;
		if (typeid(*this) == typeid(ElectionRound))
		{
			for (int i = 0; i < sal->getLogSizeD(); i++)
			{
				district = sal->getDistrict(i);
				districts.add(district);
				m.addDistrict(district);
			}
		}
		else
		{
			delete districts.getTByIndex(0);
			district = sal->getDistrict(0);
			districts.setPointer(0, district);
			m.addToStart(district);
		}

		delete sal;

		return true;
	}
	
	//SimpleElectionRound
	SimpleElectionRound::SimpleElectionRound() :ElectionRound() {}

	SimpleElectionRound::SimpleElectionRound(const int representNum):ElectionRound()
	{
		this->ElectionRound::addDistrict("General District", representNum,1);//1 for divided district
	}

	SimpleElectionRound::~SimpleElectionRound(){}

	bool SimpleElectionRound::addDistrict(const std::string& districtName, int representNum, int serialType)
	{
		return false;
	}

	bool SimpleElectionRound::addCitizen(const std::string& name, int id, int birthYear, int districtNum)
	{
		return ElectionRound::addCitizen(name, id, birthYear, 0);
	}

	bool SimpleElectionRound::addCandidate(int idCandidate, int partyNum, int districtNum)
	{
		return  ElectionRound::addCandidate(idCandidate, partyNum, 0);
	}

	void SimpleElectionRound::printResult(int* sumOfCandidatesByParty, int* sumOfVotesByParty, PointerArray<Citizen>& presidentWonByDistrict)
	{
		District* district = districts.getTByIndex(0);

		std::cout << "percentage votes ";
		std::cout << HelperFunc::getPercentage(district->getSumVotes(), district->getNumberOfCitizens()) << std::endl << std::endl;

		int* partyPrintOrderByIndex = HelperFunc::indexesOrder(district->getVotesForPartyArray(), parties.getLength());
		Party* party;
		std::list<Citizen*>* arr;
		int j;
		for (int i = 0; i < parties.getLength(); i++)
		{
			j = partyPrintOrderByIndex[i];
			party = parties.getTByIndex(j);
			arr = district->getDistricRep()->getCitizenArray(party);


			std::cout << "Party: " << party->getName() << ", president: " << party->getPresident().getName() << ", number of represents: " << arr->size();
			std::cout << ", votes: " << district->getVotesForParty(j) << ", percentage: ";
			std::cout << HelperFunc::getPercentage(district->getVotesForParty(j), district->getSumVotes()) << std::endl;
			std::cout << "representatives: " << std::endl << arr << std::endl; 
		}
		delete[] partyPrintOrderByIndex;
	}
}
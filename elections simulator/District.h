#pragma once
#include"Citizen.h"
#include "HelperFunc.h"

namespace election
{
	template<class T>class Rep;
	class Party;
	class SaveAndLoad;

	class District
	{
	protected:
		static int districtNumgenerator;
		int districtNum;
		std::string name;
		std::map<int, Citizen*> voters;
		int representNum;
		int sumVotes;
		float votePercentage;

		int* votesByParty;
		int logSize;
		int phySize;
		Rep<Party>* candidatesByParty;
		
	public:
		//CONSTRUCTORS
		District();
		District(const std::string& name, int representNum, int partiesSize);
		virtual ~District();
		District(const District&) = delete;

		//GETS
		int getTNum() const;
		const std::string& getName() const;
		int getNumberOfCitizens() const;
		int getRepresentNum() const;
		int getSumVotes() const;
		int getVotesForParty(int partyNum);
		int getDistrictNumgenerator() const;
		int* getVotesForPartyArray();
		Rep<Party>* getDistricRep();
		std::map<int, Citizen*>& getVoters();
		//Gets the candidates 
		std::list<Citizen*>* getCandidatesOfParty(Party* p);

		//SETS
		bool setCandidatesByParty(Rep<Party>* rep);
		bool setDistrictNumgenerator(int num);

		//OPERATORS
		void operator=(const District&);
		friend  std::ostream& operator<<(std::ostream& os, const District& district);
		virtual void print(std::ostream& os) const;

		//OTHER
		void addVoter(Citizen* ciz);
		void addParty();
		void addVote(int partyNum);
		void updateVotePercentage();

		//SAVE AND LOAD
		virtual bool save(std::ostream& outfile, SaveAndLoad* sal);
		virtual bool load(std::istream& infile, SaveAndLoad* sal);
	};

	class DividedDistrict :public District
	{
	public:
		DividedDistrict() :District(){}
		DividedDistrict(std::string name, int representNum, int partiesSize):District(name, representNum, partiesSize){}
		virtual ~DividedDistrict(){}

		virtual void print(std::ostream& os) const;

		//SAVE AND LOAD
		virtual bool save(std::ostream& outfile, SaveAndLoad* sal);
		virtual bool load(std::istream& infile, SaveAndLoad* sal);
	};
}
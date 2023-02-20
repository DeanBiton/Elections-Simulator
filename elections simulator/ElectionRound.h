#pragma once
#include "ElectionRoundMemory.h"
#include"HelperFunc.h"
namespace election
{

	class ElectionRound 
	{
		
	protected:
		int year;
		int month;
		int day;
		PointerArray<District> districts;
		PointerArray<Party> parties;
		std::map<int, Citizen*> citizens;
		ElectionRoundMemory m;

		/*These functions are for election()*/

		//This function calculate the results for each district and will put the right data to it's destined array.
		bool electionDistrict(District* district, int* sumOfCandidatesByParty, int* sumOfVotesByParty, PointerArray<Citizen>& presidentWonByDistrict);

		//Gets the number of representative for each party in district.
		int* getRepresentsByPartyForDistrict(District& district);

		//Creates a DistrictRep for the district according to the results.
		Rep<Party>* createCandidatesByParty(int* representsByParty, District* district);

		int findDistrictWinner(int* representsByParty);

		//Adds to the represents number in sumOfCandidatesByParty according to the district type.
		void representsToAdd(District* district, int* sumOfCandidatesByParty, int indexPartyNumWon, int* representsByParty);

		//functions for printResult function
		void printDate() const;

		void printResultdistric(PointerArray<Citizen>& presidentWonByDistrict) const;
		void printDistrictInfo(District* district, const std::string& presidentWonByDistrict) const;

		void printResultparty(int* sumOfCandidatesByParty, const int* sumOfVotesByParty) const;

	public:

		ElectionRound();
		~ElectionRound();

		virtual bool addDistrict(const std::string& districtName, int representNum,int serialType =0);
		virtual bool addCitizen(const std::string& name, int id, int birthYear, int districtNum);
		bool addParty(const std::string& name, int idPresident);
		virtual bool addCandidate(int idCandidate, int partyNum, int districtNum);

		const int getYear() const;
		const int getMonth() const;
		const int getDay() const;
		PointerArray<District>& getDistricts();
		PointerArray<Party>& getParties();
		std::map<int, Citizen*>& getCitizens();

		void setDate(int _year, int _month, int _day);

		void printDistricts();
		void printCitizens();
		void printParties();

		bool vote(int id, int partynum);

		bool election();

		virtual void printResult(int* sumOfCandidatesByParty, int* sumOfVotesByParty, PointerArray<Citizen>& presidentWonByDistrict);

		//SAVE AND LOAD
		virtual bool save(std::string fileName);
		virtual bool load(std::istream& infile);
	};

	class SimpleElectionRound : public ElectionRound
	{

	public:
		SimpleElectionRound();
		SimpleElectionRound(const int representNum);
		~SimpleElectionRound();
		virtual bool addDistrict(const std::string& districtName, int representNum, int serialType = 0);
		virtual bool addCitizen(const std::string& name, int id, int birthYear, int districtNum) override;
		virtual bool addCandidate(int idCandidate, int partyNum, int districtNum) override;

		void printResult(int* sumOfCandidatesByParty, int* sumOfVotesByParty, PointerArray<Citizen>& presidentWonByDistrict);
	};
}

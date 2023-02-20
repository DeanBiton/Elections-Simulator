#define _CRT_SECURE_NO_WARNINGS
#include "Main.h"
using namespace std;
using namespace election;


bool Main::loadCase(ElectionRound*& r1)
{
	std::string fileName;
	std::ifstream infile;

	std::cout << "Enter file name" << std::endl;
	std::cin >> fileName;
	
	infile.open(fileName, std::ios::binary);
	if (!infile.is_open())
	{
		throw "File was not opened successfully";
	}
	else
	{
		r1 = SaveAndLoad::createElectionRound(infile);
		 r1->load(infile);

		infile.close();
		if (infile.is_open())
		{
			throw "File was not closed successfully";
		}
	}

	return true;
}

void Main::electionmain(ElectionRound* r1)
{
	Main::Menu2 key = Main::Menu2::addDistrict;
	bool check = true;

	int representativesNum, id, birthYear, districtNum, partyNum, districtType;
	string name;
	std::fstream file;

	while (key != Main::Menu2::exit)
	{
		HelperFunc::printmenu2();
		key = Main::getOptionMenu2();
		cin.ignore();
		try {
			switch (key)
			{
			case Main::Menu2::addDistrict:
			{
				if (typeid(*r1) == typeid(ElectionRound))
				{
					std::cout << "enter district type:" << std::endl;
					std::cout << "0. Unified" << std::endl;
					std::cout << "1. Divided" << std::endl;
					std::cin >> districtType;
					cin.ignore();
					std::cout << "enter district name" << std::endl;
					std::cin >> name;
					std::cout << "enter number of represents" << std::endl;
					std::cin >> representativesNum;

					r1->addDistrict(name, representativesNum, districtType);
				}

				if (typeid(*r1) == typeid(SimpleElectionRound))
					cout << "cannot add districts"<<endl;

				break;

			}
			case Main::Menu2::addCitizen:
			{

				std::cout << "Enter citizen's name" << std::endl;
				std::cin >> name;
				std::cout << "Enter citizen's id" << std::endl;
				std::cin >> id;
				std::cout << "Enter citizen's birthYear" << std::endl;
				std::cin >> birthYear;
				std::cout << "Enter citizen's district number" << std::endl;
				std::cin >> districtNum;

				r1->addCitizen(name, id, birthYear, districtNum);
				break;
			}
			case Main::Menu2::addParty:
			{
				std::cout << "enter party's name" << std::endl;
				std::cin >> name;
				std::cout << "enter president id" << std::endl;
				std::cin >> id;

				r1->addParty(name, id);
				break;
			}
			case Main::Menu2::addCandidate:
			{
				std::cout << "enter candidate's id" << std::endl;
				std::cin >> id;
				std::cout << "enter candidate's party number" << std::endl;
				std::cin >> partyNum;
				std::cout << "enter candidate's district number" << std::endl;
				std::cin >> districtNum;

				r1->addCandidate(id, partyNum, districtNum);
				break;
			}
			case Main::Menu2::printDistricts:
			{
				r1->printDistricts();
				break;
			}
			case Main::Menu2::printCitizens:
			{
				r1->printCitizens();
				break;
			}
			case Main::Menu2::printParties:
			{
				r1->printParties();
				break;
			}
			case Main::Menu2::vote:
			{
				std::cout << "enter id" << std::endl;
				std::cin >> id;
				std::cout << "enter party number" << std::endl;
				std::cin >> partyNum;

				r1->vote(id, partyNum);
				break;
			}
			case Main::Menu2::election:
			{
				r1->election();
				break;
			}
			case Main::Menu2::exit:
			{
				std::cout << "goodbye :)" << std::endl;
				break;
			}
			case Main::Menu2::save:
			{
				std::cout << "enter file name" << std::endl;
				std::cin >> name;
				r1->save(name);
				break;
			}
			case Main::Menu2::load:
			{
				delete r1;
				r1 = nullptr;
				loadCase(r1);
				break;
			}
			default:
			{
				std::cout << "wrong menu selection" << std::endl;
				break;
			}
			}
		}
		catch (std::invalid_argument & ex) { cout << ex.what() << endl; }

	}
}

Main::Menu1 Main::getOptionMenu1()
{
	int option;
	
	cin >> option;

	return static_cast<Menu1>(option);
}

Main::Menu2 Main::getOptionMenu2()
{
	int option;

	cin >> option;

	return static_cast<Menu2>(option);
}
int main()
{
	try {
		ElectionRound* r1 = nullptr;
		int year=0, month=0, day=0, representNum = 0;
		int type=0;
		Main::Menu1 key;
		HelperFunc::printmenu1();
		key = Main::getOptionMenu1();
		cin.ignore();

		try {
			switch (key)
			{
			case Main::Menu1::create:
			{
				int simpletype = static_cast<int>(electionRoundType::SimpleElectionRound);
				int normal = static_cast<int>(electionRoundType::ElectionRound);
				

				while (!HelperFunc::checkDate(day, month, year))
				{
					std::cout << "Enter year" << std::endl;
					std::cin >> year;
					std::cout << "Enter month" << std::endl;
					std::cin >> month;
					std::cout << "Enter day" << std::endl;
					std::cin >> day;
				}
				type = -1;
				while (!(SaveAndLoad::checkElectionType(type)))
				{
					std::cout << "Enter election type:" << std::endl;
					std::cout << "1. Election round" << std::endl;
					std::cout << "2. Simple election round" << std::endl;
					std::cin >> type;
				}

				if (type == simpletype)
				{
					std::cout << "Enter number of represents" << std::endl;
					std::cin >> representNum;

					while (representNum <= 0)
					{
						std::cout << "Illigal number of represents, please try again" << std::endl;
						std::cin >> representNum;
					}
				}

				r1 = SaveAndLoad::createElectionRound(static_cast<electionRoundType>(type), representNum);
				r1->setDate(year, month, day);
				Main::electionmain(r1);

				break;
			}
			case Main::Menu1::load:
			{
				if (Main::loadCase(r1))
					Main::electionmain(r1);

				break;
			}
			case Main::Menu1::exit:
			{
				std::cout << "goodbye :)" << std::endl;
				break;
			}
			default:
			{
				std::cout << "wrong menu selection" << std::endl;
				break;
			}
			}
		}
		catch (std::bad_alloc) { cout << "allocation faild" << endl; }
		catch (exception) { cout << "Error load/save file" << endl; }
		catch (const char* msg) { cout << msg << endl; }
	}
	catch (...) { cout << "something went wrong" << endl; }
}


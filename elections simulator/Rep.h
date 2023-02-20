#pragma once
#include "PointerArray.h"
#include "HelperFunc.h"
namespace election
{
	template <typename T>
	class Rep
	{
	private:
		PointerArray<T> tPointers;
		PointerArray<std::list<Citizen*>> citizensLists;

	public:
		////CONSTRUCTOR AND DESTRUCTOR
		Rep() : tPointers(), citizensLists() {}
		Rep(const PointerArray<T>& other) : tPointers(other), citizensLists()
		{
			for (int i = 0; i < tPointers.getLength(); i++)
			{
				citizensLists.add(nullptr);
			}
		}
		Rep(const Rep& other) = delete;
		~Rep() 
		{
			for(int i = 0; i < citizensLists.getLength(); i++)
			{
				delete citizensLists.getTByIndex(i);
			}
		}

		////GETS
		int getLogSize() const
		{
			return tPointers.getLength();
		}

		const PointerArray<T>& getTPointersArray() const
		{
			return tPointers;
		}
		T* getTByIndex(const int i) const
		{
			return tPointers.getTByIndex(i);
		}

		//get matched array according to T* t
		std::list<Citizen*>* getCitizenArray(const T* t) const
		{
			std::list<Citizen*>* lst = nullptr;
			int i = tPointers.getIndexByT(t);
			if (i != -1)
			{
				lst = citizensLists.getTByIndex(i);
			}

			return lst;
		}
		std::list<Citizen*>* getCitizenArray(const int index)
		{
			if (index >= 0 && index < citizensLists.getLength())
			{
				return citizensLists.getTByIndex(index);
			}
			else
			{
				return nullptr;
			}
		}

		////SETS
		//set array according to T* t
		bool setCitizenArray(T* t, std::list<Citizen*>* arr)
		{
			int i = tPointers.getIndexByT(t);
			bool flag = false;

			if (i != -1)
			{
				citizensLists.setPointer(i, arr);
				flag = true;
			}

			return flag;
		}

		////Operators
		void operator=(const Rep& other)
		{
			tPointers = other.tPointers;
			citizensLists = other.citizensLists;
		}

		void print() const;

		////OTHER
		//add citizen to citizen array of the T* t
		bool addCitizen(T* t, Citizen* ciz)
		{
			if (ciz == nullptr)
				throw std::invalid_argument("ciz");

			int i = tPointers.getIndexByT(t);
			if (i == -1)
			{
				throw std::invalid_argument("doesnt exist");
			}
			
			std::list<Citizen*>* lst = citizensLists.getTByIndex(i);
			if (lst == nullptr)
			{
				try { lst = new std::list<Citizen*>; }
				catch (std::bad_alloc& ex) { throw; }

				citizensLists.setPointer(i, lst);
			}
			lst->push_back(ciz);
			return true;
		}

		//add newT to the tpointers array
		bool addT(T* newT)
		{
			if (tPointers.checkIfExist(newT))
				return false;

			tPointers.add(newT);//add newT
			citizensLists.add(nullptr);

			return true;
		}

		bool isTExist(T* t)
		{
			return tPointers.checkIfExist(t);
		}

		////SAVE AND LOAD
		bool save(std::ostream& outfile, SaveAndLoad* sal)
		{
			return (tPointers.save(outfile, sal) && citizensLists.save(outfile, sal));
		}
		bool load(std::istream& infile, SaveAndLoad* sal)
		{
			return (tPointers.load(infile, sal) && citizensLists.load(infile, sal));
		}
	};

}
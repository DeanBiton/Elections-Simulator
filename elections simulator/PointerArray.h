#pragma once
#include "DynamicArray.h"
#include "Party.h"
namespace election
{
	template <typename T>
	class PointerArray
	{
	private:
		DynamicArray<T*> arr;

	public:
		//CONSTRUCTORS AND DESTRUCTOR
		PointerArray() {}
		PointerArray(const PointerArray& other) : arr() {
			(*this) = other;
		}
		~PointerArray() {}

		//GETS
		T* getT(int TNum)
		{
			T* t = nullptr;
			for (auto itr : arr)
			{
				if (itr->getTNum() == TNum)
				{
					t = itr;
					break;
				}
			}

			return t;
		}

		int getLength() const
		{
			return arr.size();
		}

		int getPhySize() const
		{
			return arr.capacity();
		}

		const int getTNumByIndex(const int index) const
		{
			if (index >= arr.size())
				return -1;

			return arr[index]->getTNum();
		}

		const int getIndexByT(const T* other) const
		{
			typename DynamicArray<T*>::const_iterator itr = arr.begin();
			for (; itr != arr.end(); itr++)
			{
				if (*itr == other)
					return itr.getIndex();
			}
			return -1;
		}

		T* getTByIndex(const int index)const
		{
			if (index >= arr.size())
				return nullptr;

			return arr[index];
		}

		bool checkIfExist(T* other)
		{
			if (other == nullptr)
				return true;

			for (auto itr : arr)
			{
				if ((itr->getName() == other->getName()) ||
					(itr->getTNum() == other->getTNum()))
					return true;
			}
			return false;
		}

		bool checkIfExist(const int TNum)
		{
			for (auto itr : arr)
			{
				if (itr->getTNum() == TNum)
					return true;
			}
			return false;
		}

		//OPERATORS
		friend std::ostream& operator<<(std::ostream& os, const PointerArray& other)
		{
			for (auto itr : other.arr)
			{
				os << *itr;
			}
			return os;
		}

		PointerArray& operator=(const PointerArray& other)
		{
			arr = other.arr;
			return *(this);
		}

		//OTHER
		bool add(T* other)//This function adds a T* other to the array.
		{
			arr.insert(arr.end(), other);
			return true;
		}

		void setPointer(int index, T* other)
		{
			arr[index] = other;
		}

		//SAVE AND LOAD
		bool save(std::ostream& outfile, SaveAndLoad* sal);
		bool load(std::istream& infile, SaveAndLoad* sal);
	};
}
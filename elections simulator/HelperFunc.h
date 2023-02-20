#pragma once
#include<iostream>
#include <map>
#include <list>

namespace election
{
	enum class MonthType { January = 1, February, March, April, May, June, July, August, September, October, November, December};

	class Citizen;
	class HelperFunc
	{
	public:
		
		static void printmenu1();
		static void printmenu2();

		static float getPercentage(const int mone, const int mechane);

		/*This function gets an int array arr.
		The function will return a new int array with the indexes of arr ascending order.
		For example: the first number in the new array is the index of the biggest number in arr(if equal, it will put the smaller index first)*/
		static int* indexesOrder(int* arr, int size);

		/*functions for indexesOrder*/
		static void merge(int** arr, int l, int m, int r);
		static void mergeSort(int** arr, int l, int r);

		//create a size number of int array with the size of 2.
		static int** createDoubleArray(int size);

		//deletes a size number of int array with the size of 2.
		static void deleteDoubleArray(int** arr, int size);

		//assume T has a << operator.
		template<class T>
		static void printPointersMap(std::map<int, T*>& map)
		{
			for (auto itr : map)
			{
				std::cout << *(itr.second) << std::endl;
			}
		}

		//assume T has a << operator.
	//	std::list<Citizen*>
		template<class T>
		static void printPointersList(std::list<T*>& lst)
		{
			for (auto itr : lst)
			{
				std::cout << *itr << std::endl;
			}
		}

		static bool checkDate(int day, int month, int year);
	};
}
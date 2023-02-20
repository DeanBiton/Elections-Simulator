#include "HelperFunc.h"

namespace election
{
	float HelperFunc::getPercentage(const int mone, const int mechane)
	{
		if (mechane == 0)
			return 0;

		return static_cast<float>(mone) / mechane*100;
	}

    void HelperFunc::printmenu1()
    {
        std::cout << std::endl;
        std::cout << "1: create a new election round" << std::endl;
        std::cout << "2: load election round" << std::endl;
        std::cout << "3: exit" << std::endl;
        std::cout << std::endl;
    }
	void HelperFunc::printmenu2()
	{
		std::cout << std::endl;
		std::cout << "1: add district" << std::endl;
		std::cout << "2: add citizen" << std::endl;
		std::cout << "3: add party" << std::endl;
		std::cout << "4: add citizen as party candidate" << std::endl;
		std::cout << "5: print all districts" << std::endl;
		std::cout << "6: print all citizens" << std::endl;
		std::cout << "7: print all parties" << std::endl;
		std::cout << "8: vote" << std::endl;
		std::cout << "9: print election results" << std::endl;
		std::cout << "10: exit" << std::endl ;
        std::cout << "11: save" << std::endl;
        std::cout << "12: load" << std::endl;

		std::cout << std::endl;
	}

    int* HelperFunc::indexesOrder(int* arr, int size)
    {
        //creating an array to sort with the indexes
        int** temp = new int* [size];
        for (int i = 0; i < size; i++)
        {
            temp[i] = new int[2];
            temp[i][0] = arr[i];
            temp[i][1] = i;
        }

        //sorting
        mergeSort(temp, 0, size - 1);

        //creating and returning the ordered array.
        int* indexesOrder = new int[size];
        for (int i = 0; i < size; i++)
        {
            indexesOrder[i] = temp[i][1];
            delete[] temp[i];
        }

        delete[] temp;
        return indexesOrder;
    }
    
    void HelperFunc::merge(int** arr, int l, int m, int r)
    {
        int n1 = m - l + 1;
        int n2 = r - m;

        int** L = createDoubleArray(n1);
        int** R = createDoubleArray(n2);

        // Copy data to temp arrays L and R
        for (int i = 0; i < n1; i++)
        {
            L[i][0] = arr[l + i][0];
            L[i][1] = arr[l + i][1];
        }

        for (int j = 0; j < n2; j++)
        {
            R[j][0] = arr[m + 1 + j][0];
            R[j][1] = arr[m + 1 + j][1];
        }

        int i = 0;
        int j = 0;
        int k = l;

        //comparing numbers from both arrays and inputs accordingly 
        while (i < n1 && j < n2)
        {
            if ((L[i][0] > R[j][0]) || (L[i][0] == R[j][0] && L[i][1] < R[j][1]))
            {
                arr[k][0] = L[i][0];
                arr[k][1] = L[i][1];
                i++;
            }
            else
            {
                arr[k][0] = R[j][0];
                arr[k][1] = R[j][1];
                j++;
            }
            k++;
        }

        //fills the rest of the elements

        while (i < n1) {
            arr[k][0] = L[i][0];
            arr[k][1] = L[i][1];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k][0] = R[j][0];
            arr[k][1] = R[j][1];
            j++;
            k++;
        }
        deleteDoubleArray(L, n1);
        deleteDoubleArray(R, n2);
    }

    void HelperFunc::mergeSort(int** arr, int l, int r)
    {
        if (l >= r) {
            return;
        }
        int m = (l + r - 1) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }

    int** HelperFunc::createDoubleArray(int size)
    {
        int** arr = new int* [size];
        for (int i = 0; i < size; i++)
        {
            arr[i] = new int[2];
        }
        return arr;
    }

    void HelperFunc::deleteDoubleArray(int** arr, int size)
    {
        for (int i = 0; i < size; i++)
        {
            delete[] arr[i];
        }
        delete[] arr;
    }

	bool HelperFunc::checkDate(int day, int month, int year)
	{
		if (year < 1 || day < 1)
			return false;

        MonthType M = static_cast<MonthType>(month);
		bool check = true;

		switch (M) {
        case MonthType::January:
			if (day > 31)
				check = false;
			break;
        case MonthType::February:
			if (day > 28)
				check = false;
			break;
        case MonthType::March:
			if (day > 31)
				check = false;
			break;
        case MonthType::April:
			if (day > 30)
				check = false;
			break;
        case MonthType::May:
			if (day > 31)
				check = false;
			break;
        case MonthType::June:
			if (day > 30)
				check = false;
			break;
        case MonthType::July:
			if (day > 31)
				check = false;
			break;
        case MonthType::August:
			if (day > 31)
				check = false;
			break;
        case MonthType::September:
			if (day > 30)
				check = false;
			break;
        case MonthType::October:
			if (day > 31)
				check = false;
			break;
        case MonthType::November:
			if (day > 30)
				check = false;
			break;
        case MonthType::December:
			if (day > 31)
				check = false;
			break;
		default:
			check = false;
		}
			return check;

	}
}
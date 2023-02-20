#include "Rep.h"
#include "SaveAndLoad.h"
namespace election
{
	template<>
	void Rep<District>::print() const
	{
		for (int i = 0; i < tPointers.getLength(); i++)
		{
			if (citizensLists.getTByIndex(i) != nullptr)
			{
				std::cout << "Candidates of district " << tPointers.getTNumByIndex(i) << ":" << std::endl;
				HelperFunc::printPointersList(*citizensLists.getTByIndex(i));
				std::cout << std::endl;
			}
		}
	}
}
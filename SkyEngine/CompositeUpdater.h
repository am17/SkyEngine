#pragma once
#include <list>

using namespace std;

template<class Item>
class CompositeUpdater
{
public:
	CompositeUpdater(list<Item> *aComponentList);
	bool update();
protected:
	virtual bool updateComponent(const Item&) = 0;
private:
	list<Item> *pComponentList;
};

template<class Item>
CompositeUpdater<Item>::CompositeUpdater(list<Item> *aComponentList)
	:pComponentList(aComponentList)
{}

template<class Item>
bool CompositeUpdater<Item>::update()
{
	bool result = false;

	for (list<Item>::iterator it = pComponentList->begin(); it != pComponentList->end(); ++it)
	{
		Item currItem = *it;

		if (currItem)
		{
			result = updateComponent(currItem);

			if (!result)
			{
				break;
			}
		}
	}

	return result;
}
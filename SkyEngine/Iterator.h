#pragma once

template<class Item>
class Iterator
{
public:
	virtual void first() = 0;
	virtual void next() = 0;
	virtual bool isDone() const = 0;
	virtual Item currentItem() const = 0;
};

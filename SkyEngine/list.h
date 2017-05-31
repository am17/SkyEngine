#pragma once
#include "Iterator.h"

template <class Item>
class ListItem{
public:
	ListItem(const Item* tContent);
	~ListItem();
	Item& GetItem();
	bool Compare(Item* test);
	ListItem* next;
private:
	Item* content;

};

template<class Item>
class List
{
public:
	List(long size = DEFAULT_LIST_CAPACITY);
	List(List&);
	~List();
	List& operator=(const List&);

	long count() const;
	Item& get(long index) const;
	Item& first() const;
	Item& last() const;
	bool includes(const Item&) const;

	void append(const Item&);
	void prepend(const Item&);

	void remove(const Item&);
	void removeLast();
	void removeFirst();
	void removeAll();

	Item& top() const;
	void push(const Item&);
	Item& pop();

	Iterator<Item>* createIterator() const;
private:
	long capacity;
	long currentSize;
	ListItem<Item>* rootNode;
	static const long DEFAULT_LIST_CAPACITY = 100;
};

template <class Item>
List<Item>::List(long size) 
	:capacity(size),
	currentSize(0), 
	rootNode(nullptr)
{}

template <class Item>
List<Item>::~List(){

}

template <class Item>
List<Item>& List<Item>::operator=(const List& rhs){
	if (this == &rhs){
		return *this;
	}
	else {
		(*this).capacity = rhs.capacity;
		(*this).currentSize = rhs.currentSize;
		(*this).rootNode = rhs.rootNode;
		return *this;
	}
}

template <class Item>
List<Item>::List(List& rhs){
	(*this).capacity = rhs.capacity;
	(*this).currentSize = rhs.currentSize;
	(*this).rootNode = rhs.rootNode;
}

template <class Item>
long List<Item>::count() const{
	return currentSize;
}

template <class Item>
Item& List<Item>::get(long index) const{
	ListItem<Item>* p = rootNode;
	for (int i = 0; i < index - 1; i++){
		p = p->next;
	}
	return p->GetItem();
}

template <class Item>
Item& List<Item>::first() const{
	return rootNode->GetItem();
}

template <class Item>
Item& List<Item>::last() const{
	return get(currentSize);
}

template <class Item>
bool List<Item>::includes(const Item& test) const{
	ListItem<Item>* p = rootNode;
	for (int i = 0; i < currentSize; i++){
		if (p->Compare(&test)){
			return true;
		}
		p = p->next;
	}
	return false;
}

template <class Item>
void List<Item>::append(const Item& temp){
	ListItem<Item>* tempNode = new ListItem<Item>(&temp);
	if (currentSize == 0)	{
		rootNode = tempNode;
		currentSize++;
	}
	else {
		ListItem<Item>* p = rootNode;
		for (int i = 0; i < currentSize - 1; i++){
			p = p->next;
		}
		p->next = tempNode;
		currentSize++;
	}
}

template <class Item>
void List<Item>::prepend(const Item& temp){
	ListItem<Item>* temp = new ListItem<Item>(&temp);
	if (currentSize == 0)	{
		rootNode = temp;
		currentSize++;
	}
	else {
		temp->next = rootNode;
		rootNode = temp;
		currentSize++;
	}
}

template <class Item>
void List<Item>::remove(const Item& test){
	ListItem<Item>* p = rootNode;
	if (p->Compare(&test)){
		removeFirst();
	}
	else {
		for (int i = 1; i < currentSize; i++){
			ListItem<Item>* temp = p;
			p = p->next;
			if (p->compare(&test)){
				temp->next = p->next;
				delete p;
				currentSize--;
			}
		}
	}
}

template <class Item>
void List<Item>::removeLast(){
	ListItem* p = rootNode;
	for (int i = 1; i < currentSize - 1; i++){
		p = p->next;
	}
	ListItem* temp = p;
	p = p->next;
	delete p;
	temp->next = NULL;
	currentSize--;
}

template <class Item>
void List<Item>::removeFirst(){
	if (currentSize != 1){
		ListItem* p = rootNode;
		rootNode = rootNode->next;
		delete p;
		currentSize--;
	}
	else {
		delete rootNode;
		currentSize--;
	}
}

template <class Item>
Item& List<Item>::pop(){
	Item* temp = &Last();
	RemoveLast();
	return *temp;
}

template <class Item>
void List<Item>::push(const Item& temp){
	append(temp);
}

template <class Item>
Item& List<Item>::top() const{
	return last();
}

template <class Item>
Iterator<Item>* List<Item>::createIterator() const
{
	return new ListIterator<Item>(this);
}

template <class Item>
ListItem<Item>::~ListItem(){

}
template <class Item>
ListItem<Item>::ListItem(const Item* tContent) 
	: content(const_cast<Item*>(tContent)), 
	next(nullptr){

}

template <class Item>
Item& ListItem<Item>::GetItem(){
	return *content;
}

template <class Item>
bool ListItem<Item>::Compare(Item* test){
	if (content == test){
		return true;
	}
	else {
		return false;
	}
}

template<class Item>
class ListIterator : public Iterator<Item>
{
public:
	ListIterator(const List<Item>* aList): _list(aList), _current(0)
	{
		_current = 0;
	}
	virtual void first()
	{
		_current = 0;
	}
	virtual void next()
	{
		_current++;
	}
	virtual bool isDone() const
	{
		return _current >= _list->count();
	}
	virtual Item currentItem() const
	{
		if (!isDone())
		{
			return _list->get(_current);
		}
		else
		{
			throw "OutOfBounds";
		}
	}
private:
	const List<Item>* _list;
	long _current;
};



#include <iostream>
#include <stdlib.h>

using namespace std;

template <class myType>
class linkedStack {
	struct nodeType {
		myType item;
		linkedStack<myType>::nodeType * link;
	};
	public:
		linkedStack();
		~linkedStack();
		bool isEmpty();
		int getStackCount();
		void push(const myType &newItem);
		myType pop();
		myType traversal(myType &node);
		void resetIt();
		void destroyList();
	private:
		int itemCount;
		nodeType *stackTop, *iterator;
};

template <class myType>
linkedStack<myType>::linkedStack() {
	stackTop = NULL;
	iterator = NULL;
	itemCount = 0;
}

template <class myType>
linkedStack<myType>::~linkedStack() {
	linkedStack<myType>::nodeType * current;
	while(stackTop != NULL){
		current = stackTop->link;
		delete stackTop;
		stackTop = current;
	}
	stackTop = NULL;
	iterator = NULL;
	current = NULL;
}

template <class myType>
bool linkedStack<myType>::isEmpty() {
	return stackTop == NULL;
}

template <class myType>
void linkedStack<myType>::destroyList() {
		while (!isEmpty()) {
			pop();
		}
}

template <class myType>
int linkedStack<myType>::getStackCount(){
	return itemCount;
}

template <class myType>
void linkedStack<myType>::push(const myType &newItem){
	linkedStack<myType>::nodeType * node;
	node = new linkedStack<myType>::nodeType;
	if (isEmpty()){
		stackTop = node;
		stackTop->item = newItem;
		stackTop->link = NULL;
	}
	else {
		node->item = newItem;
		node->link = stackTop;
		stackTop = node;
	}
	iterator = stackTop;
	itemCount++;
}

template <class myType>
myType linkedStack<myType>::pop(){
	if (!(isEmpty())) {
		myType info;
		linkedStack<myType>::nodeType * temp = stackTop;
		info = temp->item;
		temp = stackTop->link;
		delete stackTop;
		stackTop = temp;
		itemCount--;
		return info;
	}
	return stackTop->item;
}

template <class myType>
void linkedStack<myType>::resetIt() {
	iterator = stackTop;
}
template <class myType>
myType linkedStack<myType>::traversal(myType &node) {
	node = iterator->item;
	if (iterator->link != NULL) {
		iterator = iterator->link;
	}
	else {
		resetIt();
	}
	return node;
}

#include <iostream>
#include <fstream>
#include <cmath>



template <class Type>
class priorityQueue {
    public:
    priorityQueue(int = 5000);
    ~priorityQueue();
    int entries() const;
    void insert(const Type, const unsigned long long);
    void insertSwap(const Type, const unsigned long long);
    void printHeap() const;
    bool deleteMin(Type &, unsigned long long &);
    bool isEmpty() const;
    bool readData(const std::string);
    
    
    private:
    struct heapNode{
		unsigned long long priority;
		Type item;
	};
    int count;
    int heapSize;
    heapNode *myHeap;
    void swap(int, int);
    void resize();
    void reheapUp(int);
    void reheapDown(int);
    void buildHeap();
};





template <class Type>
priorityQueue<Type>::priorityQueue(int amount){
    if (amount <= 5000){
        heapSize = 5000;
    }else{
        heapSize = amount;
    }
    count = 0;
    
    myHeap = new heapNode [heapSize];
}

template <class Type>
priorityQueue<Type>::~priorityQueue(){
    delete [] myHeap;
    count = 0;
    
    
}

template <class Type>
int priorityQueue<Type>::entries() const{
    return count;
}

template <class Type>
void priorityQueue<Type>::insert(const Type newItem, const unsigned long long pri){
    count++;
    if ( count >= heapSize ){
        resize();
    }
    
    myHeap[count].item = newItem;
    myHeap[count].priority = pri;
    //balancing
    reheapUp(count);
    
    
}

template <class Type>
void priorityQueue<Type>::insertSwap(const Type newItem, const unsigned long long pri){
	for (int i = 0; i < count; i++){
		if(myHeap[i].item == newItem){
			myHeap[i].priority=pri;
			reheapUp(i);
			return;
		}
		
	}
    count++;
    if ( count >= heapSize ){
        resize();
    }
    
    myHeap[count].item = newItem;
    myHeap[count].priority = pri;
    //balancing
    reheapUp(count);
    
    
}

template <class Type>
void priorityQueue<Type>::printHeap() const{
	int n = 1;
    for ( int i = 1; i <= count; i++ ){
        std::cout << myHeap[i].item << "  " << myHeap[i].priority << std::endl;
        if ( (i+1 == pow(2, n)) ){
			std::cout << std::endl;
			n++;
		}
    }
    std::cout << std::endl;
}

template <class Type>
void priorityQueue<Type>::reheapUp(int loc){
    int parent = loc/2;
    if (loc == 1){
        return;
    }
    if ( myHeap[loc].priority < myHeap[parent].priority ){
        swap(loc, parent);
        reheapUp(parent);
    }
}

template <class Type>
void priorityQueue<Type>::reheapDown(int loc){
    int left = loc*2;
    int right = left + 1;
    
    if(left > count){
        return;
    }
    if(right > count){
        if (myHeap[loc].priority > myHeap[left].priority){
            swap(loc, left);
            //reheapDown(left);                                                                   
        }
        return;
    }
    if (myHeap[right].priority < myHeap[left].priority){
        if (myHeap[loc].priority > myHeap[right].priority){
            swap(loc, right);
            reheapDown(right);
        }
    }else{
        if (myHeap[loc].priority > myHeap[left].priority){
            swap(loc, left);
            reheapDown(left);
        }
        
    }
}



template <class Type>
void priorityQueue<Type>::swap(int first, int second){
    heapNode tmp;
    tmp = myHeap[first];
    myHeap[first] = myHeap[second];
    myHeap[second] = tmp;
}

template <class Type>
bool priorityQueue<Type>::deleteMin(Type &returnItem, unsigned long long &returnPri){
    if(count == 0){
        return false;
    }
    if(count == 1){
        returnItem = myHeap[1].item;
        returnPri = myHeap[1].priority;
        count--;
        return true;
    }
    
    returnItem = myHeap[1].item;
    returnPri = myHeap[1].priority;
    swap(1, count);
    count--;
    reheapDown(1);
    return true;
} 

template <class Type>
void priorityQueue<Type>::resize(){
    heapNode *newHeap;
    newHeap = new heapNode [heapSize*2];
    for (int i = 1; i < count; i++){
        newHeap[i].item = myHeap[i].item;
        newHeap[i].priority = myHeap[i].priority;
    }

    heapSize = heapSize*2;
    delete [] myHeap;
    myHeap = newHeap;
}

template <class Type>
bool priorityQueue<Type>::isEmpty() const{
    return(count == 0);
}

template <class Type>
bool priorityQueue<Type>::readData(const std::string fileName){
	bool flag = false;
	std::ifstream inFile;
	Type itemRead;
	unsigned long long priorityRead;
	inFile.open(fileName);
	if( !inFile.is_open()){
		return false;
	}
	
	inFile >> itemRead;
	inFile >> priorityRead;
	while( !inFile.fail() ){
		insert(itemRead, priorityRead);
		inFile >> itemRead;
		inFile >> priorityRead;
		flag = true;
	}
	inFile.close();
	// build heap to balance
	buildHeap();
	return flag;
}

template <class Type>
void priorityQueue<Type>::buildHeap(){
	int loc;
	int parent;
	for (int i = count; i > 1; i--){
		loc = i;
		parent = loc/2;
		while( loc != 1 ){
			if( myHeap[loc].priority < myHeap[parent].priority ){
				swap(loc, parent);
			}
			loc = parent;
			parent = loc/2;
		}
	}
}

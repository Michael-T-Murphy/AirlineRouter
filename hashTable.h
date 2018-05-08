#include <iostream>
#include <string>

using namespace std;


class hashTable {
	public:
		hashTable();
		~hashTable();
		bool insert (const string, const string, const string);
		bool lookup (const string, string&, string&);
		bool remove (const string);
		void printHash() const;
		void showHashStats() const;
		void hashLookup(unsigned int,  string&);
		void refLookup(unsigned int,  string&);
		unsigned int hashReturn(const string) const;
	private:
		unsigned int hash(string) const;
		bool insertIntoHash(const unsigned int, const string, const string , const string);
		bool removeFromHash(const unsigned int);
		void rehash();
		unsigned int hashSize, reSizeCount, collisionCount, entries;
		string **addrHash;
		static constexpr double loadFactor = 0.65;
		static constexpr unsigned int intialHashSize = 15000;
	};

//**************************************************************************************************
// Public Functions
//**************************************************************************************************

hashTable::hashTable() {
	hashSize = intialHashSize;
	reSizeCount = 0;
	collisionCount = 0;
	entries = 0;
	addrHash = new string *[hashSize];
	for (unsigned int i = 0; i < hashSize; i++) {
		addrHash[i] = new string[3];
	}
}

hashTable::~hashTable() {
	for (unsigned int i = 0; i < hashSize; i++) {
		delete [] addrHash[i];
	}
	delete [] addrHash;
}

bool hashTable::insert(const string refKey, const string Name, const string listPos) {
	//Check If In List
	string tmp1, tmp2;
	if (lookup(refKey, tmp1, tmp2)) {
		return false;
	} 

	//Check LoadFactor
	double currLoadFactor = double(entries)/double(hashSize);
	if (currLoadFactor >  loadFactor) {
		rehash();
	}

	//Insert the Values
	unsigned int hashKey = hash(refKey);
	if (addrHash[hashKey][0] == "" || addrHash[hashKey][0] == "*"){
		return insertIntoHash(hashKey, refKey, Name, listPos);
	}
	unsigned int i = hashKey + 1;
	collisionCount++;
	while(1) {
		if (i == hashSize) {
			i = 0;
		}
		if (addrHash[i][0] == "" || addrHash[i][0] == "*") {
			return insertIntoHash(i, refKey, Name, listPos);
		}
		if (i == hashKey) {
			return false;
		}
		i++;
		collisionCount++;
	}
}

void hashTable::hashLookup(unsigned int refKey, string &refCode) {
	refCode = addrHash[refKey][2];
}

void hashTable::refLookup(unsigned int refKey, string &refCode) {
	refCode = addrHash[refKey][0];
}

bool hashTable::lookup(const string refKey, string& Name, string& listPos) {
	unsigned int i = hash(refKey);
	while (addrHash[i][0] != ""){
		if (addrHash[i][0] == refKey) {
			Name = addrHash[i][1];
			listPos = addrHash[i][2];
			return true;
		}
		i++;
		if (i == hashSize) {
			i = 0;
		}
	}
	return false;
}

bool hashTable::remove(const string refKey){
	unsigned int hashKey = hash(refKey);
	//Loop Through to End
	while (addrHash[hashKey][0] != "") {
		if (addrHash[hashKey][0] == refKey) {
			return removeFromHash (hashKey);
		}
		hashKey++;
		if (hashKey == hashSize) {
			hashKey = 0;
		}
	}
	return false; //Should Never Reach Here But It Satisfies The Compiler
}

void hashTable::printHash() const {
	for (unsigned int i = 0; i  < hashSize; i++) {
		if (addrHash[i][0] != "" && addrHash [i][0] != "*") {
			cout << addrHash[i][0] << " : ";
			cout << addrHash[i][1] << " : ";
			cout << addrHash[i][2] << " : " << endl;
		}
	}
}

void hashTable::showHashStats() const{
	cout << "Hash Stats" << endl;
	cout << "   Current Entries Count: " << entries << endl;
	cout << "   Current Hash Size: " << hashSize << endl;
	cout << "   Hash Resize Operations: " << reSizeCount << endl;
	cout << "   Hash Collisions: "<< collisionCount << endl;
}

unsigned int hashTable::hashReturn(const string refKey) const {
	unsigned int i = hash(refKey);
	while (addrHash[i][0] != ""){
		if (addrHash[i][0] == refKey) {
			return i;
		}
		i++;
		if (i == hashSize) {
			i = 0;
		}
	}
	return i;
}
//**************************************************************************************************
// Private Functions
//**************************************************************************************************

bool hashTable::insertIntoHash(const unsigned int key, const string ref, const string name, const string listPos) {
	addrHash[key][0] = ref;
	addrHash[key][1] = name;
	addrHash[key][2] = listPos;
	entries++;
	return true;
}

bool hashTable::removeFromHash(const unsigned int curr) {
	addrHash[curr][0] = "*";
	addrHash[curr][1] = "*";
	addrHash[curr][2] = "*";
	entries--;
	return true;
}

unsigned int hashTable::hash(string key) const{
	unsigned int hash = 0;
// Hash Based on Horner's Rule
 
	for (int i = key.length() - 1; i >= 0; i--) {
		hash = (key[i] + 128*hash) % hashSize;
	} 
	return hash; 
}

void hashTable::rehash() {
	unsigned int oldSize = hashSize;
	hashSize = hashSize*2;
	entries = 0;
	string ** tmpHash;
	tmpHash = addrHash;
	addrHash = NULL;
	addrHash = new string *[hashSize];
	for (unsigned int i = 0; i < hashSize; i++) {
		addrHash[i] = new string [3];
	}
	//Copy the Values from OG Hash
	for (unsigned int i = 0;	i < oldSize; i++) {
		if (tmpHash[i][0] != "" && tmpHash[i][0] != "*") {
			insert(tmpHash[i][0], tmpHash[i][1], tmpHash[i][2]);
		}
	}

	for (unsigned int i = 0; i < oldSize; i++) {
		delete [] tmpHash[i];
	}

	reSizeCount++;
	delete [] tmpHash;
	tmpHash = NULL;
}

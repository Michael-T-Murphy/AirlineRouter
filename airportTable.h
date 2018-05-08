#pragma once
#include <string>
#include <iostream>


struct airPort{
	std::string APcode;
	std::string APname;
	std::string APcity;
	std::string APcountry;
	int APvertex;
};


class airportTable{
    public:
    airportTable();
    ~airportTable();
    bool insert(const std::string, const std::string, const std::string, const std::string, const int);
    unsigned int hash(const std::string);
    bool lookup(const std::string, std::string &, std::string &, std::string &, int &);
    bool nameLookup(const std::string, std::string &);
    void printHash() const;
    bool search(const std::string);
    int vertexLookup(const std::string);
 
    private:
    unsigned int hashSize;
    unsigned int entries;
    airPort *APHash;
    static constexpr unsigned int initialHashSize = 15000;  
};


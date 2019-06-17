#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class AugmentedProduction
{
public:
    char lhs;
    string rhs;

    AugmentedProduction() {}
    AugmentedProduction(char _lhs, string _rhs) : lhs(_lhs), rhs(_rhs) {}
};

class LR0Item
{

    private:
        // list of productions
        vector <AugmentedProduction* > productions;

    public:
        // list of out-edges
        map<int, char> gotos;

        LR0Item() {}
        ~LR0Item() {}

        // add production
        void Push(AugmentedProduction *p);

        // return the number of productions
        int Size();

        // return whether or not this item contains the production prodStr
        bool Contains(string production);

        // overloaded index operator; access pointer to production.
        AugmentedProduction* operator[](const int index);
};

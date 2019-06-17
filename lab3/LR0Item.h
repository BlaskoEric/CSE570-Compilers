#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class ExtendedProduction
{
public:
    char lhs;
    string rhs;

    ExtendedProduction() {}
    ExtendedProduction(char _lhs, string _rhs) : lhs(_lhs), rhs(_rhs) {}
};

class LR0Item
{

    private:
        // list of productions
        vector <ExtendedProduction* > productions;

    public:
        // list of out-edges
        map<int, char> gotos;

        LR0Item() {}
        ~LR0Item() {}

        // add production
        void Push(ExtendedProduction *p);

        // return the number of productions
        int Size();

        // return whether or not this item contains the production prodStr
        bool Contains(string production);

        // overloaded index operator; access pointer to production.
        ExtendedProduction* operator[](const int index);
};

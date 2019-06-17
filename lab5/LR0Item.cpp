#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <map>
#include <cstdlib>
#include "LR0Item.h"

using namespace std;

    // add production
    void LR0Item::Push(AugmentedProduction *p)
    {
        productions.push_back(p);
    }

    // return the number of productions
    int LR0Item::Size()
    {
        return int(productions.size());
    }

    // return whether or not this item contains the production prodStr
    bool LR0Item::Contains(string production)
    {
        for (auto it = productions.begin(); it != productions.end(); it++)
        {
            string existing = string(&(*it)->lhs, 1) + "->" + (*it)->rhs;
            //cout << " Comparing: " << thisStr << " , " << prodStr << endl;
            if (strcmp(production.c_str(), existing.c_str()) == 0)
                return true;
        }
        return false;
    }

    // overloaded index operator; access pointer to production.
    AugmentedProduction* LR0Item::operator[](const int index)
    {
        return productions[index];
    }


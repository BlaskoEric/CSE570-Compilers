/*******************************************************************
 * Name         : Eric Blasko
 * Lab          : Lab 3
 * Date         : 3/4/2019
 * Title        : LR0Item
 * Description  : This class is an object to represent the contents
 *                of the LR sets. Each object has a vector which
 *                contains all of the given productions. Along with
 *                a goto map which states which other state to goto
 *                on the given input
 ********************************************************************/

#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <map>
#include <cstdlib>
#include "LR0Item.h"

using namespace std;

    // add production
    void LR0Item::Push(ExtendedProduction *p)
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
    ExtendedProduction* LR0Item::operator[](const int index)
    {
        return productions[index];
    }


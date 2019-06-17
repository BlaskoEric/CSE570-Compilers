/*******************************************************************
 * Name         : Eric Blasko
 * Lab          : Lab 5
 * Date         : 3/11/2019
 * Title        : Action / Goto Tables
 * Description  : This program reads in a grammar and parses the
 *                grammar to produce the first, follows, and LR(0)
 *                sets. Once these sets have been obtained, we uses
 *                the LR(0) sets and follow sets to produce a action
 *                goto table. From this table we can easily determine
 *                if a grammar is SLR or not. If there is a shift-
 *                shirt conflict or a shift-reduce conflict then it
 *                is not SLR
 ********************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include "LR0Item.h"

using namespace std;

string file = "test";
vector<char> terminal;
vector<char> nonTerminal;
map<char, string> production;
map<char, set<char> > firstMap;
map<char, set<char> > followMap;
typedef map<char, vector<string> > AugmentedGrammar;
typedef map<string, int> gotoMap;

int main();
bool firstProduction = true;
void printAll();
bool isTerminal(char);
void first();
void follow();
void printGrammar();
void nextClosure(string);
void add_closure(char, LR0Item& iem, AugmentedGrammar&);
void getLR0Items(vector<LR0Item>&, AugmentedGrammar&, int&, gotoMap&);
void actionGoto(vector<LR0Item>&, AugmentedGrammar&, int&, gotoMap&);

//Main will open and read in grammar, and push them to proper vector.
//Once complete first and followed are called and everything is printed.
int main()
{
    int itemid = -1;
    AugmentedGrammar AugGrammar;
    vector<LR0Item> lr0items = { LR0Item() };
    gotoMap globalGoto;

    //opening stream
    string line;
    string lhs, rhs;
    string delim = "->";

    fstream input;
    input.open(file.c_str(), ios::in);
    if (!input)
        cerr << "error opening grammar.txt" << endl;

    printf("Augmented Grammar\n");

    //read line by line
    getline(input, line);
    while (!input.eof())
    {
        //check if first value is lower case or other terminal char and push
        //to vector
        char val = line[0];
        if (islower(val) || !isalpha(val))
            terminal.push_back(val);

        //check if first value is upper case and push to vector
        if (isupper(val))
        {
            //push only once to nonTerminal vector. If not empty make sure duplicates
            //are not added
            if (nonTerminal.empty())
            {
                AugGrammar['\''].push_back(val + "");
                string temp = ".";
                temp.push_back(val);
                lr0items[0].Push(new AugmentedProduction('\'', temp));
                printf("'->%c\n", val);
                nonTerminal.push_back(val);
                continue;
            }
            else if (!(find(nonTerminal.begin(), nonTerminal.end(), val) != nonTerminal.end()))
                nonTerminal.push_back(val);

            //if production is empty add current production. Else add to the current
            //productions
            auto pos = line.find(delim);
            rhs = line.substr(pos + delim.length(), std::string::npos);
            if (production[val] == "")
            {
                production[val] = line.substr(3, line.length());
                AugGrammar[val].push_back(rhs);
                printf("%c->%s\n", val, rhs.c_str());
                if (isTerminal(rhs[0]) == false)
                    lr0items[0].Push(new AugmentedProduction(val, "." + rhs));
            }
            else
            {
                production[val] += "|" + line.substr(3, line.length());
                AugGrammar[val].push_back(rhs);
                printf("%c->%s\n", val, rhs.c_str());
                if (isTerminal(rhs[0]) == false)
                    lr0items[0].Push(new AugmentedProduction(val, "." + rhs));
            }
        }

        getline(input, line);
    }

    //close file
    input.close();

    //print grammar, get first set and follow set and print all sets
    //printGrammar();
    first();
    follow();
    printAll();

    //find closures and gotos.Items are printed as then are created
    printf("\nClosures and Gotos\n");
    while (++itemid < int(lr0items.size())) {
        getLR0Items(lr0items, AugGrammar, itemid, globalGoto);
    }

    actionGoto(lr0items, AugGrammar, itemid, globalGoto);

    return 0;
}

//Prints the grammar. Will print each terminal follow by non terminals and
//their productions 
void printGrammar()
{
    printf("Condesned Grammar\n");
    cout << "Grammar" << endl;
    for (int i = 0; i < terminal.size(); i++)
        cout << terminal[i] << endl;

    cout << "$" << endl;

    for (int i = 0; i < nonTerminal.size(); i++)
        cout << nonTerminal[i] << "->" << production[nonTerminal[i]] << endl;

    cout << "$" << endl;


    cout << endl;
}

//Prints the first and follow sets for the given grammar. Each element in the
//set is seperated by a comma
void printAll()
{
    cout << endl;

    //print first set
    printf("First Sets\n");
    for (int i = 0; i < nonTerminal.size(); i++)
    {
        cout << "First(" << nonTerminal[i] << ") = {";
        for (auto it = firstMap[nonTerminal[i]].begin(); it != firstMap[nonTerminal[i]].end(); it++)
        {
            cout << *it;
            auto it2 = next(it, 1);
            if (it2 != firstMap[nonTerminal[i]].end())
            {
                cout << ",";
            }
        }

        cout << "}" << endl;
    }

    cout << endl;

    //print follow set
    printf("Follow Sets\n");
    for (int i = 0; i < nonTerminal.size(); i++)
    {
        cout << "Follow(" << nonTerminal[i] << ") = {";
        for (auto it = followMap[nonTerminal[i]].begin(); it != followMap[nonTerminal[i]].end(); it++)
        {
            cout << *it;
            auto it2 = next(it, 1);
            if (it2 != followMap[nonTerminal[i]].end())
            {
                cout << ",";
            }
        }

        cout << "}" << endl;
    }
}

//Checks if the given char is a terminal or not terminal value
bool isTerminal(char input)
{
    if (input == 'e')
        return true;
    if (find(terminal.begin(), terminal.end(), input) != terminal.end())
        return true;
    else
        return false;
}

//Find the first set for each nonTerminal. Will loop until there are no 
//changes that have been made for an entire read through of the grammer.
void first()
{
    char lhs;
    bool complete = false;
    bool changes = true;
    int i = 0;

    //loop until there has been no changes and i == 0 again
    while (!complete)
    {
        //get nonTerminal
        lhs = nonTerminal[i++%nonTerminal.size()];
        i = i % nonTerminal.size();
        if (i == 0)
            changes = false;

        if (lhs == '\'')
            continue;

        //get nonTerminals productions
        int j = 0;
        bool firstChar = true;
        string expr = production[lhs];

        //loop through production
        while (j < expr.length())
        {
            //if first read of production get first char
            char first;
            if (firstChar == true)
            {
                firstChar = false;
                first = expr[0];
            }
            //if not first char, loop till '|' is found and get next char
            else
            {
                char temp = expr[j];
                while (temp != '|' && j <= expr.length())
                    temp = expr[j++];
                if (j >= expr.length())
                    continue;
                first = expr[j];
            }
            //if value is terminal add value to lhs first set.
            if (isTerminal(first))
            {
                set<char> builder = firstMap[lhs];
                bool found = builder.find(first) != builder.end();
                if (!found)
                {
                    firstMap[lhs].insert(first);
                    changes = true;
                }
            }
            //if not a terminal, add the first of the nonTerminal to lhs
            else
            {
                set<char> next = firstMap[first];
                for (auto k = next.begin(); k != next.end(); k++)
                {
                    char temp = *k;
                    bool found = firstMap[lhs].find(temp) != firstMap[lhs].end();
                    if (!found)
                    {
                        set<char> builder = firstMap[lhs];
                        firstMap[lhs].insert(temp);
                        changes = true;
                    }
                }
            }
        }

        //check if no changes have been made
        if (i == nonTerminal.size() - 1 && changes == false)
            complete = true;
    }
}

//Find the follow set for each nonTerminal. Will loop until there are no 
//changes that have been made for an entire read through of the grammer.
void follow()
{
    char lhs;
    bool complete = false;
    bool changes = false;
    int i = 0;

    //insert "$" to the follow set of the firs non terminal
    followMap[nonTerminal[0]].insert('$');

    //loop until no changes have been made for entire iteration of the 
    //non Terminals
    while (!complete)
    {
        //get a non Terminal
        lhs = nonTerminal[i++%nonTerminal.size()];
        i = i % nonTerminal.size();
        if (i == 0)
            changes = false;

        if (lhs == '\'')
            continue;

        //get its production
        int j = 0;
        bool first = true;
        string expr = production[lhs];

        //loop through entire production
        while (j < expr.length())
        {
            char firstChar;
            char followChar;

            //get the first two char of production
            if (first == true)
            {
                firstChar = expr[0];
                followChar = expr[1];
                if (isTerminal(firstChar))
                {
                    j++;
                    if (firstChar == 'e')
                        continue;
                    firstChar = expr[j];
                    followChar = expr[j + 1];
                }
                first = false;
                j++;
            }
            //get next two char as long as neither are '|'
            else
            {
                firstChar = expr[j];
                followChar = expr[j + 1];
                while (firstChar == '|' || followChar == '|' || isTerminal(firstChar) == true)
                {
                    firstChar = expr[j];
                    if (firstChar == '\0')
                        continue;
                    followChar = expr[j + 1];
                    j++;
                }

                j++;
            }

            if (firstChar == '\0' && followChar == '\0')
                continue;

            //if first char is Non Terminal and there is no other char, add follow of lhs
            //to follow of first char
            if (isTerminal(firstChar) == false && followChar == '\0')
            {
                set<char> builder = followMap[lhs];
                set<char> builder2 = followMap[firstChar];
                for (auto k = builder.begin(); k != builder.end(); k++)
                {
                    bool found = followMap[firstChar].find(*k) != followMap[firstChar].end();
                    if (!found && *k != 'e')
                    {
                        followMap[firstChar].insert(*k);
                        changes = true;
                    }
                }
            }

            //if first char is nonTerminal and second is terminal, add second value to follow
            //of first Char, except for 'e'
            if (isTerminal(firstChar) == false && isTerminal(followChar) == true && followChar != '\0')
            {
                if (followChar != 'e')
                {
                    bool found = followMap[firstChar].find(followChar) != followMap[firstChar].end();
                    if (!found)
                    {
                        followMap[firstChar].insert(followChar);
                        changes = true;
                    }
                }
            }

            //if both are non Terminals, add the first of the second char to the follow of the
            //first char
            if (isTerminal(firstChar) == false && isTerminal(followChar) == false && followChar != '\0')
            {
                set<char> builder = followMap[firstChar];
                set<char> builder2 = firstMap[followChar];
                set<char> builder3 = followMap[lhs];
                bool epsilon = false;
                for (auto k = builder2.begin(); k != builder2.end(); k++)
                {
                    if (*k == 'e')
                        epsilon = true;
                    if (*k != 'e')
                    {
                        bool found = followMap[firstChar].find(*k) != followMap[firstChar].end();
                        if (!found)
                        {
                            followMap[firstChar].insert(*k);
                            changes = true;
                        }
                    }
                }

                //if first of second char had epsilon, then add follow of lhs to follow of
                //first char
                if (epsilon)
                {
                    for (auto k = builder3.begin(); k != builder3.end(); k++)
                    {
                        if (*k != 'e')
                        {
                            bool found = followMap[firstChar].find(*k) != followMap[firstChar].end();
                            if (!found)
                            {
                                followMap[firstChar].insert(*k);
                                changes = true;
                            }
                        }
                    }
                }
            }

            //loop unitl there has been no changes
            if (i == nonTerminal.size() - 1 && changes == false)
                complete = true;
        }
    }

}

/* void add_closure
 * If 'next' is the current input symbol and next is nonterminal, then the set
 * of LR(0) items reachable from here on next includes all LR(0) items reachable
 * from here on FIRST(next). Add all grammar productions with a lhs of next */
void add_closure(char lookahead, LR0Item& item, AugmentedGrammar& grammar)
{
    // only continue if lookahead is a non-terminal
    if (!isupper(lookahead))
        return;

    string lhs = string(&lookahead, 1);

    // iterate over each grammar production beginning with p->rhs[next]
    // to see if that production has already been included in this item.
    for (int i = 0; i < grammar[lookahead].size(); i++)
    {
        string rhs = "." + grammar[lookahead][i];
        // if the grammar production for the next input symbol does not yet
        // exist for this item, add it to the item's set of productions
        if (!item.Contains(lhs + "->" + rhs))
            item.Push(new AugmentedProduction(lookahead, rhs));
    }
}

// produce the graph of LR(0) items from the given augmented grammar
void getLR0Items(vector<LR0Item>& lr0items, AugmentedGrammar& grammar, int& itemid, gotoMap& globalGoto)
{
    printf("I%d:\n", itemid);

    // ensure that the current item contains te full closure of it's productions
    for (int i = 0; i < lr0items[itemid].Size(); i++)
    {
        string rhs = lr0items[itemid][i]->rhs;
        char lookahead = rhs[rhs.find('.') + 1];
        add_closure(lookahead, lr0items[itemid], grammar);
    }

    int nextPos;
    char lookahead;
    char lhs;
    string rhs;
    AugmentedProduction *prod;

    // iterate over each production in this LR(0) item
    for (int i = 0; i < lr0items[itemid].Size(); i++)
    {
        lhs = lr0items[itemid][i]->lhs;
        rhs = lr0items[itemid][i]->rhs;
        string production = string(&lhs, 1) + "->" + rhs;

        // get lookahead if one exists
        lookahead = rhs[rhs.find('.') + 1];
        if (lookahead == '\0')
        {
            printf("\t%-20s\n", &production[0]);
            continue;
        }

        // if there is no goto defined for the current input symbol from this
        // item, assign one.
        if (lr0items[itemid].gotos.find(lookahead) == lr0items[itemid].gotos.end())
        {
            // that one instead of creating a new one
            // if there is a global goto defined for the entire production, use
            if (globalGoto.find(production) == globalGoto.end())
            {
                lr0items.push_back(LR0Item());
                // new right-hand-side is identical with '.' moved one space to the right
                string newRhs = rhs;
                int atpos = newRhs.find('.');
                swap(newRhs[atpos], newRhs[atpos + 1]);
                // add item and update gotos
                lr0items.back().Push(new AugmentedProduction(lhs, newRhs));
                lr0items[itemid].gotos[lookahead] = lr0items.size() - 1;
                globalGoto[production] = lr0items.size() - 1;
            }
            else
            {
                // use existing global item
                lr0items[itemid].gotos[lookahead] = globalGoto[production];
            }
            printf("\t%-20s goto(I%d,%c)\n", &production[0], globalGoto[production], lookahead);
        }
        else
        {
            // there is a goto defined, add the current production to it
            // move . one space to right for new rhs
            int at = rhs.find('.');
            swap(rhs[at], rhs[at + 1]);
            // add production to next item if it doesn't already contain it
            int nextItem = lr0items[itemid].gotos[lookahead];
            if (!lr0items[nextItem].Contains(string(&lhs, 1) + "->" + rhs))
                lr0items[nextItem].Push(new AugmentedProduction(lhs, rhs));
            swap(rhs[at], rhs[at + 1]);
            printf("\t%-20s\n", &production[0]);
        }
    }
}

//This function uses the canonical LR(0) sets and the follow sets to create the action/goto tables
//for the given grammar. The end of the function also handles printing the tables. If there are any
//shift reduce or reduce-reduce conficts, the table with display more than one action in its table.
//This would mean that the language is not a valid SLR grammar
void actionGoto(vector<LR0Item>& lr0items, AugmentedGrammar& grammar, int& itemid, gotoMap& globalGoto)
{
    //maps for both grammar, action and goto table
    map <string, int> augGrammar;
    map < string, map <string, string > > action;
    map < string, map <string, string > > gotoTable;

    char lookahead;
    int state = 0;

    //sets a map for which state a follows goes to if the handle has reached the end
    for (int i = 0; i < lr0items[0].Size(); i++)
    {
        string lhs = "";
        lhs.push_back(lr0items[0][i]->lhs);
        string temp = lhs + "->" + lr0items[0][i]->rhs;
        temp.erase(remove(temp.begin(), temp.end(), '.'), temp.end());
        augGrammar[temp] = i;
    }

    //Creating the header for the action table
    for (int i = 0; i < terminal.size(); i++)
    {
        string temp = "";
        temp.push_back(terminal[i]);
        action["0State"][temp] = temp;

    }

    //Creating the header for the goto table
    for (int i = 0; i < nonTerminal.size(); i++)
    {

        string temp = "";
        temp.push_back(nonTerminal[i]);
        gotoTable["0State"][temp] = temp;
    }

    //while state is less then all LR items
    while (state < itemid)
    {
        //Set all items in this row to " ". Will make it easier for printing table
        for (int i = 0; i < terminal.size(); i++)
        {
            string firstmap;
            string temp = "";
            if (state >= 10)
                firstmap = "I-" + to_string(state);
            else
                firstmap = "I- " + to_string(state);
            temp.push_back(terminal[i]);
            action[firstmap][temp] = " ";

        }

        //Set all items in this row to " ".Will make it easier for printing table
        for (int i = 0; i < nonTerminal.size(); i++)
        {
            string firstmap;
            if (state >= 10)
                firstmap = "I-" + to_string(state);
            else
                firstmap = "I- " + to_string(state);
            string temp = "";
            temp.push_back(nonTerminal[i]);
            gotoTable[firstmap][temp] = " ";
        }

        //Loop through each production the given LR state
        for (int i = 0; i < lr0items[state].Size(); i++)
        {
            char lhs = lr0items[state][i]->lhs;
            string rhs = lr0items[state][i]->rhs;
            string production = "";
            production.push_back(lhs);
            char lookahead = rhs[rhs.find('.') + 1];

            //if the dot is at end of the handle
            if (lookahead == '\0')
            {
                lookahead = rhs[rhs.find('.') - 1];

                //iterate through the follows of lhs. For each non terminal in follows, reduce and goto
                //the given state. If reduce goes to state 1, accept.
                if (isTerminal(lookahead))
                {
                    string firstmap;
                    for (auto it = followMap[lhs].begin(); it != followMap[lhs].end(); it++)
                    {
                        string temp = "";
                        temp.push_back(lhs);
                        temp += "->" + rhs;
                        temp.erase(remove(temp.begin(), temp.end(), '.'), temp.end());
                        int reduceto = augGrammar[temp];
                        if (reduceto == 0)
                            continue;
                        if (state >= 10)
                            firstmap = "I-" + to_string(state);
                        else
                            firstmap = "I- " + to_string(state);
                        string second = "";
                        second.push_back(*it);
                        if(reduceto == 0)                        
                            action[firstmap][second] += "acc";
                        else
                            action[firstmap][second] += "r" + to_string(reduceto);
                    }

                }
                //iterate through the follows of lhs. For each non terminal in follows, reduce and goto
                //the given state. If reduce goes to state 1, accept.
                else
                {

                    string firstmap;
                    for (auto it = followMap[lhs].begin(); it != followMap[lhs].end(); it++)
                    {
                        string temp = "";
                        temp.push_back(lhs);
                        temp += "->" + rhs;
                        temp.erase(remove(temp.begin(), temp.end(), '.'), temp.end());
                        int reduceto = augGrammar[temp];
                        if (reduceto == 0)
                            continue;
                        if (state >= 10)
                            firstmap = "I-" + to_string(state);
                        else
                            firstmap = "I- " + to_string(state);
                        string second = "";
                        second.push_back(*it);
                        if (reduceto == 1)
                            action[firstmap][second] += "acc";
                        else
                            action[firstmap][second] += "r" + to_string(reduceto);
                    }
                }
            }

            //if not at end of handle. If the value is non terminal shift and go to given state
            if (isTerminal(lookahead))
            {
                string firstmap;
                int stateto = globalGoto[production + "->" + rhs];
                if (stateto == 0)
                    continue;
                if (state >= 10)
                    firstmap = "I-" + to_string(state);
                else
                    firstmap = "I- " + to_string(state);
                string second = "";
                second.push_back(lookahead);
                action[firstmap][second] += "s" + to_string(stateto);
            }
            //if not at end of handle. If the value is terminal set goto table and goto given state
            else
            {
                string firstmap;
                int stateto = globalGoto[production + "->" + rhs];
                if (stateto == 0)
                    continue;
                if (state >= 10)
                    firstmap = "I-" + to_string(state);
                else
                    firstmap = "I- " + to_string(state);
                string second = "";
                second.push_back(lookahead);
                gotoTable[firstmap][second] += to_string(stateto);
            }


        }
        //increase to next state
        state++;

    }

    //header for action/goto table
    cout << "\t\t\t\t" << "ACTION" << "\t\t\t\t" << "GOTO" << endl;
    auto it3 = gotoTable.begin();

    //iterate through both action and goto map 
    for (auto it = action.begin(); it != action.end(); it++)
    {
        cout << "\n" << it->first << "\t";
        {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                cout << " | " << it2->second << "\t";
            }
            for (auto it4 = it3->second.begin(); it4 != it3->second.end(); it4++)
            {
                cout << " || " << it4->second << "\t";
            }
        }
        if(it3 != gotoTable.end())
            it3++;
    }
    cout << endl;
}

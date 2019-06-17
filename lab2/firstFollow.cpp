/*******************************************************************
 * Name         : Eric Blasko
 * Lab          : Lab 2
 * Date         : 2/4/2019
 * Title        : Grammar First/Follow
 * Description  : This program reads in a grammar and parses the 
 *                grammar to break it up into terminals, non terminals
 *                and the non terminal productions. There are two 
 *                functions that then find the given grammars first
 *                and follow sets. If the grammar is not condesned it
 *                will be condesned in this program. The grammar, first
 *                set and follow set are printed to console.
 ********************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

using namespace std;

//vectors for terminals and non terminals. productions, firstMap and
//final map use nonTerminal as its key.
string file = "grammar.txt";
vector<char> terminal;
vector<char> nonTerminal;
map<char, string> production;
map<char, set<char> > firstMap;
map<char, set<char> > followMap;

//Forward declarations
bool firstProduction = true;
void printAll();
bool isTerminal(char);
void first();
void follow();
void printGrammar();

//Main will open and read in grammar, and push them to proper vector.
//Once complete first and followed are called and everything is printed.
int main()
{
    //opening stream
    string line;
    fstream input;
    input.open(file.c_str(), ios::in);
    if (!input)
        cerr << "error opening grammar.txt" << endl;

    //read line by line
    getline(input, line);
    while (!input.eof())
    {
        //check if first value is lower case or other terminal char and push
        //to vector
        char val = line[0];
        if (islower(val) || val == '+' || val == '*' || val == '(' || val == ')')
            terminal.push_back(val);

        //check if first value is upper case and push to vector
        if (isupper(val))
        {
            //push only once to nonTerminal vector. If not empty make sure duplicates
            //are not added
            if (nonTerminal.empty())
                nonTerminal.push_back(val);
            else if (!(find(nonTerminal.begin(), nonTerminal.end(), val) != nonTerminal.end()))
                nonTerminal.push_back(val);
            
            //if production is empty add current production. Else add to the current
            //productions
            if (production[val] == "")
                production[val] = line.substr(3, line.length());
            else
                production[val] += "|" + line.substr(3, line.length());
        }

        getline(input, line);       
    }

    //close file
    input.close();

    //print grammar, get first set and follow set and print all sets
    printGrammar();
    first();
    follow();
    printAll();
}

//Prints the grammar. Will print each terminal follow by non terminals and
//their productions 
void printGrammar()
{
    cout << "Grammar" << endl;
    for (int i = 0; i < terminal.size(); i++)
        cout << terminal[i] << endl;

    cout << "$" << endl;

    for (int i = 0; i < nonTerminal.size(); i++)
        cout << nonTerminal[i] << "->" << production[nonTerminal[i]] << endl;

    cout << "$" << endl;
}

//Prints the first and follow sets for the given grammar. Each element in the
//set is seperated by a comma
void printAll()
{
    cout << endl;
    
    //print first set
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
    bool changes = false;
    int i = 0;

    //loop until there has been no changes and i == 0 again
    while (!complete)
    {
        //get nonTerminal
        lhs = nonTerminal[i++%nonTerminal.size()];
        i = i % nonTerminal.size();
        if(i == 0)
            changes = false;

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
        if (i == nonTerminal.size()-1 && changes == false)
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

    //loop until no changes have been made for entire iteration of the 
    //non Terminals
    while (!complete)
    {
        //get a non Terminal
        lhs = nonTerminal[i++%nonTerminal.size()];
        i = i % nonTerminal.size();
        if (i == 0)
            changes = false;

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
                while (firstChar == '|' || followChar == '|' && !isTerminal(firstChar))
                {
                    firstChar = expr[j];
                    followChar = expr[j + 1];
                    j++;
                }

                j++;
            }

            //if first char is Non Terminal and there is no other char, add follow of lhs
            //to follow of first char
            if (isTerminal(firstChar) == false and followChar == '\0')
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
            if (isTerminal(firstChar) == false and isTerminal(followChar) == true)
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
            if (isTerminal(firstChar) == false and isTerminal(followChar) == false)
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
    //insert "$" to the follow set of the firs non terminal
    followMap[nonTerminal[0]].insert('$');
}

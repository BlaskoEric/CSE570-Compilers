//Name          : Eric Blasko
//Lab           : Lab 1
//Date          : January 22 2019
//Title         : dfa.cpp
//Description   : This program takes in a string from the user and determines
//                if the entire string or any substrings are found withing
//                any of the given languages. The symbols that are used for 
//                both languages consist of a and b. Extra credit was complete


#include <iostream>
#include <string>
#include <vector>

using namespace std;

//These enum values are used for ease of reading and writing the program
//STATE0 = 0, STATE1 = 1, STATE2 = 2 FINAL_STATE = 4. 
typedef enum {
    STATE0 = 0,
    STATE1,
    STATE2,
    FINAL_STATE} States;

//2D array that represents the state transition table for the DFA of (a|b)*abb
const States DFA_1[4][2] = {
    //'a'           'b'
    {STATE1,        STATE0},
    {STATE1,        STATE2},
    {STATE1,        FINAL_STATE},
    {STATE1,        STATE0}
};

//2D array that represents the state transition table for the DFA of (a|b)*baa
const States DFA_2[4][2] = {
    //'a'           'b'
    {STATE0,        STATE1},
    {STATE2,        STATE1},
    {FINAL_STATE,   STATE1},
    {STATE0,        STATE1}
};

//Iterate through string from users input. Both DFA's start at state0 and based
//on current char at position of string both DFA's move to the approperiate
//state. Their are two vectors for both DFA's that record all valid cases where
//dfa accepts a string
void isLanguage(string& input)
{
    int state1 = STATE0;
    int state2 = STATE0;

    bool invalidChar = false;
    vector<int> state1StartPos;
    vector<int> state2StartPos;
	
	vector<string> state1string;
	vector<string> state2string;

    for(int i = 0; i < input.length(); i++)
    {
        switch(input[i])
        {
            //for case a and b, first the state of the dfa is changed, a new position
            //to there vectors is added, and each vector is concatinated with a or b.
            //if the current state == final state, all vector are printed to give 
            //all substrings
            case 'a':
                state1 = DFA_1[state1][0];                              //change state
                state1string.push_back("");                             //add to vector
                state1StartPos.push_back(i+1);                          //add to vector
                for(int i = 0; i < state1string.size();i++)
                    state1string[i] += "a";                             //concatinate to each string in vector

                state2 = DFA_2[state2][0];                              //same as above
                state2string.push_back("");
                state2StartPos.push_back(i+1);
                for(int i = 0; i < state2string.size();i++)
                    state2string[i] += "a";
    
                if(state1 == FINAL_STATE)                               //print all vectors if final state
                {
                    for(int j = 0; j < state1string.size() - 2;j++)
                    printf("%d-%d %s is in L1\n ",state1StartPos[j],i+1,state1string[j].c_str());
                } 
                if(state2 == FINAL_STATE)                               //same as above
                {
                    for(int j = 0; j < state2string.size() - 2;j++)
                    printf("%d-%d %s is in L2\n ",state2StartPos[j],i+1,state2string[j].c_str());
                }
                    continue;

            //follows same as above
           case 'b':
                state1 = DFA_1[state1][1];
                state1string.push_back("");
                state1StartPos.push_back(i+1);
                for(int i = 0; i < state1string.size();i++)
                    state1string[i] += "b";

                state2 = DFA_2[state2][1];
                state2string.push_back("");
                state2StartPos.push_back(i+1);
                for(int i = 0; i < state2string.size();i++)
                    state2string[i] += "b";

                if(state1 == FINAL_STATE)
                {
                    for(int j = 0; j < state1string.size() - 2;j++)
                        printf("%d-%d %s is in L1\n ",state1StartPos[j],i+1,state1string[j].c_str()); 
                }
                if(state2 == FINAL_STATE)
                {
                    for(int j = 0; j < state2string.size() - 2;j++)
                        printf("%d-%d %s is in L2\n ",state2StartPos[j],i+1,state2string[j].c_str());
                }
                continue;

            default:					                            //Invalid characters - clear all vectors
                invalidChar = true;
		        
                state1 = STATE0;                                    //return to state0
                state1StartPos.clear();                             //clear vector
                state1string.clear();                               //clear vector
		        
                state2 = STATE0;                                    //same as above
                state2StartPos.clear();
                state2string.clear();
                continue;
        }
    }

	//print if entire string is of language 1
    if(state1 == FINAL_STATE && invalidChar == false)
        printf("\"%s\" is in L1 \n\n",input.c_str());

    //print if entire string is of language 2
    else if (state2 == FINAL_STATE && invalidChar == false)
        printf("\"%s\" is in L2 \n\n",input.c_str());

    //print if entire string is of neither language 1 or 2   
    if(state1 != FINAL_STATE && state2 != FINAL_STATE || invalidChar == true)
        printf("\"%s\" is not in any language \n\n",input.c_str());

};

//This function clears the terminal and display's user prompt
void setPrompt()
{
    printf("\033[2J");                  //clear terminal
    printf("\033[H");                   //return cursor to top
    printf("Sigma = {a,b}*\n");        // '\u03a3' is unicode for epsilon
    printf("L1 = (a|b)*abb \n");
    printf("L2 = (a|b)*baa \n");   
    printf("Enter a string consisting of \u03a3 to see if it is in L1 or L2 \n");
    printf("Enter 'exit' to quit \n\n"); 
}

//Main function that calls setPrompt(), gets user input and checks if isLanguage();
//loops unit user types 'exit'.
int main()
{
    string input;

    setPrompt();

    getline(cin,input);

    while(input != "exit")
    {
        printf("%s\n",input.c_str());
        isLanguage(input);
            
        getline(cin,input);
    }
}

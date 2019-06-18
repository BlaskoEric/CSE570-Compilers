# CSE570-Compilers
CSE570 Compilers with Dr. Gomez

# Labs

### Lab 1
* This program takes in a string from the user and determines
if the entire string or any substrings are found withing
any of the given languages. The symbols that are used for 
both languages consist of a and b. Extra credit was complete

### Lab 2
* This program reads in a grammar and parses the 
grammar to break it up into terminals, non terminals
and the non terminal productions. There are two 
functions that then find the given grammars first
and follow sets. If the grammar is not condesned it
will be condesned in this program. The grammar, first
set and follow set are printed to console.

### Lab 3
* This program reads in a grammar and parses the
grammar to produce the first, follows, and LR(0)
sets. The LR(0) sets are saved as objects from the
LR0Item class. Each object contains a vector of
productions at that state along with gotos on given
inputs. As the program finds the cannonical LR(0)
sets, they are printed as soon as the are created.

### Lab 4
* This program reads in a grammar and parses the
grammar to produce the first, follows, and LR(0)
sets. The LR(0) sets are saved as objects from the
LR0Item class. Each object contains a vector of
productions at that state along with gotos on given
inputs. As the program finds the cannonical LR(0)
sets, they are printed as soon as the are created.

### Lab 5
* This program reads in a grammar and parses the
grammar to produce the first, follows, and LR(0)
sets. Once these sets have been obtained, we uses
the LR(0) sets and follow sets to produce a action
goto table. From this table we can easily determine
if a grammar is SLR or not. If there is a shift-
shirt conflict or a shift-reduce conflict then it
is not SLR

# Project (Lex/yacc)
* A simple calculator using both lex and yacc. This calculator 
can store values under any single character variable and can
perform most basic calculations such as sin, cos, ceiling, floor,
powers and square roots. 

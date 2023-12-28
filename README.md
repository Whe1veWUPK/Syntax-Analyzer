# Syntax-Analyzer
This Project is the SEU CSE's Dr. Qi's Compile principle class's Lab 2(Syntax Analyzer).

## Analysis Range
* The analyzer uses the **top-down** method to analyze.
* So the grammar to be analyzed must be  ```LL(1)``` grammar.
* The non-terminal are between ```'A'``` and ```'Z'```.
* And there must be a start symbol ```'S'```.

## Other
* The analyzer has been set a ```panic model```.
* The ```synchronize set``` is set the follow set that is not in the predict table.
* The production's format should be ```X -> xxx``` (Notice there are ```spaces``` between the leftpart of the production and ```->```, between ```->``` and rightpart of production).
* Compile the ```main.cpp``` and then run it to analyze the grammar.
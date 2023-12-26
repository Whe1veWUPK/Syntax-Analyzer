#ifndef PREDICTTABLE_HPP
#define PREDICTTABLE_HPP
class PredictTable{
private:
    std::unordered_set<char> terminal;// the terminal
    std::unordered_set<char> nonTerminal; // the nonterminal
    std::unordered_map<std::string, int> item; //the table's item
public:
    void addTerminal(char ch); //add terminal
    void addNonTerminal(char ch); //add nonterminal
    void addItem(char nonTerminal,char terminal, int index); // add the table item

    bool isInTerminalSet(char ch);// determine whether the character is in the terminal set
    bool isInNonTerminal(char ch);// determine whether the character is in the non-terminal set
    bool isAnItemInTable(char nonTerminal,char terminal); // determine whether the item is in the table

    void traverseTerminal();//traverse the terminal set
    void traverseNonTerminal();// traverse the non-terminal set
    void traversePredictTable(); // traverse the predict table

    
    std::unordered_set<char> getTerminalSet(); // get terminal set
    std::unordered_set<char> getNonTerminalSet(); //get Non-terminal set

};

#endif
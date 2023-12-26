#include<iostream>
#include<unordered_map>
#include<unordered_set>

#include "PredictTable.hpp"


//add terminal
void PredictTable::addTerminal(char ch){
    std::unordered_set<char>::iterator iter;
    iter = this->terminal.find(ch);
    if(iter==this->terminal.end()){
        this->terminal.insert(ch);
    }

}

//add nonterminal
void PredictTable::addNonTerminal(char ch){
    std::unordered_set<char>::iterator iter;
    iter = this->nonTerminal.find(ch);
    if(iter==this->nonTerminal.end()){
        this->nonTerminal.insert(ch);
    }
}

//add item
void PredictTable::addItem(char nonTerminal,char terminal,int index){
    std::unordered_map<std::string, int>::iterator iter;
    std::string temp;
    temp.push_back(nonTerminal);
    temp.push_back(terminal);
    iter = this->item.find(temp);
    if(iter==this->item.end()){
        this->item.insert(std::make_pair(temp,index));
    }
}


// determine whether the character is in the terminal set
bool PredictTable::isInTerminalSet(char ch){
    std::unordered_set<char>::iterator iter;
    iter = this->terminal.find(ch);
    if(iter==this->terminal.end()){
        return false;
    }
    else{
        return true;
    }
}

// determine whether the character is in the nonterminal set
bool PredictTable::isInNonTerminal(char ch){
    std::unordered_set<char>::iterator iter;
    iter = this->nonTerminal.find(ch);
    if(iter==this->nonTerminal.end()){
        return false;
    }
    else{
        return true;
    }
}

// determine whether the item is in the table
bool PredictTable::isAnItemInTable(char nonTerminal,char terminal){
    std::unordered_map<std::string, int>::iterator iter;
    std::string temp;
    temp.push_back(nonTerminal);
    temp.push_back(terminal);
    iter = this->item.find(temp);
    if(iter==this->item.end()){
        return false;
    }
    else{
        return true;
    }
}

//traverse the terminal set
void PredictTable::traverseTerminal(){
    std::unordered_set<char>::iterator iter;
    for (iter = this->terminal.begin(); iter != this->terminal.end();++iter){
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

//traverse the non-terminal set
void PredictTable::traverseNonTerminal(){
    std::unordered_set<char>::iterator iter;
    for (iter = this->nonTerminal.begin(); iter != this->nonTerminal.end();++iter){
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

// traverse the predict table
void PredictTable::traversePredictTable(){
    for (auto iter = this->item.begin(); iter != this->item.end();++iter){
        std::cout << " ( " << iter->first[0] << " , " << iter->first[1] << " , " << iter->second <<" ) "<< "\n";
    }
}


// get terminal set
std::unordered_set<char> PredictTable::getTerminalSet(){
    return this->terminal;
}

// get non-terminal set
std::unordered_set<char> PredictTable::getNonTerminalSet(){
    return this->nonTerminal;
}
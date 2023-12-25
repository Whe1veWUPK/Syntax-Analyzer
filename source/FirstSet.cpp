#include<iostream>
#include<unordered_set>

#include "FirstSet.hpp"


//default constructor
FirstSet::FirstSet(){
    this->index = -1;
    this->isOver = false;
}

//constructor
FirstSet::FirstSet(int index){
    this->setIndex(index);
    this->isOver = false;
}

//is InFirst Set
bool FirstSet::isInFirstSet(char ch){
    auto findResult=firstSet.find(ch);
    if(findResult==firstSet.end()){
        return false;
    }
    else{
        return true;
    }
}

//set function of index
void FirstSet::setIndex(int index){
    this->index = index;
}

//get function of index
int FirstSet::getIndex(){
    return this->index;
}

//whether the set has been calculated
bool FirstSet::isFinished(){
    return this->isOver;
}

//add character into the first set
void FirstSet::addIntoFirstSet(char ch){
    auto findResult = firstSet.find(ch);
    if(findResult==firstSet.end()){
        this->firstSet.insert(ch);
    }      
    else{
        return;
    }
}
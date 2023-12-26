#include<iostream>
#include<unordered_set>

#include "FollowSet.hpp"

//default constructor
FollowSet::FollowSet(){
    setIndex(-1);
    setFlag(false);
}

//constructor
FollowSet::FollowSet(int index){
    setIndex(index);
    setFlag(false);
}

//whether it is in the follow set
bool FollowSet::isInFollowSet(char ch){
    std::unordered_set<char>::iterator iter;
    iter = this->followSet.find(ch);
    if(iter!=this->followSet.end()){
        return true;
    }
    else{
        return false;
    }
}

// set function of index
void FollowSet::setIndex(int index){
    this->index = index;
}

// get function of index
int FollowSet::getIndex(){
    return this->index;
}

// add character into the follow set
void FollowSet::addIntoFollowSet(char ch){
    if(!isInFollowSet(ch)){
        this->followSet.insert(ch);
    }
}

// traverse the follow set
void FollowSet::traverseSet(){
    std::unordered_set<char>::iterator iter;
    for (iter = this->followSet.begin(); iter != this->followSet.end();++iter){
        std::cout << *iter << " ";
    }
    std::cout << "\n";
}

// set flag
void FollowSet::setFlag(bool flag){
    this->isOver = flag;
}

// whether the follow set has been calculated over
bool FollowSet::isFinished(){
    return this->isOver;
}

// get follow set
std::unordered_set<char> FollowSet::getFollowSet(){
    return this->followSet;
}

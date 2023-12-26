#ifndef FOLLOWSET_HPP
#define FOLLOWSET_HPP
class FollowSet{
private:
    int index; //the index of the nonterminal
    std::unordered_set<char> followSet;// follow set of the nonterminal
    bool isOver; // whether the follow set has been calculated over

public:
    FollowSet(); //default constructor
    FollowSet(int index); // constructor
    bool isInFollowSet(char ch); //whether it is in the follow set

    void setIndex(int index); // set function of index
    int getIndex(); // get function of index

    void addIntoFollowSet(char ch);// add character into the follow set
    void traverseSet(); // traverse the follow set

    void setFlag(bool flag); //set the flag
    bool isFinished(); // whether the follow set has been calculated

    std::unordered_set<char> getFollowSet(); //get follow set
};


#endif
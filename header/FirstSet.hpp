#ifndef FIRSTSET_HPP
#define FIRSTSET_HPP
class FirstSet{
private:
    int index;// the index of production
    std::unordered_set<char> firstSet;
    bool isOver;// whether the set has been calculated
public:
    FirstSet();//default constructor
    FirstSet(int index); // constructor
    bool isInFirstSet(char ch);//whether it is in the first Set

    void setIndex(int index); // set function of index
    int getIndex(); //get function of index

    bool isFinished(); // whether the first set has been calculated

    void addIntoFirstSet(char ch);// add character into the first set
};

#endif
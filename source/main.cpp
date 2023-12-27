#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <string>

#include "Production.hpp"
#include "FirstSet.hpp"
#include "FollowSet.hpp"
#include "PredictTable.hpp"

int numberOfNonterminal = 0;
std::vector<std::string> originalProduction;  // store the productions
std::unordered_map<std::string, int> hashMap; //hash map from nonterminal to memory's index
std::vector<std::vector<int>> nonTerminalToIndex; //the nonterminal's index

// get the num of the Productions
int getNumOfProductions(std::fstream*ifs){
    int number = 0;
    std::string temp;
    while(getline(*ifs, temp)){
        originalProduction.push_back(temp);
        ++number;
    }
    return number;
}

//process the productions
void processProductions(Production*productions){

    std::string leftPart;
    std::string rightPart;
    std::string temp;
    for (int i = 0; i < originalProduction.size(); ++i){
        
        std::istringstream iss(originalProduction[i]);
        iss >> leftPart;
        iss >> temp;
        iss >> rightPart;

        productions[i].setLeftPart(leftPart);
        productions[i].setRightPart(rightPart);
    }

}

// create the hash from nonterminal to hash table's index
void createTHash(Production*productions,int number){
    std::unordered_map<std::string, int>::iterator iter;
    for (int i = 0; i < number; ++i){
        iter = hashMap.find(productions[i].getLeftPart());
        if(iter==hashMap.end()){
            //the hash map does not has this nonterminal
            std::string leftPart = productions[i].getLeftPart();
            hashMap.insert(std::make_pair(leftPart, numberOfNonterminal));
            ++numberOfNonterminal;
            

            //initial the vector and push this first production's index into the vector
            std::vector<int> temp;
            temp.push_back(i);
            nonTerminalToIndex.push_back(temp);
        }
        else{
            // the hash map has the this nonterminal
            nonTerminalToIndex[iter->second].push_back(i);
        }
    }
}


//calculate concrete production's first set
void calFirstSet(FirstSet*firstSet,Production*production,int index){
    if(firstSet[index].isFinished()){
        //this production's first set has been calculated
        return;
    }
    std::string rightPart=production[index].getRightPart(); // the right part of production
    int length = rightPart.size();
    if(rightPart[0]<'A'||rightPart[0]>'Z'){
        // the rightPart's first character is a terminal, then this production's first set has calculated over
        firstSet[index].addIntoFirstSet(rightPart[0]);
        firstSet[index].setOverFlag(true);
    }
    else{
        for (int j = 0; j < length;++j){
            if(rightPart[j]<'A'||rightPart[j]>'Z'){
                firstSet[index].addIntoFirstSet(rightPart[j]);
                firstSet[index].setOverFlag(true);
                break;
            }
            
            //find the production
            std::unordered_map<std::string, int>::iterator iter;
            std::string temp;
            temp.push_back(rightPart[j]);
            iter = hashMap.find(temp);
            int num = iter->second;  
            

            //calculate the first set
            bool hasEmpty = false;
            for (int z = 0; z < nonTerminalToIndex[num].size(); ++z){
                calFirstSet(firstSet, production, nonTerminalToIndex[num][z]);
                if(firstSet[nonTerminalToIndex[num][z]].isInFirstSet('#')){
                    hasEmpty = true;
                }
            }

            //merge the set
            std::unordered_set<char> tempSet;
            for (int z = 0; z < nonTerminalToIndex[num].size();++z){
                std::unordered_set<char>z1 = firstSet[nonTerminalToIndex[num][z]].getFirstSet();
                std::unordered_set<char>::iterator iter;
                for(iter=z1.begin();iter!=z1.end();++iter){
                    tempSet.insert(*iter);
                }
            }

            if(!hasEmpty){
               //over
               std::unordered_set<char>::iterator iter;
               for (iter = tempSet.begin(); iter != tempSet.end();++iter){
                   firstSet[index].addIntoFirstSet(*iter);
               }
               firstSet[index].setOverFlag(true);
               break;
            }
            else{
                //not over
                if(j!=length-1){
                    //is not the last nonterminal
                    std::unordered_set<char>::iterator iter;
                    for (iter = tempSet.begin(); iter != tempSet.end();++iter){

                        //insert all characters except the '#'
                        if((*iter)!='#'){
                            firstSet[index].addIntoFirstSet(*iter);
                        }
                    }
                }
                else{
                    std::unordered_set<char>::iterator iter;
                    for (iter = tempSet.begin(); iter != tempSet.end();++iter){
                        firstSet[index].addIntoFirstSet(*iter);
                    }

                    firstSet[index].setOverFlag(true);
                    break;
                }
            }
        }
        return;
    }
}

//calculate the first set of each production
void calAllFirstSet(FirstSet*firstSet,Production*productions,int number){
    std::string rightPart;  //string that store the right part of the production
    for (int i = 0; i < number; ++i){
        
        calFirstSet(firstSet, productions, i);
    }
}

// calculate the non-terminal's follow set
void calFollowSet(FollowSet*followSet,FirstSet*firstSet,Production*productions,int number,int index,std::string nonTerminal){
    //std::cout << "Now is cal " << nonTerminal << "\n";
    // if the follow set has been calculated over

    if (followSet[index].isFinished())
    {
        return;
    }
    for (int i = 0; i < number;++i){
        std::string rightPart = productions[i].getRightPart();
        std::string leftPart = productions[i].getLeftPart();
        //std::cout << "Cur Production is " << i << "\n";
        for (int c = 0; c < rightPart.size(); ++c)
        {
            std::string temp;
            temp.push_back(rightPart[c]);
            if (temp == nonTerminal){
                //std::cout << "In " << i << " has the " << nonTerminal << "\n";
                if (c == rightPart.size() - 1)
                {
                    //if this nonTerminal is the end of this production
                    auto iter = hashMap.find(leftPart);
                    // calculate the leftPart's non-terminal's follow set (if needed)
                    if(leftPart!=nonTerminal){
                     calFollowSet(followSet, firstSet,productions, number, iter->second, iter->first);

                     // merge the leftPart's follow set into the nonTerminal's follow set
                     std::unordered_set<char> tempSet = followSet[iter->second].getFollowSet();

                     auto iterT = tempSet.begin();
                     for (iterT; iterT != tempSet.end();++iterT){
                         //std::cout << nonTerminal << " add " << iter->first << "'s follow set's " << *iterT<<"\n";
                         followSet[index].addIntoFollowSet(*iterT);
                     }
                    }
                    else{
                        break;
                    }
                }
                else{
                    if(rightPart[c+1]<'A'||rightPart[c+1]>'Z'){
                        //the follow of this non-terminal is a terminal
                        //std::cout << nonTerminal << " add " << " terrminal "<< rightPart[c+1]<<"\n";
                        followSet[index].addIntoFollowSet(rightPart[c + 1]);
                        break;
                    }
                    else{
                        //the follow of this non-terminal is a non-terminal
                        for (int i1 = c + 1; i1 < rightPart.size();++i1){
                            if(rightPart[i1]>='A'&&rightPart[i1]<='Z'){
                                std::string temp;
                                temp.push_back(rightPart[i1]);
                                auto findRt = hashMap.find(temp);
                                bool hasEmpty = false;
                                int indexOfMemory = findRt->second;
                                //std::cout << nonTerminal << " merge " << findRt->first << "'s first set.\n";
                                // merge the first set
                                for (int i2 = 0; i2 < nonTerminalToIndex[indexOfMemory].size();++i2){
                                    int indexOfProductions = nonTerminalToIndex[indexOfMemory][i2];
                                    std::unordered_set<char> tempSet = firstSet[indexOfProductions].getFirstSet();
                                    for (auto i3 = tempSet.begin(); i3 != tempSet.end();++i3){
                                        if(*i3!='#'){
                                            // not eps
                                            //std::cout << nonTerminal << " add " << findRt->first << "'s first set's " << *i3 << "\n";
                                            followSet[index].addIntoFollowSet(*i3);
                                        }
                                        else{
                                            hasEmpty = true;
                                        }
                                    }
                                }
                                if(!hasEmpty){
                                    // the merge set is over
                                    break;
                                }
                                else if(hasEmpty&&(i1==rightPart.size()-1)){
                                    // merge the leftPart's follow set into the non-terminal's follow set
                                    auto p = hashMap.find(leftPart);
                                    int indexOfLeft = p->second;
                                    if(leftPart!=nonTerminal){
                                      calFollowSet(followSet, firstSet, productions, number, indexOfLeft, leftPart);
                                      std::unordered_set<char> tempSet = followSet[indexOfLeft].getFollowSet();

                                      for (auto iter = tempSet.begin(); iter != tempSet.end();++iter){
                                          //std::cout << nonTerminal << " add " << leftPart << " 's follow set's " << *iter << "\n";
                                          followSet[index].addIntoFollowSet(*iter);
                                      }
                                    }
                                    else{
                                        break;
                                    }
                                }
                            }
                            else{
                                //terminal
                                //std::cout << nonTerminal << " add " <<  " terminal " << rightPart[i1] << "\n";     
                                followSet[index].addIntoFollowSet(rightPart[i1]);
                                break;
                            }
                        }
                        break;
                    }
                    
                }
            }
        }
    }
    // the follow set has been calculated over
    followSet[index].setFlag(true);

}

// calculate all non-terminal's follow set
void calAllFollowSet(FollowSet*followSet,FirstSet*firstSet,Production*productions,int number){
    auto s = hashMap.find("S");
    if(s!=hashMap.end()){

        //1. add '$' to Follow(S)
        int sIndex = s->second;
        followSet[sIndex].addIntoFollowSet('$');
        //std::cout << "Initial S's follow set: ";

        //std::cout << "The hash table's size is: " << hashMap.size() << "\n";
        // 2. calculate all non-terminal's follow set
        for (auto nonTerminal = hashMap.begin(); nonTerminal!=hashMap.end();++nonTerminal){
            //std::cout << "Now is cal " << nonTerminal->first << "\n";
            //
            std::string nonT = nonTerminal->first;

            calFollowSet(followSet, firstSet,productions,number,nonTerminal->second,nonT);
            
        }
    }
    else{
        std::cerr << "Error: No Grammar Start Sysmbol.\n";
        exit(0);
    }
}

//create all predict table's item
void createPredictTable(FirstSet*firstSet,FollowSet*followSet,PredictTable*predictTable,Production*productions,int number){
    std::unordered_set<char> terminalSet = predictTable->getTerminalSet();
    for (int i = 0; i < number; ++i){
        std::string leftPart = productions[i].getLeftPart();
        std::string rightPart = productions[i].getRightPart();
        bool hasEmpty = firstSet[i].isInFirstSet('#');
        
        for (auto iter = terminalSet.begin(); iter != terminalSet.end(); ++iter){
            if(firstSet[i].isInFirstSet(*iter)&&(*iter!='#')){
                //if it is in the first set
                predictTable->addItem(leftPart[0], *iter, i);
            }
        }
        if(hasEmpty){
            auto findResult = hashMap.find(leftPart);
            int memoryIndex = findResult->second;
            std::unordered_set<char> tempSet = followSet[memoryIndex].getFollowSet();
            for (auto iter = tempSet.begin(); iter != tempSet.end();++iter){
                predictTable->addItem(leftPart[0], *iter, i);
            }
        }
    } 
}

void startAnalyze(PredictTable*predictTable,Production*productions){
    std::cout << "Please input the string to be analyze:\n";
    std::string str;
    std::cin >> str;
    std::stack<char> analyzeStack;
    str.push_back('$');

    std::cout << "Matched\tStack\tInput\tAction\n";

    // the curlocation of the input string
    int ip = 0;

    // initial stack
    analyzeStack.push('$');
    analyzeStack.push('S');
    std::string stackQueue;
    
    stackQueue.insert(0,1,'S');
    stackQueue.push_back('$');

    std::string matched;
    
    char X = analyzeStack.top();
    while(X!='$'){
        bool findResult = predictTable->isAnItemInTable(X, str[ip]);
        if (X == str[ip]){
            analyzeStack.pop();
            matched.push_back(str[ip]);
            ++ip;

            //update remained
            std::string temp;
            temp = str.substr(ip);
            // update matched

            
            //update stack queue
            stackQueue = stackQueue.substr(1);
            // output
            std::cout << matched << "\t" << stackQueue << "\t"<<temp<< "\tMatch " <<X<< "\n";
        }
        else if(X<'A'||X>'Z'){
            std::cerr << "Analyze error: the top character is a terminal.\n";
        }
        else if(!findResult){
            std::cerr << "Analyze error: the item is empty.\n";
        }
        else{
            int index = predictTable->getIndex(X, str[ip]);
            std::string temp;
            temp = str.substr(ip);

            analyzeStack.pop();
            stackQueue = stackQueue.substr(1);
            std::string rightPart = productions[index].getRightPart();
            for (int i = rightPart.size() - 1; i >= 0;--i){
                if(rightPart[i]!='#'){
                    // the character is not eps
                    analyzeStack.push(rightPart[i]);
                    stackQueue.insert(0, 1,rightPart[i]);
                }
            }
            std::cout << matched << "\t" << stackQueue << "\t"<<temp<< "\tOutput " << productions[index].getLeftPart() << " -> " << productions[index].getRightPart() << "\n";
        }
        X = analyzeStack.top();
    }
}

int main(){

    std::cout << "Please input the file path that stored the LL(1) grammar."<<"\n";
    std::string inputFile;
    
    std::cin >> inputFile;

    std::fstream ifs;
    ifs.open(inputFile, std::ios::in);
    
    //if the input file can not be opened
    if(!ifs.is_open()){
        std::cerr << "The file can not be opened.\n";
        return -1;
    }

    originalProduction.clear();
    hashMap.clear();
    nonTerminalToIndex.clear();
    numberOfNonterminal = 0;

    //  get the num of productions
    int number = getNumOfProductions(&ifs);

    Production *productions = new Production[number];

    // process the productions
    processProductions(productions);
    
    
    //create hash from nonterminal to index of productions
    createTHash(productions, number);



    // initial first set
    FirstSet *firstSet = new FirstSet[number];

    for (int i = 0; i < number;++i){
        firstSet[i].setIndex(i);
    }
    
    //calculate the first set
    calAllFirstSet(firstSet, productions, number);

    std::cout << "Each Production's first set is as follows: \n";
    for (int i = 0; i < number; ++i){
        std::cout << i << " :  ";
        firstSet[i].traverseSet();
    }

    PredictTable predictTable;
    // initial the terminal set and the non-terminal set
    for (int i = 0; i < number;++i){
        std::string leftPart = productions[i].getLeftPart();
        std::string rightPart = productions[i].getRightPart();
        for (int l = 0; l < leftPart.size();++l){
            if(leftPart[l]<'A'||leftPart[l]>'Z'){
                predictTable.addTerminal(leftPart[l]);
            }
            else{
                predictTable.addNonTerminal(leftPart[l]);
            }
        }
        for (int r = 0; r < rightPart.size();++r){
            if(rightPart[r]<'A'||rightPart[r]>'Z'){
                predictTable.addTerminal(rightPart[r]);
            }
            else{
                predictTable.addNonTerminal(rightPart[r]);
            }
        }
    }

    std::cout << "The terminal set: \n";
    predictTable.traverseTerminal();

    std::cout << "The non-terminal set: \n";
    predictTable.traverseNonTerminal();
    
    
    // initial follow set
    FollowSet *followSet = new FollowSet[numberOfNonterminal];
    for (int i = 0; i < numberOfNonterminal;++i){
        followSet[i].setIndex(i);
    }

    calAllFollowSet(followSet,firstSet, productions,number);

    std::cout << "The followings are follow set of the non-terminals.\n";
    // traverse the follow set
    for (int i = 0; i < numberOfNonterminal;++i){
        
        followSet[i].traverseSet();
    }

    // create the table item
    createPredictTable(firstSet, followSet, &predictTable, productions, number);

    

    //start analyze
    char flag = 'Y';
    while(flag=='Y'){
        startAnalyze(&predictTable, productions);
        std::cout << "Do you want to Continue? Yes(Y) or No(N)\n";
        std::cin >> flag;
    }

    ifs.close();
    return 0;
}
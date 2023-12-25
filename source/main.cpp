#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "Production.hpp"

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

    for (int i = 0; i < numberOfNonterminal;++i){
        for (int j = 0; j < nonTerminalToIndex[i].size();++j){
            std::cout << nonTerminalToIndex[i][j] << " ";
        }
        std::cout << "\n";
    }
    // close the file
    ifs.close();
    return 0;
}
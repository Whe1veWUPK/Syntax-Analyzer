#include <iostream>
#include "Production.hpp"

//default constructor
Production::Production(){
    
}


//the constructor of Production
Production::Production(std::string leftPart, std::string rightPart){
    setLeftPart(leftPart);
    setRightPart(rightPart);
}

//set function of leftPart
void Production::setLeftPart(std::string leftPart){
    this->leftPart = leftPart;
    return;
}

//set function of rightPart
void Production::setRightPart(std::string rightPart){
    this->rightPart = rightPart;
    return;
}

//get function of leftPart
std::string Production::getLeftPart(){
    return this->leftPart;
}

//get function of rightPart
std::string Production::getRightPart(){
    return this->rightPart;
}
#ifndef  PRODUCTION_HPP
#define  PRODUCTION_HPP

class Production{
private:
    std::string leftPart;
    std::string rightPart;
public:
    Production();//default constructor
    Production(std::string leftPart, std::string RightPart); //Constructor
    void setLeftPart(std::string leftPart); //set function of leftPart
    void setRightPart(std::string rightPart);//set function of rightPart
    std::string getLeftPart();//get function of leftPart
    std::string getRightPart();//get function of rightPart
};

#endif
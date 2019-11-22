#ifndef THINK_H
#define THINK_H

#include <iostream>
#include <vector>
#include <string>
#include <QString>

extern bool databaseMade;

class Think
{
private :

    std::string cakeType;
    std::string deliveryDate;
    std::string deliveryTime;
    std::string deliveryAddress;
    std::string paymentType;
    std::string flavour;
    std::string fruit;
    std::string topping;
    std::string name;
    std::string emailID;
    std::string mobileNumber;
    std::string candleType;
    std::string isEggPartDone;
    std::string textOnCake;

    QString ID;

    bool isFruit;
    bool isEggCake;
    bool isNameOnCake;
    bool isCandleOnCake;
    bool isHeartShappedCake;

    int age;
    int cakeWeight;
    int numberOfCandles;

    float price;

public:

    int botStart = 0;

    Think()
    {
        cakeType = deliveryDate = deliveryTime = deliveryAddress = paymentType = flavour = fruit = topping = name  = emailID = mobileNumber = candleType = textOnCake = "";
        isFruit = isEggCake = isNameOnCake = isCandleOnCake = isHeartShappedCake = false;
        age = -1 ; price = 0 ; numberOfCandles = 0;
        cakeWeight = 1000;
    }

    int randVal(int MAX_VAL);
    std::string getPhrase(std::string type , int index);
    bool checkEmailID(std::string emailID);
    std::string response(std::string s);
    bool detectName(std::string s);
    QString writeUserData();
    std::string getStartingQuestionStatus(int& index);
    std::string getBirthdayQuestionStatus(int& index);
    std::string getWedQuestionStatus(int& index);
    std::string getEndingQuestionStatus(int& index);
    bool detectCakeType(std::string s);
    bool detectEggOrEggless(std::string s);
    bool detectFlavour(std::string s);
    bool detectTopping(std::string s);
    bool detectCandleType(std::string s);
    bool detectNumberOfCandles(std::string s);
    bool findWord(std::string s , std::vector<std::string> pat);
    void calcPrice();
    bool detectTextOnCake(std::string s);
    bool detectCakeWeight(std::string s);
    bool detectMobileNumber(std::string s);
    bool detectPaymentType(std::string s);
    bool detectFruit(std::string s);
    bool isItLeapYear(int day);
    bool checkDateTime(std::string s);
    std::string makeSorryRepeatQuestion(std::string questionType , int& Index , std::string stage);
    std::string getEmailId();
    QString getID();
    QString getHTML();

};

#endif // THINK_H

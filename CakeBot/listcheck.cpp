#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <QDebug>

#include "listcheck.h"

using namespace std;

int toppingCheck(string s)
{
    ifstream file;
    file.open("topping.csv");
    string toppingPresent;
    string toppingAmount;

    while(file.good())
    {
        getline(file , toppingPresent , ',');
        getline(file , toppingAmount);
        if(toppingPresent == s)
        {
            int intAmount = stoi(toppingAmount);
            file.close();
            return intAmount;
        }
    }
    file.close();
    return -1;
}

int fruitCheck(string s)
{
    ifstream file;
    file.open("fruit.csv");
    string fruitPresent;
    string fruitAmount;

    while(file.good())
    {
        getline(file , fruitPresent , ',');
        getline(file , fruitAmount);
        if(fruitPresent == s)
        {
            int intAmount = stoi(fruitAmount);
            file.close();
            return intAmount;
        }
    }
    file.close();
    return -1;
}

int flavourCheck(string s)
{
    ifstream file;
    file.open("flavour.csv");
    string flavourPresent;
    string flavourAmount;

    while(file.good())
    {
        getline(file , flavourPresent , ',');
        getline(file , flavourAmount);
        if(flavourPresent == s)
        {
            int intAmount = stoi(flavourAmount);
            file.close();
            return intAmount;
        }
    }
    file.close();
    return -1;
}

int candleCheck(string s)
{
    ifstream file;
    file.open("candle.csv");
    string candlePresent;
    string candleAmount;

    while(file.good())
    {
        getline(file , candlePresent , ',');
        getline(file , candleAmount);
        if(candlePresent == s)
        {
            int intAmount = stoi(candleAmount);
            file.close();
            return intAmount;
        }
    }
    file.close();
    return -1;
}

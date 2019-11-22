#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <regex>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QDate>
#include <QDir>

#include "listcheck.h"
#include "database.h"
#include "think.h"

using namespace std;

#define MAX_HAPPY_PHRASE          5
#define MAX_NOT_FOUND_PHRASE      3
#define MAX_START_Q               5
#define MAX_BDAY_Q                4
#define MAX_WED_Q                 5
#define MAX_END_Q                 7

int Year();
QString Month();
QString Weekday();
int Day();
int Hour();
int Minute();
int Second();

QString formatTwoDigit(int x)
{
    QString q;
    if (!(x/10)) {
        q = "0" + QString::number(x);
    }
    else {
        q = QString::number(x);
    }
    return q;
}

QString Think::getID()
{
    /*********** ID FORMAT **********/
    /*
     *   ID = day + month + hour + minute + second
     *
     *   example :- if date is 9th september 2019 & time is 7:30:00 am , then ID = 0909073000  { 09+09+07+30+00 }
    */

    QString d = formatTwoDigit(QDate::currentDate().day());
    QString m = formatTwoDigit(QDate::currentDate().month());
    QString m2 = formatTwoDigit(QTime::currentTime().minute());
    QString s = formatTwoDigit(QTime::currentTime().second());
    QString h = formatTwoDigit(QTime::currentTime().hour());

    QString id = d + m + h + m2 + s;
    this->ID = id;

    return id;
}

string Think::getStartingQuestionStatus(int& index)
{
    switch(index) {
        case 1:
            ++index;
            return ("What type of cake do you want - birthday , wedding/anniversary ceremony or just for casual purpose ? ");

        case 2:
            ++index;
            return ("So do you want an eggless cake ? ");

        case 3:
            ++index;
            return ("What flavour of cake do you want ?\nWe have these flavour available :-\n\"Black Forest\" , \"Butter Scotch\" , \"Chocolate\" , \"Rainbow\" , \"Coffee\" , \"Vanilla\" , \"Fruits\" ");

        case 4:
              ++index;
              return ("Do you want any topping of your choice on the cake ?\nWe have got \"Nut & Raisins\" , \"Choco Chips\" , \"Strawberry\" , \"Cherry\" and \"Gems\" ");

    }
    return "";
}

string Think::getBirthdayQuestionStatus(int& index)
{
    switch(index) {
        case 1:
            ++index;
            return ("Do you want name to be written on the cake ?\nIf yes , please write the name like this -> happy birthday xyz ");

        case 2:
            ++index;
            return ("Do you want candles on the cake ? And if yes which one - we have got both fancy numbered candles and colorful stick candles ");

        case 3:
            ++index;
            if (this->candleType == "numbered") {
                return ("For which age do you want the candle ?");
            }
            return ("How many candles do you wish to purchase ? ");

    }
    return "";
}

string Think::getWedQuestionStatus(int& index)
{
    switch(index) {
        case 1:
            ++index;
            return ("Do you want a heart shapped cake ? ");

        case 2:
            ++index;
            return ("Do you want any text to be written on the cake ? If yes please write the text inside bracket (eg. [text here] ");

        case 3:
            ++index;
            return ("Do you want candles on the cake ? And if yes which one - we have got both fancy numbered candles and colorful stick candles ");

        case 4:
            ++index;
            if (this->candleType == "numbered") {
                return ("What number on candle do you want ?");
            }
            return ("How many candles do you wish to purchase ? ");

    }
    return "";
}

string Think::getEndingQuestionStatus(int& index)
{
    switch(index) {
        case 1:
            ++index;
            return ("How much weight of cake do you wish to purchase (in gram) ? ");

        case 2:
            ++index;
            return ("On which date and time do you want your cake to be delivered (write in dd-mm,hour(am/pm) time format , eg. 02-12,6pm) ? ");

        case 3:
            ++index;
            return ("Could you provide your billing address ? ");

        case 4:
            ++index;
            return ("What mode of payment do you wish to continue - we offer both online payment and cash/card-on-delivery ");

        case 5:
            ++index;
            return ("Could you share your email-id for sending confirmation mail ");

        case 6:
            ++index;
            return ("Could you please share your mobile number ");

    }
    return "";
}

QString Think::writeUserData()
{

    QString directory_name = formatTwoDigit(QDate::currentDate().day()) + "-" +
            formatTwoDigit(QDate::currentDate().month());

    QDir create_dir(directory_name);

    if (!create_dir.exists()) {
        create_dir.mkpath(".");
    }

    ofstream userData;
    // QDate d = QDate::currentDate();
    // QTime t = QTime::currentTime();
    QString id = this->getID();
    string fileName = directory_name.toStdString() + '/' + id.toStdString() + ".txt";


    userData.open(fileName, ios::app | ios::out);
    userData << getHTML().toStdString();
    userData.close();


    /*********** THIS METHOD OF SAVING DATA RAW IN .TXT FORMAT IS NOW DEPRECATED ***
     *********** THE NEW METHOD IS STORING IN HTML FORMAT DIRECTLY *****************

    userData.open(fileName, ios::app | ios::out);
    userData << " ID : " << id.toStdString() << " \n\n";
    userData << " Order date : " << d.toString("dd.MMM.yyyy").toStdString() << "\n\n";
    userData << " Order time : " << t.toString("hh.mm.ss.AP").toStdString() << "\n\n";
    userData << " Cake type : " << this->cakeType << " \n\n";
    userData << " Cake flavour : " << this->flavour << " \n\n";
    if (this->isFruit) {
        userData << " Fruit type : " << this->fruit << " \n\n";
    }
    userData << " Eggless cake : " << (this->isEggCake ? "No " : "Yes ") << "\n\n";
    if (!(this->isFruit)) {
        userData << " Cake topping : " << this->topping << " \n\n";
    }
    userData << " Total price : ₹ " << static_cast<int>(ceil(this->price)) << " \n\n";
    userData << " Payment type : " << this->paymentType << " \n\n";
    if (this->isHeartShappedCake) {
        userData << " Cake Shape : Heart \n\n";
    }
    if (this->isNameOnCake) {
        userData << " Name on cake : " << this->name << " \n\n";
    }
    if (this->textOnCake != "") {
        userData << " Text on cake : " << this->textOnCake << " \n\n";
    }
    if (this->isCandleOnCake) {
        userData << " Type of candle on cake : " << this->candleType << " \n\n";
        if (this->candleType == "numbered") {
            userData << " Number on candle : " << this->age << "\n\n";
        }
        else {
            userData << " Number of candles : " << this->numberOfCandles << " \n\n";
        }
    }
    userData << " Cake weight : " << this->cakeWeight << "grams \n\n";
    userData << " Delivery date : " << this->deliveryDate << " \n\n";
    userData << " Delivery time : " << this->deliveryTime << " \n\n";
    userData << " Delivery address : " << this->deliveryAddress<< " \n\n";
    userData << " Mobile number : " << this->mobileNumber << " \n\n";
    userData << " Email ID : " << this->emailID << "\n\n";
    userData.close();

    *******************************************************************************/

    ofstream storePath;
    storePath.open(QDir::currentPath().toStdString() + "/email_sender/userdata.txt", ios::app | ios::out);
    storePath << QDir::currentPath().toStdString() + '/' + directory_name.toStdString() + '/' + id.toStdString() + ".txt";
    storePath.close();

    ofstream storeEmail;
    storeEmail.open(QDir::currentPath().toStdString() + "/email_sender/useremail.txt", ios::app | ios::out);
    storeEmail << (this->emailID);
    storeEmail.close();

    ofstream cancelOrder;
    cancelOrder.open(QDir::currentPath().toStdString() + "/email_sender/cancel_order.txt", ios::app | ios::out);
    cancelOrder.close();

    return id;
}

QString addRow(QString col1 , QString col2 , bool makeBox)
{
    static int idx = 1;
    QString row;
    if (idx%2 == 0) {
        row = "<tr><td style = \"padding: 8px;\"><strong>";
    }
    else {
        row = "<tr style = \"background-color: #f2f2f2;\"><td style = \"padding: 8px;\"><strong>";
    }
    row += col1;
    row += "</strong></td><td>";
    if (makeBox) {
        row += "<span style=\"background-color: #4485b8; color: #fff; padding: 2px 8px; font-weight: bold; border-radius: 5px;\">";
    }
    row += col2;
    if (makeBox) {
        row += "</span>";
    }
    row += "</td></tr>";
    idx++;
    return row;
}

QString convertToQString(string s)
{
    return QString::fromUtf8(s.data() , static_cast<int>(s.size()));
}

QString Think::getHTML()
{
    QString start, end, data = "";
    start = "<div style=\"text-align: center;\">"\
            "<h1 style=\"color: #4485b8;\">Cake Order Receipt</h1>"\
            "</div>"\
            "<div>"\
            "<table style=\"vertical-align: middle; horizontal-align: center; width: 100%;\">"\
            "<tbody>";

    end = "</tbody>"\
           " </table>"\
           " </div>"\
           " <br>"\
           " <div><hr /></div>"\
           " <div>"\
           " <p><em>Generated by <strong>Cake Bot</strong> , developed by Kaustav Bhattacharjee , 2nd Sem CSE BTech IIIT-G</em></p>"\
           " </div>";

    data += start;

    data += addRow(" ID " , this->ID , true);
    data += addRow(" Order date " , QDate::currentDate().toString("dd.MMM.yyyy") , false);
    data += addRow(" Order time " , QTime::currentTime().toString("hh.mm.ss.AP") , false);
    data += addRow(" Cake type ", convertToQString(this->cakeType) , false);
    data += addRow(" Cake flavour ", convertToQString(this->flavour) , false);

    if (this->isFruit) {
        data += addRow("Fruit type : " , convertToQString(this->fruit) , false);
    }

    data += addRow(" Eggless cake ", (this->isEggCake ? "No" : "Yes") , false);

    if (!(this->isFruit)) {
        data += addRow(" Cake topping ", convertToQString(this->topping) , false);
    }

    data += addRow(" Total price ", "&#x20b9; " + QString::number(static_cast<int>(ceil(this->price))) , false);
    data += addRow(" Payment type ", convertToQString(this->paymentType) , false);

    if (this->isHeartShappedCake) {
        data += addRow(" Cake shape ", " Heart " , false);
    }

    if (this->isNameOnCake) {
        data += addRow(" Name on cake ", convertToQString(this->name) , false);
    }

    if (this->textOnCake != "") {
        data += addRow(" Text on cake ", convertToQString(this->textOnCake) , false);
    }

    if (this->isCandleOnCake) {
        data += addRow(" Type of candle on cake ", convertToQString(this->candleType) , false);
        if (this->candleType == "numbered") {
            data += addRow(" Number on candle : " , QString::number(this->age) , false);
        }
        else {
            data += addRow(" Number of candles ", QString::number(this->numberOfCandles) , false);
        }
    }

    data += addRow(" Cake weight ", QString::number(this->cakeWeight) + " grams " , false);
    data += addRow(" Delivery date ", convertToQString(this->deliveryDate) , false);
    data += addRow(" Delivery time ", convertToQString(this->deliveryTime) , false);
    data += addRow(" Delivery address ", convertToQString(this->deliveryAddress) , false);
    data += addRow(" Mobile number ", convertToQString(this->mobileNumber) , false);
    data += addRow(" Email ID ", convertToQString(this->emailID) , false);

    data += end;
    return data;
}

string Think::getEmailId()
{
    return (this->emailID);
}

string Think::getPhrase(string type , int index)
{
    string* phrase;

    string happy[MAX_HAPPY_PHRASE] , notFound[MAX_NOT_FOUND_PHRASE], notUnderstand[3];

    happy[0] = "Oh so sweet ! ";
    happy[1] = "Oh that's great ! ";
    happy[2] = "Oh that's good to hear ! ";
    happy[3] = "Fantastic ! ";
    happy[4] = "I am happy to hear that ! ";

    notFound[0] = "Sorry, but I am afraid that we don't have that. ";
    notFound[1] = "Oops , we don't have that. ";
    notFound[2] = "Sorry, but unfortunately we don't have that. ";

    notUnderstand[0] = "Sorry, but could you please repeat ";
    notUnderstand[1] = "Please, do you mind repeating ";
    notUnderstand[2] = "I am afraid but I couldn't get it. Would you mind repeating ";


    if (type == "happy") phrase = happy;
    else if (type == "notUnderstand") phrase = notUnderstand;
    else phrase = notFound;

    return phrase[index];
}


int Think::randVal(int MAX_VAL)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int index = abs(rand() % MAX_VAL);
    return index;
}

bool Think::checkEmailID(string emailID)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (regex_match(emailID,pattern)) { this->emailID = emailID; return true; }
    return false;
}

bool Think::findWord(string s , vector<string> pat)
{
    for (auto i=0 ; i<static_cast<int>(pat.size()) ; i++) {
        if (s.find(pat[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

bool Think::detectCakeType(string s)
{
    // for birthday cake
    vector<string> bday { "bir","bd" };
    if (findWord(s,bday) == true) {
        this->cakeType = "Birthday";
        return true;
    }

    // for wedding/anniversary cake
    vector<string> wed { "wed","anni","anny" };
    if (findWord(s,wed) == true) {
        this->cakeType = "Wedding/Anniversary";
        return true;
    }

    // for casual cake
    vector<string> normal { "casu","norm" };
    if (findWord(s,normal) == true) {
        this->cakeType = "Casual";
        return true;
    }
    return false;
}

bool Think::detectEggOrEggless(string s)
{
    vector<string> withEgg { "with","no","nah","dont","meh","egg" };
    vector<string> withoutEgg { "without","yes","less","sur","ok","mayb" };
    if (findWord(s,withEgg)) {
        this->isEggCake = true;
        this->isEggPartDone = "yes";
        return true;
    }
    if (findWord(s,withoutEgg)) {
        this->isEggCake = false;
        this->isEggPartDone = "yes";
        return true;
    }
    this->isEggPartDone = "no";
    return false;
}

bool Think::detectFlavour(string s)
{
    vector<string> rainbow { "rain","raim","ran" };
    vector<string> chocolate { "choco","coco" };
    vector<string> black_forest { "black","blac","blak","fores" };
    vector<string> butter_scotch { "butt","scot" };
    vector<string> coffee { "cof" };
    vector<string> vanilla { "vani" };
    vector<string> fruit {"fru" , "mix"};

    if (findWord(s,rainbow)) { this->flavour = "Rainbow"; return true; }
    if (findWord(s,chocolate)) { this->flavour = "Chocolate"; return true; }
    if (findWord(s,black_forest)) { this->flavour = "Black Forest"; return true; }
    if (findWord(s,butter_scotch)) { this->flavour = "Butter Scotch"; return true; }
    if (findWord(s,coffee)) { this->flavour = "Coffee"; return true; }
    if (findWord(s,vanilla))  { this->flavour = "Vanilla"; return true; }
    if (findWord(s,fruit)) { this->flavour = "Fruit"; this->isFruit = true; return true; }
    return false;
}

bool Think::detectFruit(string s)
{
    vector<string> mix { "mix" };
    vector<string> dry { "dry" };

    if (findWord(s,mix)) { this->fruit = "Mix Fruits"; return true; }
    if (findWord(s,dry)) { this->fruit = "Dry Fruits"; return true; }
    return false;
}

bool Think::detectTopping(string s)
{
    vector<string> nut { "nut","cashew","kism","rai" };
    vector<string> choco { "choco","coco","chip","wafe" };
    vector<string> straw { "stra","berr" };
    vector<string> cherry { "cherr" };
    vector<string> gems { "gem","gen","color" };

    if (findWord(s,nut)) { this->topping = "Nut & Raisins"; return true; }
    if (findWord(s,choco)) { this->topping = "Choco Chips"; return true; }
    if (findWord(s,straw)) { this->topping = "Strawberry"; return true; }
    if (findWord(s,cherry)) { this->topping = "Cherry"; return true; }
    if (findWord(s,gems)) { this->topping = "Gems"; return true; }
    return false;
}

void Think::calcPrice()
{
    // taking cake's base price to be 200 per 1000 g

    float price = flavourCheck(this->flavour) + toppingCheck(this->topping);
    if (this->isEggCake) {
        price += 60;
    }
    if (this->isCandleOnCake) {
        if (this->candleType == "stick") {
            price += (this->numberOfCandles)*(candleCheck("stick"));
        }
        else{
            price += (this->numberOfCandles)*(candleCheck("numbered"));
        }
    }
    price += 200; // cake's base price is here
    price = (price*(this->cakeWeight))/1000;
    if (this->fruit != "") {
        price += fruitCheck(this->fruit);
    }
    this->price = price;
}

bool Think::detectName(string s)
{
    size_t pos = s.find("day") , pos1 = s.find("dy");
    if (pos == string::npos) {
        if (pos1 == string::npos) {
              pos = s.find(' ');
              string c = s.substr(0,pos);
              this->name = c;
              return true;
        }
        pos = pos1 + 2;
    }
    else{
        pos += 3;
    }
    while (s[pos] == ' ' || s[pos] == ',' || s[pos] == ':' || s[pos] == '-' || s[pos] == '[') {
        pos++;
    }
    string t = s.substr(pos);
    if (t.size() == 0) return false;
    size_t blank = t.find(" ");
    t = t.substr(0,blank);
    this->name = t;
    return true;
}

bool Think::detectCandleType(string s)
{
    vector<string> stick {"sti","colo","penc"};
    vector<string> numbered {"num","fanc","age"};

    if(findWord(s,stick)) {this->candleType = "stick"; return true;}
    if(findWord(s,numbered)) {this->candleType = "numbered"; return true;}

    return false;
}

bool Think::detectNumberOfCandles(string s)
{
    stringstream ss;
    ss << s;
    string temp = "";
    int number;
    while(!ss.eof()){
        ss >> temp;
        if (stringstream(temp) >> number) { this->numberOfCandles = number; return true; }
        temp = "";
    }
    return false;
}

bool Think::detectTextOnCake(string s)
{
    size_t start = s.find("[");
    if (start == string::npos) return false;
    string c = s.substr(start+1);
    start = c.find("]");
    c = c.substr(0,start);
    if (c.size() == 0) return false;
    this->textOnCake = c;
    return true;
}

bool Think::detectCakeWeight(string s)
{
    stringstream ss;
    ss << s;
    string temp = "";
    int number = 0 , n , digits = 0;
    while (!ss.eof()) {
        ss >> temp;
        if (stringstream(temp) >> n) {
            number = number*10 + n;
            digits++;
        }
        temp = "";
    }
    if (digits == 0) return false;
    this->cakeWeight = number;
    return true;
}

bool Think::isItLeapYear(int day){
    QDate d;
    int year = d.year();
    return ( (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) ? day==29 : day==28 );
}

bool Think::checkDateTime(string s){

    // date
    int day=0,month=0;
    try{
        day = stoi(s.substr(0,s.find('-')));
    }
    catch(...){
        return false;
    }
    try{
        month = stoi(s.substr(s.find('-')+1,s.find(',')));
    }
    catch(...){
        return false;
    }

    if(month<1 || month>12) return false;
    if(day<1) return false;

    int mon = (month/8 ? (1 + month%8) : month);
    if(month==2 && !isItLeapYear(day)) return false;
    else if(mon%2 && day>31) return false;
    else if(!(mon%2) && day>30) return false;


    // time
    string t = s.substr(s.find(',')+1);
    string period = "";
    if(t.find("am") != string::npos) {
        t = t.substr(0,t.find("am"));
        period = "am";
    }
    else if(t.find("pm") != string::npos) {
        t = t.substr(0,t.find("pm"));
        period = "pm";
    }
    else return false;

    int time;
    try{
        time = stoi(t);
    }
    catch(...){
        return false;
    }

    string full_date = (QDate(2019,month,day).toString("dd.MMM")).toStdString();
    transform(period.begin(), period.end(), period.begin(), ::toupper);
    string full_time = to_string(time) + "." + period;

    this->deliveryDate = full_date;
    this->deliveryTime = full_time;

    return true;
}

bool Think::detectPaymentType(string s)
{
    vector<string> online { "onl","net" };
    vector<string> cod { "cod","offli","cas","card" };
    if (findWord(s,online)) { this->paymentType = "Online"; return true; }
    if (findWord(s,cod)) { this->paymentType = "Cash/Card-on-delivery"; return true; }
    return false;
}

bool Think::detectMobileNumber(string s)
{
    if(s.length() != 10) return false;
    else if(s[0] == '0') return false;
    for(size_t i=0; i<s.length(); i++) {
        int num = s[i]-'0';
        if (num<0 || num>9) {
            return false;
        }
    }
    this->mobileNumber = s;
    return true;
}

string Think::makeSorryRepeatQuestion(string questionType , int& Index , string stage)
{
    string send;
    if (stage == "start") {
        send = getStartingQuestionStatus(--Index);
    }
    else if (stage == "birthday") {
        send = getBirthdayQuestionStatus(--Index);
    }
    else if (stage == "wed") {
        send = getWedQuestionStatus(--Index);
    }
    else if (stage == "end") {
        send = getEndingQuestionStatus(--Index);
    }
    char c = send[0] + 32;
    send = c + send.substr(1);
    send = getPhrase(questionType , randVal(3)) + send;
    return send;
}

string Think::response(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    static int startIndex = 1;
    static int midIndex = 1;
    static int endIndex = 1;
    static int stages = 0;
    static bool analyse_start = false;
    static bool analyse_mid = false;
    static bool analyse_end = false;

    // ANALYSE USER STRING "S"

    // 1. ANALYSE THE STARTING PART OF QUESTIONAIRE

    if (startIndex <= MAX_START_Q && analyse_start) {
        switch (startIndex-1) {
            case 1 :
                if (!detectCakeType(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , startIndex , "start");
                }
                break;

            case 2 :
                if (!detectEggOrEggless(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , startIndex , "start");
                }
                break;

            case 3 :
                if (!detectFlavour(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , startIndex , "start");
                }
                break;

            case 4 :
                if (this->isFruit) {
                    if (!detectFruit(s)) {
                        return ("Sorry but would you mind repeating what type of fruits cake do you want :- \"mix fruits\" , \"dry fruits\" ");
                    }
                }
                else if (!detectTopping(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , startIndex , "start");
                }
                stages++;
                break;
        }
        analyse_start = false;
    }

    // 2. ANALYSE THE TYPE_OF_CAKE QUESTIONAIRE

    // 2.(a) BIRTHDAY TYPE

    vector<string> no {"no","na","meh","don"};
    vector<string> yes {"ye","yu","ok","sur"};

    if (this->cakeType == "Birthday" && midIndex <= MAX_BDAY_Q && analyse_mid) {
        switch (midIndex-1) {
            case 1 : {
                if (findWord(s , no)) {
                    if (findWord(s , yes)) {
                        return "Please type either yes or no";
                    }
                    this->isNameOnCake = false;
                    break;
                }
                vector<string> bday {"bir","day","dy"};
                if (findWord(s,yes) || findWord(s,bday)) {
                    this->isNameOnCake = true;
                    if (!detectName(s)) {
                        if (this->name == "") {
                            return makeSorryRepeatQuestion("notUnderstand" , midIndex , "birthday");
                        }
                    }
                }
            }
            break;

            case 2 : {
                if (findWord(s , no)) {
                    if (findWord(s , yes)) {
                        return "Please type either yes or no";
                    }
                    this->isCandleOnCake = false;
                    midIndex++;
                    stages++;
                    break;
                }
                this->isCandleOnCake = true;
                if (!detectCandleType(s)) {
                    if (this->candleType == "") {
                        return makeSorryRepeatQuestion("notUnderstand" , midIndex , "birthday");
                    }
                }
            }
            break;

            case 3 :
                if (this->candleType == "numbered") {
                    int age = stoi(s);
                    if (age <= 0 || age >= 150) {
                        --midIndex;
                        return "Please enter a valid age between 1 to 150";
                    }
                    else {
                        this->age = age;
                    }
                }
                else if (this->isCandleOnCake && !detectNumberOfCandles(s)) {
                    if (this->numberOfCandles == 0) {
                        return makeSorryRepeatQuestion("notUnderstand" , midIndex , "birthday");
                    }
                }
                stages++;
            break;

        }
        analyse_mid = false;
    }

    // 2.(b) WEDDING/ANNIVERSARY TYPE

    else if(this->cakeType == "Wedding/Anniversary" && startIndex <= MAX_WED_Q && analyse_mid){
        switch(midIndex-1){
            case 1 : {
                if (findWord(s , yes)) {
                    if (findWord(s , no)) {
                        return "Please type either yes or no";
                    }
                    this->isHeartShappedCake = true;
                }
            }
            break;

            case 2 : {
                if (findWord(s , yes)) {
                    if (findWord(s , no)) {
                        return "Please type either yes or no";
                    }
                    if (!detectTextOnCake(s)) {
                        return makeSorryRepeatQuestion("notUnderstand" , midIndex , "wed");
                    }
                }
            }
            break;

            case 3 : {
                if (findWord(s , no)) {
                    if (findWord(s , yes)) {
                        return "Please type either yes or no";
                    }
                    this->isCandleOnCake = false;
                    midIndex++;
                    stages++;
                    break;
                }
                this->isCandleOnCake = true;
                if (!detectCandleType(s)) {
                    if (this->candleType == "") {
                        return makeSorryRepeatQuestion("notUnderstand" , midIndex , "wed");
                    }
                }
            }
            break;

            case 4 :
                if (this->candleType == "numbered") {
                    int age = stoi(s);
                    if (age <= 0 || age >= 150) {
                        --midIndex;
                        return "Please enter a number between 1 to 150";
                    }
                    else {
                        this->age = age;
                    }
                }
                else if (!detectNumberOfCandles(s)) {
                    if (this->isCandleOnCake && this->numberOfCandles == 0) {
                        return makeSorryRepeatQuestion("notUnderstand" , midIndex , "wed");
                    }
                }
                stages++;
            break;

        }
        analyse_mid = false;
    }

    // 3. ANALYSE THE ENDING PART OF QUESTIONAIRE

    if (endIndex <= MAX_END_Q && analyse_end) {
        switch (endIndex-1) {
            case 1 :
                if (!detectCakeWeight(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , endIndex , "end");
                }
                if (this->cakeWeight < 250.0 || this->cakeWeight > 2000.0) {
                    return ("We only offer cake weights between 250 grams & 2000 grams ");
                }
                break;

            case 2 :
                if (!checkDateTime(s)) {
                    return ("Please enter correct date-time format, that is dd-mm,hour(am/pm) example 02-06,5pm");
                }
                break;

            case 3 :
                this->deliveryAddress = s;
                break;

            case 4 :
                if (!detectPaymentType(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , endIndex , "end");
                }
                break;

            case 5 :
                if (!checkEmailID(s)) {
                    return makeSorryRepeatQuestion("notUnderstand" , endIndex , "end");
                }
                break;

            case 6 :
                if (!detectMobileNumber(s)) {
                    if (this->mobileNumber == "") {
                        return makeSorryRepeatQuestion("notUnderstand" , endIndex , "end");
                    }
                }
                stages++;
                break;
        }
        analyse_end = false;
    }


    // ASK QUESTION TO USER

    // 1. ASK STARTING QUESTIONS

    if (startIndex < MAX_START_Q) {
        if (startIndex == 4 && this->flavour == "Fruit") {
            startIndex++;
            analyse_start = true;
            return ("What type of fruits cake do you want :- \"mix fruits\" , \"dry fruits\" ");
        }
        string ask = getStartingQuestionStatus(startIndex);
        analyse_start = true;
        return ask;
    }

    // 2. ASK TYPE_OF_CAKE QUESTIONS

    // 2.(a) ASK BIRTHDAY QUESTIONS

    static bool incremented_midIndex = false;

    if (this->cakeType == "Birthday" && midIndex < MAX_BDAY_Q) {
        if (this->isFruit && !incremented_midIndex) {
            midIndex++;
            incremented_midIndex = true;
        }
        string ask = getBirthdayQuestionStatus(midIndex);
        analyse_mid = true;
        return ask;
    }

    // 2.(b) ASK WEDDING/ANNIVERSARY QUESTIONS

    if (this->cakeType == "Wedding/Anniversary" && midIndex < MAX_WED_Q) {
        if (midIndex == 2 && this->isFruit == true) {
            midIndex++;
        }
        string ask = getWedQuestionStatus(midIndex);
        analyse_mid = true;
        return ask;
    }

    // 3. ASK ENDING QUESTIONS

    if (endIndex < MAX_END_Q) {
        string ask = getEndingQuestionStatus(endIndex);
        analyse_end = true;
        return ask;
    }


    // FINAL COMMENTS FROM THE BOT

    this->calcPrice();

    if (stages == 3) {
        return "Your total amount sums up to ₹" + to_string(static_cast<int>(ceil(this->price))) + "\nTo end the session please type quit ";
    }

    return ("To end the session please type quit");
}

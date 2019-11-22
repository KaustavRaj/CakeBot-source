#include "WakeUpTheBot.h"
#include "printreceipt.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QDir>

void WakeUpTheBot::sendBotReply(QString reply)
{
    emit botreply(reply);
}

void WakeUpTheBot::userMsg(const QString reply)
{
    this->startTheBot(reply);
}

void WakeUpTheBot::startTheBot(QString phrase)
{

    if (phrase == "printTheFile321") {
        std::string path = QDir::currentPath().toStdString();
        PrintReceipt p;
        p.printer(bot.getHTML());
        std::string email = bot.getEmailId();
        path = "python " + path + "/email_sender/main.py";
        system(path.c_str());
        sendBotReply("Your cake order has been placed. Thanks for purchasing.");
        return;
    }

    static int cnt = 0;

    if (cnt != 0) {
        if (phrase == "quit"){
            bot.writeUserData();
            cnt = 0;
            return;
        }
    }

    std::string get;

    try {
        get = bot.response(phrase.toStdString());
    }
    catch (...) {
        qDebug() << "BOT RESPONSE ISSUE ... :- ";
        qDebug() << "PHRASE :- " << phrase;
    }

    QString t = QString::fromUtf8(get.data() , static_cast<int>(get.size()));
    sendBotReply(t);

    cnt++;
}




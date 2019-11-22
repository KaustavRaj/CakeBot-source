#ifndef WAKEUPTHEBOT_H
#define WAKEUPTHEBOT_H

#include "database.h"
#include "think.h"
#include <QObject>
#include <QTimer>
#include <QLineEdit>
#include <QVariant>

class WakeUpTheBot : public QObject
{
    Q_OBJECT

public:

    Q_INVOKABLE WakeUpTheBot (QObject* parent = nullptr) : QObject(parent) {}
    Q_INVOKABLE void sendBotReply(QString s);
    Q_INVOKABLE void startTheBot(QString phrase);
    void repeat();

    Think bot;


signals:
    void botreply(const QString reply);

public slots:
    void userMsg(const QString reply);

private:
    QTimer *mTimer;

};

#endif // WAKEUPTHEBOT_H

#ifndef PRINTRECEIPT_H
#define PRINTRECEIPT_H

#include <QObject>
#include "think.h"

class PrintReceipt : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE PrintReceipt();
    Q_INVOKABLE void printer(QString data);
};

#endif // PRINTRECEIPT_H

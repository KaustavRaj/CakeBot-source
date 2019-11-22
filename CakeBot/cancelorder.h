#ifndef CANCELORDER_H
#define CANCELORDER_H

#include <QObject>

class CancelOrder : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE CancelOrder();
    Q_INVOKABLE void cancel();
};

#endif // CANCELORDER_H

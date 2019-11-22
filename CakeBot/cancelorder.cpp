#include "cancelorder.h"
#include "WakeUpTheBot.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <cstdlib>
#include <fstream>

CancelOrder::CancelOrder()
{

}

void CancelOrder::cancel()
{
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Order Cancellation",
                                         "Enter Order ID:", QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);

    if (ok && !text.isEmpty()) {
        bool ok2;
        QString text2 = QInputDialog::getText(nullptr, "Order Cancellation",
                                             "Enter Email ID:", QLineEdit::Normal,
                                             QDir::home().dirName(), &ok2);

        if (ok2 && !text2.isEmpty()) {
            std::string cancel_path = QDir::currentPath().toStdString();
            std::ofstream file;
            file.open(cancel_path + "/email_sender/cancel_order.txt");
            file << text.toStdString() << "\n";
            file << text2.toStdString();
            file.close();
            std::string path = "python " + cancel_path + "/email_sender/order_cancel.py";
            qDebug() << QString::fromUtf8(path.data(), static_cast<int>(path.size()));
            system(path.c_str());

            QMessageBox msgBox;
            msgBox.setText("Cake order with Order ID " + text + " has been cancelled");
            msgBox.exec();
        }
    }

    return;
}

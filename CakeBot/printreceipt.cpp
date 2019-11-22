#include "printreceipt.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QTextEdit>
#include <QFile>
#include <QDebug>
#include <fstream>

PrintReceipt::PrintReceipt() {

}

void PrintReceipt::printer(QString data)
{
    QPrinter printer;
    printer.setPrinterName("receipt print");
    QPrintDialog dialog(&printer);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    QTextEdit txt;

    txt.setHtml(data);
    txt.print(&printer);
}

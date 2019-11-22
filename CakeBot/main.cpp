#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QApplication>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QDialog>
#include <QTimer>
#include <QtQml>

#include <Windows.h>    /* For changing title-bar color & reverting back when the app is closed */
#pragma comment(lib, "user32.lib")

#include "voice.h"
#include "database.h"
#include "printreceipt.h"
#include "botcontact.h"
#include "cancelorder.h"
#include "WakeUpTheBot.h"

/*void MyDialog::reject()
{
    QMessageBox::StandardButton reply = QMessageBox::Yes;
    if (changes) {
        reply = QMessageBox::question( this, app,
                                        tr("Are you sure you want to quit ?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);
    }
    if (reply == QMessageBox::Yes) {
        QDialog::reject();
    }
}*/

static void connectToDatabase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid())
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
        {
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
        }
    }

    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDir.mkpath("."))
    {
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));
    }

    const QString fileName = writeDir.absolutePath() + "/userbotconversation.sqlite3";
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<SqlContactModel>("sql.contact", 1, 0, "SqlContactModel");
    qmlRegisterType<SqlConversationModel>("sql.conversation", 1, 0, "SqlConversationModel");
    qmlRegisterType<WakeUpTheBot>("cake.bot", 1, 0, "WakeUpTheBot");
    qmlRegisterType<PrintReceipt>("receipt.print", 1, 0, "PrintReceipt");
    qmlRegisterType<CancelOrder>("cancel.order", 1, 0, "CancelOrder");
    qmlRegisterType<Voice>("bot.speak", 1, 0, "Voice");

    connectToDatabase();
    QQmlApplicationEngine engine;

    WakeUpTheBot bots;
    engine.rootContext()->setContextProperty("bots", &bots);

    PrintReceipt pr;
    engine.rootContext()->setContextProperty("pr", &pr);

    CancelOrder cancelOrder;
    engine.rootContext()->setContextProperty("cancelOrder", &cancelOrder);

    Voice botSpeak;
    engine.rootContext()->setContextProperty("botSpeak", &botSpeak);

    /********* Changing title-bar color here  *********//*

    int aElements[2] = {COLOR_WINDOW, COLOR_ACTIVECAPTION};
    DWORD aOldColors[2];
    DWORD aNewColors[2];
    aOldColors[0] = GetSysColor(aElements[0]);  // Get the current color of the window background.
    aOldColors[1] = GetSysColor(aElements[1]);  // Get the current color of the active caption.
    aNewColors[0] = aOldColors[0];  // For window background (not changing it)
    aNewColors[1] = RGB(0x80, 0x00, 0x80);  // dark purple
    SetSysColors(2, aElements, aNewColors); // Setting color

    *//********* Not using the above color changing feature for now *******/



    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
        {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    engine.load(url);
    int returnVal = 0;
    //QTimer::singleShot(5*1000,startTheBot(),start());
    //wakeUpTheBot botstart;
    returnVal = returnVal*(app.exec());

        /*if (returnVal == 1000)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Cake Bot", "Are you sure you want to quit ?", QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                app.quit();
            }
        }
        */

    //SetSysColors(2, aElements, aOldColors);   // Reverting back to system color
    return returnVal;
}

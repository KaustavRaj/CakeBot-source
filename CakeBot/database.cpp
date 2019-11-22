#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include "database.h"

static const char *conversationsTableName = "Conversations";
bool databaseMade = false;

static void createTable()
{
    if (QSqlDatabase::database().tables().contains(conversationsTableName)) {
        return;
    }

    QSqlQuery query;
    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS 'Conversations' ("
        "'author' TEXT NOT NULL,"
        "'recipient' TEXT NOT NULL,"
        "'timestamp' TEXT NOT NULL,"
        "'message' TEXT NOT NULL,"
        "FOREIGN KEY('author') REFERENCES Contacts ( name ),"
        "FOREIGN KEY('recipient') REFERENCES Contacts ( name )"
        ")")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }
}

SqlConversationModel::SqlConversationModel(QObject *parent) :
    QSqlTableModel(parent)
{
    createTable();
    setTable(conversationsTableName);
    setSort(2, Qt::DescendingOrder);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    databaseMade = true;
}

QString SqlConversationModel::recipient() const
{
    return m_recipient;
}

void SqlConversationModel::setRecipient(const QString &recipient)
{
    if (recipient == m_recipient)
        return;

    m_recipient = recipient;

    const QString filterString = QString::fromLatin1(
        "(recipient = '%1' AND author = 'Me') OR (recipient = 'Me' AND author='%1')").arg(m_recipient);
    setFilter(filterString);
    select();

    emit recipientChanged();
}

QVariant SqlConversationModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QVariant SqlConversationModel::getAllPrevUserResponses() const
{
    QSqlQuery query("SELECT * FROM Conversations");
    QString s = "";
    while(query.next())
    {
        if(query.value(1).toString() == "Me")
        {
            break;
        }
        s += query.value(3).toString();
    }
    return s;
}

QHash<int, QByteArray> SqlConversationModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "author";
    names[Qt::UserRole + 1] = "recipient";
    names[Qt::UserRole + 2] = "timestamp";
    names[Qt::UserRole + 3] = "message";
    return names;
}

void SqlConversationModel::sendMessage(const QString &recipient, const QString &message)
{
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    QSqlRecord newRecord = record();
    newRecord.setValue("author", makeAuthor);
    newRecord.setValue("recipient", recipient);
    newRecord.setValue("timestamp", timestamp);
    newRecord.setValue("message", message);
    if (!insertRecord(rowCount(), newRecord)) {
        qWarning() << "Failed to send message:" << lastError().text();
        return;
    }

    submitAll();
}

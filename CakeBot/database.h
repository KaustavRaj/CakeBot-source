#ifndef SQLCONVERSATIONMODEL_H
#define SQLCONVERSATIONMODEL_H

#include <QSqlTableModel>

extern int botStart;
extern bool databaseMade;

class SqlConversationModel : public QSqlTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)

public:
    int botStart;
    QString makeAuthor = "Me";
    SqlConversationModel(QObject *parent = nullptr);

    QString recipient() const;
    void setRecipient(const QString &recipient);

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant getAllPrevUserResponses() const;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void sendMessage(const QString &recipient, const QString &message);

signals:
    void recipientChanged();

private:
    QString m_recipient;
};

#endif // SQLCONVERSATIONMODEL_H

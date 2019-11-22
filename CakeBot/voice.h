#ifndef VOICE_H
#define VOICE_H

#include <QObject>
#include <QVector>
#include <QVoice>
#include <QTextToSpeech>

class Voice : public QObject
{
    Q_OBJECT

public:

    Q_INVOKABLE Voice (QObject* parent = nullptr);
    Q_INVOKABLE void speak(QString s);
    Q_INVOKABLE void stop();
    void configure();

private:
    QTextToSpeech *m_speech;
    QVector<QVoice> m_voices;

};

#endif // VOICE_H

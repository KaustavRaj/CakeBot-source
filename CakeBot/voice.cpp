#include "voice.h"

Voice::Voice (QObject* parent) : QObject(parent), m_speech(nullptr) {
    configure();
}

void Voice::speak(QString s)
{
    m_speech->say(s);
}

void Voice::stop()
{
    m_speech->stop();
}

void Voice::configure()
{
    QString engineName = "default";
    delete m_speech;
    if (engineName == "default")
        m_speech = new QTextToSpeech(this);
    else
        m_speech = new QTextToSpeech(engineName, this);

    // Language selection
    QVector<QLocale> locales = m_speech->availableLocales();
    QLocale current = m_speech->locale();
    foreach (const QLocale &locale, locales) {
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        if (locale.name() == current.name())
            current = locale;
    }

    // Gender selection
    m_voices = m_speech->availableVoices();
    QVoice currentVoice = m_speech->voice();
    foreach (const QVoice &voice, m_voices) {
        if (voice.name() == "Microsoft Zira Desktop") {
            m_speech->setVoice(voice);
        }
    }
}

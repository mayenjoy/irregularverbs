#ifndef IRREGULARVERB_H
#define IRREGULARVERB_H

#include <QString>
#include <QList>

class IrregularVerb
{
private:
    QString infinitive;
    QList<QString> past;
    QList<QString> pastParticiple;
    QList<QString> translation;

public:
    IrregularVerb();

    QString getInfinitive() const;
    QString getPast() const;
    QString getPastParticiple() const;
    QString getTranslation() const;

    bool isPast(QString) const;
    bool isPastParticiple(QString) const;
    bool isTranslation(QString) const;

    void setInfinitive(QString);
    void addPast(QString);
    void addPastParticiple(QString);
    void addTranslation(QString);

    int appearances, fails, hits;

    QString toString() const;
};

#endif // IRREGULARVERB_H

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

public:
    IrregularVerb();

    QString getInfinitive() const;
    QString getPast() const;
	QString getPastParticiple() const;

    bool isPast(QString) const;
	bool isPastParticiple(QString) const;

    void setInfinitive(QString);
    void addPast(QString);
	void addPastParticiple(QString);

	bool operator<(const IrregularVerb & ) const;

    int appearances, fails, hits;

    QString toString() const;
};

#endif // IRREGULARVERB_H

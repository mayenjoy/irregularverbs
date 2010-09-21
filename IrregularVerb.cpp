#include "IrregularVerb.h"
#include <iostream>
#include <QStringList>

IrregularVerb::IrregularVerb()
{
    appearances = 0;
    fails = 0;
    hits = 0;
}

QString IrregularVerb::getInfinitive() const
{
    return infinitive;
}

QString IrregularVerb::getPast() const
{
    QString string = "";

    for (int i = 0; i < past.size(); i++)
    {
        if (i > 0)
            string += "/";
        string += past.at(i);
    }

    return string;
}

QString IrregularVerb::getPastParticiple() const
{
    QString string = "";

    for (int i = 0; i < pastParticiple.size(); i++)
    {
        if (i > 0)
            string += "/";
        string += pastParticiple.at(i);
    }

    return string;
}

QString IrregularVerb::getTranslation() const
{
    QString string = "";

    for (int i = 0; i < translation.size(); i++)
    {
        if (i > 0)
            string += "/";
        string += translation.at(i);
    }

    return string;
}

bool IrregularVerb::isPast(QString past) const
{
    return this->past.contains(past);
}

bool IrregularVerb::isPastParticiple(QString pastParticiple) const
{
    return this->pastParticiple.contains(pastParticiple);
}

bool IrregularVerb::isTranslation(QString translation) const
{
    return this->translation.contains(translation);
}

void IrregularVerb::setInfinitive(QString infinitive)
{
    this->infinitive = infinitive;
}

void IrregularVerb::addPast(QString past)
{
    this->past.append(past.split("/"));
}

void IrregularVerb::addPastParticiple(QString pastParticiple)
{
    this->pastParticiple.append(pastParticiple.split("/"));
}

void IrregularVerb::addTranslation(QString translation)
{
    this->translation.append(translation.split("/"));
}

QString IrregularVerb::toString() const
{
    QString aux, aux2, aux3;
    return getInfinitive().replace(" ", "_") + " " + getPast().replace(" ", "_") + " " + getPastParticiple().replace(" ", "_") + " " + getTranslation().replace(" ", "_") + " " + aux.setNum(appearances) + " " + aux2.setNum(fails) + " " + aux3.setNum(hits);
}

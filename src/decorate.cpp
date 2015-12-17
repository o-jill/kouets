/**
 * @file decorate.cpp
 * @brief decorate lines.
 *
 */

#include "decorate.h"

#include <QtCore>

// == == == == == == == ==
// DecorateBase
// == == == == == == == ==

QString DecorateBase::Decorate(QTextStream *ts)
{
    QString result;
    for (;;) {
        QString line = ts->readLine();
        if (line.length() == 0)
            break;
        result += decorate(line);
    }
    return result;
}

/**
 * @brief DecorateBase::Decorate
 *
 * @param sl splitted strings by QString::split("\n").
 *           the last element will not be processed to concatenate the next input.
 *
 * @return decorated string.
 */
QString DecorateBase::Decorate(QStringList *sl)
{
    QString result;
    for (int i = 0 ; i < sl->size()-1 ; ++i) {
        QString line = sl->at(i);
        if (line.length() == 0)
            continue;
        result += decorate(line);
    }
    return result;
}

// == == == == == == == ==
// DecorateGCppVs7
// == == == == == == == ==

QString DecorateGCppVs7::decorate(QString str)
{
    QString result;
    QRegExp reg1("(.+)\\(([0-9]+)\\):(.+) (\\[.+\\]) (\\[[0-9]+\\])");
    QRegExp reg2("(\\D+): (\\d+)");

    str.replace("<", "&lt;");
    str.replace(">", "&gt;");
    // str.replace("&", "&amp;");  // it may not be needed...

    if (reg1.indexIn(str) >= 0) {
        result += "<B>" + reg1.cap(1) + "</B>";
        result += "(<FONT COLOR='RED'>" + reg1.cap(2) + "</FONT>):";
        result += "<STRONG>"+reg1.cap(3)+"</STRONG> ";
        result += "<FONT COLOR='BLUE'>" + reg1.cap(4) + "</FONT> ";
        result += "<FONT COLOR='GREEN'>" + reg1.cap(5) + "</FONT>";
        result += "<BR>";
    } else if (reg2.indexIn(str) >= 0) {
        result += reg2.cap(1);
        result += ": <FONT COLOR='RED'>" + reg2.cap(2) + "</FONT>";
        nerror_ = reg2.cap(2).toInt();
    } else {
        result += str;
        result += "<BR>";
    }

    return result;
}

QString DecorateNone::decorate(QString str)
{
    str.replace("<", "&lt;");
    str.replace(">", "&gt;");

    QString result = str;
    result += "<BR>";

    return result;
}

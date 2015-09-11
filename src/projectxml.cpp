/**
 * @file projectxml.cpp
 * @brief Ç±Ç±Ç…ê‡ñæÇèëÇ≠
 */

#include "projectxml.h"

#include <QtCore>
#include <QtXml>
#include <QDebug>

ProjectXML::ProjectXML()
    :QXmlDefaultHandler(), state_(TAG_NONE)
{
}

bool ProjectXML::readFile(const QString &fileName)
{
    QFile file(fileName);
    return readFile(&file);
}

bool ProjectXML::readFile(QFile *pfile)
{
    QXmlInputSource inputSource(pfile);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    return reader.parse(inputSource);
}

bool ProjectXML::startElement(const QString &namespaceURI,
                  const QString &localName,
                  const QString &qName,
                  const QXmlAttributes &attributes)
{
    if (qName.compare("kouets", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_KOUETS;
    } else if (qName.compare("config", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_CONFIG;
    } else if (qName.compare("item", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_ITEM;
        fc_.Init();
    } else if (qName.compare("file", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_FILE;
    } else if (qName.compare("apppath", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_APPPATH;
    } else if (qName.compare("cmdline", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_CMDLINE;
    } else if (qName.compare("parser", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_PARSER;
    }
    return true;
}

bool ProjectXML::endElement(const QString &namespaceURI,
                const QString &localName,
                const QString &qName)
{
    if (qName.compare("kouets", Qt::CaseInsensitive) == 0) {
        state_ &= ~TAG_BIT_KOUETS;
    } else if (qName.compare("config", Qt::CaseInsensitive) == 0) {
        state_ &= ~TAG_BIT_CONFIG;
    } else if (qName.compare("item", Qt::CaseInsensitive) == 0) {
        items_.push_back(fc_);
        state_ &= ~TAG_BIT_ITEM;
    } else if (qName.compare("file", Qt::CaseInsensitive) == 0) {
        state_ &= ~TAG_BIT_FILE;
    } else if (qName.compare("apppath", Qt::CaseInsensitive) == 0) {
        state_ &= ~TAG_BIT_APPPATH;
    } else if (qName.compare("cmdline", Qt::CaseInsensitive) == 0) {
        state_ &= ~TAG_BIT_CMDLINE;
    } else if (qName.compare("parser", Qt::CaseInsensitive) == 0) {
        state_ &= ~TAG_BIT_PARSER;
    }
    return true;
}

bool ProjectXML::characters(const QString &str)
{
    if (state_ == TAG_CONFIG_APPPATH) {
        defaultapppath_ = str.trimmed();
    } else if (state_ == TAG_CONFIG_CMDLINE) {
        defaultcmdline_ = str.trimmed();
    } else if (state_ == TAG_ITEM_FILE) {
        fc_.SetFilename(str.trimmed());
    } else if (state_ == TAG_ITEM_APPPATH) {
        fc_.SetAppPath(str.trimmed());
    } else if (state_ == TAG_ITEM_CMDLINE) {
        fc_.SetCmdLine(str.trimmed());
    } else if (state_ == TAG_ITEM_PARSER) {
        fc_.SetParser(str.trimmed());
    } else {
        // nothing to do
    }
    // qDebug() << "chara:" << str;
    return true;
}

bool ProjectXML::fatalError(const QXmlParseException &e)
{
    qDebug() << e.message()
             << QString("@%1:%2").arg(e.lineNumber()).arg(e.columnNumber());
            // << e.publicId()
            // << e.systemId();
    return true;
}

bool ProjectXML::startDocument()
{
    return true;
}

bool ProjectXML::endDocument()
{
    return true;
}

void ProjectXML::dump()
{
    qDebug() << "ProjectXML::dump() -- >";
    qDebug() << "defaultapppath_:" << defaultapppath_;
    qDebug() << "defaultcmdline_:" << defaultcmdline_;
    int count = items_.size();
    for (int i = 0 ; i < count ; ++i) {
        qDebug() << QString("item[%1]:filename:").arg(i)
                 << items_[i].Filename();
        if (items_[i].IsDefaultAppPath()) {
            qDebug() << QString("item[%1]:AppPath:default").arg(i);
        } else {
            qDebug() << QString("item[%1]:AppPath:").arg(i)
                     << items_[i].AppPath();
        }
        if (items_[i].IsDefaultCmdLine()) {
            qDebug() << QString("item[%1]:cmdline:default").arg(i);
        } else {
            qDebug() << QString("item[%1]:cmdline:").arg(i)
                     << items_[i].CmdLine();
        }
        if (items_[i].IsDefaultParser()) {
            qDebug() << QString("item[%1]:Parser:default").arg(i);
        } else {
            qDebug() << QString("item[%1]:Parser:").arg(i)
                     << items_[i].Parser();
        }
    }
    qDebug() << "ProjectXML::dump() -- <";
}

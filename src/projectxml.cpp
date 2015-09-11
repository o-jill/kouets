/**
 * @file projectxml.cpp
 * @brief Ç±Ç±Ç…ê‡ñæÇèëÇ≠
 */

#include "projectxml.h"

#include <QtCore>
#include <QtXml>

ProjectXML::ProjectXML()
    :QXmlDefaultHandler()
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
        //
    } else if (qName.compare("config", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_CONFIG;
    } else if (qName.compare("item", Qt::CaseInsensitive) == 0) {
        state_ |= TAG_BIT_ITEM;
        fc_.Init();
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
        defaultapppath_ = str;
    } else if (state_ == TAG_CONFIG_CMDLINE) {
        defaultcmdline_ = str;
    } else if (TAG_ITEM_APPPATH) {
        fc_.SetAppPath(str);
    } else if (TAG_ITEM_CMDLINE) {
        fc_.SetCmdLine(str);
    } else if (TAG_ITEM_PARSER) {
        fc_.SetParser(str);
    } else {
        // nothing to do
    }
    return true;
}

bool ProjectXML::fatalError(const QXmlParseException &exception)
{
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

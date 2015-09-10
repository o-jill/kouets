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
    QXmlInputSource inputSource(&file);
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
    return true;
}

bool ProjectXML::endElement(const QString &namespaceURI,
                const QString &localName,
                const QString &qName)
{
    return true;
}

bool ProjectXML::characters(const QString &str)
{
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

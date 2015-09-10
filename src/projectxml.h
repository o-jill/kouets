/**
 * @file projectxml.h
 * @brief Ç±Ç±Ç…ê‡ñæÇèëÇ≠
 */

#ifndef __PROJECTXML_H__
# define __PROJECTXML_H__

#include <QtCore>
#include <QtXml>

#include "fileconfig.h"

class ProjectXML : public QXmlDefaultHandler
{
public:
    ProjectXML();

    bool readFile(const QString &fileName);

protected:
    virtual bool startDocument();
    virtual bool endDocument();
    virtual bool startElement(const QString &namespaceURI,
                    const QString &localName, const QString &qName,
                    const QXmlAttributes &attributes);
    virtual bool endElement(const QString &namespaceURI,
                    const QString &localName, const QString &qName);
    virtual bool characters(const QString &str);
    virtual bool fatalError(const QXmlParseException &exception);

private:
    QString defaultapppath_;
    QString defaultcmdline_;
    int bactivatetab_;
    int bwraplines_;
    QVector<FileConfig> items_;
    int state_;
};

#endif  // __PROJECTXML_H__

/*
QFile *file = new QFile(argv[1]);

QXmlSimpleReader xmlReader;
QXmlInputSource *source = new QXmlInputSource(file);

Handler *handler = new Handler;
xmlReader.setContentHandler(handler);
xmlReader.setErrorHandler(handler);

bool ok = xmlReader.parse(source);
 */

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
    enum {
        TAG_BIT_KOUETS  = 0x80000000u,
        TAG_BIT_CONFIG  = 0x40000000u,
        TAG_BIT_ITEM    = 0x20000000u,
        TAG_BIT_APPPATH = 0x00008000u,
        TAG_BIT_CMDLINE = 0x00004000u,
        TAG_BIT_PARSER  = 0x00002000u,
        TAG_BIT_FILE    = 0x00001000u,
        // TAG_BIT_,

        TAG_CONFIG_APPPATH = (TAG_BIT_KOUETS|TAG_BIT_CONFIG|TAG_BIT_APPPATH),
        TAG_CONFIG_CMDLINE = (TAG_BIT_KOUETS|TAG_BIT_CONFIG|TAG_BIT_CMDLINE),

        TAG_ITEM_FILE    = (TAG_BIT_KOUETS|TAG_BIT_ITEM|TAG_BIT_FILE),
        TAG_ITEM_APPPATH = (TAG_BIT_KOUETS|TAG_BIT_ITEM|TAG_BIT_APPPATH),
        TAG_ITEM_CMDLINE = (TAG_BIT_KOUETS|TAG_BIT_ITEM|TAG_BIT_CMDLINE),
        TAG_ITEM_PARSER  = (TAG_BIT_KOUETS|TAG_BIT_ITEM|TAG_BIT_PARSER),

        TAG_NONE = 0
    };
public:
    ProjectXML();

    bool readFile(const QString &fileName);
    bool readFile(QFile *pfile);

    bool IsDefaultAppPath() {return bapppath_;}
    bool IsDefaultCmdLine() {return bcmdline_;}
    QString AppPath() {return apppath_;}
    QString CmdLine() {return cmdline_;}

    int ItemSize() {return items_.size();}
    FileConfig at(int n) {return items_[n];}
    QVector<FileConfig>* Files() {return &items_;}

    void dump();
protected:
    virtual bool startDocument();
    virtual bool endDocument();
    virtual bool startElement(const QString &namespaceURI,
                    const QString &localName, const QString &qName,
                    const QXmlAttributes &attributes);
    virtual bool endElement(const QString &namespaceURI,
                    const QString &localName, const QString &qName);
    virtual bool characters(const QString &str);
    virtual bool fatalError(const QXmlParseException &e);

private:
    int bapppath_;
    QString apppath_;
    int bcmdline_;
    QString cmdline_;
    int bactivatetab_;
    int bwraplines_;
    QVector<FileConfig> items_;
    FileConfig fc_;
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

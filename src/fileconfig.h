/**
 * @file fileconfig.h
 * @brief file configuration.
 */

#ifndef __FILECONFIG_H__
# define __FILECONFIG_H__

#include <QString>
#include <QDebug>

class FileConfig
{
public:
    enum {
        False = 0,
        True = 1
    };
public:
    FileConfig()
        :bapppath_(False), bcmdline_(False), bparser_(False) {}
    FileConfig(const FileConfig &rhs)
        :bapppath_(False), bcmdline_(False), bparser_(False) {
        filename_ = rhs.filename_;
        abspath_ = rhs.abspath_;

        bapppath_ = rhs.bapppath_;
        if (bapppath_) {
            apppath_ = rhs.apppath_;
        } else {
        }

        bcmdline_ = rhs.bcmdline_;
        if (bcmdline_) {
            cmdline_ = rhs.cmdline_;
        } else {
        }

        bparser_ = rhs.bparser_;
        if (bparser_) {
            parser_ = rhs.parser_;
        } else {
        }
    }

    ~FileConfig() {}

    void Init() {
        bapppath_ = False;
        bcmdline_ = False;
        bparser_ = False;
        filename_.clear();
        abspath_.clear();
        apppath_.clear();
        cmdline_.clear();
        parser_.clear();
    }

    void SetFilename(const QString &str) {
        filename_ = str;
        QFileInfo fi(str);
        abspath_ = fi.absoluteFilePath();
    }
    void SetFilename_(const QString &str) {
        filename_ = str;
    }
    void SetAbsPath_(const QString &str) {
        abspath_ = str;
    }
    QString& Filename() {
        return filename_;
    }
    QString& AbsPath() {
        return abspath_;
    }
    void SetAppPath(const QString &str) {
        apppath_ = str;
        bapppath_ = True;
    }
    bool IsDefaultAppPath() {
        return bapppath_;
    }
    QString& AppPath() {
        return apppath_;
    }
    void SetCmdLine(const QString &str) {
        cmdline_ = str;
        bcmdline_ = True;
    }
    bool IsDefaultCmdLine() {
        return bcmdline_;
    }
    QString& CmdLine() {
        return cmdline_;
    }
    void SetParser(const QString &str) {
        parser_ = str;
        bparser_ = True;
    }
    bool IsDefaultParser() {
        return bparser_;
    }
    QString& Parser() {
        return parser_;
    }
private:
    QString filename_;
    QString abspath_;
    QString apppath_;
    int bapppath_;
    QString cmdline_;
    int bcmdline_;
    QString parser_;
    int bparser_;
};

#endif  // __FILECONFIG_H__

/**
 * @file fileconfig.h
 * @brief Ç±Ç±Ç…ê‡ñæÇèëÇ≠
 */

#ifndef __FILECONFIG_H__
# define __FILECONFIG_H__

#include <QString>
#include <QDebug>

class FileConfig
{
public:
    FileConfig()
        :bapppath_(FALSE), bcmdline_(FALSE), bparser_(FALSE) {}
    FileConfig(const FileConfig &rhs)
        :bapppath_(FALSE), bcmdline_(FALSE), bparser_(FALSE) {
        filename_ = rhs.filename_;

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
        bapppath_ = FALSE;
        bcmdline_ = FALSE;
        bparser_ = FALSE;
        filename_.clear();
        apppath_.clear();
        cmdline_.clear();
        parser_.clear();
    }

    void SetFilename(const QString &str) {
        QFileInfo fi(str);
        filename_ = fi.absoluteFilePath();
    }
    void SetFilename_(const QString &str) {
        filename_ = str;
    }
    QString& Filename() {
        return filename_;
    }
    void SetAppPath(const QString &str) {
        apppath_ = str;
        bapppath_ = TRUE;
    }
    bool IsDefaultAppPath() {
        return bapppath_;
    }
    QString& AppPath() {
        return apppath_;
    }
    void SetCmdLine(const QString &str) {
        cmdline_ = str;
        bcmdline_ = TRUE;
    }
    bool IsDefaultCmdLine() {
        return bcmdline_;
    }
    QString& CmdLine() {
        return cmdline_;
    }
    void SetParser(const QString &str) {
        parser_ = str;
        bparser_ = TRUE;
    }
    bool IsDefaultParser() {
        return bparser_;
    }
    QString& Parser() {
        return parser_;
    }
private:
    QString filename_;
    QString apppath_;
    int bapppath_;
    QString cmdline_;
    int bcmdline_;
    QString parser_;
    int bparser_;
};

#endif  // __FILECONFIG_H__

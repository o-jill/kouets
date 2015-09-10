/**
 * @file fileconfig.h
 * @brief Ç±Ç±Ç…ê‡ñæÇèëÇ≠
 */

#ifndef __FILECONFIG_H__
# define __FILECONFIG_H__

class FileConfig
{
public:
    FileConfig()
        :apppath_(NULL), cmdline_(NULL), parser_(NULL) {}
    FileConfig(const FileConfig&rhs) {
        filename_ = rhs.filename_;
        if (rhs.apppath_) {
            SetAppPath(*rhs.apppath_);
        } else {
            apppath_ = NULL;
        }
        if (rhs.cmdline_) {
            SetCmdLine(*rhs.cmdline_);
        } else {
            cmdline_ = NULL;
        }
        if (rhs.parser_) {
            SetParser(*rhs.parser_);
        } else {
            parser_ = NULL;
        }
    }
    ~FileConfig() {
        if (apppath_) {
            delete apppath_;
            apppath_ = NULL;
        }
        if (cmdline_) {
            delete cmdline_;
            cmdline_ = NULL;
        }
        if (parser_) {
            delete parser_;
            parser_ = NULL;
        }
    }

    void SetFilename(const QString &str) {
        filename_ = str;
    }
    QString& Filename() {
        return filename_;
    }
    void SetAppPath(const QString &str) {
        if (apppath_) {
            delete apppath_;
        }
        apppath_ = new QString(str);
    }
    bool IsDefaultAppPath() {
        return (apppath_ == NULL);
    }
    QString& AppPath() {
        return *apppath_;
    }
    void SetCmdLine(const QString &str) {
        if (cmdline_) {
            delete cmdline_;
        }
        cmdline_ = new QString(str);
    }
    bool IsDefaultCmdLine() {
        return (cmdline_ == NULL);
    }
    QString& CmdLine() {
        return *cmdline_;
    }
    void SetParser(const QString &str) {
        if (parser_) {
            delete parser_;
        }
        parser_ = new QString(str);
    }
    bool IsDefaultParser() {
        return (parser_ == NULL);
    }
    QString& Parser() {
        return *parser_;
    }
private:
    QString filename_;
    QString *apppath_;
    QString *cmdline_;
    QString *parser_;
}

#endif  // __FILECONFIG_H__

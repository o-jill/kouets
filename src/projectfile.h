#ifndef __PROJECT_FILE_H__
# define __PROJECT_FILE_H__

#include <QtCore>

#include "fileconfig.h"

class ProjectFile
{
public:
    enum {
        FORMAT_UNKNOWN = 0,
        FORMAT_PLAIN,
        FORMAT_XML,
        // FORMAT_JSON,
    };
public:
    ProjectFile() {}
    ~ProjectFile() {}

    void Add(QString path) {
        FileConfig fc;
        fc.SetFilename(path);
        fc_.push_back(fc);
        updatedlist_.push_back(QDateTime());
    }

    int Remove(QString path);

    int Open(const QString &path);
    int Save(const QString &path) {
        return SaveXML(path);
    }
    int SavePlainText(const QString &path);
    int SaveXML(const QString &path);

    int size() {return fc_.size();}
    QString atFilename(int idx) {
        return fc_[idx].Filename();
    }
    FileConfig& at(int idx) {
        return fc_[idx];
    }
    QString atName(int idx) {
        QFileInfo fi(atFilename(idx));
        return fi.fileName();
    }
    QString atPath(int idx) {
        QFileInfo fi(atFilename(idx));
        return fi.absolutePath();
    }
    QDateTime lastUpdated(int idx) {
        return updatedlist_[idx];
    }

    void Copy(QVector<FileConfig> *fc) {
        fc_.resize(fc->size());
        updatedlist_.resize(fc->size());

        for (int i = 0 ; i < fc->size() ; ++i) {
            fc_[i] = fc->at(i);
            updatedlist_[i] = QDateTime();
        }
    }

    /*void Sort() {
        pathlist_.sort();
    }*/

    bool isUpdated(int idx);
    void resetUpdated(int idx);
    void resetUpdated(const QString&path);
    int Find(const QString&path);
private:
    QString apppath_;
    QString cmdline_;
    QVector<FileConfig> fc_;
    QVector<QDateTime> updatedlist_;
};

#endif

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
        pathlist_.push_back(path);
        updatedlist_.push_back(QDateTime());
    }

    int Remove(QString path);

    int Open(const QString &path);
    int Save(const QString &path);

    int size() {return pathlist_.size();}
    QString at(int idx) {
        return pathlist_[idx];
    }
    QString atName(int idx) {
        QFileInfo fi(pathlist_[idx]);
        return fi.fileName();
    }
    QString atPath(int idx) {
        QFileInfo fi(pathlist_[idx]);
        return fi.absolutePath();
    }
    QDateTime lastUpdated(int idx) {
        return updatedlist_[idx];
    }

    void Sort() {
        pathlist_.sort();
    }

    bool isUpdated(int idx);
    void resetUpdated(int idx);
    void resetUpdated(const QString&path);
    int Find(const QString&path);
private:
    QStringList pathlist_;
    QString apppath_;
    QString cmdline_;
    QVector<FileConfig> fc_;
    QVector<QDateTime> updatedlist_;
};

#endif

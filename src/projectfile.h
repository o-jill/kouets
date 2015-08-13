#ifndef __PROJECT_FILE_H__
# define __PROJECT_FILE_H__

#include <QtCore>

class ProjectFile
{
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

    void Sort() {
        pathlist_.sort();
    }

    bool isUpdated(int idx);
private:
    QStringList pathlist_;
    QVector<QDateTime> updatedlist_;
};

#endif

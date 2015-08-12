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
    }

    int Remove(QString path);

    int Open(QFile &file);
    int Save(QFile &file);

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

private:
    QStringList pathlist_;
};

#endif

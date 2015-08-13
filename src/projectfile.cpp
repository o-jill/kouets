#include "projectfile.h"

#include <QtCore>

int ProjectFile::Open(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return -1;
    QDir::setCurrent(path);
    for ( ; ; ) {
        QByteArray ba = file.readLine();
        if (ba.size() == 0)
            break;

        if (ba[0] == '#') {
            continue;
        }

        ba[ba.size()-1] = '\0';
        QFileInfo fi(ba);
        if (fi.isFile()) {
            pathlist_.push_back(fi.absoluteFilePath());
            updatedlist_.push_back(QDateTime());
        }
    }
    return pathlist_.size();
}

int ProjectFile::Save(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return -1;
    QDir::setCurrent(path);
    file.write("# kouets project file\n#\n");
    QDir dir = QDir::current();
    for (int i = 0 ; i < pathlist_.size() ; ++i) {
        file.write(dir.relativeFilePath(pathlist_[i]).toLocal8Bit());
        file.putChar('\n');
    }
    return 1;
}

int ProjectFile::Remove(QString path)
{
    int sz = pathlist_.size();
    for (int i = 0 ; i < sz ; ++i ) {
        if (path.compare(pathlist_[i]) == 0) {
            pathlist_.removeAt(i);
            return 1;
        }
    }
    return -1;
}

bool ProjectFile::isUpdated(int idx)
{
    QString fname = pathlist_[idx];
    QFileInfo fi(fname);
    QDateTime before = updatedlist_[idx];
    bool ret = (fi.lastModified() != before);
    updatedlist_[idx] = fi.lastModified();
    return ret;
}

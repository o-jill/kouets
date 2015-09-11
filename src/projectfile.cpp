#include "projectfile.h"

#include <QtCore>

#include "projectxml.h"

/**
 * @brief ProjectFile::Open
 * @param path
 * @return -2:unknown format, -1:open error, 0:no content, more than zero:number of files.
 */
int ProjectFile::Open(const QString &path)
{
    pathlist_.clear();
    updatedlist_.clear();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return -1;
    QFileInfo pfi(path);
    QDir::setCurrent(pfi.absolutePath());
    QByteArray ba = file.readLine();
    int fileformat = FORMAT_UNKNOWN;
    switch (ba[0]) {
    case '#':
        fileformat = FORMAT_PLAIN;
        break;
    case '<':
        fileformat = FORMAT_XML;
        break;
    default:
        return -2;
    }
    file.seek(0);

    if (fileformat == FORMAT_PLAIN) {
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
    } else if (fileformat == FORMAT_XML) {
        file.close();
        ProjectXML prjxml;
        if (prjxml.readFile(&file)) {
            prjxml.dump();
            int count = prjxml.ItemSize();
            for (int i = 0 ; i < count ; ++i) {
                FileConfig fc = prjxml.at(i);
                QFileInfo fi(fc.Filename());
                if (fi.isFile()) {
                    pathlist_.push_back(fi.absoluteFilePath());
                    updatedlist_.push_back(QDateTime());
                }
            }
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
    for (int i = 0 ; i < sz ; ++i) {
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

void ProjectFile::resetUpdated(int idx)
{
    updatedlist_[idx] = QDateTime();
}

void ProjectFile::resetUpdated(const QString &path)
{
    int idx = Find(path);
    if (idx >= 0)
        updatedlist_[idx] = QDateTime();
}

int ProjectFile::Find(const QString &path)
{
    int count = pathlist_.size();
    int idx;
    for (idx = 0 ; idx < count ; ++idx) {
        if (path == pathlist_[idx])
            return idx;
    }
    return -1;
}

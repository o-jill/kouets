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
    fc_.clear();
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
                Add(fi.absoluteFilePath());
                updatedlist_.push_back(QDateTime());
            }
        }
    } else if (fileformat == FORMAT_XML) {
        file.close();
        ProjectXML prjxml;
        if (prjxml.readFile(&file)) {
#ifdef _DEBUG
            prjxml.dump();
#endif
            Copy(prjxml.Files());
        }
    }

    return fc_.size();
}

int ProjectFile::SavePlainText(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return -1;
    QDir::setCurrent(path);
    file.write("# kouets project file\n#\n");
    QDir dir = QDir::current();
    for (int i = 0 ; i < fc_.size() ; ++i) {
        file.write(dir.relativeFilePath(atFilename(i)).toLocal8Bit());
        file.putChar('\n');
    }
    return 1;
}

int ProjectFile::SaveXML(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return -1;
    QDir::setCurrent(path);
#if 0
    file.write("# kouets project file\n#\n");
    QDir dir = QDir::current();
    for (int i = 0 ; i < pathlist_.size() ; ++i) {
        file.write(dir.relativeFilePath(pathlist_[i]).toLocal8Bit());
        file.putChar('\n');
    }
#endif
    return 1;
}

int ProjectFile::Remove(QString path)
{
    int sz = fc_.size();
    for (int i = 0 ; i < sz ; ++i) {
        if (path.compare(atFilename(i)) == 0) {
            fc_.remove(i);
            updatedlist_.remove(i);
            return 1;
        }
    }
    return -1;
}

bool ProjectFile::isUpdated(int idx)
{
    QString fname = atFilename(idx);
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
    int count = fc_.size();
    int idx;
    for (idx = 0 ; idx < count ; ++idx) {
        if (path == atFilename(idx))
            return idx;
    }
    return -1;
}

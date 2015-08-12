#include "projectfile.h"

#include <QtCore>

int ProjectFile::Open(QFile &file)
{
    for ( ; ; ) {
        QByteArray ba = file.readLine();
        if (ba.size() == 0)
            break;

        if (ba[0] == '#') {
            continue;
        }
        QFileInfo fi(ba);
        if (fi.isFile())
            pathlist_.push_back(ba);
    }
    return pathlist_.size();
}

int ProjectFile::Save(QFile &file)
{
    QDateTime now;
    file.write("# kouets project file\n#\n");

    for (int i = 0 ; i < pathlist_.size() ; ++i) {
        file.write(pathlist_[i].toLocal8Bit());
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

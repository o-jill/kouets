/**
 * @file decorate.h
 * @brief decorate lines.
 *
 */
#ifndef __DECORATE_H__
#define __DECORATE_H__

#include <QString>
#include <QTextStream>
#include <QVector>

class DecorateBase
{
public:
    DecorateBase() :nerror_(-1) {}
    ~DecorateBase() {}

    virtual QString decorate(QString str) = 0;

    QString Decorate(QTextStream *ts);
    QString Decorate(QStringList *sl);

    QString Name() const {return name_;}
    int ErrorNum() {return nerror_;}
    void Reset() {nerror_ = -1;}

protected:
    QString name_;
    int nerror_;

#ifdef __KOUETS_TEST__
public:
    void testtweak(int n) {
        nerror_ = n;
    }
#endif
};

/**
 * No decoration
 */
class DecorateNone : public DecorateBase
{
public:
    DecorateNone() :DecorateBase() {name_ = "none";}

    virtual QString decorate(QString str);
};

/**
 * Google C++ style checker
 * (python cpplint.py --output=vs7)
 */
class DecorateGCppVs7 : public DecorateBase
{
public:
    DecorateGCppVs7() :DecorateBase() {name_ = "cpplint.py_VS7";}

    virtual QString decorate(QString str);
};

/**
 * Google C++ style checker
 * (python cpplint.py)
 */
class DecorateGCpp : public DecorateBase
{
public:
    DecorateGCpp() :DecorateBase() {name_ = "cpplint.py";}

    virtual QString decorate(QString str);
};

/**
 * Cppcheck
 * A tool for static C/C++ code analysis
 */
class DecorateCppCheck : public DecorateBase
{
public:
    DecorateCppCheck() :DecorateBase() {name_ = "cppcheck";}

    virtual QString decorate(QString str);
};

class DecorationManager
{
public:
    DecorationManager() {
        lib_.push_back(new DecorateNone());
        lib_.push_back(new DecorateGCppVs7());
        lib_.push_back(new DecorateGCpp());
        lib_.push_back(new DecorateCppCheck());
    }
    DecorateBase*find(QString name) {
        for (QVector<DecorateBase*>::iterator itr = lib_.begin();
                 itr != lib_.end() ; ++itr) {
            if ((*itr)->Name().compare(name, Qt::CaseInsensitive) == 0) {
                return *itr;
            }
        }
        return NULL;
    }
    int Size() {return lib_.size();}
    QString Name(int idx) {return lib_[idx]->Name();}
private:
    QVector<DecorateBase*> lib_;
};

#endif  // __DECORATE_H__

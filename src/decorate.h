/**
 * @file decorate.h
 * @brief �����ɐ���������
 *
 */
#ifndef __DECORATE_H__
#define __DECORATE_H__

#include <QString>
#include <QTextStream>

class DecorateBase
{
public:
    DecorateBase() {}
    ~DecorateBase() {}

    virtual QString decorate(QString str) = 0;

    QString Decorate(QTextStream *ts);
};

/**
 * Google C++ style checker
 * (python cpplint.py --output=vs7)
 */
class DecorateGCppVs7 : public DecorateBase
{
public:
    DecorateGCppVs7() :DecorateBase(), nerror_(-1) {}

    virtual QString decorate(QString str);

    int ErrorNum() {return nerror_;}
private:
    int nerror_;
};

#endif  // __DECORATE_H__

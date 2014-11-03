#ifndef LESS_STRCMP_QSTRING_H
#define LESS_STRCMP_QSTRING_H
/*
less_strcmp_QString
======================================
QString comparison functor that acts using std::strcmp instead of operator<, allowing
comparison to match the Source Engine's string implementation sort.
*/
#include <functional>
#include <cstring>
#include <QString>

class less_strcmp_QString:
        public std::binary_function<QString, QString, bool>
{
public:
    bool operator()(QString const& lhs, QString const& rhs) const;
};

inline bool less_strcmp_QString::operator()(QString const& lhs, QString const& rhs) const
{
    return std::strcmp(lhs.toStdString().c_str(), rhs.toStdString().c_str()) < 0;
}

#endif // LESS_STRCMP_QSTRING_H

#include "filesystem.h"

QString toQString(boost::filesystem::path const& pathToConvert)
{
    return QString(pathToConvert.string().c_str());
}
boost::filesystem::path toPath(QString const& qstringToConvert)
{
    boost::filesystem::path result{qstringToConvert.toStdWString()};
    result.make_preferred();
    return result;
}

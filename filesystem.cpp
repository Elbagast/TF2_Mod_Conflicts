#include "filesystem.h"

QString tf2mc::toQString(filesystem::path const& pathToConvert)
{
    return QString(pathToConvert.string().c_str());
}
tf2mc::filesystem::path tf2mc::toPath(QString const& qstringToConvert)
{
    filesystem::path result{qstringToConvert.toStdWString()};
    result.make_preferred();
    return result;
}

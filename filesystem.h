#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>
#include <QString>

namespace tf2mc
{
    // alias the experimental std library filesystem namespace
    namespace filesystem
    {
        using namespace std::experimental::filesystem::v1;
    }

    QString toQString(filesystem::path const& pathToConvert);
    filesystem::path toPath(QString const& qstringToConvert);
}



#endif // FILESYSTEM_H

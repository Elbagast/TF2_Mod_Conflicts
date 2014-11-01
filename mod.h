#ifndef MOD_H
#define MOD_H
/*

*/
#include <list>
#include <QString>
#include <QStringList>
#include "qt_boost/filesystem.h"

class Mod
{
public:
    Mod(QString const& modName, std::list<QString> const& filePaths);
    Mod(boost::filesystem::path const& modName, std::list<QString> const& filePaths);

    QString const& name() const;
    std::list<QString> const& files() const;

    void addFile(QString const& file);
    void removeFile(QString const& file);

private:
    QString m_modName;
    std::list<QString> m_filePaths;
};
// inline member functions
//================================================================================
inline QString const& Mod::name() const
{
    return m_modName;
}
inline std::list<QString> const& Mod::files() const
{
    return m_filePaths;
}

// other functions
//================================================================================
inline bool is_directory_mod(boost::filesystem::path const& modPath)
{
    return boost::filesystem::is_directory(modPath);
}
inline bool is_vpk_mod(boost::filesystem::path const& modPath)
{
    return boost::filesystem::is_regular_file(modPath) && modPath.extension().wstring() == L".vpk";
}
inline bool is_valid_mod(boost::filesystem::path const& modPath)
{
    return boost::filesystem::exists(modPath) && (is_directory_mod(modPath) || is_vpk_mod(modPath));
}

#endif // MOD_H

#ifndef MOD_H
#define MOD_H
/*
Mod
======================================
Class to contain a list of files and for a given mod. The mod may be a directory or a .vpk
file. Files are sorted using std::strcmp so that the sort orders correspond to how the
Source engine's string implementation sort.

It might be worth making Mod an abstract class with DirectoryMod and VpkMod derived classes,
though I haven't thought about the usefullness of this yet. Maybe if a mod was responsible for
loading itself on demand...

...or we needed .vpk actions in a non-Mod context like accessing the game files...
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

#ifndef MOD_H
#define MOD_H

#include <list>

#include <QString>
#include <QStringList>

#include "filesystem.h"


namespace tf2mc
{
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
    class Mod
    {
    public:
        Mod(QString const& modName, std::list<QString> const& filePaths);
        Mod(filesystem::path const& modName, std::list<QString> const& filePaths);

        QString const& name() const;
        std::list<QString> const& files() const;

        void addFile(QString const& file);
        void removeFile(QString const& file);

    private:
        QString m_modName;
        std::list<QString> m_filePaths;
    };

    bool is_directory_mod(filesystem::path const& modPath);
    bool is_vpk_mod(filesystem::path const& modPath);
    bool is_valid_mod(filesystem::path const& modPath);
}

// inline member functions
//================================================================================
inline QString const& tf2mc::Mod::name() const
{
    return m_modName;
}
inline std::list<QString> const& tf2mc::Mod::files() const
{
    return m_filePaths;
}

// other functions
//================================================================================
inline bool tf2mc::is_directory_mod(filesystem::path const& modPath)
{
    return filesystem::is_directory(modPath);
}
inline bool tf2mc::is_vpk_mod(filesystem::path const& modPath)
{
    return filesystem::is_regular_file(modPath) && modPath.extension().wstring() == L".vpk";
}
inline bool tf2mc::is_valid_mod(filesystem::path const& modPath)
{
    return filesystem::exists(modPath) && (is_directory_mod(modPath) || is_vpk_mod(modPath));
}

#endif // MOD_H

#ifndef MODCONFLICT_H
#define MODCONFLICT_H

#include <list>

#include <QString>

namespace tf2mc
{
    class Mod;
    /*
    ModConflict
    ======================================
    Class to contain a list of files and a list of mods which ALL contain ALL of those files.
    Mods and files are sorted using std::strcmp so that the sort orders correspond to how the
    Source engine's string implementation sort.
    */
    class ModConflict
    {
    public:
        ModConflict() = default;

        void addMod(QString const& modName);
        void addFile(QString const& filePath);

        std::list<QString> const& mods() const;
        std::list<QString> const& files() const;

        void sort();

    private:
        std::list<QString> m_modNames;
        std::list<QString> m_filePaths;
    };

    // inline member functions
    //================================================================================
    inline std::list<QString> const& ModConflict::mods() const
    {
        return m_modNames;
    }
    inline std::list<QString> const& ModConflict::files() const
    {
        return m_filePaths;
    }

    // Relational operators
    //================================================================================
    inline bool operator< (ModConflict const& lhs, ModConflict const& rhs)
    {
        if (lhs.files() < rhs.files())
            return true;
        else if (lhs.files() == rhs.files())
        {
            if (lhs.mods() < rhs.mods())
                return true;
            return false;
        }
        else
            return false;
    }
    inline bool operator> (ModConflict const& lhs, ModConflict const& rhs)  {return rhs < lhs;}
    inline bool operator<=(ModConflict const& lhs, ModConflict const& rhs)  {return !(lhs > rhs);}
    inline bool operator>=(ModConflict const& lhs, ModConflict const& rhs)  {return !(lhs < rhs);}

    inline bool operator==(ModConflict const& lhs, ModConflict const& rhs)
    {
        return (lhs.mods() == rhs.mods()) && (lhs.files() == rhs.files());
    }
    inline bool operator!=(ModConflict const& lhs, ModConflict const& rhs)  {  return !(lhs == rhs);}

    // Other functions
    //================================================================================
    std::list<ModConflict> getModConflicts(std::list<Mod> const& modList);
}

#endif // MODCONFLICT_H

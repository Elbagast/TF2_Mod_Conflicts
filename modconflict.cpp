#include "modconflict.h"
#include "mod.h"
#include "less_strcmp_qstring.h"
#include <map>
#include <algorithm>
#include <functional>

void ModConflict::addMod(QString const& modName)
{
    m_modNames.push_back(modName);
    m_modNames.sort(less_strcmp_QString());
}
void ModConflict::addFile(QString const& filePath)
{
    m_filePaths.push_back(filePath);
    m_filePaths.sort(less_strcmp_QString());
}

void ModConflict::sort()
{
    m_modNames.sort(less_strcmp_QString());
    m_filePaths.sort(less_strcmp_QString());
}

std::list<ModConflict> getModConflicts(std::list<Mod> const& modList)
{
    std::list<ModConflict> results;

    // Get all the unique files
    // OK QStringList lacks a .unique() function...goddammit
    std::list<QString> unique_files;

    // And a map of all files where key = file, value = mod
    std::multimap<QString, QString> all_files;
    for(auto& mod : modList)
    {
        for(auto& file : mod.files())
        {
            // could do a find on every file before this but that seems overkill
            unique_files.push_back(file);
            all_files.emplace(file, mod.name());
        }
    }
    // sort files in a way that matches Valve's string implementation
    unique_files.sort(less_strcmp_QString());
    unique_files.unique();

    // Analyse the files and mods for conflicts
    for (auto& file : unique_files)
    {
        size_t count = all_files.count(file);
        if (count > 1)
        {
            // for a conflicting file
            // get a list of mods it's in
            std::list<QString> mods;
            for (auto i = all_files.equal_range(file).first, end = all_files.equal_range(file).second; i != end; ++i)
            {
                mods.push_back(i->second);
            }
            //std::list<ModConflict>::iterator existing_conflict = modmatch_ModConflict(conflicts.begin(), conflicts.end(), mods);	// this is almost std::find, it has to comapre mods to a part of the structure
            auto existingConflictIterator = std::find_if(results.begin(), results.end(), [&](ModConflict const& mc){ return mc.mods() == mods;});
            if (existingConflictIterator != results.end()) // a conflict exists with that mod list add the file to it
            {
                existingConflictIterator->addFile(file);
            }
            else // create a new conflict entry with that file
            {
                ModConflict new_conflict{};
                for(auto& mod : mods)
                    new_conflict.addMod(mod);
                new_conflict.addFile(file);
                results.push_back(new_conflict);
            }
        }
    }
    // Sort in ModConflicts in decending order
    results.sort(std::greater<ModConflict>());

    return results;
}

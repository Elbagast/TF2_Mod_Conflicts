#include "mod.h"
#include <algorithm>

Mod::Mod(QString const& modPath, std::list<QString> const& filePaths):
    m_modName(modPath),
    m_filePaths(filePaths)
{
}

Mod::Mod(boost::filesystem::path const& modPath, std::list<QString> const& filePaths):
    Mod(QString(modPath.string().c_str()), filePaths)
{
}

void Mod::addFile(QString const& file)
{
    if (std::find(m_filePaths.begin(), m_filePaths.end(), file) != m_filePaths.end())
    {
        m_filePaths.push_back(file);
        m_filePaths.sort();
    }
}
void Mod::removeFile(QString const& file)
{
    m_filePaths.remove(file);
}

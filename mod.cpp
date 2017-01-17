#include "mod.h"
#include <algorithm>

tf2mc::Mod::Mod(QString const& modPath, std::list<QString> const& filePaths):
    m_modName(modPath),
    m_filePaths(filePaths)
{
}

tf2mc::Mod::Mod(filesystem::path const& modPath, std::list<QString> const& filePaths):
    Mod(QString(modPath.string().c_str()), filePaths)
{
}

void tf2mc::Mod::addFile(QString const& file)
{
    if (std::find(m_filePaths.begin(), m_filePaths.end(), file) != m_filePaths.end())
    {
        m_filePaths.push_back(file);
        m_filePaths.sort();
    }
}
void tf2mc::Mod::removeFile(QString const& file)
{
    m_filePaths.remove(file);
}

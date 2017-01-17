#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modconflict.h"

#include <string>
#include <thread>

#include <QFileDialog>
#include <QProcess>
#include <QSettings>


tf2mc::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tf2Path{},
    m_vpkExePath{},
    m_customDirectoryPath{},
    m_ignoreModsModel{new EditableListModel},
    m_ignoreFilesModel{new EditableListModel}
{
    ui->setupUi(this);
    ui->autoSettingsWidget->hide();

    // Connect the triggered() signal as this is only emitted when the user interacts with it
    QObject::connect(ui->actionSet_Team_Fortress_2_path, SIGNAL(triggered()),
                     this, SLOT(browseForTf2Path()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()),
                     this, SLOT(close()));

    ui->ignoreModsListView->setModel(m_ignoreModsModel.get());
    ui->ignoreFilesListView->setModel(m_ignoreFilesModel.get());


    readSettings();
    //message_settings();
}

// Qt Creator doesn't like this line but it is legal and compiles
tf2mc::MainWindow::~MainWindow() = default;

void tf2mc::MainWindow::closeEvent(QCloseEvent* event)
{
    // if settings have changed...ask if we want to save them
    // save settings
    // save window gemoetry
    writeSettings();
    QMainWindow::closeEvent(event);
}

// private slots
//================================================================================
void tf2mc::MainWindow::browseForTf2Path()
{
    QString tf2Path = QFileDialog::getExistingDirectory(this, tr("Your 'Team Fortess 2' directory"));
    if(tf2Path.size() != 0)
        ui->tf2pathLineEdit->setText(tf2Path);
}

void tf2mc::MainWindow::updateTf2Path(QString tf2Path)
{
    using filesystem::path;
    using filesystem::exists;
    using filesystem::is_directory;

    //outputAppend("updateTf2Path(QString)");
    path new_tf2Path(tf2Path.toStdWString());
    new_tf2Path.make_preferred();
    if(exists(new_tf2Path) && is_directory(new_tf2Path))
    {
        m_tf2Path = new_tf2Path;
    }
    else
    {
        m_tf2Path.clear();
    }
    updateVpkExe(m_tf2Path);
    updateCustomDirectory(m_tf2Path);
}

void tf2mc::MainWindow::scanForConflicts()
{
    outputAppend("Scan for mod conflicts");
    outputAppend("========================================");

    // Test settings are valid
    if (!settingsAreValid())
    {
         message_settingsAreNotValid();
         return;
    }

    // Generate a list of Mods
    // Ignored mods and files are already removed
    std::list<Mod> modList = parseAllMods();

    // Analyse it

    std::list<ModConflict> modConflictList = getModConflicts(modList);
    // Output results
    size_t conflictCount = modConflictList.size();
    size_t conflictNumber = 1;
    for (auto& conflict : modConflictList)
    {
        // These conversions are a bit much...
        QString conflictTitle = QString("Conflict ")
                                + std::to_string(conflictNumber).c_str()
                                + " of "
                                + std::to_string(conflictCount).c_str();
        outputAppend(conflictTitle);
        outputAppend("----------------------------------------------------------------------------------------------------");

        QString filesTitle = QString(std::to_string(conflict.files().size()).c_str())
                             + " files in "
                             + std::to_string(conflict.mods().size()).c_str()
                             + " mods:";
        outputAppend(filesTitle);

        for (auto& file : conflict.files())
        {
            outputAppend(file);
        }
        outputAppend();

        QString usedModTitle = "Game will use files in:";
        outputAppend(usedModTitle);

        auto modListIterator = conflict.mods().begin();
        outputAppend(*modListIterator);
        outputAppend();

        // advance past the first item
        ++modListIterator;

        QString otherModTitle = "Ignoring versions in:";
        outputAppend(otherModTitle);
        for(auto end = conflict.mods().end(); modListIterator != end; ++modListIterator)
        {
            outputAppend(*modListIterator);
        }
        outputAppend();

        ++conflictNumber;
    }
}
void tf2mc::MainWindow::listAllModFiles()
{
    outputAppend("List of all files in all mods");
    outputAppend("========================================");
    // Test settings are valid
    if (!settingsAreValid())
    {
         message_settingsAreNotValid();
         return;
    }
    // Generate a list of Mods
    // Ignored mods and files are already removed
    std::list<Mod> modList = parseAllMods();

    // Output it
    for (auto const& mod : modList)
    {
        outputAppend(mod.name());
        outputAppend("----------------------------------------------------------------------------------------------------");
        for (auto const& file : mod.files())
        {
            outputAppend(file);
        }
        outputAppend();
    }
}
void tf2mc::MainWindow::showSettings()
{
    message_settings();
}
void tf2mc::MainWindow::showAutoSettings(bool value)
{
    if(value)
    {
        ui->autoSettingsWidget->show();
    }
    else
    {
        ui->autoSettingsWidget->hide();
    }
}

void tf2mc::MainWindow::updateVpkExe(filesystem::path const& tf2Path)
{
    using filesystem::path;
    using filesystem::exists;
    using filesystem::is_regular_file;

    path vpkExePath = tf2Path / path("\\bin\\vpk.exe");
    vpkExePath.make_preferred();
    if(exists(vpkExePath) && is_regular_file(vpkExePath))
    {
        m_vpkExePath = vpkExePath;
        ui->vpkExeLabel->setText(toQString(vpkExePath));
    }
    else
    {
        m_vpkExePath.clear();
        ui->vpkExeLabel->setText("Could not find \\Team Fortress 2\\bin\\vpk.exe");
    }
}
void tf2mc::MainWindow::updateCustomDirectory(filesystem::path const& tf2Path)
{
    using filesystem::path;
    using filesystem::exists;
    using filesystem::is_directory;

    path customDirectoryPath = tf2Path / path("\\tf\\custom");
    customDirectoryPath.make_preferred();
    if(exists(customDirectoryPath) && is_directory(customDirectoryPath))
    {
        m_customDirectoryPath = customDirectoryPath;
        ui->customDirectoryLabel->setText(toQString(customDirectoryPath));
    }
    else
    {
        m_customDirectoryPath.clear();
        ui->customDirectoryLabel->setText("Could not find \\Team Fortress 2\\tf\\custom");
    }
}

tf2mc::Mod tf2mc::MainWindow::parseDirectoryMod(filesystem::path const& modPath) const
{
    using filesystem::path;
    using filesystem::is_regular_file;
    using filesystem::recursive_directory_iterator;

    std::list<QString> modFiles;
    for (recursive_directory_iterator recDirIterator(modPath), end; recDirIterator != end; ++recDirIterator)
    {
        if (is_regular_file(*recDirIterator))					// found a file
        {
            path filePath(*recDirIterator);                                 // path p is BLAH/tf/custom/dir/stuff/file.ext
            size_t dir_length(modPath.wstring().size());                    // path dir is BLAH/tf/custom/dir
            path short_filePath(filePath.wstring().substr(dir_length + 1));	// cut p down to stuff/file.ext
            short_filePath.make_preferred();                                // stuff\file.ext

            modFiles.push_back(toQString(short_filePath));
        }
    }
    Mod parsedMod{modPath.filename(), modFiles};
    return parsedMod;
}
tf2mc::Mod tf2mc::MainWindow::parseVpkMod(filesystem::path const& modPath) const
{
    QString qVpkExePath = toQString(m_vpkExePath);
    QStringList qVpkExeArguments;
    qVpkExeArguments << "l" << toQString(modPath);

    QProcess process{};
    process.start(qVpkExePath, qVpkExeArguments);

    if (!process.waitForStarted())
    {
        // failed to start....
    }

    if (!process.waitForFinished())
    {
        // failed to end....
    }

    // Sanity check on vpk.exe output would go here - error throw too

    QStringList modFiles = QString(process.readAll()).split("\r\n");
    // The split results in the last entry begin empty, os remove it
    if(modFiles.back().isEmpty())
    {
        modFiles.pop_back();
    }

    // Path formatting to preferred
    modFiles.replaceInStrings("/", "\\");

    Mod parsedMod{modPath.filename(), modFiles.toStdList()};
    return parsedMod;
}

std::list<tf2mc::Mod> tf2mc::MainWindow::parseAllMods()
{
    using filesystem::path;
    using filesystem::is_regular_file;
    using filesystem::is_directory;
    using filesystem::directory_iterator;

    std::list<Mod> modList;

    for (directory_iterator dirIterator(m_customDirectoryPath), end; dirIterator != end; ++dirIterator)
    {
        path modPath{*dirIterator};
        if (is_directory_mod(modPath))
        {
            modList.push_back(parseDirectoryMod(modPath));
        }
        else if (is_vpk_mod(modPath))
        {
           modList.push_back(parseVpkMod(modPath));
        }
        // everything else is ignored
    }

    // remove ignored mods
    if (ui->ignoreModsCheckBox->isChecked())
    {
       // message_ignoreMods();
        for (auto& modName : m_ignoreModsModel->getData())
        {
            modList.remove_if([&modName](Mod const& foundMod){ return foundMod.name() == modName;});
        }
    }

    // remove ignored files
    if (ui->ignoreFilesCheckBox->isChecked())
    {
        //message_ignoreFiles();
        for (auto& mod : modList)
        {
            for (auto& file : m_ignoreFilesModel->getData())
            {
                mod.removeFile(file);
            }
        }
    }

    return modList;
}

// Member variable convenience
//================================================================================
bool tf2mc::MainWindow::settingsAreValid() const
{
    return !(m_tf2Path.empty() ||
             m_vpkExePath.empty() ||
             m_customDirectoryPath.empty() );
}
bool tf2mc::MainWindow::ignoreMods() const
{
    return ui->ignoreModsCheckBox->isChecked();
}
bool tf2mc::MainWindow::ignoreFiles() const
{
    return ui->ignoreFilesCheckBox->isChecked();
}
void tf2mc::MainWindow::outputAppend(QString const& value)
{
    ui->outputTextEdit->append(value);
}
void tf2mc::MainWindow::outputClear()
{
    ui->outputTextEdit->clear();
}

void tf2mc::MainWindow::setIgnoreMods(bool value)
{
    ui->ignoreModsCheckBox->setChecked(value);
}
void tf2mc::MainWindow::setIgnoreFiles(bool value)
{
    ui->ignoreFilesCheckBox->setChecked(value);
}

// Messages printed to output
//================================================================================
void tf2mc::MainWindow::message_settingsAreNotValid()
{
    outputAppend("Could not run, settings are invalid.");
}
void tf2mc::MainWindow::message_settings()
{
    outputClear();

    outputAppend("Settings");
    outputAppend("========================================");

    outputAppend("TF2 path:");
    outputAppend(toQString(m_tf2Path));

    outputAppend("vpk.exe path:");
    outputAppend(toQString(m_vpkExePath));

    outputAppend("tf\\custom path:");
    outputAppend(toQString(m_customDirectoryPath));

    outputAppend();
    message_ignoreMods();
    message_ignoreFiles();
}
void tf2mc::MainWindow::message_ignoreMods()
{
    outputAppend("Ignore mods:");
    outputAppend("--------------------");
    if (m_ignoreModsModel->getData().empty())
    {
        outputAppend("Not ignore any mods.");
    }
    else
    {
        for(auto& mod : m_ignoreModsModel->getData())
        {
             outputAppend(mod);
        }
    }
    outputAppend();
}
void tf2mc::MainWindow::message_ignoreFiles()
{
    outputAppend("Ignore files:");
    outputAppend("--------------------");
    if (m_ignoreFilesModel->getData().empty())
    {
        outputAppend("Not ignore any files.");
    }
    else
    {
        for(auto& file : m_ignoreFilesModel->getData())
        {
             outputAppend(file);
        }

    }
    outputAppend();
}

// Settings functions
//================================================================================
QString tf2mc::MainWindow::getSettingsPath() const
{
    using filesystem::path;

    // Path to this executable
    path selfPath = toPath(QApplication::arguments().front());

    // Path to the settings file
    //path settingsPath{ selfPath.parent_path() / "settings.ini"};
    path settingsPath = selfPath;
    settingsPath.replace_extension("ini");

    return toQString(settingsPath);
}

void tf2mc::MainWindow::readSettings()
{
    // this first line works, but I have no idea where the file goes
    //QSettings settings("Elbagast", "TF2_Mod_Conflicts");

    QSettings settings(getSettingsPath(), QSettings::IniFormat);
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    //updateTf2Path(settings.value("TF2").toString());
    ui->tf2pathLineEdit->setText(settings.value("TF2").toString());

    setIgnoreMods(settings.value("IGNORE_MODS").toBool());

    int modCount = settings.beginReadArray("MODS");
    for(int index = 0; index < modCount; ++index)
    {
        settings.setArrayIndex(index);
        m_ignoreModsModel->addItem(settings.value("MOD").toString());
    }
    settings.endArray();

    setIgnoreFiles(settings.value("IGNORE_FILES").toBool());

    int fileCount = settings.beginReadArray("FILES");
    for(int index = 0; index < fileCount; ++index)
    {
        settings.setArrayIndex(index);
        m_ignoreFilesModel->addItem(settings.value("FILE").toString());
    }
    settings.endArray();
}
void tf2mc::MainWindow::writeSettings()
{
    //QSettings settings("Elbagast", "TF2_Mod_Conflicts");
    QSettings settings(getSettingsPath(), QSettings::IniFormat);

    settings.clear();

    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    settings.setValue("TF2", toQString(m_tf2Path) );

    settings.setValue("IGNORE_MODS", ignoreMods());

    settings.beginWriteArray("MODS");
    int mod_index = 0;
    for(auto& mod : m_ignoreModsModel->getData())
    {
        settings.setArrayIndex(mod_index);
        settings.setValue("MOD", mod);
        ++mod_index;
    }
    settings.endArray();

    settings.setValue("IGNORE_FILES", ignoreFiles());

    settings.beginWriteArray("FILES");
    int file_index = 0;
    for(auto& file : m_ignoreFilesModel->getData())
    {
        settings.setArrayIndex(file_index);
        settings.setValue("FILE", file);
        ++file_index;
    }
    settings.endArray();
}

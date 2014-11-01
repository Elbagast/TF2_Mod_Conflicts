#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include "mod.h"
#include "qt_boost/filesystem.h"
#include "editablelistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow :
        public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void closeEvent(QCloseEvent* event) override;

private slots:
    // Act on user interaction with ui->browseForTf2PathButton
    void browseForTf2Path();
    // When ui->tf2PathLineEdit emits textChanged(), this is called to update all the settings
    void updateTf2Path(QString tf2Path);

    // Act on user interaction with ui->scanForConflictsButton
    void scanForConflicts();
    // Act on user interaction with ui->listAllModFilesButton
    void listAllModFiles();
    // Act on user interaction with ui->showSettingsButton
    void showSettings();

    void showAutoSettings(bool value);

private:
    void updateVpkExe(boost::filesystem::path const& tf2Path);
    void updateCustomDirectory(boost::filesystem::path const& tf2Path);

    Mod parseDirectoryMod(boost::filesystem::path const& modPath) const;
    Mod parseVpkMod(boost::filesystem::path const& modPath) const;

    std::list<Mod> parseAllMods();

    // Member variable convenience
    //========================================
    bool settingsAreValid() const;
    bool ignoreMods() const;
    bool ignoreFiles() const;
    void outputAppend(QString const& value = QString());
    void outputClear();

    void setIgnoreMods(bool value);
    void setIgnoreFiles(bool value);

    // Messages printed to output
    //========================================
    void message_settingsAreNotValid();
    void message_settings();
    void message_ignoreMods();
    void message_ignoreFiles();

    // Settings functions
    //========================================
    QString getSettingsPath() const;
    void readSettings();
    void writeSettings();

    // Member variables
    //========================================
    Ui::MainWindow *ui;

    boost::filesystem::path m_tf2Path;
    boost::filesystem::path m_vpkExePath;
    boost::filesystem::path m_customDirectoryPath;

    std::unique_ptr<EditableListModel> m_ignoreModsModel;
    std::unique_ptr<EditableListModel> m_ignoreFilesModel;
};


QString toQString(boost::filesystem::path const& pathToConvert);
boost::filesystem::path toPath(QString const& qstringToConvert);

#endif // MAINWINDOW_H

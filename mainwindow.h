#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>
#include <memory>

#include <QMainWindow>

#include "mod.h"
#include "filesystem.h"
#include "editablelistmodel.h"
#include "ui_mainwindow.h"


namespace Ui
{
    class MainWindow;
}

namespace tf2mc
{
    /*
    MainWindow
    ======================================
    QMainWindow class that currently encapsulates all of the operations the program does, including
    storing all the settings as member variables.

    It might be worth breaking this class up a bit.
    */
    class MainWindow :
            public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

        // override to allow saving of settings in here
        void closeEvent(QCloseEvent* event) override;

    private slots:
        // Act on user interaction with ui->browseForTf2PathButton
        void browseForTf2Path();
        // When ui->tf2PathLineEdit emits textChanged(), this is called to update all the settings.
        // Right now this means to set the field programatically, you should use
        // ui->tf2PathLineEdit->setText(...);
        // then the change will be visible and also cascade down to this slot (and beyond)
        void updateTf2Path(QString tf2Path);

        // Act on user interaction with ui->scanForConflictsButton
        void scanForConflicts();
        // Act on user interaction with ui->listAllModFilesButton
        void listAllModFiles();
        // Act on user interaction with ui->showSettingsButton
        void showSettings();
        // Act on user interaction with ui->autoSettingsCheckBox
        void showAutoSettings(bool value);

    private:
        // Convenience functions to update these fields
        void updateVpkExe(filesystem::path const& tf2Path);
        void updateCustomDirectory(filesystem::path const& tf2Path);

        // Mod parsing functions
        //========================================
        Mod parseDirectoryMod(filesystem::path const& modPath) const;
        Mod parseVpkMod(filesystem::path const& modPath) const;
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
        std::unique_ptr<Ui::MainWindow> ui;

        filesystem::path m_tf2Path;
        filesystem::path m_vpkExePath;
        filesystem::path m_customDirectoryPath;

        std::unique_ptr<EditableListModel> m_ignoreModsModel;
        std::unique_ptr<EditableListModel> m_ignoreFilesModel;
    };
}

#endif // MAINWINDOW_H

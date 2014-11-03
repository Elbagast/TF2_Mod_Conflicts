TF2 Mod Conflicts
================================================================================
A program which scans the Team Fortress 2 custom folder and analyses the contents
for any conflicts between these files, then reports which files will be used by
the game if any conflicts are found. The user can specify mods and files which the
program will ignore, primarily t stop files which the game itself creates in all
mods from cluttering the output with files the user won't care about.

Currently it uses vpk.exe using the command line "vpk.exe l [mod.vpk]" to capture
a list of a .vpk file's contents and the Boost filesystem library to analyse
directory mods.

The program saves all settings between runs.


Required Libraries:
- Boost 1.55.0 ( boost/filesystem.hpp )
- Qt 5.3

Built using:
- Qt Creator
- MSVC 2013 update 3 compiler

#ifndef QT_BOOST_FILESYSTEM_H
#define QT_BOOST_FILESYSTEM_H
/*
Workaround for moc eating boost macros by wrapping the include in a guard.

Include this file instead of the actual one so you don't have to repeat this guard every time.
  */
#ifndef Q_MOC_RUN
#include <boost/filesystem.hpp>
#endif // Q_MOC_RUN

#endif // QT_BOOST_FILESYSTEM_H

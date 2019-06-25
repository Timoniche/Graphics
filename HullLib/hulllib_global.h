#ifndef HULLLIB_GLOBAL_H
#define HULLLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HULLLIB_LIBRARY)
#  define HULLLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HULLLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HULLLIB_GLOBAL_H

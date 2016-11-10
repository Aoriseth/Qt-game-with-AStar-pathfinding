#ifndef WORLD_GLOBAL_H
#define WORLD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WORLD_LIBRARY)
#  define WORLDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WORLDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WORLD_GLOBAL_H

#ifndef CHEMSPIDER_GLOBAL_HPP
#define CHEMSPIDER_GLOBAL_HPP

#include <QtCore/QtGlobal>

#if defined(CHEMSPIDER_LIBRARY)
#  define CHEMSPIDERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CHEMSPIDERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CHEMSPIDER_GLOBAL_H


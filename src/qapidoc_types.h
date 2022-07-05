#pragma once

#include "./qapidoc_global.h"
#include <QBitArray>
#include <QByteArray>
#include <QChar>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QUuid>
#include <QVariant>
#include <QVariantHash>
#include <QVariantList>
#include <QVariantMap>
#include <QVector>

namespace QApiDoc {

//!
//! \brief QStringList
//!
typedef QVector<QByteArray> QByteArrayList;

//!
//! \brief object_ignore_methods
//! \return
//!
const QStringList &object_ignore_methods();

} // namespace QApiDoc

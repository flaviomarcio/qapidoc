#include "./qapidoc_object_mapper.h"
#include "./qapidoc_meta_types.h"
#include "./qapidoc_types.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>

namespace QApiDoc {

static bool writeProperty(QObject *object, const QMetaProperty &property, const QVariant &value)
{
    auto type = property.typeId();
    QVariant vValue = value;

    if (property.write(object, vValue))
        return true;

    if (QMetaTypeUtilMetaString.contains(type)) {
        QVariant v;
        if (QMetaTypeUtilObjects.contains(value.typeId()))
            v = QJsonDocument::fromVariant(vValue).toJson(QJsonDocument::Compact);
        else
            v = vValue.toByteArray();

        switch (type) {
        case QMetaType::QUuid:
            if (property.write(object, vValue.toUuid()))
                return true;
            break;
        case QMetaType::QUrl:
            if (property.write(object, vValue.toUrl()))
                return true;
            break;
        case QMetaType::QString:
            if (property.write(object, v.toString()))
                return true;
            break;
        case QMetaType::QByteArray:
            if (property.write(object, v.toByteArray()))
                return true;
            break;
        case QMetaType::QChar:
            if (property.write(object, v.toChar()))
                return true;
            break;
        case QMetaType::QBitArray:
            if (property.write(object, v.toBitArray()))
                return true;
            break;
        default:
            return false;
        }
    }

    if (QMetaTypeUtilIntegers.contains(type)) { //ints

        switch (type) {
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            if (property.write(object,
                               static_cast<qlonglong>(QLocale::c().toDouble(vValue.toString()))))
                return true;
            if (property.write(object, QLocale::c().toLongLong(vValue.toString())))
                return true;
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            if (property.write(object, QLocale::c().toInt(vValue.toString())))
                return true;
            if (property.write(object, QLocale::c().toInt(vValue.toString())))
                return true;
            if (property.write(object, QLocale::c().toUInt(vValue.toString())))
                return true;
            break;
        case QMetaType::Double:
            if (property.write(object, QLocale::c().toDouble(vValue.toString())))
                return true;
            break;
        default:
            return false;
        }
    }

    if (QMetaTypeUtilObjects.contains(type)) {
        switch (type) {
        case QMetaType::QVariantMap:
            if (property.write(object, vValue.toHash()))
                return true;
            break;
        case QMetaType::QVariantHash:
            if (property.write(object, vValue.toHash()))
                return true;
            break;
        case QMetaType::QVariantList:
            if (property.write(object, vValue.toList()))
                return true;
            break;
        case QMetaType::QStringList:
            if (property.write(object, vValue.toStringList()))
                return true;
            break;
        default:
            return false;
        }
    }

    if (QMetaTypeUtilDates.contains(type)) {
        switch (type) {
        case QMetaType::QDate:
            if (property.write(object, vValue.toDate()))
                return true;
            break;
        case QMetaType::QDateTime:
            if (property.write(object, vValue.toDateTime()))
                return true;
            break;
        case QMetaType::QTime:
            if (property.write(object, vValue.toTime()))
                return true;
            break;
        default:
            return false;
        }
    }

    if (QMetaTypeUtilBool.contains(type) || QMetaTypeUtilBool.contains(value.typeId())) {
        switch (type) {
        case QMetaType::Bool:
            if (property.write(object, vValue.toBool()))
                return true;
            break;
        default:
            break;
        }

        switch (vValue.typeId()) {
        case QMetaType::Bool:
            if (property.write(object, vValue.toBool()))
                return true;
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::ULongLong:
        case QMetaType::LongLong:
        case QMetaType::Double:
            if (property.write(object, (vValue.toInt() == 1)))
                return true;
            break;
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar: {
            auto vv = vValue.toString().toLower();
            bool vBool = (vv == QStringLiteral("true"));
            if (property.write(object, vBool))
                return true;
            break;
        }
        default:
            return false;
        }
    }
    return false;
}

ObjectMapper::ObjectMapper(QObject *parent) : QObject{parent} {}

ObjectMapper::~ObjectMapper() {}

ObjectMapper &ObjectMapper::operator=(const QVariant &value)
{
    this->load(value);
    return *this;
}

ObjectMapper &ObjectMapper::operator<<(const QVariant &value)
{
    this->read(value);
    return *this;
}

ObjectMapper &ObjectMapper::operator+=(const QVariant &value)
{
    this->read(value);
    return *this;
}

QVariant ObjectMapper::toVariant() const
{
    QVariantHash __return;
    const auto &object_ignore_methods = QApiDoc::object_ignore_methods();
    auto &metaObject = *this->metaObject();
    for (int col = 0; col < metaObject.propertyCount(); ++col) {
        auto property = metaObject.property(col);

        if (object_ignore_methods.contains(property.name()))
            continue;

        const auto value = property.read(this);

        if (value.isNull() || !value.isValid())
            continue;

        switch (property.typeId()) {
        case QMetaType::Double:
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
        case QMetaType::User:
            if (value.toDouble() == 0)
                continue;
            break;
        case QMetaType::QUuid:
            if (value.toUuid().isNull())
                continue;
            break;
        case QMetaType::QUrl:
            if (value.toUrl().isEmpty())
                continue;
            break;
        case QMetaType::QDate:
            if (!value.toDate().isValid())
                continue;
            break;
        case QMetaType::QTime:
            if (!value.toTime().isValid())
                continue;
            break;
        case QMetaType::QDateTime:
            if (!value.toDateTime().isValid())
                continue;
            break;
        case QMetaType::QString:
        case QMetaType::QByteArray:
        case QMetaType::QChar:
        case QMetaType::QBitArray:
            if (value.toByteArray().trimmed().isEmpty())
                continue;
            break;
        case QMetaType::QVariantMap:
        case QMetaType::QVariantHash:
            if (value.toHash().isEmpty())
                continue;
            break;
        case QMetaType::QVariantList:
        case QMetaType::QStringList:
            if (value.toList().isEmpty())
                continue;
            break;
        default:
            break;
        }
        __return.insert(property.name(), value);
    }
    return __return.isEmpty() ? QVariant() : __return;
}

QVariantHash ObjectMapper::toHash() const
{
    return this->toVariant().toHash();
}

QVariantMap ObjectMapper::toMap() const
{
    return this->toVariant().toMap();
}

QByteArray ObjectMapper::toJson() const
{
    return QJsonDocument::fromVariant(this->toVariant()).toJson(QJsonDocument::Indented);
}

ObjectMapper &ObjectMapper::clear()
{
    const auto &object_ignore_methods = QApiDoc::object_ignore_methods();
    auto metaObject = this->metaObject();
    for (int col = 0; col < metaObject->propertyCount(); ++col) {
        auto property = metaObject->property(col);

        if (object_ignore_methods.contains(property.name()))
            continue;

        switch (property.typeId()) {
        case QMetaType::User:
            property.write(this, 0);
            continue;
        case QMetaType::QUuid:
            property.write(this, QUuid{});
            continue;
        case QMetaType::QUrl:
            property.write(this, QUrl());
            continue;
        case QMetaType::QDate:
            property.write(this, QDate());
            continue;
        case QMetaType::QTime:
            property.write(this, QTime());
            continue;
        case QMetaType::QDateTime:
            property.write(this, QDateTime());
            continue;
        case QMetaType::Double:
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            property.write(this, 0);
            continue;
        case QMetaType::QString:
            property.write(this, QString{});
            continue;
        case QMetaType::QByteArray:
            property.write(this, QByteArray{});
            continue;
        case QMetaType::QChar:
            property.write(this, QChar());
            continue;
        case QMetaType::QBitArray:
            property.write(this, QBitArray());
            continue;
        case QMetaType::QVariantMap:
            property.write(this, QVariantMap());
            continue;
        case QMetaType::QVariantHash:
            property.write(this, QVariantHash());
            continue;
        case QMetaType::QVariantList:
            property.write(this, QVariantList());
            continue;
        case QMetaType::QStringList:
            property.write(this, QStringList());
            continue;
        default:
            property.write(this, QVariant());
        }
    }
    return *this;
}

bool ObjectMapper::isEmpty()
{
    return !this->toVariant().isValid();
}

bool ObjectMapper::load(const QVariant &value)
{
    return this->clear().read(value);
}

bool ObjectMapper::read(const QVariant &value)
{
    const auto &object_ignore_methods = QApiDoc::object_ignore_methods();
    QVariantHash vHash;
    switch (value.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList: {
        auto vList = value.toList();
        return this->read(vList.isEmpty() ? QVariantHash{} : vList.first());
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap: {
        vHash = value.toHash();
        break;
    }
    case QMetaType::QString:
    case QMetaType::QByteArray: {
        QFile file(value.toString());
        if (file.exists()) {
            if (!file.open(file.ReadOnly)) {
                qWarning() << file.errorString();
                return {};
            }
            auto bytes = file.readAll();
            file.close();
            return this->read(QJsonDocument::fromJson(bytes).toVariant());
        }
        return this->read(QJsonDocument::fromJson(file.readAll()).toVariant());
    }
    default:
        return {};
    }

    if (vHash.isEmpty())
        return {};

    QHash<QString, QMetaProperty> propertList;
    auto &metaObject = *this->metaObject();
    for (int col = 0; col < metaObject.propertyCount(); ++col) {
        auto property = metaObject.property(col);
        if (object_ignore_methods.contains(property.name()))
            continue;

        propertList.insert(QByteArray{property.name()}.toLower(), property);
    }

    bool __return = false;
    QHashIterator<QString, QVariant> i(vHash);
    while (i.hasNext()) {
        i.next();
        const auto &v = i.value();
        if (v.isNull() || !v.isValid())
            continue;

        auto &p = propertList[i.key().toLower()];
        if(!p.isValid())
            continue;

        if (writeProperty(this, p, v))
            __return = true;
    }
    return __return;
}

bool ObjectMapper::load(QObject *object)
{
    this->clear();
    QVariantHash vHash;
    const auto &object_ignore_methods = QApiDoc::object_ignore_methods();
    auto &metaObject = *object->metaObject();
    for (int col = 0; col < metaObject.propertyCount(); ++col) {
        auto property = metaObject.property(col);

        if (object_ignore_methods.contains(property.name()))
            continue;

        vHash[QByteArray{property.name()}.toLower()] = property.read(object);
    }
    return this->read(vHash);
}

bool ObjectMapper::save(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(file.Unbuffered | file.Truncate | file.WriteOnly)) {
        qWarning() << file.errorString();
        return false;
    }
    file.write(this->toJson());
    file.flush();
    file.close();
    return true;
}

} // namespace QApiDoc

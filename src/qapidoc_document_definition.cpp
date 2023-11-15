#include "./qapidoc_document_definition.h"
#include <QVariantHash>

namespace QApiDoc {

class DefinitionPvt:public QObject
{
public:
    Definition *parent = nullptr;
    QString _name;
    QVariantHash _schema;
    explicit DefinitionPvt(Definition *parent):QObject{parent}, parent{parent} {}
};

Definition::Definition(QObject *parent) : ObjectMapper{parent}, p{new DefinitionPvt{this}}
{
}

const QString &Definition::name() const
{
    return p->_name;
}

Definition &Definition::name(const QString &newName)
{
    return this->setName(newName);
}

Definition &Definition::setName(const QString &newName)
{
    if (p->_name == newName)
        return *this;
    p->_name = newName;
    emit nameChanged();
    return *this;
}

const QVariantHash &Definition::schema() const
{
    return p->_schema;
}

Definition &Definition::schema(const QVariantHash &newSchema)
{
    return this->setSchema(newSchema);
}

Definition &Definition::setSchema(const QVariantHash &newSchema)
{
    if (p->_schema == newSchema)
        return *this;
    p->_schema = newSchema;
    emit schemaChanged();
    return *this;
}

} // namespace QApiDoc

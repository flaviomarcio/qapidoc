#include "./qapidoc_document_definition.h"
#include <QVariantHash>

namespace QApiDoc {

class DefinitionPvt:public QObject
{
public:
    Definition *parent = nullptr;
    QString _name;
    QVariantHash _schema;
    explicit DefinitionPvt(Definition *parent):QObject{parent} { this->parent = parent; }

    virtual ~DefinitionPvt() {}
};

Definition::Definition(QObject *parent) : ObjectMapper{parent}
{
    this->p = new DefinitionPvt{this};
}

Definition::~Definition()
{
    //delete this->p;
}

//QVariantHash Definition::toRefDefinition()const
//{
//
//    static const auto c_schemaRef = QStringLiteral("$ref");
//    static const auto c_PrefixDefinitionName = QStringLiteral("#/definitions/");
//    QVariantHash __return;
//    __return.insert(c_schemaRef, c_PrefixDefinitionName + p->_name);
//    return __return;
//}

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

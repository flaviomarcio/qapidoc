#include "./qapidoc_document_security_definition.h"
#include <QVariant>

namespace QApiDoc {

typedef QHash<int, const QMetaObject *> SecurityDefinitionFactoryMetaObject;

Q_GLOBAL_STATIC(SecurityDefinitionFactoryMetaObject, staticSecurityDefinitionMetaObject)

class SecurityDefinitionPvt:public QObject
{
public:
    SecurityDefinition *parent = nullptr;
    QString _description;
    QString _schemaNametion;

    explicit SecurityDefinitionPvt(SecurityDefinition *parent) : QObject{parent}
    {
        this->parent = parent;
    }
};

SecurityDefinition::SecurityDefinition(QObject *parent) : ObjectMapper{parent}
{
    this->p = new SecurityDefinitionPvt{this};
}

bool SecurityDefinition::registerSecurityDefinition(int securityDefinitionType,
                                                    const QMetaObject &metaObject)
{
    staticSecurityDefinitionMetaObject->insert(securityDefinitionType, &metaObject);
    return true;
}

SecurityDefinition *SecurityDefinition::newInstance(QObject *parent,
                                                    const SecurityDefinitionType securityType)
{
    if (!staticSecurityDefinitionMetaObject->contains(securityType))
        return nullptr;

    auto metaObject = staticSecurityDefinitionMetaObject->value(securityType);

    auto object = metaObject->newInstance(Q_ARG(QObject *, parent));
    if (object == nullptr)
        delete object;

    auto definition = dynamic_cast<SecurityDefinition *>(object);
    if (definition == nullptr)
        delete object;

    return definition;
}

const QString &SecurityDefinition::description() const
{
    
    return p->_description;
}

SecurityDefinition &SecurityDefinition::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

SecurityDefinition &SecurityDefinition::setDescription(const QString &newDescription)
{
    
    if (p->_description == newDescription)
        return *this;
    p->_description = newDescription;
    emit descriptionChanged();
    return *this;
}

SecurityDefinition &SecurityDefinition::resetDescription()
{
    return setDescription({});
}

QString SecurityDefinition::typeSecurityToString() const
{
    if (this->typeSecurity() >= qapi_SecurityDefinitionType.count())
        return {};
    return qapi_SecurityDefinitionType.at(this->typeSecurity());
}

const QString &SecurityDefinition::schemaName() const
{
    
    return p->_schemaNametion;
}

SecurityDefinition &SecurityDefinition::schemaName(const QString &newSchemaName)
{
    return this->setSchemaName(newSchemaName);
}

SecurityDefinition &SecurityDefinition::setSchemaName(const QString &newSchemaName)
{
    
    if (p->_schemaNametion == newSchemaName)
        return *this;
    p->_schemaNametion = newSchemaName;
    emit schemaNameChanged();
    return *this;
}

SecurityDefinition &SecurityDefinition::resetSchemaName()
{
    return setSchemaName({});
}

} // namespace QApiDoc

#include "./qapidoc_document_security_definition_basic.h"

namespace QApiDoc {

class SecurityDefinitionBasicPvt:public QObject
{
public:
    SecurityDefinitionBasic *parent = nullptr;
    QString _name;

    explicit SecurityDefinitionBasicPvt(SecurityDefinitionBasic *parent):QObject{parent}, parent{parent}{}

};

SecurityDefinitionBasic::SecurityDefinitionBasic(QObject *parent) : SecurityDefinition{parent}, p{new SecurityDefinitionBasicPvt{this}}
{
}

const QString &SecurityDefinitionBasic::name() const
{
    
    return p->_name;
}

SecurityDefinitionBasic &SecurityDefinitionBasic::name(const QString &newName)
{
    return this->setName(newName);
}

SecurityDefinitionBasic &SecurityDefinitionBasic::setName(const QString &newName)
{
    
    if (p->_name == newName)
        return *this;
    p->_name = newName;
    emit nameChanged();
    return *this;
}

SecurityDefinitionBasic &SecurityDefinitionBasic::resetName()
{
    return setName({});
}

SecurityDefinition::SecurityDefinitionType SecurityDefinitionBasic::typeSecurity() const
{
    return sstBasic;
}

} // namespace QApiDoc

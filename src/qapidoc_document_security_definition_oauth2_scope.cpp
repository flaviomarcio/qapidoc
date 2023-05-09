#include "./qapidoc_document_security_definition_oauth2_scope.h"
#include <QVariant>

namespace QApiDoc {

//#define dPvt() auto &p = *reinterpret_cast<SecurityDefinitionOAuth2ScopePvt *>(this->p)

class SecurityDefinitionOAuth2ScopePvt:public QObject
{
public:
    SecurityDefinitionOAuth2Scope *parent = nullptr;
    QString _description;
    QString _scopeName;
    explicit SecurityDefinitionOAuth2ScopePvt(SecurityDefinitionOAuth2Scope *parent):QObject{parent}
    {
        this->parent = parent;
    }

    virtual ~SecurityDefinitionOAuth2ScopePvt() {}
};

SecurityDefinitionOAuth2Scope::SecurityDefinitionOAuth2Scope(QObject *parent) : ObjectMapper{parent}
{
    this->p = new SecurityDefinitionOAuth2ScopePvt{this};
}

const QString &SecurityDefinitionOAuth2Scope::description() const
{
    
    return p->_description;
}

SecurityDefinitionOAuth2Scope &SecurityDefinitionOAuth2Scope::description(
    const QString &newDescription)
{
    return this->setDescription(newDescription);
}

SecurityDefinitionOAuth2Scope &SecurityDefinitionOAuth2Scope::setDescription(
    const QString &newDescription)
{
    
    if (p->_description == newDescription)
        return *this;
    p->_description = newDescription;
    emit descriptionChanged();
    return *this;
}

SecurityDefinitionOAuth2Scope &SecurityDefinitionOAuth2Scope::resetDescription()
{
    return setDescription({});
}

const QString &SecurityDefinitionOAuth2Scope::scopeName() const
{
    
    return p->_scopeName;
}

SecurityDefinitionOAuth2Scope &SecurityDefinitionOAuth2Scope::scopeName(const QString &newScopeName)
{
    return this->setScopeName(newScopeName);
}

SecurityDefinitionOAuth2Scope &SecurityDefinitionOAuth2Scope::setScopeName(
    const QString &newScopeName)
{
    
    if (p->_scopeName == newScopeName)
        return *this;
    p->_scopeName = newScopeName;
    emit scopeNameChanged();
    return *this;
}

SecurityDefinitionOAuth2Scope &SecurityDefinitionOAuth2Scope::resetScopeName()
{
    return setScopeName({});
}

} // namespace QApiDoc

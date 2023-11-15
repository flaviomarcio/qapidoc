#include "./qapidoc_document_security_definition_oauth2.h"
#include "./qapidoc_document_security_definition_oauth2_scope.h"

namespace QApiDoc {

class SecurityDefinitionOAuth2Pvt:public QObject
{
public:
    SecurityDefinitionOAuth2 *parent = nullptr;
    QString _name;
    QString _authorizationUrl;
    QString _flow;
    QList<SecurityDefinitionOAuth2Scope *> _scopes;
    explicit SecurityDefinitionOAuth2Pvt(SecurityDefinitionOAuth2 *parent):QObject{parent}, parent{parent}{}
};

SecurityDefinitionOAuth2::SecurityDefinitionOAuth2(QObject *parent) : SecurityDefinition{parent}, p{new SecurityDefinitionOAuth2Pvt{this}}
{
}

SecurityDefinition::SecurityDefinitionType SecurityDefinitionOAuth2::typeSecurity() const
{
    return sstOAuth2;
}

const QVariantHash SecurityDefinitionOAuth2::scopesObject() const
{
    
    QVariantHash __return;
    for (auto &item : p->_scopes)
        __return.insert(item->scopeName(), item->description());
    return __return;
}

QList<SecurityDefinitionOAuth2Scope *> &SecurityDefinitionOAuth2::scopes() const
{
    
    return p->_scopes;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::scopes(const QVariant &newScopes)
{
    return this->setScopes(newScopes);
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::setScopes(const QVariant &newScopes)
{
    
    QVariantList vList;
    switch (newScopes.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList: {
        qDeleteAll(p->_scopes);
        p->_scopes.clear();
        vList = newScopes.toList();
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap: {
        qDeleteAll(p->_scopes);
        p->_scopes.clear();
        vList = newScopes.toHash().values();
        break;
    }
    default:
        vList.append(newScopes);
    }

    for (auto &v : vList) {
        auto item = new SecurityDefinitionOAuth2Scope(this);
        if (!item->load(v)) {
            delete item;
            continue;
        }
        p->_scopes.append(item);
    }
    emit scopesChanged();
    return *this;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::setScopes(
    const QList<SecurityDefinitionOAuth2Scope *> &newScopes)
{
    
    auto aux = p->_scopes;
    p->_scopes.clear();
    for (auto &item : newScopes) {
        aux.removeOne(item);
        item->setParent(this);
        p->_scopes.append(item);
    }
    qDeleteAll(aux);
    emit scopesChanged();
    return *this;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::resetScopes()
{
    return setScopes(QVariantList{});
}

const QString &SecurityDefinitionOAuth2::flow() const
{
    
    return p->_flow;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::flow(const QString &newFlow)
{
    return this->setFlow(newFlow);
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::setFlow(const QString &newFlow)
{
    
    if (p->_flow == newFlow)
        return *this;
    p->_flow = newFlow;
    emit flowChanged();
    return *this;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::resetFlow()
{
    return setFlow({});
}

const QString &SecurityDefinitionOAuth2::authorizationUrl() const
{
    
    return p->_authorizationUrl;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::authorizationUrl(
    const QString &newAuthorizationUrl)
{
    return this->setAuthorizationUrl(newAuthorizationUrl);
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::setAuthorizationUrl(
    const QString &newAuthorizationUrl)
{
    
    if (p->_authorizationUrl == newAuthorizationUrl)
        return *this;
    p->_authorizationUrl = newAuthorizationUrl;
    emit authorizationUrlChanged();
    return *this;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::resetAuthorizationUrl()
{
    return setAuthorizationUrl({});
}

const QString &SecurityDefinitionOAuth2::name() const
{
    
    return p->_name;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::name(const QString &newName)
{
    return this->setName(newName);
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::setName(const QString &newName)
{
    
    if (p->_name == newName)
        return *this;
    p->_name = newName;
    emit nameChanged();
    return *this;
}

SecurityDefinitionOAuth2 &SecurityDefinitionOAuth2::resetName()
{
    return setName({});
}

} // namespace QApiDoc

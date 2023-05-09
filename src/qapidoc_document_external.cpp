#include "./qapidoc_document_external.h"

//#define dPvt() auto &p = *reinterpret_cast<DocumentExternalPvt *>(this->p)

namespace QApiDoc {

class DocumentExternalPvt:public QObject
{
public:
    DocumentExternal *parent = nullptr;
    QString _description;
    QString _url;

    explicit DocumentExternalPvt(DocumentExternal *parent):QObject{parent} { this->parent = parent; }

};

DocumentExternal::DocumentExternal(QObject *parent) : ObjectMapper{parent}
{
    this->p = new DocumentExternalPvt{this};
}

const QString &DocumentExternal::url() const
{  
    return p->_url;
}

DocumentExternal &DocumentExternal::url(const QString &newUrl)
{
    return this->setUrl(newUrl);
}

DocumentExternal &DocumentExternal::setUrl(const QString &newUrl)
{
    
    if (p->_url == newUrl)
        return *this;
    p->_url = newUrl;
    emit urlChanged();
    return *this;
}

DocumentExternal &DocumentExternal::resetUrl()
{
    return setUrl({});
}

const QString &DocumentExternal::description() const
{
    
    return p->_description;
}

DocumentExternal &DocumentExternal::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

DocumentExternal &DocumentExternal::setDescription(const QString &newDescription)
{
    
    if (p->_description == newDescription)
        return *this;
    p->_description = newDescription;
    emit descriptionChanged();
    return *this;
}

DocumentExternal &DocumentExternal::resetDescription()
{
    return setDescription({});
}

} // namespace QApiDoc

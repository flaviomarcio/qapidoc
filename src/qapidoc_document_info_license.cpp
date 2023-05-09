#include "./qapidoc_document_info_license.h"

namespace QApiDoc {

//#define dPvt() auto &p = *reinterpret_cast<InfoLicensePvt *>(this->p)

class InfoLicensePvt:public QObject
{
public:
    InfoLicense *parent = nullptr;
    QString _name;
    QString _url;

    explicit InfoLicensePvt(InfoLicense *parent):QObject{parent} { this->parent = parent; }
};

InfoLicense::InfoLicense(QObject *parent) : ObjectMapper{parent}
{
    this->p = new InfoLicensePvt{this};
}

const QString &InfoLicense::url() const
{
    
    return p->_url;
}

InfoLicense &InfoLicense::url(const QString &newUrl)
{
    this->setUrl(newUrl);
    return *this;
}

InfoLicense &InfoLicense::setUrl(const QString &newUrl)
{
    
    if (p->_url == newUrl)
        return *this;
    p->_url = newUrl;
    emit urlChanged();
    return *this;
}

InfoLicense &InfoLicense::resetUrl()
{
    return setUrl({});
}

const QString &InfoLicense::name() const
{
    
    return p->_name;
}

InfoLicense &InfoLicense::name(const QString &newName)
{
    this->setName(newName);
    return *this;
}

InfoLicense &InfoLicense::setName(const QString &newName)
{
    
    if (p->_name == newName)
        return *this;
    p->_name = newName;
    emit nameChanged();
    return *this;
}

InfoLicense &InfoLicense::resetName()
{
    return setName({});
}

} // namespace QApiDoc

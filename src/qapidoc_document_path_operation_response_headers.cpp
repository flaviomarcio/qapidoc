#include "./qapidoc_document_path_operation_response_headers.h"

namespace QApiDoc {

//#define dPvt() auto &p = *reinterpret_cast<HeadersPvt *>(this->p)

class HeadersPvt:public QObject
{
public:
    Headers *parent = nullptr;
    QString _description;
    QString _format;
    QString _name;
    QString _type;
    explicit HeadersPvt(Headers *parent):QObject{parent} { this->parent = parent; }
};

Headers::Headers(QObject *parent) : ObjectMapper{parent}
{
    this->p = new HeadersPvt{this};
}

const QString &Headers::name() const
{
    
    return p->_name;
}

Headers &Headers::name(const QString &newName)
{
    return this->setName(newName);
}

Headers &Headers::setName(const QString &newName)
{
    
    if (p->_name == newName)
        return *this;
    p->_name = newName;
    emit nameChanged();
    return *this;
}

Headers &Headers::resetName()
{
    return setName({});
}

const QString &Headers::description() const
{
    
    return p->_description;
}

Headers &Headers::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

Headers &Headers::setDescription(const QString &newDescription)
{
    
    if (p->_description == newDescription)
        return *this;
    p->_description = newDescription;
    emit descriptionChanged();
    return *this;
}

Headers &Headers::resetDescription()
{
    return setDescription({});
}

const QString &Headers::type() const
{
    
    return p->_type;
}

Headers &Headers::type(const QString &newType)
{
    return this->setType(newType);
}

Headers &Headers::setType(const QString &newType)
{
    
    if (p->_type == newType)
        return *this;
    p->_type = newType;
    emit typeChanged();
    return *this;
}

Headers &Headers::resetType()
{
    return setType({});
}

const QString &Headers::format() const
{
    
    return p->_format;
}

Headers &Headers::format(const QString &newFormat)
{
    return this->setFormat(newFormat);
}

Headers &Headers::setFormat(const QString &newFormat)
{
    
    if (p->_format == newFormat)
        return *this;
    p->_format = newFormat;
    emit formatChanged();
    return *this;
}

Headers &Headers::resetFormat()
{
    return setFormat({});
}

} // namespace QApiDoc

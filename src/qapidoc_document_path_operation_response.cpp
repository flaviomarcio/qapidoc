#include "./qapidoc_document_path_operation_response.h"
#include <QVariant>

namespace QApiDoc {

class ResponsePvt:public QObject
{
public:
    Response *parent = nullptr;
    Definition _schema;
    QString _description;
    QVariant _statusCode;
    QVariantHash _examples;
    QList<Headers *> _headers;

    explicit ResponsePvt(Response *parent):QObject{parent}, parent{parent} {}
};

Response::Response(QObject *parent) : ObjectMapper{parent}, p{new ResponsePvt{this}}
{
}

QString Response::statusCode() const
{
    
    return p->_statusCode.toString();
}

Response &Response::statusCode(const QVariant &newStatusCode)
{
    return this->setStatusCode(newStatusCode);
}

Response &Response::setStatusCode(const QVariant &newStatusCode)
{
    
    if (p->_statusCode == newStatusCode)
        return *this;
    p->_statusCode = newStatusCode.toString();
    emit statusCodeChanged();
    return *this;
}

Response &Response::resetStatusCode()
{
    return setStatusCode({});
}

Definition &Response::schema()
{
    
    return p->_schema;
}

QVariantHash Response::schemaObject()
{
    
    return p->_schema.toVariant().toHash();
}

Response &Response::setSchema(const QVariant &newSchema)
{
    
    p->_schema.load(newSchema);
    emit schemaChanged();
    return *this;
}

Response &Response::setSchema(const Definition &newSchema)
{
    
    p->_schema.load(newSchema.toVariant());
    emit schemaChanged();
    return *this;
}

Response &Response::resetSchema()
{
    
    p->_schema.clear();
    return *this;
}

QVariantList Response::headersObject() const
{
    
    QVariantList __return;
    for (auto &item : p->_headers) {
        __return << item->toVariant();
    }
    return __return;
}

QList<Headers *> &Response::headers() const
{
    
    return p->_headers;
}

Response &Response::headers(const QVariantList &newHeaders)
{
    return this->setHeaders(newHeaders);
}

Response &Response::setHeaders(const QVariantList &newHeaders)
{
    
    qDeleteAll(p->_headers);
    p->_headers.clear();
    for (auto &v : newHeaders) {
        auto item = new Headers();
        if (!item->load(v)) {
            delete item;
            continue;
        }
        p->_headers << item;
    }
    emit headersChanged();
    return *this;
}

Response &Response::setHeaders(const QList<Headers *> &newHeaders)
{
    
    auto aux = p->_headers;
    p->_headers.clear();
    for (auto &item : newHeaders) {
        aux.removeOne(item);
        p->_headers << item;
    }
    qDeleteAll(aux);
    emit headersChanged();
    return *this;
}

Response &Response::resetHeaders()
{
    return setHeaders(QVariantList{});
}

const QString &Response::description() const
{
    
    return p->_description;
}

Response &Response::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

Response &Response::setDescription(const QString &newDescription)
{
    
    if (p->_description == newDescription)
        return *this;
    p->_description = newDescription;
    emit descriptionChanged();
    return *this;
}

Response &Response::resetDescription()
{
    return setDescription({});
}

QVariantHash &Response::examples()
{
    
    return p->_examples;
}

Response &Response::examples(const QVariantHash &newExamples)
{
    return this->setExamples(newExamples);
}

Response &Response::setExamples(const QVariantHash &newExamples)
{
    
    if (p->_examples == newExamples)
        return *this;
    p->_examples = newExamples;
    emit examplesChanged();
    return *this;
}

Response &Response::resetExamples()
{
    return setExamples({});
}

const QVariantHash Response::toExamplestoHash() const
{
    
    int vExampleNumber = 0;
    QVariantHash __return;
    vExampleNumber = 0;
    QHashIterator<QString, QVariant> i(p->_examples);
    while (i.hasNext()) {
        i.next();
        __return.insert(QString::number(++vExampleNumber), i.value());
    }
    return __return;
}

} // namespace QApiDoc

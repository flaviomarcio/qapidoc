#include "./qapidoc_document_path_operation.h"
#include <QUuid>
#include <QVariant>
//#include "./qapidoc_document_path.h"
//#include "./qapidoc_meta_types.h"

namespace QApiDoc {

//#define dPvt() auto &p = *reinterpret_cast<PathOperationPvt *>(this->p)

class PathOperationPvt:public QObject
{
public:
    PathOperation *parent = nullptr;
    bool deprecated = false;
    DocumentExternal externalDocs;
    PathOperation::QApiPathTypeOperation operation = PathOperation::sptoNotDefined;
    QString description;
    QString operationId;
    QString summary;
    QStringList consumes;
    QStringList produces;
    QStringList security;
    QList<Tag *> tags;
    QHash<QString, Response *> responses;
    QList<Parameter *> parameters;

    explicit PathOperationPvt(PathOperation *parent):QObject{parent} { this->parent = parent; }
};

PathOperation::PathOperation(QObject *parent) : ObjectMapper{parent}
{
    this->p = new PathOperationPvt{this};
}

PathOperation::QApiPathTypeOperation PathOperation::operation() const
{

    return p->operation;
}

PathOperation &PathOperation::operation(const QVariant &newOperation)
{
    return this->setOperation(newOperation);
}

PathOperation &PathOperation::setOperation(const QVariant &newOperation)
{
    auto index = qapi_pathTypeOperationList().indexOf(newOperation.toString().toLower());
    if (index >= 0)
        p->operation = QApiPathTypeOperation(index);
    else
        p->operation = QApiPathTypeOperation(newOperation.toInt());
    emit operationChanged();
    return *this;
}

PathOperation &PathOperation::resetOperation()
{
    return setOperation({});
}

const QString &PathOperation::description() const
{

    return p->description;
}

PathOperation &PathOperation::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

PathOperation &PathOperation::setDescription(const QString &newDescription)
{
    if (p->description == newDescription)
        return *this;
    p->description = newDescription;
    emit descriptionChanged();
    return *this;
}

PathOperation &PathOperation::resetDescription()
{
    return setDescription({});
}

const QString &PathOperation::summary() const
{
    return p->summary;
}

PathOperation &PathOperation::setSummary(const QString &newSummary)
{
    if (p->summary == newSummary)
        return *this;
    p->summary = newSummary;
    emit summaryChanged();
    return *this;
}

PathOperation &PathOperation::resetSummary()
{
    return setSummary({});
}

bool PathOperation::deprecated() const
{
    return p->deprecated;
}

PathOperation &PathOperation::setDeprecated(bool newDeprecated)
{
    if (p->deprecated == newDeprecated)
        return *this;
    p->deprecated = newDeprecated;
    emit deprecatedChanged();
    return *this;
}

PathOperation &PathOperation::resetDeprecated()
{
    return setDeprecated({});
}

QStringList &PathOperation::consumes()
{
    return p->consumes;
}

PathOperation &PathOperation::consumes(const QString &newConsume)
{
    if (p->consumes.contains(newConsume))
        return *this;
    return this->setConsumes(QStringList(p->consumes) << newConsume);
}

PathOperation &PathOperation::consumes(const QStringList &newConsumes)
{
    return this->setConsumes(newConsumes);
}

PathOperation &PathOperation::setConsumes(const QStringList &newConsumes)
{
    if (p->consumes == newConsumes)
        return *this;
    p->consumes = newConsumes;
    emit consumesChanged();
    return *this;
}

PathOperation &PathOperation::resetConsumes()
{
    return setConsumes({});
}

QStringList &PathOperation::produces()
{
    return p->produces;
}

PathOperation &PathOperation::produces(const QString &newProduce)
{
    if (p->produces.contains(newProduce))
        return *this;
    return this->setProduces(QStringList(p->produces) << newProduce);
}

PathOperation &PathOperation::produces(const QVariant &newProduces)
{
    return this->setProduces(newProduces);
}

PathOperation &PathOperation::setProduces(const QVariant &newProduces)
{
    switch (newProduces.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList: {
        p->produces = newProduces.toStringList();
        break;
    }
    default:
        p->produces.append(newProduces.toString());
    }
    emit producesChanged();
    return *this;
}

PathOperation &PathOperation::resetProduces()
{
    return setProduces({});
}

QVariantList PathOperation::parametersObject() const
{
    QVariantList __return;
    for (auto &item : p->parameters)
        __return.append(item->toVariant());
    return __return;
}

QList<Parameter *> &PathOperation::parameters() const
{
    return p->parameters;
}

PathOperation &PathOperation::parameters(const QVariant &newParameter)
{
    return this->setParameters(newParameter);
}

PathOperation &PathOperation::parameters(const Parameter &newParameter)
{
    return this->parameters(newParameter.toVariant());
}

PathOperation &PathOperation::setParameters(const QVariant &newParameters)
{
    if (!newParameters.isValid())
        return *this;
    QVariantList vList;
    switch (newParameters.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList: {
        qDeleteAll(p->parameters);
        p->parameters.clear();
        vList = newParameters.toList();
        break;
    }
    default:
        vList.append(newParameters);
    }
    for (auto &v : vList) {
        auto item = new Parameter(this);
        if (!item->load(v)) {
            delete item;
            continue;
        }
        p->parameters.append(item);
    }
    emit parametersChanged();
    return *this;
}

PathOperation &PathOperation::setParameters(const QList<Parameter *> &newParameters)
{
    auto aux = p->parameters;
    p->parameters.clear();
    for (auto &item : newParameters) {
        aux.removeOne(item);
        item->setParent(this);
        p->parameters.append(item);
    }
    qDeleteAll(aux);
    emit parametersChanged();
    return *this;
}

PathOperation &PathOperation::resetParameters()
{
    return setParameters(QVariantList{});
}

QVariantHash PathOperation::responsesObject() const
{
    QVariantHash __return;
    for (auto &item : p->responses)
        __return.insert(item->statusCode(), item->toVariant());
    return __return;
}

QHash<QString, Response *> &PathOperation::responses() const
{
    return p->responses;
}

PathOperation &PathOperation::responses(const QVariant &newResponse)
{
    switch (newResponse.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash:
        return this->setResponses(newResponse.toList());
    default:
        auto item = new Response(this);
        if (!item->load(newResponse)) {
            delete item;
            return *this;
        }
        p->responses.insert(item->statusCode(), item);
        emit responsesChanged();
        return *this;
    }
}

PathOperation &PathOperation::responses(const Response &newResponse)
{
    return this->responses(newResponse.toVariant());
}

PathOperation &PathOperation::setResponses(const QVariant &newResponses)
{
    qDeleteAll(p->responses);
    p->responses.clear();
    switch (newResponses.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList: {
        qDeleteAll(p->responses);
        p->responses.clear();
        for (auto &v : newResponses.toList()) {
            auto item = new Response(this);
            if (!item->load(v)) {
                delete item;
                return *this;
            }
            p->responses.insert(item->statusCode(), item);
            emit responsesChanged();
        }
        break;
    }
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash: {
        qDeleteAll(p->responses);
        p->responses.clear();
        QHashIterator<QString, QVariant> i(newResponses.toHash());
        while (i.hasNext()) {
            i.next();
            auto item = new Response(this);
            if (!item->load(i.value())) {
                delete item;
                continue;
            }
            p->responses.insert(i.key(), item);
        }
        emit responsesChanged();
        break;
    }
    default:
        auto item = new Response(this);
        if (!item->load(newResponses)) {
            delete item;
            return *this;
        }
        p->responses.insert(item->statusCode(), item);
        emit responsesChanged();
    }
    return *this;
}

PathOperation &PathOperation::setResponses(const QList<Response *> &newResponses)
{
    auto aux = p->responses.values();
    p->responses.clear();
    for (auto &item : newResponses) {
        aux.removeOne(item);
        item->setParent(this);
        p->responses.insert(item->statusCode(), item);
    }
    qDeleteAll(aux);
    emit responsesChanged();
    return *this;
}

PathOperation &PathOperation::resetResponses()
{
    return setResponses(QVariantHash{});
}

QVariantList PathOperation::security() const
{
    QVariantList __return;
    for (auto &item : p->security)
        __return.append(QVariantHash{{item, QVariant()}});
    return __return;
}

PathOperation &PathOperation::security(const QString &newSecurity)
{
    if (newSecurity.contains(newSecurity))
        return *this;
    return this->setSecurity(p->security << newSecurity);
}

PathOperation &PathOperation::security(const QStringList &newSecurity)
{
    return this->setSecurity(newSecurity);
}

PathOperation &PathOperation::setSecurity(const QVariant &newSecurity)
{
    switch (newSecurity.typeId()) {
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash: {
        p->security = newSecurity.toHash().keys();
        break;
    }
    case QMetaType::QVariantList:
    case QMetaType::QStringList: {
        p->security = newSecurity.toStringList();
        break;
    }
    default:
        p->security.clear();
    }
    emit securityChanged();
    return *this;
}

PathOperation &PathOperation::resetSecurity()
{
    return setSecurity({});
}

QVariantList PathOperation::tagsObject() const
{
    QVariantList __return;
    for (auto &item : p->tags)
        __return.append(item->toVariant());
    return __return;
}

QList<Tag *> &PathOperation::tags() const
{
    return p->tags;
}

PathOperation &PathOperation::tags(const QVariant &newTag)
{
    return this->setTags(newTag);
}

PathOperation &PathOperation::setTags(const QVariant &newTags)
{
    switch (newTags.typeId()) {
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        return this->setTags(newTags.toList());
    default:
        auto item = new Tag(this);
        if (!item->load(newTags)) {
            delete item;
            return *this;
        }
        p->tags.append(item);
        emit tagsChanged();
        return *this;
    }
}

PathOperation &PathOperation::setTags(const QVariantList &newTags)
{
    qDeleteAll(p->tags);
    p->tags.clear();
    for (auto &v : newTags) {
        auto item = new Tag(this);
        if (!item->load(v)) {
            delete item;
            continue;
        }
        p->tags.append(item);
    }
    emit tagsChanged();
    return *this;
}

PathOperation &PathOperation::resetTags()
{
    return setTags(QVariantList{});
}

const QString &PathOperation::operationId() const
{

    return p->operationId;
}

PathOperation &PathOperation::operationId(const QVariant &newOperationId)
{
    return this->setOperationId(newOperationId);
}

PathOperation &PathOperation::setOperationId(const QVariant &newOperationId)
{
    switch (newOperationId.typeId()) {
    case QMetaType::QUuid:
        p->operationId = newOperationId.toUuid().toString();
        break;
    default:
        p->operationId = newOperationId.toString();
    }
    emit operationIdChanged();
    return *this;
}

PathOperation &PathOperation::resetOperationId()
{
    return setOperationId({});
}

QVariantList PathOperation::toMimeTypesList(const QStringList &vMimeTypesList) const
{
    QVariantList __return;
    for (const auto &item : vMimeTypesList)
        __return.append(item);
    return __return;
}

QString PathOperation::operationObject() const
{
    const auto &pathTypeOperationList = qapi_pathTypeOperationList();
    if (p->operation >= 0 && p->operation < pathTypeOperationList.count())
        return pathTypeOperationList.at(this->operation());
    return {};
}

DocumentExternal &PathOperation::externalDocs()
{
    return p->externalDocs;
}

QVariantHash PathOperation::externalDocsObject() const
{
    return p->externalDocs.toVariant().toHash();
}

PathOperation &PathOperation::externalDocs(const QVariant &newExternalDocs)
{
    return this->setExternalDocs(newExternalDocs);
}

PathOperation &PathOperation::setExternalDocs(const QVariant &newExternalDocs)
{
    p->externalDocs.load(newExternalDocs);
    emit externalDocsChanged();
    return *this;
}

PathOperation &PathOperation::setExternalDocs(const DocumentExternal &newExternalDocs)
{
    p->externalDocs.load(newExternalDocs.toVariant());
    emit externalDocsChanged();
    return *this;
}

PathOperation &PathOperation::resetExternalDocs()
{
    return setExternalDocs(QVariantList{});
}

} // namespace QApiDoc

#include "./qapidoc_document.h"
#include "./qapidoc.h"
#include "./qapidoc_types.h"
#include <QDir>
#include <QStringList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QStringList>

namespace QApiDoc {

class DocumentPvt
{
public:
    Document *parent = nullptr;
    DocumentExternal externalDocs;
    Info info;
    QStringList basePath;
    QString host = QStringLiteral("localhost");
    QVariantList consumes;
    QVariantList produces;
    QList<Definition *> definitions;
    QList<Parameter *> parameters;
    QList<Path *> paths;
    QList<SecurityDefinition *> securityDefinitions;
    QList<Tag *> tags;
    QApiTransferProtocolSchemes schemes;
    explicit DocumentPvt(Document *parent) { this->parent = parent; }

    virtual ~DocumentPvt() {}
};

Document::Document(QObject *parent) : ObjectMapper{parent}
{
    this->p = new DocumentPvt{this};
}

Document::~Document()
{
    delete p;
}

QByteArray Document::version() const
{
    return this->qapi_ger_version;
}

bool Document::load(QObject *object)
{
    if (!ObjectMapper::load(object))
        return false;

    auto document = this;

    QHash<QByteArray, QMetaMethod> vMethodHash;
    const auto &object_ignore_methods = QApiDoc::object_ignore_methods();
    auto &metaObject = *object->metaObject();
    for (int index = 0; index < metaObject.methodCount(); ++index) {
        auto method = metaObject.method(index);

        if (method.methodType() != method.Method)
            continue;

        if (object_ignore_methods.contains(method.name()))
            continue;

        if (!method.name().startsWith(Q_API_DOC_METHOD_START_NAME))
            continue;

        vMethodHash.insert(method.name(), method);
    }

    if (!vMethodHash.contains(Q_API_DOC_METHOD_NAME_INFORMATION))
        return {};

    { //document definition
        auto &method = vMethodHash[Q_API_DOC_METHOD_NAME_INFORMATION];
        if (!method.isValid())
            return {};

        vMethodHash.remove(method.name());

        if (!method.invoke(object, Qt::DirectConnection, Q_ARG(QApiDocument*, document))) {
            qWarning() << QStringLiteral("Invalid calling method: ") << method.name();
            return {};
        }
    }

    { //document information path
        //auto basePath=QStringLiteral("%1/%%1").arg(document->basePath()).simplified().toLower();
        QList<Path *> paths;
        QHashIterator<QByteArray, QMetaMethod> i(vMethodHash);
        while (i.hasNext()) {
            i.next();

            auto &method = i.value();

            if (!method.isValid())
                continue;

            if (!method.name().startsWith(Q_API_DOC_METHOD_NAME_PATH_STARTS_WITH))
                continue;

            QString methodName = method.name().simplified().split('_').last().toLower();
            auto path = new Path(document);
            path->setUri(QStringLiteral("/") + methodName);
            if (!method.invoke(object, Qt::DirectConnection, Q_ARG(Path*, path))) {
                qWarning() << QStringLiteral("Invalid invoke method: ") << method.name();
                delete path;
                continue;
            }
            paths << path;
        }
        document->setPaths(paths);
    }
    return true;
}

const QStringList &Document::basePath() const
{

    return p->basePath;
}

Document &Document::basePath(const QVariant &newBasePath)
{
    this->setBasePath(newBasePath);
    return *this;
}

Document &Document::setBasePath(const QVariant &newBasePath)
{

    if (p->basePath == newBasePath.toStringList())
        return *this;
    switch (newBasePath.typeId()) {
        case QMetaType::QStringList:
        case QMetaType::QVariantList:
        {
            for(auto &v:newBasePath.toList()){
                auto path=v.toString().trimmed().toLower();
                if(path.isEmpty())
                    continue;
                p->basePath.append(path);
            }
            break;
        }
        default:
            p->basePath.append(QStringLiteral("/"));
    }
    emit basePathChanged();
    return *this;
}

Document &Document::resetBasePath()
{
    return setBasePath({});
}

const QString &Document::host() const
{

    return p->host;
}

Document &Document::host(const QString &newHost)
{
    return this->setHost(newHost);
}

Document &Document::setHost(const QString &newHost)
{

    if (p->host == newHost)
        return *this;
    p->host = newHost;
    emit hostChanged();
    return *this;
}

Document &Document::resetHost()
{
    return setHost({});
}

const QVariantList Document::schemesObject() const
{

    QVariantList __return;
    for (auto &item : qapiTransferProtocolScheme()) {
        if (!p->schemes.testFlag(item))
            continue;
        __return << this->qapi_transfer_protocol_scheme.at(item);
    }
    return __return;
}

QApiTransferProtocolSchemes Document::schemes() const
{
    return p->schemes;
}

Document &Document::schemes(const QVariant &newSchemes)
{
    return this->setSchemes(newSchemes);
}

Document &Document::setSchemes(const QVariant &newSchemes)
{
    auto appendSchema = [this](const QVariant &v) {
        switch (v.typeId()) {
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
        case QMetaType::Double:
            p->schemes = p->schemes | QApiTransferProtocolSchemes(v.toInt());
            break;
        default:
            const auto &schemesHash = qapiTransferProtocolSchemeHash();
            if (!schemesHash.contains(v.toString()))
                break;
            auto scheme = schemesHash.value(v.toString());
            p->schemes = p->schemes | scheme;
        }
    };

    switch (newSchemes.typeId()) {
    case QMetaType::QStringList:
    case QMetaType::QVariantList: {
        p->schemes = {};
        for (auto &v : newSchemes.toList())
            appendSchema(v);
        break;
    }
    default:
        appendSchema(newSchemes);
    }
    return *this;
}

Document &Document::setSchemes(const QApiTransferProtocolSchemes &newSchemes)
{

    if (p->schemes == newSchemes)
        return *this;
    p->schemes = newSchemes;
    emit schemesChanged();
    return *this;
}

Document &Document::resetSchemes()
{
    return setSchemes(QVariantList{});
}

Info &Document::info()
{

    return p->info;
}

const QVariantHash Document::infoObject() const
{

    return p->info.toVariant().toHash();
}

Document &Document::setInfo(const QVariant &newInfo)
{

    p->info.load(newInfo);
    emit infoChanged();
    return *this;
}

Document &Document::setInfo(const Info &newInfo)
{

    p->info.load(newInfo.toVariant());
    emit infoChanged();
    return *this;
}

Document &Document::resetInfo()
{

    p->info.clear();
    return *this;
}

QList<Tag *> &Document::tags() const
{

    return p->tags;
}

Document &Document::tags(const QVariant &newTags)
{
    return this->setTags(newTags);
}

Document &Document::tags(const QVariantList &newTags)
{
    return this->setTags(newTags);
}

Document &Document::tags(Tag *newTags)
{
    return this->setTags(newTags);
}

Document &Document::tags(const QList<Tag *> &newTags)
{
    return this->setTags(newTags);
}

Document &Document::setTags(const QVariant &newTags)
{

    auto item = new Tag(this);
    if (!item->load(newTags)) {
        delete item;
        return *this;
    }
    p->tags << item;
    return *this;
}

const QVariantList Document::tagsObject() const
{

    QVariantList __return;
    for (auto &item : p->tags)
        __return << item->toVariant();
    return __return;
}

Document &Document::setTags(const QVariantList &newTags)
{

    qDeleteAll(p->tags);
    p->tags.clear();
    for (auto &v : newTags) {
        auto item = new Tag(this);
        if (!item->load(v)) {
            delete item;
            continue;
        }
        p->tags << item;
    }
    emit tagsChanged();
    return *this;
}

Document &Document::setTags(Tag *newTags)
{

    this->setParent(this);
    p->tags << newTags;
    return *this;
}

Document &Document::setTags(const QList<Tag *> &newTags)
{

    auto aux = p->tags;
    p->tags.clear();
    for (auto &item : newTags) {
        aux.removeOne(item);
        item->setParent(this);
        p->tags << item;
    }
    qDeleteAll(aux);
    emit tagsChanged();
    return *this;
}

Document &Document::resetTags()
{
    return setTags(QVariantList{});
}

const QVariantList &Document::consumes() const
{

    return p->consumes;
}

Document &Document::consumes(const QString &newConsume)
{

    if (!p->consumes.contains(newConsume)) {
        p->consumes << newConsume;
        emit producesChanged();
    }
    return *this;
}

Document &Document::consumes(const QVariantList &newConsumes)
{
    return this->setConsumes(newConsumes);
}

Document &Document::setConsumes(const QVariantList &newConsumes)
{

    p->consumes = newConsumes;
    emit consumesChanged();
    return *this;
}

Document &Document::resetConsumes()
{
    return setConsumes({});
}

const QVariantList &Document::produces() const
{

    return p->produces;
}

Document &Document::produces(const QString &newProduce)
{

    p->produces << newProduce;
    emit producesChanged();
    return *this;
}

Document &Document::produces(const QVariantList &newProduces)
{
    return this->setProduces(newProduces);
}

Document &Document::setProduces(const QVariantList &newProduces)
{

    if (p->produces == newProduces)
        return *this;
    p->produces = newProduces;
    emit producesChanged();
    return *this;
}

Document &Document::resetProduces()
{
    return setProduces({});
}

const QVariantList Document::pathsObject() const
{

    QVariantList __return;
    for (auto &item : p->paths)
        __return << item->toVariant();
    return __return;
}

QList<Path *> &Document::paths() const
{

    return p->paths;
}

Document &Document::paths(const QVariant &newPath)
{

    auto item = new Path(this);
    if (!item->load(newPath)) {
        delete item;
        return *this;
    }
    p->paths << item;
    emit securityDefinitionsChanged();
    return *this;
}

Document &Document::paths(const QVariantHash &newPaths)
{
    this->setPaths(newPaths);
    return *this;
}

Document &Document::paths(Path *newPaths)
{
    return this->setPaths(newPaths);
}

Document &Document::setPaths(const QVariant &newPaths)
{

    qDeleteAll(p->paths);
    p->paths.clear();
    switch (newPaths.typeId()) {
    case QMetaType::QVariantList: {
        for (auto &v : newPaths.toList()) {
            auto item = new Path(this);
            if (!item->load(v)) {
                delete item;
                continue;
            }
            p->paths.append(item);
        }
        emit pathsChanged();
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap: {
        QHashIterator<QString, QVariant> i(newPaths.toHash());
        while (i.hasNext()) {
            i.next();
            auto &v = i.value();
            auto item = new Path(this);
            if (!item->load(v)) {
                delete item;
                continue;
            }
            item->setUri(i.key());
            p->paths.append(item);
        }
        emit pathsChanged();
        break;
    }
    default:
        break;
    }

    return *this;
}

Document &Document::setPaths(Path *&newPaths)
{

    newPaths->setParent(this);
    if (!p->paths.contains(newPaths)) {
        p->paths << newPaths;
        emit pathsChanged();
    }
    return *this;
}

Document &Document::setPaths(const QList<Path *> &newPaths)
{

    auto aux = p->paths;
    p->paths.clear();
    for (auto &item : newPaths) {
        aux.removeOne(item);
        item->setParent(this);
        p->paths << item;
    }
    qDeleteAll(aux);
    emit pathsChanged();
    return *this;
}

Document &Document::resetPaths()
{
    return setPaths(QVariantHash{});
}

const QVariantList Document::definitionsObject() const
{

    QVariantList __return;
    for (auto &v : p->definitions)
        __return << v->toVariant();
    return __return;
}

QList<Definition *> &Document::definitions() const
{

    return p->definitions;
}

Document &Document::definitions(const QVariant &newDefinition)
{

    auto item = new Definition(this);
    if (!item->load(newDefinition)) {
        delete item;
        return *this;
    }
    p->definitions << item;
    emit securityDefinitionsChanged();
    return *this;
}

Document &Document::definitions(const QVariantHash &newDefinitions)
{
    return this->setDefinitions(newDefinitions);
}

Document &Document::definitions(Definition *newDefinitions)
{
    return this->setDefinitions(newDefinitions);
}

Document &Document::definitions(const QList<Definition *> &newDefinitions)
{
    this->setDefinitions(newDefinitions);
    return *this;
}

Document &Document::setDefinitions(const QVariant &newDefinitions)
{

    qDeleteAll(p->paths);
    p->paths.clear();
    switch (newDefinitions.typeId()) {
    case QMetaType::QVariantList: {
        for (auto &v : newDefinitions.toList()) {
            auto item = new Definition(this);
            if (!item->load(v)) {
                delete item;
                continue;
            }
            p->definitions << item;
        }
        emit definitionsChanged();
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap: {
        QHashIterator<QString, QVariant> i(newDefinitions.toHash());
        while (i.hasNext()) {
            i.next();
            auto item = new Definition(this);
            item->setName(i.key()).setSchema(i.value().toHash());
            p->definitions << item;
        }
        emit definitionsChanged();
        break;
    }
    default:
        break;
    }
    return *this;
}

Document &Document::setDefinitions(Definition *newDefinitions)
{

    newDefinitions->setParent(this);
    p->definitions << newDefinitions;
    return *this;
}

Document &Document::setDefinitions(const QList<Definition *> &newDefinitions)
{

    auto aux = p->definitions;
    p->definitions.clear();
    for (auto &item : newDefinitions) {
        aux.removeOne(item);
        item->setParent(this);
        p->definitions << item;
    }
    qDeleteAll(aux);
    emit definitionsChanged();
    return *this;
}

Document &Document::resetDefinitions()
{
    return setDefinitions(QVariantHash{});
}

const QVariantList Document::securityDefinitionsObject() const
{

    QVariantList __return;
    for (auto &v : p->securityDefinitions)
        __return << v->toVariant();
    return __return;
}

QList<SecurityDefinition *> &Document::securityDefinitions()
{

    return p->securityDefinitions;
}

Document &Document::securityDefinitions(const QVariant &newSecurityDefinition)
{

    auto item = new SecurityDefinition(this);
    if (!item->load(newSecurityDefinition)) {
        delete item;
        return *this;
    }
    p->securityDefinitions << item;
    emit securityDefinitionsChanged();
    return *this;
}

Document &Document::securityDefinitions(const QVariantHash &newSecurityDefinitions)
{
    this->setSecurityDefinitions(newSecurityDefinitions);
    return *this;
}

Document &Document::securityDefinitions(const QList<SecurityDefinition *> &newSecurityDefinitions)
{
    this->setSecurityDefinitions(newSecurityDefinitions);
    return *this;
}

Document &Document::setSecurityDefinitions(const QVariant &newSecurityDefinitions)
{

    qDeleteAll(p->securityDefinitions);
    p->securityDefinitions.clear();

    switch (newSecurityDefinitions.typeId()) {
    case QMetaType::QVariantList: {
        for (auto &v : newSecurityDefinitions.toList()) {
            auto item = new SecurityDefinition(this);
            if (!item->load(v)) {
                delete item;
                continue;
            }
            p->securityDefinitions << item;
        }
        emit securityDefinitionsChanged();
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap: {
        QHashIterator<QString, QVariant> i(newSecurityDefinitions.toHash());
        while (i.hasNext()) {
            i.next();
            auto item = new SecurityDefinition(this);
            if (!item->load(i.value())) {
                delete item;
                continue;
            }
            p->securityDefinitions << item;
        }
        emit securityDefinitionsChanged();
        break;
    }
    default:
        break;
    }
    return *this;
}

Document &Document::setSecurityDefinitions(const QList<SecurityDefinition *> &newSecurityDefinitions)
{

    auto aux = p->securityDefinitions;
    p->securityDefinitions.clear();
    for (auto &item : newSecurityDefinitions) {
        aux.removeOne(item);
        item->setParent(this);
        p->securityDefinitions << item;
    }
    qDeleteAll(aux);
    emit securityDefinitionsChanged();
    return *this;
    /*
    const QString qapi_ExternalDocs = QStringLiteral("externalDocs");
    const QString qapi_ExternalDocsDescription = QStringLiteral("description");
    const QString qapi_SecurityDefinitions = QStringLiteral("securityDefinitions");
    const QString qapi_SecurityDefinitionsType = QStringLiteral("type");

    auto vSecurityDefinitionsHash = vSwaggerObject.value(this->qapi_SecurityDefinitions).toHash();
    QHashIterator <QString, QVariant> i(vSecurityDefinitionsHash);
    while (i.hasNext()){
        i.next();
        auto vSecurityDefinitionItemHash = i.value().toHash();
        if (
            vSecurityDefinitionItemHash.isEmpty()
            ||
            !vSecurityDefinitionItemHash.contains(this->qapi_SecurityDefinitionsType)
            ||
            vSecurityDefinitionItemHash.contains(this->qapi_SecurityDefinitionsType)
            ){
            continue;//TODO TRETA NO IF REVER CODIGO
        }

        auto vSecurityDefinitionType=SecurityDefinitionType(vSecurityDefinitionItemHash.value(this->qapi_SecurityDefinitionsType).toInt());
        auto vSecurityDefinition = SecurityDefinition::newInstance(this, vSecurityDefinitionType);
        if (vSecurityDefinition!=nullptr)
            this->securityDefinitions(i.value());
    }
    */
}

Document &Document::resetSecurityDefinitions()
{
    setSecurityDefinitions(QVariantHash{});
    emit securityDefinitionsChanged();
    return *this;
}

const QVariantList Document::parametersObject() const
{

    QVariantList __return;
    for (auto &v : p->parameters)
        __return << v->toVariant();
    return __return;
}

QList<Parameter *> &Document::parameters() const
{

    return p->parameters;
}

Document &Document::parameters(const QVariant &newParameter)
{

    auto item = new Parameter(this);
    if (!item->load(newParameter)) {
        delete item;
        return *this;
    }
    p->parameters << item;
    emit parametersChanged();
    return *this;
}

Document &Document::parameters(const QVariantList &newParameters)
{
    return this->setParameters(newParameters);
}

Document &Document::parameters(const QList<Parameter *> &newParameters)
{
    this->setParameters(newParameters);
    return *this;
}

Document &Document::setParameters(const QVariantList &newParameters)
{

    qDeleteAll(p->parameters);
    p->parameters.clear();
    for (auto &v : newParameters)
        this->parameters(v);
    return *this;
}

Document &Document::setParameters(const QList<Parameter *> &newParameters)
{

    auto aux = p->parameters;
    p->parameters.clear();
    for (auto &item : newParameters) {
        aux.removeOne(item);
        this->setParent(this);
        p->parameters << item;
    }
    qDeleteAll(aux);
    emit parametersChanged();
    return *this;
}

Document &Document::resetParameters()
{
    return setParameters(QVariantList{});
}

DocumentExternal &Document::externalDocs()
{

    return p->externalDocs;
}

const QVariantHash Document::externalDocsObject() const
{

    return p->externalDocs.toVariant().toHash();
}

Document &Document::externalDocs(const QVariant &newExternalDocs)
{
    return this->setExternalDocs(newExternalDocs);
}

Document &Document::setExternalDocs(const QVariant &newExternalDocs)
{

    p->externalDocs.load(newExternalDocs);
    emit externalDocsChanged();
    return *this;
}

Document &Document::setExternalDocs(const DocumentExternal &newExternalDocs)
{

    p->externalDocs.load(newExternalDocs.toVariant());
    emit externalDocsChanged();
    return *this;
}

Document &Document::resetExternalDocs()
{

    p->externalDocs.clear();
    return *this;
}

} // namespace QApiDoc

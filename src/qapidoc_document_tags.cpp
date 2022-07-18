#include "./qapidoc_document_tags.h"

namespace QApiDoc {

class TagPvt
{
public:
    Tag *parent = nullptr;
    DocumentExternal externalDocs;
    QString description;
    QString name;

    explicit TagPvt(Tag *parent) { this->parent = parent; }

    virtual ~TagPvt() {}
};

Tag::Tag(QObject *parent) : ObjectMapper{parent}
{
    this->p = new TagPvt{this};
}

Tag::~Tag()
{
    delete p;
}

const QString &Tag::description() const
{

    return p->description;
}

Tag &Tag::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

Tag &Tag::setDescription(const QString &newDescription)
{

    if (p->description == newDescription)
        return *this;
    p->description = newDescription;
    emit descriptionChanged();
    return *this;
}

Tag &Tag::resetDescription()
{
    return setDescription({});
}

const DocumentExternal &Tag::externalDocs() const
{

    return p->externalDocs;
}

QVariantHash Tag::externalDocsObject() const
{

    return p->externalDocs.toVariant().toHash();
}

Tag &Tag::externalDocs(const QVariant &newExternalDocs)
{
    return this->setExternalDocs(newExternalDocs);
}

Tag &Tag::setExternalDocs(const QVariant &newExternalDocs)
{

    p->externalDocs.load(newExternalDocs);
    emit externalDocsChanged();
    return *this;
}

Tag &Tag::setExternalDocs(const DocumentExternal &newExternalDocs)
{

    p->externalDocs.load(newExternalDocs.toVariant());
    emit externalDocsChanged();
    return *this;
}

Tag &Tag::resetExternalDocs()
{
    return setExternalDocs(QVariantList{});
}

const QString &Tag::name() const
{

    return p->name;
}

Tag &Tag::name(const QString &newName)
{
    return this->setName(newName);
}

Tag &Tag::setName(const QString &newName)
{

    if (p->name == newName)
        return *this;
    p->name = newName;
    emit nameChanged();
    return *this;
}

Tag &Tag::resetName()
{
    return setName({});
}

} // namespace QApiDoc

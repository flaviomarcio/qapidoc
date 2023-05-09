#pragma once

#include "./qapidoc_object_mapper.h"

namespace QApiDoc {
class DocumentExternalPvt;
//!
//! \brief The DocumentExternal class
//!
class Q_API_DOC_EXPORT DocumentExternal : public ObjectMapper
{
    Q_OBJECT

    //!
    Q_PROPERTY(QString description READ description WRITE setDescription RESET resetDescription
                   NOTIFY descriptionChanged)

    //!
    Q_PROPERTY(QString url READ url WRITE setUrl RESET resetUrl NOTIFY urlChanged)

public:
    Q_INVOKABLE explicit DocumentExternal(QObject *parent = nullptr);

    //!
    //! \brief description
    //! \return
    //!
    const QString &description() const;
    DocumentExternal &description(const QString &newDescription);
    DocumentExternal &setDescription(const QString &newDescription);
    DocumentExternal &resetDescription();

    //!
    //! \brief url
    //! \return
    //!
    const QString &url() const;
    DocumentExternal &url(const QString &newUrl);
    DocumentExternal &setUrl(const QString &newUrl);
    DocumentExternal &resetUrl();

signals:
    void descriptionChanged();
    void urlChanged();
private:
    DocumentExternalPvt *p = nullptr;
};

} // namespace QApiDoc

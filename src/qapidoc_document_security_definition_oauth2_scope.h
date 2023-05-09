#pragma once

#include "./qapidoc_object_mapper.h"

namespace QApiDoc {
class SecurityDefinitionOAuth2ScopePvt;
//!
//! \brief The SecurityDefinitionOAuth2Scope class
//!
class Q_API_DOC_EXPORT SecurityDefinitionOAuth2Scope : public ObjectMapper
{
    Q_OBJECT

    //!
    Q_PROPERTY(QString scopeName READ scopeName WRITE setScopeName RESET resetScopeName NOTIFY
                   scopeNameChanged)

    //!
    Q_PROPERTY(QString description READ description WRITE setDescription RESET resetDescription
                   NOTIFY descriptionChanged)

public:
    Q_INVOKABLE explicit SecurityDefinitionOAuth2Scope(QObject *parent = nullptr);

    //!
    //! \brief scopeName
    //! \return
    //!
    const QString &scopeName() const;
    SecurityDefinitionOAuth2Scope &scopeName(const QString &newScopeName);
    SecurityDefinitionOAuth2Scope &setScopeName(const QString &newScopeName);
    SecurityDefinitionOAuth2Scope &resetScopeName();

    //!
    //! \brief description
    //! \return
    //!
    const QString &description() const;
    SecurityDefinitionOAuth2Scope &description(const QString &newDescription);
    SecurityDefinitionOAuth2Scope &setDescription(const QString &newDescription);
    SecurityDefinitionOAuth2Scope &resetDescription();

signals:
    //!
    //! \brief scopeNameChanged
    //!
    void scopeNameChanged();

    //!
    //! \brief descriptionChanged
    //!
    void descriptionChanged();
private:
    SecurityDefinitionOAuth2ScopePvt *p = nullptr;
};

} // namespace QApiDoc

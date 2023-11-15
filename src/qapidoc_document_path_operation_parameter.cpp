#include "./qapidoc_document_path_operation_parameter.h"
#include <QVariant>

static const QStringList &__typeParameter()
{
    return qapi_TypeParameter();
}

namespace QApiDoc {

class ParameterPvt:public QObject
{
public:
    Parameter *parent = nullptr;
    bool _allowEmptyValue = false;
    bool _requiredValue = false;
    Definition _schema;
    QString _defaultValuehema;
    QString _descriptionehema;
    QString _format;
    QString _name;
    QString _pattern;
    QString _ref;
    QStringList _enumValuef;
    QVariantHash _items;
    Parameter::QApiParameterInLocation _inLocation = Parameter::srplPath;
    Parameter::QApiTypeParameter _typeParameter;

    explicit ParameterPvt(Parameter *parent):QObject{parent}, parent{parent} {}
};

Parameter::Parameter(QObject *parent) : ObjectMapper{parent}, p{new ParameterPvt{this}}
{
}

Parameter::QApiParameterInLocation Parameter::inLocation() const
{   
    return p->_inLocation;
}

Parameter &Parameter::inLocation(const QVariant &newInLocation)
{
    return this->setInLocation(newInLocation);
}

Parameter &Parameter::setInLocation(const QVariant &newInLocation)
{  
    if (p->_inLocation == newInLocation)
        return *this;
    auto index = qapi_requestParameterInLocation().indexOf(newInLocation.toString().toLower());
    if (index >= 0)
        p->_inLocation = QApiParameterInLocation(index);
    else
        p->_inLocation = QApiParameterInLocation(newInLocation.toInt());
    emit inLocationChanged();
    return *this;
}

Parameter &Parameter::resetInLocation()
{
    return setInLocation({});
}

QString Parameter::inLocationToString() const
{   
    return qapi_requestParameterInLocation().at(p->_inLocation);
}

bool Parameter::required() const
{   
    return p->_requiredValue;
}

Parameter &Parameter::required(bool newRequired)
{
    return this->setRequired(newRequired);
}

Parameter &Parameter::setRequired(bool newRequired)
{   
    if (p->_requiredValue == newRequired)
        return *this;
    p->_requiredValue = newRequired;
    emit requiredChanged();
    return *this;
}

Parameter &Parameter::resetRequired()
{
    return setRequired({});
}

Definition &Parameter::schema()
{   
    return p->_schema;
}

QVariantHash Parameter::schemaObject()
{   
    return p->_schema.toVariant().toHash();
}

Parameter &Parameter::schema(const QVariant &newSchema)
{
    return this->setSchema(newSchema);
}

Parameter &Parameter::setSchema(const QVariant &newSchema)
{   
    p->_schema.load(newSchema);
    emit schemaChanged();
    return *this;
}

Parameter &Parameter::setSchema(const Definition &newSchema)
{   
    p->_schema.load(newSchema.toVariant());
    emit schemaChanged();
    return *this;
}

Parameter &Parameter::resetSchema()
{
    setSchema(QVariant{});
    return *this;
}

const QString &Parameter::description() const
{
    return p->_descriptionehema;
}

Parameter &Parameter::description(const QString &newDescription)
{
    return this->setDescription(newDescription);
}

Parameter &Parameter::setDescription(const QString &newDescription)
{
    
    if (p->_descriptionehema == newDescription)
        return *this;
    p->_descriptionehema = newDescription;
    emit descriptionChanged();
    return *this;
}

Parameter &Parameter::resetDescription()
{
    return setDescription({});
}

Parameter::QApiTypeParameter Parameter::typeParameter() const
{   
    return p->_typeParameter;
}

Parameter &Parameter::type(const int &newTypeParameter)
{
    return this->setTypeParameter(newTypeParameter);
}

Parameter &Parameter::typeParameter(const int &newTypeParameter)
{
    return this->setTypeParameter(newTypeParameter);
}

Parameter &Parameter::setTypeParameter(const QVariant &newTypeParameter)
{
    if (p->_typeParameter == newTypeParameter)
        return *this;
    auto index = __typeParameter().indexOf(newTypeParameter.toString().toLower());
    if (index >= 0)
        p->_typeParameter = QApiTypeParameter(index);
    else
        p->_typeParameter = QApiTypeParameter(newTypeParameter.toInt());
    emit typeParameterChanged();
    return *this;
}

Parameter &Parameter::resetTypeParameter()
{
    return setTypeParameter({});
}

const QString &Parameter::pattern() const
{
    return p->_pattern;
}

Parameter &Parameter::pattern(const QString &newPattern)
{
    return this->setPattern(newPattern);
}

Parameter &Parameter::setPattern(const QString &newPattern)
{
    if (p->_pattern == newPattern)
        return *this;
    p->_pattern = newPattern;
    emit patternChanged();
    return *this;
}

Parameter &Parameter::resetPattern()
{
    return setPattern({});
}

const QVariantHash &Parameter::items() const
{
    return p->_items;
}

Parameter &Parameter::setItems(const QVariantHash &newItems)
{
    if (p->_items == newItems)
        return *this;
    p->_items = newItems;
    emit itemsChanged();
    return *this;
}

Parameter &Parameter::resetItems()
{
    return setItems({});
}

const QString &Parameter::format() const
{
    return p->_format;
}

Parameter &Parameter::format(const QString &newFormat)
{
    return this->setFormat(newFormat);
}

Parameter &Parameter::setFormat(const QString &newFormat)
{
    if (p->_format == newFormat)
        return *this;
    p->_format = newFormat;
    emit formatChanged();
    return *this;
}

Parameter &Parameter::resetFormat()
{
    return setFormat({});
}

const QString &Parameter::defaultValue() const
{
    return p->_defaultValuehema;
}

Parameter &Parameter::defaultValue(const QString &newDefault)
{
    return this->setDefaultValue(newDefault);
}

Parameter &Parameter::setDefaultValue(const QString &newDefault)
{
    if (p->_defaultValuehema == newDefault)
        return *this;
    p->_defaultValuehema = newDefault;
    emit defaultValueChanged();
    return *this;
}

Parameter &Parameter::resetDefaultValue()
{
    return setDefaultValue({});
}

QStringList &Parameter::enumValue()
{
    return p->_enumValuef;
}

Parameter &Parameter::enumValue(const QStringList &newEnum)
{
    return this->setEnumValue(newEnum);
}

Parameter &Parameter::setEnumValue(const QStringList &newEnum)
{
    p->_enumValuef = newEnum;
    emit enumValueChanged();
    return *this;
}

Parameter &Parameter::resetEnumValue()
{
    return setEnumValue({});
}

bool Parameter::allowEmptyValue() const
{
    return p->_allowEmptyValue;
}

Parameter &Parameter::allowEmptyValue(bool newAllowEmptyValue)
{
    return this->setAllowEmptyValue(newAllowEmptyValue);
}

Parameter &Parameter::setAllowEmptyValue(bool newAllowEmptyValue)
{   
    if (p->_allowEmptyValue == newAllowEmptyValue)
        return *this;
    p->_allowEmptyValue = newAllowEmptyValue;
    emit allowEmptyValueChanged();
    return *this;
}

Parameter &Parameter::resetAllowEmptyValue()
{
    return setAllowEmptyValue({});
}

const QString &Parameter::ref() const
{
    return p->_ref;
}

Parameter &Parameter::ref(const QString &newRef)
{
    return this->setRef(newRef);
}

Parameter &Parameter::setRef(const QString &newRef)
{   
    if (p->_ref == newRef)
        return *this;
    p->_ref = newRef;
    emit refChanged();
    return *this;
}

Parameter &Parameter::resetRef()
{
    return setRef({});
}

const QString &Parameter::name() const
{
    return p->_name;
}

Parameter &Parameter::name(const QString &newName)
{
    return this->setName(newName);
}

Parameter &Parameter::setName(const QString &newName)
{
    if (p->_name == newName)
        return *this;
    p->_name = newName;
    emit nameChanged();
    return *this;
}

Parameter &Parameter::resetName()
{
    return setName({});
}

} // namespace QApiDoc

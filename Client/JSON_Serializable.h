#pragma once

#include <QVector>
#include <QSharedPointer>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>

/**
 * If you subclass from this, then we have clean methods of converting to/from JSON.
 */
class JSON_Serializable {
public:
    JSON_Serializable();
    virtual ~JSON_Serializable();

    /** Implement this method to populate ourself from this json. */
    virtual void fromJSON(const QJsonObject & json) = 0;

    /** Implement this method to populate this json from ourself. */
    virtual void toJSON(QJsonObject & json) const = 0;

    QString toString() const;

    static QString stringValue(const QJsonObject &propertyTree, const char * key);
    static int intValue(const QJsonObject &propertyTree, const char * key);
    static double doubleValue(const QJsonObject &propertyTree, const char * key);
    static bool boolValue(const QJsonObject &propertyTree, const char * key);
    static QJsonValue jsonValue(const QJsonObject &propertyTree, const char * key);

    static void setStringValue(QJsonObject &json, const char * key, const QString & value);
    static void setIntValue(QJsonObject &json, const char * key, const int value);
};

/**
 * Read vector from this JSON.
 */
template <class ObjectType>
void fromJSON(QVector<QSharedPointer<ObjectType>> & vec, const QJsonArray &array) {
    for (auto iter = array.begin(); iter != array.end(); ++iter) {
        auto obj = *iter;

        QSharedPointer<ObjectType> thisDiff = QSharedPointer<ObjectType>::create();
        thisDiff->fromJSON(obj.toObject());
        vec.push_back(thisDiff);
    }
}

/**
 * Write this vector to this json.
 */
template <class ObjectType>
void toJSON(const QVector<QSharedPointer<ObjectType>> & vec, QJsonArray &array) {
    for (const QSharedPointer<ObjectType> & obj: vec) {
        QJsonObject childJson;
        obj->toJSON(childJson);
        array.push_back(childJson);
    }
}

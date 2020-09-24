#include <QJsonDocument>

#include "JSON_Serializable.h"

/**
 * Constructor.
 */
JSON_Serializable::JSON_Serializable() {
}

/**
 * Destructor.
 */
JSON_Serializable::~JSON_Serializable() {
}

/**
 * Convert the object to a string.
 */
QString
JSON_Serializable::toString() const {
    QJsonObject json;
    toJSON(json);
    QJsonDocument doc(json);
    QString str = doc.toJson(QJsonDocument::Indented);
    return str;
}

/**
 * Get this key's value as a string.
 *
 * @returns The object in string representation or an empty string if the key isn't found.
 */
QString
JSON_Serializable::stringValue(const QJsonObject &propertyTree, const char * key)
{
	auto it = propertyTree.find(key);
	if (it != propertyTree.end()) {
        QJsonValue value = *it;
        if (!value.isNull()) {
            return value.toString();
		}
	}
	return "";
}

/**
 * Get this key's value as an int. May throw an exception if the key is found but
 * it's a non-integer value.
 *
 * @returns the value or 0 if not found.
 */
int
JSON_Serializable::intValue(const QJsonObject &propertyTree, const char * key)
{
	auto it = propertyTree.find(key);
	if (it != propertyTree.end()) {
        QJsonValue value = *it;
        if (!value.isNull()) {
            return value.toInt();
		}
	}
	return 0;
}

/**
 * Get this key's value as a double. May throw an exception if the key is found but
 * it's a non-floating value.
 *
 * @returns the value or 0.0 if not found.
 */
double
JSON_Serializable::doubleValue(const QJsonObject &propertyTree, const char * key)
{
	auto it = propertyTree.find(key);
	if (it != propertyTree.end()) {
        QJsonValue value = *it;
        if (!value.isNull()) {
            return value.toDouble();
		}
	}
    return 0.0;
}

/**
 * Get this key's value as a boolean. May throw an exception if the key is found but
 * it's a non-boolean value.
 *
 * @returns the value or false if not found.
 */
bool
JSON_Serializable::boolValue(const QJsonObject &propertyTree, const char * key)
{
	auto it = propertyTree.find(key);
	if (it != propertyTree.end()) {
        QJsonValue value = *it;
        if (!value.isNull()) {
            return value.toBool();
		}
	}
	return false;
}

/**
 * Find this key and return the corresponding JSON object.
 *
 * @returns the value or an empty JSON object.
 */
QJsonValue
JSON_Serializable::jsonValue(const QJsonObject &propertyTree, const char * key) {
	auto it = propertyTree.find(key);
	if (it != propertyTree.end()) {
		return *it;
	}

    QJsonValue value;
    return value;
}

/**
 * Sets a string value in this JSON only if we have content.
 */
void
JSON_Serializable::setStringValue(QJsonObject &obj, const char * key, const QString &value) {
    if (value.length() > 0) {
        obj[key] = value;
    }
}

/**
 * Sets a long value in this JSON only if we have content -- value != 0. If you want to store
 * zeroes anyway, don't use this method.
 */
void
JSON_Serializable::setIntValue(QJsonObject &obj, const char * key, int value) {
    if (value != 0) {
        obj[key] = value;
    }
}

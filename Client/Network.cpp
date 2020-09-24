
#include <QtCore>
#include <QCryptographicHash>

#include "Network.h"

QNetworkAccessManager manager;

Network::Network()
{
}

Network::~Network()
{
}

/**
 * Set the header Authorization: Basic <hash>.
 */
void
Network::setAuthorization(QNetworkRequest &request, const QString &username, const QString &pw) {
    QString concatenated = username + ":" + pw;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
}


/**
 * Make a ping call.
 */
void
Network::ping() {
    QNetworkRequest req(QString("/demo/ping"));
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    QNetworkReply * reply = manager.get(req);

    QObject::connect(reply, &QNetworkReply::finished, this, &Network::gotPing);
}

void
Network::gotPing() {
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

    emit pingCompleted(reply);
}

/**
 * Retrieve all event templates. Emits gotEventTemplates when done.
 */
void
Network::getAllEventTemplates() {
    QNetworkRequest req(QString("/demo/event_templates"));
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    setAuthorization(req, "demo", "demo");
    QNetworkReply * reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, this, &Network::gotAllEventTemplates);
}

/**
 * The fetch of Event Templates has completed.
 */
void
Network::gotAllEventTemplates() {
    QNetworkReply * reply = static_cast<QNetworkReply *>(QObject::sender());
    if (reply->error()) {
    }
    else {
        QByteArray body = reply->readAll();
        QJsonDocument doc(QJsonDocument::fromJson(body));
        QJsonObject obj = doc.object();
        QJsonValue tValue = obj["eventTemplates"];

        EventTemplate::Vector vec;

        if (tValue != QJsonValue::Undefined && tValue.isArray()) {
            fromJSON(vec, tValue.toArray());
        }

        emit gotEventTemplates(vec);
    }
}

/**
 * Retrieve all event templates. Emits gotEventTemplates when done.
 */
void
Network::getAllEvents() {
    QNetworkRequest req(QString("/demo/events"));
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    setAuthorization(req, "demo", "demo");
    QNetworkReply * reply = manager.get(req);
    connect(reply, &QNetworkReply::finished, this, &Network::gotAllEvents);
}

/**
 * The fetch of all Events has completed.
 */
void
Network::gotAllEvents() {
    QNetworkReply * reply = static_cast<QNetworkReply *>(QObject::sender());
    if (reply->error()) {
    }
    else {
        QByteArray body = reply->readAll();
        QJsonDocument doc(QJsonDocument::fromJson(body));
        QJsonObject obj = doc.object();
        QJsonValue tValue = obj["events"];

        Event::Vector vec;
        if (tValue != QJsonValue::Undefined && tValue.isArray()) {
            fromJSON(vec, tValue.toArray());
        }
        qDebug() << "Size of Events array: " << vec.size();

        emit gotEvents(vec);
    }
}

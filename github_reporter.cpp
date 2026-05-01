#include "gitee_reporter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDebug>

GiteeReporter::GiteeReporter(QObject* parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
{
    loadConfig();
}

GiteeReporter::~GiteeReporter()
{
    saveConfig();
}

GiteeReporter& GiteeReporter::instance()
{
    static GiteeReporter instance;
    return instance;
}

void GiteeReporter::setToken(const QString& token)
{
    m_token = token;
    saveConfig();
}

void GiteeReporter::setRepo(const QString& owner, const QString& repo)
{
    m_owner = owner;
    m_repo = repo;
    m_apiBase = QString("https://gitee.com/api/v5/repos/%1/%2").arg(owner, repo);
    saveConfig();
}

void GiteeReporter::loadConfig()
{
    QSettings settings("TenneQt", "Feedback");
    m_token = settings.value("gitee/token").toString();
    m_owner = settings.value("gitee/owner", "maheos").toString();
    m_repo = settings.value("gitee/repo", "Tenne-Qt").toString();
    m_apiBase = QString("https://gitee.com/api/v5/repos/%1/%2").arg(m_owner, m_repo);
}

void GiteeReporter::saveConfig()
{
    QSettings settings("TenneQt", "Feedback");
    settings.setValue("gitee/token", m_token);
    settings.setValue("gitee/owner", m_owner);
    settings.setValue("gitee/repo", m_repo);
}

void GiteeReporter::submitFeedback(const QString& title,
                                   const QString& body,
                                   const QByteArray& screenshotData)
{
    if (m_token.isEmpty()) {
        emit tokenInvalid();
        return;
    }

    m_pendingImage = screenshotData;
    m_pendingComment = body;
    createIssue(title, body);
}

void GiteeReporter::createIssue(const QString& title, const QString& body)
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_apiBase + "/issues"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("token " + m_token).toUtf8());
    request.setRawHeader("User-Agent", "Tenne-Qt/1.0");

    QJsonObject issueData;
    issueData["title"] = title;
    issueData["body"] = body;
    issueData["labels"] = "feedback,mis-detection";

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(issueData).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleIssueCreation(reply);
    });
}

void GiteeReporter::handleIssueCreation(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Create issue failed:" << reply->errorString();
        qDebug() << "Response:" << reply->readAll();
        emit submitFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    m_currentIssueNumber = QString::number(obj["number"].toInt());
    QString issueUrl = obj["html_url"].toString();

    reply->deleteLater();

    if (!m_pendingImage.isEmpty()) {
        uploadImage(m_pendingImage, m_currentIssueNumber);
    } else {
        emit submitSuccess(issueUrl);
    }
}

void GiteeReporter::uploadImage(const QByteArray& imageData, const QString& issueNumber)
{
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        "form-data; name=\"file\"; filename=\"screenshot.png\"");
    imagePart.setBody(imageData);
    multiPart->append(imagePart);

    QNetworkRequest request;
    request.setUrl(QUrl(m_apiBase + "/issues/" + issueNumber + "/comments"));
    request.setRawHeader("Authorization", ("token " + m_token).toUtf8());

    QNetworkReply* reply = m_nam->post(request, multiPart);
    multiPart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply, issueNumber]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString issueUrl = QString("%1/issues/%2").arg(m_apiBase, issueNumber);
            emit submitSuccess(issueUrl);
        } else {
            qDebug() << "Image upload failed:" << reply->errorString();
            QString issueUrl = QString("%1/issues/%2").arg(m_apiBase, issueNumber);
            emit submitSuccess(issueUrl);
        }
        reply->deleteLater();
    });
}

void GiteeReporter::addComment(const QString& issueNumber, const QString& body)
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_apiBase + "/issues/" + issueNumber + "/comments"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("token " + m_token).toUtf8());

    QJsonObject commentData;
    commentData["body"] = body;

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(commentData).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleCommentCreation(reply);
    });
}

void GiteeReporter::handleCommentCreation(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Add comment failed:" << reply->errorString();
    }
    reply->deleteLater();
}
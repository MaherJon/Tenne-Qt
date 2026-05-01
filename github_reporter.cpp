#include "github_reporter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QBuffer>
#include <QDebug>

static const QString REPO_OWNER = "MaherJon";
static const QString REPO_NAME = "Tenne-Qt";
static const QString API_BASE = "https://api.github.com/repos/" + REPO_OWNER + "/" + REPO_NAME;

GitHubReporter::GitHubReporter(QObject* parent)
    : QObject(parent), m_nam(new QNetworkAccessManager(this))
{
    loadToken();
}

GitHubReporter::~GitHubReporter()
{
    delete m_nam;
}

GitHubReporter& GitHubReporter::instance()
{
    static GitHubReporter instance;
    return instance;
}

void GitHubReporter::loadToken()
{
    QSettings settings;
    m_token = settings.value("github/token").toString();
    m_owner = settings.value("github/owner", REPO_OWNER).toString();
    m_repo = settings.value("github/repo", REPO_NAME).toString();
}

void GitHubReporter::saveToken(const QString& token)
{
    QSettings settings;
    settings.setValue("github/token", token);
    settings.setValue("github/owner", REPO_OWNER);
    settings.setValue("github/repo", REPO_NAME);
}

void GitHubReporter::setToken(const QString& token)
{
    m_token = token;
    saveToken(token);
}

bool GitHubReporter::hasToken() const
{
    return !m_token.isEmpty();
}

void GitHubReporter::submitFeedback(const QString& title,
                                    const QString& body,
                                    const QByteArray& screenshotData)
{
    if (m_token.isEmpty()) {
        emit tokenInvalid();
        return;
    }

    m_pendingImage = screenshotData;
    createIssue(title, body);
}

void GitHubReporter::createIssue(const QString& title, const QString& body)
{
    QNetworkRequest request;
    request.setUrl(QUrl(API_BASE + "/issues"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("token " + m_token).toUtf8());
    request.setRawHeader("User-Agent", "Tenne-Qt-Feedback/1.0");

    QJsonObject issueData;
    issueData["title"] = title;
    issueData["body"] = body;
    issueData["labels"] = QJsonArray::fromStringList({"feedback", "mis-detection"});

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(issueData).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleIssueCreation(reply);
    });
}

void GitHubReporter::handleIssueCreation(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit submitFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (!doc.isObject()) {
        emit submitFailed("Invalid response from GitHub API");
        reply->deleteLater();
        return;
    }

    m_currentIssueNumber = QString::number(doc.object()["number"].toInt());

    if (!m_pendingImage.isEmpty()) {
        uploadImage(m_pendingImage, m_currentIssueNumber);
    } else {
        emit submitSuccess(API_BASE + "/issues/" + m_currentIssueNumber);
    }

    reply->deleteLater();
}

void GitHubReporter::uploadImage(const QByteArray& imageData, const QString& issueNumber)
{
    QString base64Image = QString::fromLatin1(imageData.toBase64());

    QString commentBody = QString(
                              "![截图](data:image/png;base64,%1)\n\n"
                              "*附：程序运行截图*"
                              ).arg(base64Image);

    QJsonObject commentData;
    commentData["body"] = commentBody;

    QNetworkRequest request;
    request.setUrl(QUrl(API_BASE + "/issues/" + issueNumber + "/comments"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("token " + m_token).toUtf8());
    request.setRawHeader("User-Agent", "Tenne-Qt-Feedback/1.0");

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(commentData).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleImageUpload(reply);
    });
}

void GitHubReporter::handleImageUpload(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        emit submitSuccess(API_BASE + "/issues/" + m_currentIssueNumber);
    } else {
        emit submitFailed("Issue created but image upload failed: " + reply->errorString());
    }
    reply->deleteLater();
}
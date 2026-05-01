#ifndef GITEE_REPORTER_H
#define GITEE_REPORTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QString>

class GiteeReporter : public QObject
{
    Q_OBJECT
public:
    static GiteeReporter& instance();

    void setToken(const QString& token);
    void setRepo(const QString& owner, const QString& repo);
    QString getToken() const { return m_token; }
    bool hasToken() const { return !m_token.isEmpty(); }

    void submitFeedback(const QString& title,
                        const QString& body,
                        const QByteArray& screenshotData = QByteArray());

signals:
    void submitSuccess(const QString& issueUrl);
    void submitFailed(const QString& error);
    void tokenInvalid();

private:
    GiteeReporter(QObject* parent = nullptr);
    ~GiteeReporter();

    void createIssue(const QString& title, const QString& body);
    void addComment(const QString& issueNumber, const QString& body);
    void uploadImage(const QByteArray& imageData, const QString& issueNumber);
    void handleIssueCreation(QNetworkReply* reply);
    void handleCommentCreation(QNetworkReply* reply);
    void handleImageUpload(QNetworkReply* reply, const QString& issueNumber);

    void loadConfig();
    void saveConfig();

    QNetworkAccessManager* m_nam;
    QString m_token;
    QString m_owner;
    QString m_repo;
    QString m_apiBase;

    QByteArray m_pendingImage;
    QString m_currentIssueNumber;
    QString m_pendingComment;

    struct PendingRequest {
        enum Type { CreateIssue, AddComment, UploadImage };
        Type type;
        QByteArray data;
        QString issueNumber;
    };
    QList<PendingRequest> m_pendingRequests;
    void processNextRequest();
    bool m_isProcessing = false;
};

#endif // GITEE_REPORTER_H
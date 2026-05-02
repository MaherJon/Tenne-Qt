#ifndef GITHUB_REPORTER_H
#define GITHUB_REPORTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>

class GitHubReporter : public QObject
{
    Q_OBJECT
public:
    static GitHubReporter& instance();

    void setToken(const QString& token);
    bool hasToken() const;
    QString getToken() const { return m_token; }

    void submitFeedback(const QString& title,
                        const QString& body,
                        const QByteArray& screenshotData = QByteArray());

signals:
    void submitSuccess(const QString& issueUrl);
    void submitFailed(const QString& error);
    void tokenInvalid();

private:
    GitHubReporter(QObject* parent = nullptr);
    ~GitHubReporter();

    void createIssue(const QString& title, const QString& body);
    void uploadImage(const QByteArray& imageData, const QString& issueNumber);

    void handleIssueCreation(QNetworkReply* reply);
    void handleImageUpload(QNetworkReply* reply);

    QNetworkAccessManager* m_nam;
    QString m_token;
    QString m_owner;
    QString m_repo;
    QByteArray m_pendingImage;
    QString m_currentIssueNumber;

    void loadToken();
    void saveToken(const QString& token);
};

#endif // GITHUB_REPORTER_H
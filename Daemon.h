#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <clang-c/Index.h>
#ifdef EBUS
#include <QtNetwork>
#endif

class Daemon : public QObject
{
    Q_OBJECT
public:
    Daemon(QObject* parent = 0);
    ~Daemon();

    bool start();
    Q_INVOKABLE QString runCommand(const QStringList& args);

    enum LookupType {
        Definition,
        Declaration,
        Reference
    };
private slots:
    void onFileChanged(const QString &path);
private:
    QString lookup(const QString &name, LookupType type);
    QString lookupLine(const QStringList& args);
    QString addMakefile(const QString& path, const QStringList& args);
    QString addSourceFile(const QStringList& args);
    QString removeSourceFile(const QString& file);
    bool writeAST(const QHash<QString, CXTranslationUnit>::const_iterator &it);
    bool addSourceFile(const QFileInfo& fi, unsigned options =
                       CXTranslationUnit_CacheCompletionResults,
                       QString* result = 0);
    bool addMakefileLine(const QList<QByteArray>& line);
    QString fileList(const QStringList &args);

private:
    CXIndex m_index;
    QHash<QString, CXTranslationUnit> m_translationUnits;
    QFileSystemWatcher m_fileSystemWatcher;
#ifdef EBUS
    QTcpServer *m_server;
    QHash<QTcpSocket*, qint16> m_connections;

    void read(QTcpSocket *socket);
    Q_SLOT void onNewConnection();
    Q_SLOT void onReadyRead();
    Q_SLOT void onDisconnected();
#endif
    
};

#endif

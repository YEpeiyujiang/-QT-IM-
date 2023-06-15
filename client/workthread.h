#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include<QThread>
#include<QSharedPointer>

class WorkThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = nullptr);
    ~WorkThread();
signals:

private:
    QThread *m_thread;
};

#endif // WORKTHREAD_H

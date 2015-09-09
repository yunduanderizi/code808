#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H
 #include <QThread>
 #include <QMutex>

class SearchThread:public QThread
{
    Q_OBJECT
public:
    SearchThread();
    ~SearchThread();

signals:
  void MsgSignal();

protected:
    void run();

private:
    //QMutex m_mutex;
    //volatile bool m_bStopped;
};

#endif // SEARCHTHREAD_H

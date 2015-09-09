#ifndef GUIDETHREAD_H
#define GUIDETHREAD_H
#include <QThread>

class GuideThread : public QThread
{
    Q_OBJECT
public:
    GuideThread();
    public:
        virtual void run();
signals:
       void updateguide();
};


#endif // GUIDETHREAD_H

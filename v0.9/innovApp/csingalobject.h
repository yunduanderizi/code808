#ifndef CSINGALOBJECT_H
#define CSINGALOBJECT_H

#include <QObject>
class CSingalObject;
static CSingalObject *s_ptranslater = NULL;

#define SINGALMODULE (CSingalObject::instance())
class CSingalObject : public QObject
{
    Q_OBJECT
        CSingalObject();
        ~CSingalObject();
signals:
    void carlocation();
    void gpsinfo();
    void windowcontrol();
    void gasstation();
    void park();
    void locatecar();
    void locatehome();
    void destination();
    void destinationrecord();
    void navigation();
    void gpsinforeportcycle();
    void roadbook();
    void hospital();
    void realtimeroad();
    void roadreport();
    void commonreq();
    void setdaynightmode();
    void rightbuttonroute();
    void addressbook();
    void roadbook2();
    void getdaynightmode();
    void showmenu();
    void shutdown();

public:
    QString strInputBakeup;
    void setStrInputBakeup(QString &strObj);
    QString getStrInputBakeup();
    QString length2String(unsigned int ulDist);

    static CSingalObject* instance()
    {
        if (s_ptranslater == NULL)
        {
            s_ptranslater = new CSingalObject();
        }
        return s_ptranslater;
    }
    void Translate(int messageid,int a_ulParam1,int a_ulParam2);
public slots:

};

#endif // CSINGALOBJECT_H

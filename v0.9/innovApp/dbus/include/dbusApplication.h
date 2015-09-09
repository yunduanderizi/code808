
#ifndef DBUS_APPLICATION_H_20100621
#define DBUS_APPLICATION_H_20100621

class QCoreApplication;

class DBusApplicationC
{
public:
	DBusApplicationC(int argc,char *argv[]);
	~DBusApplicationC();

public:
	int loop();

	static unsigned int version();
	static const char *releaseDate();

private:
	QCoreApplication *m_QtAPP;
};


#endif


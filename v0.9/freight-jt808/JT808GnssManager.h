#ifndef JT808GNSSMANAGER_H
#define JT808GNSSMANAGER_H
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <set>
#include <map>
#include <sstream>
#include <boost/circular_buffer.hpp>

using namespace std;
unsigned long GetTickCount();

struct SendDate
{
	int type;
	int total;
	unsigned char buff[1024];
};

typedef boost::circular_buffer<SendDate *> GnssFiFo;




class JT808GnssManager
{
public:
    bool Init();
    void StopGnssUp();
    void SetGnssUp( );
    void SetGnssUp( int gnssupmode, int gnssupset );
public:
    JT808GnssManager();
    ~JT808GnssManager();
    void GnssProc();
    void GnssSend();
private:

private:
    int             m_socket_dp;
    sockaddr_in     m_addr_gnss;
    int             m_gnssupmode;
    int             m_gnssupset;
    int             m_times;
    int             m_distace;
    unsigned long   m_gnssbegintime;
    unsigned long   m_gnsslasttime;
	GnssFiFo Gnssqueue ;

};
extern JT808GnssManager g_JT808GnssManager;
#endif // GNSSMANAGER_H

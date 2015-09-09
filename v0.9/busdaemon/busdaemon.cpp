#include <boost/program_options.hpp>
#include <iostream>
#include <dbusClient.h>
#include "LogUtility.h"
#include "busmessage.h"
#include "SerialConnectionManager.h"
#include "McuMessageHandler.h"
#include "GpsMessageHandler.h"
#include "GpsLocationMessageHandler.h"
#include "DeviceManagerDbusMessage.h"

namespace po = boost::program_options;

using namespace std;

static int ProcessOptions(int argc, char *argv[])
{
    try {

        po::options_description options("Allowed options");
        options.add_options()
            ("help", "produce help message")
            ("debug", po::value<int>(), "set debug level")
            ("log", po::value<int>(), "set log level")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << options << "\n";
            return 1;
        }

        if (vm.count("debug")) {
            cout << "debug level was set to " 
                 << vm["debug"].as<int>() << ".\n";
            LogUtility::SetStdoutLevel(vm["debug"].as<int>());
        } else {
            cout << "debug level was not set.\n";
        }
		
        if (vm.count("log")) {
            cout << "log level was set to " 
                 << vm["log"].as<int>() << ".\n";
            LogUtility::SetLogLevel(vm["log"].as<int>());
        } else {
            cout << "log level was not set.\n";
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}

DBusClientC    *g_pDbusClient;
DBusServerC    *g_pDbusServer;
McuMessageHandler mcuHandler;

int main(int argc, char *argv[])
{
    if(ProcessOptions(argc, argv) != 0)
    {
        return 1;
    }
	
	DBusApplicationC app(argc, argv);

    DBusServerC server(BUSD_DBUS_SERVER_ID,BUSD_DBUS_SERVER_OBJ);

    g_pDbusServer =&server;

    if(server.connect()<0)
    {
        cerr << "Failed to connect to DBUS server.\r\n";
        return -1;
    }
    
    SerialConnectionManager::Instance()->Start();
    GpsMessageHandler::Instance()->RegisterHandler(&mcuHandler);
    GpsMessageHandler::Instance()->Start();
    
    GpsLocationMessageHandler::Instance()->RegisterHandler(&mcuHandler);
    GpsLocationMessageHandler::Instance()->Start();

    DBusClientC client(DEVICE_MANAGER_DBUS_SERVER_ID, DEVICE_MANAGER_DBUS_SERVER_OBJ);
    g_pDbusClient= &client;
    g_pDbusClient->registerNotify(DEVICE_MANAGER_NOTF_ID_DRIVING_STATE,DeviceManagerDrivingStateHandler,DeviceDrivingStateDbusNotifyC::factory);

    if(g_pDbusClient->connect()<0)
    {
        cerr << "Failed to connect to DBUS client.\r\n";
        return -1;
    }

    return app.loop();
}

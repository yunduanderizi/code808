#ifndef _CONNECTION_MANAGER_INTERFACE_H_
#define _CONNECTION_MANAGER_INTERFACE_H_

typedef enum
{
    ENUM_CONNECTION_STATE_CONNECTTING = 0x01,
    ENUM_CONNECTION_STATE_CONNECTED = 0x02,
    ENUM_CONNECTION_STATE_UNBLOCKED = 0x03,
    ENUM_CONNECTION_STATE_DISCONNECTED = 0x04
} ENUM_CONNECTION_EVENT;

typedef enum
{
    ENUM_CONNECTION_ID_MASTER = 0x01,
    ENUM_CONNECTION_ID_SECONDARY = 0x02,
    ENUM_CONNECTION_ID_ASSIGN= 0x03,
    ENUM_CONNECTION_ID_IC= 0x04,
    ENUM_CONNECTION_ID_QREGISTER= 0x05
} ENUM_CONNECTION_ID;

class ConnectionEventHandlerInterface
{
public:
    virtual int ConnectionEventHandler(ENUM_CONNECTION_EVENT state, ENUM_CONNECTION_ID conn) = 0;
};

class ConnectionPacketHandlerInterface
{
public:
    virtual int PacketHandler(unsigned char *buf, int len) = 0;
};
#endif

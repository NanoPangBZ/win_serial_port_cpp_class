#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <thread>

class SerialPort{
public:
    SerialPort();
    virtual ~SerialPort();

    typedef struct{
        std::string     portName;           //端口号
        std::string     driverName;         //驱动信息
    }SerialPortInfo;

    typedef struct{
        uint32_t bound;
        uint8_t  dataBit;
        uint8_t  stopBit;
    }SerialConnectCfg;

    typedef enum{
        RECEIVE = 0,
        DISCONNET = 1,
        CONNECTED = 2
    }SerialPortEventCode;

    typedef struct{
        SerialPortEventCode code;
        SerialPort*         instance;
        void*               user_ctx;
    }SerialPortEvent;

    bool connect( std::string portName , uint32_t bound = 115200 , uint8_t dataBit = 1 , uint8_t stopBit = 1 );
    bool connect( std::string portName , SerialConnectCfg& cfg);
    bool connect( int portNumber , uint32_t bound = 115200 , uint8_t dataBit = 1 , uint8_t stopBit = 1 );
    bool connect( int portNumber , SerialConnectCfg& cfg);
    bool connect( SerialPortInfo& info , uint32_t bound = 115200 , uint8_t dataBit = 1 , uint8_t stopBit = 1 );
    bool connect( SerialPortInfo& info , SerialConnectCfg& cfg );
    bool disconnect();
    bool isConnected();
    size_t receiveLen();
    SerialPortInfo      connectedSerialPortInfo();
    SerialConnectCfg    connectedSerialPortCfg();

    /**
     * @brief 向串口写入(同步，内部带锁)
     * 
     * @param dat 数据
     * @param size 大小
     * @return true 成功
     * @return false 失败
     */
    bool write(uint8_t*dat , size_t size);
    /**
     * @brief 从串口读取(异步，内部带锁，直接从接收缓存中获取)
     * 
     * @param buf 缓存区
     * @param size 读取的大小
     * @return int 读取到的数量
     */
    int  read(uint8_t*buf , size_t size);

    void registerEventHandler( int (*handler)(SerialPortEvent) , void* ctx )
    {
        _eventHandler = handler;
        _eventHandlerUserCtx = ctx;
    }

    void cancelEventHandler()
    {
        _eventHandler = NULL;
    }

    static std::vector<SerialPortInfo> getSerialPortList();

private:
    SerialPortInfo      _serialInfo;
    SerialConnectCfg    _serialCfg;
    int (*_eventHandler)(SerialPortEvent);
    void*   _eventHandlerUserCtx;
    uint8_t _receiveBuf[2048];
    size_t  _receiveLen;
    bool _connect();

    static void _refreshSerialPortInfoList();
    static std::vector<SerialPortInfo>    _serialPortInfoList;
};

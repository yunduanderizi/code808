#include "radioManager.h"
#include "abstractRadio.h"

#include "radioManager_TDA7703.h"
#include "radioManagerOld.h"
#include "si4754radio.h"

#if defined(TDA7703)
#define RADIO_DEVICE_TYPE RADIO_TDA7703
#elif defined(SI4754_OLD)
#define RADIO_DEVICE_TYPE RADIO_SI4754_OLD
#elif defined(SI4754)
#define RADIO_DEVICE_TYPE RADIO_SI4754
#else
#error "###ERROR:No Radio module selected..."
#endif
RadioManagerC::RadioManagerC()
{
    printf("++++++++++++++++++++RadioManagerC()++++++++++++++++++++\n");
    m_radio=NULL;
    m_radio = radioFactory(RADIO_DEVICE_TYPE);
    if(m_radio)
    {
        connect(m_radio,SIGNAL(searchChannelChanged(int,int)),this,SIGNAL(searchChannelChanged(int,int)));
        connect(m_radio,SIGNAL(searchedOneChannel(int)),this,SIGNAL(searchedOneChannel(int)));
        connect(m_radio,SIGNAL(searchEnd(int)),this,SIGNAL(searchEnd(int)));
    }
}
RadioManagerC::~RadioManagerC()
{
    if(m_radio)
        delete m_radio;
}

int RadioManagerC::openRadio(int frequencySection,int channel,int volumeArg)
{
    printf("++++++++++++++++++++++++++++RadioManagerC::openRadio+++++++++++++++++++++++++++++\n");
    if(m_radio)
        return m_radio->openRadio(frequencySection,channel,volumeArg);
    else
        return -1;
}

int RadioManagerC::closeRadio()
{
    printf("+++++++++++++++++++++++++++RadioManagerC::closeRadio()+++++++++++++++++++++\n");
    if(m_radio)
        return m_radio->closeRadio();
    else
        return -1;
}


int RadioManagerC::adjustVolume(int volumeArg,int mode)
{
    printf("++++++++++++++++++adjustVolume+++++++++++++++++\n");
    if(m_radio)
        return m_radio->adjustVolume(volumeArg,mode);
    else
        return -1;
}

int RadioManagerC::getVolume()
{
    printf("++++++++++++++++++getVolume()++++++++++++++++\n");
    if(m_radio)
        return m_radio->getVolume();
    else
        return -1;

}


int RadioManagerC::changeChannel(int channel)
{
    printf("++++++++++++++++RadioManagerC::changeChannel+++++++++++++++++++\n");
    if(m_radio)
        return m_radio->changeChannel(channel);
    else
        return -1;
}


int RadioManagerC::beginSearch(int channel,bool isListenTest)
{
    printf("+++++++++++++++++++RadioManagerC::beginSearch++++++++++++++++++++++\n");
    if(m_radio)
        return m_radio->beginSearch(channel,isListenTest);
    else
        return -1;
}

int RadioManagerC::stopSearch()
{
    printf("++++++++++++++++++RadioManagerC::stopSearch()+++++++++++++++++++++++++\n");
    if(m_radio)
        return m_radio->stopSearch();
    else
        return -1;

}
int RadioManagerC::getVersion(int size,char* buf)
{
    printf("++++++++++++++++++++++RadioManagerC::getVersion++++++++++++++++++++++\n");
    if(m_radio)
    {
        return m_radio->getVersion(size,buf);
    }
    return -1;
}

AbstractRadio* RadioManagerC::radioFactory(Radio_Type  radioType)
{
    AbstractRadio * pRadio =NULL;
    switch(radioType)
    {
        case RADIO_TDA7703:
            pRadio =  new RadioManager_TDA7703;
            break;
        case RADIO_SI4754_OLD:
            pRadio =  new RadioManagerOld;
            break;
        case RADIO_SI4754:
             pRadio = new si4754Radio;
            break;
        default:
            break;
    }
    return pRadio;
}



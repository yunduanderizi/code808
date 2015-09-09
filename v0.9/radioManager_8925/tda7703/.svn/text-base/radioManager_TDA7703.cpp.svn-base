#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <QSettings>
#include "radioManager_TDA7703.h"
#include "radioDefines.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <unistd.h>
#include "PUBLIC.H"
#ifdef __cplusplus
}
#endif

#define debug 0
#define dprintf(msg...) if(debug) printf(msg)

#define RM_STATUS_INIT   	0
#define RM_STATUS_OPENED	1
#define RM_STATUS_LISTEN 	2
#define RM_STATUS_SEARCH 	3

#define RM_FM_SEARCH_INTERVAL	200
#define RM_AM_SEARCH_INTERVAL	200
#define RM_SEARCHED_INTERVAL	50

#define RADIO_CONFIG_FILE_PATHNAME		"/nand3/applicationdata/radio.ini"



RadioManager_TDA7703::RadioManager_TDA7703()
{
	status =RM_STATUS_INIT;
	volume =RM_VOLUME_MAX;

	listenedChannel =0;
	RadioInit();
	m_voiceSwithFunction =NULL;
	m_voiceSwithUserData =NULL;
	m_voiceStatus =RM_VOICE_SWITCH_OFF;

	HIT_Startup_Seq();
	timeId=-1;
}
RadioManager_TDA7703::~RadioManager_TDA7703()
{
	if(status !=RM_STATUS_INIT)
	{
		closeRadio();
	}
}

int RadioManager_TDA7703::openRadio(int frequencySection,int channel,int volumeArg)
{

	dprintf("++++++++++++++++++++++++++++++ RadioManager_TDA7703::openRadio++++++++++++++++++++++++++++\n");
	int  searchFlag =0;
	if(channel ==0)
	{
		searchFlag =1;

		if(m_voiceSwithFunction !=NULL)
		{
			m_voiceSwithFunction(RM_VOICE_SWITCH_OFF,m_voiceSwithUserData);
			m_voiceStatus =RM_VOICE_SWITCH_OFF;
		}

		if(frequencySection==RM_FREQ_SECTION_FM)
		{
			channel =RM_FM_FREQ_MIN;
		}
		else
		{
			frequencySection =RM_FREQ_SECTION_AM;
			channel     	 =RM_AM_FREQ_MIN;
		}
	}
	else
	{
		if(frequencySection == RM_FREQ_SECTION_FM)
		{
			if(channel<RM_FM_FREQ_MIN || channel>RM_FM_FREQ_MAX)
				return RM_R_ERROR_RANGE;
		}
		else
		{
			if(channel<RM_AM_FREQ_MIN || channel>RM_AM_FREQ_MAX)
				return RM_R_ERROR_RANGE;

			frequencySection =RM_FREQ_SECTION_AM;
		}

	}


	int result =openRadioAtChannel(frequencySection,channel,searchFlag);

	if(result !=RM_R_OK)
		return result;

	adjustVolume(volumeArg,RM_VOLUME_SET);

	if(searchFlag!=0)
	{
		status =RM_STATUS_SEARCH;

		listenedChannel =0;
		lastSearchChannel     =0;
		currentSearchChannel =channel;

		channelMeter 	=0;
		searchedChannel =0;
		meterIsUping	=0;

		if(frequencySection == RM_FREQ_SECTION_FM)
			timeId =startTimer(RM_FM_SEARCH_INTERVAL);
		else
			timeId =startTimer(RM_AM_SEARCH_INTERVAL);

		emit searchChannelChanged(lastSearchChannel,currentSearchChannel);
	}
	else
	{
		listenedChannel =channel;
		status =RM_STATUS_LISTEN;
	}

	return RM_R_OK;
}

int RadioManager_TDA7703::closeRadio()
{
	if(status == RM_STATUS_INIT)
		return RM_R_OK;


	if(status ==RM_STATUS_SEARCH)
	{
		killTimer(timeId);
		status =RM_STATUS_INIT;
		dprintf("[radio][%4d] ~~~~~~~~~ close radio searchEnd cause[%d]   \n", __LINE__, RM_SEARCH_R_CANCEL);
		emit searchChannelChanged(currentSearchChannel,0);
		emit searchEnd(RM_SEARCH_R_CANCEL);

	}
	else
	{
		status =RM_STATUS_INIT;
	}
	return RM_R_OK;
}


int RadioManager_TDA7703::adjustVolume(int volumeArg,int mode)
{
	switch(mode)
	{
		case RM_VOLUME_SET:
			volume =volumeArg;
			break;
		case RM_VOLUME_INC:
			volume +=volumeArg;
			break;
		case RM_VOLUME_DEC:
			volume -=volumeArg;
			break;
		default:	
			return RM_R_ERROR_MODE;
	}

	if(volume<RM_VOLUME_MIN)
		volume =RM_VOLUME_MIN;
	else if(volume>RM_VOLUME_MAX)
		volume =RM_VOLUME_MAX;

	return RM_R_OK;
}

int RadioManager_TDA7703::getVolume()
{
	return volume;
}


int RadioManager_TDA7703::changeChannel(int channel)
{
	dprintf("++++++++++++++++++++++RadioManager_TDA7703::changeChannel [%d] +++++++++++++++++++++++++\n", channel);
	int	section;
	if(channel >=RM_AM_FREQ_MIN && channel<=RM_AM_FREQ_MAX)
		section=RM_FREQ_SECTION_AM;
	else if(channel >=RM_FM_FREQ_MIN && channel<=RM_FM_FREQ_MAX)
		section=RM_FREQ_SECTION_FM;
	else
		return RM_R_ERROR_RANGE;

	listenedChannel =channel;

	if(status !=RM_STATUS_LISTEN || freqSection!=section)
	{
		return openRadioAtChannel(section,channel,0);
	}

	if(freqSection==RM_FREQ_SECTION_FM)
		selectFMChannel(channel,0);
	else
		selectAMChannel(channel,0);

	return RM_R_OK;
}


int RadioManager_TDA7703::beginSearch(int channel ,bool isListenTest)
{

	printf("+++++++++++++++++RadioManager_TDA7703::beginSearch %d+++++++++++++++\n", channel);

	int	section;
	if(channel >=RM_AM_FREQ_MIN && channel<=RM_AM_FREQ_MAX)
		section=RM_FREQ_SECTION_AM;
	else if(channel >=RM_FM_FREQ_MIN && channel<=RM_FM_FREQ_MAX)
		section=RM_FREQ_SECTION_FM;
	else
		return RM_R_ERROR_RANGE;

	if(m_voiceSwithFunction !=NULL)
	{
		m_voiceSwithFunction(RM_VOICE_SWITCH_OFF,m_voiceSwithUserData);
		m_voiceStatus =RM_VOICE_SWITCH_OFF;
	}

	if(status !=RM_STATUS_SEARCH || freqSection!=section)
	{
		int result =openRadioAtChannel(section,channel,1);

		if(result !=RM_R_OK)
			return result;

		status =RM_STATUS_SEARCH;

		listenedChannel =0;
		lastSearchChannel     =0;
		currentSearchChannel =channel;

		channelMeter 	=0;
		searchedChannel =0;
		meterIsUping	=0;

		if(freqSection == RM_FREQ_SECTION_FM)
			timeId =startTimer(RM_FM_SEARCH_INTERVAL);
		else
			timeId =startTimer(RM_AM_SEARCH_INTERVAL);

		emit searchChannelChanged(lastSearchChannel,currentSearchChannel);

		return RM_R_OK;
	}

	killTimer(timeId);

	lastSearchChannel    =currentSearchChannel;
	currentSearchChannel =channel;

	emit searchChannelChanged(lastSearchChannel,currentSearchChannel);

	channelMeter 	=0;
	searchedChannel =0;
	meterIsUping	=0;

	if(freqSection == RM_FREQ_SECTION_FM)
		timeId =startTimer(RM_FM_SEARCH_INTERVAL);
	else
		timeId =startTimer(RM_AM_SEARCH_INTERVAL);

	return RM_R_OK;
}

int RadioManager_TDA7703::stopSearch()
{
	if(status !=RM_STATUS_SEARCH)
		return RM_R_OK;

	killTimer(timeId);


	status = RM_STATUS_LISTEN;
	if(listenedChannel !=0)
	{
		if(freqSection ==RM_FREQ_SECTION_FM)
			selectFMChannel(listenedChannel,0);
		else
			selectAMChannel(listenedChannel,0);

	}
	else
	{
		closeRadio();
	}

	emit searchChannelChanged(currentSearchChannel,0);
	dprintf("[radio][%4d] ~~~~~~~~~~~~~~~~~searchEnd cause[%d]   \n", __LINE__, RM_SEARCH_R_CANCEL);
	emit searchEnd(RM_SEARCH_R_CANCEL);

	return RM_R_OK;
}

void RadioManager_TDA7703::setVoiceSwitchFunction(RMVoiceSwitchFunction /*voiceSwithFunction*/,void */*userData*/)
{
	/*
	   m_voiceSwithFunction =voiceSwithFunction;
	   m_voiceSwithUserData =userData;
	   */
}

int RadioManager_TDA7703::openRadioAtChannel(int frequencySection,int channel,char searchFlag)
{
	dprintf("-++++++++++++++ open Radio start +++++++++++++++-\n");
	
	if(status ==RM_STATUS_LISTEN || status ==RM_STATUS_SEARCH)
	{

		if(status ==RM_STATUS_SEARCH)
		{
			killTimer(timeId);
			emit searchChannelChanged(currentSearchChannel,0);
			emit searchEnd(RM_SEARCH_R_CANCEL);
		}

		status =RM_STATUS_OPENED;
	}


	if(frequencySection==RM_FREQ_SECTION_FM)
	{
		HIT_Change_Band(CHANGE_FM);
	}
	else
	{
		HIT_Change_Band(CHANGE_AM);
	}
	usleep(100*1000);



	if(frequencySection==RM_FREQ_SECTION_FM)
	{
		freqSection =RM_FREQ_SECTION_FM;
		selectFMChannel(channel,searchFlag);

	}
	else
	{
		freqSection =RM_FREQ_SECTION_AM;
		selectAMChannel(channel,searchFlag );
	}

	status =RM_STATUS_LISTEN;

	return RM_R_OK;
}

void RadioManager_TDA7703::searchFMChannel(int channel,char searchFlag)
{

	RadioSetFreq(channel/10);
}


void RadioManager_TDA7703::selectFMChannel(int channel,char /*searchFlag*/)
{

	RadioSetFreq(channel/10);
}

void RadioManager_TDA7703::searchAMChannel(int channel,char searchFlag)
{
	RadioSetFreq(channel);
}


void RadioManager_TDA7703::selectAMChannel(int channel,char /*searchFlag*/)
{
	RadioSetFreq(channel);
}

void RadioManager_TDA7703::timerEvent(QTimerEvent */*event*/)
{

	killTimer(timeId);

	if(searchedChannel>0)
	{
		searchedChannel =0;

		searchNextChannel();
		return;
	}

	if(0){
		int fm_Smeter, fm_Multi, fm_Detun, fm_Adj;
		int am_Smeter, am_Detun;
		readParameterConfig(freqSection, &fm_Smeter, &fm_Detun, &fm_Multi, &fm_Adj);
	}

	//printf("##############################currentSearchChannel[%d] last[%d]\n", currentSearchChannel,lastSearchChannel);
	HIT_GetCFQuality(3);
	printf("==============================Multi[%d] Adj[%d] Detun[=%d=] Smeter[==%d==] +Channel[%d] \n", 
		Radio.Multipath, Radio.AdjChannel, Radio.Detuning, Radio.Smeter, currentSearchChannel);

		

	if ( RM_FREQ_SECTION_FM == freqSection )
	{
		if(Radio.Smeter >= 180 && Radio.AdjChannel <= 120 && Radio.Multipath <= 20 &&  Radio.Detuning < 6)
		{
			if(m_voiceSwithFunction!=NULL)
			{
				m_voiceSwithFunction(RM_VOICE_SWITCH_ON,m_voiceSwithUserData);
				m_voiceStatus =RM_VOICE_SWITCH_ON;
			}

			searchedChannel =currentSearchChannel;
			listenedChannel =currentSearchChannel;

			printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Searched One FM Channel[%d]\n\n", searchedChannel);
			if(searchedChannel > 0){
				timeId =startTimer(RM_SEARCHED_INTERVAL);
				emit searchedOneChannel(searchedChannel);
			}else{
				searchNextChannel();
			}

		}
		else
		{
			searchNextChannel();
		}
	}
	else
	{
		if(Radio.Smeter>= 176 && Radio.Detuning<16)
		{
			if(m_voiceSwithFunction!=NULL)
			{
				m_voiceSwithFunction(RM_VOICE_SWITCH_ON,m_voiceSwithUserData);
				m_voiceStatus =RM_VOICE_SWITCH_ON;
			}

			searchedChannel =currentSearchChannel;
			listenedChannel =currentSearchChannel;

			printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Searched One AM Channel[%d]\n", searchedChannel);
			
			if(searchedChannel > 0){
				timeId =startTimer(RM_SEARCHED_INTERVAL);
				emit searchedOneChannel(searchedChannel);
			}else{
				searchNextChannel();
			}
		}
		else
		{	
			searchNextChannel();
		}

	}
}

void RadioManager_TDA7703::searchNextChannel()
{
	int nextSearchInterval =RM_AM_SEARCH_INTERVAL;
	if(freqSection == RM_FREQ_SECTION_FM)
		nextSearchInterval =RM_FM_SEARCH_INTERVAL;

	lastSearchChannel =currentSearchChannel;

	if(freqSection ==RM_FREQ_SECTION_FM)
	{
		currentSearchChannel +=RM_FM_FREQ_STEP;
		if(currentSearchChannel>RM_FM_FREQ_MAX)
		{
			currentSearchChannel=0;
		}
	}
	else
	{
		currentSearchChannel +=RM_AM_FREQ_STEP;
		if(currentSearchChannel>RM_AM_FREQ_MAX)
		{
			currentSearchChannel=0;
		}
	}

	dprintf("lastSearchChannel[%d] currentSearchChannel[%d]\n", lastSearchChannel, currentSearchChannel);


	if ( currentSearchChannel > 108000 )
	{
		currentSearchChannel=0;
	}


	emit searchChannelChanged(lastSearchChannel,currentSearchChannel);

	if(currentSearchChannel>0)
	{
		if(m_voiceSwithFunction!=NULL &&  m_voiceStatus ==RM_VOICE_SWITCH_ON)
		{
			m_voiceSwithFunction(RM_VOICE_SWITCH_OFF,m_voiceSwithUserData);
			m_voiceStatus =RM_VOICE_SWITCH_OFF;
		}

		if(freqSection ==RM_FREQ_SECTION_FM)
			searchFMChannel(currentSearchChannel,1);
		else
			searchAMChannel(currentSearchChannel, 1);

		timeId =startTimer(nextSearchInterval);
	}
	else
	{
		if(listenedChannel!=0)
		{
			if(m_voiceSwithFunction!=NULL)
				m_voiceSwithFunction(RM_VOICE_SWITCH_ON,m_voiceSwithUserData);

			if(freqSection ==RM_FREQ_SECTION_FM)
				selectFMChannel(listenedChannel,0);
			else
				selectAMChannel(listenedChannel,0);

			status =RM_STATUS_LISTEN;
		}
		else
		{
			closeRadio();
		}
		emit searchEnd(RM_SEARCH_R_OK);
	}

	return;
}


void RadioManager_TDA7703::readParameterConfig( int frequencySection, int *smeter, int *detun, int *multi, int *adj )
{
	QSettings *ConfigIni = new QSettings(RADIO_CONFIG_FILE_PATHNAME,QSettings::IniFormat,0); 
	if ( NULL == ConfigIni )
	{
		return;
	}
	int size = 0;
	int ret = 0;

	if ( RM_FREQ_SECTION_FM == frequencySection )
	{
		size = ConfigIni->beginReadArray("fm_config");
		
		ret = ConfigIni->value("fm_Smeter", 0 ).toInt();
		if (  0 != ret )
		{
			*smeter = ret;
		}

		ret = ConfigIni->value("fm_Detun", 0 ).toInt();
		if (  0 != ret )
		{
			*detun = ret;
		}
		ret = ConfigIni->value("fm_Multi", 0 ).toInt();
		if (  0 != ret )
		{
			*multi = ret;
		}
		ret = ConfigIni->value("fm_Adj", 0 ).toInt();
		if (  0 != ret )
		{
			*adj = ret;
		}

	}else{
		size = ConfigIni->beginReadArray("am_config");
		
		ret = ConfigIni->value("am_Smeter", 0 ).toInt();
		if (  0 != ret )
		{
			*smeter = ret;
		}

		ret = ConfigIni->value("am_Detun", 0 ).toInt();
		if (  0 != ret )
		{
			*detun = ret;
		}
	}

	ConfigIni->endArray();
}




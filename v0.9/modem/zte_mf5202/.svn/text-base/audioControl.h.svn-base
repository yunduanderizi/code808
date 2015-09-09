/**
 * @file   audioControl.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110301
 * 
 * @brief  
 *      Implement for phone audio control and sms audio control
 * 
 */

#ifndef AUDIOCONTROL_H_20110301
#define AUDIOCONTROL_H_20110301

#include <QObject>
#include <QTimerEvent>
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
#include "phoneMonitorVoiceReportBase.h"
#else
#include "audio_control_client.h"
#endif
#include "play_sign_sound.h"

class PhoneAudioControlC : public QObject
{
	Q_OBJECT
		
public:
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
	PhoneAudioControlC(PhoneMonitorVoiceReportBase *reportBase);
#else
	PhoneAudioControlC(AudioCtrlClientC *pAudioCtrlClient);
#endif

	~PhoneAudioControlC();

	int playRingAudio();

	int playIncomingCallAudio();

	int playDefaultIncomingCallAudio();

	int playDTMFAudio();
	
	void stopPlay();

	int mute();

	int unMute();
	
private:
	PlaySignSound m_Player;

#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
	PhoneMonitorVoiceReportBase *m_reportBase;
#else
	AudioCtrlClientC *m_pAudioCtrlClient;
#endif
	int m_iTimerForSendDTMF;
	void run();
	void timerEvent(QTimerEvent * event);
};



class SmsAudioControlC : public QObject
{
	Q_OBJECT
		
public:
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
	SmsAudioControlC(PhoneMonitorVoiceReportBase *reportBase);
#else
	SmsAudioControlC(AudioCtrlClientC *pAudioCtrlClient);
#endif
	~SmsAudioControlC();

	int playAudio();

	void stopPlay();

public slots:
	void slotFinished();
	
private:
	PlaySignSound m_Player;
	
#ifdef WITH_NEW_AUDIO_CONTROL_SYSTEM
	PhoneMonitorVoiceReportBase *m_reportBase;
#else
	AudioCtrlClientC *m_pAudioCtrlClient;
#endif

	void run();
};

#endif



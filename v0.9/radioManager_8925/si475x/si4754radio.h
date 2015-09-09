#ifndef SI4754RADIO_H
#define SI4754RADIO_H
#include "abstractRadio.h"
#include <QThread>
#include <QMutex>

class si4754Radio:public AbstractRadio
{
    Q_OBJECT

public:
    si4754Radio();
    int closeRadio();
    int changeChannel(int /*channel*/);
    int beginSearch(int /*channel*/,bool isListenTest=false);
    int stopSearch();
    int getVersion(int size,char* buf);
    void run();

private slots:
    void slotSearchChannel(void);
private:
    void readParameterConfig(void);
	int readPatchFile(QString fileName,unsigned char **buf);
    void InitFM(void);
    void InitAM(void);
    void closeFM(void);
    void closeAM(void);
    void changeFM(int channel);
    void changeAM(int channel);
    int seekFM(void);
    int seekAM(void);
    void mute(void);
    void unMute(void);
    bool isFM(int channel);
    bool isAM(int channel);
    int m_radioMode;//AM/FM模式 或者初始化模式
    int m_currentFrequency;//当前模式
    bool m_isSearching;//是否正在搜索
    bool serch_once;   //搜索一圈
#if 1
    int m_fmRssi;//FM信号质量
    int m_amRssi;//am信号质量
    int m_fmSnr;//FM信噪比
    int m_amSnr;//am信噪比
    int m_volumn;//FM/AM音量
#endif
    QMutex fmTmpMutex;
    QMutex amTmpMutex;
    bool listenTest;
    int firstTwoChannel; //AM开始搜索值改为 1620，因为522的话会跳过522，位531，因此需要过滤开始的 1620，1629
};

#endif // SI4754RADIO_H

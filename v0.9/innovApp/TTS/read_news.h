#if defined(ARM_LINUX)
#ifndef READ_THREAD
#define READ_THREAD
#include <QApplication>
#include <QThread>
#include <QDebug>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ivTTS.h"
#include "alsa/asoundlib.h"
#include <QMutex>
#include <QWaitCondition>
#include <QStringList>

/* TTS运行状态 */
typedef enum{
        ivTTS_Status_Idle = 0, /* 无效状态 */
        ivTTS_Status_Ready,    /* 就绪状态 */
        ivTTS_Status_Play,     /* 开始状态 */
        ivTTS_Status_Exit      /* 退出状态 */
}ivTTS_Status;

class ReadThread : public QThread
{
        Q_OBJECT
public:
        ReadThread();
        ~ReadThread();

        static ReadThread *object();

        void aisound_play();
        void aisound_stop();
        void aisound_start(const QString &content,int warn = 0);
        int aisound_start(int index,const QStringList &content,int warn = 0);

        ivTTS_Status getRunStatus(void);
        ivTTS_Status setRunStatus(ivTTS_Status status);
        int getBeingReadedIndex(void);
        snd_pcm_uframes_t getFramesValue(void);
        snd_pcm_t *getHandleValue(void);


private:
        void aisound_create();
        int init_asound();
        int aisound_release();
        int play_sign_sound();
        void release_asound();
        int check_read(int fd, void* buf);
        void run();
        int setHandleValue(snd_pcm_t *h);
        int start();

private:	
        ivHTTS hTTS;
        ivTResPackDescExt   tResPackDesc;
        ivPByte pHeap;
        ivPByte p_pHeap;
        ivTTSErrID ivReturn;
        ivTTS_Status run_status;         /* 运行状态 */
        QMutex m_setMutex;
        QStringList contentListToBeRead;
        int listIndex;
        QString contentToBeRead;
        bool isReadList;
        bool isReadString;
        int warning_state;

        int fd; 
        int asound_size;
        long loops;
        char *buffer;
        char *p_buffer;
        int channels;
        unsigned int val;
        int pcm_dir;
        snd_pcm_uframes_t frames;
        snd_pcm_hw_params_t* params;
        snd_pcm_t* handle;

        static ReadThread *thisObject;
        QMutex m_quitMutex;
        QWaitCondition m_quitCond;

signals:
        void strReadEnd(void);
        void listReadEnd(void);
        void oneListReadend(int);
        void oneListReadErr(int);
        void strReadErr(void);

};

#endif
#endif


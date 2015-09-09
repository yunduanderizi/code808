#include "read_Information.h"

#define ivTTS_HEAP_SIZE 70000 /* constant for TTS heap size(混含音效 )*/
#define READ_MAX_SIZE 10000
#define MAXSIZE 1024

static ivPByte			g_pRes;	
/* Message */
ivTTSErrID DoMessage_news()
{
        /* 获取消息，用户实现 */
        if(ivTTS_Sta_Play == ReadThread_NEWS::object()->getRunStatus())
        {
                /* 继续合成 */
                return ivTTS_ERR_OK;
        }
        else
        {
                /* 退出合成 */
                return ivTTS_ERR_EXIT;
        }
}

ivTTSErrID OnOutput_news(
        ivUInt16		nCode,			/* [in] output data code */
        ivCPointer	pcData,			/* [in] output data buffer */
        ivSize		nSize )			/* [in] output data size */
{
        int err = 0;
        snd_pcm_t* handle=ReadThread_NEWS::object()->getHandleValue();

        err = snd_pcm_writei(handle, pcData, nSize/2);
        if ( err == -EPIPE)
        {
                fprintf(stderr,"underrun occurred! err: %s\n", snd_strerror(err));
                if( (err = snd_pcm_prepare(handle)) < 0)
                {
                        fprintf(stderr,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(err));
                }
        }
        else if (err < 0)
        {
                fprintf(stderr,"err writei!\n");
        }
        else if (err != (int)ReadThread_NEWS::object()->getFramesValue() )
        {
                //fprintf(stderr,"short writei!\n");
        }
        return ivTTS_ERR_OK;
}

/* read resource callback */
ivBool ivCall ReadResCB_news(
        ivPointer		pParameter,		/* [in] user callback parameter */
        ivPointer		pBuffer,		              /* [out] read resource buffer */
        ivResAddress	iPos,			/* [in] read start position */
        ivResSize		nSize )			/* [in] read size */
{
        /*FILE* pFile = (FILE*)pParameter;
        int err;

        fseek(pFile, iPos, SEEK_SET);
        err = fread(pBuffer, 1, nSize, pFile);

        if (err == (int)nSize)
        {
                return ivTrue;
        }
        else
        {
                return ivFalse;
        }*/
	unsigned char *res = (unsigned char *)pParameter;
	memcpy(pBuffer, res+iPos, nSize); 

	return ivTrue;
}

/* output callback */
ivTTSErrID ivCall OutputCB_news(
        ivPointer		pParameter,		/* [in] user callback parameter */
        ivUInt16		nCode,			/* [in] output data code */
        ivCPointer	pcData,			/* [in] output data buffer */
        ivSize		nSize )			/* [in] output data size */
{
        /* 获取线程消息，是否退出合成 */
        ivTTSErrID tErr = DoMessage_news();

        if (tErr != ivTTS_ERR_OK)
        {
                return tErr;
        }

        /* 把语音数据送去播音 */
        return OnOutput_news(nCode, pcData, nSize);
}

/* parameter change callback */
ivTTSErrID ivCall ParamChangeCB_news(
        ivPointer  	pParameter,		/* [in] user callback parameter */
        ivUInt32		nParamID,		/* [in] parameter id */
        ivUInt32		nParamValue )		/* [in] parameter value */
{
        return ivTTS_ERR_OK;
}

/* progress callback */
ivTTSErrID ivCall ProgressCB_news(
        ivPointer		pParameter,		/* [in] user callback parameter */
        ivUInt32		iProcPos,		/* [in] current processing position */
        ivUInt32		nProcLen )		/* [in] current processing length */
{
        fprintf(stderr,"********** iprocpos=%ld **********\n",iProcPos);
        return ivTTS_ERR_OK;
}

ivUInt16 ivCall LogCB_news(
        ivPointer	pParameter,			/* [out] user callback parameter */
        ivCPointer	pcData,				/* [in] output data buffer */
        ivSize		nSize )				/* [in] output data size */
{
        FILE *pFile = (FILE*)pParameter;
        ivSize nWriten = fwrite(pcData,1,nSize,pFile);

        if( nWriten == nSize )
        {
                return ivTTS_ERR_OK;
        }
        else
        {
                return ivTTS_ERR_FAILED;
        }
}

ReadThread_NEWS::ReadThread_NEWS()
{
        channels = 0;
        frames = 0;
        run_status = ivTTS_Sta_Idle;
        handle = 0;
        listIndex = -1;
        isReadList = false;
        warning_state = 0;
}

ReadThread_NEWS::~ReadThread_NEWS()
{
}

ReadThread_NEWS * ReadThread_NEWS::thisObject = NULL;
ReadThread_NEWS * ReadThread_NEWS::object()
{
        if(thisObject==NULL)
        {
                thisObject = new ReadThread_NEWS();
        }
        return thisObject;
}

void ReadThread_NEWS::aisound_create()
{
        /* 设置状态 */
        run_status = ivTTS_Sta_Idle;

        /* 分配堆 */
        pHeap = (ivPByte)malloc(ivTTS_HEAP_SIZE);
        memset(pHeap, 0, ivTTS_HEAP_SIZE);

        p_pHeap = pHeap;

	int nSize;
	FILE *fpRes;
	fpRes = fopen("Resource.irf", "r");
	fseek(fpRes,0,SEEK_END);
	nSize = ftell(fpRes);
	fseek(fpRes, 0, SEEK_SET);
	g_pRes = (ivPByte)malloc(nSize);
	fread(g_pRes,1,nSize,fpRes);
	fclose(fpRes);       

        /* 初始化资源 */
        /* 可以有多个资源包，可以分包*/
        tResPackDesc.pCBParam = (ivPointer)g_pRes;  
        tResPackDesc.pfnRead = ReadResCB_news;
        tResPackDesc.pfnMap = NULL;
        tResPackDesc.nSize = 0;

        /* TTS内部使用 */
        tResPackDesc.pCacheBlockIndex = NULL;
        tResPackDesc.pCacheBuffer = NULL;
        tResPackDesc.nCacheBlockSize = 0;
        tResPackDesc.nCacheBlockCount = 0;
        tResPackDesc.nCacheBlockExt = 0;

        /* 创建 TTS 实例 ，使用Log，就将最后1个参数设为&tUserInfo，不用就设置成NULL*/
        ivReturn = ivTTS_Create(&hTTS, (ivPointer)pHeap, ivTTS_HEAP_SIZE, ivNull, (ivPResPackDescExt)&tResPackDesc, (ivSize)1,NULL);

        /* 设置音频输出回调 */
        ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_OUTPUT_CALLBACK, (ivUInt32)OutputCB_news);

        /* 设置输入文本代码页 */
        ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_UTF8);

        /* 设置语种 */
        ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_LANGUAGE, ivTTS_LANGUAGE_AUTO);	

        /* 设置音量 */
        ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_VOLUME, ivTTS_VOLUME_NORMAL);

        /*块式合成， 设置发音人为 XIAOYAN */
        ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_ROLE, ivTTS_ROLE_XIAOYAN);
}

//void ReadThread_NEWS::aisound_start()
void ReadThread_NEWS::aisound_start(const QString &content,int warn)
{
        m_quitMutex.lock();
        if(run_status ==  ivTTS_Sta_Play)
        {
                run_status = ivTTS_Sta_Exit;
                m_quitCond.wait(&m_quitMutex);
                wait();
        }

        contentToBeRead = content;
        run_status = ivTTS_Sta_Play;
        warning_state = warn;

        isReadString = true;
        //super.start();
        QThread::start();

        m_quitMutex.unlock();
}

int ReadThread_NEWS::aisound_start(int index,const QStringList &contentList,int warn)
{
        m_quitMutex.lock();
        if(contentList.empty())
        {
                listIndex = -1;
                m_quitMutex.unlock();
                return -1;
        }
        else if(index < 0)
        {
                listIndex = 0;
        }
        else if(index >= contentList.count())
        {
                listIndex = 0;
        }
        else
        {
                listIndex = index;
        }

        if(run_status ==  ivTTS_Sta_Play)
        {
                run_status = ivTTS_Sta_Exit;
                m_quitCond.wait(&m_quitMutex);
                wait();
        }

        contentListToBeRead = contentList;
        run_status = ivTTS_Sta_Play;
        warning_state = warn;

        isReadList = true;
        isReadString = false;

        //super.start();
        QThread::start();

        m_quitMutex.unlock();

        return 0;
}

void ReadThread_NEWS::aisound_play()
{
        char text_buf[READ_MAX_SIZE];
        int read_return_value;

        if(init_asound() < 0)
        {
                fprintf(stderr,"..XXXXXXXXX.%d,%s,%s...XXXXXXXXXXXXXXXXXXXXX...\n",__LINE__,__FUNCTION__,__FILE__);
                return;
        }
        else
        {
        }

        aisound_create();

        //fprintf(stderr,"........diwei,line:%d,%s,%s...........\n",__LINE__,__FUNCTION__,__FILE__);

        /* 设置状态 */
        m_quitMutex.lock();
        run_status = ivTTS_Sta_Play;
        m_quitMutex.unlock();
        //fprintf(stderr,"........diwei,line:%d,%s,%s...........\n",__LINE__,__FUNCTION__,__FILE__);

        //ivReturn = ivTTS_SynthText(hTTS, ivText(readString), -1);
        if( isReadString && run_status == ivTTS_Sta_Play)
        {
                //fprintf(stderr,"........diwei,line:%d,%s,%s,isReadList=%d...........\n",__LINE__,__FUNCTION__,__FILE__,isReadList);
                ivReturn = ivTTS_SynthText(hTTS, ivText(contentToBeRead.toUtf8().data()), -1);
                if(0 == ivReturn)
                {
                        emit strReadEnd();
                }
                else
                {
                        emit strReadErr();
                }
                if(isReadList)
                {
                        for(;listIndex >=0 && listIndex < contentListToBeRead.count();listIndex++)
                        {			        
                                if(warning_state)
                                        play_sign_sound();
                                ivReturn = ivTTS_SynthText(hTTS, ivText(contentListToBeRead[listIndex].toUtf8().data()), -1);
                                if(0 == ivReturn)
                                {
                                        emit oneListReadend(listIndex);
                                }
                                else
                                {
                                        emit oneListReadErr(listIndex);
                                        if(ivTTS_Sta_Exit == run_status)
                                        {
                                                break;
                                        }
                                }
                        }
                        if(contentListToBeRead.count() == listIndex)
                                emit listReadEnd();
                }
                else
                {
                }
                //fprintf(stderr,"........diwei,line:%d,%s,%s,isReadList=%d...........\n",__LINE__,__FUNCTION__,__FILE__,isReadList);
        }
        else if(isReadList && run_status == ivTTS_Sta_Play)
        {
                for(;listIndex >=0 && listIndex < contentListToBeRead.count();listIndex++)
                {		        
                        if(warning_state)
                                play_sign_sound();
                        ivReturn = ivTTS_SynthText(hTTS, ivText(contentListToBeRead[listIndex].toUtf8().data()), -1);
                        if(0 == ivReturn)
                        {
                                emit oneListReadend(listIndex);
                        }
                        else
                        {
                                emit oneListReadErr(listIndex);
                                if(ivTTS_Sta_Exit == run_status)
                                {
                                        break;
                                }
                        }
                        //fprintf(stderr,"........diwei,line:%d,%s,%s,listIndex:%d...........\n",__LINE__,__FUNCTION__,__FILE__,listIndex);
                }
                if(contentListToBeRead.count() == listIndex)
                        emit listReadEnd();		
        }
        aisound_release();
        release_asound();
        if(listIndex >= contentListToBeRead.count())
        {
                listIndex = -1;
                isReadList = false;
        }

        m_quitMutex.lock();
        run_status = ivTTS_Sta_Idle;
        m_quitCond.wakeOne();
        m_quitMutex.unlock();
}

void ReadThread_NEWS::aisound_stop()
{
        /* TTS正在运行时才需要停止 */
        m_quitMutex.lock();

        if(run_status == ivTTS_Sta_Play)
        {
                run_status = ivTTS_Sta_Exit;
                m_quitCond.wait(&m_quitMutex);
                wait();
        }

        run_status = ivTTS_Sta_Idle;

        isReadList = false;
        isReadString = false;
        listIndex = -1;

        m_quitMutex.unlock();
}

int ReadThread_NEWS::aisound_release()
{
        /* 逆初始化 */
        ivReturn = ivTTS_Destroy(hTTS);

        if ( tResPackDesc.pCacheBlockIndex )
        {
                free(tResPackDesc.pCacheBlockIndex);
        }
        if ( tResPackDesc.pCacheBuffer )
        {
                free(tResPackDesc.pCacheBuffer);
        }
        if ( pHeap )
        {
                free(p_pHeap);
        }

        if (tResPackDesc.pCBParam)
        {
                /*FILE *a;
                a = (FILE*)tResPackDesc.pCBParam;
                fclose(a);*/
		if(g_pRes != NULL)
			free(g_pRes);
		g_pRes = NULL;
                tResPackDesc.pCBParam= NULL;
        }

        /* 设置状态 */
        //run_status = ivTTS_Sta_Idle;
}

int ReadThread_NEWS::init_asound()
{
        int err = 0;

        fprintf(stderr,"snd_pcm_open\n");

        if ((err = snd_pcm_open(&handle, "plug:news", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
        {
                fprintf(stderr,"snd_pcm_open failed!\n");

                return -1;
        }

        fprintf(stderr,"params\n");

        snd_pcm_hw_params_alloca(&params);

        if ((err = snd_pcm_hw_params_any(handle, params)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params_any failed!   err = %d\n", err);

                return -1;
        }
	
        if ((err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params_set_access failed!   err = %d\n", err);

                return -1;
        }
	
        if ((err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params_set_format failed!   err = %d\n", err);

                return -1;
        }

        channels = val = 1;

        if ((err = snd_pcm_hw_params_set_channels(handle, params, val)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params_set_channels failed!   err = %d\n", err);

                return -1;
        }
	
        val = 16000;

        if ((err = snd_pcm_hw_params_set_rate_near(handle, params, &val, &pcm_dir)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params_set_rate_near failed!   err = %d\n", err);

                return -1;
        }

        fprintf(stderr,"set_hw_params!\n");

        if ( (err = snd_pcm_hw_params(handle, params)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params-failed!	err = %d err: %s\n", err, snd_strerror(err));

                return -1;
        }
	
        fprintf(stderr,"snd_pcm_hw_params_get_period_size!\n");

        if ((err = snd_pcm_hw_params_get_period_size(params, &frames, &pcm_dir)) < 0)
        {
                fprintf(stderr,"snd_pcm_hw_params_get_period_size failed!   err = %d\n", err);

                return -1;
        }
	
        fprintf(stderr,"-----------frames = %d\n", frames);	

        asound_size = frames * channels * 2;

        buffer = (char *)malloc(sizeof(char) * asound_size);

        p_buffer = buffer;

        return 0;
}

int ReadThread_NEWS::play_sign_sound()
{
        int err = 0;

        fprintf(stderr,"begin open sound_file!\n");	

        if ((fd = open("NewsSound.pcm", O_RDWR)) < 0)
        {
                fprintf(stderr,"open file failed!\n");

                return -1;
        }

        fprintf(stderr,"-----------size = %d\n", asound_size);	

        loops = 1;

        while (loops)
        {
                if ((err = read(fd, buffer, asound_size)) == 0)
                {
                        fprintf(stderr,"end of file!\n");
                        break;
                }
                else if (err != asound_size)
                        fprintf(stderr,"short read!\n");

                err = snd_pcm_writei(handle, buffer, frames);
                if ( err == -EPIPE)
                {
			fprintf(stderr,"underrun occurred! err: %s\n", snd_strerror(err));
			if( (err = snd_pcm_prepare(handle)) < 0)
			{
				fprintf(stderr,"snd_pcm_prepare_failed! err: %s\n", snd_strerror(err));
			}
                }
                else if (err < 0)
                {
                        fprintf(stderr,"err writei!\n");
                }
                else if (err != (int)frames)
                {
                        //fprintf(stderr,"short writei!\n");
                }
        }
        ::close(fd);
        return 0;
}

void ReadThread_NEWS::release_asound()
{
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        free(p_buffer);
}

int ReadThread_NEWS::check_read(int fd, void* buf)
{
        int char_num = 0;
        int sign_num = 0;
        char character[1];
        int read_return_value;

        while (sign_num < 5)
        {
                read_return_value = read(fd,character,1);
                if (read_return_value == -1)
                {
                        fprintf(stderr,"fail to read line %d,file is:%s\n",__LINE__,__FILE__);
                        return -1;
                }
                else if (read_return_value == 0)
                {
                        break;
                }

                char_num++;

                if (character[0] == '@')
                {
                        sign_num++;			
                }
        }

        if (lseek(fd,0-char_num,SEEK_CUR) == -1)
        {
                fprintf(stderr,"fail to lseek line %d,file is:%s\n",__LINE__,__FILE__);
                return -1;
        }

        if (sign_num == 0)
        {
                return 0;
        }

        return read(fd,buf,char_num);	
}

void ReadThread_NEWS::run()
{
        aisound_play();
}

ivTTS_Status_News ReadThread_NEWS::setRunStatus(ivTTS_Status_News status)
{
        m_setMutex.lock();
        run_status = status;
        m_setMutex.unlock();
        return run_status;
}

ivTTS_Status_News ReadThread_NEWS::getRunStatus(void)
{
        return run_status;
}

int ReadThread_NEWS::getBeingReadedIndex(void)
{
        return listIndex;
}

int ReadThread_NEWS::setHandleValue(snd_pcm_t *h)
{
        m_setMutex.lock();
        handle = h;
        m_setMutex.unlock();

        return 0;
}

snd_pcm_t *ReadThread_NEWS::getHandleValue(void)
{
        return handle;
}

snd_pcm_uframes_t ReadThread_NEWS::getFramesValue(void)
{
        return frames;
}

int ReadThread_NEWS::start()
{
        //super.start();
        QThread::start();
        return 0;
}


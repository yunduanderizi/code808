#include "si4754radio.h"
#include "radioDefines.h"
#include <QTimer>
#include <QSettings>
#include <QRegExp>
#include "stdlib.h"
#include <QFile>
#include <QApplication>
#ifdef __cplusplus
extern "C" {
#endif
//#include "si475xAMRX.h"
//#include "si475xFMRX.h"
#include "radioCommon.h"
#include "asc1411.h"
#ifdef __cplusplus
}
#endif
#define RADIO_CONFIG_FILE_PATHNAME		"/nand3/applicationdata/radio.ini"
#define AM_PATCH_FILE   "/nand3/applicationdata/fix_amrx_patch.csg"
int channel_fm_tmp=0;
int channel_am_tmp=0;
si4754Radio::si4754Radio():m_radioMode(RM_FREQ_SECTION_NULL),
    m_currentFrequency(-1),
    m_isSearching(false),
    serch_once(false)
#if 0
    m_fmRssi(20),
    m_amRssi(20),
    m_fmSnr(3),
    m_amSnr(3),
    m_volumn(32)
#endif
{
   // readParameterConfig();
  // 	io2w_init();
  	init_i2c();
    //在读取时有一个重置收音机模块，随时出问题时即可重置 [zhenyubin 2015/05/06]
    realtime_readstatus();
//   	asc1411_configure();
#if 0
	unsigned char * pPatch=NULL;
	u32 line = readPatchFile(AM_PATCH_FILE,&pPatch);
	setAMPatch(pPatch);
	setAMPatchLine(line);
#endif
    listenTest=false;
    firstTwoChannel=0;
	printf("+++++++++++++++++++++++++si4574Radio()++++++++++++++++++++\n");
}
int si4754Radio::closeRadio()
{
	printf("+++++++++++++++++si4754Radio::closeRadio()+++++++++++++++++\n");
//    mute();
    return 0;
}

int si4754Radio::changeChannel(int channel)
{
    printf("++++++++++++++++++si4754Radio::changeChannel++++++++++++++++++++++++\n");
    printf("+++++++++++++++++channel is++++++++\n");
    printf("%d\n",channel);
    if(isFM(channel))//FM的台
    {
        if(m_radioMode == RM_FREQ_SECTION_AM)//当前是AM模式
        {
        //    closeAM();//关闭AM才能初始化FM
        //    InitFM();
        }
        else if(m_radioMode== RM_FREQ_SECTION_NULL)//当前没有设置模式
        {
         //   closeAM();//关闭FM才能初始化AM
         //   InitFM();
        }
        m_radioMode = RM_FREQ_SECTION_FM;
        changeFM(channel);

    }
    else if(isAM(channel))//AM的台
    {
        if(m_radioMode == RM_FREQ_SECTION_FM)//当前是FM模式
        {
          //  closeFM();//关闭FM才能初始化AM
           // InitAM();
        }
        else if(m_radioMode == RM_FREQ_SECTION_NULL)//当前没有设置模式
        {
           // closeFM();//关闭FM才能初始化AM
           // InitAM();
        }
        m_radioMode = RM_FREQ_SECTION_AM;
        changeAM(channel);

    }
    return 1;
}

int si4754Radio::beginSearch(int channel,bool isListenTest)
{
    printf("++++++++++++++++++++++++++++si4754Radio::beginSearch+++++++++++++++++++++++++\n");
    int res = -1;
    listenTest=isListenTest;
    if(isFM(channel))//FM的台
    {
        changeChannel(channel);
        if(m_radioMode != RM_FREQ_SECTION_FM)
        {
            qWarning("[%s] FM channel ,but not FM mode",__func__);
            return -1;
        }
        m_isSearching = true;

        //防止有时不为0，造成搜到小于此值的频道退出搜索
        fmTmpMutex.lock();
        channel_fm_tmp=0;
        fmTmpMutex.unlock();

        if(this->isRunning())
        {
            stopSearch();
            usleep(100000);
        }
        start();
        res =0;
    }
    else
    {
        //如果是AM这个模块需从1620开始 循环到522 【zhenyubin 2015/05/12】
        changeChannel(1620);
        //changeChannel(channel);
        if(m_radioMode != RM_FREQ_SECTION_AM)
        {
            qWarning("[%s] AM channel ,but not AM mode",__func__);
            return -1;
        }
        m_isSearching = true;
        firstTwoChannel=0;

        //防止有时不为0，造成搜到小于此值的频道退出搜索
        amTmpMutex.lock();
        channel_am_tmp=0;
        amTmpMutex.unlock();

        if(this->isRunning())
        {
            stopSearch();
            usleep(100000);
        }
        start();
        res =0;
    }
    return res;

}

int si4754Radio::stopSearch(void)
{
   printf("++++++++++++++++si4754Radio::stopSearch++++++++++++++++\n");
   unsigned char buffer[16];
   if(m_isSearching==true)
   {
   	m_isSearching = false;
   };
   fmTmpMutex.lock();
   channel_fm_tmp = 0;
   fmTmpMutex.unlock();

   amTmpMutex.lock();
   channel_am_tmp = 0; 
   amTmpMutex.unlock();
#if 0 
   si475x_read_current_status(8,buffer);
   if(buffer[3]==0x03)
   {
   	seek(1);                 //播放状态，再次搜台，停止搜索
   }
#endif
   return 0;
}
int si4754Radio::getVersion(int size,char* buf)
{
    int res = -1;
#if 0
   res = si475x_getPartInformation(size,buf);
#endif
//  res = si475x_read_current_status(size,(unsigned char *)buf);
   return res;
}

void si4754Radio::run()
{
    usleep(500000);
    printf("Radio Thread Start Run ... ... ... ... ...\n");
    int res = -1;
    while(m_isSearching)
    {
        if(m_radioMode == RM_FREQ_SECTION_FM)
        {
            QApplication::processEvents();
            res =  seekFM();
            printf("++++++++++++++++++++res is++++++++++++++++\n");
            printf("%d\n",res);
            for(;;)
            {
                if(m_isSearching == false) //停止搜台
                {
                    seekFM();
                    usleep(100000);
                    printf("Stop FM Search By User!\n");
                    emit searchEnd(RM_SEARCH_R_OK);
                    return;
                }
                QApplication::processEvents();
                read_status();
                printf("FM len is:%d , id is:%d ,version is:%d ,statu is :%d , band is : %d , freq is : %d\n",LEN,ID,VER,STATUS,BADN,FREQ);
                if(STATUS==1)
                {
                    printf("resetseting......\n");
                }
                if(STATUS==2)
                {
                    printf("Tuning......\n");
                }
                if(STATUS==2)
                {
                    printf("Playing.......\n");
                }
                if(STATUS==3)
                {
                    printf("Searching......\n");
                }
                if(STATUS==4)
                {
                    printf("Error......\n");
                    break;
                }
                if(STATUS==5)
                {
                    printf("Not found.....\n");
                    break;
                }
                if(STATUS==6)
                {
                    printf("Finding.....\n");
                    break;
                }
                QApplication::processEvents();
                wait_ms(15);
                QApplication::processEvents();

                //超出搜索范围后停止搜索
                if(FREQ*10>=RM_FM_FREQ_MAX)
                {
                    seekFM();
                    usleep(100000);
                    printf("Stop FM Searching By Self 1\n");
                    emit searchEnd(RM_SEARCH_R_OK);
                    return;
                }
            }
            if(STATUS==6)//为0表示收到台
            {
                int channel = FREQ;
                printf("channel is:%d\n",channel);
                printf("channel_fm_tmp is:%d\n",channel_fm_tmp);
                if(channel_fm_tmp>=channel)
                {
                    seekFM();
                    usleep(100000);
                    emit searchEnd(RM_SEARCH_R_OK);
                    fmTmpMutex.lock();
                    channel_fm_tmp=0;
                    fmTmpMutex.unlock();
                    return;
                }
                else if(isFM(channel*10))//在FM范围内,超范围的台丢弃
                {
                    fmTmpMutex.lock();
                    channel_fm_tmp=channel;
                    fmTmpMutex.unlock();
                    emit searchedOneChannel(channel*10);
                    usleep(100000);
                    if(listenTest) //试听时搜多到一个频道即停止搜索 [zhenyubin 2015/05/04]
                    {
                        seekFM();
                        usleep(100000);
                        emit searchEnd(RM_SEARCH_R_OK);
                        fmTmpMutex.lock();
                        channel_fm_tmp=0;
                        fmTmpMutex.unlock();
                        printf("FM === === listen test end !!!!\n");
                        return;
                    }
                }
                else
                {
                    seekFM();
                    usleep(100000);
                    emit searchEnd(RM_SEARCH_R_OK);
                    return;
                }
            }
            else//收不到台表示收台完成
            {
                emit searchEnd(RM_SEARCH_R_OK);
                return;
            }
            //超出搜索范围后停止搜索
            if(FREQ*10>=RM_FM_FREQ_MAX)
            {
                seekFM();
                usleep(100000);
                printf("Stop FM Searching By Self 2\n");
                emit searchEnd(RM_SEARCH_R_OK);
                return;
            }
        }
        else
        {
            QApplication::processEvents();
            res =  seekAM();
            printf("++++++++++++++++++++res is++++++++++++++++\n");
            printf("%d\n",res);
            for(;;)
            {
                if(m_isSearching == false) //停止搜台
                {
                    seekAM();
                    usleep(100000);
                    printf("Stop AM Search By User!\n");
                    emit searchEnd(RM_SEARCH_R_OK);
                    return;
                }
                QApplication::processEvents();
                read_status();
                printf("AM len is:%d , id is:%d ,version is:%d ,statu is :%d , band is : %d , freq is : %d\n",LEN,ID,VER,STATUS,BADN,FREQ);
                //AM搜索开始值改为1620，若为522，会跳过522，直接从531开始，因此需要过滤掉开始的1620，1629
                if(firstTwoChannel <= 5)
                {
                    if((FREQ == 1620) || (FREQ == 1629))
                    {
                        wait_ms(15);
                        QApplication::processEvents();
                        firstTwoChannel++;
                        printf("Filter.....%d\n",FREQ);
                        continue;
                    }
                }
                firstTwoChannel++;

                if(STATUS==1)
                {
                    printf("resetseting......\n");
                }
                if(STATUS==2)
                {
                    printf("Tuning......\n");
                }
                if(STATUS==2)
                {
                    printf("Playing.......\n");
                }
                if(STATUS==3)
                {
                    printf("Searching......\n");
                }
                if(STATUS==4)
                {
                    printf("Error......\n");
                    break;
                }
                if(STATUS==5)
                {
                    printf("Not found.....\n");
                    break;
                }
                if(STATUS==6)
                {
                    printf("Finding.....\n");
                    break;
                }
                QApplication::processEvents();
                wait_ms(15);
                QApplication::processEvents();

                if(FREQ>=RM_AM_FREQ_MAX)
                {
                    seekAM();
                    usleep(100000);
                    printf("Stop AM Search By Self! 1\n");
                    emit searchEnd(RM_SEARCH_R_OK);
                    return;
                }
            }
            if(STATUS==6)//为0表示收到台
            {
                int channel = FREQ;
                printf("channel is:%d\n",channel);
                printf("channel_am_tmp is:%d\n",channel_am_tmp);
                if(channel_am_tmp>=channel)
                {
                    seekAM();
                    usleep(100000);
                    emit searchEnd(RM_SEARCH_R_OK);
                    amTmpMutex.lock();
                    channel_am_tmp=0;
                    amTmpMutex.unlock();
                    return;
                }
                else if(isAM(channel))//在AM范围内,超范围的台丢弃
                {
                    amTmpMutex.lock();
                    channel_am_tmp=channel;
                    amTmpMutex.unlock();
                    emit searchedOneChannel(channel);
                    usleep(100000);
                    if(listenTest) //试听时搜多到一个频道即停止搜索 [zhenyubin 2015/05/04]
                    {
                        seekAM();
                        usleep(100000);
                        emit searchEnd(RM_SEARCH_R_OK);
                        amTmpMutex.lock();
                        channel_am_tmp=0;
                        amTmpMutex.unlock();
                        printf("AM === === listen test end !!!!\n");
                        return;
                    }
                }
                else
                {
                    seekAM();
                    usleep(100000);
                    emit searchEnd(RM_SEARCH_R_OK);
                    return;
                }
            }
            else//收不到台表示收台完成
            {
                emit searchEnd(RM_SEARCH_R_OK);
                return;
            }
            if(FREQ>=RM_AM_FREQ_MAX)
            {
                seekAM();
                usleep(100000);
                printf("Stop AM Search By Self! 2\n");
                emit searchEnd(RM_SEARCH_R_OK);
                return;
            }
        }
    }
    emit searchEnd(RM_SEARCH_R_OK);
}

void si4754Radio::slotSearchChannel(void)
{
    unsigned char buffer[16]={'0'};
    int res = -1;
    if(m_isSearching)
    {
        if(m_radioMode == RM_FREQ_SECTION_FM)
        {
            QApplication::processEvents();
            res =  seekFM();
	    printf("++++++++++++++++++++res is++++++++++++++++\n");
	    printf("%d\n",res);
//	    si475x_read_current_status(8,buffer);
	    printf("+++++++++++++++++++++++++OK+++++++++++++++++\n");
//	    read_status();
	    for(;;)
	    {
            QApplication::processEvents();
		read_status(); 
        printf("FM len is:%d , id is:%d ,version is:%d ,statu is :%d , band is : %d , freq is : %d\n",LEN,ID,VER,STATUS,BADN,FREQ);
		if(STATUS==1)
		{
			printf("resetseting......\n");
		}
		if(STATUS==2)
		{
			printf("Tuning......\n");
		}
		if(STATUS==2)
		{
			printf("Playing.......\n");
		}
		if(STATUS==3)
		{
			printf("Searching......\n");
		}
		if(STATUS==4)
		{
			printf("Error......\n");
//			Reset();
			break;
		}
		if(STATUS==5)
		{
			printf("Not found.....\n");
			break;
		}
		if(STATUS==6)
		{
			printf("Finding.....\n");
			break;
		}			
		wait_ms(30);
	    }
#if 1
            if(STATUS==6)//为0表示收到台
            {
                int channel = FREQ;
	    	    printf("++++++++++++++++++get the fre++++++++++++++++++\n");
		printf("channel is:%d\n",channel);
		printf("channel_fm_tmp is:%d\n",channel_fm_tmp);
		if(channel_fm_tmp>=channel)
		{
		   printf("+++++++++=1+++++++++++\n");
		   emit searchEnd(RM_SEARCH_R_OK);
		   channel_fm_tmp=0;
		}
		else if(isFM(channel*10))//在FM范围内,超范围的台丢弃
                {
		    printf("++++++++++2+++++++++\n");
                    channel_fm_tmp=channel;
		    emit searchedOneChannel(channel*10);
                    QTimer::singleShot(100,this,SLOT(slotSearchChannel()));
                }
                else
                {
		    printf("+++++++++++++3++++++++++\n");
                    emit searchEnd(RM_SEARCH_R_OK);
                }
            }
            else//收不到台表示收台完成
            {
                	printf("++++++++++++++stop+++++++++++++++++\n");
		    emit searchEnd(RM_SEARCH_R_OK);
		    printf("+++++++++++stop1+++++++++++++++\n");
            }
#endif
        }
        else
        {
            QApplication::processEvents();
		res =  seekAM();	
		for(;;)
		{
            QApplication::processEvents();
			read_status();
                printf("AM len is:%d , id is:%d ,version is:%d ,statu is :%d , band is : %d , freq is : %d\n",LEN,ID,VER,STATUS,BADN,FREQ);
			if(STATUS==1)
			{
				printf("resetseting......\n");
			}
			if(STATUS==2)
			{
				printf("Tuning......\n");
			}
			if(STATUS==2)
			{
				printf("Playing.......\n");
			}
			if(STATUS==3)
			{
				printf("Searching......\n");
			}
			if(STATUS==4)
			{
				printf("Error......\n");
//				Reset();
				break;
			}
			if(STATUS==5)
			{
				printf("Not found.....\n");
				break;
			}
			if(STATUS==6)
			{
				printf("Finding.....\n");
				break;
			}
			wait_ms(30);
		}
            if(STATUS==6)//为0表示收到台
            {
		
                int channel = FREQ;
		if(channel_am_tmp>=channel)
		{
		   emit searchEnd(RM_SEARCH_R_OK);
		   channel_am_tmp=0;
		}
                else if(isAM(channel))//在AM范围内,超范围的台丢弃
                {
		    channel_am_tmp=channel;
                    emit searchedOneChannel(channel);
                    QTimer::singleShot(100,this,SLOT(slotSearchChannel()));
                }
                else
                {
                    emit searchEnd(RM_SEARCH_R_OK);
                }
            }
            else//收不到台表示收台完成
            {
                emit searchEnd(RM_SEARCH_R_OK);
            }
        }
    }
    else
    {
        emit searchEnd(RM_SEARCH_R_OK);
    }
}

void si4754Radio::readParameterConfig(void)
{
    QSettings set(RADIO_CONFIG_FILE_PATHNAME,QSettings::IniFormat,0);
    m_fmRssi = set.value("/fm_config/rssi",QVariant(m_fmRssi)).toInt();
    m_fmSnr = set.value("/fm_config/snr",QVariant(m_fmSnr)).toInt();
    m_amRssi = set.value("/am_config/rssi",QVariant(m_amRssi)).toInt();
    m_amSnr = set.value("/am_config/snr",QVariant(m_amSnr)).toInt();
	m_volumn = set.value("/fm_config/vol",QVariant(m_volumn)).toInt();
    m_volumn = 255;
	qWarning("[FM] RSSI= %d,SNR = %d  [AM] RSSI =%d ,SNR = %d m_volumn =%d\n",m_fmRssi,m_fmSnr,m_amRssi,m_amSnr,m_volumn);
}
int si4754Radio::readPatchFile(QString fileName,unsigned char **bufOut)
{
	int resCount=0;

	QFile f;
	QByteArray data;
	QList<QByteArray> line_data_list;
	QRegExp dataExp;
	QString sPartern = QString("^0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S),0x(\\S\\S)*");
	dataExp.setPattern(sPartern);

	f.setFileName(fileName);
	f.open(QIODevice::ReadOnly);
	data =f.readAll();
	f.close();
	line_data_list = data.split('\n');
	qWarning("[%s] [%d]\n",__func__,line_data_list.count());
	unsigned char * buf =(unsigned char * )malloc(line_data_list.count()*8);
	*bufOut = buf;
	unsigned char *p = buf;
	int res = 0;
	for(int i=0;i<line_data_list.count();i++)
	 {
		QString sData =line_data_list.at(i);
		//printf("%s\n",sData.toLocal8Bit().data());
		res = dataExp.indexIn(sData);
		//printf("res = %d\n",res);
		if(res>=0)
		{
			resCount++;
			for(int j=0;j<8;j++)
			{
				*p++ = (unsigned char)((dataExp.cap(j+1).toInt(0,16))&0xff);
				//printf("[%s][%d] ",(dataExp.cap(j+1)).toLocal8Bit().data(),*(p-1));
			}
			//printf("\n");
		}
	 }
	 printf("resCount=[%d]\n",resCount);
	 return resCount;

}

void si4754Radio::InitFM(void)
{
	printf("++++++++++++++++++++++++++++++InitFM()+++++++++++++++++++\n");
   	initFM(); 
#if 0
   setFmSnrRssi((u16)m_fmRssi,(u16)m_fmSnr);
   si475xFMRX_initialize();
   si475xFMRX_set_volume(m_volumn);
	//wait_ms(100);
	//si475xFMRX_mute(0);
#endif
}

void si4754Radio::InitAM(void)
{
	initAM();
#if 0
    setAmSnrRssi((u16)m_amRssi,(u16)m_amSnr);
    si475xAMRX_initialize();
    si475xAMRX_set_volume(m_volumn);
	//wait_ms(100);
	//si475xAMRX_mute(0);
#endif
}
void si4754Radio::closeFM(void)
{
//	si475xFMRX_mute(1);
//	si475xFMRX_powerdown();
}

void si4754Radio::closeAM(void)
{
//	si475xAMRX_mute(1);
//	si475xAMRX_powerdown();
}

void si4754Radio::changeFM(int channel)
{
	printf("+++++++++++si4754Radio::changeFM++++++++++++++\n");
//	si475xFMRX_tune(0,(u16)((channel/10)&0xffff));

    	change_fm((u16)((channel/10)&0xffff));
}

void si4754Radio::changeAM(int channel)
{
	printf("++++++++++++si4754Radio::changeAM++++++++++++++++++\n");
	change_am((u16)(channel & 0xffff));
//    si475xAMRX_tune((u16)(channel&0xffff));
}

int si4754Radio::seekFM(void)
{
  //  return si475xFMRX_seek(1,1);// seek up, 0 = stop at band limits

	return seek(1);
}

int si4754Radio::seekAM(void)
{
//    return si475xAMRX_seek(1,1);// seek up, 0 = wrap at band limits
	return seek(1);
}

void si4754Radio::mute(void)
{
#if 0
 	if(m_radioMode == RM_FREQ_SECTION_FM)
    {
        si475xFMRX_mute(1);
    }
    else if(m_radioMode == RM_FREQ_SECTION_AM)
    {
        si475xAMRX_mute(1);
    }
#endif
}

void si4754Radio::unMute(void)
{
#if 0
    if(m_radioMode == RM_FREQ_SECTION_FM)
    {
        si475xFMRX_mute(0);
    }
    else if(m_radioMode == RM_FREQ_SECTION_AM)
    {
        si475xAMRX_mute(0);
    }
#endif
}

bool si4754Radio::isFM(int channel)
{
    if(channel>= RM_FM_FREQ_MIN && channel <= RM_FM_FREQ_MAX)
        return true;
    return false;
}

bool si4754Radio::isAM(int channel)
{
    if(channel>= RM_AM_FREQ_MIN && channel <= RM_AM_FREQ_MAX)
        return true;
    return false;

}

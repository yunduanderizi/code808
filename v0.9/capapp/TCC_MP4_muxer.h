#ifndef _TCC_MP4_MUXER_H_
#define _TCC_MP4_MUXER_H_

#include "muxer.h"
#include "mp4_muxer.h"

enum MP4_MUXER_STATE_e
{
	MP4_STATE_READY = 0,
	MP4_STATE_AFTER_SEQUENCE_HEADER,
	MP4_STATE_AFTER_FIRST_FRAME,
	MP4_STATE_CLOSED
};

typedef struct tcc_mp4_muxer_context
{
	enum MP4_MUXER_STATE_e state;
	unsigned int MuxAudioDSI_Size;
	unsigned char MuxAudioDSI[256];

	mp4_mux_handle_t mux_handle;
	mp4_mux_init_t mux_init;
	mp4_mux_putinfo_t mux_putinfo;
	mp4_mux_error_t mux_error;
	mp4_mux_putstream_t putstream;
	mp4_mux_outstream_t outstream;
}TCC_MP4_MUXER_CONTEXT_t;


TCC_MP4_MUXER_CONTEXT_t* Mux_TCC_MP4_EncOpen(	unsigned char *FileName, unsigned char VideoCodecType, 
						unsigned char AudioCodecType, unsigned long m_ulSamplePerSec, unsigned long m_ulBitsPerSample, unsigned long m_ulChannels,
						unsigned int Width, unsigned int Height, char *MPEG4DSI,unsigned int MPEG4DSI_Size,
						char *XMuxAACDSI,unsigned int XMuxAACDSI_Size, unsigned int isUserData);
                                                        
int Mux_TCC_MP4_EncClose(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx);


int Mux_TCC_MP4_EncFrame(	TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, void* pBuff, unsigned int buffSize, 
									unsigned char KeyFrame, unsigned int dts);


int Mux_TCC_MP4_EncGPSInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, char *pGpsinfo, int size);
#if 0
//int Mux_TCC_MP3_EncGSensorInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, void *data);
int Mux_TCC_MP3_EncGSensorInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, void *data, unsigned int sample_num);

//int Mux_TCC_MP3_EncGSensorInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, GSENSOR_ARRAY *pGSensorinfo);
//int Mux_TCC_MP3_EncGSensorInfo(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int streamType, tcc_sensor_accel_t *pGSensorinfo);
int Mux_TCC_MP3_EncGSensorDetected(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx, int isDetected);
#endif

long long Mux_TCC_MP4_GetFileSize(TCC_MP4_MUXER_CONTEXT_t* mp4_ctx);


#endif //_TCC_MP4_MUXER_H_

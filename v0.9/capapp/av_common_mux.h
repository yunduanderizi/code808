/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2009-2011 Telechips Inc.
  19~23 Floor, Luther Building, 7-20 Sincheon-dong, Songpa-gu, Seoul, Korea					
 \endverbatim
 ***********************************************************************
 */
/*!
 ***********************************************************************
 *
 * \file
 *		av_common_mux.h
 * \date
 *		2009/09/02
 * \author
 *		AV algorithm group (AValgorithm@telechips.com) 
 * \brief
 *		common header of muxer
 * \version
 *		1.0.2 : 2009/10/21 
 *			- av_common_dmx.h was removed and av_common_types.h and av_common_fcc_tag.h was included.
 *		1.0.1 : 2009/10/08
 *		1.0.0 : 2009/09/02
 *
 ***********************************************************************
 */
#ifndef _AV_COMMON_MUX_H_
#define _AV_COMMON_MUX_H_

#include "av_common_fcc_tag.h"
#include "av_common_types.h"

typedef av_handle_t	av_mux_handle_t; //!< the type of a muxer's handle
typedef av_result_t	av_mux_result_t; //!< the type of a muxer's result


/*!
 ============================================================
 *
 *	Constant Definition
 *
 ============================================================
*/

//! Muxer OP Code
#define MUXOP_OPEN				 0 //!< muxer open (initialize)
#define MUXOP_CLOSE				 1 //!< muxer close
#define MUXOP_PUT_STREAM		 2 //!< get stream
#define MUXOP_GET_LASTERROR		 3 //!< get last error


//! Muxer Return Value
#define MUXRET_SUCCESS			 0
#define MUXRET_FAILED			-1
#define MUXRET_END_OF_STREAM	-2
#define MUXRET_INVALID_HANDLE	-3
#define MUXRET_FILE_OPEN_FAILED	-4

//! Stream Type
#define MUXTYPE_NONE			 0
#define MUXTYPE_VIDEO			 1
#define MUXTYPE_AUDIO			 2
#define MUXTYPE_SUBTITLE		 4
#define MUXTYPE_GRAPHIC			 8








#endif//_AV_COMMON_MUX_H_
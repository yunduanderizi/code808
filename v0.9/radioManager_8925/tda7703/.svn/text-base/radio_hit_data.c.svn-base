typedef struct Band_conststuct {
	unsigned int BandFreq;
	unsigned int MemFreq[6];
	unsigned char CurrentMem;
} BandConstStuct;
typedef struct Area_stuct {
	BandConstStuct Band[6];
	unsigned char FMStep;
	unsigned char FMSeekStep;
	unsigned int FMMaxFreq;
	unsigned int FMMinFreq;
	unsigned char FMStepOirt;
	unsigned int FMMaxFreqOirt;
	unsigned int FMMinFreqOirt;
	unsigned char AMStepMW;
	unsigned int AMMaxFreqMW;
	unsigned int AMMinFreqMW;
	unsigned char AMStepLW;
	unsigned int AMMaxFreqLW;
	unsigned int AMMinFreqLW;
     unsigned int AMStepSW;
     unsigned int AMMaxFreqSW;
     unsigned int AMMinFreqSW;
	unsigned char FMStepWB;
	unsigned int FMMaxFreqWB;
	unsigned int FMMinFreqWB;
} AreaStuct;

static const AreaStuct Area[]=
{
	// USA
	8750,8750,9010,9810,10610,10790,8750,0,
	8750,8750,9670,10490,10690,10790,8750,0,
	//162400, 162425, 162450, 162475, 162500, 162525,
	62400, 62400, 62425, 62450, 62475, 62500, 62525, 0,//Weather band
	//8750,8750,9670,10490,10690,10790,8750,0,
	530,530,600,1000,1600,1720,530,0,
	530,530,600,1000,1600,1720,530,0,	
	//1730KHz,(xxx-1384)*5KHz,..., 25080KHz
    1730,1730,2000,3000,4000,5000,6400,0, /* sw frequency */
	20,20,10790,8750,
	0,0,0,
	10,1720,530,
	0,0,0,
 	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	25,62550,62400,

	// LATIN
	8750,8750,9010,9810,10610,10800,8750,0,
	8750,8750,8750,8750,8750,8750,8750,0,
	////162400, 162425, 162450, 162475, 162500, 162525
	//2400, 2400, 2425, 2450, 2475, 2500, 2525, 0,//Weather band
	8750,8750,9010,9810,10610,10800,8750,0,
	520,520,600,1000,1400,1620,520,0,
	520,520,520,520,520,520,520,0,
	//1730KHz,(xxx-1384)*5KHz,..., 25080KHz
    1730,1730,2000,3000,4000,5000,6400,0, /* sw frequency */
	10,10,10800,8750,
	0,0,0,
	10,1620,520,
	0,0,0,
 	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,
	//25,2550,2400,

	// EUROPE
	8750,8750,9510,9810,10610,10800,9120,0,
	8780,8780,9180,9390,9710,10300,10430,0,
	8750,8750,8750,8750,8750,8750,8750,0,
	//522,522,603,999,1404,1620,522,0,
	522,522,846,999,1404,1620,522,0,
	//522,522,603,999,1404,1620,522,0,
	144,144,171,216,270,288,144,0,//---tempory remove
	//1730KHz,(xxx-1384)*5KHz,..., 25080KHz
    1730,1730,2000,3000,4000,5000,6400,0, /* sw frequency */
	5,10,10800,8750,
	//5,5,10800,8750,
	0,0,0,
	9,1620,522,
 	9,288,144,
 	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,

	// OIRT
	6500,6500,8750,9000,9800,10600,10800,0,
	6500,6500,8750,9000,9800,10600,10800,0,
	6500,6500,8750,9000,9800,10600,10800,0,
	531,531,603,999,1404,1620,531,0,
	531,531,603,999,1404,1620,531,0,
	//1730KHz,(xxx-1384)*5KHz,..., 25080KHz
    1730,1730,2000,3000,4000,5000,6400,0, /* sw frequency */
	//144,144,171,216,270,288,144,0,
	5,10,10800,8750,
	3,7400,6500,
	9,1620,522,
 	9,288,144,
 	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */
	0,0,0,
 	
 	// JAPAN
	7600,7600,7820,7900,8000,8500,9000,0,
	7600,7600,7900,8200,8500,8800,9000,0,
	7600,7600,7950,8250,8550,8850,9000,0,
	531,531,603,999,1404,1620,531,0,
	531,531,603,999,1404,1620,531,0,
	//144,144,171,216,270,288,144,0,
	//1730KHz,(xxx-1384)*5KHz,..., 25080KHz
    1730,1730,2000,3000,4000,5000,6400,0, /* sw frequency */
	5,10,9000,7600,
	0,0,0,
	9,1620,522,
 	//9,288,144,
 	0,0,0,
 	1,6400,1730,            /* sw step freq(5k), max freq setting(5k), min freq(5k), */ 	
	0,0,0,
};

#if HIT_ADA//ada version

	#ifdef HIT_64
		//#include "Radio_hit_340data.c"
	#endif

	#ifdef HIT_44
	//	#include "radio_hit44_330rc23_data.c"
	#endif

#endif

#if HIT_ADB //adb version
	#ifdef HIT_64
	//	#include "Radio_hit64_430data.c"
	#endif

	#ifdef HIT_44
		#include "Radio_hit44_417data.c"
	#endif
#endif


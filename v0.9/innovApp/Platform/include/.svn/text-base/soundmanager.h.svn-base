#pragma once
/**
  @file
  Contains interface declarations for a SoundManager interface, 
  which can be used to call functions to play sound. 

  Copyright (C) 2005-2007, Navman Technology New Zealand Ltd.  All rights Reserved.
*/

#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "multimedialib.h"
//#ifdef HAS_FEATURE_DDG2P
// MapCore
#include <language/laInit.h>            // for TLanguageCode
//#endif // HAS_FEATURE_DDG2P

namespace navman {

  namespace multimedia {

    class ISoundSource;

    /**
     * Default definitions.
     */
    #define DEFAULT_TTS_LANGUAGE  L"British English"
    #define DEFAULT_TTS_VOICE     L"Emily"
    #define SOUND_PLAYING_EVENT_NAME  L"SmartST_SoundPlayingEvent"
    #define SOUND_PAUSE_EVENT_NAME    L"SmartST_SoundPauseEvent"


    /**
      interface receiving SoundManager events
    */
    class ISoundManagerEventListener : public IObject
    {
    public:

      /**
        called when volume is set.
        @param volume the volume.
      */
      virtual void onVolumeSet(UINT16 volume) = 0;

    };

    /**
      interface used to register for SoundManager events
    */
    class ISoundManagerEventSource : public IObject
    {
    public:

      /**
        registers a listener for events
        @param pListener listener to add
        @return true on success, false otherwise
      */
      virtual bool addListener(ISoundManagerEventListener *pListener) = 0;

      /**
        unregisters a listener so it won't receive more events
        (though if events are still pending it may receive them)
        @param pListener listener to remove
        @return true on success, false otherwise
      */
      virtual bool removeListener(ISoundManagerEventListener *pListener) = 0;

    };

    /**
       SoundManager singleton Interface.
       This component acts as an access point for functionality contained within the 
       multimedia library. It has functions for playing sounds and prompts, and 
       for configuring TextToSpeech.
    */
    class ISoundManager : public ISoundManagerEventSource
    {
    public:

      /**
        Different input lines for different types of sounds. Negative
        values are used to identify direct lines into the mixer that
        do not utilise queuing, so are used for instant alerts.
      */
      typedef enum
      {
        System = -1,
        Alert = System,
        Guidance = 0,
        Phone = 1,
        SMS = Phone,
        Music = 3
      } StreamType;

      /**
        Different attenuations for different sound streams. These attenuations
        will be used when multiple sounds are playing simultaneously.
      */
      typedef enum
      {
        AlertAttenuation = 0,             /// 1st (highest) priority, no attenuation.
        GuidanceAttenuation = 0,          /// 2nd priority, no attenuation.
        PhoneAttenuation = -1000,         /// 3rd priority, -1000 attenuation.
        MusicAttenuation = -2000          /// 4th priority, -2000 attenuation.
      } StreamAttenuation;

      /**
        Different sound file types.
      */
      typedef enum
      {
        WAVFile,
        PCMFile
      } SoundFileType;

      /**
        Plays a given sound file. If the file does not exist then nothing is
        played.
        @param filePath The name of the sound file to play. 
        @param fileType The type of sound file. e.g. .wav, .pcm, etc.
        @param streamType The sound stream that will be used to play the sound.
        @return true on success, false otherwise. 
      */
      virtual bool play( const WCHAR *filePath, 
                         const SoundFileType fileType, 
                         const StreamType streamType ) = 0;

      /**
        Plays speech based on the text passed into the function using the current
        TTS voice.  The text string is first checked to ensure that it can safely
        be rendered by the TTS engine, if not, the function returns false.
        @param text The text that is to be played as speech.
        @param streamType The sound stream that will be used to play the sound.
        @param priority The priority of the sound. This determines the position at 
        which the sound will be inserted into the list relative to other sounds 
        in the sound stream.
      */
      virtual bool speak( const WCHAR *text, 
                          const StreamType streamType,
                          const UINT32 priority ) = 0;

      /** 
        Plays a legacy guidance prompt.
        @param source The source data to be used to play the prompt.
        @param priority The priority of the sound. This determines the position at 
        which the sound will be inserted into the list relative to other sounds 
        in the sound stream.
      */
      virtual bool speak( ISoundSource * source, const UINT32 priority ) = 0;

      /** 
        Set the TextToSpeech voice that will be used for reading text and guidance prompts.
        @param language The language of the desired TTS voice. e.g. L"Australian English".
        @param voiceName The name of the TTS voice. e.g. L"Karen".
        @return true on success, false otherwise.
      */
      virtual bool setTTSVoice( const WCHAR *language, const WCHAR *voiceName ) = 0;

//#ifdef HAS_FEATURE_DDG2P
      /**
        Add phonetic dictionary.  Only one dictionary can be added per language code.
        @param languageCode MapCore language code.
        @param dictionary VAD/VDE phonetic dictionary for given locale.
        @return true on success, false otherwise.
      */
      virtual bool registerDictionary( TLanguageCode languageCode, const WCHAR* dictionaryFileName ) = 0;

      /**
        Drop all phonetic dictionaries registered with addDictionary().
        @return true on success, false otherwise.
      */
      virtual bool unregisterAllDictionaries( void ) = 0;
//#endif // HAS_FEATURE_DDG2P

      /** 
      Retrieves the current system volume.
      @return current system volume
      */
      virtual UINT16 getVolume( void ) = 0;

      /** 
        Changes the volume of the platform's audio output device. 
        Changes in output levels take effect immediately.
        @param volume 16-bit integer specifying volume level for the audio output 
        device. Valid values range from 1 (minimum) to 100 (maximum). 
        @return true on success, false otherwise.
      */
      virtual bool setVolume( const UINT16 volume ) = 0;

      /**
        Sets the text string to be used to calculate the volume level / attenuation
        look up table. This function MUST be called BEFORE the first setVolume
        is called.
        @param volumeTable Comma separated volume table.
        @return true on success, false otherwise.
      */
      virtual bool setVolumeTable( const WCHAR * volumeTable ) = 0;

      /**
        Guidance prompts are muted when this is set.
        @param mute Set to true to mute guidance, false to unmute.
      */
      virtual void muteGuidance( bool mute ) = 0;
      
	  /**
	    Return the mute state of voice guidance.
	  */
      virtual bool getMuted( void ) = 0;

      /**
        Clear the sound stream.
        @param stream The stream to be cleared.
        @return true on success, false otherwise.
      */
      virtual bool clearStream( const StreamType stream ) = 0;

      /**
        Returns the voice availability
      */
      virtual bool getAvailability( void ) const = 0;

      /**
        Sets the voice availability
      */
      virtual void setAvailability( bool succeeded ) = 0;

      /**
        Sets the sounds of auxiliary applications to quiet or not quiet.
        @param quiet Set to true to quiet auxiliary applications, false to re-play sound.
      */
      virtual bool attenuate(bool quiet) = 0;
    };

    // Access to Sound Manager singleton - Not exposing a creator class forces 
    // the object to be used as a singleton.
    ISoundManager* SoundManager( void );

  } // namespace multimedia
} // namespace navman

#endif // _SOUNDMANAGER_H_

#include "innovEngine_sdk.h"
#include "platformTTS.h"
#include <dlfcn.h>

/*----------------------------------------------------------------*/
/* Loads a grammar library. */
void* platformLoadGrammar(const char *fname, TGrammarInterface *grammarInterface) {

  assert( fname && grammarInterface );
  memset( grammarInterface, 0, sizeof(TGrammarInterface) );

  void* libHandle;
  if ((libHandle = dlopen(fname, RTLD_LAZY)) == NULL)
    return NULL;

  if ((grammarInterface->characterAsTokens = (TLangLibCharacterAsTokens*) dlsym (libHandle, "langlib_characterAsTokens")) == NULL)
    return NULL;
  if ((grammarInterface->getGrammarInfo = (TLangLibGetGrammarInfo*) dlsym (libHandle, "langlib_getGrammarInfo")) == NULL)
    return NULL;
  if ((grammarInterface->guideInstructionsAsText = (TLangLibGuideInstructionsAsText*) dlsym (libHandle, "langlib_guideInstructionsAsText")) == NULL)
    return NULL;
  if ((grammarInterface->guideInstructionsAsTokens = (TLangLibGuideInstructionsAsTokens*) dlsym (libHandle, "langlib_guideInstructionsAsTokens")) == NULL)
    return NULL;
  if ((grammarInterface->isSpeechFileCompatible = (TLangLibIsSpeechFileCompatible*) dlsym (libHandle, "langlib_isSpeechFileCompatible")) == NULL)
    return NULL;
  if ((grammarInterface->init = (TLangLibInit*) dlsym (libHandle, "langlib_init")) == NULL)
    return NULL;
  if ((grammarInterface->trafficInstructionAsTokens = (TLangLibTrafficInstructionAsTokens*) dlsym (libHandle, "langlib_trafficInstructionAsTokens")) == NULL)
    return NULL;
  if ((grammarInterface->tokenAsText = (TLangLibTokenAsText*) dlsym (libHandle, "langlib_tokenAsText")) == NULL)
    return NULL;
  if ((grammarInterface->phraseAsText = (TLangLibPhraseAsText*) dlsym (libHandle, "langlib_phraseAsText")) == NULL)
    return NULL;
  if ((grammarInterface->countryNameAsText = (TLangLibCountryNameAsText*) dlsym (libHandle, "langlib_countryNameAsText")) == NULL)
    return NULL;
  grammarInterface->initLookup = (TLangLibInitLookup*) dlsym (libHandle, "langlib_initLookup");

  return libHandle;
}

void platformUnloadGrammar(void* handle)
{
}


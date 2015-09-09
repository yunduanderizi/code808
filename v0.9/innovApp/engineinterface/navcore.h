#ifndef NAVCORE_H
#define NAVCORE_H

#define AUTH_BUFFER_SIZE     (1024 * 64)
#define CDKEYS_FILENAME      "cdkeys.dat"
#define NUM_COLOURS          256

enum{NO_CROSSHAIR,MOVING_CROSSHAIR,FIXED_CROSSHAIR};

#include <innovEngine_sdk.h>

class navcore
{
public:
  navcore();
  virtual ~navcore();
  void init();
  void createcontext();
  bool initContext( ); //initialize the graphics context
  bool initViewport( ); //initialize the viewPort
//  void setConfigFile(const char *configFile){strcpy(m_configFile,configFile);};
//  char *getConfigFile(){return m_configFile;};

  void saveViewport( );
  TViewport *getViewport();
  void set3DViewport( );
  void set2DViewport( );

  WORLD_RENDERPARAMS renderParams;
  TBitmap bitmap;
  TGraphContext context; //graphics context, will be used mainly by navcoreDisplay
  TViewport viewport;

protected:
  UINT32 oldLength;
  UINT8  *screen;
  UINT8 colourDepth;

private:

//  char m_configFile[MAX_PATH];
  TFontCollection fontCollection;
  TGraphExtents graph_extents[2048]; // should be set to the rasterization surface's height
};

#endif // NAVCORE_H

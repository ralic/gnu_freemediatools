/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#ifndef VFXPLUGIN_H
#define VFXPLUGIN_H

#include "jahplugintemplate.h"
#include <qstring.h>

class jahPlugin;

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

/*
extern "C"   {
jahPlugin* getPluginClass();
}
*/
 

namespace PLUGIN
{
  namespace CSFISHEYE {


    MY_EXPORT jahPlugin* getPluginClass();

class MyPlugin : public jahPlugin
{

public:
    MyPlugin()
    {
        initSettings();

        initializePlugin();
    };

    ~MyPlugin()
    {
        debug(">>DeBooted the plugin");
    };

    void   initializePlugin();

    void   processImage(void);


};

  };

};



#endif

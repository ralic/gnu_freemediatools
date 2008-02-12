/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#ifndef VFXPLUGIN_H
#define VFXPLUGIN_H

#include "config.h"
#include "jahplugintemplate.h"
#include <qstring.h>

#include <GL/glew.h>
#include "gpumathlib.h"

class jahPlugin;

extern "C" {  RFXNVDISTORTION_API jahPlugin* getPluginClass();  };

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
        //debug(">>DeBooted the plugin");
    };

    void   initializePlugin();

    void   processImage(void);
    void   processGpuFx(void);
    void   processGpuArbFx(void);

    //variables for effect
    int*        permutation_table;
    float4*     gradient_table;
};




#endif

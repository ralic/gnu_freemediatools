/*******************************************************************************
**
** A Jahshaka JitFx Plugin
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

#include <math.h>
#include <stdlib.h>

class jahPlugin;

extern "C" {  JITPSEUDOCOLOR_API jahPlugin* getPluginClass();  };

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

    void   setImage(QImage theimage) { vfximage = new QImage (theimage); }
    QImage getImage(void)            { return *vfximage; }
    void   processImage(void);

//private:

    QString data;
    

};




#endif

/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahcreate.h"
#include "jahtracer.h"
#include "jahtranslate.h"
#include "jahpreferences.h"
#include "themes.h"

void JahControl::initializeVariables()
{
	//////////////////////////////////////////////////////////
	//set up pointers to singletons for full class access
   
    jtrace = JahTrace::getInstance();	//set up tracer
    jt = JahTranslate::getInstance();	//set up translator

   	/////////////////////////////////////////////////////////////////
    //ok lets load the plugins in now...
    jtrace->info( ">Initializing Variables");

	//////////////////////////////////////////////////////////
    //initialize jahthemes
    jahthemes = new JahThemes ();
    
	//////////////////////////////////////////////////////////
    //initialize JahBasePath & Mediapath
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath   = jprefs.getBasePath().data();
    
    jtrace->info( "JahBasePath",JahBasePath);

    JahMediaPath  = jprefs.getMediaPath().data();

    jtrace->info( "JahMediaPath",JahMediaPath);

    JahDefaultRes = jprefs.getJahResolution();




	//////////////////////////////////////////////////////////
    //this is a global variable used by all modules
	//to keep track of the latest clip number
    globalclipnumber = 0;

	//used to keep track of modules when adding them
	module_counter = 0;

    //a flag for grabbing desktop key
    getKeyStatus = false;
    setActiveModuleButton(NULL);

	desktopButtonStatus = false;
}

void JahControl::initalizeLibraries()
{
}


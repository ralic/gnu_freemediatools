/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVBLUR_CONFIG_INC_
#define RFXNVBLUR_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVBLUR_EXPORTS
		#define RFXNVBLUR_API __declspec( dllexport )
	#else
		#define RFXNVBLUR_API __declspec( dllimport )
	#endif // RFXNVBLUR_EXPORTS
#else
	#define RFXNVBLUR_API
#endif // WIN32

#endif // RFXNVBLUR_CONFIG_INC_

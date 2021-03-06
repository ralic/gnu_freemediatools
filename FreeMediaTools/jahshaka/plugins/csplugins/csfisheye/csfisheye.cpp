/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "csfisheye.h"
#include <math.h>

/////////////////////////////////////////////////////////
// external linked clearspeed routine
extern "C" void fisheye_main(float xcenter, float ycenter, float fisheye_radius, float gamma, int image_width, int image_height);

////////////////////////////////////////////////////////
// clearspeed settings

//#define    NUMROWS    1024
//#define    NUMCOLS    1024

extern struct pixel_t               coordinate_out_buffer[1024][1024];
extern char                         char_out_buffer[1024][1024];

struct pixel_t
{
	int        x;
	int        y;
};

///////////////////////////////////////////////////
// Mandatory routine used to return instance of
// plugin to calling application


namespace PLUGIN
{
  namespace CSFISHEYE {
    jahPlugin* getPluginClass()
      {
	return new MyPlugin();
	//return new MyPlugin;
      }

//////////////////////////////////////////////////
// Plugin image subroutines start here

void    MyPlugin::initializePlugin(void)
{
    pluginName    = "CSfisheye";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;                  //class of plugin ie CPU, GPU, MeshFX

    QString guid = "9d373148-dc36-11d9-9475-00e08161165f";
    setGuid(guid);

    //we are using 1 slider
    usesliders = true;
    numsliders = 3;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X";
    sliderlabel[1]="Y";
    sliderlabel[2]="R";

    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Fisheye Fx";
}

//////////////////////////////////////////////////
// Plugin image subroutines start here

void    MyPlugin::processImage(void)
{
    int                                            width;
    int                                            height;
    int                                            row;
    int                                            column;

    float                                        fisheye_radius;
    float                                        xcenter;
    float                                        ycenter;
    float                                        gamma;

    QImage newimage;
    newimage = vfximage->copy();
    
    height = Height;
    width = Width;

    xcenter = slider[1] + ((float)width / 2.0);
    ycenter = slider[2] + ((float)height / 2.0);
    fisheye_radius = (slider[4] * 2.0) + 2.0;
    gamma  = (slider[5] * 0.01) + 2.0;

    //fxtimer->starttime();

    fisheye_main(xcenter, ycenter, fisheye_radius, gamma, width, height);

    for (row = 0; row < height; row++)
    {
        for (column = 0; column < height; column++)
        {

            if (row == 256 && column == 256)
                printf("row = %d  column = %d  coordinate_out_buffer[row][column].x = %d  coordinate_out_buffer[row][column].y = %d\n",
                    row, column, coordinate_out_buffer[row][column].x, coordinate_out_buffer[row][column].y);

            if ( (coordinate_out_buffer[row][column].x > 0) && (coordinate_out_buffer[row][column].x < width) &&
                (coordinate_out_buffer[row][column].y > 0) && (coordinate_out_buffer[row][column].y < height))
            {

                uint* pnew = (uint *)newimage.scanLine(row) + column;
                uint* pold = (uint *)vfximage->scanLine(coordinate_out_buffer[row][column].y) + coordinate_out_buffer[row][column].x;
                *pnew = *pold;

            }
        }
    }


    *vfximage = newimage;

    //fxtimer->endtime();
    //fxtimer->printtime("Time for CS301 fisheye: ");

}

};

};

/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvpan.h"
#include <math.h>

void    MyPlugin::isComplexArbSupported(void)
{
    QString sourceName = tempBasePath+"plugins/rfxplugins/rfxnvpan/pan_frag_arb_gpu.fp";
    HASARB =  checkComplexArbSupport(sourceName, max_fp_instructions, max_vp_instructions );
}


void    MyPlugin::processGpuArbFx(void)
{
    ////////////////////////////////////////////
    //main imageprocessing loop

    float                                       horizontal;
    float                                       vertical;

    GLint                                       width;
    GLint                                       height;

    float2                                      scale_factor;

    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;

    unsigned char*                              vertex_program;
    unsigned char*                              fragment_program;

    static int                                  count = 0;

    //////////////////////////////////////////////////////////
    //we need to send over...

    width  = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

    //////////////////////////////////////////////////////////

    horizontal = slider[1];
    vertical = slider[2];

    scale_factor.x = texwidthratio;
    scale_factor.y = texheightratio;

    //////////////////////////////////////////////////////////
    {
        vertex_program = loadshaderfile(JahBasePath+"rfxcore/jahshaka_basic_vert_arb.vp");

        if (!vertex_program)
        {
            return;
        }
    }

    loadJahshakaBasicArb(width, height, camera_distance,vertex_program, vertex_program_handle);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,  getSrcTextureId() );

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxnvpan/pan_frag_arb_gpu.fp");

        if (!fragment_program)
        {
            return;
        }
    }

    glGenProgramsARB(1, &fragment_program_handle);
    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragment_program_handle);
    glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, "pan_frag_arb_gpu.fp");
    }

    /////////////////////////////////////////////////////////

    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, horizontal, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, vertical, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 2, width, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 3, height, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 4, scale_factor.x, scale_factor.y, 0.0f, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_ARB);

    //////////////////////////////////////////////////////////////////////////
    glBegin(GL_QUADS);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio, 0.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio,
                                texheightratio, 0.0, 1.0);
            glVertex2f( float_width / 2.0, float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0,
                                texheightratio, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, float_height / 2.0);


    glEnd(); //GL_QUADS

    ////////////////////////////////////////////////////////////////////////

    glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);

    /////////////////////////////////////////////////////////////////////

    glDisable(GL_VERTEX_PROGRAM_ARB);
    glDisable(GL_FRAGMENT_PROGRAM_ARB);

    delete [] vertex_program;
    delete [] fragment_program;

    count++;

    glDeleteProgramsARB(1, &vertex_program_handle);
    glDeleteProgramsARB(1, &fragment_program_handle);
}




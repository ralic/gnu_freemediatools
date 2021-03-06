/*******************************************************************************
**
** File part of the 'timeline' project.
** Copyright (C) 2005-2006 Visual Media Fx Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
/*
  this code was broken-
  fixed it with help from the file:
source/Jahshaka/JahDesktop/desktop/desktopcanvas.cpp
*/
#include <qapplication.h>
#include <qpainter.h>
#include <qfont.h>
#include <qtimer.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qcursor.h>

#include <qpalette.h>
#include <qstring.h>
#include <math.h>

#include "ruler.h"
#include "cursor-xpms.h"

Ruler::Ruler( QWidget* parent, const char* name ) : QWidget( parent, name ), _startFrame( 0.0 ), _nowframe( 0 ),
                    _pixelsPerFrame( 1.232572 ), _majorTickHeight( 10 ), _minorTickHeight( 5 ), 
                    _dragging( false ), _draggingnow( false ), _dropFrame( false ), _fps( 25 )
{
    _maxTicksPerPixel = 25.0/675.0; // seems like a good number. Need to be sure labels dont clash.

    // make sure external widgets know the initial posn of the now line.
    emit nowStringChanged( frameToString( _nowframe ) );
    emit nowChanged ( _nowframe );

    //setMouseTracking( true );

    mustGenerateBackground = true;
    mustDetermineTicks = true; 
    _drawfancybg = true;  // TODO remove hard code of this!

    // important: we might want to do all our own painting, incl clearing.
    if ( _drawfancybg )
        setWFlags( WNoAutoErase );

    // TODO change this section, too much hardcoded stuff, just use the palette?
    bool invert = false;
    if ( palette().color( QPalette::Active, QColorGroup::Foreground ) == QColor( Qt::white) )
        invert = true;

    if ( _drawfancybg )
    {   
        if ( invert )
        {
            _uppercol = QColor( "gray15" ); 
            _lowercol = QColor( "gray10" );
            _foregroundcol = QColor( Qt::white );
            _highlightcol = QColor( "gray20" );
            _framecol = QColor ( Qt::black );
            _nowmarkercol = QColor( 244, 180, 57 ); // orange.  was( 0, 4, 57 );
            _nowmarkerpencol = QColor ( Qt::black );
        }
        else
        {
            _uppercol = QColor( "gray95" ); 
            _lowercol = QColor( "gray90" );
            _foregroundcol = QColor( Qt::black );
            _highlightcol = QColor ( Qt::white );
            _framecol = QColor ( Qt::black );
            _nowmarkercol = QColor ( "gray75" );
            _nowmarkerpencol = colorGroup().text();
        }
    }
    else
    {   _uppercol = colorGroup().background();
        _lowercol = colorGroup().background();
        _foregroundcol = colorGroup().foreground();
        _highlightcol = QColor ( Qt::white );
        _framecol = _foregroundcol;
        _nowmarkercol = _uppercol;
        _nowmarkerpencol = colorGroup().text();
    }

    // set palette to (hopefully) hint to Qt not to use stupid colors for wipes before paint event.
    // TODO this doesnt get updated if we set palette later, so it flickers in that case.
    QPalette p = this->palette();
    QColorGroup cg;
    p.setColor(QColorGroup::Base, _uppercol );
    p.setColor(QColorGroup::Background, _uppercol); 
    p.setColor(QColorGroup::Foreground, _foregroundcol); 
    this->setPalette(p);

    setFramesPerSecond( 25 ); 

    setMaxFrames( framesPerSecond() * 60 * 10 ); // 10 minute project.

    //_nowpixmap = QPixmap( "slider_thumb_on.png" );// Testing
    _nowsize = 8;
    _nowsizehalf = 4;
    _nowxpos = 50;

}

Ruler::~Ruler()
{   // nothing extra to do.
}

void Ruler::setFramesPerSecond( int num )
{
    if ( num >= 0 )
    {
        _fps = num; 
        _mspf = 1000.0 * double( 1/ double(_fps) );
    }
}

void Ruler::setTimeCodeFromString( const QString & tcstring )
{
    // assumes first number is the fps, then checks later for dropframe.
    QString str = tcstring.stripWhiteSpace().upper();
    QString qfps = str;
    qfps.remove("FPS").remove("DF");
    //printf("setTimeCodeFromString input str is %s  str is %s \n", tcstring.latin1(), str.latin1() );

    bool ok; 
    int fps = qfps.toInt( &ok );
    if ( !ok || fps < 1 || fps > 1000 )
        return;
    
    if ( fps != framesPerSecond() )
    {   setFramesPerSecond( fps );
        //printf( "setTimeCodeFromString fps %i \n", fps );
    }
    if ( str.contains( "DF" ) )
    {    setDropFrame( true );
         //printf( "dropframe TRUE \n" );
    }
    else
    {    setDropFrame( false );
         //printf( "dropframe FALSE \n" );
    }
    
    // Force a full repaint, ticks will have changed.
    mustDetermineTicks = true; 
    mustGenerateBackground = true;
    repaint();
}

/* // TO TEST
void Ruler::updateNowLinePixel( double x )
{
    printf ( "updateNowLinePixel old %f new %f \n", _nowframe, x );
    _nowframe = x / _pixelsPerFrame;
    repaint();
}
*/

void Ruler::genBackground()
{
    if ( mustDetermineTicks )
       determineTicks();
    mustDetermineTicks = false;

    bg = QPixmap( size() );
    bg.fill(colorGroup().base() ); 
   
    QPainter paint( &bg );    

    // draw a fancy bg if we want to
    if ( _drawfancybg )
    {   paint.setBrush( _uppercol );
        paint.setPen( Qt::NoPen );
        paint.drawRect ( 1, 1, width() - 2, height()/2 );

        paint.setBrush( _lowercol  );
        paint.drawRect ( 1, height()/2, width() -2, height()/2  );

        // Draw a 2 pixel highlight at the top (just below the frame)
        paint.setPen( _highlightcol );
        paint.drawLine( 0, 1, width(), 1 );
        paint.drawLine( 0, 2, width(), 2 );
    }

    // paint the tick marks
    paint.setPen( _foregroundcol );
    paintTickMarks( paint );
    
    // Finally, frame drawn on top.
    paint.setPen( _framecol );
    paint.setBrush( Qt::NoBrush  );
    paint.drawRect ( 0, 0, width(), height() );
 
    //NO!  paint.end();
    mustGenerateBackground = false;
}

void Ruler::paintTickMarks( QPainter &painter )
{
    if ( _majorTickFrames == 0 || _minorTickFrames == 0 )
        return;

    // Major ticks.
    double frame = _firstMajorTickFrame; 
    double endFrame = _startFrame + double(width())/_pixelsPerFrame;
    //printf( "V2 _startFrame %f endFrame %f _pixelsPerFrame %f \n", _startFrame, endFrame, _pixelsPerFrame );
    while ( frame <= endFrame )
    {
        paintFrameTick( painter, frame ); // TODO frame was INT!!
        frame += _majorTickFrames;
        //printf("painted frame tick %i \n", frame );
    }
    // Minor ticks
    frame = int(_firstMinorTickFrame);
    endFrame = _startFrame + double(width())/_pixelsPerFrame;
    //printf( "V2 _startFrame %f endFrame %f _pixelsPerFrame %f \n", _startFrame, endFrame, _pixelsPerFrame );
    while ( frame <= endFrame )
    {
        paintFrameTick( painter, frame, true );
        frame += _minorTickFrames;
        //printf("painted frame tick %i \n", frame );
    }
    return;
}

void Ruler::paintFrameTick( QPainter &painter, double frame, bool minor )
{
    double xposd = ( frame - _startFrame ) * _pixelsPerFrame;
    int xpos = int( xposd );
    int ypos;
    if ( minor )
        ypos = height() - _minorTickHeight;
    else
        ypos = height() - _majorTickHeight;

    painter.drawLine( xpos, height(), xpos , ypos  );

    // Major ticks always have label, minor ticks never do. 
    if ( !minor )
    {   painter.setFont( QFont( "Arial", 7 )); 
        painter.drawText( xpos +1 , ypos -1, frameToString( frame ) );
    }

    //printf("painted frame tick %i xpos %i ypos %i _startFrame %f \n", frame, xpos, ypos, _startFrame );
}

void Ruler::paintNowMarker( QPainter &painter, int xpos, int nowsize)
{
    QColor gradient = _nowmarkercol;

    painter.setPen( gradient );
    int r, g, b;
    //painter.setBrush( _nowmarkercol );
    
    for ( int i = 0; i < nowsize; i++ )
    {   gradient.getRgb( &r, &g, &b );
        gradient.setRgb( r, (g+(5*i)) < 256 ? g + (5*i) : g , b );
        painter.setPen( gradient );
        painter.drawLine( xpos - i, height() - ( 2 + i), xpos +i, height() - ( 2 + i) );
    }

    painter.setPen( _nowmarkerpencol );
    QPointArray tri( 0 );
    tri.putPoints( 0, 4,   xpos, height() -1,   xpos - nowsize , height()-nowsize -1,    xpos + nowsize , height()-nowsize -1,    xpos, height() -1);     
    //painter.drawPolygon( tri, false, 0, 4 ); // (used this before doing custom fill)
    painter.drawPolyline( tri, 0, 4 );
}

void Ruler::paintEvent ( QPaintEvent * e)
{
    if ( mustGenerateBackground )
        genBackground();

// For now, just let qt clear our widget, we'll paint directly (seems fine for non-fancy bg rect)
    QPainter paint( this );
   
    // paint the pre-drawn bg onto ourselves.
    paint.drawPixmap( rect(), bg );

 
    _nowxpos = double( double(_nowframe) - _startFrame ) * _pixelsPerFrame;
    emit nowLinePixelMoved( _nowxpos );
    int xnowpix = int ( _nowxpos );
    
    paintNowMarker( paint, xnowpix, _nowsize );

    paint.setPen( colorGroup().text() ); // (so it matches the text labels)
    paint.drawLine( xnowpix, height() - _nowsize - 2, xnowpix, 1 );
    //_nowxpos = xnowpix;
    //NO! paint.end();
    emit nowLinePixelMoved( _nowxpos );
}

bool Ruler::tickChecker( double totalms, double major, double minor )
{
    double numminorticks = totalms / minor;
    double nummajorticks = totalms / major;
    // We could check major + minor ticks, but we end up counting the overlap ones twice.
    //int totticks = numminorticks + nummajorticks;
    // ... instead, we now just check minor ticks?
    double totticks = numminorticks;
    double maxticks = double( _maxTicksPerPixel * double(width()) );
    //printf( " tickChecker --> totticks= %i _maxticks= %i \n", totticks, maxticks );
    if ( totticks > 0 && totticks < maxticks )
    {  
        return true; 
    }
    
    return false;
}

void Ruler::determineTicks()
{
    // TODO could be optimised. We only need to do the tickChecker stuff when the zoom changes, 
    // but we need to re-set the _firstMajorTickFrame etc when we scroll. 
    //qDebug(" determineTicks ");

    int numticks;
    int totalticks = 0;
    int nsecperiod;
    int msecperiod;
    _majorTickFrames = 0; 
    _minorTickFrames = 0; 

   long totframes =  ( width() / _pixelsPerFrame );
   double totms = double( totframes) * _mspf;
   //printf(" V2 totframes %i totms %f \n", totframes, totms );
   int nummajorticks;
   int numminorticks;
 
   bool print = false;
   double major;
   double minor; 

   // f & f/16
   minor = 0.0625 * _mspf; 
   major = 1.0 * _mspf; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  f and f/16 \n " );
       _majorTickFrames = double(major) / 1000.0 * (double)_fps;
       _minorTickFrames = double(minor) / 1000.0 * double(_fps);
       //printf(" f & f/2   _majorTickFrames %f  _minorTickFrames %f \n", _majorTickFrames,  _minorTickFrames );
       _firstMajorTickFrame = int ( _startFrame / double( 1.0) );
       _firstMajorTickFrame *= int( 1 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 16.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // f & f/8
   minor = 0.125 * _mspf; 
   major = 1.0 * _mspf; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  f and f/8 \n " );
       _majorTickFrames = double(major) / 1000.0 * (double)_fps;
       _minorTickFrames = double(minor) / 1000.0 * double(_fps);
       //printf(" f & f/2   _majorTickFrames %f  _minorTickFrames %f \n", _majorTickFrames,  _minorTickFrames );
       _firstMajorTickFrame = int ( _startFrame / double( 1.0) );
       _firstMajorTickFrame *= int( 1 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 8.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // f & f/4
   minor = 0.25 * _mspf; 
   major = 1.0 * _mspf; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  f and f/4 \n " );
       _majorTickFrames = double(major) / 1000.0 * (double)_fps;
       _minorTickFrames = double(minor) / 1000.0 * double(_fps);
       //printf(" f & f/2   _majorTickFrames %f  _minorTickFrames %f \n", _majorTickFrames,  _minorTickFrames );
       _firstMajorTickFrame = int ( _startFrame / double( 1.0) );
       _firstMajorTickFrame *= int( 1 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 4.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // f & f/2
   minor = 0.5 * _mspf; 
   major = 1.0 * _mspf; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  f and f/2 \n " );
       _majorTickFrames = double(major) / 1000.0 * (double)_fps;
       _minorTickFrames = double(minor) / 1000.0 * double(_fps);
       //printf(" f & f/2   _majorTickFrames %f  _minorTickFrames %f \n", _majorTickFrames,  _minorTickFrames );
       _firstMajorTickFrame = int ( _startFrame / double( 1.0) );
       _firstMajorTickFrame *= int( 1 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 2.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
    // 2f & f [but only if this fits into fps ]
   if ( framesPerSecond() % 2 == 0 )
   {
       minor = 1.0 * _mspf; 
       major = 2.0 * _mspf; 
       if ( tickChecker( totms, major, minor ) )
       {   
       if ( print ) printf(" *** tickchecked  2f and f \n " );
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double( 2.0) );
       _firstMajorTickFrame *= int( 2 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 2.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
       }
   }
   // 5f & f [but only if this fits into fps ]
   if ( framesPerSecond() % 5 == 0 )
   {
       minor = 1.0 * _mspf; 
       major = 5.0 * _mspf; 
       if ( tickChecker( totms, major, minor ) )
       {   
       if ( print ) printf(" *** tickchecked  5f and f \n " );
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double( 5.0) );
       _firstMajorTickFrame *= int( 5 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 5.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
       }
   }
   // 10f & 2f [but only if this fits into fps ]
   if ( framesPerSecond() % 10 == 0 )
   {
       minor = 1.0 * _mspf; 
       major = 10.0 * _mspf; 
       if ( tickChecker( totms, major, minor ) )
       {   
       if ( print ) printf(" *** tickchecked  10f and f \n " );
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double( 10.0) );
       _firstMajorTickFrame *= int( 10 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 10.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
       }
   }
   // 12f & 2f [but only if this fits into fps ]
   if ( framesPerSecond() % 12 == 0 )
   {
       minor = 2.0 * _mspf; 
       major = 12.0 * _mspf; 
       if ( tickChecker( totms, major, minor ) )
       {   
       if ( print ) printf(" *** tickchecked  12f and 2f \n " );
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double( 12.0) );
       _firstMajorTickFrame *= int( 12 ); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 6.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
       }
   }
   
   // s and s/4
   minor = 250; 
   major = 1000; 
   if ( tickChecker( totms, major, minor ) )
   {   
       if ( print ) printf(" *** tickchecked  s and s/4 \n " );
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(1.0 * _fps) );
       _firstMajorTickFrame *= int(1.0 * _fps); 
       // Do rounding only on 1 tick type, and set the other exactly from that. 
       _firstMinorTickFrame = _firstMajorTickFrame  - 4.0*(_minorTickFrames );
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // s and s/2
   minor = 500; 
   major = 1000; 
   if ( tickChecker( totms, major, minor ) )
   {    if ( print ) printf(" *** tickchecked  s and s/2 \n " );
        _majorTickFrames = double(major) / 1000 * _fps;
        _minorTickFrames = double(minor) / 1000 * _fps;
        _firstMajorTickFrame = int ( _startFrame / double(1.0 * _fps) );
        _firstMajorTickFrame *= int(1.0 * _fps); 
        // Do rounding only on 1 tick type, and set the other exactly from that. 
        _firstMinorTickFrame = _firstMajorTickFrame  - 2.0*(_minorTickFrames );
        emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
        return;
   }
   // 2s and s/2
   minor = 500; 
   major = 2000; 
   if ( tickChecker( totms, major, minor ) )
   {    if ( print ) printf(" *** tickchecked  2s and s/2 \n " );
        _majorTickFrames = double(major) / 1000 * _fps;
        _minorTickFrames = double(minor) / 1000 * _fps;
        _firstMajorTickFrame = int ( _startFrame / double(2.0 * _fps) );
        _firstMajorTickFrame *= int(2.0 * _fps); 
        // Do rounding only on 1 tick type, and set the other exactly from that. 
        _firstMinorTickFrame = _firstMajorTickFrame  - 4.0*(_minorTickFrames );
        emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
        return;
   }
   // 2s and s
   minor = 1000; 
   major = 2000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  2s and s \n " ); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(2.0 * _fps) );
       _firstMajorTickFrame *= int(2.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( _fps ) );
       _firstMinorTickFrame *= int( _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 5s and s
   minor = 1000; 
   major = 5000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  5s and s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(5.0 * _fps) );
       _firstMajorTickFrame *= int(5.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 1.0 * _fps ) );
       _firstMinorTickFrame *= int( 1.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 10s and s
   minor = 1000; 
   major = 10000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  10s and s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(10.0 * _fps) );
       _firstMajorTickFrame *= int(10.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 1.0 * _fps ) );
       _firstMinorTickFrame *= int( 1.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 15s and s
   minor = 1000; 
   major = 15000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  15s and s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(15.0 * _fps) );
       _firstMajorTickFrame *= int(15.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 1.0 * _fps ) );
       _firstMinorTickFrame *= int( 1.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 30s and 2s
   minor = 2000; 
   major = 30000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  30s and 2s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(30.0 * _fps) );
       _firstMajorTickFrame *= int(30.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 2.0 * _fps ) );
       _firstMinorTickFrame *= int( 2.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 1m and 15s
   minor = 15000; 
   major = 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  1m and 15s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(60.0 * _fps) );
       _firstMajorTickFrame *= int(60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 15.0 * _fps ) );
       _firstMinorTickFrame *= int( 15.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 1m and 30s
   minor = 30000; 
   major = 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  1m and 30s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(60.0 * _fps) );
       _firstMajorTickFrame *= int(60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 30.0 * _fps ) );
       _firstMinorTickFrame *= int( 30.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 2m and 30s
   minor = 30000; 
   major = 120000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  2m and 30s \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(2* 60.0 * _fps) );
       _firstMajorTickFrame *= int(2.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 30.0 * _fps ) );
       _firstMinorTickFrame *= int( 30.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 5m and m
   minor = 60000; 
   major = 5* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  5m and m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(5* 60.0 * _fps) );
       _firstMajorTickFrame *= int(5.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 10m and m
   minor = 60000; 
   major = 10* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  10m and m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(10* 60.0 * _fps) );
       _firstMajorTickFrame *= int(10.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 15m and m
   minor = 60000; 
   major = 15* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  15m and m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(15* 60.0 * _fps) );
       _firstMajorTickFrame *= int(15.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 30m and m
   minor = 60000; 
   major = 30* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print) printf(" *** tickchecked  30m and m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(30* 60.0 * _fps) );
       _firstMajorTickFrame *= int(30.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // h and 15m
   minor = 15* 60000; 
   major = 60* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  h and 15m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 15.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 15.0 * 60.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // h and 30m
   minor = 30* 60000; 
   major = 60* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  h and 30m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 30.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 30.0 * 60.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 2h and 30m
   minor = 30* 60000; 
   major = 120* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  2h and 30m \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(2.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(2.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 30.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 30.0 * 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 2h and h
   minor = 60* 60000; 
   major = 120* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  2h and h \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(2.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(2.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 4h and h
   minor = 60* 60000; 
   major = 240* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  4h and h \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(4.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(4.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 6h and h
   minor = 60* 60000; 
   major = 360* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  6h and h \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(6.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(6.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 12h and h
   minor = 60* 60000; 
   major = 720* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  12h and h \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(12.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(12.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * 60.0 * _fps);
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // 12h and 2h  [this one seems to have more ticks than the previous one?]
   minor = 60* 60000; 
   major = 720* 60000; 
   if ( tickChecker( totms, major, minor ) )
   {   if ( print ) printf(" *** tickchecked  12h and h \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(12.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(12.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 2.0 * 60.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 2.0 * 60.0 * 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
   // d and h
   minor = 60* 60000; 
   major = 1440* 60000; 
   if ( 1 ) // NB the final tick settings are always true, else we cant show anything ;-)
   {   if ( print ) printf(" *** tickchecked  12h and h \n "); 
       _majorTickFrames = double(major) / 1000 * _fps;
       _minorTickFrames = double(minor) / 1000 * _fps;
       _firstMajorTickFrame = int ( _startFrame / double(24.0 *60* 60.0 * _fps) );
       _firstMajorTickFrame *= int(24.0 * 60.0 * 60.0 * _fps); 
       _firstMinorTickFrame = int ( _startFrame / double( 60.0 * 60.0 * _fps ) );
       _firstMinorTickFrame *= int( 60.0 * 60.0 * _fps); 
       emit majorTicksChanged( _firstMajorTickFrame, _majorTickFrames );
       return;
   }
}

QString Ruler::frameToString( int frame )
{
    int hh,mm,ss,ff;
    int theframe;
    if ( _dropFrame )
        theframe = dropFrameIncrease( frame );
    else 
        theframe = frame;

    hh = ( theframe / (60*60 * _fps ) ) % 24;
    mm = ( theframe / (60* _fps ) ) % 60;
    ss = ( theframe / ( _fps ) ) % 60;
    ff = theframe % _fps;
    QString qhh, qmm, qss, qff;
    qhh.setNum( hh );
    qmm.setNum( mm );
    qss.setNum( ss );
    qff.setNum( ff );
    QString label = qhh.rightJustify( 2, '0' ) + ":" +  qmm.rightJustify( 2, '0' ) + ":" + qss.rightJustify( 2, '0' ) + ":" + qff.rightJustify( 2, '0' );
    return label;
}

int Ruler::dropFrameIncrease( int f )
{
    int val = f;
    int prevVal = val;
    int prevAdj = 0;

    //this should never loop more than 4 times for tc in the range 0-24hrs
    do
    {
        prevVal = val;
        int minutes = val / 1800;
        int tens = (val / 18000);
        int adj = 2 * (minutes - tens);

        val += (adj - prevAdj);
        prevAdj = adj;
    } 
    while(val != prevVal);

    return val;
}

int Ruler::dropFrameDecrease( int f )
{
    int minutes = f / 1800;
    int tens = (f / 18000);
    int adj = 2 * (minutes - tens);

    int val = f - adj;

    return val;
}

void Ruler::mousePressEvent(QMouseEvent* e)
{
    if ( ( e->button() == Qt::RightButton  )  )
    {	e->ignore();
        _dragging = false;
        return; 
    }
    if ( e->pos().x() > ( _nowxpos - _nowsize ) && e->pos().x() < ( _nowxpos + _nowsize ) ) // dragging now marker
    {    _draggingnow = true;
    }
    else // dragging background, i.e. whole time line.
    {    _dragging = true;
         qApp->setOverrideCursor( 
				 QCursor( 
					 QPixmap(
						 hand_cursor_xpm
						 )
					 )
				 );
    }
    
    _dragpoint = e->pos();
    return;
}

void Ruler::mouseReleaseEvent(QMouseEvent* e)
{
    qApp->restoreOverrideCursor();

	if ( e->button() == RightButton )
	{	e->ignore();
	    return;
	}	

	_dragging = false;   
    _draggingnow = false; 

    mustGenerateBackground = true;
    update();
}

void Ruler::wheelEvent ( QWheelEvent * event )
{  
    // delta is +120 for pushing wheel away from user, -120 for pulling toward user.
    if ( event->delta() > 0 )
        zoomIn();
    else if ( event->delta() < 0 )
        zoomOut();
    event->accept();
}

void Ruler::mouseMoveEvent(QMouseEvent* e)
{
     bool moved = false;
    if ( _dragging ) {
		
        int signedxdrag = e->pos().x() - _dragpoint.x();
        int xdrag = abs( signedxdrag );
       
		if ( signedxdrag < 0 )
        {   
            moved = true; 
            _dragpoint = e->pos();

            _startFrame += double(xdrag)/double(_pixelsPerFrame);
            if ( _startFrame + double(width())/_pixelsPerFrame > _maxFrames )
                _startFrame -= double(xdrag)/double(_pixelsPerFrame);

            // TODO scroll ruler when nowbar moves off screen?
        }
		else if ( signedxdrag > 0 ) 
        {   
            moved = true; 
            _dragpoint = e->pos();
            
            _startFrame -= double(xdrag)/(_pixelsPerFrame);
            if ( _startFrame < 0.0 )
                _startFrame = 0.0;
        }
		
        if ( moved )
        {   
            mustGenerateBackground = true;
            mustDetermineTicks = true; // SEE TODO in fn. Could optimise this.
            emit startFrameChanged( _startFrame );
            //printf(" just emitted startFrameChanged \n " );
            update();
        }
	
    }
    else if ( _draggingnow )
    {
        // TODO snap to frame...

        int signedxdrag = e->pos().x() - _dragpoint.x();
        int xdrag = abs( signedxdrag );

        double newxpos = _nowframe/_pixelsPerFrame + double(signedxdrag)/_pixelsPerFrame;

        _nowframe += double(signedxdrag) / _pixelsPerFrame;
        //printf(" drag: signedxdrag %i _nowframe %f \n ", signedxdrag,  _nowframe );

        if ( _nowframe < 0.0 )
                _nowframe = 0.0;
        // Could make ruler step if marker goes off screen. Maybe dont want this in final version.
        //if ( _nowframe < _startFrame ) 
        //    stepLeft();

        moved = true; 
        _dragpoint = e->pos();

        if ( moved )
        {
            emit nowChanged( int( _nowframe ));
            emit nowStringChanged( frameToString( _nowframe ) );
            // (xpos of line is emitted in paintEvent)
            update();
        }
    }
}

void Ruler::zoom( double scalef )
{
    // Dont let zoom go to crazy levels, e.g.  where we have < 2 frames on the ruler, 
    // or too many ticks at most zoomed out level.
    double check = _pixelsPerFrame / scalef;
    if ( ( scalef < 1.0 && check > 0.5*width() ) || ( scalef > 1.0 && check < 0.000055 )  )
        return;
    else
        _pixelsPerFrame = check;
    // TODO scale from left/right/centre/now.

    //printf(" zoom scalef = %f,  _pixelsPerFrame %f \n", scalef, _pixelsPerFrame);

    mustGenerateBackground = true;
    mustDetermineTicks = true; // zoom changed, so must check our tick settings.
    update();
}

void Ruler::stepRight()
{
    _startFrame += 100/_pixelsPerFrame;
    mustGenerateBackground = true;
    mustDetermineTicks = true;
    update();
}

void Ruler::stepLeft()
{
    _startFrame -= 100/_pixelsPerFrame;
    if ( _startFrame < 0.0 ) 
        _startFrame = 0.0;
    mustGenerateBackground = true;
    mustDetermineTicks = true;
    update();
}

void Ruler::setMaxFrames( double frame )
{
    if ( frame < 0.0 )
        return;
    _maxFrames = frame;
}

void Ruler::resizeEvent( QResizeEvent *e )
{
    mustGenerateBackground = true;
    // (Dont need mustDetermineTicks = true since ticks spacinf and start frame will not have changed.)
    QWidget::resizeEvent( e );
}


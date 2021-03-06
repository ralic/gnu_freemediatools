/*******************************************************************************
 **
 ** The source file for the Jahshaka jahdesktop code
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the file that sets up  the desktop interface
// and connects it to the routines

#include "jahcreate.h"

#include "desktop.h"
#include "dialogs.h"
#include <qtextview.h>
#include <qframe.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qstylefactory.h>
#include <widget.h>
#include <jahtranslate.h>
#include <jahformatter.h>
#include <jahpreferences.h>
#include <openmedialib.h>
#include <stretchableButton.h>
#include <iostream>

void JahControl::CreateDesktopFeedbackBox( QWidget* parent ) 
{
    m_desktopFeedbackLabel = new QLabel( parent );
    m_desktopFeedbackLabel->setText( jt->tr( "FreeMediaTools Desktop"  ) );
    QFont f( parent->font() );
    f.setItalic( true );
    m_desktopFeedbackLabel->setFont( f );
    m_desktopFeedbackLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    if ( !parent->isHidden() )
    {
        m_desktopFeedbackLabel->show();
    }
}

void JahControl::CreateDesktopOptions( QHBox* frame ) 
{
    // Note, we have hidden and disabled (but not deleted) buttons which still dont do anything. 
    // The slot connections are also commented out for those buttons. They can be
    // enabled and hooked up to a slot again if code is written for those buttons.
    
    desktoppanel = new QFrame( frame, "desktoppanel" );
    desktoppanel->setFrameShape( QFrame::NoFrame );

    QVBoxLayout *vlayout = new QVBoxLayout( desktoppanel, 0, 0 );

    // five rows, eleven columns
    QGridLayout* grid = new QGridLayout( 3, 13 );
    
    for ( unsigned int i=0; i<12; ++i )
    {
        grid->setColStretch( i, 0 );
    }
    grid->setColStretch( 12, 1 );

    JahFormatter::setMarginAndSpacing( grid );
    
    library = new QPushButton( desktoppanel, "library" );
    library->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( library, 0, 1 );

#ifdef USE_NETWORK
    network = new QPushButton( desktoppanel, "network" );
    network->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( network, 2, 1 );
#endif

    /// insert separator
    grid->addMultiCellWidget( JahFormatter::createSeparator( desktoppanel ), 0, 2, 2, 2 );

    expandclip = new QPushButton( desktoppanel, "expandclip" );
    expandclip->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( expandclip, 0, 3 );

    exportclip = new QPushButton( desktoppanel, "exportclip" );
    exportclip->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( exportclip, 2, 3 );

    /// insert separator
    grid->addMultiCellWidget( JahFormatter::createSeparator( desktoppanel ), 0, 2, 4, 4 );

    ColorModuleButton = new QPushButton( desktoppanel, "colorize" );
    ColorModuleButton->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( ColorModuleButton, 0, 5 );

    TrackerModuleButton = new QPushButton( desktoppanel, "tracker" );
    TrackerModuleButton->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( TrackerModuleButton, 2, 5 );

    encoder = new QPushButton( desktoppanel, "encoder" );
    encoder->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( encoder, 0, 6 );

    KeyerModuleButton = new QPushButton( desktoppanel, "keyer" );
    KeyerModuleButton->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( KeyerModuleButton, 2, 6 );
    
    /// insert separator
    grid->addMultiCellWidget( JahFormatter::createSeparator( desktoppanel ), 0, 2, 7, 7 );

    tidydesktop = new QPushButton( desktoppanel, "tidydesktop" );
    tidydesktop->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( tidydesktop, 0, 8 );
    
    cleardesktop = new QPushButton( desktoppanel, "cleardesktop" );
    cleardesktop->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( cleardesktop, 2, 8 );

    /// insert separator
    grid->addMultiCellWidget( JahFormatter::createSeparator( desktoppanel ), 0, 2, 9, 9 );
    
    Preferences = new QPushButton( desktoppanel, "Preferences" );
    Preferences->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( Preferences, 0, 10 );

    Quit = new QPushButton( desktoppanel, "Quit" );
    Quit->setFixedSize( QSize( 100, 25 ) );
    grid->addWidget( Quit, 2, 10 );

#if 0
	// Misplaced buttons
    lockclip = new QPushButton( desktoppanel, "lockclip" );
    lockclip->setGeometry( QRect( 140, 20, 100, 25 ) );
    lockclip->setEnabled( false );  //lockclip->hide(); // Disabled for now.

    desktopundo = new QPushButton( desktoppanel, "desktopundo" );
    desktopundo->setGeometry( QRect( 140, 50, 100, 25 ) );
    desktopundo->setEnabled( false );  //desktopundo->hide(); // Disabled for now.

    lockall = new QPushButton( desktoppanel, "lockall" );
    lockall->setGeometry( QRect( 140, 110, 100, 25 ) );
    lockall->setEnabled( false );  //lockall->hide(); // Disabled for now.

    unlockall = new QPushButton( desktoppanel, "unlockall" );
    unlockall->setGeometry( QRect( 140, 140, 100, 25 ) );
    unlockall->setEnabled( false );  //unlockall->hide(); // Disabled for now.

    desktopredo = new QPushButton( desktoppanel, "desktopredo" );
    desktopredo->setGeometry( QRect( 260, 50, 100, 25 ) );
    desktopredo->setEnabled( false );  //desktopredo->hide(); // Disabled for now.

    expandall = new QPushButton( desktoppanel, "expandall" );
    expandall->setGeometry( QRect( 260, 110, 100, 25 ) );
    expandall->setEnabled( false );  //expandall->hide(); // Disabled for now.

    collapseall = new QPushButton( desktoppanel, "collapseall" );
    collapseall->setGeometry( QRect( 260, 140, 100, 25 ) );
    collapseall->setEnabled( false );  //collapseall->hide(); // Disabled for now.
    trimmer = new QPushButton( desktoppanel, "trimmer" );
    trimmer->setGeometry( QRect( 380, 140, 100, 25 ) );
    trimmer->setEnabled( false );  //trimmer->hide(); // Disabled for now.

    broadcaster = new QPushButton( desktoppanel, "broadcaster" );
    broadcaster->setGeometry( QRect( 500, 110, 100, 25 ) );
    broadcaster->setEnabled( false );  //broadcaster->hide(); // Disabled for now.

    videoio = new QPushButton( desktoppanel, "videoio" );
    videoio->setGeometry( QRect( 500, 140, 100, 25 ) );
    videoio->setEnabled( false );  //videoio->hide(); // Disabled for now.

    loaddesktop = new QPushButton( desktoppanel, "loaddesktop" );
    loaddesktop->setGeometry( QRect( 620, 110, 100, 25 ) );
    loaddesktop->setEnabled( false );  //loaddesktop->hide(); // Disabled for now.

    savedesktop = new QPushButton( desktoppanel, "savedesktop" );
    savedesktop->setGeometry( QRect( 620, 140, 100, 25 ) );
    savedesktop->setEnabled( false );  //savedesktop->hide(); // Disabled for now.

    lockclip->setText( jt->tr( "Lock" ) );
    lockall->setText( jt->tr( "Lock All" ) );
    unlockall->setText( jt->tr( "Unlock All" ) );
    expandclip->setText( jt->tr( "Expand" ) );
    expandall->setText( jt->tr( "Expand All" ) );
    collapseall->setText( jt->tr( "Collapse All" ) );
#ifdef USE_NETWORK
    network->setText( jt->tr( "Network" ) );
#endif
    encoder->setText( jt->tr( "Encoder" ) );
    loaddesktop->setText( jt->tr( "Load Desktop" ) );
    desktopundo->setText( jt->tr( "Undo" ) );
    desktopredo->setText( jt->tr( "Redo" ) );
#endif

    ///////////////////////////////////////////////////////////////////
    //language itmes, set up translator
    //JahTranslate& jt = JahTranslate::getInstance();
    
    library->setText( jt->tr( "Library" ) );
    expandclip->setText( jt->tr( "Expand" ) );
    ColorModuleButton->setText( jt->tr( "Colorize" ) );
    TrackerModuleButton->setText( jt->tr( "Tracker" ) );
    KeyerModuleButton->setText( jt->tr( "Keyer" ) );
#ifdef USE_NETWORK
    network->setText( jt->tr( "Network" ) );
#endif
    encoder->setText( jt->tr( "Encoder" ) );
    
    exportclip->setText( jt->tr( "Export Clip" ) );
    tidydesktop->setText( jt->tr( "Tidy Desktop" ) );
    cleardesktop->setText( jt->tr( "Clear Desktop" ) );
    Preferences->setText( jt->tr( "Preferences" ) );
    Quit->setText( jt->tr( "&Quit" ) );

    //////////////////////////////////////////////////////////////////
    // signals and slots connections
    
    connect( exportclip,  SIGNAL(clicked()), JahDesktopMod, SLOT(slotExport())  );
    connect( library,  SIGNAL(clicked()), SLOT(changeModeLibrary())  );
    connect( expandclip, SIGNAL( clicked() ), JahDesktopMod, SLOT( slotExpand() ) );
    
    connect( ColorModuleButton,  SIGNAL(clicked()), SLOT(changeModeColorize())  );
    connect( TrackerModuleButton, SIGNAL( clicked() ), this, SLOT( changeModeTracker() ) );
    connect( KeyerModuleButton, SIGNAL( clicked() ), this, SLOT( changeModeKeyer() ) );
#ifdef USE_NETWORK
    connect( network,  SIGNAL(clicked()), SLOT(changeModeNetworkCom())  );
#endif
    connect( encoder,  SIGNAL(clicked()), SLOT(changeModeEncoding())  );
    connect( tidydesktop, SIGNAL( clicked() ), JahDesktopMod, SLOT( tidyDesktop() ) );
    connect( cleardesktop, SIGNAL(clicked()), JahDesktopMod, SLOT( clearAll()) );
    
    connect( Preferences,  SIGNAL(clicked()), this, SLOT(changeModePref())  );
    connect( Quit, SIGNAL(clicked()), qApp, SLOT( quit()) );
    
    vlayout->addLayout( grid );
    vlayout->addStretch( );
}

void JahControl::CreateDesktopMiddleFrame(QHBox* frame) 
{
    //set up translator
    //JahTranslate& jt = JahTranslate::getInstance();
    
    page = new QWidget( frame, "page" );
    pageLayout = new QHBoxLayout( page, 0, 0, "pageLayout");

    pageLayout->addSpacing( 20 );
    
   
    //modeselectbutton
    expandbutton = new QPushButton( page, "modeselectbutton" );
    expandbutton->setText( tr( "expand"  ) );
    connect( expandbutton,  SIGNAL(clicked()), JahDesktopMod, SLOT( slotExpand())  );
    
    lockbutton = new QPushButton( page, "modeselectbutton" );
    lockbutton->setText( tr( "tidy"  ) );
    connect( lockbutton, SIGNAL( clicked() ), JahDesktopMod, SLOT( tidyDesktop() ) );

    sharebutton = new QPushButton( page, "modeselectbutton" );
    sharebutton->setText( tr( "clear"  ) );
    connect( sharebutton, SIGNAL(clicked()), JahDesktopMod, SLOT( clearAll()) );
    
    pageLayout->addWidget( expandbutton );
    pageLayout->addWidget( lockbutton );
    pageLayout->addWidget( sharebutton );
    pageLayout->addStretch( 1 );
    
    //this should really popup a modal dialog box to do the renaming
    //vs being attached to the calling object
    
    QString path = JahBasePath + "Pixmaps/desktopController/";

    std::wcerr << "going to load values from " << JahBasePath << endl;

    DesktopName = new StretchableButton( page, "desktopnamebutton" );
    DesktopName->setPixmap( path + "name_off.png", path + "name_on.png" );
    DesktopName->setMaximumSize( 148, 24 );
    DesktopName->setStretchableRange( 77, 148 );

    connect ( DesktopName,    SIGNAL(clicked()), this, SLOT( nameClip()) );
    popup2 = new FancyPopup( page, "Enter Clipname",250,130 );  //send size and position as well
    connect ( popup2,      SIGNAL( returnText(QString)), this, SLOT( renameDesktopAsset(QString)) );
    
    
    DesktopLoad = new StretchableButton( page, "desktopnamebutton" );
    DesktopLoad->setMaximumSize( 148, 24 );
    DesktopLoad->setStretchableRange( 77, 148 );
    DesktopLoad->setPixmap( path + "load_off.png", path + "load_on.png" );
    connect( DesktopLoad,  SIGNAL(clicked()), JahDesktopMod, SLOT(slotLoadIconview())  );  //this slot is located in desktop.h
    
    DesktopPlay = new StretchableButton( page, "desktopnamebutton" );
    DesktopPlay->setMaximumSize( 148, 24 );
    DesktopPlay->setStretchableRange( 77, 148 );
    DesktopPlay->setPixmap( path + "play_off.png", path + "play_on.png" );
    connect( DesktopPlay,  SIGNAL(clicked()), SLOT(changeModePlayer())  ); 
    
    DeleteButton = new StretchableButton( page, "desktopnamebutton" );
    DeleteButton->setMaximumSize( 148, 24 );
    DeleteButton->setStretchableRange( 84, 134 );
    DeleteButton->setPixmap( path + "delete_off.png", path + "delete_on.png" );
    connect ( DeleteButton,  SIGNAL(clicked()), JahDesktopMod, SLOT( clearItem()) );
    
    pageLayout->addWidget( DesktopName, 2 );
    pageLayout->addWidget( DesktopLoad, 2 );
    pageLayout->addWidget( DesktopPlay, 2 );
    pageLayout->addWidget( DeleteButton, 2 );
    
    ////////////////////////////////////////////////////////////////////
    
    pageLayout->addStretch( 1 );
    
//     desktopselectbutton = new QComboBox( FALSE, page, "renderercombo" );
//     desktopselectbutton->setFixedSize( 120, 20 );
//     desktopselectbutton->insertItem("Desktop");
//     desktopselectbutton->insertItem(">New Desktop");
//     desktopselectbutton->insertItem(">Del Desktop");
//     desktopselectbutton->setCurrentItem(0); //4:2:0
//  connect( desktopselectbutton,  SIGNAL(activated(int)), this, SLOT(setRenderer(int))  );
    
    /////////////////////////////////////////////////////////////////////////
    //these buttons work in tandem for get and return of images from modules
    //need to put these somewhere mon!
    ReturnButton = new QPushButton( page, "ReturnButton" );
    ReturnButton->setFixedSize( 120, 20 );
    ReturnButton->setText( jt->tr( "&Return"  ) );
    ReturnButton->setPalette( QPalette( QColor(255, 25, 25) ) );
    connect( ReturnButton,  SIGNAL(clicked()), SLOT( ReturnDesktop())  );
    ReturnButton->hide();
    
    // pageLayout->addWidget( desktopselectbutton );
    pageLayout->addWidget( ReturnButton );

    pageLayout->addSpacing( 20 );
}

////////////////////////////////////////////////////////////////
//icon for desktop preferences locate jahpath
static const char* basefolder_xpm[] = {
    "15 14 4 1",
    "   c None",
    ".  c #000000",
    "+  c #FFFF00",
    "@  c #FFFFFF",
    "          .    ",
    "               ",
    "          .    ",
    "       .     . ",
    "  ....  . . .  ",
    " .+@+@.  . .   ",
    "..........  . .",
    ".@+@+@+@+@..   ",
    ".+@+@+@+@+. .  ",
    ".@+@+@+@+@.  . ",
    ".+@+@+@+@+.    ",
    ".@+@+@+@+@.    ",
    ".+@+@+@+@+.    ",
    "...........    "};

void JahControl::CreateDesktopPreferences( QHBox* f )
{
    QWidget* frame = new QWidget( f );
    QGridLayout* masterLayout = new QGridLayout( frame, 2, 10 );
    JahFormatter::setMarginAndSpacing( masterLayout );
    masterLayout->setRowStretch( 0, 0 );
    masterLayout->setRowStretch( 1, 1 );
    masterLayout->setColStretch( 0, 0 );
    masterLayout->setColStretch( 9, 1 );

    masterLayout->addWidget( createCol1( frame ), 0, 0 );
    masterLayout->addWidget( JahFormatter::createSeparator( frame ), 0, 1 );
    masterLayout->addWidget( createCol2( frame ), 0, 2 );
    masterLayout->addWidget( JahFormatter::createSeparator( frame ), 0, 3 );
    masterLayout->addWidget( createCol3( frame ), 0, 4 );
    masterLayout->addWidget( JahFormatter::createSeparator( frame ), 0, 5 );
    masterLayout->addWidget( createCol4( frame ), 0, 6 );
    masterLayout->addWidget( JahFormatter::createSeparator( frame ), 0, 7 );
    masterLayout->addWidget( createCol5( frame ), 0, 8 );
    
    PreferenceslanguageChange();    
}

QWidget* JahControl::createCol1( QWidget* p )
{
    QWidget* col = new QWidget( p );

    //////////////////////////////////////////////////////////////////
    // column 1
    QBoxLayout* colLayout = new QVBoxLayout( col );
    JahFormatter::setMarginAndSpacing( colLayout );

    //add set style text here
    stylelabel = new QLabel( col, "storagelabel" );
    stylelabel->setAlignment( int( QLabel::AlignCenter ) );
    stylelabel->setText( jt->tr( "Modify Style" ) );
    colLayout->addWidget( stylelabel );

    stylebutton = new QPushButton( col, "Pref1" );
    stylebutton->setText( tr( "Set Style" ) );
    connect( stylebutton, SIGNAL(clicked()),  SLOT(setJahTheme()) );
    colLayout->addWidget( stylebutton );

    ////////////////////////////////////////////////////////////////
    colorlabel = new QLabel( col, "storagelabel" );
    colorlabel->setAlignment( int( QLabel::AlignCenter ) );
    colorlabel->setText( jt->tr( "Modify Color" ) );
    colLayout->addWidget( colorlabel );
    
    // place colour option buttons in a grid
    QGridLayout* colourButtonGrid = new QGridLayout( 2, 2 );
    colLayout->addLayout( colourButtonGrid );

    Pref5 = new QPushButton( col, "Pref5" );
    connect( Pref5, SIGNAL(clicked()),  SLOT(setstyleGrey()) );
    colourButtonGrid->addWidget( Pref5, 0, 0 );

    Pref6 = new QPushButton( col, "Pref6" );
    connect( Pref6, SIGNAL(clicked()),  SLOT(setstyleBlue()) );
    colourButtonGrid->addWidget( Pref6, 0, 1 );

    Pref7 = new QPushButton( col, "Pref7" );
    connect( Pref7,  SIGNAL(clicked()), SLOT(setstyleBlack()) );
    colourButtonGrid->addWidget( Pref7, 1, 0 );

    Pref8 = new QPushButton( col, "Pref8" );
    connect( Pref8,  SIGNAL(clicked()), SLOT(setstyleWhite()) );  //change to signal...?
    colourButtonGrid->addWidget( Pref8, 1, 1 );

    //this is the remember prefs option
    // then why not name it as such... cha
    Pref10 = new QCheckBox( col, "Pref10" );
    Pref10->setChecked(jahStyleprefs);
    connect( Pref10,  SIGNAL(clicked()), this, SLOT(setStyleOption())  );
    colLayout->addWidget( Pref10 );

    return col;
}

QWidget* JahControl::createCol2( QWidget* p )
{
    QWidget* col = new QWidget( p );

    ////////////////////////////////////////////////////////
    // column 2
    QBoxLayout* colLayout = new QVBoxLayout( col );
    JahFormatter::setMarginAndSpacing( colLayout );

    storagelabel = new QLabel( col, "storagelabel" );
    storagelabel->setAlignment( int( QLabel::AlignCenter ) );
    storagelabel->setText( jt->tr( "User Storage" ) );
    colLayout->addWidget( storagelabel );

    // make the line edit and folder button appear side by side
    storageLocationLayout = new QHBoxLayout();
    colLayout->addLayout( storageLocationLayout );

    StorageBox = new QLineEdit( col, "StorageBox" ); 
    /*
// TODO:Check this obj :
the cursor  is written to init data.
JahControl::createCol2(QWidget*) (jahdesktopinterface.cpp:454)
JahControl::CreateDesktopPreferences(QHBox*) (jahdesktopinterface.cpp:369)
JahControl::CreateDesktopModuleOptions() (jahmodules.cpp:163)
JahControl::CreateDesktopModule(QHBox*, QHBox*) (jahmodules.cpp:132)
JahControl::createJahDesktopUI(QFrame*, QFrame*) (jahui.cpp:184)

==28068==    by 0x83702B7: JahControl::createCol2(QWidget*) (jahdesktopinterface.cpp:476)
==28068==    by 0x83794E7: JahControl::CreateDesktopPreferences(QHBox*) (jahdesktopinterface.cpp:372)
==28068==    by 0x8385E5B: JahControl::CreateDesktopModuleOptions() (jahmodules.cpp:163)
==28068==    by 0x8385FD2: JahControl::CreateDesktopModule(QHBox*, QHBox*) (jahmodules.cpp:132)
==28068==    by 0x8398347: JahControl::createJahDesktopUI(QFrame*, QFrame*) (jahui.cpp:184)
==28068==    by 0x8368160: JahControl::JahControl(QGLWidget*, QWidget*, JahHeader*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QWidget*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*) (jahcreate.cpp:213)
==28068==    by 0x83AB2A4: Jahshaka::Jahshaka(QWidget*, char const*, unsigned) (jahshaka.cpp:106)
==28068==    by 0x83AC9E5: main (main.cpp:166)
*/
    StorageBox->setText( JahMediaPath );
    storageLocationLayout->addWidget( StorageBox );


    StorageLocation = new QToolButton( col, "StorageLocation" );
    StorageLocation->setFixedSize( QSize( 31, 21 ) );
    StorageLocation->setIconSet( QPixmap( basefolder_xpm ) );
    connect( StorageLocation , SIGNAL(clicked()),  SLOT(getStoragePath()) );

    storageLocationLayout->addWidget( StorageLocation );
    
    StorageButton = new QPushButton( col, "StorageButton" );
    connect( StorageButton, SIGNAL(clicked()),  SLOT(setStoragePath()) );
    colLayout->addWidget( StorageButton );

    // insert separator
    colLayout->addWidget( JahFormatter::createSeparator( col, false ) );

    renderlabel = new QLabel( col, "renderlabel" );
    renderlabel->setAlignment( int( QLabel::AlignCenter ) );
    colLayout->addWidget( renderlabel );

    // place the combos in a grid with their labels
    QGridLayout* renderLayout = new QGridLayout( 2, 2 );
    colLayout->addLayout( renderLayout );

    renderqualitylabel = new QLabel( col, "renderqualitylabel" );
    renderqualitylabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    renderLayout->addWidget( renderqualitylabel, 0, 0 );
        
    renderqualitycombo = new QComboBox( col, "renderqualitycombo" );
    renderqualitycombo->insertItem( "JPEG" );
    renderqualitycombo->insertItem( "PNG" );
    renderqualitycombo->insertItem( "BMP" );
#ifdef MLTSUPPORT
    renderqualitycombo->insertItem( "MLT" );
#endif
    //check prefs and read in renderquality
    renderqualitycombo->setCurrentItem   ( jahrenderquality );
    connect( renderqualitycombo,   SIGNAL(activated( int )),    SLOT(setGlobalRenderQuality( int )) );
    renderLayout->addWidget( renderqualitycombo, 0, 1 );

    rendererlabel = new QLabel( col, "rendererlabel" );
    rendererlabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    renderLayout->addWidget( rendererlabel, 1, 0 );

    renderercombo = new QComboBox( FALSE, col, "renderercombo" );
    renderercombo->insertItem("GL Renderer (fast)");
    renderercombo->insertItem("Qt GL Renderer (fast)");
    renderercombo->insertItem("Software (slow)");
    renderercombo->insertItem("Qt Renderer Exp (fastest)");
    renderercombo->setCurrentItem(renderer); //4:2:0
    connect( renderercombo,  SIGNAL(activated(int)), this, SLOT(setRenderer(int))  );
    renderLayout->addWidget( renderercombo, 1, 1 );

    return col;
}

QWidget* JahControl::createCol3( QWidget* p )
{
    QWidget* col = new QWidget( p );
    
    ////////////////////////////////////////////////////
    // column 3
    //insert http browsing preferences here
    QBoxLayout* colLayout = new QVBoxLayout( col );
    JahFormatter::setMarginAndSpacing( colLayout );

    httpdlabel = new QLabel( col, "renderlabel" );
    httpdlabel->setAlignment( int( QLabel::AlignCenter ) );
    colLayout->addWidget( httpdlabel );

    // setup interface for Jah Httpd
    //need a qcheckbox for auto lunch here
    
    bindButton = new QPushButton("Start", col);
    colLayout->addWidget( bindButton );
    
    //need to pull the default values from the preferences
    //instead of hardcoding them ie the "1" and "8080"
    backlogLineEdit = new QLineEdit("1", col);
    colLayout->addWidget( backlogLineEdit );
    
    portLineEdit = new QLineEdit("8080", col);
    colLayout->addWidget( portLineEdit );
    
    return col;
}

QWidget* JahControl::createCol4( QWidget* p )
{
    QWidget* col = new QWidget( p );

    //////////////////////////////////////////////////////////////
    // column 4
    // ui settings here
    QBoxLayout* colLayout = new QVBoxLayout( col );
    JahFormatter::setMarginAndSpacing( colLayout );

    uisettingslabel = new QLabel( col, "uisettingslabel" );
    uisettingslabel->setAlignment( int( QLabel::AlignCenter ) );
    colLayout->addWidget( uisettingslabel );
    
    QuickStartBox = new QCheckBox( col, "QuickStart" );
	QuickStartBox->setChecked( JahPrefs::getInstance().getQuickStart( ) );
	connect( QuickStartBox, SIGNAL( clicked( ) ), this, SLOT( setQuickStart( ) ) );
    colLayout->addWidget( QuickStartBox );

    PositionPrefs = new QCheckBox( col, "PositionPrefs" );
    PositionPrefs->setChecked(rememberprefs);
    connect( PositionPrefs,  SIGNAL(clicked()), this, SLOT(setGlobalPositionSize())  );
    colLayout->addWidget( PositionPrefs );

    DualHead = new QCheckBox( col, "DualHead" );
    DualHead->setChecked(dualheadprefs);
    connect( DualHead,  SIGNAL(clicked()), this, SLOT(setDualHeadOption())  );
    colLayout->addWidget( DualHead );    

    VideoHead = new QCheckBox( col, "VideoHead" );
    VideoHead->setChecked(videohead);
    connect( VideoHead,  SIGNAL(clicked()), this, SLOT(setVideoHeadOption())  );
    colLayout->addWidget( VideoHead );    

    MenuOption = new QCheckBox( col, "MenuOption" );
    MenuOption->setChecked(menuprefs);
    connect( MenuOption,  SIGNAL(clicked()), this, SLOT(setMenuOption())  );
    colLayout->addWidget( MenuOption );    

    WideScreenBox = new QCheckBox( col, "WideScreen" );
    WideScreenBox->setChecked(widescreenprefs);
    connect( WideScreenBox,  SIGNAL(clicked()), this, SLOT(setWidescreenOption())  );
    colLayout->addWidget( WideScreenBox );    

    return col;
}

QWidget* JahControl::createCol5( QWidget* p )
{
    QWidget* col = new QWidget( p );
    
    /////////////////////////////////////////////////////////////
    // column 5
    QBoxLayout* colLayout = new QVBoxLayout( col );
    JahFormatter::setMarginAndSpacing( colLayout );

    languagelabel = new QLabel( col, "languagelabel" );
    languagelabel->setAlignment( int( QLabel::AlignCenter ) );
    colLayout->addWidget( languagelabel );
    
    LanguageCombo = new QComboBox( FALSE, col, "LanguageCombo" );
    LanguageCombo->insertItem("English");
    LanguageCombo->insertItem("Spanish");
    LanguageCombo->insertItem("French");
    LanguageCombo->insertItem("Portuguese");
    LanguageCombo->insertItem("German");
    LanguageCombo->insertItem("Italian");
    LanguageCombo->setCurrentText( JahPrefs::getInstance().getLanguage().c_str() );
    connect( LanguageCombo,  SIGNAL(activated(const QString &)), this, SLOT(setLanguage(const QString &))  );
    jt->setLanguage( JahPrefs::getInstance().getLanguage().c_str() );
    colLayout->addWidget( LanguageCombo );
    
    jahreslabel = new QLabel( col, "languagelabel" );
    jahreslabel->setAlignment( int( QLabel::AlignCenter ) );
    colLayout->addWidget( jahreslabel );
    
    JahresCombo = new QComboBox( FALSE, col, "JahresCombo" );

    projectData thedata;    
    thedata.buildComboBox(JahresCombo);
    
    JahresCombo->setCurrentItem(JahDefaultRes); //should be languagepref
    connect( JahresCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );
    colLayout->addWidget( JahresCombo );
    
    // add separator (horizontal)
    colLayout->addWidget( JahFormatter::createSeparator( col, false ) );
    
    PrefExit = new QPushButton( col, "PrefExit" );
    connect( PrefExit,  SIGNAL(clicked()), SLOT(changeModePrefReturn())  );
    colLayout->addWidget( PrefExit );

    return col;
}

void JahControl::PreferenceslanguageChange()
{
    
    Pref5->setText( jt->tr( "Grey" ) );
    Pref6->setText( jt->tr( "Blue" ) );
    Pref7->setText( jt->tr( "Black" ) );
    Pref8->setText( jt->tr( "White" ) );
    
    Pref10->setText( jt->tr( "Remember Style" ) );
    StorageLocation->setText( jt->tr( "Dir" ) );
    StorageButton->setText( jt->tr( "Set Path" ) );
    
    QuickStartBox->setText( jt->tr( "Quick Start" ) );
    MenuOption->setText( jt->tr( "Use Menus" ) );
    DualHead->setText( jt->tr( "Dual Head" ) );
    VideoHead->setText( jt->tr( "Video Head" ) );
    WideScreenBox->setText( jt->tr( "Fixed Media Tray" ) );
    PositionPrefs->setText( jt->tr( "Remember UI" ) );
    
    httpdlabel->setText( jt->tr( "Web Desktop" ) );
    PrefExit->setText( jt->tr( "RETURN" ) );
    
    //storagelabel->setText( jt->tr( "Media Storage" ) );
    renderlabel->setText( jt->tr( "Render Settings" ) );
    uisettingslabel->setText( jt->tr( "User Interface" ) );
    
    languagelabel->setText( jt->tr( "Language" ) );
    jahreslabel->setText( jt->tr( "Resolution" ) );
    
    rendererlabel->setText( jt->tr( "Renderer" ) );
    renderqualitylabel->setText( jt->tr( "Quality" ) );
}



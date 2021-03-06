/*****************************************************************************
 * NasuTek Commander
 * Copyright (C) 2012 NasuTek Enterprises <mmanley@nasutek.com>
 * Copyright (C) 2005-2008 Piotr Pszczolkowski <piotr@beesoft.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtPanel.h"
#include "QBtView.h"
#include "QBtEventsController.h"
#include "QBtConfig.h"
#include "QBtSettings.h"
#include "QBtWorkspace.h"
#include "QBtTabBar.h"
#include <QStackedWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDir>
#include <QProcess>
#include <QLineEdit>
#include <QKeyEvent>
#include <QtDebug>
#include <QPushButton>

#if _WIN32
#include <windows.h>
#endif

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtPanel::DIRS     = QT_TR_NOOP( "Dirs:"     );
const char* const QBtPanel::FILES    = QT_TR_NOOP( "Files:"    );
const char* const QBtPanel::SELECTED = QT_TR_NOOP( "Selected:" );


//*******************************************************************
// QBtPanel                                          CONSTRUCTOR
//*******************************************************************
QBtPanel::QBtPanel( const qint32 in_idx, QWidget* const in_parent ) : QWidget( in_parent )
, idx_      ( in_idx )
, path_     ( new QComboBox )
, fstab_    ( new QComboBox )
, tbar_     ( new QBtTabBar )
, wstack_   ( new QStackedWidget )
, dirs_     ( new QLabel )
, files_    ( new QLabel )
, selected_ ( new QLabel )
, infolabel_( new QLabel )
  , dotdot_   (new QPushButton(".."))
  , rootbutton_(new QPushButton("/"))
{
   path_->setEditable( true );
   path_->setDuplicatesEnabled( false );
   path_->setMinimumWidth( 5 * QFontMetrics( font() ).width( 'X' ) );
   fstab_->setMinimumWidth( 5 * QFontMetrics( font() ).width( 'X' ) );
   tbar_->setElideMode( Qt::ElideLeft );

   //===== zaden z ponizszych elementow nie reaguje na tab =====
   path_  ->setFocusPolicy( Qt::NoFocus );
   fstab_ ->setFocusPolicy( Qt::NoFocus );
   tbar_  ->setFocusPolicy( Qt::NoFocus );
   wstack_->setFocusPolicy( Qt::NoFocus );

   // sciezka/fstab ---------------------------------------
   QVBoxLayout* const hb_top = new QVBoxLayout;

   QHBoxLayout* const hb_drivebar = new QHBoxLayout;
   hb_drivebar->setMargin( 0 );
   hb_drivebar->addWidget( fstab_ );
   hb_drivebar->addWidget( infolabel_ );
   hb_drivebar->addWidget( rootbutton_ );
   hb_drivebar->addWidget( dotdot_ );
   hb_drivebar->setStretchFactor( infolabel_, 8 );
   fstab_->setFixedWidth(80);
   rootbutton_->setFixedWidth(30);
   dotdot_->setFixedWidth(30);

   hb_top->addLayout(hb_drivebar);
   hb_top->addWidget(path_);

   // opisy dirs/files/selected ---------------------------
   QHBoxLayout* const hb_dirs = new QHBoxLayout;
   hb_dirs->addWidget( new QLabel( tr( DIRS ) ) );
   hb_dirs->addWidget( dirs_ );
   hb_dirs->setStretchFactor( dirs_, 100 );
   QHBoxLayout* const hb_files = new QHBoxLayout;
   hb_files->addWidget( new QLabel( tr( FILES ) ) );
   hb_files->addWidget( files_ );
   hb_files->setStretchFactor( files_, 100 );
   QHBoxLayout* const hb_selected = new QHBoxLayout;
   hb_selected->addWidget( new QLabel( tr( SELECTED ) ) );
   hb_selected->addWidget( selected_ );
   hb_selected->setStretchFactor( selected_, 100 );
   QGridLayout* const grid = new QGridLayout;
   grid->setMargin( 0 );
   grid->addLayout( hb_dirs, 0, 0 );
   grid->addLayout( hb_files, 0, 1 );
   grid->addLayout( hb_selected, 0, 2 );

   // main layout -----------------------------------------   
   QVBoxLayout* const vb = new QVBoxLayout;
   vb->setMargin( 4 );
   vb->setSpacing( 4 );
   vb->addLayout( hb_top );
   vb->addWidget( wstack_ );
   vb->addLayout( grid );
   vb->addWidget( tbar_ );
   vb->setStretchFactor( wstack_, 100 );
   setLayout( vb );
   
   update_dirs_count( 0 );
   update_files_count( 0 );
   update_selected_count( 0 );
   update_fstab();
   
   restore();

   QBtEventsController::instance()->append( this, QBtEvent::NEW_TAB_REQUEST   );
   QBtEventsController::instance()->append( this, QBtEvent::DEL_TAB_REQUEST   );
   QBtEventsController::instance()->append( this, QBtEvent::SHOW_LEFT_FSTAB   );
   QBtEventsController::instance()->append( this, QBtEvent::SHOW_RIGHT_FSTAB  );

   connect( path_            , SIGNAL( activated      ( const QString& ) ),
            this             , SLOT  ( path_changed   ( const QString& ) ) );
   connect( fstab_           , SIGNAL( activated      ( const QString& ) ),
            this             , SLOT  ( fstab_changed  ( const QString& ) ) );
   connect( path_->lineEdit(), SIGNAL( editingFinished()                 ),
            this             , SLOT  ( edit_finished  ()                 ) );
   connect( tbar_            , SIGNAL( currentChanged ( int )            ),
            this             , SLOT  ( tab_bar_changed( int )            ) );
   connect( tbar_            , SIGNAL( mouse_event    ()                 ),
            this             , SLOT  ( tab_mouse_event()                 ) );
}
// end of QBtPanel

//*******************************************************************
// ~QBtPanel                                              DESTRUCTOR
//*******************************************************************
QBtPanel::~QBtPanel()
{
   save();
   QBtEventsController::instance()->remove( this );
}
// end of ~QBtPanel

//*******************************************************************
// tab_bar_changed                                      PRIVATE slot
//*******************************************************************
void QBtPanel::tab_bar_changed( const int in_idx )
{
   wstack_->setCurrentIndex( in_idx );
   QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
   current_path( view->current_path() );
}
// end of tab_bar_changed

//*******************************************************************
// tab_mouse_event                                      PRIVATE slot
//*******************************************************************
void QBtPanel::tab_mouse_event()
{
   QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
   view->setFocus( Qt::MouseFocusReason );
}
// end of tab_mouse_event

//*******************************************************************
// keyPressEvent                                   PRIVATE inherited
//*******************************************************************
void QBtPanel::keyPressEvent( QKeyEvent* in_event )
{
   if( Qt::AltModifier == ( in_event->modifiers() & Qt::AltModifier ) ) {
      switch( in_event->key() ) {
         case Qt::Key_L:
            in_event->accept();
            QBtEventsController::instance()->send_event( QBtEvent::SHOW_LEFT_FSTAB );
            break;
         case Qt::Key_R:
            in_event->accept();
            QBtEventsController::instance()->send_event( QBtEvent::SHOW_RIGHT_FSTAB );
            break;
      }
   }
   QWidget::keyPressEvent( in_event );
}
// end of keyPressEvent

//*******************************************************************
// customEvent                                     PRIVATE inherited
//*******************************************************************
void QBtPanel::customEvent( QEvent* const in_event )
{
   const QBtEvent* const event = dynamic_cast<QBtEvent*>( in_event );
   const int type = static_cast<int>( event->type() );
   bool handled = false;

   const QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );

   if( view->hasFocus() ) {
      switch( type ) {
         case QBtEvent::NEW_TAB_REQUEST:
            new_tab_request();
            handled = true;
            break;
         case QBtEvent::DEL_TAB_REQUEST:
            del_tab_request();
            handled = true;
            break;
      }
   }

   if( !handled ) {
      switch( type ) {
         case QBtEvent::SHOW_LEFT_FSTAB:
            if( QBtWorkspace::LEFT_PANEL_ID == idx_ ) {
               fstab_->showPopup();
            }
            break;
         case QBtEvent::SHOW_RIGHT_FSTAB:
            if( QBtWorkspace::RIGHT_PANEL_ID == idx_ ) {
               fstab_->showPopup();
            }
            break;
      }
   }
}
// end of customEvent

//*******************************************************************
// view_with_focus                                            PUBLIC
//*******************************************************************
QBtView* const QBtPanel::current_view() const
{
   return dynamic_cast< QBtView* >( wstack_->currentWidget() );
}
// end of view_with_focus

//*******************************************************************
// set_tab_idx                                                PUBLIC
//*******************************************************************
void QBtPanel::set_tab_id( const qint32 in_id )
{
   idx_ = in_id;
}
// end of set_tab_id

//*******************************************************************
// show_fstab                                                 PUBLIC
//*******************************************************************
void QBtPanel::show_fstab()
{
   fstab_->showPopup();
}
// end of show_fstab

//*******************************************************************
// save                                                      PRIVATE
//*******************************************************************
void QBtPanel::save()
{
   QBtSettings stt;
   const QString group = ( QBtWorkspace::LEFT_PANEL_ID == idx_ )
                         ? QBtConfig::LEFT_PANEL_GROUP
                         : QBtConfig::RIGHT_PANEL_GROUP;

   if( QBtWorkspace::LEFT_PANEL_ID == idx_ ) {
      // aktualny katalog ------------------------------------
      QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
      const QString current_path = ( view ) ?  view->current_path() : QString();
      stt.save( group + QBtConfig::CURR_LEFT_DIR_KEY, current_path );
      // aktualna pozycja
      const QString current_file = view->selected_file_full_name();
      stt.save( group + QBtConfig::SELECTED_LEFT_KEY, current_file );
      // historia katalogow ----------------------------------
      QStringList path_data = QStringList();
      for( qint32 i = 1; i < path_->count(); ++i ) {
         path_data << path_->itemText( i );
      }
      stt.save( group + QBtConfig::FOLDERS_LEFT_KEY, path_data );
   }
   else {
      // aktualny katalog ------------------------------------
      QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
      const QString current_path = ( view ) ?  view->current_path() : QString();
      stt.save( group + QBtConfig::CURR_RIGHT_DIR_KEY, current_path );
      // aktualna pozycja
      const QString current_file = view->selected_file_full_name();
      stt.save( group + QBtConfig::SELECTED_RIGHT_KEY, current_file );
      // historia katalogow ----------------------------------
      QStringList path_data = QStringList();
      for( qint32 i = 1; i < path_->count(); ++i ) {
         path_data << path_->itemText( i );
      }
      stt.save( group + QBtConfig::FOLDERS_RIGHT_KEY, path_data );
   }
}
// end of save

//*******************************************************************
// restore                                                   PRIVATE
//*******************************************************************
void QBtPanel::restore()
{
   QBtSettings stt;
   QVariant data;
   const QString group = ( QBtWorkspace::LEFT_PANEL_ID == idx_ )
                         ? QBtConfig::LEFT_PANEL_GROUP
                         : QBtConfig::RIGHT_PANEL_GROUP;

   if( QBtWorkspace::LEFT_PANEL_ID == idx_ ) {
      // aktualny katalog ------------------------------------
      QString path = QString();
      if( stt.read( group + QBtConfig::CURR_LEFT_DIR_KEY, data ) ) {
         path = data.toString();
      }
      
      QFileInfo finfo( path );
      if( !finfo.exists() || !finfo.isExecutable() || !finfo.isReadable() ) {
         path = QDir::homePath();
      }
      new_tab( path );
      //-----------
      QString current_file = QString();
      if( stt.read( group + QBtConfig::SELECTED_LEFT_KEY, data ) ) {
         current_file = data.toString();
      }
      QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
      if( view ) view->set_initial_file_request( current_file );

      // historia katalogow ----------------------------------
      if( stt.read( group + QBtConfig::FOLDERS_LEFT_KEY, data ) ) {
         path_->addItems( data.toStringList() );
      }
   }
   else {
      // aktualny katalog ------------------------------------
      QString path = QString();
      if( stt.read( group + QBtConfig::CURR_RIGHT_DIR_KEY, data ) ) {
         path = data.toString();
      }
      QFileInfo finfo( path );
      if( !finfo.exists() || !finfo.isExecutable() || !finfo.isReadable() ) {
         path = QDir::homePath();
      }
      new_tab( path );
      //-----------
      QString current_file = QString();
      if( stt.read( group + QBtConfig::SELECTED_RIGHT_KEY, data ) ) {
         current_file = data.toString();
      }
      QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
      if( view ) view->set_initial_file_request( current_file );
      // historia katalogow ----------------------------------
      if( stt.read( group + QBtConfig::FOLDERS_RIGHT_KEY, data ) ) {
         path_->addItems( data.toStringList() );
      }
   }
}
// end of restore

//*******************************************************************
// new_tab_request                                           PRIVATE
//*******************************************************************
void QBtPanel::new_tab_request()
{
   const QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
   const qint32 idx = new_tab( view->current_path() );
   tbar_->setCurrentIndex( idx );
}
// end of new_tab_request

//*******************************************************************
// del_tab_request                                           PRIVATE
//*******************************************************************
void QBtPanel::del_tab_request()
{
   if( tbar_->count() > 1 ) {
      QBtView* view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
      if( view ) {
         const qint32 idx = wstack_->indexOf( view );
         wstack_->removeWidget( view );
         tbar_->removeTab( idx );
         delete view;
         view = 0;
      }
      view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
      if( view ) {
         view->setFocus( Qt::OtherFocusReason );
      }
   }
}
// end of del_tab_request

//*******************************************************************
// new_tab                                                   PRIVATE
//*******************************************************************
qint32 QBtPanel::new_tab( const QString& in_path )
{
   qint32 idx = tbar_->addTab( in_path );

   QBtView* const view = new QBtView( in_path );
   connect( view, SIGNAL( dir_count             ( qint32 ) ),
            this, SLOT  ( update_dirs_count     ( qint32 ) ) );
   connect( view, SIGNAL( file_count            ( qint32 ) ),
            this, SLOT  ( update_files_count    ( qint32 ) ) );
   connect( view, SIGNAL( select_count          ( qint32 ) ),
            this, SLOT  ( update_selected_count ( qint32 ) ) );
   connect( view, SIGNAL( path_changed          ( const QString& ) ),
            this, SLOT  ( current_path          ( const QString& ) ) );

   wstack_->insertWidget( idx, view );
   return idx;
}
// end of new_tab

//*******************************************************************
// current_path                                         PRIVATE slot
//*******************************************************************
void QBtPanel::current_path( const QString& in_path )
{
   if( in_path.isEmpty() ) return;

   path_->disconnect();
   
   const qint32 idx = path_->findText( in_path );
   if( idx != -1 ) {
      path_->removeItem( idx );
   }
   path_->insertItem( 0, in_path );
   path_->setCurrentIndex( 0 );
   
   connect( path_, SIGNAL( activated   ( const QString& ) ),
            this , SLOT  ( path_changed( const QString& ) ) );

   tbar_->setTabText( tbar_->currentIndex(), in_path );
}
// end of current_path

//*******************************************************************
// update_fstab                                              PRIVATE
//*******************************************************************
void QBtPanel::update_fstab()
{
#if !_WIN32
   QStringList args;
   args << "-h" << "-T";
   
   QProcess sc;
   sc.start( "df", args );

   QByteArray data;
   while( sc.waitForReadyRead() ) {
      data += sc.readAll();
   }

   const QStringList text = QString::fromLocal8Bit( data ).split( '\n', QString::SkipEmptyParts );
   for( qint32 i = 1; i < text.size(); ++i ) {
      const QStringList items = text[ i ].split( ' ', QString::SkipEmptyParts );
      if( items.first().startsWith( '/' ) && items.last().startsWith( '/' ) ) {
         fstab_->addItem( items.last() );
      }
   }
#else
    //On Windows its not really an FSTAB but a Drive List. So we will list all the drives
    //Windows knows about.

    char szBuffer[1024];
    GetLogicalDriveStringsA(1024, szBuffer);
    char *pch = szBuffer;
    while (*pch) {
        fstab_->addItem(pch);
        pch = &pch[strlen(pch) + 1];
    }
#endif
}
// end of update_fstab

//*******************************************************************
// update_dirs_count                                    PRIVATE slot
//*******************************************************************
void QBtPanel::update_dirs_count( const qint32 in_count )
{
   dirs_->setText( QString::number( in_count ) );
}
// end of update_dirs_count

//*******************************************************************
// update_files_count                                   PRIVATE slot
//*******************************************************************
void QBtPanel::update_files_count( const qint32 in_count )
{
   files_->setText( QString::number( in_count ) );
}
// end of update_files_count

//*******************************************************************
// update_selected_count                                PRIVATE slot
//*******************************************************************
void QBtPanel::update_selected_count( const qint32 in_count )
{
   selected_->setText( QString::number( in_count ) );
}
// end of update_selected_count

//*******************************************************************
// path_changed                                         PRIVATE slot
//*******************************************************************
void QBtPanel::path_changed( const QString& in_text )
{
   current_path( in_text );
   QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
   view->update( in_text );
}
// end of path_changed

//*******************************************************************
// fstab_changed                                        PRIVATE slot
//*******************************************************************
void QBtPanel::fstab_changed( const QString& in_path )
{
   QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
   view->setFocus( Qt::OtherFocusReason );
   view->update( in_path );
}
// end of fstab_activated

//*******************************************************************
// edit_finished                                        PRIVATE slot
//*******************************************************************
void QBtPanel::edit_finished()
{
   QBtView* const view = dynamic_cast< QBtView* >( wstack_->currentWidget() );
   view->setFocus( Qt::OtherFocusReason );
}
// end of edit_finished

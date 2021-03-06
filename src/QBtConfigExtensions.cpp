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
#include "QBtConfigExtensions.h"
#include "QBtEditExtensions.h"
#include "QBtConfig.h"
#include "QBtSettings.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QPushButton>

/*------- local constants:
-------------------------------------------------------------------*/
const char* const QBtConfigExtensions::REMOVE = QT_TR_NOOP( "&Remove" );
const char* const QBtConfigExtensions::ADD    = QT_TR_NOOP( "&Add" );
const char* const QBtConfigExtensions::EDIT   = QT_TR_NOOP( "&Edit" );
const char* const QBtConfigExtensions::EXT    = QT_TR_NOOP( "Ext" );
const char* const QBtConfigExtensions::PRG    = QT_TR_NOOP( "Program" );
const char* const QBtConfigExtensions::PAR    = QT_TR_NOOP( "Parameters" );


//*******************************************************************
// QBtConfigExtensions                                       CONSTRUCTOR
//*******************************************************************
QBtConfigExtensions::QBtConfigExtensions( QWidget* in_parent ) : QWidget( in_parent )
, table_  ( new QTreeWidget )
, remove_ ( new QPushButton( tr( REMOVE ) ) )
, add_    ( new QPushButton( tr( ADD ) ) )
, edt_    ( new QPushButton( tr( EDIT ) ) )
{
   table_->setEditTriggers( QTreeView::NoEditTriggers );
   table_->setRootIsDecorated( false );
   table_->setSortingEnabled( true );
   table_->sortByColumn( 0, Qt::AscendingOrder );
   
   QTreeWidgetItem* const header = new QTreeWidgetItem;
   header->setText( 0, tr( EXT ) );
   header->setText( 1, tr( PRG ) );
   header->setText( 2, tr( PAR ) );
   table_->setHeaderItem( header );
   
   QVBoxLayout* const btn_layout = new QVBoxLayout;
   btn_layout->addStretch( 100 );
   btn_layout->addWidget( remove_ );
   btn_layout->addStretch( 20 );
   btn_layout->addWidget( add_ );
   btn_layout->addWidget( edt_ );
   btn_layout->addStretch( 100 );

   QHBoxLayout* const main_layout = new QHBoxLayout;
   main_layout->addWidget( table_ );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( remove_, SIGNAL( clicked() ), this, SLOT( remove() ) );
   connect( add_   , SIGNAL( clicked() ), this, SLOT( add   () ) );
   connect( edt_   , SIGNAL( clicked() ), this, SLOT( edit  () ) );

   restore();
}
// end of QBtConfigExtensions

//*******************************************************************
// add_item                                                  PRIVATE
//*******************************************************************
void QBtConfigExtensions::add_item( const QString& in_ext, const QString& in_prg, const QString& in_pars )
{
   QTreeWidgetItem* const root = table_->invisibleRootItem();
   if( root ) {
      QTreeWidgetItem* const itm = new QTreeWidgetItem;
      if( itm ) {
         itm->setText( 0, in_ext );
         itm->setText( 1, in_prg );
         itm->setText( 2, in_pars );
         root->addChild( itm );
         
         table_->resizeColumnToContents( 0 );
         table_->resizeColumnToContents( 1 );
      }
   }
}
// end of add_item

//*******************************************************************
// remove                                               PRIVATE slot
//-------------------------------------------------------------------
// Usuwa aktualna pozycje w tablicy.
//*******************************************************************
void QBtConfigExtensions::remove()
{
   QTreeWidgetItem* const root = table_->invisibleRootItem();
   if( root ) {
      QTreeWidgetItem* const itm = table_->currentItem();
      if( itm ) {
         root->removeChild( itm );
      }
   }
}
// end of remove

//*******************************************************************
// add                                                  PRIVATE slot
//-------------------------------------------------------------------
// Dodaje nowa pozycje do tablicy.
//*******************************************************************
void QBtConfigExtensions::add()
{
   QBtEditExtensions dialog( this );
   if( QDialog::Accepted == dialog.exec() ) {
      QString ext = QString();
      QString prg = QString();
      QString par = QString();
      dialog.data( ext, prg, par );
      add_item( ext, prg, par );
   }
   table_->setFocus( Qt::OtherFocusReason );
}
// end of add

//*******************************************************************
// edit                                                 PRIVATE slot
//-------------------------------------------------------------------
// Edytuje aktualna pozycje tablicy.
//*******************************************************************
void QBtConfigExtensions::edit()
{
   QTreeWidgetItem* const itm = table_->currentItem();
   if( !itm ) return;
      
   QString ext = itm->text( 0 );
   QString prg = itm->text( 1 );
   QString par = itm->text( 2 );

   QBtEditExtensions dialog( this, ext, prg, par );
   if( QDialog::Accepted == dialog.exec() ) {
      dialog.data( ext, prg, par );
      itm->setText( 0, ext );
      itm->setText( 1, prg );
      itm->setText( 2, par );
   }
   table_->setFocus( Qt::OtherFocusReason );
}
// end of edit

//*******************************************************************
// restore                                                    PUBLIC
//*******************************************************************
void QBtConfigExtensions::restore()
{
   QStringList data = QStringList();

   QBtSettings stt;
   QVariant value;
   if( stt.read( QBtConfig::EXT_GROUP + QBtConfig::EXT_KEY, value ) ) {
      data = value.toStringList();
   }

   const qint32 n = data.size();
   for( qint32 i = 0; i < n; ++i ) {
      const QStringList record = data.at( i ).split( ',' );
      add_item( record.at( 0 ), record.at( 1 ), record.at( 2 ) );
   }
}
// end of restore

//*******************************************************************
// save                                                       PUBLIC
//*******************************************************************
void QBtConfigExtensions::save()
{
   QStringList data = QStringList();
   
   QTreeWidgetItem* const root = table_->invisibleRootItem();
   if( root ) {
      const qint32 n = root->childCount();
      for( qint32 i = 0; i < n; ++i ) {
         const QTreeWidgetItem* const itm = root->child( i );
         QString record = itm->text( 0 );
         record += ',';
         record += itm->text( 1 );
         record += ',';
         record += itm->text( 2 );
         data << record;
      }
   }

   QBtSettings stt;
   stt.save( QBtConfig::EXT_GROUP + QBtConfig::EXT_KEY, data );
}
// end of save

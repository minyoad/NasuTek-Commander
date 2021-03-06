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
#include "QBtConfig.h"
#include "QBtSettings.h"
#include "QBtButtonSpace.h"
#include <QApplication>
#include <QtDebug>

/*------- local constants:
-------------------------------------------------------------------*/
const QString QBtConfig::MAIN_WINDOW_GROUP = "MainWindow/";
const QString QBtConfig::POS_KEY           = "position";
const QString QBtConfig::SIZE_KEY          = "size";
const QString QBtConfig::STATE_KEY         = "state";
const QString QBtConfig::BTN_MODE_KEY      = "button mode";
const QString QBtConfig::BTN_FONT_KEY      = "button font";
const QString QBtConfig::FILTER_KEY        = "filter";
//---------------------------
const QString QBtConfig::WORKSPACE_GROUP   = "Workspace/";
const QString QBtConfig::SPLITTER_KEY      = "splitter";
//---------------------------
const QString QBtConfig::LEFT_PANEL_GROUP   = "LeftPanel/";
const QString QBtConfig::RIGHT_PANEL_GROUP  = "RightPanel/";
const QString QBtConfig::CURR_LEFT_DIR_KEY  = "current_left_dir";
const QString QBtConfig::CURR_RIGHT_DIR_KEY = "current_right_dir";
const QString QBtConfig::SELECTED_LEFT_KEY  = "selected_left_item";
const QString QBtConfig::SELECTED_RIGHT_KEY = "selected_right_item";
const QString QBtConfig::FOLDERS_LEFT_KEY   = "folders_left";
const QString QBtConfig::FOLDERS_RIGHT_KEY  = "folders_right";
//---------------------------
const QString QBtConfig::TERMINAL_GROUP     = "Terminal/";
const QString QBtConfig::DIFF_GROUP         = "Diff/";
const QString QBtConfig::USE_DEFAULT_KEY    = "use_default";
const QString QBtConfig::COMMAND_KEY        = "command";
//---------------------------
const QString QBtConfig::EXT_GROUP          = "Extensions/";
const QString QBtConfig::EXT_KEY            = "data";
//---------------------------
const QString QBtConfig::VIEW_GROUP    = "View/";
const QString QBtConfig::FONT_KEY      = "font";
const QString QBtConfig::BKG_F_NC_NS_COLOR_KEY  = "bkg f nc ns";
const QString QBtConfig::TXT_F_NC_NS_COLOR_KEY  = "txt f nc ns";
const QString QBtConfig::BKG_NF_NC_NS_COLOR_KEY = "bkg nf nc ns";
const QString QBtConfig::TXT_NF_NC_NS_COLOR_KEY = "txt nf nc ns";
const QString QBtConfig::BKG_F_NC_S_COLOR_KEY   = "bkg f nc s";
const QString QBtConfig::TXT_F_NC_S_COLOR_KEY   = "txt f nc s";
const QString QBtConfig::BKG_NF_NC_S_COLOR_KEY  = "bkg nf nc s";
const QString QBtConfig::TXT_NF_NC_S_COLOR_KEY  = "txt nf nc s";
const QString QBtConfig::BKG_F_C_S_COLOR_KEY    = "bkg f c s";
const QString QBtConfig::TXT_F_C_S_COLOR_KEY    = "txt f c s";
const QString QBtConfig::BKG_NF_C_S_COLOR_KEY   = "bkg nf c s";
const QString QBtConfig::TXT_NF_C_S_COLOR_KEY   = "txt nf c s";
const QString QBtConfig::BKG_F_C_NS_COLOR_KEY   = "bkg f c ns";
const QString QBtConfig::TXT_F_C_NS_COLOR_KEY   = "txt f c ns";
const QString QBtConfig::BKG_NF_C_NS_COLOR_KEY  = "bkg nf c ns";
const QString QBtConfig::TXT_NF_C_NS_COLOR_KEY  = "txt nf c ns";


const bool    QBtConfig::DEFAULT_FILTER_STATE       = true;
const QColor  QBtConfig::DEFAULT_BKG_F_NC_NS_COLOR  = QColor( 255, 255, 255 );
const QColor  QBtConfig::DEFAULT_TXT_F_NC_NS_COLOR  = QColor(   0,   0,   0 );
const QColor  QBtConfig::DEFAULT_BKG_NF_NC_NS_COLOR = QColor( 255, 255, 255 );
const QColor  QBtConfig::DEFAULT_TXT_NF_NC_NS_COLOR = QColor( 100, 100, 100 );
const QColor  QBtConfig::DEFAULT_BKG_F_NC_S_COLOR   = QColor( 255, 129,  95 );
const QColor  QBtConfig::DEFAULT_TXT_F_NC_S_COLOR   = QColor(   0,   0,   0 );
const QColor  QBtConfig::DEFAULT_BKG_NF_NC_S_COLOR  = QColor( 215, 220, 205 );
const QColor  QBtConfig::DEFAULT_TXT_NF_NC_S_COLOR  = QColor( 211,  71,  90 );
const QColor  QBtConfig::DEFAULT_BKG_F_C_S_COLOR    = QColor( 255,  10,  10 );
const QColor  QBtConfig::DEFAULT_TXT_F_C_S_COLOR    = QColor( 255, 255, 255 );
const QColor  QBtConfig::DEFAULT_BKG_NF_C_S_COLOR   = QColor( 170, 170, 170 );
const QColor  QBtConfig::DEFAULT_TXT_NF_C_S_COLOR   = QColor( 211,  71,  90 );
const QColor  QBtConfig::DEFAULT_BKG_F_C_NS_COLOR   = QColor(   0,  85, 255 );
const QColor  QBtConfig::DEFAULT_TXT_F_C_NS_COLOR   = QColor( 255, 255, 255 );
const QColor  QBtConfig::DEFAULT_BKG_NF_C_NS_COLOR  = QColor( 150, 150, 150 );
const QColor  QBtConfig::DEFAULT_TXT_NF_C_NS_COLOR  = QColor( 255, 255, 255 );


//*******************************************************************
// QBtConfig                                     CONSTRUCTOR private
//*******************************************************************
QBtConfig::QBtConfig() : QObject()
{
   setParent( qApp );
   read_data();
}
// end of QBtConfig

//*******************************************************************
// ~QBtConfig                                      DESTRUCTOR public
//*******************************************************************
QBtConfig::~QBtConfig()
{}
// end of ~QBtConfig

//*******************************************************************
// instance                                            PUBLIC static
//*******************************************************************
QBtConfig* const QBtConfig::instance()
{
   static QBtConfig* ptr = 0;
   if( !ptr ) ptr = new QBtConfig;
   return ptr;
}
// end of instance

//*******************************************************************
// read_data                                                 PRIVATE
//*******************************************************************
void QBtConfig::read_data()
{
   QBtSettings stt;
   QVariant    data;

   // filter state
   filter_ = DEFAULT_FILTER_STATE;
   if( stt.read( MAIN_WINDOW_GROUP + FILTER_KEY, data ) ) {
      filter_ = data.toBool();
   }
   // button mode
   btn_mode_ = QBtButtonSpace::FULL_DESC;
   if( stt.read( MAIN_WINDOW_GROUP + BTN_MODE_KEY, data ) ) {
      btn_mode_ = data.toInt();
   }
   // button font
   btn_font_ = QFont();
   if( stt.read( MAIN_WINDOW_GROUP + BTN_FONT_KEY, data ) ) {
      btn_font_.fromString( data.toString() );
   }

   // font
   font_ = QFont();
   if( stt.read( VIEW_GROUP + FONT_KEY, data ) ) {
      font_.fromString( data.toString() );
   }
   // bkg f nc ns
   bkg_f_nc_ns_color_ = DEFAULT_BKG_F_NC_NS_COLOR;
   if( stt.read( VIEW_GROUP + BKG_F_NC_NS_COLOR_KEY, data ) ) {
      bkg_f_nc_ns_color_.setNamedColor( data.toString() );
   }
   // txt f nc ns
   txt_f_nc_ns_color_ = DEFAULT_TXT_F_NC_NS_COLOR;
   if( stt.read( VIEW_GROUP + TXT_F_NC_NS_COLOR_KEY, data ) ) {
      txt_f_nc_ns_color_.setNamedColor( data.toString() );
   }
   // bkg nf nc ns
   bkg_nf_nc_ns_color_ = DEFAULT_BKG_NF_NC_NS_COLOR;
   if( stt.read( VIEW_GROUP + BKG_NF_NC_NS_COLOR_KEY, data ) ) {
      bkg_nf_nc_ns_color_.setNamedColor( data.toString() );
   }
   // txt nf nc ns
   txt_nf_nc_ns_color_ = DEFAULT_TXT_NF_NC_NS_COLOR;
   if( stt.read( VIEW_GROUP + TXT_NF_NC_NS_COLOR_KEY, data ) ) {
      txt_nf_nc_ns_color_.setNamedColor( data.toString() );
   }
   // bkg f nc s
   bkg_f_nc_s_color_ = DEFAULT_BKG_F_NC_S_COLOR;
   if( stt.read( VIEW_GROUP + BKG_F_NC_S_COLOR_KEY, data ) ) {
      bkg_f_nc_s_color_.setNamedColor( data.toString() );
   }
   // txt f nc s
   txt_f_nc_s_color_ = DEFAULT_TXT_F_NC_S_COLOR;
   if( stt.read( VIEW_GROUP + TXT_F_NC_S_COLOR_KEY, data ) ) {
      txt_f_nc_s_color_.setNamedColor( data.toString() );
   }
   // bkg nf nc s
   bkg_nf_nc_s_color_ = DEFAULT_BKG_NF_NC_S_COLOR;
   if( stt.read( VIEW_GROUP + BKG_NF_NC_S_COLOR_KEY, data ) ) {
      bkg_nf_nc_s_color_.setNamedColor( data.toString() );
   }
   // txt nf nc s
   txt_nf_nc_s_color_ = DEFAULT_TXT_NF_NC_S_COLOR;
   if( stt.read( VIEW_GROUP + TXT_NF_NC_S_COLOR_KEY, data ) ) {
      txt_nf_nc_s_color_.setNamedColor( data.toString() );
   }
   // bkg f c s
   bkg_f_c_s_color_ = DEFAULT_BKG_F_C_S_COLOR;
   if( stt.read( VIEW_GROUP + BKG_F_C_S_COLOR_KEY, data ) ) {
      bkg_f_c_s_color_.setNamedColor( data.toString() );
   }
   // txt f c s
   txt_f_c_s_color_ = DEFAULT_TXT_F_C_S_COLOR;
   if( stt.read( VIEW_GROUP + TXT_F_C_S_COLOR_KEY, data ) ) {
      txt_f_c_s_color_.setNamedColor( data.toString() );
   }
   // bkg nf c s
   bkg_nf_c_s_color_ = DEFAULT_BKG_NF_C_S_COLOR;
   if( stt.read( VIEW_GROUP + BKG_NF_C_S_COLOR_KEY, data ) ) {
      bkg_nf_c_s_color_.setNamedColor( data.toString() );
   }
   // txt nf c s
   txt_nf_c_s_color_ = DEFAULT_TXT_NF_C_S_COLOR;
   if( stt.read( VIEW_GROUP + TXT_NF_C_S_COLOR_KEY, data ) ) {
      txt_nf_c_s_color_.setNamedColor( data.toString() );
   }
   // bkg f c ns
   bkg_f_c_ns_color_ = DEFAULT_BKG_F_C_NS_COLOR;
   if( stt.read( VIEW_GROUP + BKG_F_C_NS_COLOR_KEY, data ) ) {
      bkg_f_c_ns_color_.setNamedColor( data.toString() );
   }
   // txt f c ns
   txt_f_c_ns_color_ = DEFAULT_TXT_F_C_NS_COLOR;
   if( stt.read( VIEW_GROUP + TXT_F_C_NS_COLOR_KEY, data ) ) {
      txt_f_c_ns_color_.setNamedColor( data.toString() );
   }
   // bkg nf c ns
   bkg_nf_c_ns_color_ = DEFAULT_BKG_NF_C_NS_COLOR;
   if( stt.read( VIEW_GROUP + BKG_NF_C_NS_COLOR_KEY, data ) ) {
      bkg_nf_c_ns_color_.setNamedColor( data.toString() );
   }
   // txt nf c ns
   txt_nf_c_ns_color_ = DEFAULT_TXT_NF_C_NS_COLOR;
   if( stt.read( VIEW_GROUP + TXT_NF_C_NS_COLOR_KEY, data ) ) {
      txt_nf_c_ns_color_.setNamedColor( data.toString() );
   }
}
// end of read_data

//*******************************************************************
// save_data                                                  PUBLIC
//*******************************************************************
void QBtConfig::save_data() const
{
   QBtSettings stt;

   // filter state
   stt.save( MAIN_WINDOW_GROUP + FILTER_KEY, filter_ );
   // button mode
   stt.save( MAIN_WINDOW_GROUP + BTN_MODE_KEY, btn_mode_ );
   // button font
   stt.save( MAIN_WINDOW_GROUP + BTN_FONT_KEY, btn_font_.toString() );

   // font
   stt.save( VIEW_GROUP + FONT_KEY, font_.toString() );
   // background color
   stt.save( VIEW_GROUP + BKG_F_NC_NS_COLOR_KEY, bkg_f_nc_ns_color_.name() );
   // text color
   stt.save( VIEW_GROUP + TXT_F_NC_NS_COLOR_KEY, txt_f_nc_ns_color_.name() );
   // bkg nf nc ns
   stt.save( VIEW_GROUP + BKG_NF_NC_NS_COLOR_KEY, bkg_nf_nc_ns_color_.name() );
   // txt nf nc ns
   stt.save( VIEW_GROUP + TXT_NF_NC_NS_COLOR_KEY, txt_nf_nc_ns_color_.name() );
   // bkg f nc s
   stt.save( VIEW_GROUP + BKG_F_NC_S_COLOR_KEY, bkg_f_nc_s_color_.name() );
   // txt f nc s
   stt.save( VIEW_GROUP + TXT_F_NC_S_COLOR_KEY, txt_f_nc_s_color_.name() );
   // bkg nf nc s
   stt.save( VIEW_GROUP + BKG_NF_NC_S_COLOR_KEY, bkg_nf_nc_s_color_.name() );
   // txt nf nc s
   stt.save( VIEW_GROUP + TXT_NF_NC_S_COLOR_KEY, txt_nf_nc_s_color_.name() );
   // bkg f c s
   stt.save( VIEW_GROUP + BKG_F_C_S_COLOR_KEY, bkg_f_c_s_color_.name() );
   // txt f c s
   stt.save( VIEW_GROUP + TXT_F_C_S_COLOR_KEY, txt_f_c_s_color_.name() );
   // bkg nf c s
   stt.save( VIEW_GROUP + BKG_NF_C_S_COLOR_KEY, bkg_nf_c_s_color_.name() );
   // txt nf c s
   stt.save( VIEW_GROUP + TXT_NF_C_S_COLOR_KEY, txt_nf_c_s_color_.name() );
   // bkg f c ns
   stt.save( VIEW_GROUP + BKG_F_C_NS_COLOR_KEY, bkg_f_c_ns_color_.name() );
   // txt f c ns
   stt.save( VIEW_GROUP + TXT_F_C_NS_COLOR_KEY, txt_f_c_ns_color_.name() );
   // bkg nf c ns
   stt.save( VIEW_GROUP + BKG_NF_C_NS_COLOR_KEY, bkg_nf_c_ns_color_.name() );
   // txt nf c ns
   stt.save( VIEW_GROUP + TXT_NF_C_NS_COLOR_KEY, txt_nf_c_ns_color_ );
}
// end of save_data

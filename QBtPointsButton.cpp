/********************************************************************
 * Copyright (C) Piotr Pszczolkowski; Copyright (C) 2012 NasuTek Enterprises
 *-------------------------------------------------------------------
 * This file is part of NasuTek Commander.
 *
 * NasuTek Commander is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * NasuTek Commander is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NasuTek Commander; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *-------------------------------------------------------------------
 * Project      : NasuTek Commander
 * File         : QBtPointsButton.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2007.11.25
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtPointsButton.h"

/*------- local constants:
-------------------------------------------------------------------*/
const QString QBtPointsButton::POINTS = "...";


//*******************************************************************
// QBtPointsButton                                       CONSTRUCTOR
//*******************************************************************
QBtPointsButton::QBtPointsButton( QWidget* const in_parent )
: QPushButton( POINTS, in_parent )
{}
// end of QBtPointsButton
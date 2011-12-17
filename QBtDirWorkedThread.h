#ifndef INCLUDED_QBtWorkedThread_h
#define INCLUDED_QBtWorkedThread_h

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
 * File         : QBtDirWorkedThread.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 31.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtWorkedThread.h"

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDirWorkedThread : public QBtWorkedThread
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDirWorkedThread( QObject* = 0 );
private:
   QBtDirWorkedThread( const QBtDirWorkedThread& );
   QBtDirWorkedThread& operator=( const QBtDirWorkedThread& );

//******* METHODS *******
public:
   virtual void update( const QString& );
   virtual void run   ();
};

#endif // INCLUDED_QBtDirWorkedThread_h
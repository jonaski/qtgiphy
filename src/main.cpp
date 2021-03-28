/*
   This file is part of QtGiphy
   Copyright 2021, Jonas Kvinge <jonas@jkvinge.net>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <version.h>
#include <config.h>

#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QLoggingCategory>

#include "logging.h"

int main(int argc, char *argv[]) {

  QCoreApplication::setApplicationName("qtgiphy");
  QCoreApplication::setOrganizationName("qtgiphy");
  QCoreApplication::setApplicationVersion(QTGIPHY_VERSION_DISPLAY);
  QCoreApplication::setOrganizationDomain("jkvinge.net");

  QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

  logging::Init();

  QApplication a(argc, argv);

  Q_INIT_RESOURCE(data);
  Q_INIT_RESOURCE(icons);

  MainWindow w;

  return a.exec();

}

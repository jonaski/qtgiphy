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

#include <QDir>
#include <QFile>
#include <QList>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QDebug>

#include "iconloader.h"

QIcon IconLoader::Load(const QString &name, const int fixed_size, const int, const int) {

  QIcon ret;

  if (name.isEmpty()) {
    qDebug() << "Icon name is empty!";
    return ret;
  }

  QList<int> sizes;
  if (fixed_size == 0) { sizes << 22 << 32 << 48 << 64; }
  else sizes << fixed_size;

  const QString path(":/icons/%1x%2/%3.png");
  for (int s : sizes) {
    QString filename(path.arg(s).arg(s).arg(name));
    if (QFile::exists(filename)) ret.addFile(filename, QSize(s, s));
  }

  return ret;

}

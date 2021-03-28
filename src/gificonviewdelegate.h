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

#ifndef GIFICONVIEWDELEGATE_H
#define GIFICONVIEWDELEGATE_H

#include <QStyledItemDelegate>

class GifIconViewDelegate : public QStyledItemDelegate {
  Q_OBJECT

 public:
  GifIconViewDelegate(QObject *parent = nullptr);
  ~GifIconViewDelegate();

  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &idx) const override;
};

#endif // GIFICONVIEWDELEGATE_H

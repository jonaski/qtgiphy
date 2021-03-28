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

#ifndef GIFICONMODEL_H
#define GIFICONMODEL_H

#include <memory>

#include <QtGlobal>
#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QString>
#include <QPixmap>

#include "gificonitem.h"

class GifIconModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit GifIconModel(QObject *parent = nullptr);

  enum Role {
    Role_Movie = Qt::UserRole + 1,
    Role_Size,
    Role_Last
  };

  void appendRow(GifIconItemPtr item) { AddItems(GifIconItemList() << item); }
  void AddItems(GifIconItemList items);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override { Q_UNUSED(parent); return items_.count(); }

  GifIconItemPtr item_at(const int i) { return items_[i]; }

  void Reset();

 private:
  int columnCount(const QModelIndex &parent = QModelIndex()) const override { Q_UNUSED(parent); return 1; }
  Qt::ItemFlags flags(const QModelIndex &idx) const override;

 protected:
  QVariant data(const QModelIndex &idx, int role) const override;

  GifIconItemList items_;
  QPixmap pixmap_;

};

#endif // GIFICONMODEL_H

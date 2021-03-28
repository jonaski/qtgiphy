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

#include <QAbstractListModel>
#include <QPixmap>
#include <QSize>

#include "logging.h"
#include "iconloader.h"
#include "gificonmodel.h"
#include "gificonitem.h"
#include "gificonview.h"

GifIconModel::GifIconModel(QObject *parent) : QAbstractListModel(parent) {}

QVariant GifIconModel::data(const QModelIndex &idx, int role) const {

  if (!idx.isValid()) return QVariant();

  GifIconItemPtr item = items_[idx.row()];
  switch (role) {
    case Qt::DisplayRole:
      break;
    case Qt::DecorationRole:
      break;
    case Role_Size:
      return item->size();
      break;
    case Qt::SizeHintRole:
      return item->scaled_movie()->scaledSize();
      break;
    case Role_Movie:
      return QVariant::fromValue<std::shared_ptr<QMovie>>(item->scaled_movie());
    default:
      break;
  }

  return QVariant();

}

Qt::ItemFlags GifIconModel::flags(const QModelIndex &idx) const {

  if (!idx.isValid()) return Qt::ItemFlags();

  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  return flags;

}

void GifIconModel::AddItems(GifIconItemList items) {

  const qint64 start = items_.count();
  const qint64 end = start + items.count() - 1;

  beginInsertRows(QModelIndex(), start, end);

  for (qint64 i = start; i <= end; ++i) {
    GifIconItemPtr item = items[i - start];
    items_.insert(i, item);

  }

  endInsertRows();

  emit dataChanged(index(0, 0), index(rowCount() - 1, 0));

}

void GifIconModel::Reset() {

  beginRemoveRows(QModelIndex(), 0, items_.count());
  items_.clear();
  endRemoveRows();

}

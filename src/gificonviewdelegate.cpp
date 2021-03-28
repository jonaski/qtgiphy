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

#include <memory>

#include <QObject>
#include <QMap>
#include <QStyledItemDelegate>
#include <QMovie>
#include <QVariant>
#include <QLabel>
#include <QPainter>

#include "logging.h"
#include "gificonview.h"
#include "gificonviewdelegate.h"
#include "gificonitem.h"
#include "gificonmodel.h"
#include "gificonview.h"

GifIconViewDelegate::GifIconViewDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

GifIconViewDelegate::~GifIconViewDelegate() {}

void GifIconViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &idx) const {

  const QVariant data = idx.data(GifIconModel::Role_Movie);
  if (!data.canConvert<std::shared_ptr<QMovie>>()) return;

  std::shared_ptr<QMovie> movie = data.value<std::shared_ptr<QMovie>>();
  if (!movie) return;

  GifIconView *view = qobject_cast<GifIconView*>(const_cast<QWidget*>(option.widget));
  if (!view) return;

  const QSize size = idx.data(GifIconModel::Role_Size).toSize();
  const QString text = QString::number(size.width()) + "x" + QString::number(size.height());

  QFont font(option.font);
  font.setPointSizeF(6.0);
  font.setBold(true);

  const QFontMetrics metrics(font);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
  const int text_width = metrics.horizontalAdvance(text);
#else
  const int text_width = metrics.width(text);
#endif

  const QRect image_rect(option.rect.left(), option.rect.top(), option.rect.width(), option.rect.width());
  const QRect background_rect(image_rect.right() - 4 - text_width - 3 * 2, image_rect.bottom() - 4 - metrics.height() - 1 * 2, text_width + 3 * 2, metrics.height() + 1 * 2);
  const QRect text_rect(background_rect.left() + 3, background_rect.top() + 1, text_width, metrics.height());

  painter->save();

  if (view->selectionModel()->selectedIndexes().contains(idx)) {
    painter->setOpacity(0.6);
  }

  // GIF
  painter->drawPixmap(option.rect, movie->currentPixmap());

  // Background
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(QColor(0, 0, 0, 200));
  painter->setBrush(QColor(0, 0, 0, 175));
  painter->drawRoundedRect(background_rect, 5.0, 5.0);

  // Text
  painter->setPen(Qt::white);
  painter->setFont(font);
  painter->drawText(text_rect, text);

  painter->restore();

  QStyledItemDelegate::paint(painter, option, idx);

}

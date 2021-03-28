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

#include <QByteArray>
#include <QString>
#include <QUrl>
#include <QBuffer>
#include <QMovie>
#include <QImage>

#include "gificonitem.h"

GifIconItem::GifIconItem(const QUrl &url, const QString &title) : url_(url), title_(title) {}

GifIconItem::~GifIconItem() {}

void GifIconItem::Init(const QString &mimetype, const QByteArray &data, const QImage &image) {

  mimetype_ = mimetype;
  data_ = data;
  size_ = image.size();
  buffer_scaled_ = std::make_shared<QBuffer>(&data_);
  scaled_movie_ = std::make_shared<QMovie>(buffer_scaled_.get());
  scaled_movie_->setScaledSize(QSize(200, 200));
  scaled_movie_->start();

}

void GifIconItem::StartFull() {

  buffer_full_ = std::make_shared<QBuffer>(&data_);
  full_movie_ = std::make_shared<QMovie>(buffer_full_.get());
  full_movie_->start();

}

void GifIconItem::StopFull() {

  full_movie_->stop();
  full_movie_.reset();
  buffer_full_.reset();

}

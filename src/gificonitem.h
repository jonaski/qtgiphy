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

#ifndef GIFICONITEM_H
#define GIFICONITEM_H

#include <memory>

#include <QtGlobal>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QUrl>
#include <QBuffer>
#include <QMovie>
#include <QSize>
#include <QImage>

Q_DECLARE_METATYPE(std::shared_ptr<QMovie>)

class GifIconItem : public std::enable_shared_from_this<GifIconItem> {
 public:
  explicit GifIconItem(const QUrl &url = QUrl(), const QString &title = QString());
  ~GifIconItem();

  void Init(const QString &mimetype, const QByteArray &data, const QImage &image);

  QUrl url() const { return url_; }
  QString title() const { return title_; }
  QString mimetype() const { return mimetype_; }
  QByteArray data() const { return data_; }
  QSize size() const { return size_; }

  std::shared_ptr<QMovie> full_movie() const { return full_movie_; }
  std::shared_ptr<QMovie> scaled_movie() const { return scaled_movie_; }

  void StartFull();
  void StopFull();

 protected:
  QUrl url_;
  QString title_;
  QString mimetype_;
  QByteArray data_;
  QSize size_;

  std::shared_ptr<QBuffer> buffer_scaled_;
  std::shared_ptr<QBuffer> buffer_full_;
  std::shared_ptr<QMovie> scaled_movie_;
  std::shared_ptr<QMovie> full_movie_;
};

typedef std::shared_ptr<GifIconItem> GifIconItemPtr;
typedef QList<GifIconItemPtr> GifIconItemList;

#endif // GIFICONITEM_H

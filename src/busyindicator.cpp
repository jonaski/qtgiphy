/*
   Copyright 2010, David Sansome <me@davidsansome.com>
   Copyright 2018-2021, Jonas Kvinge <jonas@jkvinge.net>

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

#include <QWidget>
#include <QString>
#include <QMovie>
#include <QLabel>
#include <QSizePolicy>
#include <QBoxLayout>

#include "busyindicator.h"

class QHideEvent;
class QShowEvent;

BusyIndicator::BusyIndicator(const QString &text, QWidget* parent)
  : QWidget(parent) {
  Init(text);
}

BusyIndicator::BusyIndicator(QWidget* parent)
  : QWidget(parent) {
  Init(QString());
}

void BusyIndicator::Init(const QString &text) {

  movie_.reset(new QMovie(":/pictures/spinner.gif"));
  label_ = new QLabel;

  QLabel *icon = new QLabel;
  icon->setMovie(movie_.get());
  icon->setMinimumSize(16, 16);

  label_->setWordWrap(true);
  label_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(icon);
  layout->addSpacing(6);
  layout->addWidget(label_);

  set_text(text);

}

BusyIndicator::~BusyIndicator() {}

void BusyIndicator::showEvent(QShowEvent*) {
  movie_->start();
}

void BusyIndicator::hideEvent(QHideEvent*) {
  movie_->stop();
}

void BusyIndicator::set_text(const QString &text) {
  label_->setText(text);
  label_->setVisible(!text.isEmpty());
}

QString BusyIndicator::text() const {
  return label_->text();
}

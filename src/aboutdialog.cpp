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

#include "config.h"

#include <memory>

#include <QtGlobal>
#include <QCoreApplication>
#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QString>
#include <QIcon>
#include <QFlags>
#include <QPoint>
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QKeySequence>
#include <QMovie>
#include <QCloseEvent>
#include <QMouseEvent>

#include "iconloader.h"
#include "logging.h"

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui_(new Ui_AboutDialog), dopefish_(new QMovie(":/pictures/dopefish.gif")) {

  ui_->setupUi(this);

  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  setWindowTitle(tr("About QtGiphy"));

  ui_->label_icon->setPixmap(IconLoader::Load("giphy").pixmap(64));

  QFont title_font;
  title_font.setBold(true);
  title_font.setPointSize(title_font.pointSize() + 2);
  ui_->headline->setFont(title_font);

  QString html;
  html += "<p>";
  html += tr("About QtGiphy");
  html += "<br />";
  html += tr("Version %1").arg(QCoreApplication::applicationVersion());
  html += "</p>";
  html += tr("</p>");
  ui_->headline->setText(html);

  html.clear();

  html += "<p>";
  html += tr("QtGiphy is a small program to show and save GIF's from the Giphy API written by %1.").arg("<a href=\"https://github.com/jonaski\">Jonas Kvinge</a>");
  html += "</p>";
  ui_->text->setText(html);

  ui_->buttonBox->button(QDialogButtonBox::Close)->setShortcut(QKeySequence::Close);

  connect(ui_->buttonBox, &QDialogButtonBox::accepted, this, &AboutDialog::Close);
  connect(ui_->buttonBox, &QDialogButtonBox::rejected, this, &AboutDialog::Close);

}

void AboutDialog::closeEvent(QCloseEvent *e) {

  HideDopeFish();

  QDialog::closeEvent(e);

}

void AboutDialog::Close() {

  HideDopeFish();
  hide();

}

void AboutDialog::HideDopeFish() {

  if (dopefish_->state() == QMovie::Running) {
    dopefish_->stop();
    ui_->label_icon->clear();
    ui_->label_icon->setPixmap(IconLoader::Load("giphy").pixmap(64));
  }

}

void AboutDialog::mouseDoubleClickEvent(QMouseEvent *e) {

  if (e->button() == Qt::RightButton) {
    ui_->label_icon->clear();
    ui_->label_icon->setMovie(dopefish_.get());
    dopefish_->setSpeed(250);
    dopefish_->start();
  }

  QDialog::mouseDoubleClickEvent(e);

}

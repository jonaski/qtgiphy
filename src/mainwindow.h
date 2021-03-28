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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory.h>

#include <QMainWindow>
#include <QPoint>
#include <QModelIndex>

#include "gificonitem.h"

class QMenu;
class QNetworkAccessManager;
class QNetworkReply;

class Ui_MainWindow;
class AboutDialog;
class GifIconModel;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  static const char *kUrlSearch;
  static const char *kAPITokenB64;

  void RequestGIF(GifIconItemPtr item);
  void ShowGif(GifIconItemPtr item);
  void SaveGif(GifIconItemPtr item);

 private slots:
  void ShowAbout();
  void Exit();
  void Search();
  void Clear();
  void SearchReply(QNetworkReply *reply);
  void RequestGifReply(QNetworkReply *reply, GifIconItemPtr item);
  void DoubleClicked(const QModelIndex &idx);
  void GifIconRightClick(const QPoint &global_pos, const QModelIndex &idx);
  void ShowGifRightClick();
  void SaveGifRightClick();

 private:
  Ui_MainWindow *ui_;
  AboutDialog *aboutdialog_;
  GifIconModel *model_;
  QNetworkAccessManager *network_;
  QMenu *menu_;
  QModelIndex menu_index_;
  GifIconItemList items_;
  int requests_;

};

#endif // MAINWINDOW_H

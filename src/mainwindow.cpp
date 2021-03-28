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

#include <QMainWindow>
#include <QByteArray>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QFileDialog>
#include <QMimeType>
#include <QMimeData>
#include <QMimeDatabase>
#include <QImage>
#include <QImageWriter>
#include <QIcon>
#include <QDropEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "logging.h"
#include "iconloader.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gificonmodel.h"
#include "gificonview.h"

#include "aboutdialog.h"

const char *MainWindow::kUrlSearch = "https://api.giphy.com/v1/gifs/search";
const char *MainWindow::kAPITokenB64 = "UzI1YlVDa1k0aWY3Q0FkQ2MyNjFackN4QVAwNlZBZzc=";

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui_(new Ui_MainWindow),
  aboutdialog_(new AboutDialog(this)),
  model_(new GifIconModel(this)),
  network_(new QNetworkAccessManager(this)),
  menu_(new QMenu(this)),
  requests_(0) {

  ui_->setupUi(this);

  setWindowIcon(IconLoader::Load("giphy"));

  ui_->result->setModel(model_);
  ui_->busy->hide();

  ui_->action_about_qt->setIcon(QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
  ui_->action_about->setIcon(IconLoader::Load("giphy"));
  ui_->action_exit->setIcon(IconLoader::Load("application-exit"));
  //ui_->action_settings->setIcon(IconLoader::Load("configure"));

  QObject::connect(ui_->exit, &QPushButton::clicked, this, &MainWindow::Exit);
  QObject::connect(ui_->action_exit, &QAction::triggered, this, &MainWindow::Exit);

  QObject::connect(ui_->action_about, &QAction::triggered, this, &MainWindow::ShowAbout);
  QObject::connect(ui_->action_about_qt, &QAction::triggered, qApp, &QApplication::aboutQt);

  QObject::connect(ui_->search, &QPushButton::clicked, this, &MainWindow::Search);
  QObject::connect(ui_->clear, &QPushButton::clicked, this, &MainWindow::Clear);

  QObject::connect(ui_->result, &QAbstractItemView::doubleClicked, this, &MainWindow::DoubleClicked);
  QObject::connect(ui_->result, &GifIconView::RightClicked, this, &MainWindow::GifIconRightClick);

  menu_->addAction(IconLoader::Load("refresh"), tr("Show Gif"), this, &MainWindow::ShowGifRightClick);
  menu_->addAction(IconLoader::Load("document-save"), tr("Save Gif"), this, &MainWindow::SaveGifRightClick);

  ui_->clear->setIcon(IconLoader::Load("edit-clear-list"));
  ui_->exit->setIcon(IconLoader::Load("application-exit"));

  show();

}

MainWindow::~MainWindow() {

  delete ui_;

}

void MainWindow::ShowAbout() {

  aboutdialog_->show();
  aboutdialog_->activateWindow();

}

void MainWindow::Exit() {

  QApplication::quit();
  qApp->quit();

}

void MainWindow::Search() {

  if (ui_->search->text().isEmpty()) return;

  Clear();

  requests_ = 0;
  ui_->busy->show();

  QUrl url(kUrlSearch);
  QUrlQuery query;
  query.addQueryItem("api_key", QByteArray::fromBase64(kAPITokenB64));
  query.addQueryItem("q", QUrl::toPercentEncoding(ui_->text->text()));
  query.addQueryItem("limit", QString::number(ui_->limit->value()));
  url.setQuery(query);
  QNetworkRequest req(url);
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
  req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
#else
  req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif
  QNetworkReply *reply = network_->get(req);
  QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() { MainWindow::SearchReply(reply); });

}

void MainWindow::SearchReply(QNetworkReply *reply) {

  reply->deleteLater();

  QByteArray data = reply->readAll();

  QJsonParseError json_error;
  QJsonDocument json_doc = QJsonDocument::fromJson(data, &json_error);
  if (json_error.error != QJsonParseError::NoError || json_doc.isEmpty() || !json_doc.isObject()) return;

  QJsonObject json_obj = json_doc.object();

  if (json_obj.isEmpty() || !json_obj.contains("data") || !json_obj["data"].isArray()) return;

  QJsonArray array = json_obj["data"].toArray();

  for (const QJsonValueRef value : array) {

    if (!value.isObject()) continue;

    QJsonObject obj = value.toObject();
    if (!obj.contains("title") || !obj.contains("images")) continue;

    QString title = obj["title"].toString();

    QJsonObject images = obj["images"].toObject();
    if (!images.contains("original")) {
      continue;
    }

    QJsonObject original = images["original"].toObject();
    if (!original.contains("url")) {
      continue;
    }
    QUrl url = original["url"].toString();

    GifIconItemPtr item = std::make_shared<GifIconItem>(url, title);

    RequestGIF(item);

  }

}

void MainWindow::RequestGIF(GifIconItemPtr item) {

  QNetworkRequest req(item->url());
  QNetworkReply *reply = network_->get(req);
  ++requests_;
  QObject::connect(reply, &QNetworkReply::finished, this, [this, reply, item]() { RequestGifReply(reply, item); });

}

void MainWindow::RequestGifReply(QNetworkReply *reply, GifIconItemPtr item) {

  reply->deleteLater();

  --requests_;

  QByteArray data = reply->readAll();

  QString mimetype = QMimeDatabase().mimeTypeForData(data).name();
  QImage image;

  if (mimetype == "image/gif" && image.loadFromData(data)) {
    item->Init(mimetype, data, image);
    model_->appendRow(item);
  }

  if (requests_ == 0) ui_->busy->hide();

}

void MainWindow::Clear() {
  model_->Reset();
}

void MainWindow::DoubleClicked(const QModelIndex &idx) {

  if (!idx.isValid()) return;

  GifIconItemPtr item = model_->item_at(idx.row());

  ShowGif(item);

}


void MainWindow::ShowGif(GifIconItemPtr item) {

  if (items_.contains(item)) return;

  QDialog *dialog = new QDialog(this);
  dialog->setAttribute(Qt::WA_DeleteOnClose, true);
  QLabel *label = new QLabel(dialog);

  item->StartFull();
  items_<< item;

  QObject::connect(dialog, &QDialog::destroyed, this, [this, item](){
    item->StopFull();
    if (items_.contains(item)) {
      items_.removeAll(item);
    }
  });

  label->setMovie(item->full_movie().get());

  QString title_text = item->title();
  QSize size = item->full_movie()->currentPixmap().size();
  title_text += " (" + QString::number(size.width()) + "x" + QString::number(size.height()) + "px)";

  dialog->setWindowTitle(title_text);
  dialog->setFixedSize(size);
  dialog->show();

}

void MainWindow::SaveGif(GifIconItemPtr item) {

  QString initial_file_name = item->title() + ".gif";
  initial_file_name = initial_file_name.toLower();
  initial_file_name.replace(QRegularExpression("\\s"), "-");

  QString save_filename = QFileDialog::getSaveFileName(this, tr("Save Gif"), initial_file_name, tr("Images (*.gif)") + ";;" + tr("All files (*)"));

  if (save_filename.isEmpty()) return;

  QFileInfo fileinfo(save_filename);
  if (fileinfo.suffix().isEmpty()) {
    save_filename.append(".gif");
    fileinfo.setFile(save_filename);
  }

  QFile file(save_filename);
  if (file.open(QIODevice::WriteOnly)) {
    file.write(item->data());
    file.close();
  }

}

void MainWindow::GifIconRightClick(const QPoint &global_pos, const QModelIndex &idx) {

  if (!idx.isValid()) return;

  menu_index_ = idx;

  menu_->popup(global_pos);

}

void MainWindow::ShowGifRightClick() {

  if (!menu_index_.isValid()) return;

  GifIconItemPtr item = model_->item_at(menu_index_.row());

  ShowGif(item);

}

void MainWindow::SaveGifRightClick() {

  if (!menu_index_.isValid()) return;

  GifIconItemPtr item = model_->item_at(menu_index_.row());

  SaveGif(item);

}

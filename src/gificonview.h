/*
   This file is part of QtGiphy
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

#ifndef GIFICONVIEW_H
#define GIFICONVIEW_H

#include <QListView>
#include <QVector>
#include <QString>
#include <QPalette>
#include <QPoint>
#include <QRect>
#include <QRegion>

class QAbstractItemModel;
class QPainter;
class QModelIndex;
class QPaintEvent;
class QResizeEvent;
class QContextMenuEvent;

class GifIconView : public QListView {
  Q_OBJECT

  // Horizontal space separating an item from the left and right edges of the widget.
  Q_PROPERTY(int item_indent READ item_indent WRITE set_item_indent)

 public:
  explicit GifIconView(QWidget *parent = nullptr);

  int item_indent() const { return item_indent_; }
  void set_item_indent(const int value) { item_indent_ = value; }

  // QAbstractItemView
  QModelIndex moveCursor(CursorAction action, Qt::KeyboardModifiers modifiers) override;
  void setModel(QAbstractItemModel *model) override;

 protected:
  // QWidget
  void paintEvent(QPaintEvent *e) override;
  void resizeEvent(QResizeEvent *e) override;

  // QAbstractItemView
  void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>& = QVector<int>()) override;
  QModelIndex indexAt(const QPoint &p) const override;
  void rowsInserted(const QModelIndex &parent, int start, int end) override;
  void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
  QRect visualRect(const QModelIndex &idx) const override;
  QRegion visualRegionForSelection(const QItemSelection &selection) const override;
  void contextMenuEvent(QContextMenuEvent *e) override;

 signals:
  void RightClicked(QPoint global_pos, QModelIndex idx);

 private slots:
  void LayoutItems();

 private:
  // Returns the items that are wholly or partially inside the rect.
  QVector<QModelIndex> IntersectingItems(const QRect &rect) const;

  // Returns the index of the item above (d=-1) or below (d=+1) the given item.
  int IndexAboveOrBelow(int index, const int d) const;

  QVector<QRect> visual_rects_;
  int item_indent_;

};

#endif  // GIFICONVIEW_H

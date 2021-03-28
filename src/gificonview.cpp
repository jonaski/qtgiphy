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

#include <QtGlobal>
#include <QListView>
#include <QScrollBar>
#include <QString>
#include <QLocale>
#include <QPainter>
#include <QPalette>
#include <QVector>
#include <QRect>
#include <QPoint>
#include <QSize>
#include <QStyle>
#include <QStyleOption>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>

#include "logging.h"

#include "gificonmodel.h"
#include "gificonview.h"
#include "gificonviewdelegate.h"
#include "gificonitem.h"

GifIconView::GifIconView(QWidget *parent)
    : QListView(parent),
      item_indent_(10) {

  setFlow(LeftToRight);
  setViewMode(IconMode);
  setResizeMode(Adjust);
  setWordWrap(true);
  setDragEnabled(false);
  setSelectionRectVisible(true);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  verticalScrollBar()->setSingleStep(20);
  setItemDelegate(new GifIconViewDelegate(this));

}

void GifIconView::setModel(QAbstractItemModel *model) {

  QListView::setModel(model);

  LayoutItems();

}

void GifIconView::resizeEvent(QResizeEvent *e) {

  LayoutItems();
  QListView::resizeEvent(e);

}

void GifIconView::rowsInserted(const QModelIndex &parent, int start, int end) {

  QListView::rowsInserted(parent, start, end);
  LayoutItems();

  GifIconModel *gificonmodel = qobject_cast<GifIconModel*>(model());
  if (!gificonmodel) return;

  for (int i = start ; i <= end ; ++i) {
    GifIconItemPtr item = gificonmodel->item_at(i);
    QPersistentModelIndex idx = model()->index(i, 0);
    QObject::connect(item->scaled_movie().get(), &QMovie::frameChanged, this, [this, idx]() {
      if (viewport()->rect().intersects(visualRect(idx))) {
        update(idx);
      }
    }, Qt::QueuedConnection);
  }

}

void GifIconView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>&) {

  QListView::dataChanged(topLeft, bottomRight);
  LayoutItems();

}

void GifIconView::LayoutItems() {

  if (!model()) return;

  const int count = model()->rowCount();

  QPoint next_position(0, 0);
  int max_row_height = 0;

  visual_rects_.clear();
  visual_rects_.reserve(count);

  for (int i = 0 ; i < count ; ++i) {
    const QModelIndex idx(model()->index(i, 0));
    const QSize size = rectForIndex(idx).size();

    // Take into account padding and spacing
    QPoint this_position(next_position);
    if (this_position.x() == 0) {
      this_position.setX(this_position.x() + item_indent_);
    }
    else {
      this_position.setX(this_position.x() + spacing());
    }

    // Should this item wrap?
    if (next_position.x() != 0 && this_position.x() + size.width() >= viewport()->width()) {
      next_position.setX(0);
      next_position.setY(next_position.y() + max_row_height);
      this_position = next_position;
      this_position.setX(this_position.x() + item_indent_);

      max_row_height = 0;
    }

    // Set this item's geometry
    visual_rects_.append(QRect(this_position, size));

    // Update next index
    next_position.setX(this_position.x() + size.width());
    max_row_height = qMax(max_row_height, size.height());
  }

  verticalScrollBar()->setRange(0, next_position.y() + max_row_height - viewport()->height());
  update();

}

QRect GifIconView::visualRect(const QModelIndex &idx) const {

  if (idx.row() < 0 || idx.row() >= visual_rects_.count()) return QRect();

  return visual_rects_[idx.row()].translated(-horizontalOffset(), -verticalOffset());

}

QModelIndex GifIconView::indexAt(const QPoint &p) const {

  const QPoint viewport_p = p + QPoint(horizontalOffset(), verticalOffset());

  const int count = visual_rects_.count();
  for (int i = 0 ; i < count ; ++i) {
    if (visual_rects_[i].contains(viewport_p)) {
      return model()->index(i, 0);
    }
  }

  return QModelIndex();

}

void GifIconView::paintEvent(QPaintEvent *e) {

  // This code was adapted from QListView::paintEvent(), changed to use the visualRect() of items.

  QStyleOptionViewItem option;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  initViewItemOption(&option);
#else
  option = viewOptions();
#endif

  if (isWrapping()) {
    option.features = QStyleOptionViewItem::WrapText;
  }

  option.locale = locale();
  option.locale.setNumberOptions(QLocale::OmitGroupSeparator);
  option.widget = this;

  QPainter painter(viewport());

  const QRect viewport_rect(e->rect().translated(horizontalOffset(), verticalOffset()));
  QVector<QModelIndex> toBeRendered = IntersectingItems(viewport_rect);

  const QModelIndex current = currentIndex();
  const QAbstractItemModel *itemModel = model();
  const QItemSelectionModel *selections = selectionModel();
  const bool focus = (hasFocus() || viewport()->hasFocus()) && current.isValid();
  const QStyle::State state = option.state;
  const QAbstractItemView::State viewState = this->state();
  const bool enabled = (state & QStyle::State_Enabled) != 0;

  int maxSize = (flow() == TopToBottom) ? viewport()->size().width() - 2 * spacing() : viewport()->size().height() - 2 * spacing();

  QVector<QModelIndex>::const_iterator end = toBeRendered.constEnd();
  for (QVector<QModelIndex>::const_iterator it = toBeRendered.constBegin(); it != end; ++it) {
    if (!it->isValid()) {
      continue;
    }

    option.rect = visualRect(*it);

    if (flow() == TopToBottom)
      option.rect.setWidth(qMin(maxSize, option.rect.width()));
    else
      option.rect.setHeight(qMin(maxSize, option.rect.height()));

    option.state = state;
    if (selections && selections->isSelected(*it))
      option.state |= QStyle::State_Selected;
    if (enabled) {
      QPalette::ColorGroup cg = QPalette::Active;
      if ((itemModel->flags(*it) & Qt::ItemIsEnabled) == 0) {
        option.state &= ~QStyle::State_Enabled;
        cg = QPalette::Disabled;
      }
      else {
        cg = QPalette::Normal;
      }
      option.palette.setCurrentColorGroup(cg);
    }
    if (focus && current == *it) {
      option.state |= QStyle::State_HasFocus;
      if (viewState == EditingState)
        option.state |= QStyle::State_Editing;
    }

    itemDelegate()->paint(&painter, option, *it);
  }

}

void GifIconView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) {

  QVector<QModelIndex> indexes(IntersectingItems(rect.translated(horizontalOffset(), verticalOffset())));

  QItemSelection selection;
  for (const QModelIndex &idx : indexes) {
    selection << QItemSelectionRange(idx);
  }

  selectionModel()->select(selection, command);

}

QVector<QModelIndex> GifIconView::IntersectingItems(const QRect &rect) const {

  QVector<QModelIndex> ret;

  const int count = visual_rects_.count();
  for (int i = 0; i < count; ++i) {
    if (rect.intersects(visual_rects_[i])) {
      ret.append(model()->index(i, 0));
    }
  }

  return ret;

}

QRegion GifIconView::visualRegionForSelection(const QItemSelection &selection) const {

  QRegion ret;
  for (const QModelIndex &idx : selection.indexes()) {
    ret += visual_rects_[idx.row()];
  }
  return ret;

}

QModelIndex GifIconView::moveCursor(CursorAction action, Qt::KeyboardModifiers) {

  if (model()->rowCount() == 0) {
    return QModelIndex();
  }

  int ret = currentIndex().row();
  if (ret == -1) {
    ret = 0;
  }

  switch (action) {
    case MoveUp:    ret = IndexAboveOrBelow(ret, -1); break;
    case MovePrevious:
    case MoveLeft:  ret --; break;
    case MoveDown:  ret = IndexAboveOrBelow(ret, +1); break;
    case MoveNext:
    case MoveRight: ret ++; break;
    case MovePageUp:
    case MoveHome:  ret = 0; break;
    case MovePageDown:
    case MoveEnd:   ret = model()->rowCount() - 1; break;
  }

  return model()->index(qBound(0, ret, model()->rowCount()), 0);

}

int GifIconView::IndexAboveOrBelow(int index, const int d) const {

  const QRect orig_rect(visual_rects_[index]);

  while (index >= 0 && index < visual_rects_.count()) {
    const QRect rect(visual_rects_[index]);
    const QPoint center(rect.center());

    if ((center.y() <= orig_rect.top() || center.y() >= orig_rect.bottom()) && center.x() >= orig_rect.left() && center.x() <= orig_rect.right()) {
      return index;
    }

    index += d;
  }

  return index;

}

void GifIconView::contextMenuEvent(QContextMenuEvent *e) {

  QModelIndex source_index = indexAt(e->pos());

  emit RightClicked(e->globalPos(), source_index);
  e->accept();

}

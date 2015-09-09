#include <QMouseEvent>
#include <QScrollBar>
#include <QStylePainter>
#include <QFontMetrics>
#include <QPixmapCache>
#include "uiresultlist.h"
//#include "guiconfig.h"

#if defined(QDESIGNER_EXPORT_WIDGETS)
QString	g_text("Navinfo result list test.");
#endif

//#define RESULT_LIST_FLAG 1

UIResultList::UIResultList(QWidget *parent)
: QListWidget(parent)
, _pressedItemWidget(0)
, _timerId(0)
, _speed(0)
, _viewItemCount(0)
, _prevIndex(-1)
, _itemVisible(false)
, _scrollFlag(false)
, _hasCheckBox(false)
, _textTwoFontFlag(false)
, _baseFontColorFlag(false)
, _twoFontColorFlag(false)
{
	//_resPath = GetHMICfg()->cResHmiFolder;
#if defined(QT_PLUGIN)
    _resPath = "../resource/";
#else
    _resPath = ":/";
#endif

	setFrameShape(QFrame::NoFrame);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setAutoScroll(false);
	setUniformItemSizes(true);

	setStyleSheet("background-color: transparent;");

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(listViewClicked(const QModelIndex&)));
	connect(this, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(listItemPressed(QListWidgetItem *)));

	_itemSize = QSize(100, 16);
}

UIResultList::~UIResultList()
{
	if (_timerId)
	{
		killTimer(_timerId);
	}

	cleanAll();
}

void UIResultList::updateList(int index)
{
	int ncount = count() - 5;
	if (ncount < 0)
	{
		ncount = 0;
		listScrollTo(0);
	}
	else 
	{
		if(index < ncount)
		{
			listScrollTo(index);
		}
		else
		{
			listScrollTo(ncount);
		}
	}
}

int UIResultList::getcurpos()
{
	return row(itemAt( 10, spacing()*2));
}

void UIResultList::cleanAll()
{
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem *widgetItem = item(i);
		removeItemWidget(widgetItem);

		delete widgetItem;
	}
	clear();

	for (int i=0; i<_itemWidgetList.count(); ++i)
	{
		delete _itemWidgetList[i];
	}
	_itemWidgetList.clear();
	QPixmapCache::clear();
	update();
}

void UIResultList::pushItem(const QString* text)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	QItemWidget* itemWidget = new QItemWidget(_itemSize);

	item->setSizeHint(_itemSize);

	itemWidget->setFont(font());
	itemWidget->setNormalImage(_itemBackgroundImage);
	itemWidget->setPressedImage(_itemBackgroundImage + 1);

	if (_baseFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_baseFontColor);
	}

	if (_twoFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_twoFontColor);
	}

	itemWidget->setTextRect(_itemTextRect, _itemTextRect + 1);
	itemWidget->setTextOne(text);

	addItem(item);

	_itemWidgetList.push_back(itemWidget);
}

void UIResultList::pushItem(const QString* text1, const QString* text2)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	QItemWidget* itemWidget = new QItemWidget(_itemSize);

	item->setSizeHint(_itemSize);

	itemWidget->setFont(font());
	itemWidget->setNormalImage(_itemBackgroundImage);
	itemWidget->setPressedImage(_itemBackgroundImage + 1);

	if (_baseFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_baseFontColor);
	}

	if (_twoFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_twoFontColor);
	}

	itemWidget->setTextRect(_itemTextRect, _itemTextRect + 1);
	itemWidget->setTextOne(text1);
	if (_textTwoFontFlag)
	{
		itemWidget->setTextTwo(text2, &_textTwoFont);
	}
	else
	{
		itemWidget->setTextTwo(text2);
	}

	addItem(item);

	_itemWidgetList.push_back(itemWidget);
}

void UIResultList::pushItem(const QPixmap icon, const QString* text, bool hasCheckBox)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	QItemWidget* itemWidget = new QItemWidget(_itemSize);

	item->setSizeHint(_itemSize);

	itemWidget->setFont(font());
	itemWidget->setNormalImage(_itemBackgroundImage);
	itemWidget->setPressedImage(_itemBackgroundImage + 1);

	if (_baseFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_baseFontColor);
	}

	if (_twoFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_twoFontColor);
	}

	itemWidget->setIconRect(_itemIconRect, _itemIconRect + 1);
	if (_hasCheckBox && hasCheckBox)
	{
		itemWidget->setHasCheckBox(_hasCheckBox);
		itemWidget->setCheckImage(_checkImage, _checkImage+1);
	}
	else
	{
		itemWidget->setIconOne(icon);
	}

	itemWidget->setTextRect(_itemTextRect, _itemTextRect + 1);
	itemWidget->setTextOne(text);

	addItem(item);

	_itemWidgetList.push_back(itemWidget);
}

void UIResultList::pushItem(const QPixmap icon, const QString* text1, const QString* text2, bool hasCheckBox)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	QItemWidget* itemWidget = new QItemWidget(_itemSize);

	item->setSizeHint(_itemSize);

	itemWidget->setFont(font());
	itemWidget->setNormalImage(_itemBackgroundImage);
	itemWidget->setPressedImage(_itemBackgroundImage + 1);

	if (_baseFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_baseFontColor);
	}

	if (_twoFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_twoFontColor);
	}

	itemWidget->setIconRect(_itemIconRect, _itemIconRect + 1);
	if (_hasCheckBox && hasCheckBox)
	{
		itemWidget->setHasCheckBox(_hasCheckBox);
		itemWidget->setCheckImage(_checkImage, _checkImage+1);
	}
	else
	{
		itemWidget->setIconOne(icon);
	}

	itemWidget->setTextRect(_itemTextRect, _itemTextRect + 1);
	itemWidget->setTextOne(text1);
	if (_textTwoFontFlag)
	{
		itemWidget->setTextTwo(text2, &_textTwoFont);
	}
	else
	{
		itemWidget->setTextTwo(text2);
	}

	addItem(item);

	_itemWidgetList.push_back(itemWidget);
}

void UIResultList::pushItem(const QPixmap icon1, const QPixmap icon2, const QString* text, bool hasCheckBox)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	QItemWidget* itemWidget = new QItemWidget(_itemSize);

	item->setSizeHint(_itemSize);

	itemWidget->setFont(font());
	itemWidget->setNormalImage(_itemBackgroundImage);
	itemWidget->setPressedImage(_itemBackgroundImage + 1);

	if (_baseFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_baseFontColor);
	}

	if (_twoFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_twoFontColor);
	}

	itemWidget->setIconRect(_itemIconRect, _itemIconRect + 1);
	if (_hasCheckBox && hasCheckBox)
	{
		itemWidget->setHasCheckBox(_hasCheckBox);
		itemWidget->setCheckImage(_checkImage, _checkImage+1);
	}
	else
	{
		itemWidget->setIconOne(icon1);
	}

	itemWidget->setIconTwo(icon2);

	itemWidget->setTextRect(_itemTextRect, _itemTextRect + 1);
	itemWidget->setTextOne(text);

	addItem(item);

	_itemWidgetList.push_back(itemWidget);
}

void UIResultList::pushItem(const QPixmap icon1, const QPixmap icon2, const QString* text1, const QString* text2, bool hasCheckBox)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	QItemWidget* itemWidget = new QItemWidget(_itemSize);

	item->setSizeHint(_itemSize);

	itemWidget->setFont(font());
	itemWidget->setNormalImage(_itemBackgroundImage);
	itemWidget->setPressedImage(_itemBackgroundImage + 1);

	if (_baseFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_baseFontColor);
	}

	if (_twoFontColorFlag)
	{
		itemWidget->setBaseFontColor(&_twoFontColor);
	}

	itemWidget->setIconRect(_itemIconRect, _itemIconRect + 1);
	if (_hasCheckBox && hasCheckBox)
	{
		itemWidget->setHasCheckBox(_hasCheckBox);
		itemWidget->setCheckImage(_checkImage, _checkImage+1);
	}
	else
	{
		itemWidget->setIconOne(icon1);
	}
	itemWidget->setIconTwo(icon2);

	itemWidget->setTextRect(_itemTextRect, _itemTextRect + 1);
	itemWidget->setTextOne(text1);
	if (_textTwoFontFlag)
	{
		itemWidget->setTextTwo(text2, &_textTwoFont);
	}
	else
	{
		itemWidget->setTextTwo(text2);
	}

	addItem(item);

	_itemWidgetList.push_back(itemWidget);
}

void UIResultList::setResPath(const QString& resPath)
{
	_resPath = resPath;

	update();
}

void UIResultList::setItemBackgroundName(const QString& imageName)
{
	_itemBackgroundName[0] = imageName;
	_itemBackgroundImage[0].load(_resPath + imageName, "PNG");

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setNormalImage(_itemBackgroundImage);
	}
#endif
}

void UIResultList::setItemPressedImageName(const QString& imageName)
{
	_itemBackgroundName[1] = imageName;
	_itemBackgroundImage[1].load(_resPath + imageName, "PNG");

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setPressedImage(_itemBackgroundImage + 1);
	}
#endif
}

void UIResultList::setHasCheckBox(bool flag)
{
	_hasCheckBox = flag;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setHasCheckBox(flag);
	}
	update();
#endif
}

bool UIResultList::itemIsChecked(int index)
{
#if defined(QDESIGNER_EXPORT_WIDGETS)
	if (index < count())
	{
		QListWidgetItem* widgetItem = item(index);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (widget)
		{
			return widget->isChecked();
		}
	}
#endif
	return false;
}

void UIResultList::setItemChecked(int index, bool flag)
{
	QListWidgetItem* widgetItem = item(index);
	QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

	if (!widget)
	{
		return;
	}
	widget->setCheckedFlag(flag);
}

void UIResultList::setUncheckedImageName(const QString& imageName)
{
	_checkImageName[0] = imageName;
	_checkImage[0].load(_resPath + imageName, "PNG");

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setCheckImage(_checkImage, _checkImage+1);
	}
#endif
}

void UIResultList::setCheckedImageName(const QString& imageName)
{
	_checkImageName[1] = imageName;
	_checkImage[1].load(_resPath + imageName, "PNG");

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setCheckImage(_checkImage, _checkImage+1);
	}
#endif
}

void UIResultList::setItemIconOneName(const QString& imageName)
{
	_itemIconName[0] = imageName;
	_itemIcon[0].load(_resPath + imageName, "PNG");

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setIconOne(_itemIcon[0]);
	}
#endif
}

void UIResultList::setItemIconTwoName(const QString& imageName)
{
	_itemIconName[1] = imageName;
	_itemIcon[1].load(_resPath + imageName, "PNG");

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setIconTwo(_itemIcon[1]);
	}
#endif
}

void UIResultList::setItemIconOneRect(const QRect& rect)
{
	_itemIconRect[0] = rect;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setIconRect(_itemIconRect, _itemIconRect + 1);
	}
#endif
}

void UIResultList::setItemIconTwoRect(const QRect& rect)
{
	_itemIconRect[1] = rect;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setIconRect(_itemIconRect, _itemIconRect + 1);
	}
#endif
}

void UIResultList::setItemTextOneRect(const QRect& rect)
{
	_itemTextRect[0] = rect;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setTextRect(_itemTextRect, _itemTextRect + 1);
	}
#endif
}

void UIResultList::setItemTextTwoRect(const QRect& rect)
{
	_itemTextRect[1] = rect;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setTextRect(_itemTextRect, _itemTextRect + 1);
	}
#endif
}

void UIResultList::setTextTwoFont(const QFont& font)
{
	_textTwoFont = font;
	_textTwoFontFlag = true;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setTextTwoFont(&_textTwoFont);
	}
#endif
}

void UIResultList::setBaseFontColor(const QColor& c)
{
	_baseFontColor = c;
	_baseFontColorFlag = true;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setBaseFontColor(&_baseFontColor);
	}
#endif
}

void UIResultList::setTwoFontColor(const QColor& c)
{
	_twoFontColor = c;
	_textTwoFontFlag = true;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	for (int i=0; i<count(); ++i)
	{
		QListWidgetItem* widgetItem = item(i);
		QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

		if (!widget)
		{
			continue;
		}
		widget->setTwoFontColor(&_twoFontColor);
	}
#endif
}


void UIResultList::setItemSize(const QSize& size)
{
	if (size.width()>50 && size.height()>10)
	{
		_itemSize = size;

		_viewItemCount = this->height() / size.height() + 1;
		update();

#if defined(QDESIGNER_EXPORT_WIDGETS)
		for (int i=0; i<count(); ++i)
		{
			QListWidgetItem* widgetItem = item(i);
			QItemWidget* widget = (QItemWidget*)itemWidget(widgetItem);

			if (!widget)
			{
				continue;
			}
			widgetItem->setSizeHint(_itemSize);
			widget->setSize(_itemSize);
		}
#endif
	}
}

void UIResultList::setItemVisible(bool flag)
{
	_itemVisible = flag;	

#if defined(QDESIGNER_EXPORT_WIDGETS)
	if (_itemVisible)
	{
		for (int i=0; i<10; ++i)
		{
			pushItem(_itemIcon[0], _itemIcon[1], &g_text, &g_text);
		}

		updateList();
	}
	else
	{
		cleanAll();
	}
#endif
}

void UIResultList::listViewClicked(const QModelIndex &index)
{
	emit listClicked(index.row());
}

void UIResultList::listItemPressed(QListWidgetItem *item)
{
	_pressedItemWidget = (QItemWidget*)itemWidget(item);
	if (_pressedItemWidget)
	{
		_pressedItemWidget->pressed();
	}
}

void UIResultList::listScrollTo(int index)
{
	if (_scrollFlag)
	{
		return;
	}
	if (index > 0)
	{
		listUpdate(index - 1);
	}
	else
	{
		listUpdate(0);
	}
	scrollTo(model()->index(index, 0), QAbstractItemView::PositionAtTop);
}


bool UIResultList::istop()
{
	int index = row(itemAt( 10, spacing()*2));
	return (index < 1);
}

bool UIResultList::isbottom()
{
	int index = row(itemAt( 10, spacing()*2));
	return (index + 5 >= count());
}


void UIResultList::listScrollDown(int step)
{
	int index = row(itemAt( 10, spacing()*2));

	listScrollTo(index + step);
}

void UIResultList::listScrollUp(int step)
{
	int index = row(itemAt( 10, spacing()*2));

	if (index < step)
	{
		index = 0;
	}
	else
	{
		index -= step;
	}

	listScrollTo(index);
}

void UIResultList::paintEvent(QPaintEvent *event)
{
	// 	if (count() < _viewItemCount)
	// 	{
	// 		listUpdate(0);
	// 	}
	// 
	// 	QListWidget::paintEvent(event);
}

void UIResultList::mousePressEvent(QMouseEvent* e)
{
#ifdef RESULT_LIST_FLAG
	QString msg = QString("mousePressEvent");
	if (_timerId)
	{
		killTimer(_timerId);
		_timerId = 0;
		_speed = 0;

		e->ignore();
		_scrollFlag = false;
	}
	else
	{
		_scrollFlag = true;
		QListView::mousePressEvent(e);
	}
	_timeStamp = QTime::currentTime();
	_lastTime = QTime::currentTime();

	_pos = e->pos().y();
	_startPos = e->pos().y();
#else
	QListView::mousePressEvent(e);
#endif
}

void UIResultList::listUpdate(int index)
{
	int index_top = index;

	if (count() < _viewItemCount)
	{
		for (int i=0; i<count(); ++i)
		{
			QListWidgetItem *widgetItem = item(i);
			QWidget* widget = itemWidget(widgetItem);

			if (0 == widget)
			{
				setItemWidget(widgetItem, _itemWidgetList[i]);
			}
		}
		return;
	}

	for (int i=0; i<10; ++i)
	{
		if (-1 != index_top)
		{
			break;
		}

		index_top = row(itemAt( 10, i));
	}

	for (int i=0; i<_viewItemCount; ++i)
	{
		int itemIndex = index_top + i;

		if (itemIndex >= count() || itemIndex < 0)
		{
			continue;;
		}

		QListWidgetItem *widgetItem = item(itemIndex);
		QWidget* widget = itemWidget(widgetItem);

		if (0 == widget)
		{
			if (itemIndex < _itemWidgetList.count())
			{
				setItemWidget(widgetItem, _itemWidgetList[itemIndex]);
			}
		}
	}

	if (_prevIndex != index_top)
	{
		_prevIndex = index_top;
		emit listScroll(index_top);
	}
}

void UIResultList::wheelEvent(QWheelEvent * e)
{
	QListView::wheelEvent(e);
	listUpdate();
}

void UIResultList::mouseMoveEvent(QMouseEvent* e)
{
	QListView::mouseMoveEvent(e);

#ifdef RESULT_LIST_FLAG
	QScrollBar* bar = verticalScrollBar();

	int y;
	if (e->pos().y()>height())
	{
		y = height();
	}
	else if (e->pos().y()<0)
	{
		y = 0;
	}
	else
	{
		y = e->pos().y();
	}
	bar->setValue(bar->value() + (_pos - y));

	//QString msg = QString("Mouse Move, dy = %1, time = %2").arg(_pos - y).arg(_timeStamp.elapsed());
	//emit postMessage(msg);

	if (_pos != y)
	{
		_lastTime = QTime::currentTime();
	}

	_pos = y;
#endif
	//listUpdate();
}

void UIResultList::mouseReleaseEvent(QMouseEvent* e)
{
	if (_pressedItemWidget)
	{
		_pressedItemWidget->released();
		_pressedItemWidget = 0;
	}

#ifdef RESULT_LIST_FLAG
	int y;

	if (e->pos().y()>height())
	{
		y = height();
	}
	else if (e->pos().y()<0)
	{
		y = 0;
	}
	else
	{
		y = e->pos().y();
	}

	if (_lastTime.elapsed() < 5)
	{
		int elapsed = _timeStamp.elapsed();

		if (0==elapsed)
		{
			elapsed = 1;
		}
		_speed = (_startPos - y)*100/elapsed;
		_timerId = startTimer(100);
		e->ignore();
	}
	else if (abs(_startPos-y)>1)
	{
		e->ignore();
		_scrollFlag = false;
	}
	else
	{
		QListView::mouseReleaseEvent(e);
		_scrollFlag = false;
	}
#else
	QListView::mouseReleaseEvent(e);
#endif
	listUpdate();
}

void UIResultList::timerEvent(QTimerEvent* e)
{
	QListView::timerEvent(e);

#ifdef RESULT_LIST_FLAG
	if (_timerId == e->timerId())
	{
		QScrollBar* bar = verticalScrollBar();
		bar->setValue(bar->value() + _speed);
		_speed = _speed*3/4;
		listUpdate();

		if (0==_speed)
		{
			killTimer(_timerId);
			_timerId = 0;
			_scrollFlag = false;
		}

		if (bar->value() == 0)
		{
			killTimer(_timerId);
			_timerId = 0;
			_scrollFlag = false;
		}

		if (bar->value() == (bar->maximum()-bar->pageStep()))
		{
			killTimer(_timerId);
			_timerId = 0;
			_scrollFlag = false;
		}
	}
#endif
}


///////////////////////////////////////////////////////////////////////////////
QItemWidget::QItemWidget(QWidget *parent)
: QWidget(parent)
, _background(0)
, _normalImage(0)
, _pressedImage(0)
, _textTwoFont(0)
, _hasCheckBox(false)
, _isChecked(false)
{
	_iconRect[0] = 0;
	_iconRect[1] = 0;
	_textRect[0] = 0;
	_textRect[1] = 0;
	_icon[0] = 0;
	_icon[1] = 0;
	_checkedImage[0] = 0;
	_checkedImage[1] = 0;
	//_text[0] = 0;
	//_text[1] = 0;
}

QItemWidget::QItemWidget(const QSize& size, QWidget *parent)
: QWidget(parent)
, _background(0)
, _normalImage(0)
, _pressedImage(0)
, _textTwoFont(0)
, _hasCheckBox(false)
, _isChecked(false)
{
	_iconRect[0] = 0;
	_iconRect[1] = 0;
	_textRect[0] = 0;
	_textRect[1] = 0;
	_icon[0] = 0;
	_icon[1] = 0;
	_checkedImage[0] = 0;
	_checkedImage[1] = 0;
	_fontColor[0] = 0;
	_fontColor[1] = 0;
	//_text[0] = 0;
	//_text[1] = 0;

	setFixedSize(size);
}

QItemWidget::~QItemWidget()
{
	if(_icon[0]){delete _icon[0];_icon[0] = NULL;}
	if(_icon[1]){delete _icon[1];_icon[1] = NULL;}
}

void QItemWidget::paintEvent(QPaintEvent *event)
{
	if (!isVisible())
	{
		return;
	}

	QStylePainter painter(this);

	if (_background && !_background->isNull())
	{
		painter.drawPixmap(0, 0, *_background);
	}

	for (int i=0; i<2; ++i)
	{

		if (_iconRect[i] && _iconRect[i]->isValid() && _icon[i] && !_icon[i]->isNull())
		{
			painter.drawPixmap(*(_iconRect[i]), *(_icon[i]));
			//painter.drawItemPixmap(*(_iconRect[i]), Qt::AlignHCenter|Qt::AlignVCenter, *(_icon[i]));
#if defined(QDESIGNER_EXPORT_WIDGETS)
			painter.setPen(Qt::DotLine);
			painter.drawRect(*(_iconRect[i]));
#endif			
		}
	}

	for (int i=0; i<2; ++i)
	{
		if (_textRect[i] &&_textRect[i]->isValid() && !_text[i].isNull())
		{
			if (1==i && _textTwoFont)
			{
				painter.setFont(*_textTwoFont);
			}

			if (_fontColor[i])
			{
				painter.setPen(*(_fontColor[i]));
			}

			QFontMetrics fontMetrics = painter.fontMetrics();
			QString text = fontMetrics.elidedText(_text[i], Qt::ElideRight, _textRect[i]->width());

			painter.drawText(*(_textRect[i]), Qt::AlignVCenter, text);
#if defined(QDESIGNER_EXPORT_WIDGETS)
			painter.setPen(Qt::DotLine);
			painter.drawRect(*(_textRect[i]));
#endif			
		}
	}
}

void QItemWidget::setSize(const QSize& size)
{
	setFixedSize(size);
}

void QItemWidget::setNormalImage(const QPixmap* image)
{ 
	_normalImage = image;
	_background = _normalImage;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setPressedImage(const QPixmap* image)
{ 
	_pressedImage = image;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setHasCheckBox(bool flag)
{
	_hasCheckBox = flag;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setCheckedFlag(bool flag)
{
	if (_hasCheckBox)
	{
		_isChecked = flag;

		if (_isChecked)
		{
			_icon[0] = _checkedImage[1];
		}
		else
		{
			_icon[0] = _checkedImage[0];
		}
	}
}

void QItemWidget::setCheckImage(const QPixmap* uncheckedImage, const QPixmap* checkedImage)
{
	_checkedImage[0] = uncheckedImage;
	_checkedImage[1] = checkedImage;

	if (_hasCheckBox)
	{
		if (_isChecked)
		{
			_icon[0] = _checkedImage[1];
		}
		else
		{
			_icon[0] = _checkedImage[0];
		}
	}

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setIconRect(const QRect* rect1, const QRect* rect2)
{
	_iconRect[0] = rect1;
	_iconRect[1] = rect2;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setTextRect(const QRect* rect1, const QRect* rect2)
{
	_textRect[0] = rect1;
	_textRect[1] = rect2;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setIconOne(const QPixmap image)
{
	//	QPixmapCache::clear();
	_icon[0] = new QPixmap(image);

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setIconTwo(const QPixmap image)
{
	//	QPixmapCache::clear();
	_icon[1] = new QPixmap(image);

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setTextOne(const QString* text)
{
	_text[0] = *text;
#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setTextTwo(const QString* text, const QFont* font)
{
	_text[1] = *text;
	_textTwoFont = font;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setBaseFontColor(const QColor* c)
{
	_fontColor[0] = c;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setTwoFontColor(const QColor* c)
{
	_fontColor[1] = c;

#if defined(QDESIGNER_EXPORT_WIDGETS)
	update();
#endif			
}

void QItemWidget::setTextTwoFont(const QFont* font)
{
	_textTwoFont = font;

	if (font)
	{
		update();
	}
}

void QItemWidget::pressed()
{
	if (_background!=_pressedImage)
	{
		_background = _pressedImage;
		update();
	}
}

void QItemWidget::released()
{
	if (_background!=_normalImage)
	{
		_background = _normalImage;
		update();
	}
}

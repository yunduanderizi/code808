#ifndef UIRESULTLIST_H
#define UIRESULTLIST_H

#include <QtGui/QWidget>
#include <QListView>
#include <QListWidget>
#include <QStandardItemModel>
#include <QTime>

class QItemWidget;

#if defined(QDESIGNER_EXPORT_WIDGETS)
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT UIResultList : public QListWidget
#else
class UIResultList : public QListWidget
#endif
{
	Q_OBJECT

		//Q_PROPERTY(QString resPath READ resPath WRITE setResPath)

		Q_PROPERTY(bool itemVisible READ itemVisible WRITE setItemVisible)
		Q_PROPERTY(QSize itemSize READ itemSize WRITE setItemSize)
		Q_PROPERTY(QString itemBackgroundName READ itemBackgroundName WRITE setItemBackgroundName)
		Q_PROPERTY(QString itemPressedImageName READ itemPressedImageName WRITE setItemPressedImageName)

		Q_PROPERTY(bool hasCheckBox READ hasCheckBox WRITE setHasCheckBox)
		Q_PROPERTY(QString uncheckedImageName READ uncheckedImageName WRITE setUncheckedImageName)
		Q_PROPERTY(QString checkedImageName READ checkedImageName WRITE setCheckedImageName)

		Q_PROPERTY(QString itemIconOneName READ itemIconOneName WRITE setItemIconOneName)
		Q_PROPERTY(QString itemIconTwoName READ itemIconTwoName WRITE setItemIconTwoName)

		Q_PROPERTY(QRect itemIconOneRect READ itemIconOneRect WRITE setItemIconOneRect)
		Q_PROPERTY(QRect itemIconTwoRect READ itemIconTwoRect WRITE setItemIconTwoRect)

		Q_PROPERTY(QRect itemTextOneRect READ itemTextOneRect WRITE setItemTextOneRect)
		Q_PROPERTY(QRect itemTextTwoRect READ itemTextTwoRect WRITE setItemTextTwoRect)

		Q_PROPERTY(QFont textTwoFont READ textTwoFont WRITE setTextTwoFont)

		Q_PROPERTY(QColor baseFontColor READ baseFontColor WRITE setBaseFontColor)
		Q_PROPERTY(QColor twoFontColor READ twoFontColor WRITE setTwoFontColor)

public:
	UIResultList(QWidget *parent = 0);
	virtual ~UIResultList();

	void setResPath(const QString& resPath);
	const QString& resPath() { return _resPath; }

	void setItemBackgroundName(const QString& imageName);
	const QString& itemBackgroundName() { return _itemBackgroundName[0]; }

	void setItemPressedImageName(const QString& imageName);
	const QString& itemPressedImageName() { return _itemBackgroundName[1]; }

	void setItemSize(const QSize& size);
	const QSize& itemSize() { return _itemSize; }

	void setHasCheckBox(bool flag);
	bool hasCheckBox() { return _hasCheckBox; }

	bool itemIsChecked(int index);

	void setItemChecked(int index, bool flag);

	void setUncheckedImageName(const QString& imageName);
	const QString& uncheckedImageName() { return _checkImageName[0]; }

	void setCheckedImageName(const QString& imageName);
	const QString& checkedImageName() { return _checkImageName[1]; }

	void setItemIconOneName(const QString& imageName);
	const QString& itemIconOneName() { return _itemIconName[0]; }

	void setItemIconTwoName(const QString& imageName);
	const QString& itemIconTwoName() { return _itemIconName[1]; }

	void setItemIconOneRect(const QRect& rect);
	const QRect& itemIconOneRect() { return _itemIconRect[0]; }

	void setItemIconTwoRect(const QRect& rect);
	const QRect& itemIconTwoRect() { return _itemIconRect[1]; }

	void setItemTextOneRect(const QRect& rect);
	const QRect& itemTextOneRect() { return _itemTextRect[0]; }

	void setItemTextTwoRect(const QRect& rect);
	const QRect& itemTextTwoRect() { return _itemTextRect[1]; }

	void setTextTwoFont(const QFont& font);
	const QFont& textTwoFont() { return _textTwoFont; }

	void setBaseFontColor(const QColor& c);
	const QColor& baseFontColor() const { return _baseFontColor; };

	void setTwoFontColor(const QColor& c);
	const QColor& twoFontColor() const { return _twoFontColor; };

	void setItemVisible(bool flag);
	bool itemVisible() { return _itemVisible; }

	void updateList(int index = 0);
	int getcurpos();
	void cleanAll();
	void pushItem(const QString* text);
	void pushItem(const QString* text1, const QString* text2);
	void pushItem(const QPixmap icon, const QString* text, bool hasCheckBox = true);
	void pushItem(const QPixmap icon, const QString* text1, const QString* text2, bool hasCheckBox = true);
	void pushItem(const QPixmap icon1, const QPixmap icon2, const QString* text, bool hasCheckBox = true);
	void pushItem(const QPixmap icon1, const QPixmap icon2, const QString* text1, const QString* text2, bool hasCheckBox = true);

	bool istop();
	bool isbottom();
protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent * e);
	void timerEvent(QTimerEvent* e);

signals:
	void postMessage(const QString& msg);
	void listClicked(int id);
	void listScroll(int index);

	public slots:
		void listScrollTo(int index);
		void listScrollDown(int step = 1);
		void listScrollUp(int step = 1);
		void listUpdate(int index = -1);


		private slots:
			void listViewClicked(const QModelIndex &index);
			void listItemPressed(QListWidgetItem *item);

private:
	QItemWidget*	_pressedItemWidget;
	QString	_resPath;

	QSize	_itemSize;
	QString	_itemBackgroundName[2];
	QPixmap	_itemBackgroundImage[2];

	bool	_hasCheckBox;
	QString	_checkImageName[2];
	QPixmap	_checkImage[2];

	QString	_itemIconName[2];
	QPixmap	_itemIcon[2];

	QRect	_itemIconRect[2];
	QRect	_itemTextRect[2];

	QFont	_textTwoFont;
	bool	_textTwoFontFlag;

	QColor	_baseFontColor;
	bool	_baseFontColorFlag;

	QColor	_twoFontColor;
	bool	_twoFontColorFlag;

	int		_timerId;
	QTime	_timeStamp;
	QTime	_lastTime;

	int		_speed;
	int		_pos;
	int		_startPos;

	bool	_itemVisible;

	bool	_scrollFlag;
	int		_prevIndex;

	int		_viewItemCount;
	QList<QItemWidget*>	_itemWidgetList;
};


class QItemWidget : public QWidget
{
	Q_OBJECT

public:
	QItemWidget(QWidget *parent = 0);
	QItemWidget(const QSize& size, QWidget *parent = 0);
	virtual ~QItemWidget();

	void setSize(const QSize& size);
	void setNormalImage(const QPixmap* image);
	void setPressedImage(const QPixmap* image);

	void setHasCheckBox(bool flag);
	void setCheckImage(const QPixmap* uncheckedImage, const QPixmap* checkedImage);

	inline bool isChecked() { return _isChecked; }

	void setCheckedFlag(bool flag);

	void setIconRect(const QRect* rect1, const QRect* rect2);
	void setTextRect(const QRect* rect1, const QRect* rect2);

	void setIconOne(const QPixmap image);
	void setIconTwo(const QPixmap image);

	void setTextOne(const QString* text);
	void setTextTwo(const QString* text, const QFont* font = 0);

	void setBaseFontColor(const QColor* c);
	void setTwoFontColor(const QColor* c);

	void setTextTwoFont(const QFont* font);

	void pressed();
	void released();

protected:
	void paintEvent(QPaintEvent *event);

private:
	QSize			_size;
	const QPixmap*	_background;
	const QPixmap*	_normalImage;	
	const QPixmap*	_pressedImage;	

	const QRect*	_iconRect[2];
	const QRect*	_textRect[2];

	const QPixmap*	_checkedImage[2];

	const QPixmap*	_icon[2];
	QString			_text[2];

	const QFont*	_textTwoFont;
	const QColor*	_fontColor[2];

	bool			_hasCheckBox;
	bool			_isChecked;
};

#endif // UIRESULTLIST_H

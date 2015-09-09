#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <QWidget>
#include <QPixmap>
#include <QBitmap>
#include <QMouseEvent>
#include <QTimer>
#include <QPainter>



typedef enum __BUTTON_STATE__
{
	BUTTON_NORMAL,//正常情况
	BUTTON_PRESS,//鼠标左键被按�?	
	BUTTON_RELEASE,//鼠标按键被释�?	
	BUTTON_MOVE,//鼠标移动
	BUTTON_PRESSMOVE,//鼠标按下并移�?	
	BUTTON_DISABLE,//按钮无效
}BUTTON_STATE, *PBUTTON_STATE;


#ifdef QT_PLUGIN
#include <QtDesigner/QDesignerExportWidget>
class QDESIGNER_WIDGET_EXPORT UIButton : public QWidget
#else
class UIButton : public QWidget
#endif
{
	Q_OBJECT
public:
	UIButton(QWidget *parent);

	~UIButton();

	Q_PROPERTY(bool visible READ isvisibale WRITE setvisible)
		Q_PROPERTY(QString text READ Text WRITE setText)
		Q_PROPERTY(QString normalbackimage  READ NormalBk WRITE setNormalBk)
		Q_PROPERTY(QString pressbackimage   READ PressBk WRITE setPressBk)
		Q_PROPERTY(QString disablebackimage  READ DisableBk WRITE setDisableBk)
		Q_PROPERTY(QString checkedimage  READ CheckedBk WRITE setCheckedBk)

		Q_PROPERTY(QString normaliconimage  READ NormalIcon WRITE setNormalIcon)
		Q_PROPERTY(QString pressiconimage  READ PressIcon WRITE setPressIcon)

		Q_PROPERTY(QString disableiconimage  READ DisableIcon WRITE setDisableIcon)

		//Q_PROPERTY(QString iconname READ IconName WRITE setIconName)
		Q_PROPERTY(QRect iconrect READ IconRect WRITE setIconRect)
		Q_PROPERTY(QRect textrect READ textRect WRITE setTextRect)
		Q_PROPERTY(QColor color READ color WRITE setColor)
		Q_PROPERTY(QColor discolor READ disablecolor WRITE setDisableColor)
		Q_PROPERTY(Qt::Alignment textalign READ textAlign WRITE settextAlign)

public:
	void setvisible(const bool bvisibale);
	bool isvisibale() const;

	void setResPath(const QString& path);
	QString ResPath() const;

	void setText(QString name);
	QString Text() const;

	void setNormalBk(const QString &imgName);
	QString NormalBk() const;

	void setPressBk(const QString &imgName);
	QString PressBk() const;

	void setDisableBk(const QString &imgName);
	QString DisableBk() const;

	void setCheckedBk(const QString &imgName);
	QString CheckedBk() const;

	void setNormalIcon(const QString &imgName);
	QString NormalIcon() const;
	void setPressIcon(const QString &imgName);
	QString PressIcon() const;
	void setDisableIcon(const QString &imgName);
	QString DisableIcon() const;

	//设置图标区域
	void setIconRect(const QRect &rect);
	QRect IconRect() const;
	//设置文本区域
	void setTextRect(const QRect &rect);
	QRect textRect() const;


	//设置文本排布
	void settextAlign(const Qt::Alignment &nalign){m_textalign = nalign;update();}
	Qt::Alignment textAlign() const{return m_textalign;}

	//设置文字颜色
	void setColor(const QColor &color);
	QColor color() const;

	//设置不可用时的颜�?	
	void setDisableColor(const QColor &color);
	QColor disablecolor() const;
private:
	bool m_bcheck;
	bool m_bfocus;
	bool m_bvisibale;
	//资源文件路径
	QString m_imgPath;

	QString m_text;
	QString m_sBgnormal;
	QString m_sBgpress;
	QString m_sBgdisable;
	QString m_sBgcheck;


	QString m_sIconnormal;
	QString m_sIconpress;

	QString m_sIcondisable;

	Qt::Alignment m_textalign;

	//响应长按事件的定时器
	QTimer* m_timer;
	QPixmap m_normalIconimg;
	QPixmap m_pressIconimg;
	QPixmap m_disableIconimg;

	QPixmap m_normalBkimg;
	QPixmap m_pressBkimg;
	QPixmap m_disableBkimg;
	QPixmap m_checkBkimg;

	BUTTON_STATE m_buttonState;



	//图标区域
	QRect m_iconRect;
	//图标区域
	QRect m_textRect;
	//文字颜色
	QColor m_color;
	//不可用时颜色
	QColor m_discolor;


	//长按信号处理
	private slots:
		void LongPress();

public:
	void setUIButtonSize(const QRect &rc);
	void setUIButtonSize(int x, int y, int width, int height);
	void enabled(bool flg);
	void setChecked(bool flg);
	bool isChecked();
	void setFocused(bool flg);
	bool isFocused();

	void Activate(){emit(clicked());};
	void adaptIconSize(bool flg=true);

signals:
	void clicked();
	void LongPressed();
	private slots:
		void uncheck();
		void cleartext(){setText("");};

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
};

#endif // UIBUTTON_H

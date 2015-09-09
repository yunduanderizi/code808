#ifndef UIPICTURE_H
#define UIPICTURE_H

// #include <QtGui/QWidget>
// 
// class UIPicture : public QWidget
// {
// 	Q_OBJECT
// 
// public:
// 	UIPicture(QWidget *parent = 0);
// 	~UIPicture();
// 
// };

#include <QWidget>


#ifdef QT_PLUGIN
#include <QtDesigner/QDesignerExportWidget>
class QDESIGNER_WIDGET_EXPORT UIPicture : public QWidget
#else
class UIPicture : public QWidget
#endif
{
	Q_OBJECT

		Q_PROPERTY(bool visible READ isvisibale WRITE setvisible)

		//Q_PROPERTY(bool parentvisible READ isparentvisibale WRITE setparentvisible)

		Q_PROPERTY(QString text READ Text WRITE setText)
		Q_PROPERTY(QString imgname  READ ImgName WRITE setImgName)
		Q_PROPERTY(QString iconname READ IconName WRITE setIconName)
		Q_PROPERTY(QString disiconname READ disIconName WRITE setdisIconName)

		Q_PROPERTY(QRect iconrect READ IconRect WRITE setIconRect)
		Q_PROPERTY(QRect textrect READ textRect WRITE setTextRect)
		Q_PROPERTY(QColor color READ color WRITE setColor)

		Q_PROPERTY(Qt::Alignment textalign READ textAlign WRITE settextAlign)
		Q_PROPERTY(bool wordwrap READ textWordWrap WRITE setWordWrap)
private:
	QPixmap m_bkimg;
	QPixmap m_iconimg;
	QPixmap m_disiconimg;

public:
	QRegion getPicRegion();
	void setvisible(const bool bvisibale);
	bool isvisibale() const;

	//设置资源文件路径
	void setResPath(const QString& path);
	QString ResPath() const;
	//设置文本
	void setText(const QString& name);
	QString Text() const;

	//设置背景
	void setImgName(const QString& name);
	QString ImgName() const;
	//设置图标
	void setIconName(const QString& name);
	QString IconName() const;
	void ReloadIcon(const QString& name);

	void setIcon(const uchar *data, int size, const char *format = 0);

	void setIcon(const QImage m_icon);

	//设置不可用图�?	
	void setdisIconName(const QString& name);
	QString disIconName() const;
	//设置图标区域
	void setIconRect(const QRect &rect);
	QRect IconRect() const;
	//设置文本区域
	void setTextRect(const QRect &rect);
	QRect textRect() const;

	//设置文本排布
	void settextAlign(const Qt::Alignment &nalign){m_textalign = nalign;update();}
	Qt::Alignment textAlign() const{return m_textalign;}

	//设置文本排布
	void setWordWrap(const bool &nflag){m_textflag = nflag;update();}
	bool textWordWrap() const{return m_textflag;}

	//设置文字颜色
	void setColor(const QColor &color);
	QColor color() const;

	void setRotate(int nrotate){m_rotate = nrotate;update();};
	int getRotate(){return m_rotate;};
	UIPicture(QWidget *parent = 0);
	~UIPicture();
private:
	bool m_bvisibale;
	//bool m_bparentvisibale;
	//资源文件路径
	QString m_imgPath;
	//文本
	QString m_text;
	//背景
	QString m_imgName;
	//图标
	QString m_iconName;
	//图标
	QString m_disiconName;
	//图标区域
	QRect m_iconRect;
	//图标区域
	QRect m_textRect;
	//文字颜色
	QColor m_color;

	Qt::Alignment m_textalign;

	bool m_textflag;

	int m_rotate;
protected:
	void paintEvent(QPaintEvent *);
};


#endif // UIPICTURE_H

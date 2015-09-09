// #include "uipicture.h"
// 
// UIPicture::UIPicture(QWidget *parent)
// 	: QWidget(parent)
// {
// 
// }
// 
// UIPicture::~UIPicture()
// {
// 
// }

#include "uipicture.h"
#include <QPainter>
//#include "guiconfig.h"
#include <QLabel>
#include <QPixmapCache>
#include <QBitmap>
UIPicture::UIPicture(QWidget *parent)
: QWidget(parent)
{
#ifndef QTPLUGIN
	m_textflag = false;
	m_rotate = 0;
#endif
	//m_imgPath = GetHMICfg()->cResHmiFolder;
#if defined(QT_PLUGIN)
    m_imgPath = "../resource/";
#else
    m_imgPath = ":/";
#endif

}

UIPicture::~UIPicture()
{

}

QRegion UIPicture::getPicRegion()
{
	QBitmap bm(m_bkimg.mask());
	QRegion region(bm);
	return region;
}

void UIPicture::setvisible(const bool bvisibale)
{
	m_bvisibale = bvisibale;
	setVisible(m_bvisibale);
	update();
}

bool UIPicture::isvisibale() const
{
	return m_bvisibale;
}

void UIPicture::setResPath(const QString& path)
{
	m_imgPath = path;
}

QString UIPicture::ResPath() const
{
	return m_imgPath;
}

void UIPicture::setText(const QString& name)
{
	m_text = name;
	update();
}

QString UIPicture::Text() const
{
	return m_text;
}

void UIPicture::setImgName(const QString& name)
{
	m_imgName = name;
	m_bkimg.load(m_imgPath + m_imgName);
	update();
}

QString UIPicture::ImgName() const
{
	return m_imgName;
}

void UIPicture::setIconName(const QString& name)
{
	m_iconName = name;
	m_iconimg.load(m_imgPath + m_iconName);

	update();
}

void UIPicture::ReloadIcon(const QString& name)
{
	m_iconName = name;
	QPixmap p;
	p.load(m_imgPath + m_iconName);
	//QPixmapCache::remove(m_imgPath + m_iconName);
	m_iconimg = p;
	update();
}


void UIPicture::setIcon(const uchar *data, int size, const char *format)
{
	if (data)
	{
		m_iconimg.loadFromData(data, size, format);
	}
	else
	{
		m_iconimg = QPixmap();
	}
	update();
}

void UIPicture::setIcon(const QImage m_icon)
{
	m_iconimg = QPixmap::fromImage(m_icon);

	update();
}

QString UIPicture::IconName() const
{
	return m_iconName;
}

void UIPicture::setdisIconName(const QString& name)
{
	m_disiconName = name;
	m_disiconimg.load(m_imgPath + m_disiconName);
	update();
}

QString UIPicture::disIconName() const
{
	return m_disiconName;
}


void UIPicture::setIconRect(const QRect &rect)
{
	m_iconRect = rect;
	update();
}


QRect UIPicture::IconRect() const
{
	return m_iconRect;
}

void UIPicture::setTextRect(const QRect &rect)
{
	m_textRect = rect;
	update();
}

QRect UIPicture::textRect() const
{
	return m_textRect;
}

void UIPicture::setColor(const QColor &color)
{
	m_color = color;
	update();
}

QColor UIPicture::color() const
{
	return m_color;
}

void UIPicture::paintEvent(QPaintEvent *)
{
	if (!m_bvisibale)
	{
		return;
	}
	QRect rc;
	rc = this->rect();
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing);

	//旋转图片
	// 	if(m_rotate != 0)
	// 	{
	// 		painter.translate(rc.center());
	// 		painter.rotate(m_rotate);
	// 		painter.translate(-rc.center());
	// 	}

	if(!m_imgName.isNull())
	{
		painter.drawPixmap(0, 0, m_bkimg);
	}

	if (this->isEnabled())
	{
		if(!m_iconimg.isNull())
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_iconimg);
			else
				painter.drawPixmap(0,0, m_iconimg);
		}
	}
	else
	{
		if(!m_iconName.isNull())
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_disiconimg);
			else
				painter.drawPixmap(0,0, m_disiconimg);
		}
	}

	if (!m_text.isEmpty())
	{
		painter.setPen(QColor(m_color));
		if (m_textflag)
		{
			painter.drawText(m_textRect,m_textalign | Qt::TextWordWrap,m_text);
		}
		else
		{
			QFontMetrics fm(font());
			QString text2 = fm.elidedText(m_text, Qt::ElideRight, m_textRect.width());
			painter.drawText(m_textRect,m_textalign,text2);
		}
	}

}

#include "uibutton.h"
//#include "guiconfig.h" 
#include <QPixmapCache>
/**************************************************************************************************
** å‡½æ•°å? UIButton
**
** åŠ? èƒ? æž„é€ å‡½æ•?**
** å? æ•? QWidget *parent                  : çˆ¶çª—å?**
** è¿? å›? æ—?**
**************************************************************************************************/
UIButton::UIButton(QWidget *parent)
:QWidget(parent)
{
	m_bcheck = false;
	m_bfocus = false;
	//m_imgPath = GetHMICfg()->cResHmiFolder;

#if defined(QT_PLUGIN)
    m_imgPath = "../resource/";
#else
    m_imgPath = ":/";
#endif

	//setMouseTracking(true);//è®¾ç½®é¼ æ ‡è·Ÿè¸ªäº‹ä»¶
	m_buttonState = BUTTON_NORMAL;
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(LongPress()));
	update();
}

void UIButton::setvisible(const bool bvisibale)
{
	m_bvisibale = bvisibale;
	setVisible(m_bvisibale);
	update();
}

bool UIButton::isvisibale() const
{
	return m_bvisibale;
}

UIButton::~UIButton()
{

}

/**************************************************************************************************
** å‡½æ•°å? mousePressEvent
**
** åŠ? èƒ? é¼ æ ‡æŒ‰ä¸‹äº‹ä»¶
**
** å? æ•? QMouseEvent *                    : é¼ æ ‡äº‹ä»¶
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::mousePressEvent(QMouseEvent *)
{
	if (BUTTON_DISABLE != m_buttonState)
	{
		m_buttonState = BUTTON_PRESS;
		m_timer->start(2000);
		repaint();
	}
}

/**************************************************************************************************
** å‡½æ•°å? mouseReleaseEvent
**
** åŠ? èƒ? é¼ æ ‡é‡Šæ”¾äº‹ä»¶
**
** å? æ•? QMouseEvent *                    : é¼ æ ‡äº‹ä»¶
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::mouseReleaseEvent(QMouseEvent *)
{
	if (BUTTON_DISABLE != m_buttonState && BUTTON_NORMAL != m_buttonState)
	{
		m_buttonState = BUTTON_RELEASE;
		if (m_timer->isActive())
			m_timer->stop();
		if (isEnabled())
		{
			emit(clicked());
		}
		update();
	}
}

/**************************************************************************************************
** å‡½æ•°å? mouseMoveEvent
**
** åŠ? èƒ? é¼ æ ‡ç§»åŠ¨äº‹ä»¶
**
** å? æ•? QMouseEvent *                    : é¼ æ ‡äº‹ä»¶
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::mouseMoveEvent(QMouseEvent *e)
{
	if (BUTTON_DISABLE != m_buttonState)
	{
		QRect rc;
		QPoint p;
		QMouseEvent *mouseEvent=NULL;

		if (QMouseEvent::MouseMove == e->type())
		{
			mouseEvent = (QMouseEvent *)e;

			if (mouseEvent->buttons() & Qt::LeftButton)
			{
				//p = e->pos();
				//rc = this->rect();
				//é¼ æ ‡ç§»åŠ¨åœ¨åŒºåŸŸå†…

				/*//å¼‚å½¢æŽ§ä»¶å“åº”çš„å®žçŽ?				QPixmap m_bkimg;
				QBitmap bm(m_bkimg.mask());
				QRegion region(bm);
				region.contains(e->pos())*/
				if(this->rect().contains(e->pos()))
					// 				if (((rc.x() <= p.x()) && ((rc.x() + rc.width()) >= p.x()))&&
					// 					((rc.y() <= p.y()) && (rc.y() + rc.height()) >= p.y()))
				{
					m_buttonState = BUTTON_PRESSMOVE;
				}
				else
				{
					if (m_timer->isActive())
						m_timer->stop();
					m_buttonState = BUTTON_NORMAL;
				}

				update();
			}
		}
		//emit move(p);
	}
}

/**************************************************************************************************
** å‡½æ•°å? enterEvent
**
** åŠ? èƒ? å½“é¼ æ ‡ç§»å…¥åˆ°æŒ‰é’®å†…æ—¶ï¼Œæ›´æ–°æŒ‰é’®å›¾æ ?**
** å? æ•? QMouseEvent *                    : é¼ æ ‡äº‹ä»¶
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::enterEvent(QEvent *)
{
	if (BUTTON_DISABLE != m_buttonState)
	{
		m_buttonState = BUTTON_MOVE;

		update();
	}
}

/**************************************************************************************************
** å‡½æ•°å? leaveEvent
**
** åŠ? èƒ? å½“é¼ æ ‡ç§»å‡ºåˆ°æŒ‰é’®ä»¥å¤–æ—¶ï¼Œæ›´æ–°æŒ‰é’®å›¾æ ‡
**
** å? æ•? QMouseEvent *                    : é¼ æ ‡äº‹ä»¶
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::leaveEvent(QEvent *)
{
	if (BUTTON_DISABLE != m_buttonState)
	{
		m_buttonState = BUTTON_NORMAL;

		update();
	}
}

void UIButton::uncheck()
{
	setChecked(false);
}

/**************************************************************************************************
** å‡½æ•°å? setNormalIcon
**
** åŠ? èƒ? è®¾ç½®æŒ‰é’®æ­£å¸¸çŠ¶æ€ä¸‹çš„å›¾æ ?**
** å? æ•? const QString &normalIconName    : å›¾æ ‡è·¯å¾„
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::setNormalIcon(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sIconnormal = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sIconnormal, m_normalIconimg)) {
			m_normalIconimg.load(m_imgPath + m_sIconnormal);
			QPixmapCache::insert(m_imgPath + m_sIconnormal, m_normalIconimg);
		}
		//m_normalIconimg.load(m_imgPath + m_sIconnormal);
	}
	update();
}

QString UIButton::NormalIcon() const
{
	return m_sIconnormal;
}

void UIButton::setPressIcon(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sIconpress = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sIconpress, m_pressIconimg)) {
			m_pressIconimg.load(m_imgPath + m_sIconpress);
			QPixmapCache::insert(m_imgPath + m_sIconpress, m_pressIconimg);
		}
		//m_pressIconimg.load(m_imgPath + m_sIconpress);

	}
	update();
}

QString UIButton::PressIcon() const
{
	return m_sIconpress;
}
/**************************************************************************************************
** å‡½æ•°å? setPressIcon
**
** åŠ? èƒ? è®¾ç½®æŒ‰é’®è¢«æŒ‰ä¸‹çŠ¶æ€ä¸‹çš„å›¾æ ?**
** å? æ•? const QString &pressIconName     : å›¾æ ‡è·¯å¾„
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::setDisableIcon(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sIcondisable = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sIcondisable, m_disableIconimg)) {
			m_disableIconimg.load(m_imgPath + m_sIcondisable);
			QPixmapCache::insert(m_imgPath + m_sIcondisable, m_disableIconimg);
		}
		//m_disableIconimg.load(m_imgPath + m_sIcondisable);

	}
	update();

}

QString UIButton::DisableIcon() const
{
	return m_sIcondisable;
}


/**************************************************************************************************
** å‡½æ•°å? setMoveIcon
**
** åŠ? èƒ? è®¾ç½®é¼ æ ‡åœ¨æŒ‰é’®ä¸Šç§»åŠ¨çŠ¶æ€ä¸‹çš„å›¾æ ?**
** å? æ•? const QString &moveIconName      : å›¾æ ‡è·¯å¾„
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::setNormalBk(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sBgnormal = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sBgnormal, m_normalBkimg)) {
			m_normalBkimg.load(m_imgPath + m_sBgnormal);
			QPixmapCache::insert(m_imgPath + m_sBgnormal, m_normalBkimg);
		}
		//m_normalBkimg.load(m_imgPath + m_sBgnormal);
	}
	update();

}

QString UIButton::NormalBk() const
{
	return m_sBgnormal;
}

void UIButton::setPressBk(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sBgpress = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sBgpress, m_pressBkimg)) {
			m_pressBkimg.load(m_imgPath + m_sBgpress);
			QPixmapCache::insert(m_imgPath + m_sBgpress, m_pressBkimg);
		}
		//m_pressBkimg.load(m_imgPath + m_sBgpress);
	}
	update();

}

QString UIButton::PressBk() const
{
	return m_sBgpress;
}

void UIButton::setDisableBk(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sBgdisable = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sBgdisable, m_disableBkimg)) {
			m_disableBkimg.load(m_imgPath + m_sBgdisable);
			QPixmapCache::insert(m_imgPath + m_sBgdisable, m_disableBkimg);
		}
		//m_disableBkimg.load(m_imgPath + m_sBgdisable);
	}
	update();

}

QString UIButton::DisableBk() const
{
	return m_sBgdisable;
}

void UIButton::setCheckedBk(const QString &imgName)
{
	if (!imgName.isEmpty())
	{
		m_sBgcheck = imgName;
		if (!QPixmapCache::find(m_imgPath + m_sBgcheck, m_checkBkimg)) {
			m_checkBkimg.load(m_imgPath + m_sBgcheck);
			QPixmapCache::insert(m_imgPath + m_sBgcheck, m_checkBkimg);
		}
		//m_checkBkimg.load(m_imgPath + m_sBgcheck);
	}
	update();

}

QString UIButton::CheckedBk() const
{
	return m_sBgcheck;
}

void UIButton::setIconRect(const QRect &rect)
{
	m_iconRect = rect;
	update();
}

QRect UIButton::IconRect() const
{
	return m_iconRect;
}

void UIButton::setTextRect(const QRect &rect)
{
	m_textRect = rect;
	update();
}

QRect UIButton::textRect() const
{
	return m_textRect;
}


void UIButton::setColor(const QColor &color)
{
	m_color = color;
	update();
}

QColor UIButton::color() const
{
	return m_color;
}

void UIButton::setDisableColor(const QColor &color)
{
	m_discolor = color;
	update();
}

QColor UIButton::disablecolor() const
{
	return m_discolor;
}

/**************************************************************************************************
** å‡½æ•°å? enabled
**
** åŠ? èƒ? è®¾ç½®æŒ‰é’®å„ç§çŠ¶æ€?ä½¿èƒ½/ç¦æ­¢)
**
** å? æ•? bool flg                         : false/true
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::enabled(bool flg)
{

	if (false == flg)
	{
		m_buttonState = BUTTON_DISABLE;
	}
	else
	{
		m_buttonState = BUTTON_NORMAL;
	}

	this->setEnabled(flg);
}

void UIButton::setChecked(bool flg)
{
	m_bcheck = flg;
	update();
}

bool UIButton::isChecked()
{
	return m_bcheck;
}

void UIButton::setFocused(bool flg)
{
	m_bfocus = flg;
	update();
}

bool UIButton::isFocused()
{
	return m_bfocus;
}

/**************************************************************************************************
** å‡½æ•°å? setUIButtonSize
**
** åŠ? èƒ? è®¾ç½®æŒ‰é’®ä½ç½®
**
** å? æ•? const QRect &rc                  : çŸ©å½¢åŒºåŸŸ
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::setUIButtonSize(const QRect &rc)
{
	this->setGeometry(rc);
}

/**************************************************************************************************
** å‡½æ•°å? setUIButtonSize
**
** åŠ? èƒ? è®¾ç½®æŒ‰é’®ä½ç½®
**
** å? æ•? int x                            : å·¦ä¸Šx
**         int y                            : å·¦ä¸Šy
**         int width                        : å®?**         int height                       : é«?**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::setUIButtonSize(int x, int y, int width, int height)
{
	this->setGeometry(x, y, width, height);
}

void UIButton::setResPath(const QString& path)
{
	m_imgPath = path;
}

QString UIButton::ResPath() const
{
	return m_imgPath;
}

void UIButton::setText(QString name)
{
	m_text = name;
	update();
}

QString UIButton::Text() const
{
	return m_text;
}

void UIButton::LongPress()
{
	if (m_timer->isActive())
		m_timer->stop();

	emit(LongPressed());
}

/**************************************************************************************************
** å‡½æ•°å? paintEvent
**
** åŠ? èƒ? é‡ç»˜
**
** å? æ•? QPaintEvent *                    : é‡ç»˜äº‹ä»¶
**
** è¿? å›? æ—?**
**************************************************************************************************/
void UIButton::paintEvent(QPaintEvent *)
{
	if (!m_bvisibale)
	{
		return;
	}
	QRect rc;
	rc = this->rect();
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing);

	if (BUTTON_PRESS == m_buttonState || BUTTON_PRESSMOVE == m_buttonState)//æŒ‰ä¸‹çŠ¶æ€?	
	{
		if (m_bcheck && !m_checkBkimg.isNull())
		{
			painter.drawPixmap(0, 0, m_checkBkimg);
		}
		else if (m_bfocus && !m_pressBkimg.isNull())
		{
			painter.drawPixmap(0, 0, m_pressBkimg);
		}
		else
		{
			painter.drawPixmap(0, 0, m_pressBkimg);
		}
		if (m_bcheck)
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_pressIconimg);
			else
				painter.drawPixmap(0,0, m_pressIconimg);
		}
		else
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_normalIconimg);
			else
				painter.drawPixmap(0,0, m_normalIconimg);
		}
	}
	else if ((BUTTON_NORMAL == m_buttonState) || (BUTTON_RELEASE == m_buttonState))//é‡Šæ”¾æˆ–æ­£å¸¸çŠ¶æ€?	
	{
		if (m_bcheck && !m_checkBkimg.isNull())
		{
			painter.drawPixmap(0, 0, m_checkBkimg);
		}
		else if (m_bfocus && !m_pressBkimg.isNull())
		{
			painter.drawPixmap(0, 0, m_pressBkimg);
		}
		else
		{
			painter.drawPixmap(0, 0, m_normalBkimg);
		}

		if (m_bcheck)
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_pressIconimg);
			else
				painter.drawPixmap(0,0, m_pressIconimg);
		}
		else
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_normalIconimg);
			else
				painter.drawPixmap(0,0, m_normalIconimg);
		}
	}
	else if (BUTTON_MOVE == m_buttonState)//ç§»åŠ¨çŠ¶æ€?	
	{
		if (m_bcheck && !m_checkBkimg.isNull())
		{
			painter.drawPixmap(0, 0, m_checkBkimg);
		}
		else if (m_bfocus && !m_pressBkimg.isNull())
		{
			painter.drawPixmap(0, 0, m_pressBkimg);
		}
		else
		{
			painter.drawPixmap(0, 0, m_normalBkimg);
		}
		if (m_bcheck)
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_pressIconimg);
			else
				painter.drawPixmap(0,0, m_pressIconimg);
		}
		else
		{
			if(m_iconRect.isValid())
				painter.drawPixmap(m_iconRect, m_normalIconimg);
			else
				painter.drawPixmap(0,0, m_normalIconimg);
		}
	}
	else if (BUTTON_DISABLE == m_buttonState)//ç¦æ­¢çŠ¶æ€?	
	{
		painter.drawPixmap(0, 0, m_disableBkimg);
		if(m_iconRect.isValid())
			painter.drawPixmap(m_iconRect, m_disableIconimg);
		else
			painter.drawPixmap(0,0, m_disableIconimg);
	}
	if (!m_text.isEmpty())
	{
		if (this->isEnabled())
		{
			painter.setPen(QColor(m_color));
		}
		else
		{
			painter.setPen(QColor(m_discolor));
		}
		QFontMetrics fm(font());
		QString text2 = fm.elidedText(m_text, Qt::ElideRight, m_textRect.width());
		painter.drawText(m_textRect,m_textalign,text2);
	}

}


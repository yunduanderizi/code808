#include <QStylePainter>
#include <QLocale>
#include "uitext.h"
//#include "guiconfig.h"

UIText::UIText(QWidget *parent)
: QLineEdit(parent)
, _textSeqId(-1)
{
	//_resPath = GetHMICfg()->cResHmiFolder;
#if defined(QT_PLUGIN)
    _resPath = "../resource/";
#else
    _resPath = ":/";
#endif

	if (QLocale::Chinese == this->locale().language())
	{
		this->setFont(_zhFont);
	}
	else if (QLocale::English == this->locale().language())
	{
		this->setFont(_enFont);
	}
}

UIText::~UIText()
{

}

void UIText::setResPath(const QString& resPath)
{
	_resPath = resPath;

	update();
}

void UIText::setBackgroundImage(const QString& imageName) 
{ 
	_backgroundImageName = imageName;
	_backgroundImageFlag = true;
	QString styleSheet;

	styleSheet += "background-image: url(";
	styleSheet += (_resPath + _backgroundImageName);
	styleSheet += ");";

	setStyleSheet(styleSheet);
}

void UIText::setTextSeqId(int sid)
{
	if (sid < 0)
	{
		_textSeqId = -1;
		return;
	}

	if (sid < _textList.size())
	{
		_textSeqId = sid;
		setText(_textList[_textSeqId]);
	}
	else
	{
		_textSeqId = -1;
	}
}

void UIText::setTextList(const QStringList& textList)
{
	_textList = textList;

	if (_textSeqId >= _textList.size())
	{
		_textSeqId = -1;
	}

	setTextSeqId(_textSeqId);
	update();
}

void UIText::setEnFont(const QFont& enFont)
{
	_enFont = enFont;

	if (QLocale::English == locale().language())
	{
		this->setFont(_enFont);
	}

	update();
}

void UIText::setZhFont(const QFont& zhFont)
{
	_zhFont = zhFont;

	if (QLocale::Chinese == locale().language())
	{
		this->setFont(_zhFont);
	}

	update();
}

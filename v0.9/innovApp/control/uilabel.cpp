#include <QStylePainter>
#include <QLocale>
#include "uilabel.h"
//#include "guiconfig.h"

UILabel::UILabel(QWidget *parent)
: QLabel(parent)
, _textSeqId(-1)
{

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

UILabel::~UILabel()
{

}

void UILabel::setResPath(const QString& resPath)
{
	_resPath = resPath;

	update();
}

void UILabel::setBackgroundImage(const QString& imageName) 
{ 
	_backgroundImageName = imageName;
	_backgroundImageFlag = true;
	QString styleSheet;

	styleSheet += "background-image: url(";
	styleSheet += (_resPath + _backgroundImageName);
	styleSheet += ");";

	setStyleSheet(styleSheet);
}

void UILabel::setTextSeqId(int sid)
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

void UILabel::setTextList(const QStringList& textList)
{
	_textList = textList;

	if (_textSeqId >= _textList.size())
	{
		_textSeqId = -1;
	}

	setTextSeqId(_textSeqId);
	update();
}

void UILabel::setEnFont(const QFont& enFont)
{
	_enFont = enFont;

	if (QLocale::English == locale().language())
	{
		this->setFont(_enFont);
	}

	update();
}

void UILabel::setZhFont(const QFont& zhFont)
{
	_zhFont = zhFont;

	if (QLocale::Chinese == locale().language())
	{
		this->setFont(_zhFont);
	}

	update();
}



// #include "uilabel.h"
// 
// UILabel::UILabel(QWidget *parent)
// 	: QWidget(parent)
// {
// 
// }
// 
// UILabel::~UILabel()
// {
// 
// }

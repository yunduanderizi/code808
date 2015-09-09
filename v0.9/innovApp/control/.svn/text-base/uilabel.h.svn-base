#ifndef UILABEL_H
#define UILABEL_H

// #include <QtGui/QWidget>
// 
// class UILabel : public QWidget
// {
// 	Q_OBJECT
// 
// public:
// 	UILabel(QWidget *parent = 0);
// 	~UILabel();
// 
// };


#include <QLabel>
#include <QMap>
#include <QPair>
#include <QList>

#if defined(QDESIGNER_EXPORT_WIDGETS)
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT UILabel : public QLabel
#else
class UILabel : public QLabel
#endif
{
	Q_OBJECT

		//Q_PROPERTY(QString resPath READ resPath WRITE setResPath)
		Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE setBackgroundImage)
		Q_PROPERTY(QStringList textList READ textList WRITE setTextList)
		Q_PROPERTY(int textSeqId READ textSeqId WRITE setTextSeqId)
		Q_PROPERTY(QFont enFont READ enFont WRITE setEnFont)
		Q_PROPERTY(QFont zhFont READ zhFont WRITE setZhFont)

public:
	UILabel(QWidget *parent = 0);
	~UILabel();

	void setResPath(const QString& resPath);
	const QString& resPath() const { return _resPath; }

	void setBackgroundImage(const QString& imageName);
	const QString& backgroundImage() const { return _backgroundImageName; }

	void setTextList(const QStringList& textList);
	const QStringList& textList() const { return _textList; };

	void setTextSeqId(int sid);
	int textSeqId() const { return _textSeqId; }

	void setEnFont(const QFont& enFont);
	const QFont& enFont() const { return _enFont; };

	void setZhFont(const QFont& zhFont);
	const QFont& zhFont() const { return _zhFont; };

private:
	QString	_resPath;

	bool	_backgroundImageFlag;
	QString	_backgroundImageName;

	int		_textSeqId;

	QStringList	_textList;
	QFont	_enFont;
	QFont	_zhFont;
};

#endif // UILABEL_H

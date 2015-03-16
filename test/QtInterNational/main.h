#include <QtGui/QApplication>
#include <QtGui>
#include <QTranslator>
#include <QPushButton>

class TranslationTest : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)
public:

	TranslationTest()
	{
		translator1 = new QTranslator(this);
		translator2 = new QTranslator(this);
		btn1 = new QPushButton(QObject::tr("Normal Button"));
		btn2 = new QPushButton(QObject::tr("Toggle Button"));
		btn2->setCheckable(TRUE);
		btn2->setChecked(TRUE);

		layout = new QVBoxLayout;
		layout->addWidget(btn1);
		layout->addWidget(btn2);
		layout->addStretch(1);
	}

	QString getEmptyString()
	{
		return "";
	}
 
	Q_INVOKABLE void selectLanguage(QString language) {
		if(language == QString("zh_CN")) {
			if(translator1->load("lang_zh_CN", "./Resources/qml/languages"))
				qApp->installTranslator(translator1);
		}
		if(language == QString("zh_TW")) {
			if(translator2->load("lang_zh_TW", "./Resources/qml/languages"))
				qApp->installTranslator(translator2);
		}
		if(language == QString("EN")){
			qApp->removeTranslator(translator1);
			qApp->removeTranslator(translator2);
		}
		emit languageChanged();
		retranslateUi();
	}

	void retranslateUi()
    {
        btn1->setText(tr("Normal Button"));
		btn2->setText(tr("Toggle Button"));
    }

	QVBoxLayout *layout;

signals:
	void languageChanged();

private:
	QTranslator *translator1;
	QTranslator *translator2;
	QPushButton *btn1;
    QPushButton *btn2;
};
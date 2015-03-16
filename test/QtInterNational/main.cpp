#include <QtDeclarative>
#include "main.h"

int main(int argc, char *argv[])
{	QApplication app(argc, argv);

	TranslationTest myObj;
	QDeclarativeView view;
	view.setSource(QUrl(QLatin1String("qrc:/qml/MainPage.qml")));
	view.rootContext()->setContextProperty("rootItem", (QObject *)&myObj);

	view.setLayout(myObj.layout);

	view.show();
	return app.exec();
}
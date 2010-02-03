#ifndef WEATHERWIDGETGRAPHICSVIEW_H_
#define WEATHERWIDGETGRAPHICSVIEW_H_

#include <QGraphicsView>

class WeatherGraphicsWindow;


class WeatherWidgetGraphicsView:public QGraphicsView
{
	Q_OBJECT
public:
	WeatherWidgetGraphicsView();
	~WeatherWidgetGraphicsView();

protected slots:
	void on_mainwindow_minimize();
	
private:
	WeatherGraphicsWindow* iMainWindow;
	QGraphicsScene* 	   iScene;
};

#endif /*WEATHERWIDGETGRAPHICSVIEW_H_*/

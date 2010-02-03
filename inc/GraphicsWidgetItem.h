#ifndef GRAPHICSWIDGETITEM_H_
#define GRAPHICSWIDGETITEM_H_

#include <QGraphicsWidget>
class QGraphicsItem;
class GraphicsWidgetItem:public QGraphicsWidget
{
	Q_OBJECT
	
public:
	
	// Takes ownership
	// I used it to take control of the item by a parameter of base class
	// Donno its usefull, but just did it as i studied an artice on ref-to-pointer
	// http://www.codeguru.com/cpp/cpp/cpp_mfc/pointers/article.php/c4089/
	GraphicsWidgetItem(QGraphicsItem* aGraphicsItem,QGraphicsWidget* aParent=0);
	
	~GraphicsWidgetItem();
	
	QGraphicsItem* graphicsItem();
	
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
	void clicked();

private:
	QGraphicsItem* iGraphicsItem;
};

#endif /*GRAPHICSWIDGETITEM_H_*/

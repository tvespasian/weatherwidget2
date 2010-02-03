#include "inc\GraphicsWidgetItem.h"
#include <QGraphicsItem>

GraphicsWidgetItem::GraphicsWidgetItem(QGraphicsItem* aGraphicsItem,
									   QGraphicsWidget* aParent)
					:iGraphicsItem(aGraphicsItem),
					QGraphicsWidget(aParent)
{
	iGraphicsItem->setParentItem(this);
}

GraphicsWidgetItem::~GraphicsWidgetItem()
{
	if(iGraphicsItem)
	{
		delete iGraphicsItem;
	}
}

QGraphicsItem* GraphicsWidgetItem::graphicsItem()
{
	return iGraphicsItem;
}

void GraphicsWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit clicked();
}

//eof
#include "squarebutton.h"

#include <QMouseEvent>

SquareButton::SquareButton(QWidget *parent) : QPushButton(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void SquareButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit leftClicked();
	}
	else if (event->button() == Qt::RightButton)
	{
		emit rightClicked();
	}
	else if (event->button() == Qt::MiddleButton)
	{
		emit middleClicked();
	}

	QPushButton::mousePressEvent(event);
}

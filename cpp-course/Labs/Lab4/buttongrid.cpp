#include "buttongrid.h"

#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QResizeEvent>
#include <QStyle>

ButtonGrid::ButtonGrid(int rows, int cols, bool showSmallButton, QWidget *parent) :
	QWidget(parent), nRows(rows), nCols(cols), counterValue(0), showButton(showSmallButton)
{
	flagLabel = new QLabel(tr("Mines:"), this);
	flagLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

	counterLabel = new QLabel(this);
	counterLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
	updateCounter();

	if (showButton)
	{
		smallButton = new SquareButton(this);
		smallButton->setStyleSheet("background-color: lightblue;");
		smallButton->show();
	}
	else
	{
		smallButton = nullptr;
	}

	for (int idx = 0; idx < nRows * nCols; ++idx)
	{
		SquareButton *btn = new SquareButton(this);
		buttons.push_back(btn);
		connect(btn, &SquareButton::leftClicked, this, [=]() {});
		connect(btn, &SquareButton::rightClicked, this, [=]() {});
		connect(btn, &SquareButton::middleClicked, this, [=]() {});
	}

	centerButton = new SquareButton(this);
	centerButton->setStyleSheet("background-color: lightblue;");

	connect(centerButton, &SquareButton::leftClicked, this, [=]() {});

	rightButton = new SquareButton(this);
	rightButton->setStyleSheet("background-color: lightgreen;");

	connect(rightButton, &SquareButton::leftClicked, this, [=]() {});

	additionalButton1 = new SquareButton(this);
	additionalButton1->setStyleSheet("background-color: lightcoral;");

	connect(additionalButton1, &SquareButton::leftClicked, this, [=]() {});

	additionalButton2 = new SquareButton(this);
	additionalButton2->setStyleSheet("background-color: lightsalmon;");

	connect(additionalButton2, &SquareButton::leftClicked, this, [=]() {});

	setMinimumSize(minimumSizeHint());
}

void ButtonGrid::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	int gridWidth = width();
	int gridHeight = height();

	if (gridWidth <= 0 || gridHeight <= 0)
		return;

	const int minCellSize = 10;
	double scaleFactor = 1.2;
	double totalScale = nRows + scaleFactor;

	int cellWidth = gridWidth / nCols;
	int cellHeight = gridHeight / totalScale;

	int cellSize = qMax(minCellSize, qMin(cellWidth, cellHeight));

	int centerSize = static_cast< int >(cellSize * scaleFactor);
	int rightSize = centerSize;

	// Центрируем контент по высоте
	int totalContentHeight = qMax(centerSize, rightSize) + (cellSize * nRows);
	int yOffset = (gridHeight - totalContentHeight) / 2;
	yOffset = qMax(0, yOffset);

	// Центрируем контент по ширине
	int xOffset = (gridWidth - (cellSize * nCols) - rightSize) / 2;
	xOffset = qMax(0, xOffset);

	// Позиционируем центральную кнопку
	int centerX = xOffset + (cellSize * nCols - centerSize) / 2;
	int centerY = yOffset;

	centerButton->setGeometry(centerX, centerY, centerSize, centerSize);
	centerButton->raise();

	// Позиционируем правую кнопку
	int rightX = centerX + centerSize;
	int rightY = centerY;

	rightButton->setGeometry(rightX, rightY, rightSize, rightSize);
	rightButton->raise();

	// Позиционируем дополнительные кнопки
	int additionalX1 = rightX + rightSize;
	int additionalY1 = rightY;

	additionalButton1->setGeometry(additionalX1, additionalY1, rightSize, rightSize);
	additionalButton1->raise();

	int additionalX2 = additionalX1 + rightSize;
	int additionalY2 = additionalY1;

	additionalButton2->setGeometry(additionalX2, additionalY2, rightSize, rightSize);
	additionalButton2->raise();

	// Позиционируем кнопки сетки
	int gridYStart = qMax(qMax(centerY + centerSize, rightY + rightSize), qMax(additionalY1 + rightSize, additionalY2 + rightSize));
	for (int idx = 0; idx < buttons.size(); ++idx)
	{
		int row = idx / nCols;
		int col = idx % nCols;
		int x = xOffset + col * cellSize;
		int y = gridYStart + row * cellSize;
		buttons[idx]->setGeometry(x, y, cellSize, cellSize);
	}

	// Позиционируем текст "Флаги"
	int flagLabelWidth = flagLabel->sizeHint().width();
	int flagLabelX = centerX - flagLabelWidth - 50;
	int flagLabelY = centerY;

	flagLabel->setGeometry(flagLabelX, flagLabelY, flagLabelWidth, 30);

	// Позиционируем счетчик
	int counterWidth = counterLabel->sizeHint().width();
	counterLabel->setMinimumWidth(40);
	int counterX = flagLabelX + flagLabelWidth + 5;
	int counterY = centerY;

	counterLabel->setGeometry(counterX, counterY, counterWidth, 30);

	// Позиционируем маленькую кнопку, если она существует
	if (smallButton)
	{
		int smallButtonSize = static_cast< int >(cellSize * 0.5);
		int smallButtonX = additionalX2 + rightSize + 5;
		int smallButtonY = centerY + (centerSize - smallButtonSize) / 2;
		smallButton->setGeometry(smallButtonX, smallButtonY, smallButtonSize, smallButtonSize);
	}
}

QSize ButtonGrid::minimumSizeHint() const
{
	const int minButtonSize = 10;
	int totalWidth = minButtonSize * nCols + minButtonSize * 2 + 50;
	int totalHeight = minButtonSize * nRows + static_cast< int >(minButtonSize * 1.2);
	return QSize(totalWidth, totalHeight);
}

void ButtonGrid::updateCounter()
{
	counterLabel->setText(QString::number(counterValue));
}

void ButtonGrid::setCounter(int value)
{
	counterValue = value;
	updateCounter();
}

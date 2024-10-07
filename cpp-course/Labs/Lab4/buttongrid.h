#ifndef BUTTONGRID_H
#define BUTTONGRID_H

#include "squarebutton.h"

#include <QLabel>
#include <QVector>
#include <QWidget>

class ButtonGrid : public QWidget
{
	Q_OBJECT

  public:
	ButtonGrid(int rows, int cols, bool showSmallButton = false, QWidget *parent = nullptr);
	~ButtonGrid() = default;

  protected:
	void resizeEvent(QResizeEvent *event) override;
	QSize minimumSizeHint() const override;

  private:
	int nRows;
	int nCols;
	QLabel *counterLabel;
	QLabel *flagLabel;
	int counterValue;
	bool showButton;

  public:
	QVector< SquareButton * > buttons;
	SquareButton *centerButton;
	SquareButton *rightButton;
	SquareButton *additionalButton1;
	SquareButton *additionalButton2;
	SquareButton *smallButton;
	void updateCounter();
	void setCounter(int value);
};

#endif	  // BUTTONGRID_H

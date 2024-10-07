#ifndef SQUAREBUTTON_H
#define SQUAREBUTTON_H

#include <QPushButton>

class SquareButton : public QPushButton
{
	Q_OBJECT

  public:
	explicit SquareButton(QWidget *parent = nullptr);

  signals:
	void leftClicked();
	void rightClicked();
	void middleClicked();

  protected:
	void mousePressEvent(QMouseEvent *event) override;
};

#endif	  // SQUAREBUTTON_H

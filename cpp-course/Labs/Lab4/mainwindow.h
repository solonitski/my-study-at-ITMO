#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"

#include <QMainWindow>
#include <QPushButton>
#include <QTranslator>
#include <QVector>

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	explicit MainWindow(Settings &settings, QWidget *parent = nullptr);
	~MainWindow() override;

  private slots:
	void startGame();
	void openSettings();
	void openLanguageDialog();

  private:
	void changeLanguage(Language lang);
	void updateButtonTexts();

	Settings settings;
	QTranslator *translator;
	QVector< QPushButton * > buttons;
	bool debugMode;
};

#endif	  // MAINWINDOW_H

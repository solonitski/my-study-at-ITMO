#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "settings.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTranslator>
#include <QVBoxLayout>

class SettingsDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit SettingsDialog(Settings &settings, QWidget *parent = nullptr);

  private slots:
	void applySettings();

  private:
	QLineEdit *widthLineEdit;
	QLineEdit *heightLineEdit;
	QLineEdit *minesLineEdit;
	Settings &settings;
};

#endif	  // SETTINGSDIALOG_H

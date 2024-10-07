#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include "settings.h"

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class LanguageDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit LanguageDialog(Settings &settings, QWidget *parent = nullptr);
	Language selectedLanguage() const;
	void acceptSelection();

  private:
	QComboBox *languageComboBox;
	Settings &settings;
};

#endif	  // LANGUAGEDIALOG_H

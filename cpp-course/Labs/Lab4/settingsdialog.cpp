#include "settingsdialog.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(Settings &settings, QWidget *parent) : QDialog(parent), settings(settings)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	setWindowTitle(tr("Settings"));

	widthLineEdit = new QLineEdit(QString::number(settings.getWidth()), this);
	heightLineEdit = new QLineEdit(QString::number(settings.getHeight()), this);
	minesLineEdit = new QLineEdit(QString::number(settings.getMines()), this);

	layout->addWidget(new QLabel(tr("Width:")));
	layout->addWidget(widthLineEdit);
	layout->addWidget(new QLabel(tr("Height:")));
	layout->addWidget(heightLineEdit);
	layout->addWidget(new QLabel(tr("Mines:")));
	layout->addWidget(minesLineEdit);

	QPushButton *applyButton = new QPushButton(tr("Apply"), this);
	layout->addWidget(applyButton);

	connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::applySettings);
}

void SettingsDialog::applySettings()
{
	int newWidth = widthLineEdit->text().toInt();
	int newHeight = heightLineEdit->text().toInt();
	int newMines = minesLineEdit->text().toInt();

	bool valid = true;
	QString errorMsg;

	if (!settings.setWidth(newWidth))
	{
		valid = false;
		errorMsg += tr("Invalid width value") + '\n';
	}
	if (!settings.setHeight(newHeight))
	{
		valid = false;
		errorMsg += tr("Invalid height value") + '\n';
	}
	if (!settings.setMines(newMines))
	{
		valid = false;
		errorMsg += tr("Invalid mines value") + '\n';
	}

	if (valid)
	{
		accept();
	}
	else
	{
		QMessageBox::warning(this, tr("Invalid input"), errorMsg);
	}
}

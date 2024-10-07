#include "language_dialog.h"

#include "settings.h"

LanguageDialog::LanguageDialog(Settings &settings, QWidget *parent) :
	QDialog(parent), languageComboBox(new QComboBox(this)), settings(settings)
{
	setWindowTitle(tr("Language"));
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QLabel *label = new QLabel(tr("Select Language"), this);

	languageComboBox->addItem("English", QVariant::fromValue(Language::EN));
	languageComboBox->addItem("Русский", QVariant::fromValue(Language::RU));
	languageComboBox->addItem("Српски", QVariant::fromValue(Language::SR));

	// Устанавливаем текущий язык
	int index = languageComboBox->findData(QVariant::fromValue(settings.getLanguage()));
	if (index != -1)
	{
		languageComboBox->setCurrentIndex(index);
	}

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QPushButton *okButton = new QPushButton(tr("OK"), this);
	QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);

	buttonLayout->addStretch();
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	mainLayout->addWidget(label);
	mainLayout->addWidget(languageComboBox);
	mainLayout->addLayout(buttonLayout);

	connect(okButton, &QPushButton::clicked, this, &LanguageDialog::acceptSelection);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void LanguageDialog::acceptSelection()
{
	settings.saveSettings();
	settings.setLanguage(selectedLanguage());
	accept();
}

Language LanguageDialog::selectedLanguage() const
{
	return static_cast< Language >(languageComboBox->currentData().value< int >());
}

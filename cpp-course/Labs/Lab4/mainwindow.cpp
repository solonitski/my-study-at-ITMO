#include "mainwindow.h"

#include "game.h"
#include "language_dialog.h"
#include "settingsdialog.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QVBoxLayout>

MainWindow::MainWindow(Settings &settings, QWidget *parent) : QMainWindow(parent), settings(settings), debugMode(false)
{
	this->settings.loadSettings();

	translator = new QTranslator(this);

	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(centralWidget);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	setWindowTitle(tr("Main Menu"));
	QPushButton *startGameButton = new QPushButton(tr("Start Game"));
	QPushButton *settingsButton = new QPushButton(tr("Settings"));
	QPushButton *changeLanguageButton = new QPushButton(tr("Change Language"));

	buttons.push_back(startGameButton);
	buttons.push_back(settingsButton);
	buttons.push_back(changeLanguageButton);

	for (auto &button : buttons)
	{
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		layout->addWidget(button);
	}

	setCentralWidget(centralWidget);

	layout->setStretch(0, 1);
	layout->setStretch(1, 1);
	layout->setStretch(2, 1);

	QStringList args = QCoreApplication::arguments();
	if (args.size() > 1 && args[1] == "dbg")
	{
		debugMode = true;
	}

	connect(startGameButton, &QPushButton::clicked, this, &MainWindow::startGame);
	connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
	connect(changeLanguageButton, &QPushButton::clicked, this, &MainWindow::openLanguageDialog);

	changeLanguage(this->settings.getLanguage());
}

MainWindow::~MainWindow()
{
	delete translator;
}

void MainWindow::startGame()
{
	Game *game = new Game(this->settings, nullptr, debugMode, true);
	game->setWindowTitle(tr("Minesweeper"));
	game->setMinimumHeight(10 * this->settings.getHeight() + 28);
	game->setMinimumWidth(10 * this->settings.getWidth());
	int width = this->settings.getWidth() <= 8 ? 400 : 400;
	int height = this->settings.getHeight() <= 8 ? 200 : 450;
	width *= this->settings.getWidth() >= 20 ? 2 : 1;
	height *= this->settings.getHeight() >= 20 ? 2 : 1;
	game->resize(width, height);
	game->show();

	this->hide();
}

void MainWindow::openSettings()
{
	SettingsDialog dialog(settings, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		settings.saveSettings();
	}
}

void MainWindow::openLanguageDialog()
{
	LanguageDialog dialog(settings, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		changeLanguage(settings.getLanguage());
		settings.saveSettings();
	}
}

void MainWindow::changeLanguage(Language lang)
{
	QString languageFile;

	switch (lang)
	{
	case Language::EN:
		languageFile = "new_sweeper_en_US.qm";
		break;
	case Language::RU:
		languageFile = "new_sweeper_ru_RU.qm";
		break;
	case Language::SR:
		languageFile = "new_sweeper_sr_RS.qm";
		break;
	default:
		return;
	}

	QString projectPath = QCoreApplication::applicationDirPath();
	QDir dir(projectPath);
	QString fullPath = dir.filePath(languageFile);

	qApp->removeTranslator(translator);

	if (translator->load(fullPath))
	{
		qApp->installTranslator(translator);
		updateButtonTexts();
		setWindowTitle(tr("Main Menu"));
	}
}

void MainWindow::updateButtonTexts()
{
	buttons[0]->setText(tr("Start Game"));
	buttons[1]->setText(tr("Settings"));
	buttons[2]->setText(tr("Change Language"));
}

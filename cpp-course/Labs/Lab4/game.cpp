#include "game.h"

#include "mainwindow.h"
#include "settingsdialog.h"
#include "squarebutton.h"

#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>

// в правилах точно не написано, но я сделал так, что выиграть можно лишь тогда, когда все мины помечены флажками.

Game::Game(Settings &settings, QWidget *parent, bool peek, bool loadState) :
	ButtonGrid(settings.getHeight(), settings.getWidth(), peek, parent), settings(settings),
	nRows(settings.getHeight()), nCols(settings.getWidth()), nMines(settings.getMines()), flagsCount(0),
	firstClick(true), isEnd(false), leftyMode(settings.getLeftyMode()), peekMode(peek)
{
	if (loadState && validateGameState())
	{
		loadGameState();
	}
	else
	{
		clearGameState();
		initGame();
	}

	for (int row = 0; row < nRows; ++row)
	{
		for (int col = 0; col < nCols; ++col)
		{
			int idx = row * nCols + col;
			SquareButton *btn = buttons.at(idx);

			connect(btn, &SquareButton::leftClicked, this, [=]() { handleLeftClick(row, col); });
			connect(btn, &SquareButton::rightClicked, this, [=]() { handleRightClick(row, col); });
		}
	}

	// перезапуск игры
	centerButton->setText("😀");
	centerButton->setStyleSheet("background-color: yellow;");
	connect(centerButton, &SquareButton::leftClicked, this, &Game::resetGameSlot);

	// кнопка настроек
	rightButton->setText("⚙");
	rightButton->setStyleSheet("background-color: grey;");
	connect(rightButton, &SquareButton::leftClicked, this, &Game::openSettingsDialog);

	// выход в меню
	additionalButton1->setText("🠻");
	additionalButton1->setStyleSheet("background-color: pink;");
	connect(additionalButton1, &SquareButton::leftClicked, this, &Game::goToMainMenu);

	// кнопка для "левшей"
	additionalButton2->setText("✋");
	additionalButton2->setStyleSheet("background-color: purple;");
	connect(additionalButton2, &SquareButton::leftClicked, this, &Game::swapLeftAndRight);

	QStringList args = QCoreApplication::arguments();
	if (args.size() > 1 && args[1] == "dbg")
	{
		peekMode = true;
	}

	// кнопка "подглядывания"
	if (peekMode)
	{
		smallButton->setText("👁️");
		connect(smallButton, &SquareButton::leftClicked, this, &Game::togglePeekMode);
	}

	setCounter(nMines);
}

void Game::initGame()
{
	field.resize(nRows * nCols);
	for (int i = 0; i < nRows * nCols; ++i)
	{
		field[i] = { false, false, false, false, 0 };
	}
	isEnd = false;
}

void Game::placeMines(int initialRow, int initialCol)
{
	int minesPlaced = 0;
	while (minesPlaced < nMines)
	{
		int row = QRandomGenerator::global()->bounded(nRows);
		int col = QRandomGenerator::global()->bounded(nCols);
		int idx = row * nCols + col;

		if ((row == initialRow && col == initialCol) || field[idx].hasMine)
			continue;

		field[idx].hasMine = true;
		++minesPlaced;
	}
}

void Game::calculateAdjacents()
{
	for (int row = 0; row < nRows; ++row)
	{
		for (int col = 0; col < nCols; ++col)
		{
			int idx = row * nCols + col;
			if (field[idx].hasMine)
			{
				field[idx].adjacentMines = -1;
				continue;
			}

			int mineCount = 0;
			for (int dr = -1; dr <= 1; ++dr)
			{
				for (int dc = -1; dc <= 1; ++dc)
				{
					int newRow = row + dr;
					int newCol = col + dc;
					if (newRow >= 0 && newRow < nRows && newCol >= 0 && newCol < nCols && field[newRow * nCols + newCol].hasMine)
					{
						++mineCount;
					}
				}
			}
			field[idx].adjacentMines = mineCount;
		}
	}
}

void Game::handleLeftClick(int row, int col)
{
	if (leftyMode)
	{
		processClick(row, col, false);
	}
	else
	{
		processClick(row, col, true);
	}
}

void Game::handleRightClick(int row, int col)
{
	if (leftyMode)
	{
		processClick(row, col, true);
	}
	else
	{
		processClick(row, col, false);
	}
}

void Game::processClick(int row, int col, bool isLeftClick)
{
	int idx = row * nCols + col;
	if (isEnd)
	{
		return;
	}

	if (isLeftClick)
	{
		if (field[idx].isFlagged || field[idx].isQuestioned)
		{
			return;
		}

		if (firstClick)
		{
			firstClick = false;
			placeMines(row, col);
			calculateAdjacents();
		}

		if (field[idx].isRevealed)
		{
			return;
		}

		revealCell(row, col);
	}
	else
	{
		if (field[idx].isRevealed)
		{
			return;
		}

		toggleFlag(row, col);
	}

	if (checkWinCondition())
	{
		QMessageBox::information(nullptr, tr("Victory"), tr("You have won!"));
		lockField();
		clearGameState();
		isEnd = true;
	}
}

void Game::revealCell(int row, int col)
{
	if (row < 0 || row >= nRows || col < 0 || col >= nCols)
		return;

	int idx = row * nCols + col;
	Cell &cell = field[idx];

	if (cell.isRevealed || cell.isFlagged)
		return;

	cell.isRevealed = true;
	SquareButton *btn = buttons.at(idx);

	if (cell.hasMine)
	{
		btn->setText("💣");
		btn->setStyleSheet("background-color: red;");
		gameOver(row, col);
	}
	else if (cell.adjacentMines > 0)
	{
		btn->setText(QString::number(cell.adjacentMines));
		btn->setEnabled(false);
	}
	else
	{
		btn->setEnabled(false);
		revealAdjacentCells(row, col);
	}
}

void Game::revealAdjacentCells(int row, int col)
{
	for (int dr = -1; dr <= 1; ++dr)
	{
		for (int dc = -1; dc <= 1; ++dc)
		{
			int newRow = row + dr;
			int newCol = col + dc;
			if (newRow >= 0 && newRow < nRows && newCol >= 0 && newCol < nCols)
			{
				int idx = newRow * nCols + newCol;
				if (!field[idx].isRevealed)
				{
					revealCell(newRow, newCol);
				}
			}
		}
	}
}

void Game::toggleFlag(int row, int col)
{
	int idx = row * nCols + col;
	Cell &cell = field[idx];
	SquareButton *btn = buttons.at(idx);

	if (!cell.isFlagged && !cell.isQuestioned)
	{
		cell.isFlagged = true;
		btn->setText("🚩");
		flagsCount++;
	}
	else if (cell.isFlagged)
	{
		cell.isFlagged = false;
		cell.isQuestioned = true;
		btn->setText("?");
		flagsCount--;
	}
	else if (cell.isQuestioned)
	{
		cell.isQuestioned = false;
		btn->setText("");
	}

	int remainingMines = nMines - flagsCount;
	setCounter(remainingMines);
}

void Game::gameOver(int row, int col)
{
	if (isEnd)
		return;

	isEnd = true;

	clearGameState();

	for (int r = 0; r < nRows; ++r)
	{
		for (int c = 0; c < nCols; ++c)
		{
			int idx = r * nCols + c;
			Cell &cell = field[idx];
			SquareButton *btn = buttons.at(idx);

			if (cell.hasMine)
			{
				btn->setText("💣");
				btn->setStyleSheet("background-color: red;");
			}
			else if (cell.adjacentMines > 0)
			{
				btn->setText(QString::number(cell.adjacentMines));
				btn->setStyleSheet("");
			}

			if (cell.isFlagged)
			{
				btn->setText("🚩");
				if (cell.hasMine)
				{
					btn->setStyleSheet("background-color: lightgreen;");
				}
				else
				{
					btn->setStyleSheet("background-color: red;");
				}
			}

			btn->setEnabled(false);
		}
	}

	int idx = row * nCols + col;
	SquareButton *btn = buttons.at(idx);
	btn->setStyleSheet("background-color: darkred;");
	btn->setText("💥");
	btn->setEnabled(false);

	centerButton->setText("😢");
	QMessageBox::information(nullptr, tr("Defeat"), tr("You have lost!"));
}

void Game::lockField()
{
	for (auto &btn : buttons)
	{
		btn->setEnabled(false);
	}
}

bool Game::checkWinCondition()
{
	for (int i = 0; i < nRows * nCols; ++i)
	{
		Cell &cell = field[i];
		if ((!cell.hasMine && !cell.isRevealed) || (cell.hasMine && !cell.isFlagged))
		{
			return false;
		}
	}
	return true;
}

void Game::resetGame()
{
	windowPosition = this->pos();

	this->close();
	Game *newGame = new Game(settings, nullptr, peekMode, false);
	newGame->setWindowTitle(tr("Minesweeper"));

	newGame->setMinimumHeight(10 * settings.getHeight() + 28);
	newGame->setMinimumWidth(10 * settings.getWidth());
	int width = settings.getWidth() <= 8 ? 400 : 400;
	int height = settings.getHeight() <= 8 ? 200 : 450;
	width *= settings.getWidth() >= 20 ? 2 : 1;
	height *= settings.getHeight() >= 20 ? 2 : 1;
	newGame->resize(width, height);

	newGame->move(windowPosition);
	newGame->show();
}

void Game::resetGameSlot()
{
	resetGame();
}

void Game::openSettingsDialog()
{
	SettingsDialog dialog(settings, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		nRows = settings.getHeight();
		nCols = settings.getWidth();
		nMines = settings.getMines();

		resetGame();
		initGame();
	}
}

void Game::goToMainMenu()
{
	this->close();
	MainWindow *mainWindow = new MainWindow(settings);
	mainWindow->setWindowTitle(tr("Main Menu"));
	mainWindow->resize(320, 180);
	mainWindow->show();
}

void Game::swapLeftAndRight()
{
	leftyMode = !leftyMode;
	settings.setLeftyMode(leftyMode);
}

void Game::togglePeekMode()
{
	if (isEnd)
	{
		return;
	}

	static bool hiddenFieldsVisible = false;

	if (!hiddenFieldsVisible)
	{
		for (int idx = 0; idx < field.size(); ++idx)
		{
			Cell &cell = field[idx];
			SquareButton *btn = buttons.at(idx);

			if (!cell.isRevealed)
			{
				if (cell.isFlagged)
				{
					btn->setText("🚩");
					if (cell.hasMine)
					{
						btn->setStyleSheet("background-color: lightgreen;");
					}
					else
					{
						btn->setStyleSheet("");
					}
				}
				else if (cell.isQuestioned)
				{
					btn->setText("?");
					if (cell.hasMine)
					{
						btn->setStyleSheet("background-color: lightgreen;");
					}
					else
					{
						btn->setStyleSheet("");
					}
				}
				else
				{
					if (cell.hasMine)
					{
						btn->setText("💣");
					}
					else if (cell.adjacentMines > 0)
					{
						btn->setText(QString::number(cell.adjacentMines));
						btn->setStyleSheet("");
					}
					else
					{
						btn->setText("");
						btn->setStyleSheet("");
					}
				}
			}
		}
		hiddenFieldsVisible = true;
	}
	else
	{
		for (int idx = 0; idx < field.size(); ++idx)
		{
			Cell &cell = field[idx];
			SquareButton *btn = buttons.at(idx);

			if (!cell.isRevealed)
			{
				if (cell.isFlagged)
				{
					btn->setText("🚩");
					btn->setStyleSheet("");
				}
				else if (cell.isQuestioned)
				{
					btn->setText("?");
					btn->setStyleSheet("");
				}
				else
				{
					btn->setText("");
					btn->setStyleSheet("");
				}
			}
		}
		hiddenFieldsVisible = false;
	}
}

void Game::closeEvent(QCloseEvent *event)
{
	if (!isEnd)
	{
		saveGameState();
		settings.saveSettings();
	}
	QWidget::closeEvent(event);
}

void Game::saveGameState()
{
	QSettings gameSettings(QCoreApplication::applicationDirPath() + "/gameState.ini", QSettings::IniFormat);
	gameSettings.setValue("nRows", nRows);
	gameSettings.setValue("nCols", nCols);
	gameSettings.setValue("nMines", nMines);
	gameSettings.setValue("flagsCount", flagsCount);
	gameSettings.setValue("firstClick", firstClick);
	gameSettings.setValue("isEnd", isEnd);
	gameSettings.setValue("leftyMode", leftyMode);
	gameSettings.setValue("peekMode", peekMode);

	for (int i = 0; i < field.size(); ++i)
	{
		QString baseKey = QString("cell%1").arg(i);
		gameSettings.setValue(baseKey + "/hasMine", field[i].hasMine);
		gameSettings.setValue(baseKey + "/isRevealed", field[i].isRevealed);
		gameSettings.setValue(baseKey + "/isFlagged", field[i].isFlagged);
		gameSettings.setValue(baseKey + "/isQuestioned", field[i].isQuestioned);
		gameSettings.setValue(baseKey + "/adjacentMines", field[i].adjacentMines);
	}
}

void Game::loadGameState()
{
	QSettings gameSettings(QCoreApplication::applicationDirPath() + "/gameState.ini", QSettings::IniFormat);
	settings.loadSettings();

	if (gameSettings.contains("nRows"))
	{
		nRows = gameSettings.value("nRows").toInt();
	}

	if (gameSettings.contains("nCols"))
	{
		nCols = gameSettings.value("nCols").toInt();
	}

	if (gameSettings.contains("nMines"))
	{
		nMines = gameSettings.value("nMines").toInt();
	}

	if (gameSettings.contains("flagsCount"))
	{
		flagsCount = gameSettings.value("flagsCount").toInt();
	}

	if (gameSettings.contains("firstClick"))
	{
		firstClick = gameSettings.value("firstClick").toBool();
	}

	if (gameSettings.contains("isEnd"))
	{
		isEnd = gameSettings.value("isEnd").toBool();
	}

	if (gameSettings.contains("leftyMode"))
	{
		leftyMode = gameSettings.value("leftyMode").toBool();
	}

	field.resize(nRows * nCols);
	for (int i = 0; i < field.size(); ++i)
	{
		QString baseKey = QString("cell%1").arg(i);
		field[i].hasMine = gameSettings.value(baseKey + "/hasMine").toBool();
		field[i].isRevealed = gameSettings.value(baseKey + "/isRevealed").toBool();
		field[i].isFlagged = gameSettings.value(baseKey + "/isFlagged").toBool();
		field[i].isQuestioned = gameSettings.value(baseKey + "/isQuestioned").toBool();
		field[i].adjacentMines = gameSettings.value(baseKey + "/adjacentMines").toInt();
	}

	for (int r = 0; r < nRows; ++r)
	{
		for (int c = 0; c < nCols; ++c)
		{
			int idx = r * nCols + c;
			SquareButton *btn = buttons.at(idx);
			updateButtonFromCell(btn, field[idx]);
		}
	}
}

void Game::updateButtonFromCell(SquareButton *btn, const Cell &cell)
{
	if (cell.isRevealed)
	{
		if (cell.hasMine)
		{
			btn->setText("💣");
			btn->setStyleSheet("background-color: red;");
		}
		else
		{
			if (cell.adjacentMines > 0)
			{
				btn->setText(QString::number(cell.adjacentMines));
			}
			else
			{
				btn->setText("");
			}
		}
		btn->setEnabled(false);
	}
	else
	{
		if (cell.isFlagged)
		{
			btn->setText("🚩");
		}
		else if (cell.isQuestioned)
		{
			btn->setText("?");
		}
		else
		{
			btn->setText("");
		}
		btn->setEnabled(true);
	}
}

bool Game::validateGameState()
{
	QSettings gameSettings(QCoreApplication::applicationDirPath() + "/gameState.ini", QSettings::IniFormat);
	settings.loadSettings();

	if (gameSettings.contains("nRows"))
	{
		int rows = gameSettings.value("nRows").toInt();
		if (!(rows > 0 && rows <= 50 && rows == settings.getHeight()))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if (gameSettings.contains("nCols"))
	{
		int cols = gameSettings.value("nCols").toInt();
		if (!(cols > 0 && cols <= 50 && cols == settings.getWidth()))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if (gameSettings.contains("nMines"))
	{
		int mines = gameSettings.value("nMines").toInt();
		if (!(mines >= 0 && mines <= nRows * nCols && mines == settings.getMines()))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if (!gameSettings.contains("flagsCount"))
	{
		return false;
	}

	if (!gameSettings.contains("firstClick"))
	{
		return false;
	}

	if (!gameSettings.contains("isEnd"))
	{
		return false;
	}

	if (!gameSettings.contains("leftyMode"))
	{
		return false;
	}

	field.resize(nRows * nCols);
	int minesInCellsCount = 0;

	for (int i = 0; i < field.size(); ++i)
	{
		QString baseKey = QString("cell%1").arg(i);
		if (!gameSettings.contains(baseKey + "/hasMine") || !gameSettings.contains(baseKey + "/adjacentMines") ||
			!gameSettings.contains(baseKey + "/isRevealed") || !gameSettings.contains(baseKey + "/isFlagged") ||
			!gameSettings.contains(baseKey + "/isQuestioned"))
		{
			return false;
		}

		bool hasMine = gameSettings.value(baseKey + "/hasMine").toBool();

		if (hasMine)
		{
			minesInCellsCount++;
		}
	}

	return minesInCellsCount == nMines;
}

void Game::clearGameState()
{
	QFile file(QCoreApplication::applicationDirPath() + "/gameState.ini");
	if (file.exists())
	{
		file.remove();
	}
}

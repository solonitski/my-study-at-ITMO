#include "settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

Settings::Settings(int width, int height, int mines, bool mode, Language lang) :
	m_width(width), m_height(height), m_mines(mines), leftyMode(mode), m_language(lang)
{
}

int Settings::getWidth() const
{
	return m_width;
}

int Settings::getHeight() const
{
	return m_height;
}

int Settings::getMines() const
{
	return m_mines;
}

bool Settings::getLeftyMode() const
{
	return leftyMode;
}

Language Settings::getLanguage() const
{
	return m_language;
}

bool Settings::setWidth(int width)
{
	if (isValidFieldSize(width, m_height))
	{
		m_width = width;
		return true;
	}
	return false;
}

bool Settings::setHeight(int height)
{
	if (isValidFieldSize(m_width, height))
	{
		m_height = height;
		return true;
	}
	return false;
}

bool Settings::setMines(int mines)
{
	if (isValidMinesCount(mines))
	{
		m_mines = mines;
		return true;
	}
	return false;
}

bool Settings::setLeftyMode(bool mode)
{
	leftyMode = mode;
	return true;
}

bool Settings::setLanguage(Language lang)
{
	if (lang < Language::COUNT)
	{
		m_language = lang;
		return true;
	}
	return false;
}

bool Settings::isValidFieldSize(int width, int height) const
{
	return width > 0 && height > 0 && width < 51 && height < 51;
}

bool Settings::isValidMinesCount(int mines) const
{
	return mines > 0 && mines < m_width * m_height;
}

void Settings::loadSettings()
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

	bool isGood =
		setWidth(settings.value("width", m_width).toInt()) && setHeight(settings.value("height", m_height).toInt()) &&
		setMines(settings.value("mines", m_width).toInt()) && setLeftyMode(settings.value("leftyMode", leftyMode).toBool()) &&
		setLanguage(static_cast< Language >(settings.value("language", static_cast< int >(m_language)).toInt()));

	if (!isGood)
	{
		setWidth(10);
		setHeight(10);
		setMines(10);
		setLeftyMode(false);
		setLanguage(Language::EN);
	}
}

void Settings::saveSettings() const
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
	settings.setValue("width", m_width);
	settings.setValue("height", m_height);
	settings.setValue("mines", m_mines);
	settings.setValue("leftyMode", leftyMode);
	settings.setValue("language", static_cast< int >(m_language));
}

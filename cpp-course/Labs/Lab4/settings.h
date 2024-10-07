#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>

enum class Language
{
	EN,	   // Английский
	RU,	   // Русский
	SR,	   // Сербский
	// ... другие языки
	COUNT,	  // Количество языков
};

Q_DECLARE_METATYPE(Language);

class Settings
{
  public:
	Settings(int width = 10, int height = 10, int mines = 10, bool mode = false, Language lang = Language::EN);

	int getWidth() const;
	int getHeight() const;
	int getMines() const;
	bool getLeftyMode() const;
	Language getLanguage() const;

	bool setWidth(int width);
	bool setHeight(int height);
	bool setMines(int mines);
	bool setLeftyMode(bool mode);
	bool setLanguage(Language lang);

	void loadSettings();
	void saveSettings() const;

  private:
	bool isValidFieldSize(int width, int height) const;
	bool isValidMinesCount(int mines) const;

	int m_width;
	int m_height;
	int m_mines;
	bool leftyMode;
	Language m_language;
};

#endif	  // SETTINGS_H

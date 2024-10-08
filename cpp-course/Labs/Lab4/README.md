# Лабораторная работа №4
## Qt

### Инструментарий и требования к работе
Работа выполняется на C++. На сервере сборка под C++20. Программа должна собираться с использованием Qt 5.12.12. В репозиторий работы обязательно должен быть приложен *.pro файл, система сборки – qmake.

### Модификация
Добавить новый функционал, без которого работа не будет проверяться:
- Добавить кнопку "левша", меняющую местами действие ЛКМ и ПКМ.
- Добавить счётчик: "число мин" минус "число проставленных флажков".
- Добавить знак вопроса по нажатию ПКМ на флаг. “?” не считается в счётчике, но как и флаг, не позволяет по нажатию на ЛКМ открыть клетку.

### Задание
Создать приложение с пользовательским интерфейсом (GUI), позволяющее играть в игру Сапёр. [Правила игры](https://ru.wikipedia.org/wiki/Сапёр_(игра)).

Онлайн-игра: [Minesweeper](https://minesweeper.online/)

### Детали реализации
1. Мины расставляются после первой открытой клетки. Первая раскрытая клетка, таким образом, не может быть миной.
2. В случае открытия клетки, вокруг которой нет мин, все клетки до окружающих непустых включительно должны раскрываться автоматически.
3. Размер поля по умолчанию: 10 на 10.
4. Число мин по умолчанию: 10.

### Обработка нажатий
- Открытие клетки (ЛКМ).
- Установка/снятие флажка на клетке (по нажатию ПКМ).

### Дополнительные требования
- Использовать STL запрещено, вместо этого использовать классы Qt (QVector и т.д.).
- Пользовательский интерфейс описывается кодом (без использования .ui/QML). Использовать иконки/изображения не запрещено (даже приветствуется).

### Реализация
1. **Запуск новой игры с настройкой размеров поля и числа мин:**
    - Параметры должны быть с проверками на корректность введённых данных в появляющемся диалоговом окне.
    - После задания параметров новой игры должно появиться поле с закрытыми клетками, по которым можно нажимать, открывая их содержимое.
2. **Проигрыш:**
    - В случае попадания на мину всё поле раскрывается и появляется сообщение о проигрыше, поле становится неактивным.
    - Ячейка с последним нажатием должна визуально отличаться от других ячеек с минами.
3. **Выигрыш:**
    - Появляется сообщение о выигрыше.
4. **Режим отладки:**
    - При запуске с аргументом командной строки `dbg` (значение argv[1]) должна появиться кнопка/галочка, позволяющая “подглядывать” за скрытым состоянием поля.
5. **Интерфейс:**
    - Приложение должно позволять через пользовательский интерфейс (menu и toolbar) запускать новую игру с теми же параметрами или с заданием параметров.
6. **Сохранение состояния:**
    - При закрытии программы с активной игрой текущее состояние должно сохраняться в .ini файл рядом с исполняемым файлом и восстанавливаться из него при следующем запуске.

### Дополнительные баллы
- Реализация локализации: поддержка нескольких языков, возможность их переключения во время работы программы и автосохранение при выходе.

### Правильность поведения интерфейса
- Адекватная реакция на масштабирование окна и другие действия пользователя.

/** 
@mainpage Формирование элементарных графических изображений
В примере показано создание приложения, которое формирует 
элементарное графическое изображение с ипользованием некоторых 
возможностей библиотеки Qt - классов QPainter и QPainterPath.
@file main.cpp
@brief Запуск приложения.
@details Создается окно из класса Painter. 
Производится запуск цикла обработки сообщенией. 
*/
#include <QApplication>
#include <QTranslator>
#include "painter.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	// Устанавливаем язык для преобразовния строк, использующих функции 
	// tr() или translate().
	// В данном примере они содержаться в файле painter.cpp.
	// Внимание! Строки, которые будут преобразовываться через QTranslator 
	// должны содержать только латинские символы!
	// Файл ru.ts сгенерирован утилитой lupdate ../painter.cpp -ts ru.ts и
	// отредактирован в Qt Linguist
	// Файл ru.qm сформирован утилитой lrelease ru.ts
	QTranslator translator;
	//путь, начинающийся с :/ означает, что требуемый файл находится 
	// в файле-ресурсе приложения. Если оставить только translations/ru.qm, 
	// то приложение будет искать файл в директории	translations относительно 
	// текущей директории
	if( translator.load(":/translations/ru.qm") ) 
		qApp->installTranslator(&translator);

	Painter painter;
	painter.show();
	return app.exec();
}

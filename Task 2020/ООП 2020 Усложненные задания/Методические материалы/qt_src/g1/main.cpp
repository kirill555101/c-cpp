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
#include "painter.h"

int main(int argc, char *argv[])
{    
  QApplication app(argc, argv);
  Painter painter;
  painter.show();
  return app.exec();
}


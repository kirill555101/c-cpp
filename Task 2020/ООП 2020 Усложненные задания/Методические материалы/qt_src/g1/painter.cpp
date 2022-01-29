/** 
  @file painter.cpp
  @brief Реализация классов программы.
*/
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include "painter.h"

/// @brief Конструктор главного окна
/// @details Устанавливает режим выравнивания элементов и создает их.
Painter::Painter()
{
  QVBoxLayout *layout = new QVBoxLayout;

  m_RenderArea = new RenderArea( this );
  QPushButton * btn = new QPushButton( "Close", this );
  QObject::connect(btn, SIGNAL(clicked(bool)), 
    this, SLOT(close()));  

  layout->addWidget( m_RenderArea );
  layout->addWidget( btn );	  

  setLayout(layout);
};


/// @brief Конструктор области отображения
/// @param parent - Если значение NULL, то область отображения станет
///        самостоятельным окном. В противном случае - виджетом-потомком.
/// @details Именно в этом методе формируется набор графических примитивов,
///          которые будут отображены при перерисовке.
RenderArea::RenderArea(QWidget *parent)
: QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  // Формируем прямоугольник
  m_Path.moveTo(20.0, 30.0);
  m_Path.lineTo(80.0, 30.0);
  m_Path.lineTo(80.0, 70.0);
  m_Path.lineTo(20.0, 70.0);
  // Замыкаем контур - проводим линию от последней точки к первой
  m_Path.closeSubpath();

  // Формируем сектор круга (в англ. - pie - кусок пирога)
  m_Path.moveTo(50.0, 50.0);
  m_Path.arcTo(20.0, 30.0, 60.0, 40.0, 60.0, 240.0);
  // Замыкаем контур
  m_Path.closeSubpath();
}

QSize RenderArea::minimumSizeHint() const
{
  return QSize(50, 50);
}
QSize RenderArea::sizeHint() const
{
  return QSize(100, 100);
}

void RenderArea::paintEvent(QPaintEvent *)
{
  // Готовим объект отображения
  QPainter painter(this);
  
  // Определяем режим масштабирования со сглаживанием 
  painter.setRenderHint(QPainter::Antialiasing);
  
  // Определяем масштаб в зависимости от текущих геометрических размеров 
  // области отображения
  painter.scale(width() / 100.0, height() / 100.0);

  // Создаем перо для отображения контуров. Используем красный цвет и толщину 
  // линий равную 2.
  painter.setPen(
    QPen(QColor(255,0,0), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

  // Отображаем контуры
  painter.drawPath( m_Path );

  // Нарисуем дополнительную вертикальную  линию
  painter.setPen(
    QPen(QColor(0,127,0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawLine(50.0, 0.0, 50.0, 100.0);
}

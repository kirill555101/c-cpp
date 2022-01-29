/** 
@file painter.cpp
@brief Реализация классов программы.
*/
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QLineEdit>
#include <QSpacerItem>
#include <QImage>
#include <QLabel>
#include "painter.h"

#define _USE_MATH_DEFINES
#include "math.h"

// Размер поля отображения
const float _x_aspect=400;
const float _y_aspect=400;

/// @brief Конструктор главного окна
/// @details Устанавливает режим выравнивания элементов и создает их.
Painter::Painter()
{
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *hlayout = new QHBoxLayout;

	// Создаём панели для ввода кривой и вывода тела вращения
	m_InputArea = new InputImage( this);
	m_RenderArea = new RenderArea( this );

	// формируем отдельную панель для органов управления
	QFrame * controls = new QFrame();
	QVBoxLayout *controllayout = new QVBoxLayout;
	controls->setLayout( controllayout );
	controls->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding);

	m_Exy=new QLineEdit( "30", this); 
	m_Exz=new QLineEdit( "0", this);
	m_Eyz=new QLineEdit( "50", this);
	QPushButton * btn_convert = new QPushButton( tr("Convert") );
	QPushButton * btn_clear = new QPushButton( tr("Clear") );

	QObject::connect(btn_clear, SIGNAL(clicked(bool)), 
		m_InputArea, SLOT(clearImage()));  

	QObject::connect(btn_convert, SIGNAL(clicked(bool)), 
		this, SLOT(convert()));

	// наполняем панель с органами управления
	controllayout->addWidget( new QLabel(tr("xy step")) );
	controllayout->addWidget( m_Exy );
	controllayout->addWidget( new QLabel(tr("xz angle")) );
	controllayout->addWidget( m_Exz );
	controllayout->addWidget( new QLabel(tr("yz angle")) );
	controllayout->addWidget( m_Eyz );
	controllayout->addWidget( btn_convert );
	controllayout->addItem( new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding) );
	controllayout->addWidget( btn_clear );

	// устанавливаем порядок отображения панелей
	hlayout->addWidget( m_InputArea );	
	hlayout->addWidget( controls );
	hlayout->addWidget( m_RenderArea );

	QPushButton * btn = new QPushButton( tr("Close"), this );
	QObject::connect(btn, SIGNAL(clicked(bool)), 
		this, SLOT(close()));  

	// формируем окончательный вид, добавляем кнопку Close
	layout->addLayout(hlayout);
	layout->addWidget( btn );	  

	setLayout(layout);
};

/// @details Сканирует панель ввода, преобразует введенные значения углов,
///          запускает формирование тела вращения
void Painter::convert()
{
	double xy_step_angle, yz_angle, xz_angle;
	QVector<BaseRenderArea::PointCoords> data;

	xy_step_angle =  m_Exy->text().toDouble()*M_PI/180.0;
	yz_angle =  m_Eyz->text().toDouble()*M_PI/180.0;
	xz_angle =  m_Exz->text().toDouble()*M_PI/180.0;

	m_InputArea->getPoints(data);
	m_RenderArea->setPoints(data, xy_step_angle, yz_angle, xz_angle);
};

/// @details Конструктор базового класса панелей ввода и отображения.
///          Устанавливает формат рамки и инициализирует общие переменные.
/// @param parent - Если значение NULL, то область отображения станет
///        самостоятельным окном. В противном случае - виджетом-потомком.
BaseRenderArea::BaseRenderArea(QWidget *parent):QFrame(parent)
{
	// формируем рамку вокруг виджета
	setFrameStyle(QFrame::Panel | QFrame::Raised);
	setLineWidth(2);
	//Установим перо по умолчанию, равное 2
	m_PenWidth=2;
};

// Раскомментировать, если необходимо сохранять отношение сторон 
// при изменении размеров окна
//void BaseRenderArea::resizeEvent(QResizeEvent *event)
//{
//	QSize size=event->size();
//	if (size.width() > size.height() ) {
//		size.setWidth(size.height());
//	} else if (size.width() < size.height() ) {
//		size.setHeight(size.width());
//	}
//	resize(size);
//}

QSize BaseRenderArea::minimumSizeHint() const
{
	return QSize(100, 100);
}
QSize BaseRenderArea::sizeHint() const
{
	return QSize(_x_aspect, _y_aspect);
}

void BaseRenderArea::paintEvent(QPaintEvent *event)
{
	// Готовим объект отображения
	QPainter painter(this);

	// Определяем режим отображения со сглаживанием 
	painter.setRenderHint(QPainter::Antialiasing);

	// Определяем масштаб в зависимости от текущих геометрических размеров 
	// области отображения
	painter.scale( width()/_x_aspect, height()/_y_aspect );

	// Создаем перо для отображения контуров. Используем красный цвет и толщину 
	// линий равную m_PenWidth.
	painter.setPen(
		QPen(QColor(255,0,0), m_PenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	// Отображаем контуры
	painter.drawPath( m_Path );

	QFrame::paintEvent(event); // рисуем рамку вокруг панели вызовом предка
}

/// @brief Конструктор области отображения
RenderArea::RenderArea(QWidget *parent)
: BaseRenderArea(parent)
{
	setBackgroundRole(QPalette::Base);
	QFont font("Arial", 12);
	m_Path.addText( 10,_y_aspect/2, font, tr("Draw on the left panel"));
	m_PenWidth=0.5;
}

/// @param data - массив точек кривой
/// @param xy_step_angle - шаг поворота при формировании тела вращения в градусах
/// @param yz_angle - угол поворота в градусах плоскости yz
/// @param xz_angle - угол поворота в градусах плоскости xz
void RenderArea::setPoints( const QVector<PointCoords> & data, 
						   double xy_step_angle, double yz_angle, double xz_angle )
{
	m_Path=QPainterPath();	// очищаем область вывода

	if(!data.size())return; // Если на входе ничего нет - выходим

	double phi;
	double x,y,z;
	double xw, yw;

	// для ускорения работы программы, синусы и косинусы
	// вычисляем заранее
	double cos_yz=cos(yz_angle), sin_yz=sin(yz_angle);
	double cos_xz=cos(xz_angle), sin_xz=sin(xz_angle);	

	// цикл, формирующий тело вращения из линии
	for( phi=M_PI/24; phi<2*M_PI+M_PI/24; phi+=xy_step_angle){
		const PointCoords  *point;
		double sin_r1=sin(phi), cos_r1=cos(phi);
		for(int i=0; i<data.size();i++){
			point=&data[i];

			// переназначаем координаты.
			z=point->x;
			x=point->y;
			// смотрим вдоль z.  Поворачиваем плоскость x-y. 
			// Поскольку y=0, вычисления относительно y отбрасываем
			xw=x*cos_r1; //-y*sin_fi;
			y=x*sin_r1; //+y*cos_fi;
			x=xw;

			// Поворачиваем в плоскости y-z
			yw=y*cos_yz-z*sin_yz;
			z=y*sin_yz+z*cos_yz; 
			y=yw;

			// Поворачиваем в плоскости x-z
			xw=x*cos_xz-z*sin_xz;
			//z=x*sin_fi+z*cos_fi;// z - дальше не используется
			x=xw;

			//проецируем на экран, отбросив z
			//Смещаем в середину панели и масштабируем с коэффициентом 0.5
			xw=_x_aspect/2+x*0.5;
			yw=_y_aspect/2+y*0.5;

			if( !i ) m_Path.moveTo(xw,yw);
			else m_Path.lineTo(xw,yw);
		}		
	}
	update();
};

InputImage::InputImage(QWidget *parent)
:BaseRenderArea(parent)
{
};

/// @details Преобразует координаты мыши в локальные координаты виджета
QPoint InputImage::from(const QPoint &point){
	
	QPoint pos = mapFrom( this, point );
	return QPoint( pos.x()* _x_aspect/ width(), pos.y()*_y_aspect/ height() );
};

void InputImage::mousePressEvent ( QMouseEvent * event )
{
	//Если нажата левая кнопка мыши - рисуем
	if (event->buttons () & Qt::LeftButton){
		m_Path.moveTo( from(event->pos()) );
	}
}
void InputImage::mouseMoveEvent ( QMouseEvent * event )
{
	if (event->buttons () & Qt::LeftButton){
		m_Path.lineTo( from(event->pos()) );
	}
	// принудительно отрисоваваем. Внимание! реализация упрощенная.
	// На каждое движение мыши будет отрисоваваться всё, что было введено ранее
	update(); 
};

void InputImage::clearImage()
{
	m_Path=QPainterPath(); // Создаём пустой объект. 
	update();// Очищаем изображение.
};

void InputImage::getPoints( QVector<PointCoords> & data )
{
	data.clear(); // очищаем массив на случай, если там что-то было
	QColor color(255,0,0); QRgb rgb_color=color.rgb();
	// формируем устройство отображения
	QImage image( _x_aspect,_y_aspect, QImage::Format_RGB32 );
	QPainter painter(&image);
	painter.setPen( QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	//выводим сохраненный рисунок
	painter.drawPath( m_Path );

	//сканируем столбцы изображения в матрице пикселов
	for(int x=0; x<_x_aspect; x++){
		// двигаемся снизу изображения
		for(int y=image.height(); y>=0; y--){
			QRgb rgb=image.pixel(x,y);
			if( rgb==rgb_color ){
				//запоминаем первую точку столбца с соответствующим цветом				
				data.push_back( PointCoords(x,image.height()-y));
				break;
			}
		}
	}
};

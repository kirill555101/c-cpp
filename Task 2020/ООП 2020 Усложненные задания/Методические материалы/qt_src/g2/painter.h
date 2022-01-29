/** 
@file painter.h
@brief Декларация классов программы.
*/
#include <QWidget>
#include <QVector>
#include <QFrame>

class RenderArea;
class InputImage;
class QLineEdit;
/**
@class Painter
@brief Главное окно приложения.
@details Обеспечивает формирование окна приложения, содержащего 
область отображения рисунка и кнопку выхода
*/
class Painter : public QWidget
{
	Q_OBJECT

public:
	Painter();

protected:
	RenderArea * m_RenderArea; ///< Область отображения рисунка.
	InputImage * m_InputArea;  ///< Область рисования кривой для вращения.
	QLineEdit * m_Exy;         ///< Шаг поворота при формирвоании тела вращения.
	QLineEdit * m_Eyz;         ///< Угол поворота в градусах плоскости yz
	QLineEdit * m_Exz;         ///< Угол поворота в градусах плоскости xz

	public slots:
		void convert();
};

/// @class BaseRenderArea
/// @brief Базовый класс для панелей ввода и отображения.
class BaseRenderArea: public QFrame
{
	Q_OBJECT

public:  
	///@brief Структура для хранения координат в массиве
	struct PointCoords {
		int x;
		int y;
		PointCoords(){ x=y=0;};
		PointCoords(int x1, int y1){
			x=x1; y=y1;
		};
	};
	BaseRenderArea(QWidget *parent = 0);

	/// @brief Минимальный размер области отображения 
	/// @details Изменение размеров окна приложения производится с учетом 
	/// допустипых размеров всех элементов. Функция обеспечивает корректное 
	/// отображение области графических примитивов
	/// @return Допустимые минимальные размеры
	QSize minimumSizeHint() const;

	/// @brief Размер области отображения по умолчанию
	/// @details При создании окна автоматически рассчитываются размеры 
	/// по указанным рекомендованным размерам всех элементов.
	/// @return рекомендованные размеры
	QSize sizeHint() const;

	/// @brief Формируте тело вращения по указанному массиву координат
	void setPoints( const QVector<PointCoords> &, 
		double xy_step_angle, double yz_angle, double xz_angle );

protected:	
	float m_PenWidth; ///< Ширина пера отображения по умолчанию.

	/// @brief Обработчик события перерисовки  
	virtual void paintEvent(QPaintEvent * event);

	QPainterPath m_Path; ///< Шаблон отображения графических примитивов.

	//	void resizeEvent(QResizeEvent *event);	
};

/**
@class RenderArea
@brief Область отображения тела вращения.
@details Формирует и отображает рисунок
*/
class RenderArea : public BaseRenderArea
{
	Q_OBJECT

public:  
	RenderArea(QWidget *parent = 0);

	/// @brief Метод для формирования тела вращения по массиву значений
	void setPoints( const QVector<PointCoords> &, 
		double xy_step_angle, double yz_angle, double xz_angle );
};

/**
@class InputImage
@brief Область формирования кривой.
@details Обеспечивает формирование, отрисовку кривой.
*/
class InputImage : public BaseRenderArea
{
	Q_OBJECT

public:
	InputImage(QWidget *parent = 0);

	/// @brief Метод для сканирования введенного изображения
	void getPoints( QVector<PointCoords> & );

	public slots:
		/// @brief Очистка изображения
		void clearImage();

protected:
	/// @brief Обработчик события нажатия клавиши мыши
	virtual void mousePressEvent ( QMouseEvent * event );
	/// @brief Обработчик события перемещения мыши
	virtual void mouseMoveEvent ( QMouseEvent * event );

	/// @brief Вспомогательный метод преобразования координат
	QPoint from(const QPoint&);
};

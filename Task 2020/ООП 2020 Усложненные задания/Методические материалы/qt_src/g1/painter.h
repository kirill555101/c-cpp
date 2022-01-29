/** 
  @file painter.h
  @brief Декларация классов программы.
*/
#include <QWidget>

class RenderArea;
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
};

/**
  @class RenderArea
  @brief Область отображения рисунка.
  @details Формирует, запоминает и отображает набор графических примитивов
*/
class RenderArea : public QWidget
{
	Q_OBJECT

public:  
	RenderArea(QWidget *parent = 0);

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

protected:
  /// @brief Обработчик события перерисовки  
	virtual void paintEvent(QPaintEvent * event);
private:
	QPainterPath m_Path; ///< Шаблон отображения графических примитивов.
};

#include <QtCore/QCoreApplication>
#include <QSet>
#include <QTextStream>
#include <QTime>

const int MAX_WORDS = 20;
const int MAX_CHARS = 2;
//*****************************************************************************
// Пример использования QSet с типом на основе стандартного

/// класс, нечувствительный к регистру
class InsensitiveString : public QString
{
public:
	/// Функция сравнения. Специцикаторы const являются обязательными!
	/// Используется только в том случае, если хэш-значения совпали, 
	/// поэтому сравнивается равенство значений.
	bool operator== ( const InsensitiveString & other ) const
	{
		return this->toLower() == other.toLower();
	};
};
/// Определяем хэш-функцию для типа InsensitiveString
uint qHash( const InsensitiveString & var )
{    // приводим к нижнему регистру и к стандартной qHash(const QString&)
	return qHash( var.toLower());
};
void test_qset()
{
	// используем стандартный поток вывода для вывода текста в консоль
	QTextStream out(stdout);
	out<<"********************"<<__FUNCTION__<<"********************"<<endl;
	// объявляем множество
	QSet <InsensitiveString> st;
	InsensitiveString str;
	// заносим случайно сгенерированные значения
	for( int i=0; i<MAX_WORDS; i++ ){
		str.clear();
		for( int j=0; j<MAX_CHARS; j++ )
			str.append( QChar( (qrand()%('z'-'A')) + 'A' ) );
		st.insert( str );
	};
	// формируем новые строки и проверяем, входят ли 
	for( int i=0; i<MAX_WORDS; i++ ){
		str.clear();
		for( int j=0; j<MAX_CHARS; j++ )
			str.append( QChar( (qrand()%('z'-'A')) + 'A' ) );

		// проверяем, есть ли уже такие значения во множестве
		// и выводим результат
		if(st.find( str ) != st.end() ){		
			out<<"Val " << str << " exist" << endl;
		} else
			out<<"Val " << str << " not exist" << endl;
	};
	// выводим все элементы множества. Обратите внимание, что элементы
	// не сортированы !!!
	QSetIterator<InsensitiveString> i(st);
	while (i.hasNext())
		out << i.next() << endl;
};
//*****************************************************************************
// Пример использования QSet с новым типом
#include <qHash>
/// класс, определяющий новый тип
class RawData
{
public:
	/// конструктор по умолчанию инициализирует случайным образом значение
	RawData(){ m_val = qrand()%10 - 5; };

	/// функция получения значения. Спецификатор const здесь необходим!
	int val() const { return m_val;};

	/// Фунция сравнения. Специцикаторы const являются обязательными!
	/// Используется только в том случае, если хэш-значения совпали, 
	/// поэтому сравнивается равенство значений.
	bool operator== ( const RawData & other ) const
	{
		return this->m_val == other.m_val;
	};
protected:
	int m_val;
};

/// Определяем новую хэш-функцию, позволяющую работать с новым типом
uint qHash( const RawData & var )
{
	// поскольку значение простое, воспользуемся стандартной хэш-функцией
    // для целого типа
	return qHash( var.val() );
};
void test_qset2()
{
	// используем стандартный поток вывода для вывода текста в консоль
	QTextStream out(stdout);
	out<<"********************"<<__FUNCTION__<<"********************"<<endl;
	
	// определяем множество и заполняем его
	QSet <RawData> st;	
	for( int i=0; i<MAX_WORDS; i++ ){
		RawData data;
		st.insert( data );
	};

    // создаем новые элементы и проверяем, есть ли такие же
	for( int i=0; i<MAX_WORDS; i++ ){
		RawData data;
		if(st.find( data ) != st.end() ){
			out<<"Val " << data.val() << " exist" << endl;			
		} else
			out<<"Val " << data.val() << " not exist" << endl;
	};

	// выводим все элементы множества.
	// Значения не сортированы!!!
	QSetIterator<RawData> i(st);
	while (i.hasNext())
		out << i.next().val() << endl;
};




//*****************************************************************************
// Пример использования QMap с типом на основе стандартного
#include <QMap>
/// класс, нечувствительный к регистру
class InsensitiveString2 : public QString
{
public:
	// переопределяем функцию сравнения. Поскольку будем использовать QMap,
	// которая использует дерево, необходимо переопределить operator<
	bool	operator< ( const InsensitiveString2 & other ) const
	{
		return this->toLower() < other.toLower();
	};
};
void test_qmap()
{
	// используем стандартный поток вывода для вывода текста в консоль
	QTextStream out(stdout);
	out<<"********************"<<__FUNCTION__<<"********************"<<endl;
	
	// объявляем map, имеющий ключ типа InsensitiveString2 и фиктивное значение
	QMap <InsensitiveString2, int> st;
	InsensitiveString2 str;
    // заносим случайно сгенерированные ключи с одинаковыми значениями
	for( int i=0; i<MAX_WORDS; i++ ){
		str.clear();
		for( int j=0; j<MAX_CHARS; j++ )
			str.append( QChar( (qrand()%('z'-'A')) + 'A' ) );
		st.insert( str, 0 );
	};
	// формируем новые строки и проверяем, входят ли 
	for( int i=0; i<MAX_WORDS; i++ ){
		str.clear();
		for( int j=0; j<MAX_CHARS; j++ )
			str.append( QChar( (qrand()%('z'-'A')) + 'A' ) );

		// проверяем, есть ли уже такие значения во множестве
		// и выводим результат
		if(st.find( str ) != st.end() ){
			out<<"Val " << str << " exist" << endl;
		} else
			out<<"Val " << str << " not exist" << endl;
	};

	// выводим все элементы map. 
	// Здесь все элементы выводятся в порядке сортировки 
	// (по возрастанию, игнорируя регистр)
	QMapIterator <InsensitiveString2, int> iter(st);
	while (iter.hasNext()){
		iter.next();
		out << iter.key() << endl;
	};
};
//*****************************************************************************
// Пример использования QMap с новым типом

/// класс, определяющий новый тип
class RawData2
{
public:
	/// конструктор по умолчанию инициализирует случайным образом значение
	RawData2(){ m_val = qrand()%10 - 5; };	
	/// функция получения значения. Спецификатор const здесь необходим!
	int val() const { return m_val;};

	/// Фунция сравнения. Специцикаторы const являются обязательными!
	/// Поскольку используем QMap, то operator<
	bool	operator< ( const RawData2 & other ) const
	{
		return this->m_val < other.m_val;
	};
protected:
	int m_val;
};
void test_qmap2()
{
	// используем стандартный поток вывода для вывода текста в консоль
	QTextStream out(stdout);
	out<<"********************"<<__FUNCTION__<<"********************"<<endl;
	
	// объявляем map, имеющий ключ типа InsensitiveString2, int
	// и заполняем парами ключ-фиктивное значение (не используемое)
	QMap <RawData2, int> st;	
	for( int i=0; i<MAX_WORDS; i++ ){
		RawData2 data;
		st.insert( data,0 );
	};

	// создаем новые элементы и проверяем, есть ли такие же
	for( int i=0; i<MAX_WORDS; i++ ){
		RawData2 data;

		// проверяем, есть ли уже такие значения во множестве
		// и выводим результат
		if(st.find( data ) != st.end() ){
			out<<"Val " << data.val() << " exist" << endl;			
		} else
			out<<"Val " << data.val() << " not exist" << endl;
	};

	// выводим все элементы map. 
	// Здесь все элементы выводятся в порядке сортировки 
	QMapIterator <RawData2, int> iter(st);
	while (iter.hasNext()){
		iter.next();
		out << iter.key().val() << endl;
	};
};
//*****************************************************************************

int main(int argc, char *argv[])
{
	// инициализируем генератор псевдослучайных чисел текущим временем
	QTime time;
	qsrand(time.secsTo(QTime::currentTime()));

	test_qset();
	test_qset2();
	test_qmap();
	test_qmap2();

	// ждем, пока пользователь нажмет Enter
	QTextStream(stdin).readLine();
	return 0;
}

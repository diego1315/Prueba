/* Clase que define un tipo de dato abstracto Lista */

template <typename E>
class List
{
private:
	/* Definición del constructor de copia */
	List(const List<E>& other) {};
	void operator = (const List<E>& other) {};
public:
	/* Definición del constructor y el destructor */
	List() {};
	virtual ~List() {};

	/* Definición de los métodos de la clase */
	virtual void insert(E element) = 0;
	virtual void append(E element) = 0;
	virtual E remove() = 0;
	virtual E getElement() = 0;
	virtual void clear() = 0;
	virtual void goToStart() = 0;
	virtual void goToEnd() = 0;
	virtual void goToPos(int pos) = 0;
	virtual void previous() = 0;
	virtual void next() = 0;
	virtual bool atStart() = 0;
	virtual bool atEnd() = 0;
	virtual int getPos() = 0;
	virtual int getSize() = 0;
	virtual int indexOf(E element, int start) = 0;
	virtual bool contains(E element) = 0;
	virtual void print() = 0;
};


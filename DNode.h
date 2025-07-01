/* Clase que implementa un nodo doblemente enlazado */

template <typename E>
class DNode
{
public:
	/* Atributos de la clase */
	E element;
	DNode<E>* next;
	DNode<E>* previous;

	/* Constructores de la clase */
	DNode(E element, DNode<E>* next, DNode<E>* previous) 
	{
		this->element = element;
		this->next = next;
		this->previous = previous;
	}

	DNode(DNode<E>* next, DNode<E>* previous) 
	{
		this->next = next;
		this->previous = previous;
	}
};

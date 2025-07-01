#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include "List.h"
#include "DNode.h"

using std::runtime_error;
using std::cout;
using std::cin;
using std::endl;

/* Clase que implementa una estructura de datos Lista como lista doblemente enlazada */

template <typename E>
class DLinkedList :
    public List<E>
{
/* Atributos de la clase */
private:
    DNode<E>* head;
    DNode<E>* tail;
    DNode<E>* current;
    int size;
/* Métodos de la clase */
public:
    /* Constructor */
    DLinkedList()
    {
        current = head = new DNode<E>(nullptr, nullptr);
        head->next = tail = new DNode<E>(nullptr, head);
        size = 0;
    }

    /* Destructor */
    ~DLinkedList()
    {
        clear();
        delete head;
        delete tail;
    }

    /* Métodos comunes de las listas como estructura de datos */
    void insert(E element)
    {
        current->next = current->next->previous = new DNode<E>(element, current->next, current);
        size++;
    }

    void append(E element)
    {
        tail->previous = tail->previous->next = new DNode<E>(element, tail, tail->previous);
        size++;
    }

    E remove()
    {
        if (current->next == tail)
            throw runtime_error("No current element.");
        E result = current->next->element;
        current->next->next->previous = current;
        current->next = current->next->next;
        size--;
        return result;
    }

    void clear()
    {
        while (head->next != tail)
        {
            head->next = head->next->next;
            delete head->next->previous;
        }
        current = tail->previous = head;
        size = 0;
    }

    E getElement()
    {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");

        return current->next->element;
    }

    void goToStart()
    {
        current = head;
    }

    void goToEnd()
    {
        current = tail->previous;
    }

    void goToPos(int pos)
    {
        if (pos < 0 || pos > size)
            throw runtime_error("Index out of bounds.");

        if (pos <= size / 2)
        {
            current = head;
            for (int i = 0; i < pos; i++)
                current = current->next;
        }
        else
        {
            current = tail->previous;
            for (int i = 0; i < size - pos; i++)
                current = current->previous;
        }
    }

    void next()
    {
        if (current->next != tail)
            current = current->next;
    }

    void previous()
    {
        if (current != head)
            current = current->previous;
    }

    bool atStart()
    {
        return current == head;
    }

    bool atEnd()
    {
        return current == tail->previous;
    }

    int getPos()
    {
        int pos = 0;
        DNode<E>* temp = head;
        while (temp != current)
        {
            temp = temp->next;
            pos++;
        }
        return pos;
    }

    int getSize()
    {
        return size;
    }

    int indexOf(E element, int start)
    {
        if (start < 0 || start > size)
            throw runtime_error("Start is out of bounds.");

        int currentPos = getPos();
        goToPos(start);
        DNode<E>* temp = current;

        int i = -1;
        while (temp != tail)
        {
            if (temp->element == element)
            {
                goToPos(currentPos);
                return i + start;
            }

            temp = temp->next;
            i++;
        }

        goToPos(currentPos);
        return -1;
    }


    bool contains(E element)
    {
        DNode<E>* temp = head;

        while (temp != tail)
        {
            if (temp->element == element)
            {
                return 1;
            }
            temp = temp->next;
        }
        return 0;
    }

    void print()
    {
        cout << "[ ";
        DNode<E>* temp = head->next;
        while (temp != tail)
        {
            if (temp == current->next)
                cout << "*";
            cout << temp->element << " ";
            temp = temp->next;
        }
        if (current == tail->previous)
            cout << "*";
        cout << "]" << endl;
    }
};
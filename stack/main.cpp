#include <iostream>
using namespace std;

template <typename T>
struct node_stack
{
    T item;
    node_stack<T>* next;
};

template <typename T>
class myStack
{
private:
    node_stack<T>* pTop;

public:
    
    myStack() 
    { 
        pTop = nullptr;
    } 

    void Push(T item)
    {
        node_stack<T>* p;
        p = new node_stack<T>; 
        p->item = item;
        p->next = pTop;
        pTop = p;
    }

    int Size()
    {
        int count = 0;
        if (pTop == nullptr)
            return 0;
        else
        {
            node_stack<T>* p = pTop;
            while (p != nullptr)
            {
                count++;
                p = p->next;
            }
        }
        return count;
    }

    void Print()
    {
        if (pTop == nullptr)
            cout << "there is nothing in this stack" << endl;
        else
        {
            node_stack<T>* p;
            p = pTop;
            while (p != nullptr)
            {
                cout << p->item << ' ';
                p = p->next;
            }
            cout << endl;
        }
    }

    void Empty()
    {
        node_stack<T>* p;
        node_stack<T>* p2;
        p = pTop;
        while (p != nullptr)
        {
            p2 = p;
            p = p->next;
            delete p2;
        }
        pTop = nullptr;
    }


    ~myStack()
    {
        Empty();
    }

    T Pop()
    {
        if (pTop == nullptr)
        {
            return 0;
        }
        node_stack<T>* p2;
        T item;
        item = pTop->item;
        p2 = pTop;
        pTop = pTop->next;
        delete p2;
        return item; // для проверки
    }
    
    T Top()
    {
        return pTop->item;
    }
};

void main()
{
    myStack<float> stack_ex;
    stack_ex.Print();
    int b = 0;
    stack_ex.Push(b);
    stack_ex.Push(3.0);
    stack_ex.Push(7.1);
    stack_ex.Push(8.6);
    stack_ex.Push(11.4);
    stack_ex.Push(14.5);
    cout << stack_ex.Top() << endl;
    stack_ex.Print();
    stack_ex.Pop();
    stack_ex.Print();
    stack_ex.Empty();
    stack_ex.Print();
    return;
}
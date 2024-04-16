#include "stack.h"

Stack::Stack():m_top(nullptr), m_count(0){}

Stack::~Stack()
{
    while(!IsEmpty())
    {
        Pop();
    }   
}

void Stack::Push(DataType data){
    Node *newNode = new Node(data,m_top);
    m_top = newNode;
    ++m_count;
}
void Stack::Pop(){
    if(!IsEmpty()){
        Node *tmp = m_top;
        m_top = m_top->m_next;
        delete tmp;
        --m_count;
    }
}

DataType Stack::Top() const{
    return m_top->m_data;
}

size_t Stack::Count()const{
    return m_count;
}

bool Stack::IsEmpty()const{
    return m_count == 0;
}

Stack::Node::Node(DataType data, Node *next) : m_data(data), m_next(next){}

DataType Stack::Node::GetData() const {
    return m_data;
}

bool Stack::Node::HasNext() const {
    return m_next != nullptr;
}

const Stack::Node& Stack::Node::GetNext() const {
    return *m_next;
}
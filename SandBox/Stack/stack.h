#ifndef STACK_H
#define STACK_H

#include <iostream>

typedef int DataType;

struct Stack
{
public:
  explicit Stack();
  ~Stack();

  void Push(DataType data);
  void Pop();
  DataType Top() const;
  size_t Count()const;
  bool IsEmpty()const;

  struct Node
  {
    explicit Node();
    explicit Node(DataType data, Node* next);

    DataType GetData() const;
    bool HasNext() const;
    const Node &GetNext() const;

    DataType m_data;
    Node* m_next;
  };

  Node *m_top;
  size_t m_count;
};

#endif
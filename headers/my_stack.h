#ifndef _MY_CONTAINER_STACK_H
#define _MY_CONTAINER_STACK_H

#include <iostream>

namespace my {

template <typename T>
class stack {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  stack();
  stack(std::initializer_list<value_type> const &items);
  stack(const stack &s);
  stack(stack &&s) noexcept;
  ~stack();
  stack<value_type> &operator=(stack &&s) noexcept;

  const_reference top() const;

  bool empty() const;
  size_type size() const;

  void push(const_reference value);
  void pop();
  void swap(stack &other);

  template <typename... Args>
  void insert_many_front(Args &&...args);

 private:
  struct node {
    node *next;
    value_type val;
    node() : next(nullptr) {}
    node(value_type val) : next(nullptr), val(val) {}
    node(node *next, value_type val) : next(next), val(val) {}
  };

 private:
  node *tail = nullptr;
  size_type count = 0;
};

template <typename T>
inline stack<T>::stack() : tail(nullptr), count(0) {}

template <typename T>
inline stack<T>::stack(std::initializer_list<value_type> const &items) {
  for (auto it = items.begin(); it != items.end(); it++) push(*it);
}

template <typename T>
stack<T>::stack(const stack &s) {
  if (this != &s) {
    count = s.count;
    if (s.tail) {
      tail = new node(s.tail->val);
      node *cur = tail;
      node *cur_s = s.tail->next;

      while (cur_s) {
        cur->next = new node(cur_s->val);
        cur = cur->next;
        cur_s = cur_s->next;
      }
    }
  }
}

template <typename T>
inline stack<T>::stack(stack &&s) noexcept {
  if (this != &s) {
    tail = s.tail;
    count = s.count;
    s.tail = nullptr;
    s.count = 0;
  }
}

template <typename T>
stack<T>::~stack() {
  if (tail != nullptr) {
    if (tail->next == nullptr)
      delete tail;
    else {
      node *cur = tail;
      node *next = tail->next;
      while (next) {
        delete cur;
        cur = next;
        next = next->next;
      }

      delete cur;
    }
  }
}

template <typename T>
inline stack<T> &stack<T>::operator=(stack &&s) noexcept {
  *this = std::move(s);

  return *this;
}

template <typename T>
inline typename stack<T>::const_reference stack<T>::top() const {
  if (!tail) throw std::runtime_error("Error: Stack is empty");
  return tail->val;
}

template <typename T>
inline bool stack<T>::empty() const {
  return tail == nullptr;
}

template <typename T>
inline typename stack<T>::size_type stack<T>::size() const {
  return count;
}

template <typename T>
inline void stack<T>::push(const_reference value) {
  tail = new node(tail, value);
  count++;
}

template <typename T>
inline void stack<T>::pop() {
  if (tail) {
    node *tmp = tail;
    tail = tmp->next;
    delete tmp;
    count--;
  }
}

template <typename T>
inline void stack<T>::swap(stack &other) {
  std::swap(count, other.count);
  std::swap(tail, other.tail);
}

template <typename T>
template <typename... Args>
void stack<T>::insert_many_front(Args &&...args) {
  size_type count = sizeof...(Args);
  if (count == 0) return;
  T tmp[count] = {std::forward<Args>(args)...};
  for (size_type i = 0; i < count; i++) {
    push(tmp[i]);
  }
}

}  // namespace my

#endif  // !_MY_CONTAINER_STACK_H

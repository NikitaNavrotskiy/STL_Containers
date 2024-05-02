#include <iostream>

namespace my {
template <typename T>
class Queue {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  Queue() : head(nullptr), tail(nullptr), size_(0) {}
  Queue(std::initializer_list<value_type> const &items) {
    head = tail = nullptr;
    size_ = 0;

    for (const auto &item : items) {
      push(item);
    }
  }
  Queue(const Queue &q);
  Queue(Queue &&q) {
    head = q.head;
    tail = q.tail;
    size_ = q.size_;

    q.head = nullptr;
    q.tail = nullptr;
    q.size_ = 0;
  }

  ~Queue() { clear(); }

  Queue &operator=(Queue &&q) noexcept;

  const_reference front() { return head->value_; }
  const_reference back() { return tail->value_; }

  bool empty() { return size_ == 0; }
  size_type size() { return size_; }

  template <typename... Args>
  void insert_many_back(Args &&...args);

  void push(const_reference value);
  void pop();
  void swap(Queue &other);

 private:
  struct Node {
    value_type value_;
    Node *next_;
    explicit Node(value_type value) : value_(value), next_(nullptr) {}
  };

  Node *head;
  Node *tail;
  size_type size_;

 private:
  void clear();
};

template <typename T>
Queue<T>::Queue(const Queue &q) {
  tail = head = nullptr;
  size_ = 0;
  for (const Node *cur = q.head; cur != nullptr; cur = cur->next_) {
    push(cur->value_);
  }
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue &&q) noexcept {
  if (this != &q) {
    clear();
    head = q.head;
    tail = q.tail;
    size_ = q.size_;

    q.head = nullptr;
    q.tail = nullptr;
    q.size_ = 0;
  }
  return *this;
}
template <typename T>
void Queue<T>::clear() {
  while (head != nullptr) pop();
}

template <typename T>
template <typename... Args>
void Queue<T>::insert_many_back(Args &&...args) {
  size_type count = sizeof...(Args);
  if (count == 0) return;
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};

  for (size_type i = 0; i < count; i++) {
    push(tmp[i]);
  }
}

template <typename T>
void Queue<T>::push(const_reference value) {
  Node *ptr = new Node(value);
  if (tail != nullptr) {
    tail->next_ = ptr;
  }
  if (head == nullptr) {
    head = ptr;
  }
  tail = ptr;
  size_++;
}

template <typename T>
void Queue<T>::pop() {
  if (head == nullptr) return;
  Node *tmp = head->next_;
  head->next_ = nullptr;
  delete head;
  head = tmp;
  size_--;
}

template <typename T>
void Queue<T>::swap(Queue &other) {
  Node *tmpHead = head;
  head = other.head;
  other.head = tmpHead;

  Node *tmpTail = tail;
  tail = other.tail;
  other.tail = tmpTail;

  std::swap(size_, other.size_);
}
}  // namespace my
#ifndef CPP2_MY_CONTAINERS_SRC_MY_LIST_H_
#define CPP2_MY_CONTAINERS_SRC_MY_LIST_H_

#include <iostream>
#include <limits>

namespace my {
template <typename T>
class List {
 public:
  class ListIterator;
  class ListConstIterator;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;
  using size_type = size_t;

  List();
  explicit List(size_type n);
  List(std::initializer_list<value_type> const& items);
  List(const List& l);
  List(List&& l) noexcept;
  ~List();
  List& operator=(List&& l) noexcept;

  const_reference front() const;
  const_reference back() const;

  iterator begin();
  iterator end();

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  iterator insert(iterator pos, const_reference value);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

  template <typename... Args>
  void insert_many_back(Args&&... args);

  template <typename... Args>
  void insert_many_front(Args&&... args);

  void erase(iterator pos);

  void push_back(const_reference value);
  void pop_back();
  void push_front(const_reference value);
  void pop_front();
  void swap(List& other);
  void merge(List& other);
  void splice(const_iterator pos, List& other);
  void reverse();
  void unique();
  void sort();

 private:
  struct Node {
    value_type value_;
    Node* next_;
    Node* prev_;
    explicit Node(value_type value)
        : value_(value), next_(nullptr), prev_(nullptr) {}
  };

  void insert_helper(Node* prev, Node* cur);

 public:
  class ListConstIterator {
   public:
    ListConstIterator() : node_(nullptr) {}
    explicit ListConstIterator(Node* node) : node_(node) {}
    ListConstIterator(const ListConstIterator& other) : node_(other.node_) {}

    ~ListConstIterator() = default;

    ListConstIterator& operator=(const ListConstIterator& other);
    bool operator!=(const ListConstIterator& other) const;

    bool operator==(const ListConstIterator& other) const;

    ListConstIterator& operator++();
    ListConstIterator operator++(int);
    ListConstIterator& operator--();
    ListConstIterator operator--(int);

    const_reference operator*() const;

    Node* getNode() const;

   protected:
    Node* node_;
    void iteration(bool isForward) noexcept;
  };

 public:
  class ListIterator : public ListConstIterator {
   public:
    ListIterator() : ListConstIterator() {}
    explicit ListIterator(Node* node) : ListConstIterator(node) {}
    ListIterator(const ListIterator& other) : ListConstIterator(other) {}
    ~ListIterator() = default;
    ListIterator& operator=(const ListIterator& other) noexcept;

    reference operator*();
  };

 private:
  Node* head;
  Node* tail;
  size_type size_;
};

template <typename T>
List<T>::List() : head(nullptr), tail(nullptr), size_(0) {}

template <typename T>
List<T>::List(List::size_type n) {
  size_ = 0;
  head = nullptr;
  tail = nullptr;
  int size = n;
  for (int i = 0; i < size; i++) {
    push_back(T{});
  }
}

template <typename T>
List<T>::List(std::initializer_list<value_type> const& items) {
  head = tail = nullptr;
  size_ = 0;
  for (const auto& item : items) {
    push_back(item);
  }
}

template <typename T>
List<T>::List(const List& l) {
  tail = head = nullptr;
  size_ = 0;
  for (const Node* cur = l.head; cur != nullptr; cur = cur->next_) {
    push_back(cur->value_);
  }
}

template <typename T>
List<T>::List(List&& l) noexcept {
  head = l.head;
  tail = l.tail;
  size_ = l.size_;

  l.head = nullptr;
  l.tail = nullptr;
  l.size_ = 0;
}

template <typename T>
List<T>::~List() {
  clear();
}

template <typename T>
List<T>& List<T>::operator=(List&& l) noexcept {
  if (this != &l) {
    clear();
    head = l.head;
    tail = l.tail;
    size_ = l.size_;

    l.head = nullptr;
    l.tail = nullptr;
    l.size_ = 0;
  }
  return *this;
}

template <typename T>
typename List<T>::const_reference List<T>::front() const {
  return head->value_;
}

template <typename T>
typename List<T>::const_reference List<T>::back() const {
  return tail->value_;
};

template <typename T>
typename List<T>::iterator List<T>::begin() {
  iterator first(head);
  return first;
}

template <typename T>
typename List<T>::iterator List<T>::end() {
  iterator last(tail ? tail->next_ : nullptr);
  return last;
}

template <typename T>
inline bool List<T>::empty() const {
  return size_ == 0;
}

template <typename T>
inline typename List<T>::size_type List<T>::size() const {
  return size_;
}

template <typename T>
inline typename List<T>::size_type List<T>::max_size() const {
  return std::numeric_limits<size_type>::max();
}

template <typename T>
void List<T>::clear() {
  while (head != nullptr) {
    pop_back();
  }
}

template <typename T>
typename List<T>::iterator List<T>::insert(iterator pos,
                                           const_reference value) {
  Node* posNode = pos.getNode();
  Node* prev = posNode->prev_;
  Node* node = new Node(value);
  if (prev == nullptr) {
    head = node;
    node->next_ = posNode;
    posNode->prev_ = node;
  } else {
    prev->next_ = node;
    node->prev_ = prev;
    node->next_ = posNode;
    posNode->prev_ = node;
  }
  ++size_;
  iterator new_pos(posNode->prev_);
  return new_pos;
}

template <typename T>
void List<T>::insert_helper(List::Node* prev, List::Node* cur) {
  Node* next = prev->next_;
  cur->prev_ = prev;
  cur->next_ = next;
  next->prev_ = cur;
  prev->next_ = cur;
}

template <typename T>
template <typename... Args>
typename List<T>::iterator List<T>::insert_many(const_iterator pos,
                                                Args&&... args) {
  Node* posNode = pos.getNode();
  Node* prev = posNode->prev_;
  size_type count = sizeof...(Args);
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};
  if (prev == nullptr) {
    Node* newNode = new Node(tmp[0]);
    head = newNode;
    head->next_ = posNode;
    posNode->prev_ = newNode;
    for (size_type i = 1; i < count; i++) {
      Node* cur = new Node(tmp[i]);
      insert_helper(newNode, cur);
      newNode = cur;
    }
  } else {
    for (size_type i = 0; i < count; i++) {
      Node* newNode = new Node(tmp[i]);
      insert_helper(prev, newNode);
      prev = newNode;
    }
  }
  --pos;
  auto it = begin();
  while (it.getNode()->value_ != pos.getNode()->value_) {
    ++it;
  }
  size_ += count;

  return it;
}

template <typename T>
template <typename... Args>
void List<T>::insert_many_back(Args&&... args) {
  size_type count = sizeof...(Args);
  if (count == 0) return;
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};
  for (size_type i = 0; i < count; i++) {
    push_back(tmp[i]);
  }
}

template <typename T>
template <typename... Args>
void List<T>::insert_many_front(Args&&... args) {
  size_type count = sizeof...(Args);
  if (count == 0) return;
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};
  for (size_type i = 0; i < count; i++) {
    push_front(tmp[i]);
  }
}

template <typename T>
void List<T>::erase(iterator pos) {
  Node* cur = head;
  if (cur == nullptr) return;

  for (; cur; cur = cur->next_) {
    if (cur->value_ == *pos) {
      Node* prev = cur->prev_;
      Node* next = cur->next_;

      if (prev != nullptr) {
        prev->next_ = next;
      }
      if (next != nullptr) {
        next->prev_ = prev;
      }
      break;
    }
  }
  delete cur;
  size_--;
}

template <typename T>
void List<T>::push_back(const_reference value) {
  Node* ptr = new Node(value);
  ptr->prev_ = tail;
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
void List<T>::pop_back() {
  if (tail != nullptr) {
    Node* ptr = tail->prev_;
    if (ptr != nullptr) {
      ptr->next_ = nullptr;
    } else {
      head = nullptr;
    }
    delete tail;
    tail = ptr;
    size_--;
  }
}

template <typename T>
void List<T>::push_front(const_reference value) {
  Node* ptr = new Node(value);
  ptr->next_ = head;
  if (head != nullptr) {
    head->prev_ = ptr;
  }
  if (tail == nullptr) {
    tail = ptr;
  }
  head = ptr;
  size_++;
}

template <typename T>
void List<T>::pop_front() {
  if (head != nullptr) {
    Node* ptr = head->next_;
    if (ptr != nullptr) {
      ptr->prev_ = nullptr;
    } else {
      tail = nullptr;
    }
    delete head;
    head = ptr;
    size_--;
  }
}

template <typename T>
void List<T>::swap(List& other) {
  Node* tmpHead = head;
  head = other.head;
  other.head = tmpHead;

  Node* tmpTail = tail;
  tail = other.tail;
  other.tail = tmpTail;

  std::swap(size_, other.size_);
}

template <typename T>
void List<T>::merge(List& other) {
  List mergedList;

  iterator it_this = begin();
  iterator it_other = other.begin();

  while ((it_this != end()) && (it_other != other.end())) {
    if (*it_this < *it_other) {
      mergedList.push_back(*it_this);
      ++it_this;
    } else {
      mergedList.push_back(*it_other);
      ++it_other;
    }
  }

  while (it_this != end()) {
    mergedList.push_back(*it_this);
    ++it_this;
  }

  while (it_other != other.end()) {
    mergedList.push_back(*it_other);
    ++it_other;
  }

  swap(mergedList);
  sort();
}

template <typename T>
void List<T>::splice(const_iterator pos, List& other) {
  if (this == &other || other.empty()) return;

  Node* posNode = pos.getNode();
  Node* prev = posNode->prev_;

  if (prev != nullptr) {
    prev->next_ = other.head;
  } else {
    head = other.head;
  }

  if (other.head != nullptr) {
    other.head->prev_ = prev;
  }

  if (posNode != nullptr) {
    other.tail->next_ = posNode;
    posNode->prev_ = other.tail;
  } else {
    tail = other.tail;
  }

  size_ += other.size_;

  other.head = nullptr;
  other.tail = nullptr;
  other.size_ = 0;
}

template <typename T>
void List<T>::reverse() {
  Node* cur = head;
  Node* temp = nullptr;

  while (cur != nullptr) {
    temp = cur->prev_;
    cur->prev_ = cur->next_;
    cur->next_ = temp;

    cur = cur->prev_;
  }

  temp = head;
  head = tail;
  tail = temp;
}

template <typename T>
void List<T>::unique() {
  Node* cur = head;

  while (cur != nullptr && cur->next_ != nullptr) {
    if (cur->value_ == cur->next_->value_) {
      Node* dup = cur->next_;
      cur->next_ = dup->next_;
      size_--;

      if (cur->next_ != nullptr) {
        cur->next_->prev_ = cur;
      } else {
        tail = cur;
      }
      delete dup;
    } else {
      cur = cur->next_;
    }
  }
}

template <typename T>
void List<T>::sort() {
  if (head == nullptr || head->next_ == nullptr) return;

  Node* cur = head->next_;

  while (cur != nullptr) {
    Node* tmp = cur;
    while ((tmp->prev_ != nullptr) && (tmp->value_ < tmp->prev_->value_)) {
      std::swap(tmp->value_, tmp->prev_->value_);
      tmp = tmp->prev_;
    }
    cur = cur->next_;
  }
}

template <typename T>
typename List<T>::ListIterator& List<T>::ListIterator::operator=(
    const ListIterator& other) noexcept {
  if (this != &other) this->node_ = other.node_;
  return *this;
}

template <typename T>
typename List<T>::reference List<T>::ListIterator::operator*() {
  return this->node_->value_;
}

template <typename T>
typename List<T>::ListConstIterator::ListConstIterator&
List<T>::ListConstIterator::operator=(const ListConstIterator& other) {
  if (this != &other) node_ = other.node_;
  return *this;
}

template <typename T>
bool List<T>::ListConstIterator::operator!=(
    const ListConstIterator& other) const {
  return node_ != other.node_;
}

template <typename T>
bool List<T>::ListConstIterator::operator==(
    const ListConstIterator& other) const {
  return node_ == other.node_;
}

template <typename T>
typename List<T>::ListConstIterator::ListConstIterator&
List<T>::ListConstIterator::operator++() {
  iteration(true);
  return *this;
}

template <typename T>
typename List<T>::ListConstIterator::ListConstIterator
List<T>::ListConstIterator::operator++(int) {
  ListConstIterator cur(*this);
  iteration(true);
  return cur;
}

template <typename T>
typename List<T>::ListConstIterator::ListConstIterator&
List<T>::ListConstIterator::operator--() {
  iteration(false);
  return *this;
}

template <typename T>
typename List<T>::ListConstIterator::ListConstIterator
List<T>::ListConstIterator::operator--(int) {
  ListConstIterator cur(*this);
  iteration(false);
  return cur;
}

template <typename T>
typename List<T>::const_reference List<T>::ListConstIterator::operator*()
    const {
  return node_->value_;
}

template <typename T>
typename List<T>::Node* List<T>::ListConstIterator::getNode() const {
  return node_;
}

template <typename T>
void List<T>::ListConstIterator::iteration(bool isForward) noexcept {
  if (node_ == nullptr) return;
  if (isForward)
    node_ = node_->next_;
  else
    node_ = node_->prev_;
}

}  // namespace my

#endif
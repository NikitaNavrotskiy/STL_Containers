#ifndef CPP2_MY_CONTAINERS_SRC_MY_VECTOR_H_
#define CPP2_MY_CONTAINERS_SRC_MY_VECTOR_H_

#include <iostream>
#include <limits>

namespace my {
template <typename T>
class Vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  Vector();
  explicit Vector(size_type n);
  Vector(std::initializer_list<value_type> const &items);
  Vector(const Vector &v);
  Vector(Vector &&v) noexcept;

  ~Vector();

  Vector &operator=(Vector &&v) noexcept;

  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  T *data();

  iterator begin();
  iterator end();

  bool empty();
  size_type size();
  size_type max_size();
  void reserve(size_type size);
  size_type capacity();
  void shrink_to_fit();

  void clear();
  iterator insert(iterator pos, const_reference value);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

  template <typename... Args>
  void insert_many_back(Args &&...args);

  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void swap(Vector &other);

 private:
  size_type len;
  size_type cap;
  T *values;
};

template <typename T>
Vector<T>::Vector() : len(0), cap(15) {
  values = new T[cap];
}

template <typename T>
Vector<T>::Vector(Vector::size_type n) {
  cap = 15;
  if (n >= cap) {
    while (n >= cap) {
      cap *= 2;
    }
  }
  len = n;
  try {
    values = new T[cap];
  } catch (std::bad_alloc &e) {
    std::cerr << "Error: Unable to allocate memory -" << e.what() << std::endl;
    exit(1);
  }
  for (size_type i = 0; i < len; i++) {
    values[i] = 0;
  }
}

template <typename T>
Vector<T>::Vector(const std::initializer_list<value_type> &items) : Vector() {
  for (const auto &item : items) {
    push_back(item);
  }
}

template <typename T>
Vector<T>::Vector(const Vector &v) {
  len = v.len;
  cap = v.cap;
  try {
    values = new T[cap];
  } catch (std::bad_alloc &e) {
    std::cerr << "Error: Unable to allocate memory -" << e.what() << std::endl;
    exit(1);
  }
  for (size_type i = 0; i < len; i++) {
    values[i] = v.values[i];
  }
}

template <typename T>
Vector<T>::Vector(Vector &&v) noexcept {
  len = v.len;
  cap = v.cap;
  values = v.values;

  v.values = nullptr;
  v.len = 0;
  v.cap = 0;
}

template <typename T>
Vector<T>::~Vector() {
  delete[] values;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector &&v) noexcept {
  if (this != &v) {
    delete[] values;
    len = v.len;
    cap = v.cap;
    values = v.values;

    v.len = 0;
    v.cap = 0;
    v.values = nullptr;
  }
  return *this;
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(Vector::size_type pos) {
  if (pos >= len) {
    throw std::out_of_range("pos >= this->size()");
  }
  return values[pos];
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](Vector::size_type pos) {
  return at(pos);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() {
  return at(0);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() {
  return at(len - 1);
}

template <typename T>
T *Vector<T>::data() {
  return values;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() {
  return values;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end() {
  return values + len;
}

template <typename T>
bool Vector<T>::empty() {
  return len == 0;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::size() {
  return len;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::max_size() {
  return std::numeric_limits<size_type>::max();
}

template <typename T>
void Vector<T>::reserve(Vector::size_type size) {
  if (size <= cap) return;
  T *tmp;
  try {
    tmp = new T[size];
  } catch (std::bad_alloc &e) {
    std::cerr << "Error: Unable to allocate memory -" << e.what() << std::endl;
    exit(1);
  }
  for (size_type i = 0; i < len; i++) {
    tmp[i] = values[i];
  }

  delete[] values;
  values = tmp;
  cap = size;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() {
  return cap;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
  if (cap * 0.75 <= len) return;
  T *tmp;
  try {
    tmp = new T[len];
  } catch (std::bad_alloc &e) {
    std::cerr << "Error: Unable to allocate memory -" << e.what() << std::endl;
    exit(1);
  }
  for (size_type i = 0; i < len; i++) {
    tmp[i] = values[i];
  }

  delete[] values;
  values = tmp;
  cap = len;
}

template <typename T>
void Vector<T>::clear() {
  delete[] values;
  values = new T[cap];
  len = 0;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(Vector::iterator pos,
                                               const_reference value) {
  if (len + 1 >= cap) {
    reserve(cap * 2);
  }
  size_type index = pos - begin();

  for (size_type i = len; i > index; --i) {
    values[i] = values[i - 1];
  }

  values[index] = value;
  ++len;
  return pos;
}

template <typename T>
template <typename... Args>
typename Vector<T>::iterator Vector<T>::insert_many(Vector::const_iterator pos,
                                                    Args &&...args) {
  size_type index = pos - begin();
  size_type count = sizeof...(Args);

  size_type new_len = len + count;
  size_type new_cap = cap;
  if (new_len >= cap) {
    while (new_len >= new_cap) {
      new_cap *= 2;
    }
    reserve(new_len * 2);
  }
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};

  for (size_type i = len + count - 1; i >= len; --i) {
    values[i] = values[i - count];
  }

  for (size_type i = 0; i < count; ++i) {
    values[index + i] = tmp[i];
  }
  len += count;
  return begin() + index;
}

template <typename T>
template <typename... Args>
void Vector<T>::insert_many_back(Args &&...args) {
  size_type count = sizeof...(Args);
  size_type index = len;
  size_type new_len = len + count;
  size_type new_cap = cap;

  if (new_len >= cap) {
    while (new_len >= new_cap) {
      new_cap *= 2;
    }
    reserve(new_cap);
  }
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};

  for (size_type i = 0; i < count; i++) {
    values[i + index] = tmp[i];
  }

  len += count;
}

template <typename T>
void Vector<T>::erase(Vector::iterator pos) {
  size_type index = pos - begin();

  for (size_type i = index; i < len - 1; i++) {
    values[i] = values[i + 1];
  }
  --len;
}

template <typename T>
void Vector<T>::push_back(const_reference value) {
  if (cap * 0.75 <= len) reserve(cap * 2);
  values[len] = value;
  ++len;
}

template <typename T>
void Vector<T>::pop_back() {
  if (len > 0) {
    --len;
  }
}

template <typename T>
void Vector<T>::swap(Vector &other) {
  T *tmp = other.values;
  other.values = values;
  values = tmp;

  std::swap(len, other.len);
  std::swap(cap, other.cap);
}
}  // namespace my

#endif

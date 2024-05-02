#ifndef _MY_CONTAINER_ARRAY_H
#define _MY_CONTAINER_ARRAY_H

#include <iostream>

namespace my {
template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  array();
  array(std::initializer_list<value_type> const &items);
  array(const array &a);
  array(array &&a) noexcept;
  ~array() = default;

  array &operator=(array &&a) noexcept;

  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference front();
  reference back();
  const_reference front() const;
  const_reference back() const;
  iterator data();
  const_iterator data() const;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void swap(array &other);
  void fill(const_reference value);

 private:
  size_type size_ = N;
  value_type arr[N];
};

template <typename T, std::size_t N>
inline array<T, N>::array() {
  for (size_type i = 0; i < N; i++) arr[i] = 0;
}

template <typename T, std::size_t N>
inline array<T, N>::array(std::initializer_list<value_type> const &items) {
  std::copy(items.begin(), items.end(), arr);
  for (size_type i = items.size(); i < N; i++) arr[i] = 0;
  size_ = N;
}

template <typename T, std::size_t N>
inline array<T, N>::array(const array &a) : array() {
  std::copy(a.arr, a.arr + a.size_, arr);
  size_ = a.size();
}

template <typename T, std::size_t N>
inline array<T, N>::array(array &&a) noexcept : array() {
  if (this != &a) {
    std::move(a.arr, a.arr + N, arr);
    size_ = a.size_;
    for (size_type i = 0; i < a.size(); i++) a.arr[i] = 0;
  }
}

template <typename T, std::size_t N>
inline typename array<T, N>::array &array<T, N>::operator=(array &&a) noexcept {
  *this = std::move(a);

  return *this;
}

template <typename T, std::size_t N>
inline typename array<T, N>::reference array<T, N>::operator[](size_type pos) {
  return at(pos);
}

template <typename T, std::size_t N>
inline typename array<T, N>::const_reference array<T, N>::operator[](
    size_type pos) const {
  return at(pos);
}

template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::at(size_type pos) {
  if (pos >= N) throw std::runtime_error("ArrayError: Index out of range");
  return arr[pos];
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::at(size_type pos) const {
  if (pos >= N) throw std::runtime_error("ArrayError: Index out of range");
  return arr[pos];
}

template <typename T, std::size_t N>
inline typename array<T, N>::reference array<T, N>::front() {
  return arr[0];
}

template <typename T, std::size_t N>
inline typename array<T, N>::const_reference array<T, N>::front() const {
  return arr[0];
}

template <typename T, std::size_t N>
inline typename array<T, N>::reference array<T, N>::back() {
  return arr[N - 1];
}

template <typename T, std::size_t N>
inline typename array<T, N>::const_reference array<T, N>::back() const {
  return arr[N - 1];
}

template <typename T, std::size_t N>
inline bool array<T, N>::empty() const {
  return size_ == 0;
}

template <typename T, std::size_t N>
inline typename array<T, N>::size_type array<T, N>::size() const {
  return size_;
}

template <typename T, std::size_t N>
inline typename array<T, N>::size_type array<T, N>::max_size() const {
  return N;
}

template <typename T, std::size_t N>
inline typename array<T, N>::iterator array<T, N>::begin() {
  return arr;
}

template <typename T, std::size_t N>
inline typename array<T, N>::const_iterator array<T, N>::begin() const {
  return arr;
}

template <typename T, std::size_t N>
inline typename array<T, N>::iterator array<T, N>::end() {
  return arr + size_;
}

template <typename T, std::size_t N>
inline typename array<T, N>::const_iterator array<T, N>::end() const {
  return arr + size_;
}

template <typename T, std::size_t N>
inline typename array<T, N>::iterator array<T, N>::data() {
  return begin();
}

template <typename T, std::size_t N>
inline typename array<T, N>::const_iterator array<T, N>::data() const {
  return begin();
}

template <typename T, std::size_t N>
inline void array<T, N>::fill(const_reference val) {
  for (size_type i = 0; i < size_; i++) arr[i] = val;
}

template <typename T, std::size_t N>
void array<T, N>::swap(array &other) {
  for (size_type i = 0; i < N; i++) std::swap(arr[i], other.arr[i]);
}

}  // namespace my

#endif  // !_MY_CONTAINER_ARRAY_H

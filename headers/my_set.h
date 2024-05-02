#ifndef _MY_CONTAINER_SET_H
#define _MY_CONTAINER_SET_H

#include "my_rbtree.h"
#include "my_vector.h"

namespace my {

template <typename T>
class set : public rb_tree<T> {
 public:
  using key_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  set() : rb_tree<key_type>() {}
  set(std::initializer_list<T> const &items);

  template <typename... Args>
  Vector<std::pair<typename rb_tree<T>::iterator, bool>> insert_many(
      Args &&...args);
};

template <typename T>
set<T>::set(std::initializer_list<T> const &items) {
  for (auto it = items.begin(); it != items.end(); it++) this->insert(*it);
}

template <typename T>
template <typename... Args>
Vector<std::pair<typename rb_tree<T>::iterator, bool>> set<T>::insert_many(
    Args &&...args) {
  Vector<std::pair<typename rb_tree<T>::iterator, bool>> res;

  size_type count = sizeof...(Args);
  if (count == 0) throw std::runtime_error("Error: Zero args");
  T tmp[sizeof...(Args)] = {std::forward<Args>(args)...};
  for (size_type i = 0; i < count; i++) {
    res.push_back(this->insert(tmp[i]));
  }

  return res;
}

}  // namespace my

#endif
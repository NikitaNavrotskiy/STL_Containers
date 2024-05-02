#ifndef _MY_CONTAINER_MAP_H
#define _MY_CONTAINER_MAP_H

#include "my_rbtree.h"
#include "my_vector.h"

namespace my {

template <typename Key, typename T>
class map : public rb_tree<std::pair<Key, T>> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename rb_tree<value_type>::iterator;
  using const_iterator = typename rb_tree<value_type>::const_iterator;
  using size_type = size_t;

  map() : rb_tree<value_type>::rb_tree(){};
  map(std::initializer_list<value_type> const &items);
  map(const map &m) : rb_tree<value_type>::rb_tree(m){};
  map(map &&m) noexcept : rb_tree<value_type>::rb_tree(std::move(m)){};

  T &at(const Key &key);
  T &operator[](const Key &key);

  std::pair<iterator, bool> insert(const value_type &value);
  std::pair<iterator, bool> insert(const Key &key, const T &obj);
  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj);
  void erase(iterator pos);

  bool contains(const Key &key);

  template <typename... Args>
  Vector<std::pair<iterator, bool>> insert_many(Args &&...args);

  iterator find(const Key &key);

 private:
  typename rb_tree<value_type>::node *find(
      typename rb_tree<value_type>::node *root, const Key &key);
  void operator_eq(typename map<Key, T>::node *&nd1,
                   typename map<Key, T>::node *&nd2);
  void swap_pairs(typename map<Key, T>::node *&nd1,
                  typename map<Key, T>::node *&nd2);
};

template <typename Key, typename T>
map<Key, T>::map(const std::initializer_list<std::pair<Key, T>> &items) {
  for (const std::pair<const Key, T> item : items)
    insert_or_assign(item.first, item.second);
}

template <typename Key, typename T>
T &map<Key, T>::at(const Key &key) {
  typename map<Key, T>::node *nd = find(this->root, key);
  if (nd == nullptr) throw std::runtime_error("Error: No such key");
  return nd->key.second;
}

template <typename Key, typename T>
T &map<Key, T>::operator[](const Key &key) {
  return at(key);
}

template <typename Key, typename T>
typename std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const std::pair<Key, T> &value) {
  typename map<Key, T>::node *new_node;
  typename map<Key, T>::node *cur = this->root;
  typename map<Key, T>::node *prev = nullptr;
  bool stop = false;
  bool res = false;
  iterator it;

  while (cur && stop == false) {
    prev = cur;
    if (value.first > cur->key.first)
      cur = cur->right;
    else if (value.first < cur->key.first)
      cur = cur->left;
    else
      stop = true;
  }

  if (!stop) {
    new_node =
        new typename map<Key, T>::node(value, prev, rb_tree<value_type>::RED);
    rb_tree<value_type>::count_++;

    if (prev) {
      if (new_node->key > prev->key)
        prev->right = new_node;
      else
        prev->left = new_node;
    } else
      this->root = new_node;
    it = rb_tree<value_type>::balance_on_insert(new_node);
    res = true;
  }

  return std::pair<iterator, bool>(it, res);
}

template <typename Key, typename T>
typename std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key &key, const T &obj) {
  return insert(std::pair(key, obj));
}

template <typename Key, typename T>
typename std::pair<typename map<Key, T>::iterator, bool>
map<Key, T>::insert_or_assign(const Key &key, const T &obj) {
  typename map<Key, T>::node *new_node;
  typename map<Key, T>::node *cur = this->root;
  typename map<Key, T>::node *prev = nullptr;
  bool stop = false;
  bool res = false;
  iterator it;

  while (cur && stop == false) {
    prev = cur;
    if (key > cur->key.first)
      cur = cur->right;
    else if (key < cur->key.first)
      cur = cur->left;
    else if (key == cur->key.first) {
      cur->key.second = obj;
      stop = true;
    } else
      stop = true;
  }

  if (!stop) {
    new_node = new
        typename map<Key, T>::node({key, obj}, prev, rb_tree<value_type>::RED);
    rb_tree<value_type>::count_++;

    if (prev) {
      if (new_node->key > prev->key)
        prev->right = new_node;
      else
        prev->left = new_node;
    } else
      this->root = new_node;
    it = rb_tree<value_type>::balance_on_insert(new_node);
    res = true;
  }

  return std::pair<iterator, bool>(it, res);
}

template <typename Key, typename T>
void map<Key, T>::erase(iterator pos) {
  typename map<Key, T>::node *replace, *delete_node = this->get_iter_node(pos),
                                       *tmp = nullptr;
  if (!delete_node)
    return;
  else if (delete_node == this->root && !this->root->left &&
           !this->root->right) {
    delete delete_node;
    this->root = nullptr;
    rb_tree<value_type>::count_--;
    return;
  }
  if (delete_node->left && delete_node->right)  // case : two children
  {
    replace = delete_node->right;
    while (replace->left) replace = replace->left;
    swap_pairs(delete_node, replace);
  } else
    replace = delete_node;
  if (replace->right && !replace->left)  // case right child, no left child
  {
    operator_eq(replace, replace->right);
    tmp = replace->right;
    replace->right = nullptr;
    delete tmp;
  } else if (replace->left &&
             !replace->right)  // case left child, no right child
  {
    operator_eq(replace, replace->left);
    tmp = replace->left;
    replace->left = nullptr;
    delete tmp;
  } else  // case no children
  {
    typename map<Key, T>::node *tmp_parent;
    typename rb_tree<value_type>::node_colors color = replace->color;
    tmp = replace;
    if (replace != this->root) {
      if (replace->parent->left == replace) replace->parent->left = nullptr;
      if (replace->parent->right == replace) replace->parent->right = nullptr;
    }
    tmp_parent = replace->parent;

    delete tmp;
    if (color == rb_tree<value_type>::BLACK)
      this->balance_on_delete(tmp_parent, nullptr);
  }
  rb_tree<value_type>::count_--;
}

template <typename Key, typename T>
bool map<Key, T>::contains(const Key &key) {
  return find(key) != nullptr;
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::find(const Key &key) {
  return iterator(find(this->root, key));
}

template <typename Key, typename T>
typename rb_tree<std::pair<Key, T>>::node *map<Key, T>::find(
    typename rb_tree<std::pair<Key, T>>::node *root, const Key &key) {
  if (root == nullptr) return nullptr;
  typename map<Key, T>::node *res;
  if (root->key.first == key)
    res = root;
  else if (root->key.first < key)
    res = find(root->right, key);
  else if (root->key.first > key)
    res = find(root->left, key);
  return res;
}

template <typename Key, typename T>
void map<Key, T>::operator_eq(typename map<Key, T>::node *&nd1,
                              typename map<Key, T>::node *&nd2) {
  nd1->key.first = nd2->key.first;
  nd1->key.second = nd2->key.second;
}

template <typename Key, typename T>
void map<Key, T>::swap_pairs(typename map<Key, T>::node *&nd1,
                             typename map<Key, T>::node *&nd2) {
  std::swap(nd1->key.first, nd2->key.first);
  std::swap(nd1->key.second, nd2->key.second);
}

template <typename Key, typename T>
template <typename... Args>
Vector<std::pair<typename map<Key, T>::iterator, bool>>
map<Key, T>::insert_many(Args &&...args) {
  Vector<std::pair<iterator, bool>> res;

  for (const auto &arg : {args...}) res.push_back(insert(arg));

  return res;
}

}  // namespace my

#endif
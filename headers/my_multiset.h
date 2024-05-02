#ifndef _MY_CONTAINER_MULTISET_H
#define _MY_CONTAINER_MULTISET_H

#include "my_set.h"

namespace my {

template <typename Key>
class multiset : public rb_tree<std::pair<Key, int>> {
 public:
  using key_type = Key;
  using value_type = std::pair<Key, int>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using const_iterator = typename rb_tree<value_type>::const_iterator;
  using iterator = typename rb_tree<value_type>::iterator;
  using size_type = size_t;

  multiset() : rb_tree<value_type>() {}
  multiset(std::initializer_list<key_type> const &itmes);
  multiset(std::initializer_list<value_type> const &items);
  multiset(const multiset &ms) : rb_tree<value_type>(ms) {}
  multiset(multiset &&ms) noexcept : rb_tree<value_type>(std::move(ms)) {}

  iterator insert(const Key &value);
  bool contains(const Key &key);
  void erase(iterator pos);
  size_type count(const Key &key);
  iterator lower_bound(const Key &key);
  iterator upper_bound(const Key &key);

  template <typename... Args>
  Vector<std::pair<iterator, bool>> insert_many(Args &&...args);
  std::pair<iterator, iterator> equal_range(const Key &key);
  iterator find(const Key &key);

 private:
  typename rb_tree<value_type>::node *find(
      typename rb_tree<value_type>::node *root, const Key &key);
  void operator_eq(typename multiset<Key>::node *&nd1,
                   typename multiset<Key>::node *&nd2);
  void swap_pairs(typename multiset<Key>::node *&nd1,
                  typename multiset<Key>::node *&nd2);
};

template <typename Key>
multiset<Key>::multiset(std::initializer_list<key_type> const &items) {
  for (auto item : items) this->insert(item);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::insert(const Key &value) {
  typename multiset<Key>::node *new_node;
  typename multiset<Key>::node *cur = this->root;
  typename multiset<Key>::node *prev = nullptr;
  bool stop = false;
  iterator it;

  while (cur && stop == false) {
    prev = cur;
    if (value > cur->key.first)
      cur = cur->right;
    else if (value < cur->key.first)
      cur = cur->left;
    else {
      stop = true;
      cur->key.second++;
      this->count_++;
    }
  }

  if (!stop) {
    new_node = new typename multiset<Key>::node(
        std::pair<key_type, int>(value, 1), prev, rb_tree<value_type>::RED);
    rb_tree<value_type>::count_++;

    if (prev) {
      if (new_node->key > prev->key)
        prev->right = new_node;
      else
        prev->left = new_node;
    } else
      this->root = new_node;
    it = rb_tree<value_type>::balance_on_insert(new_node);
  }

  return it;
}

template <typename Key>
void multiset<Key>::erase(iterator pos) {
  typename multiset<Key>::node *replace,
      *delete_node = this->get_iter_node(pos), *tmp = nullptr;
  if (!delete_node)
    return;
  else if (delete_node->key.second > 1)
    delete_node->key.second--;
  else if (delete_node == this->root && !this->root->left &&
           !this->root->right) {
    delete delete_node;
    this->root = nullptr;
    rb_tree<value_type>::count_--;
  } else {
    if (delete_node->left && delete_node->right) {
      replace = delete_node->right;
      while (replace->left) replace = replace->left;
      swap_pairs(delete_node, replace);
    } else
      replace = delete_node;
    if (replace->right && !replace->left) {
      operator_eq(replace, replace->right);
      tmp = replace->right;
      replace->right = nullptr;
      delete tmp;
    } else if (replace->left && !replace->right) {
      operator_eq(replace, replace->left);
      tmp = replace->left;
      replace->left = nullptr;
      delete tmp;
    } else {
      typename multiset<Key>::node *tmp_parent;
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
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::count(const Key &key) {
  size_type res = 0;

  auto nd = find(this->root, key);

  if (nd == nullptr)
    res = 0;
  else
    res = nd->key.second;

  return res;
}

template <typename Key>
bool multiset<Key>::contains(const Key &key) {
  return find(key) != nullptr;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::lower_bound(const Key &key) {
  typename multiset<Key>::iterator res;
  bool stop = false;

  for (auto it = this->begin(); it != this->end() && !stop; it++) {
    if ((*it).first >= key) {
      res = it;
      stop = true;
    }
  }
  return res;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::upper_bound(const Key &key) {
  typename multiset<Key>::iterator res;
  bool stop = false;

  for (auto it = this->begin(); it != this->end() && !stop; it++) {
    if ((*it).first <= key) {
      res = it;
      stop = true;
    }
  }
  return res;
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::find(const Key &key) {
  return iterator(find(this->root, key));
}

template <typename Key>
typename rb_tree<std::pair<Key, int>>::node *multiset<Key>::find(
    typename rb_tree<std::pair<Key, int>>::node *root, const Key &key) {
  if (root == nullptr) return nullptr;
  typename multiset<Key>::node *res;
  if (root->key.first == key)
    res = root;
  else if (root->key.first < key)
    res = find(root->right, key);
  else if (root->key.first > key)
    res = find(root->left, key);
  return res;
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, typename multiset<Key>::iterator>
multiset<Key>::equal_range(const Key &key) {
  return std::pair(find(key), find(key));
}

template <typename Key>
void multiset<Key>::operator_eq(typename multiset<Key>::node *&nd1,
                                typename multiset<Key>::node *&nd2) {
  nd1->key.first = nd2->key.first;
  nd1->key.second = nd2->key.second;
}

template <typename Key>
void multiset<Key>::swap_pairs(typename multiset<Key>::node *&nd1,
                               typename multiset<Key>::node *&nd2) {
  std::swap(nd1->key.first, nd2->key.first);
  std::swap(nd1->key.second, nd2->key.second);
}

template <typename Key>
template <typename... Args>
Vector<std::pair<typename multiset<Key>::iterator, bool>>
multiset<Key>::insert_many(Args &&...args) {
  Vector<std::pair<iterator, bool>> res;

  size_type count = sizeof...(Args);
  if (count == 0) throw std::runtime_error("Error: Zero args");
  Key tmp[sizeof...(Args)] = {std::forward<Args>(args)...};
  for (size_type i = 0; i < count; i++) {
    res.push_back(std::pair(insert(tmp[i]), true));
  }

  return res;
}

}  // namespace my

#endif

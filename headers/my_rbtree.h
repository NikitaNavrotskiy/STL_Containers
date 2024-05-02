#ifndef _MY_CONTAINER_RBTREE_H
#define _MY_CONTAINER_RBTREE_H

#include <iostream>
#include <limits>

namespace my {

template <typename T>
class rb_tree {
 public:
  class iterator;
  class const_iterator;

  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  rb_tree();
  rb_tree(const rb_tree &other);
  rb_tree(rb_tree &&other) noexcept;
  ~rb_tree();
  rb_tree &operator=(rb_tree &&other) noexcept;

  iterator begin();
  iterator end();

  bool empty();
  size_type size();
  size_type max_size();

  void clear();
  std::pair<iterator, bool> insert(value_type key);
  void erase(iterator pos);
  void swap(rb_tree &other);
  void merge(rb_tree &other);

  iterator find(const_reference key);
  bool contains(const_reference key);

 protected:
  struct node;

 public:
  class iterator {
   public:
    friend node *rb_tree::get_iter_node(iterator it);
    iterator();
    iterator(node *nd);
    iterator(const iterator &other);
    reference operator*() const;
    bool operator==(const iterator &other) const;
    bool operator!=(const iterator &other) const;
    iterator &operator++();
    iterator operator++(int);
    iterator &operator=(const iterator &other);

   protected:
    node *iter_node = nullptr;

   private:
    node *next(node *nd);
  };

  class const_iterator : public iterator {
   public:
    const_reference operator*() const;
  };

 protected:
  enum node_colors : bool { RED, BLACK };

  struct node {
    value_type key;
    node *parent = nullptr;
    node *left = nullptr;
    node *right = nullptr;
    node_colors color;

    node(value_type key, node *parent, node_colors color);
  };

 protected:
  node *find(node *root, const_reference key);
  node *copy(node *nd, node *parent);
  void free_node(node *nd);
  void left_rotate(node *x);
  void right_rotate(node *y);
  iterator balance_on_insert(node *nd);
  void balance_on_delete(node *parent, node *prev);
  void balance_on_delete_red_par(node *parent, node *prev);
  void balance_on_delete_black_pair_left(node *parent, node *child);
  void balance_on_delete_black_pair_right(node *parent, node *child);
  node *get_iter_node(iterator it);

 protected:
  node *root;
  size_type count_ = 0;
};

template <typename T>
rb_tree<T>::node::node(value_type key, node *parent, node_colors color)
    : key(key), parent(parent), color(color) {}

template <typename T>
rb_tree<T>::rb_tree() : root(nullptr) {}

template <typename T>
rb_tree<T>::rb_tree(const rb_tree &other) : root(nullptr) {
  if (this != &other) {
    this->root = copy(other.root, nullptr);
    count_ = other.count_;
  }
}

template <typename T>
rb_tree<T>::rb_tree(rb_tree &&other) noexcept {
  if (this != &other) {
    root = other.root;
    count_ = other.count_;
    other.root = nullptr;
    other.count_ = 0;
  }
}

template <typename T>
rb_tree<T>::~rb_tree() {
  free_node(root);
}

template <typename T>
rb_tree<T> &rb_tree<T>::operator=(rb_tree &&other) noexcept {
  if (this != &other) {
    root = other.root;
    other.root = nullptr;
  }

  return *this;
}

template <typename T>
typename rb_tree<T>::iterator rb_tree<T>::begin() {
  node *cur = root;
  node *prev = nullptr;

  while (cur) {
    prev = cur;
    cur = cur->left;
  }
  return rb_tree<T>::iterator(prev);
}

template <typename T>
typename rb_tree<T>::iterator rb_tree<T>::end() {
  return iterator(nullptr);
}

template <typename T>
inline bool rb_tree<T>::empty() {
  return root == nullptr;
}

template <typename T>
inline size_t rb_tree<T>::size() {
  return count_;
}

template <typename T>
inline size_t rb_tree<T>::max_size() {
  return std::numeric_limits<size_type>::max() /
         sizeof(typename rb_tree<T>::node);
}

template <typename T>
void rb_tree<T>::clear() {
  free_node(root);
  root = nullptr;
  count_ = 0;
}

template <typename T>
typename std::pair<typename rb_tree<T>::iterator, bool> rb_tree<T>::insert(
    value_type key) {
  node *new_node;
  node *cur = root;
  node *prev = nullptr;
  bool stop = false;
  bool res = false;
  iterator it;

  while (cur && stop == false) {
    prev = cur;
    if (key > cur->key)
      cur = cur->right;
    else if (key < cur->key)
      cur = cur->left;
    else
      stop = true;
  }

  if (!stop) {
    new_node = new node(key, prev, RED);
    count_++;

    if (prev) {
      if (new_node->key > prev->key)
        prev->right = new_node;
      else
        prev->left = new_node;
    } else
      root = new_node;
    it = balance_on_insert(new_node);
    res = true;
  }

  return std::pair<iterator, bool>(it, res);
}

template <typename T>
void rb_tree<T>::erase(iterator pos) {
  node *replace, *delete_node = get_iter_node(pos), *tmp = nullptr;
  if (!delete_node)
    return;
  else if (delete_node == root && !root->left && !root->right) {
    delete delete_node;
    root = nullptr;
    count_--;
    return;
  }
  if (delete_node->left && delete_node->right) {
    replace = delete_node->right;
    while (replace->left) replace = replace->left;
    std::swap(delete_node->key, replace->key);
  } else
    replace = delete_node;

  if (replace->right && !replace->left) {
    replace->key = replace->right->key;
    tmp = replace->right;
    replace->right = nullptr;
    delete tmp;
  } else if (replace->left && !replace->right) {
    replace->key = replace->left->key;
    tmp = replace->left;
    replace->left = nullptr;
    delete tmp;
  } else {
    node *tmp_parent;
    node_colors color = replace->color;
    tmp = replace;
    if (replace != root) {
      if (replace->parent->left == replace) replace->parent->left = nullptr;
      if (replace->parent->right == replace) replace->parent->right = nullptr;
    }
    tmp_parent = replace->parent;

    delete tmp;
    if (color == BLACK) balance_on_delete(tmp_parent, nullptr);
  }
  count_--;
}

template <typename T>
inline void rb_tree<T>::swap(rb_tree &other) {
  std::swap(root, other.root);
  std::swap(count_, other.count_);
}

template <typename T>
void rb_tree<T>::merge(rb_tree &other) {
  for (auto it = other.begin(); it != other.end(); it++) insert(*it);
}

template <typename T>
typename rb_tree<T>::iterator rb_tree<T>::find(const_reference key) {
  return iterator(find(root, key));
}

template <typename T>
bool rb_tree<T>::contains(const_reference key) {
  return find(key) != end();
}

// rb_tree::iterator

// public rb_tree::iterator

template <typename T>
rb_tree<T>::iterator::iterator() : iter_node(nullptr) {}

template <typename T>
rb_tree<T>::iterator::iterator(const iterator &other) {
  this->iter_node = other.iter_node;
}

template <typename T>
inline typename rb_tree<T>::reference rb_tree<T>::iterator::operator*() const {
  if (!iter_node) throw std::runtime_error("Error: Operator* for nullptr");
  return iter_node->key;
}

template <typename T>
inline bool rb_tree<T>::iterator::operator==(const iterator &other) const {
  return iter_node == other.iter_node;
}

template <typename T>
inline bool rb_tree<T>::iterator::operator!=(const iterator &other) const {
  return iter_node != other.iter_node;
}

template <typename T>
inline typename rb_tree<T>::iterator &rb_tree<T>::iterator::operator++() {
  iter_node = next(iter_node);

  return *this;
}

template <typename T>
inline typename rb_tree<T>::iterator rb_tree<T>::iterator::operator++(int) {
  rb_tree<T>::iterator tmp(*this);
  iter_node = next(tmp.iter_node);

  return tmp;
}

template <typename T>
typename rb_tree<T>::iterator &rb_tree<T>::iterator::operator=(
    const iterator &other) {
  iter_node = other.iter_node;
  return *this;
}

// protected rb_tree::iterator

template <typename T>
inline rb_tree<T>::iterator::iterator(node *nd) : iter_node(nd) {}

// private rb_tree::iterator

template <typename T>
typename rb_tree<T>::node *rb_tree<T>::iterator::next(node *nd) {
  if (nd == nullptr) return nullptr;
  if (nd->right) {
    nd = nd->right;
    while (nd->left) nd = nd->left;
  } else {
    node *parent = nd->parent;
    while (parent && nd == parent->right) {
      nd = parent;
      parent = parent->parent;
    }

    nd = parent;
  }

  return nd;
}

// rb_tree::const_iterator

// public rb_tree::const_iterator

template <typename T>
inline typename rb_tree<T>::const_reference
rb_tree<T>::const_iterator::operator*() const {
  if (!this->iter_node)
    throw std::runtime_error("Error: Operator* for nullptr");
  return this->iter_node->key;
}

// private rb_tree

template <typename T>
typename rb_tree<T>::node *rb_tree<T>::copy(node *nd, node *parent) {
  if (nd == nullptr) return nullptr;
  node *tmp = new node(nd->key, parent, nd->color);
  tmp->left = copy(nd->left, tmp);
  tmp->right = copy(nd->right, tmp);

  return tmp;
}

template <typename T>
void rb_tree<T>::free_node(node *nd) {
  if (nd == nullptr) return;
  free_node(nd->left);
  free_node(nd->right);

  delete nd;
}

template <typename T>
typename rb_tree<T>::node *rb_tree<T>::find(node *root, const_reference key) {
  if (root == nullptr) return nullptr;
  node *res = nullptr;
  if (root->key == key)
    res = root;
  else if (root->key < key)
    res = find(root->right, key);
  else
    res = find(root->left, key);
  return res;
}

template <typename T>
void rb_tree<T>::left_rotate(node *x) {
  node *y = x->right;
  x->right = y->left;

  if (y->left) y->left->parent = x;
  y->parent = x->parent;

  if (x->parent == nullptr)
    root = y;
  else {
    if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

template <typename T>
void rb_tree<T>::right_rotate(node *y) {
  node *x = y->left;
  y->left = x->right;

  if (x->right) x->right->parent = y;
  x->parent = y->parent;

  if (y->parent == nullptr)
    root = x;
  else {
    if (y == y->parent->right)
      y->parent->right = x;
    else
      y->parent->left = x;
  }

  x->right = y;
  y->parent = x;
}

template <typename T>
typename rb_tree<T>::iterator rb_tree<T>::balance_on_insert(node *nd) {
  bool stop = false;
  node *parent = nd->parent;
  while (nd != this->root && parent->color == RED && !stop) {
    node *grandparent = parent->parent;
    if (grandparent->left == parent) {
      node *uncle = grandparent->right;
      if (uncle && uncle->color == RED) {
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        nd = grandparent;
        parent = nd->parent;
      } else {
        if (parent->right == nd) {
          left_rotate(parent);
          std::swap(nd, parent);
        }
        right_rotate(grandparent);
        grandparent->color = RED;
        parent->color = BLACK;
        stop = true;
      }
    } else {
      node *uncle = grandparent->left;
      if (uncle && uncle->color == RED) {
        grandparent->color = RED;
        parent->color = BLACK;
        uncle->color = BLACK;
        nd = grandparent;
        parent = nd->parent;
      } else {
        if (parent->left == nd) {
          right_rotate(parent);
          std::swap(parent, nd);
        }
        left_rotate(grandparent);
        parent->color = BLACK;
        grandparent->color = RED;
        stop = true;
      }
    }
  }
  root->color = BLACK;
  return iterator(nd);
}

template <typename T>
void rb_tree<T>::balance_on_delete_black_pair_left(node *parent, node *child) {
  child = parent->left;
  if (child->color == RED) {
    node *gr_son = child->right;
    if ((gr_son && gr_son->left && gr_son->left->color == RED) ||
        (gr_son && gr_son->right && gr_son->right->color == RED)) {
      if (gr_son->left) {
        gr_son->left->color = BLACK;
        left_rotate(child);
        right_rotate(parent);
      } else if (gr_son->right) {
        parent->left->color = BLACK;
        gr_son->color = BLACK;
        gr_son->right->color = RED;
        parent->color = BLACK;
        right_rotate(parent);
        left_rotate(gr_son);
        right_rotate(parent);
      }
    } else if (gr_son && ((!gr_son->left && !gr_son->right) ||
                          (gr_son->left->color == BLACK &&
                           gr_son->right->color == BLACK))) {
      gr_son->color = RED;
      child->color = BLACK;
      right_rotate(parent);
    }
  } else {
    if (child->right && child->right->color == RED) {
      child->right->color = BLACK;
      left_rotate(child);
      right_rotate(parent);
    } else if (child->left && child->left->color == RED) {
      child->left->color = BLACK;
      right_rotate(parent);
    } else if ((!child->left && !child->right) ||
               (child->left->color == BLACK && child->right->color == BLACK)) {
      child->color = RED;
      balance_on_delete(parent->parent, parent);
    }
  }
}

template <typename T>
void rb_tree<T>::balance_on_delete_black_pair_right(node *parent, node *child) {
  child = parent->right;
  if (child->color == RED) {
    node *gr_son = child->left;
    if ((gr_son && gr_son->right && gr_son->right->color == RED) ||
        (gr_son && gr_son->left && gr_son->left->color == RED)) {
      if (gr_son->right) {
        gr_son->right->color = BLACK;
        right_rotate(child);
        left_rotate(parent);
      } else if (gr_son->left) {
        parent->right->color = BLACK;
        gr_son->color = BLACK;
        gr_son->left->color = RED;
        parent->color = BLACK;
        left_rotate(parent);
        right_rotate(gr_son);
        left_rotate(parent);
      }
    } else if (gr_son && ((!gr_son->left && !gr_son->right) ||
                          (gr_son->left->color == BLACK &&
                           gr_son->right->color == BLACK))) {
      gr_son->color = RED;
      child->color = BLACK;
      left_rotate(parent);
    }
  } else {
    if (child->left && child->left->color == RED) {
      child->left->color = BLACK;
      right_rotate(child);
      left_rotate(parent);
    } else if (child->right && child->right->color == RED) {
      child->right->color = BLACK;
      left_rotate(parent);
    } else if ((!child->left && !child->right) ||
               (child->left->color == BLACK && child->right->color == BLACK)) {
      child->color = RED;
      balance_on_delete(parent->parent, parent);
    }
  }
}

template <typename T>
void rb_tree<T>::balance_on_delete_red_par(node *parent, node *prev) {
  node *child;
  if (parent->left && (!parent->right || parent->right == prev)) {
    child = parent->left;
    if (child->right && child->right->color == RED) {
      parent->color = BLACK;
      left_rotate(child);
      right_rotate(parent);
    } else if (child && child->left && child->left->color == RED) {
      parent->color = BLACK;
      child->color = RED;
      child->left->color = BLACK;
      right_rotate(parent);
    } else if ((!child->left && !child->right) ||
               (child->left->color == BLACK && child->right->color == BLACK)) {
      parent->color = BLACK;
      child->color = RED;
    }
  } else if (parent->right && (!parent->left || parent->left == prev)) {
    child = parent->right;
    if (child->right && child->right->color == RED) {
      parent->color = BLACK;
      child->color = RED;
      child->right->color = BLACK;
      left_rotate(parent);
    } else if (child->left && child->left->color == RED) {
      parent->color = BLACK;
      right_rotate(child);
      left_rotate(parent);
    } else if ((!child->left && !child->right) ||
               (child->left->color == BLACK && child->right->color == BLACK)) {
      parent->color = BLACK;
      child->color = RED;
    }
  }
}

template <typename T>
void rb_tree<T>::balance_on_delete(node *parent, node *prev) {
  if (parent == nullptr) return;
  node *child = nullptr;
  if (parent->color == RED) {
    balance_on_delete_red_par(parent, prev);
  } else {
    if (parent->left && (!parent->right || parent->right == prev)) {
      balance_on_delete_black_pair_left(parent, child);
    } else if (parent->right && (!parent->left || parent->left == prev)) {
      balance_on_delete_black_pair_right(parent, child);
    }
  }
}

template <typename T>
inline typename rb_tree<T>::node *rb_tree<T>::get_iter_node(iterator it) {
  return it.iter_node;
}

}  // namespace my

#endif  // !_MY_CONTAINER_RBTREE_H
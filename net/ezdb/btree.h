#ifndef NET_EZDB_BTREE_H
#define NET_EZDB_BTREE_H

#include <algorithm>
#include <vector>
#include "net/ezdb/key.h"

namespace ezdb {

using ::std::lower_bound;
using ::std::vector;

template <typename T>
class BTreeNode {
 public:
  BTreeNode(int minimum_degree, bool is_leaf)
      : minimum_degree_(minimum_degree), is_leaf_(is_leaf) {}

  // No copy/assign
  BTreeNode<T>& operator=(const BTreeNode<T>&) = delete;
  BTreeNode<T>(const BTreeNode<T>&) = delete;

  // Visits every child node in our tree in order from lowest key to highest.
  void Traverse() const;

  // Searches for the node in our containng the given key.
  const BTreeNode<T> const* Search(const Key<T>& key) const;

  // Make the BTree friend of this so that we can access private members of this
  // class in BTree functions
  friend class BTree<T>;

 private:
  // Keys stored in our BTreeNode.
  vector<Key<T>> keys_;
  // Minimum degree (defines the range for number of keys).
  int minimum_degree_;
  // Child BTreeNodes.
  vector<BTreeNode*> children_;
  // Indicates that this node is a leaf node.
  bool is_leaf_;
};

template <typename T>
class BTree {
 public:
  BTree(int minimum_degree) : root_(nullptr), minimum_degree_(minimum_degree) {}

  // No copy/assign
  BTree<T>& operator=(const BTree<T>&) = delete;
  BTree<T>(const BTree<T>&) = delete;

  // Visits every node in our tree in order from lowest key to highest.
  void Traverse() const {
    if (root_ != nullptr) {
      root_->Traverse();
    }
  }

  // Searches for the node in our tree containng the given key.
  const BTreeNode<T> const* Search(const Key<T>& key) const {
    return (root_ == nullptr) ? nullptr : root_->Search(key);
  }

 private:
  // Pointer to the root of our BTree.
  BTreeNode* root_;
  // Minimum degree for nodes of our BTree.
  int minimum_degree_;
};

template <typename T>
void BTreeNode<T>::Traverse() const {
  // TODO(explodes): visit self.
  for (const auto child : children_) {
    if (!is_leaf_) {
      child.Traverse();
    }
    // TODO(explodes): visit.
  }
}

template <typename T>
const BTreeNode<T> const* BTreeNode<T>::Search(const Key<T>& key) const {
  if (keys_.empty()) {
    return nullptr;
  }

  // Find the first key greater than or equal to our target key.
  const auto search_result = lower_bound(keys_.begin(), keys_.end(), key); 

  if (search_result == keys_.end()) {
      return nullptr;
  } else if ( (*search_result).Value() == key)) {
        return this;
  } else {
      return 
  }


}

}  // namespace ezdb

#endif
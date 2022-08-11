// Copyright [Joao Victor Volpato]
#ifndef STRUCTURES_BINARY_TREE_H
#define STRUCTURES_BINARY_TREE_H

#include "array_list.h"

namespace structures {

template<typename T>
class BinaryTree {
public:
    ~BinaryTree();

    void insert(const T& data);

    void remove(const T& data);

    bool contains(const T& data) const;

    bool empty() const;

    std::size_t size() const;

    ArrayList<T> pre_order() const;

    ArrayList<T> in_order() const;

    ArrayList<T> post_order() const;

private:
    struct Node {
        explicit Node(const T& data_) {
            data = data_;
        }

        T data;
        Node* left{nullptr};
        Node* right{nullptr};

        void insert(const T& data_) {
            if (data_ < data) {
                // Insere a esquerda.
                if (left == nullptr) {
                    left = new Node(data_);
                } else {
                    left->insert(data_);
                }
            } else {
                // Insere a direita.
                if (right == nullptr) {
                    right = new Node(data_);
                } else {
                    right->insert(data_);
                }
            }
        }

        Node* remove(const T& data_) {
            if (data_ < data) {
                // Vai à esquerda.
                if (left != nullptr) {
                    left = left->remove(data_);
                    return this;
                } else {
                    return nullptr;
                }
            } else if (data_ > data) {
                // Vai à direita.
                if (right != nullptr) {
                    right = right->remove(data_);
                    return this;
                } else {
                    return nullptr;
                }
            } else {
                if (right != nullptr && left != nullptr) {
                    // 2 filhos.
                    Node* temp = right->minimum();
                    data = temp->data;
                    right = right->remove(data);
                    return this;
                } else if (right != nullptr) {
                    // 1 filho à direita.
                    Node* temp = right;
                    delete this;
                    return temp;
                } else if (left != nullptr) {
                    // 1 filho à esquerda.
                    Node* temp = left;
                    delete this;
                    return temp;
                } else {
                    // Folha.
                    delete this;
                    return nullptr;
                }
            }
        }

        bool contains(const T& data_) const {
            if (data_ == data) {
                return true;
            } else if (data_ < data && left != nullptr) {
                return left->contains(data_);
            } else if (data_ > data && right != nullptr) {
                return right->contains(data_);
            } else {
                return false;
            }
        }

        void pre_order(ArrayList<T>& v) const {
            // Root
            v.push_back(data);

            // Left
            if (left != nullptr) {
                left->pre_order(v);
            }
            // Right
            if (right != nullptr) {
                right->pre_order(v);
            }
        }

        void in_order(ArrayList<T>& v) const {
            // Left
            if (left != nullptr) {
                left->in_order(v);
            }
            // Root
            v.push_back(data);
            // Right
            if (right != nullptr) {
                right->in_order(v);
            }
        }

        void post_order(ArrayList<T>& v) const {
            // Left
            if (left != nullptr) {
                left->post_order(v);
            }
            // Right
            if (right != nullptr) {
                right->post_order(v);
            }
            // Root
            v.push_back(data);
        }

        Node* minimum() {
            if (left == nullptr) {
                return this;
            } else {
                return left->minimum();
            }
        }
    };

    Node* root{nullptr};
    std::size_t size_{0};
};

}  // namespace structures

#endif

template<typename T>
structures::BinaryTree<T>::~BinaryTree() {
    if (root != nullptr) {
        structures::ArrayList<T> list_nodes = pre_order();
        while (!list_nodes.empty()) {
            remove(list_nodes.pop_back());
        }
    }
}

template<typename T>
void structures::BinaryTree<T>::insert(const T& data) {
    if (!contains(data)) {
        if (root != nullptr) {
            root->insert(data);
        } else {
            root = new Node(data);
        }
        size_++;
    } else {
        throw std::out_of_range("Elemento ja existe na árvore.");
    }
}

template<typename T>
void structures::BinaryTree<T>::remove(const T& data) {
    if (root != nullptr) {
        if (contains(data)) {
            root->remove(data);
            size_--;
        } else {
            throw std::out_of_range("Elemento inválido.");
        }
    } else {
        throw std::out_of_range("Árvore não existe.");
    }
}

template<typename T>
bool structures::BinaryTree<T>::contains(const T& data) const {
    if (root != nullptr) {
        return root->contains(data);
    } else {
        return false;
    }
}

template<typename T>
bool structures::BinaryTree<T>::empty() const {
    return (size_ == 0);
}

template<typename T>
std::size_t structures::BinaryTree<T>::size() const {
    return size_;
}

template<typename T>
structures::ArrayList<T> structures::BinaryTree<T>::pre_order() const {
    structures::ArrayList<T> v;
    if (root != nullptr) {
        root->pre_order(v);
    }
    return v;
}

template<typename T>
structures::ArrayList<T> structures::BinaryTree<T>::in_order() const {
    structures::ArrayList<T> v;
    if (root != nullptr) {
        root->in_order(v);
    }
    return v;
}

template<typename T>
structures::ArrayList<T> structures::BinaryTree<T>::post_order() const {
    structures::ArrayList<T> v;
    if (root != nullptr) {
        root->post_order(v);
    }
    return v;
}

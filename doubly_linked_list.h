// Copyright 2022 joao victor volpato
#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>
#include <stdexcept>

namespace structures {

    template <typename T>
    class DoublyLinkedList {
    public:
        DoublyLinkedList();
        ~DoublyLinkedList();
        void clear();

        void push_back(const T &data);
        void push_front(const T &data);
        void insert(const T &data, std::size_t index);
        void insert_sorted(const T &data);

        T pop(std::size_t index);
        T pop_back();
        T pop_front();
        void remove(const T &data);

        bool empty() const;
        bool contains(const T &data) const;

        T &at(std::size_t index);
        const T &at(std::size_t index) const;

        std::size_t find(const T &data) const;
        std::size_t size() const;

    private:
        class Node {
        public:
            explicit Node(const T &data) : data_{data} {
            }
            Node(const T &data, Node *next);
            Node(const T &data, Node *prev, Node *next);

            T &data() {
                return data_;
            }
            const T &data() const {
                return data_;
            }

            Node *prev() {
                return prev_;
            }
            const Node *prev() const {
                return prev_;
            }

            void prev(Node *node) {
                prev_ = node;
            }

            Node *next() {
                return next_;
            }
            const Node *next() const {
                return next_;
            }

            void next(Node *node) {
                next_ = node;
            }

        private:
            T data_;
            Node *prev_{nullptr};
            Node *next_{nullptr};
        };

        Node *head;
        Node *tail;
        std::size_t size_;
    };

} // namespace structures

#endif

template <typename T>
structures::DoublyLinkedList<T>::DoublyLinkedList() {
    head = NULL;
    tail = NULL;
    size_ = 0;
}
template <typename T>
structures::DoublyLinkedList<T>::~DoublyLinkedList() {
    while (!empty()) {
        pop_front();
    }
}
template <typename T>
bool structures::DoublyLinkedList<T>::empty() const {
    return size_ == 0;
}
template <typename T>
void structures::DoublyLinkedList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}
template <typename T>
void structures::DoublyLinkedList<T>::push_front(const T &data) {
    Node *novo = new Node(data);
    if (head == NULL) {
        head = novo;
    } else {
        novo->next(head);
        novo->next()->prev(novo);
        head = novo;
    }
    size_++;
}
template <typename T>
void structures::DoublyLinkedList<T>::push_back(const T &data) {
    Node *novo = new Node(data);
    if (head == NULL) {
        tail = novo;
        head = novo;
    } else {
        novo->prev(tail);
        tail->next(novo);
        tail = novo;
    }
    size_++;
}
template <typename T>
void structures::DoublyLinkedList<T>::insert(const T &data, std::size_t index) {
    if (index > size_) {
        throw std::out_of_range("posicao invalida");
    }
    Node *novo = new Node(data);
    if (index == 0) {
        push_front(data);
    } else {
        Node *aux = head;
        for (std::size_t i = 0; i < index - 1; ++i) {
            aux = aux->next();
        }
        novo->next(aux->next());
        aux->next(novo);
        size_++;
    }
}
template <typename T>
T structures::DoublyLinkedList<T>::pop_front() {
    if (head == NULL || empty()) {
        throw std::out_of_range("posicao invalida");
    }
    T data = head->data();
    Node *aux = head;
    if (size_ == 1) {
        head = NULL;
    } else {
        head->next()->prev(NULL);
        head = head->next();
    }
    size_--;
    delete aux;
    return data;
}
template <typename T>
T structures::DoublyLinkedList<T>::pop_back() {
    if (head == NULL || empty()) {
        throw std::out_of_range("posicao invalida");
    }
    Node *ultimo = tail;
    T data = tail->data();
    if (size_ == 1) {
        tail = NULL;
        head = NULL;
    } else {
        tail->prev()->next(nullptr);
        tail = tail->prev();
    }
    size_--;
    delete ultimo;
    return data;
}
template <typename T>
T structures::DoublyLinkedList<T>::pop(std::size_t index) {
    if (index > size_ - 1 || index < 0) {
        throw std::out_of_range("posicao invalida");
    }
    if (index == 0) {
        return pop_front();
    } else if (index == size_ - 1) {
        return pop_back();
    }
    Node *aux = head;
    for (std::size_t i = 0; i < index; i++) {
        aux = aux->next();
    }
    aux->prev()->next(aux->next());
    aux->next()->prev(aux->prev());
    T data = aux->data();
    size_--;
    delete aux;
    return data;
}
template <typename T>
void structures::DoublyLinkedList<T>::insert_sorted(const T &data) {
    Node *novo = new Node(data);
    if (head == NULL) {
        head = novo;
        tail = novo;
    } else {
        Node *autal = head;
        while (autal->next() != NULL && autal->next()->data() < data) {
            autal = autal->next();
        }
        if (data > autal->data()) {
            novo->next(autal->next());
            novo->prev(autal);
            autal->next(novo);
        } else {
            novo->next(head);
            head = novo;
        }
    }
    size_++;
}
template <typename T>
void structures::DoublyLinkedList<T>::remove(const T &data) {
    if (head == NULL) {
        return;
    }
    if (head->data() == data) {
        pop_front();
    } else {
        std::size_t index = find(data);
        if (index != size_) {
            pop(index);
        }
    }
}
template <typename T>
std::size_t structures::DoublyLinkedList<T>::size() const {
    return size_;
}
template <typename T>
std::size_t structures::DoublyLinkedList<T>::find(const T &data) const {
    if (head == NULL) {
        return size_;
    }
    Node *aux = head;
    std::size_t index = 0;
    while (aux != NULL && aux->data() != data) {
        aux = aux->next();
        index++;
    }
    if (aux == NULL) {
        return size_;
    }
    return index;
}
template <typename T>
bool structures::DoublyLinkedList<T>::contains(const T &data) const {
    return find(data) != size_;
}
template <typename T>
T &structures::DoublyLinkedList<T>::at(std::size_t index) {
    if (index > size_) {
        throw std::out_of_range("posicao invalida");
    }
    Node *aux = head;
    for (std::size_t i = 0; i < index; ++i) {
        aux = aux->next();
    }
    return aux->data();
}
//! Copyright [year] <Copyright Owner>
#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>


namespace structures {

//! ...
template<typename T>
class LinkedList {
 public:
    //! ...
    LinkedList();  // construtor padrão
    //! ...
    ~LinkedList();  // destrutor
    //! ...
    void clear();  // limpar lista
    //! ...
    void push_back(const T& data);  // inserir no fim
    //! ...
    void push_front(const T& data);  // inserir no início
    //! ...
    void insert(const T& data, std::size_t index);  // inserir na posição
    //! ...
    void insert_sorted(const T& data);  // inserir em ordem
    //! ...
    T& at(std::size_t index);  // acessar um elemento na posição index
    //! ...
    T pop(std::size_t index);  // retirar da posição
    //! ...
    T pop_back();  // retirar do fim
    //! ...
    T pop_front();  // retirar do início
    //! ...
    void remove(const T& data);  // remover específico
    //! ...
    bool empty() const;  // lista vazia
    //! ...
    bool contains(const T& data) const;  // contém
    //! ...
    std::size_t find(const T& data) const;  // posição do dado
    //! ...
    std::size_t size() const;  // tamanho da lista

 private:
    class Node {  // Elemento
     public:
        explicit Node(const T& data):
            data_{data}
        {}

        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() {  // getter: dado
            return data_;
        }

        const T& data() const {  // getter const: dado
            return data_;
        }

        Node* next() {  // getter: próximo
            return next_;
        }

        const Node* next() const {  // getter const: próximo
            return next_;
        }

        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;
        Node* next_{nullptr};
    };

    Node* end() {  // último nodo da lista
        auto it = head;
        for (auto i = 1u; i < size(); ++i) {
            it = it->next();
        }
        return it;
    }
    
    Node* before_index(std::size_t index) {
        auto it = head;
        for (auto i = 1u; i < index; ++i) {
            it = it->next();
        }
        return it;
    }

    void insert(const T& data, Node* before); 

    Node* head{nullptr};
    std::size_t size_{0u};
};

}  // namespace structures

#endif

template<typename T>
structures::LinkedList<T>::LinkedList() {
    size_ = 0;
}

template<typename T>
structures::LinkedList<T>::~LinkedList() {
    clear();
}

template<typename T>
void structures::LinkedList<T>::clear() {
    while (!empty())
        pop_front();
}

template<typename T>
void structures::LinkedList<T>::push_back(const T& data) {
    insert(data, size_);
}

template<typename T>
void structures::LinkedList<T>::push_front(const T& data) {
    Node* aux;
    aux = new Node(data);
    if (aux == nullptr) {
        throw(std::out_of_range("Posição invalida"));
    } else {
        aux->next(head);
        head = aux;
        size_++;
    }
}

template<typename T>
void structures::LinkedList<T>::insert(const T& data, std::size_t index) {
    if (index > size_)
        throw std::out_of_range("Invalid index!");

    if (index == 0) {
        push_front(data);
    } else {
        Node* new_node = new Node(data);
        if (new_node == nullptr)
            throw std::out_of_range("Full list!");

        Node* before = before_index(index);
        Node* next = before->next();
        new_node->next(next);
        before->next(new_node);
        size_++;
    }
}

template<typename T>
void structures::LinkedList<T>::insert_sorted(const T& data) {
    if (empty()) {
        push_front(data);
    } else {
        Node *aux = head;
        std::size_t pos = size();
        for (std::size_t i = 0; i < size(); ++i) {
            if (!(data > aux->data())) {
                pos = i;
                break;
            }
        aux = aux->next();
        }
        if (pos == 0) {
            push_front(data);
        } else {
            insert(data, pos);
        }
    }
}

template<typename T>
T& structures::LinkedList<T>::at(std::size_t index) {
    if (index >= size())
        throw std::out_of_range("Posição invalida!");

    Node* aux = head;
    for (std::size_t i= 0; i < index; i++) {
        aux = aux->next();
    }
    return aux->data();
}

template<typename T>
T structures::LinkedList<T>::pop(std::size_t index) {
    if (empty())
        throw std::out_of_range("Lista vazia");
    if (index >= size())
        throw std::out_of_range("Posição invalida!");

    if (index == 0)
        return pop_front();
    Node* prev = head;
        for (std::size_t i = 1u; i < index; ++i) {
            prev = prev->next();
        }
    Node* aux = prev->next();
    T data = aux->data();
    prev->next(aux->next());
    size_--;
    delete aux;
    return data;
}

template<typename T>
T structures::LinkedList<T>::pop_back() {
    return pop(size_ - 1u);
}

template<typename T>
T structures::LinkedList<T>::pop_front() {
    if (empty())
        throw std::out_of_range("Empty list!");

    auto out = head;
    T data = out->data();
    head = out->next();
    size_--;
    delete out;
    return data;
}

template<typename T>
void structures::LinkedList<T>::remove(const T& data) {
    if (empty()) {
        throw std::out_of_range("Lista vazia");
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

template<typename T>
bool structures::LinkedList<T>::empty() const {
    if (size_ == 0) {
        return true;
    } else {
        return false;
    }
}

template<typename T>
bool structures::LinkedList<T>::contains(const T& data) const {
    return find(data) != size_;
}

template<typename T>
std::size_t structures::LinkedList<T>::find(const T& data) const {
    std::size_t index = 0u;
    Node* aux = head;
    while (index < size_) {
        if (aux->data() == data)
            break;
        aux = aux->next();
        index++;
    }
    return index;
}

template<typename T>
std::size_t structures::LinkedList<T>::size() const {
    return size_;
}

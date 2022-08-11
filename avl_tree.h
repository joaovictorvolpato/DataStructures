// Copyright [2022] <JOAOVOLP>

#include <algorithm>
#include "array_list.h"
namespace structures {

template <typename T>
class AVLTree {
public:
    AVLTree();

    ~AVLTree();

    void insert(const T &data);

    void remove(const T &data);

    bool contains(const T &data) const;

    bool empty() const;

    std::size_t size() const;

    int height() const;

    ArrayList<T> pre_order() const;

    ArrayList<T> in_order() const;

    ArrayList<T> post_order() const;

private:
    struct Node {
    public:
        //  Altura de um nodo até uma folha, é atualizado a cada inserção
        int height;
        T data;
        Node *father;
        Node *left;
        Node *right;

        explicit Node(const T &data) : data{data} {
            this->left = NULL;
            this->right = NULL;
            this->father = NULL;
        }

        Node() {
            this->left = NULL;
            this->right = NULL;
            this->father = NULL;
        }

        // Faz o insert Node na subárvore, a função sempre
        // retorna o novo root da subárvore
        Node *insertNode(const T &data_, Node *root, Node *father) {
            if (root == NULL) {  // É folha, aloca um novo node
                root = new Node(data_);
                root->father = father;
                root->updateHeight();
                return root;
            }

            Node *rotatedTree;
            if (root->data > data_) {
                // Manda inserir na arvore esquerda
                root->left = insertNode(data_, root->left, root);
                if ((getHeight(root->left) - getHeight(root->right)) > 1) {
                    // Caso tenha tido desbalanceamento
                    if (root->left->data > data_) {
                        // Se foi Left->Left a inserção
                        rotatedTree = simpleLeftRotation(root);
                    } else {
                        // Se foi Left->Right a inserção
                        rotatedTree = doubleLeftRotation(root);
                    }
                    return rotatedTree;
                } else {
                    root->updateHeight();
                }
            } else {
                root->right = insertNode(data_, root->right, root);   // Insere na árvore à direita
                if ((getHeight(root->right) - getHeight(root->left)) > 1) {
                    //  Caso tenha tido desbalanceamento
                    if (root->right->data > data_) {
                        //  Se foi Right->Left a inserção
                        rotatedTree = doubleRightRotation(root);
                    } else {
                        //  Se foi Right->Right a inserção
                        rotatedTree = simpleRightRotation(root);
                    }
                    return rotatedTree;
                } else {
                    root->updateHeight();
                }
            }

            return root;
        }

        bool contains(const T &data_) const;

        void pre_order(ArrayList<T> *list) {
            list->push_back(data);
            if (this->hasLeftChild())
                left->pre_order(list);
            if (this->hasRightChild())
                right->pre_order(list);
        }

        void in_order(ArrayList<T> *list) {
            if (this->hasLeftChild())
                this->left->in_order(list);
            list->push_back(data);
            // list->push_back(height);
            if (this->hasRightChild())
                this->right->in_order(list);
        }

        void post_order(ArrayList<T> *list) {
            if (this->hasLeftChild())
                left->post_order(list);
            if (this->hasRightChild())
                right->post_order(list);
            list->push_back(data);
        }

        Node *remove(const T &data_) {
            if (this->data == data_) {
                if (this->isLeaf()) {
                    this->resizeRemoveLeaf();  // Preciso deletar
                } else if (this->hasBothChildren()) {
                    Node *nodeToRemove = this->resizeTreeBothChildren();

                    return nodeToRemove;
                } else {
                    this->resizeRemoveWithOneChild();  // Preciso deletar
                }

                return this;

            } else if (this->data > data_) {
                if (this->left == NULL)
                    return NULL;

                return this->left->remove(data_);
            } else {
                if (this->right == NULL)
                    return NULL;

                return this->right->remove(data_);
            }
        }

        bool isLeaf() {
            return (this->left == NULL && this->right == NULL);
        }

        bool hasLeftChild() {
            return (this->left != NULL);
        }

        bool hasRightChild() {
            return (this->right != NULL);
        }

        bool hasBothChildren() {
            return (hasLeftChild() && hasRightChild());
        }
        void updateHeight() {
            int leftNodeHeight = getHeight(this->left);
            int rightNodeHeight = getHeight(this->right);
            int maior = leftNodeHeight;
            if (rightNodeHeight > maior)
                maior = rightNodeHeight;
            this->height = maior + 1;
        }

        Node *getSuccessor() {  // Get the data and delete the sucessor
            Node *successor = this->right;

            // Get the successor
            while (successor->left != NULL)
                successor = successor->left;

            // Resize the tree
            if (successor->isLeaf())
                successor->resizeRemoveLeaf();
            else
                successor->resizeRemoveWithOneChild();

            // Return the successor, already out of the tree
            return successor;
        }

        // Faz uma rotação simples à esquerda e retorna a nova raiz da subárvore
        Node *simpleLeftRotation(Node *oldRoot) {
            // Rotação
            Node *newRoot;
            // Copia o novo root para ser o left do anterior
            newRoot = oldRoot->left;
            // Pega o pai do root anterior
            newRoot->father = oldRoot->father;
            // Atualiza o filho no pai
            if (newRoot->father != NULL) {
                if (newRoot->father->left == oldRoot)
                    newRoot->father->left = newRoot;
                else
                    newRoot->father->right = newRoot;
            }
            // Seta a árvore direita do antigo para ser esquerda do novo
            oldRoot->left = newRoot->right;
            // Atualiza o father do ultimo root para ser o novo
            oldRoot->father = newRoot;
            // Atualiza o filho direito do novo root para ser o antigo
            newRoot->right = oldRoot;
            if (oldRoot->left != NULL)
                oldRoot->left->father = oldRoot;

            // Atualiza as alturas
            oldRoot->updateHeight();
            newRoot->updateHeight();

            // Retorna nova raiz
            return newRoot;
        }

        // Faz uma rotação simples à esquerda e retorna a nova raiz da subárvore
        Node *simpleRightRotation(Node *oldRoot) {
            // Rotação
            Node *newRoot;
            // Copia o right do anterior
            newRoot = oldRoot->right;
            // Copia o pai do anterior
            newRoot->father = oldRoot->father;
            // Atualiza o filho no pai se existir
            if (newRoot->father != NULL) {
                if (newRoot->father->left == oldRoot)
                    newRoot->father->left = newRoot;
                else
                    newRoot->father->right = newRoot;
            }
            // Atualiza os ponteiros do antigo root
            oldRoot->father = newRoot;
            oldRoot->right = newRoot->left;
            if (oldRoot->right != NULL)
                oldRoot->right->father = oldRoot;
            newRoot->left = oldRoot;

            // Atualiza as alturas
            oldRoot->updateHeight();
            newRoot->updateHeight();

            // Retorna nova raiz
            return newRoot;
        }

        Node *doubleRightRotation(Node *oldRoot) {
            // Rotação entre k1 e k2
            oldRoot->right = simpleLeftRotation(oldRoot->right);
            // Rotação entre k3 e k2
            return (simpleRightRotation(oldRoot));
        }

        Node *doubleLeftRotation(Node *oldRoot) {
            // Rotação entre k1 e k2
            oldRoot->left = simpleRightRotation(oldRoot->left);
            // Rotação entre k3 e k2
            return (simpleLeftRotation(oldRoot));
        }

        int getHeight(Node *node) {
            if (node == NULL)
                return -1;

            return node->height;
        }

        Node *resizeAfterDeletion(Node *root) {
            // Caso balanço caia para a esquerda
            if (getHeight(root->left) - getHeight(root->right) > 1) {
                // Left-Left -> Simples
                if (getHeight(root->left->left) > getHeight(root->left->right)) {
                    root = root->simpleLeftRotation(root);
                } else {
                    root = root->doubleLeftRotation(root);
                }
            }
            // Caso balanço caia para a direita
            if (getHeight(root->right) - getHeight(root->left) > 1) {
                // Right-Right -> Simples
                if (getHeight(root->right->right) > getHeight(root->right->left)) {
                    root = root->simpleRightRotation(root);
                } else {
                    root = root->doubleRightRotation(root);
                }
            }

            return root;
        }

        // Remove Node irá retornar o node que deve ser deletado
        Node *removeNode(const T &data_, Node *root) {
            if (root == NULL)
                return NULL;

            Node *nodeToRemove;
            if (root->data > data_) {  // Node a remover está para a esquerda
                nodeToRemove = removeNode(data_, root->left);
                root = resizeAfterDeletion(root);
                root->updateHeight();

                // Node a remover está para a direita
            } else if (root->data < data_) {
                nodeToRemove = removeNode(data_, root->right);
                root = resizeAfterDeletion(root);
                root->updateHeight();
            } else {
                // Se o root é o node a ser deletado
                if (root->isLeaf()) {
                    nodeToRemove = root;
                    if (root->father != NULL) {
                        if (root->father->left == root)
                            root->father->left = NULL;
                        else
                            root->father->right = NULL;
                    }

                } else if (root->hasBothChildren()) {
                    // Pega o sucessor desse node
                    Node *nodeToRemove = root->getSuccessor();
                    // Troca os dados
                    root->data = nodeToRemove->data;

                } else {
                    // Caso tenha só um filho atualiza os dados
                    root->resizeRemoveWithOneChild();
                    nodeToRemove = root;
                }
            }
            return nodeToRemove;
        }

        void resizeRemoveWithOneChild() {
            if (this->hasLeftChild()) {
                // Filho unico na esquerda
                this->left->father = this->father;  // Update father of child

                if (this->father->left == this)  // Se o current é filho esquerdo
                    this->father->left = this->left;
                else
                    this->father->right = this->left;

            } else {
                // Filho unico é na direita
                this->right->father = this->father;

                if (this->father->left == this)  // Se 
                    this->father->left = this->right;
                else
                    this->father->right = this->right;
            }
        }

    private:
        void resizeRemoveLeaf() {
            if (this->father == NULL)
                return;

            if (this->father->left == this)  // Se é o filho da esquerda
                this->father->left = NULL;
            else  // Se é o filho da direita
                this->father->right = NULL;
        }

        Node *resizeTreeBothChildren() {
            Node *successor = this->getSuccessor();

            this->data = successor->data;
            return successor;
        }
    };

    Node *root{nullptr};
    int quantNodes = 0;
    std::size_t size_;
};

}  // namespace structures

template <typename T>
structures::AVLTree<T>::~AVLTree() {
    while (!empty()) {
        remove(root->data);
    }
}

template <typename T>
structures::AVLTree<T>::AVLTree() {
    size_ = 0;
}

template <typename T>
void structures::AVLTree<T>::insert(const T &data) {
    if (root == NULL) {
        size_ = 1;
        quantNodes = 1;
        root = new Node(data);
        root->height = 1;
        return;
    }
    quantNodes++;
    if (root->data > data) {  // Jogando para a árvore esquerda
        root->left = root->insertNode(data, root->left, root);
        root->updateHeight();
        if ((root->getHeight(root->left) - root->getHeight(root->right)) > 1) {
            // Caso tenha tido desbalanceamento
            if (root->left->data > data) {  // Se foi Left->Left a inserção
                root = root->simpleLeftRotation(root);
            } else {  // Se foi Left->Right a inserção
                root = root->doubleLeftRotation(root);
            }
        }
    } else {  // Jogando para a árvore direita
        root->right = root->insertNode(data, root->right, root);
        root->updateHeight();
        if ((root->getHeight(root->right) - root->getHeight(root->left)) > 1) {
            // Caso tenha tido desbalanceamento
            if (root->right->data > data) {  // Se foi Right->Left a inserção
                root = root->doubleRightRotation(root);
            } else {  // Se foi Right->Right a inserção
                root = root->simpleRightRotation(root);
            }
        }
    }
    size_++;
}

template <typename T>
void structures::AVLTree<T>::remove(const T &data) {
    if (size_ == 0)
        return;

    Node *nodeToRemove;
    if (root->data > data) {  // Node a remover está para a esquerda
        nodeToRemove = root->removeNode(data, root->left);
        root = root->resizeAfterDeletion(root);
        root->updateHeight();

        // Node a remover está para a direita
    } else if (root->data < data) {
        nodeToRemove = root->removeNode(data, root->right);
        root = root->resizeAfterDeletion(root);
        root->updateHeight();
    } else {
        // Se o root é o node a ser deletado
        if (root->isLeaf()) {
            nodeToRemove = root;

        } else if (root->hasBothChildren()) {
            nodeToRemove = root->getSuccessor();  // Pega o sucessor desse node
            root->data = nodeToRemove->data;     // Troca os dados

        } else {
            nodeToRemove = root;  // Arruma a variável a ser deletada

            if (root->hasLeftChild()) {
                root->left->father = NULL;  // Remove os pointers para ela
                root = root->left;         // Atualiza o novo root
            } else {
                root->right->father = NULL;  // Remove os pointers para ela
                root = root->right;         // Atualiza o novo root
            }
        }
    }

    if (nodeToRemove != NULL) {
        printf("Rem\n");
        quantNodes--;
        printf("%d\n", quantNodes);
        delete nodeToRemove;
        size_--;
    }
    return;
}

template <typename T>
bool structures::AVLTree<T>::contains(const T &data) const {
    Node *current = root;
    while (current != NULL && current->data != data) {
        if (current->data > data) {  // Vai para a esquerda
            current = current->left;
        } else {  // Vai para a direita
            current = current->right;
        }
    }

    return current != NULL;
}

template <typename T>
int structures::AVLTree<T>::height() const {
    if (root == NULL)
        return -1;
    return root->height;
}

template <typename T>
bool structures::AVLTree<T>::empty() const {
    return size_ == 0;
}

template <typename T>
std::size_t structures::AVLTree<T>::size() const {
    return size_;
}

template <typename T>
structures::ArrayList<T> structures::AVLTree<T>::pre_order() const {
    structures::ArrayList<T> lista = structures::ArrayList<T>(size_);
    root->pre_order(&lista);
    return lista;
}

template <typename T>
structures::ArrayList<T> structures::AVLTree<T>::in_order() const {
    structures::ArrayList<T> lista = structures::ArrayList<T>(size_);
    root->in_order(&lista);
    return lista;
}

template <typename T>
structures::ArrayList<T> structures::AVLTree<T>::post_order() const {
    structures::ArrayList<T> lista = structures::ArrayList<T>(size_);
    root->post_order(&lista);
    return lista;
}
#ifndef AVLTREE_H
#define AVLTREE_H

#include "Book.h"
#include "User.h"
#include <iostream>
#include <string>
#include <algorithm>

template <typename T>
struct AVLNode {
    std::string key;
    T data;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const std::string& k, const T& d) : key(k), data(d), height(1), left(nullptr), right(nullptr) {}
};

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "ISBN: " << book.isbn << ", Título: " << book.title << ", Autor: " << book.author << ", Páginas: " << book.pages;
    return os;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "ID: " << user.userId << ", Nome: " << user.name << ", Contato: " << user.contactInfo;
    return os;
}

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int getHeight(AVLNode<T>* node);
    int getBalanceFactor(AVLNode<T>* node);
    AVLNode<T>* rotateRight(AVLNode<T>* node);
    AVLNode<T>* rotateLeft(AVLNode<T>* node);
    AVLNode<T>* balanceNode(AVLNode<T>* node);
    AVLNode<T>* insertNode(AVLNode<T>* node, const std::string& key, const T& data);
    AVLNode<T>* findMinNode(AVLNode<T>* node);
    AVLNode<T>* deleteNode(AVLNode<T>* node, const std::string& key);
    void inorderTraversal(AVLNode<T>* node);

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear(root); }
    void clear(AVLNode<T>* node);
    void insert(const std::string& key, const T& data);
    void remove(const std::string& key);
    T* search(const std::string& key);
    void displayInOrder();
};

template <typename T>
int AVLTree<T>::getHeight(AVLNode<T>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

template <typename T>
int AVLTree<T>::getBalanceFactor(AVLNode<T>* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template <typename T>
AVLNode<T>* AVLTree<T>::rotateRight(AVLNode<T>* node) {
    AVLNode<T>* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    newRoot->height = 1 + std::max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

template <typename T>
AVLNode<T>* AVLTree<T>::rotateLeft(AVLNode<T>* node) {
    AVLNode<T>* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    newRoot->height = 1 + std::max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

template <typename T>
AVLNode<T>* AVLTree<T>::balanceNode(AVLNode<T>* node) {
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (getBalanceFactor(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    } else if (balanceFactor < -1) {
        if (getBalanceFactor(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

template <typename T>
AVLNode<T>* AVLTree<T>::insertNode(AVLNode<T>* node, const std::string& key, const T& data) {
    if (node == nullptr)
        return new AVLNode<T>(key, data);

    if (key < node->key)
        node->left = insertNode(node->left, key, data);
    else if (key > node->key)
        node->right = insertNode(node->right, key, data);
    else
        node->data = data;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    return balanceNode(node);
}

template <typename T>
AVLNode<T>* AVLTree<T>::findMinNode(AVLNode<T>* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

template <typename T>
AVLNode<T>* AVLTree<T>::deleteNode(AVLNode<T>* node, const std::string& key) {
    if (node == nullptr)
        return nullptr;

    if (key < node->key)
        node->left = deleteNode(node->left, key);
    else if (key > node->key)
        node->right = deleteNode(node->right, key);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode<T>* temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else
                *node = *temp;
            delete temp;
        } else {
            AVLNode<T>* temp = findMinNode(node->right);
            node->key = temp->key;
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->key);
        }
    }

    if (node == nullptr)
        return nullptr;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    return balanceNode(node);
}

template <typename T>
void AVLTree<T>::clear(AVLNode<T>* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template <typename T>
void AVLTree<T>::insert(const std::string& key, const T& data) {
    root = insertNode(root, key, data);
}

template <typename T>
void AVLTree<T>::remove(const std::string& key) {
    root = deleteNode(root, key);
}

template <typename T>
T* AVLTree<T>::search(const std::string& key) {
    AVLNode<T>* current = root;
    while (current != nullptr) {
        if (key == current->key) {
            return &current->data;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

template <typename T>
void AVLTree<T>::displayInOrder() {
    inorderTraversal(root);
    std::cout << std::endl;
}

template <typename T>
void AVLTree<T>::inorderTraversal(AVLNode<T>* node) {
    if (node == nullptr)
        return;
    inorderTraversal(node->left);
    std::cout << "Key: " << node->key << ", Data: " << node->data << " ";
    inorderTraversal(node->right);
}

#endif /* AVLTREE_H */

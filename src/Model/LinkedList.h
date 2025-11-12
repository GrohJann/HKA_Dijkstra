//
// Created by ruvaak on 12.11.25.
//

//TODO: Check for errors

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template <typename T>
struct Node {
    T data;
    Node* previous;
    Node* next;
};



template <typename T>
class LinkedList {

private:
    Node<T>* head = nullptr;
    Node<T>* current = nullptr;
    Node<T>* last = nullptr;

public:
    LinkedList() : head(nullptr) {};

    /**
     * Appends data to start of List
     * @param data data to add
     */
    void addFirst(T data) {
        auto newNode = new Node<T>;
        newNode -> next = head;
        head = newNode;
    }

    /**
     * Appends data to end of List
     * @param data data to append
     */
    void add(T data) {
        auto newNode = new Node<T>(data);
        last -> next = newNode;
    }

    T popFirst() {
        T data = head -> data;
        head = head -> next;
        return data;
    }

    T popCurrent() {
        T data = current -> data;
        current -> previous -> next = current -> next;
        current -> next -> previous = current -> previous;
        if (current -> next != nullptr) {
            current = current -> next;
        } else {
            current = current -> previous;
        }
        return data;
    }

    T popLast() {
        T data = last -> data;
        last = last -> previous;
        last -> next = nullptr;
        return data;
    }

    T getFirst() { return head -> data; }

    T getCurrent() { return current -> data; }

    T getLast() { return last -> data; }

    void toFirst() {
        current = head;
    }

    T next() {
        current = current -> next;
        return current->data;
    }

    T previous() {
        current = current -> previous;
        return current->data;
    }

    void toLast() {
        current = last;
    }
};

#endif // LINKEDLIST_H

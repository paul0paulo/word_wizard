#include "linkedlist.h"

Node::Node(string key, int value) : key(key), value(value), next(nullptr) {} // node constructor

string Node::getKey()
{
    return key; // the word itself
}

int Node::getValue()
{
    return value; // number of times it appears in the text (frequency)
}

void Node::setKey(string key)
{
    this -> key = key; // update the word
}

void Node::setValue(int value)
{
    this -> value = value; // update the frequency of the word
}



LinkedList::LinkedList() : head(nullptr), size(0) {} // empty linked list constructor

LinkedList::~LinkedList() // linkedlist destructor
{
    clear(); // destructor to clear up all nodes
}

bool LinkedList::empty() const
{
    return head == nullptr; // check head points to null
}

unsigned int LinkedList::getSize() const
{
    return size; // return number of elements in the linked list
}

void LinkedList::insert(string key, int value)
{
    Node *inserter = head; // create a new pointer type node
    while (inserter != nullptr) // traverse through the linked list
    {
        if (inserter -> key == key) // if it exists in the linked list
        {
            inserter -> value = value; // update value
            return;                 
        }
        inserter = inserter -> next; // go to the next node
    }

    // key does not exist; create a new node
    Node *new_node = new Node(key, value); // create a new node
    new_node -> next = head; // insert it at the beginning of the linked list
    head = new_node; // make the new node the head of the linked list
    size++; // increment the size of the linked list
}

Node* LinkedList::find(string key)
{
    Node* finder = head; // create a new pointer finder of type node
    while (finder != nullptr) // traverse through a linked list
    {
        if (finder -> key == key) // if it exists in the linked list
        {
            return finder; // return key pointed by the findde
        }
        finder = finder -> next; // go to the next node
    }
    return nullptr; // key not found
}

void LinkedList::clear()
{
    while (head) // traverse through the whole list
    {
        Node* temp = head; // new pointer type node
        head = head -> next; // go to the next node
        delete temp; // delete the previos node
    }
    size = 0; // initialize size of the linked list to be zero
}
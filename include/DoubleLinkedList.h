#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <Arduino.h>  
#include <string.h>   

class Node {
public:
    int dataLength;
    String senderDevice;
    String data;
    Node* prev;
    Node* next;

    Node(int dataLength, const String& senderDevice, const String& data);
};

class DoublyLinkedList {
private:
    Node* head;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void insertAtHead(int dataLength, const String& senderDevice, const String& data);
    void insertAtTail(int dataLength, const String& senderDevice, const String& data);
    void deleteNode(Node* delNode);
    void deleteNodeBySender(const String& senderDevice);  // New function
    Node* findNodeBySender(const String& senderDevice);   // New function
    void updateNodeDataBySender(const String& senderDevice, const String& newData);
    int getDataLengthBySender(const String& senderDevice);
    void printList() const;
    void printListReverse() const;
    void clearList();
};

// === Node Constructor ===
Node::Node(int dataLength, const String& senderDevice, const String& data) 
    : dataLength(dataLength), senderDevice(senderDevice), data(data), prev(nullptr), next(nullptr) {}

// === DoublyLinkedList Constructor ===
DoublyLinkedList::DoublyLinkedList() : head(nullptr) {}

// === Destructor ===
DoublyLinkedList::~DoublyLinkedList() {
    clearList();
}

// === Insert at Head ===
void DoublyLinkedList::insertAtHead(int dataLength, const String& senderDevice, const String& data) {
    Node* newNode = new Node(dataLength, senderDevice, data);
    if (!head) {
        head = newNode;
        return;
    }
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
}

// === Insert at Tail ===
void DoublyLinkedList::insertAtTail(int dataLength, const String& senderDevice, const String& data) {
    Node* newNode = new Node(dataLength, senderDevice, data);
    if (!head) {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
}

// === Delete a Node by senderDevice ===
void DoublyLinkedList::deleteNodeBySender(const String& senderDevice) {
    Node* temp = head;
    while (temp) {
        if (temp->senderDevice == senderDevice) {  // Find node
            if (temp->prev) temp->prev->next = temp->next;
            if (temp->next) temp->next->prev = temp->prev;
            if (temp == head) head = temp->next;
            delete temp;
            Serial.println("✅ Node deleted successfully!");
            return;
        }
        temp = temp->next;
    }
    Serial.println("⚠️ Node with given senderDevice not found!");
}

// === Find Node by senderDevice ===
Node* DoublyLinkedList::findNodeBySender(const String& senderDevice) {
    Node* temp = head;
    while (temp) {
        if (temp->senderDevice == senderDevice) {
            return temp;  // Return pointer to the node
        }
        temp = temp->next;
    }
    return nullptr;  // Return nullptr if not found
}

// === Update Node Data by senderDevice ===
void DoublyLinkedList::updateNodeDataBySender(const String& senderDevice, const String& newData) {
    Node* temp = head;
    while (temp) {
        if (temp->senderDevice == senderDevice) {
            temp->data = newData;
            Serial.println("✅ Node updated successfully!");
            return;
        }
        temp = temp->next;
    }
    Serial.println("⚠️ Node with given senderDevice not found!");
}

// === Get Data Length by senderDevice ===
int DoublyLinkedList::getDataLengthBySender(const String& senderDevice) {
    Node* temp = head;
    while (temp) {
        if (temp->senderDevice == senderDevice) {
            return temp->dataLength;
        }
        temp = temp->next;
    }
    Serial.println("⚠️ Node with given senderDevice not found!");
    return -1;
}

// === Print List (Forward) ===
void DoublyLinkedList::printList() const {
    Node* temp = head;
    Serial.println("Doubly Linked List (Forward):");
    while (temp) {
        Serial.print("[Data Length: ");
        Serial.print(temp->dataLength);
        Serial.print(", Sender Device: ");
        Serial.print(temp->senderDevice);
        Serial.print(", Data: ");
        Serial.print(temp->data);
        Serial.print("] <-> ");
        temp = temp->next;
    }
    Serial.println("NULL");
}

// === Print List (Reverse) ===
void DoublyLinkedList::printListReverse() const {
    if (!head) return;

    Node* temp = head;
    while (temp->next) {
        temp = temp->next;
    }

    Serial.println("Doubly Linked List (Reverse):");
    while (temp) {
        Serial.print("[Data Length: ");
        Serial.print(temp->dataLength);
        Serial.print(", Sender Device: ");
        Serial.print(temp->senderDevice);
        Serial.print(", Data: ");
        Serial.print(temp->data);
        Serial.print("] <-> ");
        temp = temp->prev;
    }
    Serial.println("NULL");
}

// === Clear the List ===
void DoublyLinkedList::clearList() {
    Node* temp = head;
    while (temp) {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }
    head = nullptr;
}

#endif // DOUBLY_LINKED_LIST_H

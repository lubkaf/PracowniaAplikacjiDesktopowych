    #include <iostream>
    using namespace std;

    struct Node {
        int data;
        Node* next;
    };

    void AddElement(int data, Node** start);
    void PrintList(Node* start);
    void AddElementAtTheEnd(int data, Node** start);
    void DeleteElement(int data, Node** start);
    void ClearList(Node** start);

    int main() {
        Node* start = nullptr;

        AddElement(1, &start);
        AddElement(2, &start);
        AddElement(3, &start);
        AddElementAtTheEnd(0, &start);

        DeleteElement(2, &start);

        PrintList(start);

        cout<<endl;

        ClearList(&start);
        PrintList(start);

        cout<<endl;
        AddElementAtTheEnd(10, &start);
        AddElement(-10, &start);
        PrintList(start);

        ClearList(&start);

        return 0;
    }

    void AddElement(int data, Node** start) {
        Node* newNode = new Node;
        newNode->data = data;
        newNode->next = *start;
        *start = newNode;
    }
    void AddElementAtTheEnd(int data, Node** start) {
        Node* newNode = new Node;
        newNode->data = data;
        if (*start == nullptr) {
            *start = newNode;
            return;
        }
        Node* current = *start;
        newNode->next = nullptr;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    void DeleteElement(int data, Node **start) {

        if (*start == nullptr) {
            return;
        }
        //usuwanie elementu jeżeli występuje w pierwszym elemencie
        Node* temp = *start;
        if ((*start)->data == data) {
            *start = (*start)->next;
            delete temp;
            return;
        }
        //usuwanie jeżeli wystepuje w każdym innym elemencie
        Node* current = *start;
        while (current->next != nullptr && current->next->data != data) {
            current = current->next;
        }
        if (current->next == nullptr)
            return;
        temp = current->next;
        current->next = temp->next;
        delete temp;

    }
    void ClearList(Node **start) {
        Node* current = *start;
        while (current != nullptr) {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
        *start = nullptr;
    }
    void PrintList(Node* start) {
        Node* current = start;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
    }




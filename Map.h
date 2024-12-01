#ifndef __MAP_
#define __MAP_

template <typename V>
struct Node {
    const char* key;
    V value;
    Node* next;
};

template <typename V>
class Map {
    private:
    Node<V>* first;
    Node<V>* last;
    
    public:
    Map();
    ~Map();

    void add(const char* key, V value);
    V* get(const char* key);
    int size();
};

template <typename V>
Map<V>::Map() {
    first = nullptr;
    last = nullptr;
}

template <typename V>
Map<V>::~Map() {

}

template <typename V>
void Map<V>::add(const char* key, V value) {
    Node<V>* newNode = new Node<V>;

    newNode->key = key;
    newNode->value = value;
    newNode->next = nullptr;

    if(first != nullptr) {
        last->next = newNode;
    } else {
        first = newNode;
    }

    last = newNode;
}

template <typename V>
V* Map<V>::get(const char* key) {
    Node<V>* curent = first;

    while (curent != nullptr) {
        if(strcmp(curent->key, key) == 0) 
            return &curent->value;
        
        curent = curent->next;
    }

    return nullptr;
}

template <typename V>
int Map<V>::size() {
    Node<V>* curent = first;
    int count = 0;

    while (curent != nullptr) {
        count ++;

        curent = curent->next;
    }

    return count;
}


#endif
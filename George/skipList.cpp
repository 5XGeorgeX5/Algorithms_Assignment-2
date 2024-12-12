#ifndef SKIPLIST_CPP
#define SKIPLIST_CPP

#include <iostream>
#include <vector>
#include <ctime>

using std::cout, std::vector, std::pair;

template <typename Key, typename Value>
class SkipList
{
private:
    struct Node
    {
        Key key;
        Value value;
        Node *next = nullptr;
        Node *down = nullptr;
        Node() {}
        Node(Key key, Value value) : key(key), value(value) {}
    };
    const int maxNumberOfLevels = 10;
    Node *head;

    int getHeight()
    {
        int height = 1;
        while (height < maxNumberOfLevels && (rand() % 2) == 1)
        {
            ++height;
        }
        return height;
    }

    Node *createTower(int height, Key key, Value value)
    {
        Node *top = nullptr;
        for (int i = 0; i < height; i++)
        {
            Node *tmp = new Node(key, value);
            tmp->down = top;
            top = tmp;
        }
        return top;
    }

public:
    SkipList()
    {
        std::srand(static_cast<unsigned int>(std::time(0)));
        head = new Node();
        Node *current = head;
        for (int i = 1; i < maxNumberOfLevels; i++)
        {
            current->down = new Node();
            current = current->down;
        }
    }

    void insert(Key key, Value value)
    {
        int height = getHeight();
        int currentLevel = maxNumberOfLevels;
        Node *current = head;
        Node *newNode = createTower(height, key, value);
        while (current)
        {
            while (current->next && current->next->key < key)
            {
                current = current->next;
            }
            if (currentLevel <= height)
            {
                newNode->next = current->next;
                current->next = newNode;
                newNode = newNode->down;
            }
            current = current->down;
            currentLevel--;
        }
    }

    Value remove(Key key)
    {
        Node *current = head;
        Value value;
        bool found = false;

        while (current)
        {
            while (current->next && current->next->key < key)
            {
                current = current->next;
            }
            if (current->next && current->next->key == key)
            {
                Node *tmp = current->next;
                current->next = current->next->next;
                value = tmp->value;
                found = true;
                delete tmp;
            }
            current = current->down;
        }
        if (!found)
        {
            throw std::runtime_error("Key not found");
        }
        return value;
    }

    Value search(Key key)
    {
        Node *current = head;

        while (current->down)
        {
            while (current->next && current->next->key < key)
            {
                current = current->next;
            }
            if (current->next && current->next->key == key)
            {
                return current->next->value;
            }
            current = current->down;
        }
        throw std::runtime_error("Key not found");
    }

    void display()
    {
        cout << "skip List:\n";
        Node *current = head;
        int currentLevel = maxNumberOfLevels;

        while (current)
        {
            cout << "Level " << currentLevel << ": ";
            Node *tmp = current->next;
            while (tmp)
            {
                cout << tmp->key << ": " << tmp->value << " | ";
                tmp = tmp->next;
            }
            cout << '\n';
            current = current->down;
            currentLevel--;
        }
    }

    vector<pair<Key, Value>> getArray()
    {
        vector<pair<Key, Value>> result;
        Node *current = head;
        while (current->down)
        {
            current = current->down;
        }
        current = current->next;
        while (current != nullptr)
        {
            result.push_back({current->key, current->value});
            current = current->next;
        }
        return result;
    }
};

#endif

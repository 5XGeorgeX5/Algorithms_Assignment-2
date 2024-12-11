#ifndef SKIPLIST_CPP
#define SKIPLIST_CPP

#include <iostream>
#include <vector>
#include <ctime>

using std::cout, std::endl, std::vector, std::pair;

template <typename Key, typename Value>
class SkipList
{
private:
    struct Node
    {
        Key key;
        Value value;
        vector<Node *> next;
        Node(Key key, Value value, int Level) : key(key), value(value), next(Level + 1, nullptr) {}
    };
    const int maxNumberOfLevel = 5;
    Node *head;
    int Level;

public:
    SkipList()
    {
        std::srand(static_cast<unsigned int>(std::time(0)));
        head = new Node(Key{}, Value{}, maxNumberOfLevel);
        Level = 0;
    }

    void insert(Key key, Value value)
    {
        int newLevel{0};

        while (newLevel < maxNumberOfLevel && (rand() % 2) == 1)
        {
            ++newLevel;
        }

        if (Level < newLevel)
        {
            head->next.resize(newLevel + 1, nullptr);
            Level = newLevel;
        }

        Node *current{head};
        vector<Node *> Update(Level + 1, nullptr);

        for (int i{Level}; i >= 0; --i)
        {
            while (current->next[i] && current->next[i]->key < key)
            {
                current = current->next[i];
            }
            Update[i] = current;
        }

        current = current->next[0];

        if (current == nullptr || current->key != key)
        {
            Node *newNode{new Node(key, value, Level)};

            for (int i{0}; i <= newLevel; ++i)
            {
                newNode->next[i] = Update[i]->next[i];
                Update[i]->next[i] = newNode;
            }
        }
    }

    Value remove(Key key)
    {
        Node *current{head};

        vector<Node *> Update(Level + 1, nullptr);

        for (int i{Level}; i >= 0; --i)
        {
            while (current->next[i] && current->next[i]->key < key)
            {
                current = current->next[i];
            }
            Update[i] = current;
        }

        current = current->next[0];

        if (current != nullptr && current->key == key)
        {
            for (int i{0}; i <= Level; ++i)
            {
                if (Update[i]->next[i] != current)
                {
                    break;
                }
                else
                {
                    Update[i]->next[i] = current->next[i];
                }
            }
            Value value = current->value;

            delete current;

            while (Level > 0 && head->next[Level] == nullptr)
            {
                --Level;
            }
            return value;
        }
        else
        {
            throw std::runtime_error("Key not found");
        }
    }

    Value search(Key key)
    {
        Node *current{head};

        for (int i{Level}; i >= 0; --i)
        {
            while (current->next[i] && current->next[i]->key < key)
            {
                current = current->next[i];
            }
        }

        current = current->next[0];

        if (current != nullptr && current->key == key)
        {
            return current->value;
        }
        else
        {
            throw std::runtime_error("Key not found");
        }
    }

    void display()
    {
        cout << "skip List:" << endl;

        for (int i{Level}; i >= 0; --i)
        {
            Node *current{head->next[i]};

            cout << "Level " << i << ": ";

            while (current != nullptr)
            {
                cout << current->key << ": " << current->value << '\n';
                current = current->next[i];
            }
        }
    }
    vector<pair<Key, Value>> getArray()
    {
        vector<pair<Key, Value>> result;
        Node *current{head->next[0]};
        while (current != nullptr)
        {
            result.push_back({current->key, current->value});
            current = current->next[0];
        }
        return result;
    }
};

#endif
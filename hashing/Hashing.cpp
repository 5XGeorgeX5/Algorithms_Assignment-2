#include <bits/stdc++.h>
#include <functional>
using namespace std;

template <typename T>
class Hash
{
protected:
    static const int size = 97;
    function<int(int)> hashFunction;

    static int divisionHash(int key)
    {
        return key % size;
    }

    static int multiplicationHash(int key)
    {
        double A = 0.6180339887;
        return floor(size * fmod(key * A, 1));
    }

    static int midSquareHash(int key)
    {
        long long squared = (long long)key * key;
        string s = to_string(squared);
        int num_of_digits = (int)s.size();
        if (num_of_digits <= 2)
            return squared;
        return stoi(s.substr((num_of_digits - 2) / 2, 2));
    }

    static int foldingHash(int key)
    {
        string s = to_string(key);
        int index = 0;
        for (size_t i = 0; i < s.size(); i += 2)
        {
            if (i + 2 <= s.size())
            {
                index += stoi(s.substr(i, 2));
            }
            else
            {
                index += stoi(s.substr(i));
            }
        }
        return index % size;
    }

public:
    Hash(int method)
    {
        hashFunction = divisionHash;
        if (method == 1)
            hashFunction = multiplicationHash;
        if (method == 2)
            hashFunction = midSquareHash;
        if (method == 3)
            hashFunction = foldingHash;
    }
    virtual void insert(int key, const T &value) = 0;
    virtual T find(int key) = 0;
};

template <typename T>
class chainingMethodHash : public Hash<T>
{
private:
    map<int, T> data[Hash<T>::size];

public:
    chainingMethodHash(int method) : Hash<T>(method) {}

    void insert(int key, const T &value)
    {
        int index = this->hashFunction(key);
        data[index].insert({key, value});
    }

    T find(int key)
    {
        int index = this->hashFunction(key);
        if (data[index].find(key) != data[index].end())
            return data[index][key];
        return T();
    }
};

template <typename T>
class OpenAddressing : public Hash<T>
{
private:
    pair<int, T> data[Hash<T>::size];
    vector<bool> vis;
    int type;

    int linearProbing(int index, int key)
    {
        int start = index;
        while (true)
        {
            if (data[index].first == -1 || data[index].first == key)
            {
                return index;
            }
            index++;
            index %= this->size;
            if (index == start)
                return -1;
        }
    }

    int quadraticProbing(int index, int key)
    {
        vector<int> passed;
        for (int i{};; ++i)
        {
            int curIndex = (index + i * i) % this->size;
            passed.emplace_back(curIndex);
            if (vis[curIndex])
                break;
            vis[curIndex] = true;
            if (data[curIndex].first == -1 || data[curIndex].first == key)
            {
                for (auto &idx : passed)
                    vis[idx] = false;
                return curIndex;
            }
        }
        for (auto &idx : passed)
            vis[idx] = false;
        return -1;
    }

public:
    OpenAddressing(int method, int type) : Hash<T>(method)
    {
        vis.resize(this->size);
        for (int i = 0; i < this->size; ++i)
            data[i].first = -1;
        this->type = type;
    }
    void insert(int key, const T &value)
    {
        int index = this->hashFunction(key);
        if (type == 0)
            index = linearProbing(index, value);
        else
            index = quadraticProbing(index, value);
        if (index != -1)
            data[index] = {key, value};
    }

    T find(int key)
    {
        int index = this->hashFunction(key);
        if (type == 0)
            index = linearProbing(index, key);
        else
            index = quadraticProbing(index, key);
        if (index != -1 && data[index].first == key)
        {
            return data[index].second;
        }
        return T();
    }
};

template <typename T>
class DoubleHash : public Hash<T>
{
private:
    pair<int, T> data[Hash<T>::size];
    vector<int> vis;
    const int PRIME = 997;

    int Hash2(long long number)
    {
        return int(PRIME - (number % PRIME));
    }

    int probe(int h1, int h2, int key)
    {
        vector<int> passed;
        for (int i{};; ++i)
        {
            int curIndex = (h1 + i * h2) % this->size;
            if (vis[curIndex])
                break;
            vis[curIndex] = true;
            passed.emplace_back(curIndex);
            if (data[curIndex].first == -1 || data[curIndex].first == key)
            {
                for (auto &idx : passed)
                    vis[idx] = false;
                return curIndex;
            }
        }
        for (auto &idx : passed)
            vis[idx] = false;
        return -1;
    }

public:
    DoubleHash(int method) : Hash<T>(method)
    {
        vis.resize(this->size);
        for (int i = 0; i < this->size; ++i)
            data[i].first = -1;
    }

    void insert(int key, const T &value)
    {
        int h1 = this->hashFunction(key);
        int h2 = Hash2(key);
        int idx = probe(h1, h2, key);
        if (idx != -1)
            data[idx] = {key, value};
    }

    T find(int key)
    {
        int h1 = this->hashFunction(key);
        int h2 = Hash2(key);
        int idx = probe(h1, h2, key);
        if (idx != -1 && data[idx].first == key)
        {
            return data[idx].second;
        }
        return T();
    }
};

template <typename T>
class HashTable
{
private:
    Hash<T> *hashTable;

public:
    HashTable(int method, int type)
    {
        if (type == 0)
            hashTable = new chainingMethodHash<T>(method);
        else if (type == 1)
            hashTable = new OpenAddressing<T>(method, 0);
        else if (type == 2)
            hashTable = new OpenAddressing<T>(method, 1);
        else
            hashTable = new DoubleHash<T>(method);
    }

    void insert(int key, const T &value)
    {
        hashTable->insert(key, value);
    }

    T find(int key)
    {
        return hashTable->find(key);
    }
};

int main()
{
    HashTable<int> hashTable(0, 0);
    for (int i = 0; i < 100; i++)
    {
        hashTable.insert(i, 100 + i);
    }
    for (int i = 0; i < 100; i++)
    {
        cout << hashTable.find(i) << '\n';
    }
    return 0;
}
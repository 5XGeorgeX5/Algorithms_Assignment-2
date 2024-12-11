#include <iostream>
using namespace std;

template <typename T>
class RedBlackTree
{
private:
    enum Color
    {
        RED,
        BLACK
    };
    struct Node
    {
        T data;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        Color color = RED;
        Node(const T &value) : data(value) {}

        Node *getSibling()
        {
            return isLeftChild() ? parent->right : parent->left;
        }
        bool isLeftChild()
        {
            return parent->left == this;
        }
        bool hasBlackChildren()
        {
            bool isLeftChildBlack = left == nullptr || left->color == BLACK;
            bool isRightChildBlack = right == nullptr || right->color == BLACK;
            return isLeftChildBlack && isRightChildBlack;
        }
    };

    Node *root = nullptr;

    void leftRotate(Node *node)
    {
        Node *newRoot = node->right;
        Node *newRight = newRoot->left;

        if (node->parent == nullptr)
            root = newRoot;
        else if (node->isLeftChild())
            node->parent->left = newRoot;
        else
            node->parent->right = newRoot;
        newRoot->left = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        node->right = newRight;
        if (newRight != nullptr)
            newRight->parent = node;
    }

    void rightRotate(Node *node)
    {
        Node *newRoot = node->left;
        Node *newLeft = newRoot->right;

        if (node->parent == nullptr)
            root = newRoot;
        else if (node->isLeftChild())
            node->parent->left = newRoot;
        else
            node->parent->right = newRoot;
        newRoot->right = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        node->left = newLeft;
        if (newLeft != nullptr)
            newLeft->parent = node;
    }

    Node *findNode(Node *node, const T &value)
    {
        if (node == nullptr)
            return nullptr;
        if (value < node->data)
        {
            return findNode(node->left, value);
        }
        if (node->data < value)
        {
            return findNode(node->right, value);
        }
        return node;
    }

    Node *findParent(Node *node, const T &value)
    {
        if (value < node->data)
        {
            if (node->left == nullptr)
                return node;
            return findParent(node->left, value);
        }
        if (node->right != nullptr)
        {
            return findParent(node->right, value);
        }
        return node;
    }

    Node *insertNode(const T &value)
    {
        Node *newNode = new Node(value);

        if (root == nullptr)
        {
            root = newNode;
            return root;
        }
        Node *parent = findParent(root, value);
        newNode->parent = parent;
        if (value < parent->data)
            parent->left = newNode;
        else
            parent->right = newNode;
        return newNode;
    }

    void recolorAndRotate(Node *node)
    {
        if (node == root || node->parent->color == BLACK)
        {
            root->color = BLACK;
            return;
        }
        Node *parent = node->parent;
        Node *grandParent = parent->parent;
        Node *uncle = parent->isLeftChild() ? grandParent->right : grandParent->left;

        if (uncle && uncle->color == RED)
        {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandParent->color = RED;
            recolorAndRotate(grandParent);
            return;
        }
        if (parent->isLeftChild())
        {
            if (!node->isLeftChild())
            {
                leftRotate(parent);
                swap(node, parent);
            }
            rightRotate(grandParent);
        }
        else
        {
            if (node->isLeftChild())
            {
                rightRotate(parent);
                swap(node, parent);
            }
            leftRotate(grandParent);
        }
        parent->color = BLACK;
        grandParent->color = RED;
    }

    Node *getNodeToDelete(Node *node)
    {
        if (!node->left && !node->right)
            return node;
        if (!node->left)
        {
            node->data = node->right->data;
            return node->right;
        }
        Node *current = node->left;
        while (current->right)
        {
            current = current->right;
        }
        node->data = current->data;
        if (current->left)
        {
            current->data = current->left->data;
            return current->left;
        }
        return current;
    }

    Node *getFarNephew(Node *sibling)
    {
        return sibling->isLeftChild() ? sibling->left : sibling->right;
    }

    void fixDoubleBlack(Node *node)
    {
        if (node == root)
        {
            return;
        }
        Node *sibling = node->getSibling();
        if (sibling->color == RED)
        {
            node->parent->color = RED;
            sibling->color = BLACK;
            if (node->isLeftChild())
            {
                leftRotate(node->parent);
            }
            else
            {
                rightRotate(node->parent);
            }
            fixDoubleBlack(node);
            return;
        }
        if (sibling->color == BLACK && sibling->hasBlackChildren())
        {
            sibling->color = RED;
            if (node->parent->color == RED)
            {
                node->parent->color = BLACK;
            }
            else
            {
                fixDoubleBlack(node->parent);
            }
            return;
        }
        Node *farNephew = getFarNephew(sibling);
        if (!farNephew || farNephew->color == BLACK)
        {
            sibling->color = RED;
            if (sibling->isLeftChild())
            {
                sibling->right->color = BLACK;
                leftRotate(sibling);
            }
            else
            {
                sibling->left->color = BLACK;
                rightRotate(sibling);
            }
            farNephew = sibling;
            sibling = node->getSibling();
        }
        if (node->parent->color == RED)
        {
            sibling->color = RED;
            node->parent->color = BLACK;
        }
        if (node->isLeftChild())
        {
            leftRotate(node->parent);
        }
        else
        {
            rightRotate(node->parent);
        }
        farNephew->color = BLACK;
    }

    void deleteFixup(Node *node)
    {
        if (node == root)
        {
            delete root;
            root = nullptr;
            return;
        }
        if (node->color == BLACK)
        {
            fixDoubleBlack(node);
        }
        if (node->isLeftChild())
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
        delete node;
        return;
    }

    void printInOrder(Node *node)
    {
        if (node == nullptr)
            return;
        printInOrder(node->left);
        cout << node->data << " ";
        printInOrder(node->right);
    }

    void printInOrderRight(Node *node)
    {
        if (node == nullptr)
            return;
        printInOrderRight(node->right);
        cout << node->data << " ";
        printInOrderRight(node->left);
    }

    void freeTree(Node *node)
    {
        if (node == nullptr)
            return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }

public:
    void insert(const T &value)
    {
        Node *newNode = insertNode(value);
        recolorAndRotate(newNode);
    }

    void remove(const T &value)
    {
        Node *node = findNode(root, value);
        if (node == nullptr)
            return;
        Node *replacement = getNodeToDelete(node);
        deleteFixup(replacement);
    }

    void printAscending()
    {
        printInOrder(root);
        cout << endl;
    }

    void printDescending()
    {
        printInOrderRight(root);
        cout << endl;
    }

    ~RedBlackTree()
    {
        freeTree(root);
    }
};

int main()
{
    RedBlackTree<int> tree;
    for (int i = 1; i <= 20; i++)
    {
        tree.insert(i);
    }
    for (int i = 1; i <= 10; i++)
    {
        tree.remove(i);
    }
    tree.printAscending();
}
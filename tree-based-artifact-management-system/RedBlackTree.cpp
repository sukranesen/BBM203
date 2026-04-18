#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:

    if (node == nullptr)
        return;

    clear(node->left);
    clear(node->right);

    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name

   // return nullptr;
    if (node == nullptr)
            return nullptr;

        if (fullName == node->data.fullName)
            return node;

        if (fullName < node->data.fullName)
            return find(node->left, fullName);

        return find(node->right, fullName);
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise
    //return false;//temporary, you may delete this
    ResearcherNode *node = new ResearcherNode(researcher);

    bool inserted = false;
    root = bstInsert(root, node, inserted);

    if (!inserted)
    {
        delete node;
        return false;
    }

    insertFixup(node);

    if (root != nullptr)
        root->color = BLACK;

    return true;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).

    if (current == nullptr)
    {
        inserted = true;
        return node;
    }

    if (node->data.fullName < current->data.fullName)
    {
        ResearcherNode *child = bstInsert(current->left, node, inserted);
        current->left = child;
        if (child != nullptr)
            child->parent = current;
    }
    else if (node->data.fullName > current->data.fullName)
    {
        ResearcherNode *child = bstInsert(current->right, node, inserted);
        current->right = child;
        if (child != nullptr)
            child->parent = current;
    }
    else
    {
        inserted = false;
    }

    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.

    while (node != root && node->parent != nullptr && node->parent->color == RED)
    {
        ResearcherNode *parent = node->parent;
        ResearcherNode *grand = parent->parent;

        if (grand == nullptr)
            break;

        if (parent == grand->left)
        {
            ResearcherNode *uncle = grand->right;

            if (uncle != nullptr && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                node = grand;
            }
            else
            {
                if (node == parent->right)
                {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    grand = parent->parent;
                }

                parent->color = BLACK;
                if (grand != nullptr)
                {
                    grand->color = RED;
                    rotateRight(grand);
                }
            }
        }
        else
        {
            ResearcherNode *uncle = grand->left;

            if (uncle != nullptr && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand->color = RED;
                node = grand;
            }
            else
            {
                if (node == parent->left)
                {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    grand = parent->parent;
                }

                parent->color = BLACK;
                if (grand != nullptr)
                {
                    grand->color = RED;
                    rotateLeft(grand);
                }
            }
        }
    }

    if (root != nullptr)
        root->color = BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.
    //return false;

    ResearcherNode *z = findResearcher(fullName);
    if (z == nullptr)
        return false;

    ResearcherNode *y = z;
    Color yOriginalColor = y->color;

    ResearcherNode *x = nullptr;
    ResearcherNode *xParent = nullptr;

    if (z->left == nullptr)
    {
        x = z->right;
        xParent = z->parent;

        if (z->parent == nullptr)
            root = z->right;
        else if (z == z->parent->left)
            z->parent->left = z->right;
        else
            z->parent->right = z->right;

        if (z->right != nullptr)
            z->right->parent = z->parent;
    }
    else if (z->right == nullptr)
    {
        x = z->left;
        xParent = z->parent;

        if (z->parent == nullptr)
            root = z->left;
        else if (z == z->parent->left)
            z->parent->left = z->left;
        else
            z->parent->right = z->left;

        if (z->left != nullptr)
            z->left->parent = z->parent;
    }
    else
    {
        y = minimum(z->right);
        yOriginalColor = y->color;

        x = y->right;

        if (y->parent == z)
        {
            xParent = y;
        }
        else
        {
            xParent = y->parent;

            if (y->parent != nullptr)
            {
                if (y == y->parent->left)
                    y->parent->left = y->right;
                else
                    y->parent->right = y->right;
            }

            if (y->right != nullptr)
                y->right->parent = y->parent;

            y->right = z->right;
            if (y->right != nullptr)
                y->right->parent = y;
        }

        if (z->parent == nullptr)
            root = y;
        else if (z == z->parent->left)
            z->parent->left = y;
        else
            z->parent->right = y;

        y->parent = z->parent;

        y->left = z->left;
        if (y->left != nullptr)
            y->left->parent = y;

        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK)
        deleteFixup(x, xParent);

    if (root != nullptr)
        root->color = BLACK;

    return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.

    while (node != root && (node == nullptr || node->color == BLACK))
    {
        if (parent == nullptr)
            break;

        if (node == parent->left)
        {
            ResearcherNode *w = parent->right;

            if (w != nullptr && w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                w = parent->right;
            }

            if ((w == nullptr) ||
                ((w->left == nullptr || w->left->color == BLACK) &&
                 (w->right == nullptr || w->right->color == BLACK)))
            {
                if (w != nullptr)
                    w->color = RED;

                node = parent;
                parent = node->parent;
            }
            else
            {
                if (w->right == nullptr || w->right->color == BLACK)
                {
                    if (w->left != nullptr)
                        w->left->color = BLACK;

                    if (w != nullptr)
                        w->color = RED;

                    rotateRight(w);
                    w = parent->right;
                }

                if (w != nullptr)
                    w->color = parent->color;

                parent->color = BLACK;

                if (w != nullptr && w->right != nullptr)
                    w->right->color = BLACK;

                rotateLeft(parent);

                node = root;
                parent = nullptr;
            }
        }
        else
        {
            ResearcherNode *w = parent->left;

            if (w != nullptr && w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                w = parent->left;
            }

            if ((w == nullptr) ||
                ((w->left == nullptr || w->left->color == BLACK) &&
                 (w->right == nullptr || w->right->color == BLACK)))
            {
                if (w != nullptr)
                    w->color = RED;

                node = parent;
                parent = node->parent;
            }
            else
            {
                if (w->left == nullptr || w->left->color == BLACK)
                {
                    if (w->right != nullptr)
                        w->right->color = BLACK;

                    if (w != nullptr)
                        w->color = RED;

                    rotateLeft(w);
                    w = parent->left;
                }

                if (w != nullptr)
                    w->color = parent->color;

                parent->color = BLACK;

                if (w != nullptr && w->left != nullptr)
                    w->left->color = BLACK;

                rotateRight(parent);

                node = root;
                parent = nullptr;
            }
        }
    }

    if (node != nullptr)
        node->color = BLACK;
}



ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    //return node;

    ResearcherNode *cur = node;
    while (cur != nullptr && cur->left != nullptr)
        cur = cur->left;
    return cur;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.

    ResearcherNode *y = x->right;
    if (y == nullptr) return;

    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.

    ResearcherNode *x = y->left;
    if (x == nullptr) return;

    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    //return 0;

    if (node == nullptr)
        return 0;

    return 1 + getResearcherCount(node->left) + getResearcherCount(node->right);
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    //return 0;

    if (node == nullptr)
        return 0;

    return node->data.numAssigned + getTotalLoad(node->left) + getTotalLoad(node->right);
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.

    if (node == nullptr)
        return;

    std::cout << node->data.fullName << " "
              << node->data.capacity << " "
              << node->data.numAssigned << std::endl;

    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}

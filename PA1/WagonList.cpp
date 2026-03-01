#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.
    Wagon* cur = front;
    while (cur)
    {
        Wagon* nxt = cur->getNext();
        delete cur;
        cur = nxt;
    }
    front = rear = nullptr;
    totalWeight = 0;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.

    front = other.front;
    rear  = other.rear;
    totalWeight = other.totalWeight;

    other.front = other.rear = nullptr;
    other.totalWeight = 0;
}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.
    if (this == &other) return *this;

    clear();

    front = other.front;
    rear  = other.rear;
    totalWeight = other.totalWeight;

    other.front = other.rear = nullptr;
    other.totalWeight = 0;
    return *this;
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.
    for (Wagon* cur = front; cur; cur = cur->getNext())
        if (cur->getID() == id)
            return cur;
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight
    if (!w) return;
    w->setPrev(nullptr);
    w->setNext(nullptr);

    if (!front)
    { 
        front = rear = w;
    }
    else
    {
        rear->setNext(w);
        w->setPrev(rear);
        rear = w;
    }
    totalWeight += w->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    // TODO: Return true if the list has no wagons.
    return front == nullptr;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.
    if (!wagon) return;
    wagon->setPrev(nullptr);
    wagon->setNext(nullptr);

    if (!front) {
        front = rear = wagon;
        totalWeight += wagon->getWeight();
        return;
    }

    if (wagon->getWeight() >= front->getWeight()) {
        wagon->setNext(front);
        front->setPrev(wagon);
        front = wagon;
        totalWeight += wagon->getWeight();
        return;
    }

    if (wagon->getWeight() <= rear->getWeight()) {
        rear->setNext(wagon);
        wagon->setPrev(rear);
        rear = wagon;
        totalWeight += wagon->getWeight();
        return;
    }

    Wagon* cur = front->getNext();
    while (cur && cur->getWeight() >= wagon->getWeight())
        cur = cur->getNext();

    Wagon* before = cur->getPrev();
    before->setNext(wagon);
    wagon->setPrev(before);
    wagon->setNext(cur);
    cur->setPrev(wagon);

    totalWeight += wagon->getWeight();
}

void WagonList::appendList(WagonList &&other)
{
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list
   if (other.isEmpty())
        return;

    if (this->isEmpty()) {
        *this = std::move(other);
        return;
    }

    rear->setNext(other.front);
    other.front->setPrev(rear);
    rear = other.rear;

    totalWeight += other.totalWeight;

    other.front = other.rear = nullptr;
    other.totalWeight = 0;
}

Wagon *WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.
    Wagon* toRemove = findById(id);
    if (!toRemove)
        return nullptr;

    Wagon* before = toRemove->getPrev();
    Wagon* after  = toRemove->getNext();

    if (before)
        before->setNext(after);
    else
        front = after;

    if (after)
        after->setPrev(before);
    else
        rear = before;

    toRemove->setPrev(nullptr);
    toRemove->setNext(nullptr);

    totalWeight -= toRemove->getWeight();

    std::cout << "Wagon " << toRemove->getID() << " detached from Wagon List. " << "\n";
    return toRemove;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.
     Wagon* node = findById(id);
    if (!node)
        return newList;

    newList.front = node;
    newList.rear  = rear;

    Wagon* before = node->getPrev();
    if (before)
        before->setNext(nullptr);
    node->setPrev(nullptr);

    rear = before;
    if (rear == nullptr) {
        front = nullptr;
    }

    int moved = 0;
    for (Wagon* cur = newList.front; cur; cur = cur->getNext())
        moved += cur->getWeight();

    newList.totalWeight = moved;
    totalWeight -= moved;
    if (totalWeight < 0)
        totalWeight = 0;
    return newList;
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}

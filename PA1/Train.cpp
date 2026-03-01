#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    //TODO: Do the cleaning as necesssary
    wagons.clear();
    totalWeight = 0;
    nextLocomotive = nullptr;
}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{

    // TODO: Verify whether any wagon’s coupler is overloaded.
    // You must traverse from the rear (backmost wagon) toward the front.
    //
    // Splitting rule:
    // Split the train AFTER the overloaded wagon (the overloaded one stays).
    // Use WagonList::splitAtById to detach the overloaded segment into a new WagonList.
    //
    // If no overload occurs, return nullptr (no split needed).
    //
    // If a valid split occurs:
    // new Train should be named "<oldName>_split_<splitCounter>".

    // print message
    // std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
    // std::cout << newTrain->wagons << std::endl;
    long long trailingWeight = 0;
    Wagon* cur = wagons.getRear();
    while (cur)
    {
        if (trailingWeight > cur->getMaxCouplerLoad())
        {
            Wagon* splitStart = cur->getNext();
            if (!splitStart) break;

            WagonList detached = wagons.splitAtById(splitStart->getID());

            std::string newName = name + "_split_" + std::to_string(splitCounter);
            Train* newTrain = new Train(newName, destination);
            newTrain->appendWagonList(detached);

            std::cout << "Train " << name
                      << " split due to coupler overload before Wagon "
                      << splitStart->getID() << std::endl;
            std::cout << newTrain->wagons << std::endl;

            totalWeight = wagons.getTotalWeight();
            return newTrain;
        }

        trailingWeight += cur->getWeight();
        cur = cur->getPrev();
    }

    return nullptr;
}
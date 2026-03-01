#include "TrainTrack.h"
#include <iostream>

bool TrainTrack::autoDispatch = false;

TrainTrack::TrainTrack()
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(Destination::OTHERS),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::TrainTrack(Destination _dest)
    : firstLocomotive(nullptr),
      lastLocomotive(nullptr),
      destination(_dest),
      totalWeight(0), trainCounter(0)
{
}

TrainTrack::~TrainTrack()
{
    // TODO: If track is deconstucting, 
    // depart all of the trains
    // Handle pointers as necessary
    while (!isEmpty())
    {
        Train* t = departTrain();
        delete t;
    }
    firstLocomotive = lastLocomotive = nullptr;
    totalWeight = 0;
}

// Given to you ready
std::string TrainTrack::generateTrainName()
{
    ++trainCounter;
    return "Train_" + destinationToString(destination) + "_" + std::to_string(trainCounter);
}

void TrainTrack::addTrain(Train *train)
{
    if (!train) return;

    train->setNext(nullptr);
    if (!firstLocomotive) {
        firstLocomotive = lastLocomotive = train;
    } else {
        lastLocomotive->setNext(train);
        lastLocomotive = train;
    }

    totalWeight += train->getTotalWeight();

    while (autoDispatch && totalWeight > AUTO_DISPATCH_LIMIT && !isEmpty())
    {
        Train* departed = departTrain();

        if (departed) {
            std::cout << "Auto-dispatch: departing "
                      << departed->getName() << " to make room." << std::endl;
            delete departed;
        } else {
            break;
        }
    }
}


Train *TrainTrack::departTrain()
{
    // TODO: Remove the first train (front of the track) and return it.
    // use: std::cout << "Train " << removed->name << " departed from Track " << destinationToString(destination) << "." << std::endl;
    if (isEmpty()) return nullptr;

    Train* removed = firstLocomotive;
    firstLocomotive = firstLocomotive->getNext();
    if (!firstLocomotive)
        lastLocomotive = nullptr;
    removed->setNext(nullptr);
    totalWeight -= removed->getTotalWeight();

    std::cout << "Train " << removed->name<< " departed from Track " << destinationToString(destination) << "." << std::endl;

    return removed;
}

bool TrainTrack::isEmpty() const
{
    // TODO: Return true if there are no trains on this track.
    return firstLocomotive == nullptr;
}


Train *TrainTrack::findTrain(const std::string &name) const
{
    // TODO: Search for a train by name.
    // Return pointer to train if found, nullptr otherwise.
     for (Train* cur = firstLocomotive; cur; cur = cur->getNext())
        if (cur->getName() == name)
            return cur;
    return nullptr;
}

// Given to you ready
void TrainTrack::printTrack() const
{

    if (isEmpty())
        return;

    Train *current = firstLocomotive;

    std::cout << "[Track " << static_cast<int>(firstLocomotive->destination) << "] ";
    while (current)
    {
        std::cout << current->getName() << "(" << current->getTotalWeight() << "ton)-" << current->wagons << " -> ";
        current = current->getNext();
    }
    std::cout << std::endl;
    return;
}
#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/
    const int di = static_cast<int>(dest);

    bool any = false;
    for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
        if (!blockTrains[di][j].isEmpty()) { any = true; break; }

    if (!any) return nullptr;

    Train* t = new Train(trainName, dest);

    const int HAZ_IDX = static_cast<int>(CargoType::HAZARDOUS);

    struct BlockInfo { int cargoIdx; int headWeight; bool present; };
    BlockInfo blocks[NUM_CARGOTYPES_INT];
    int blocksCount = 0;

    for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
    {
        if (j == HAZ_IDX) continue;
        if (!blockTrains[di][j].isEmpty())
        {
            Wagon* head = blockTrains[di][j].getFront();
            blocks[blocksCount].cargoIdx   = j;
            blocks[blocksCount].headWeight = head ? head->getWeight() : -1;
            blocks[blocksCount].present    = (head != nullptr);
            ++blocksCount;
        }
    }

    for (int a = 0; a < blocksCount; ++a)
    {
        int best = a;
        for (int b = a + 1; b < blocksCount; ++b)
            if (blocks[b].headWeight > blocks[best].headWeight)
                best = b;
        if (best != a) { BlockInfo tmp = blocks[a]; blocks[a] = blocks[best]; blocks[best] = tmp; }
    }

    for (int k = 0; k < blocksCount; ++k)
    {
        int cj = blocks[k].cargoIdx;
        if (!blockTrains[di][cj].isEmpty())
        {
            t->appendWagonList(blockTrains[di][cj]);
        }
    }

    if (!blockTrains[di][HAZ_IDX].isEmpty())
    {
        Wagon* hzFront = blockTrains[di][HAZ_IDX].getFront();
        if (hzFront)
        {
            Wagon* oneHaz = blockTrains[di][HAZ_IDX].detachById(hzFront->getID());
            if (oneHaz)
            {
                t->addWagonToRear(oneHaz);
            }
        }
    }

    return t;
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
        {
            if (!blockTrains[i][j].isEmpty())
                return false;
        }
    }
    return true;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        for (int j = 0; j < NUM_CARGOTYPES_INT; ++j)
        {
            blockTrains[i][j].clear();
        }
    }

}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}
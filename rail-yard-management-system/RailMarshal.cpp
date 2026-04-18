#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        departureYard[i] = TrainTrack(static_cast<Destination>(i));
    }
    TrainTrack::autoDispatch = false;
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
    classificationYard.clear();

    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        while (!departureYard[i].isEmpty())
        {
            Train* t = departureYard[i].departTrain();
            if (t) delete t;
        }
    }
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.

    // if ADD_WAGON
    // Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;

    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;

    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;

    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";

    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    if (line.empty()) return;

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    if (cmd.empty()) return;

    cmd = toUpper(cmd);

    if (cmd == "ADD_WAGON")
    {
        int id, weight, maxC;
        std::string cargoStr, destStr;

        if (!(iss >> id >> cargoStr >> destStr >> weight >> maxC))
        {
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
            return;
        }

        CargoType c = parseCargo(cargoStr);
        Destination d = parseDestination(destStr);

        Wagon* w = new Wagon(id, c, d, weight, maxC);
        classificationYard.insertWagon(w);

        std::cout << "Wagon " << *w << " added to yard." << std::endl;
        return;
    }

    if (cmd == "REMOVE_WAGON")
    {
        int id;

        if (!(iss >> id))
        {
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
        }

        Wagon* removed = nullptr;

        for (int di = 0; di < NUM_DESTINATIONS_INT && !removed; ++di)
        {
            for (int cj = 0; cj < NUM_CARGOTYPES_INT && !removed; ++cj)
            {
                removed = classificationYard.getBlockTrain(di, cj).detachById(id);
            }
        }

        if (!removed)
        {
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
            return;
        }

        std::cout << "Wagon " << id << " removed." << std::endl;
        delete removed;
        return;
    }

    if (cmd == "ASSEMBLE_TRAIN")
{
    std::string destStr;

    if (!(iss >> destStr))
    {
        std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
        return;
    }

    Destination d = parseDestination(destStr);

    std::string name = getDepartureYard(d).generateTrainName();

    Train* t = classificationYard.assembleTrain(d, name);
    if (!t)
    {
        std::cout << "No wagons to assemble for " << destStr << std::endl;
        return;
    }

    TrainTrack& track = getDepartureYard(d);
    int splitCounter = 1;

    while (true)
    {
        Train* extra = t->verifyCouplersAndSplit(splitCounter);
        if (!extra) break;

        std::cout << "Train " << extra->getName()
                  << " assembled after split with " << extra->getWagons()
                  << " wagons." << std::endl;

        track.addTrain(extra);
        ++splitCounter;
    }

    std::cout << "Train " << t->getName()
              << " assembled with " << t->getWagons()
              << " wagons." << std::endl;

    track.addTrain(t);
    return;
}


    if (cmd == "DISPATCH_TRAIN")
    {
        std::string destStr;

        if (!(iss >> destStr))
        {
            std::cout << "Error: Invalid DISPATCH parameters.\n";
            return;
        }

        Destination d = parseDestination(destStr);
        TrainTrack& track = getDepartureYard(d);

        if (track.isEmpty())
        {
            std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
            return;
        }

        Train* t = track.departTrain();
        if (t)
        {
            std::cout << "Dispatching " << t->getName()
                      << " (" << t->getTotalWeight() << " tons)." << std::endl;
            delete t;
        }
        return;
    }

    if (cmd == "PRINT_YARD")
    {
        std::cout << "--- classification Yard ---\n";
        classificationYard.print();
        return;
    }

    if (cmd == "PRINT_TRACK")
    {
        std::string destStr;
        if (!(iss >> destStr))
        {
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
            return;
        }

        Destination d = parseDestination(destStr);
        getDepartureYard(d).printTrack();
        return;
    }

    if (cmd == "AUTO_DISPATCH")
    {
        std::string sw;

        if (!(iss >> sw))
        {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            return;
        }

        sw = toUpper(sw);
        if (sw == "ON")
        {
            TrainTrack::autoDispatch = true;
            std::cout << "Auto dispatch enabled" << std::endl;
        }
        else if (sw == "OFF")
        {
            TrainTrack::autoDispatch = false;
            std::cout << "Auto dispatch disabled" << std::endl;
        }
        else
        {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }
        return;
    }

    if (cmd == "CLEAR")
    {
        classificationYard.clear();

        for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
        {
            while (!departureYard[i].isEmpty())
            {
                Train* t = departureYard[i].departTrain();
                if (t) delete t;
            }
        }

        std::cout << "System cleared." << std::endl;
        return;
    }

    std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
    TrainTrack& tr = getDepartureYard(track);

    if (tr.isEmpty())
    {
        std::cout << "Error: No trains to dispatch from track "
                  << destinationToString(track) << ".\n";
        return;
    }

    Train* t = tr.departTrain();
    if (t)
    {
        std::cout << "Dispatching " << t->getName()
                  << " (" << t->getTotalWeight() << " tons)." << std::endl;
        delete t;
    }
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}

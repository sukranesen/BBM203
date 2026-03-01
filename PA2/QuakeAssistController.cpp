#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.

    std::istringstream iss(line);
    std::string cmd;

    if (!(iss >> cmd)) {
        return true;
    }
    if (cmd == "INIT_TEAMS"){
        int n;
        iss>> n;
        handleInitTeams(n);
    }else if ( cmd == "SET_TEAM_CAPACITY") {
        int id, cap;
        iss>> id>> cap;
        handleSetTeamCapacity(id, cap);
    }else if (cmd == "ADD_SUPPLY") {
        std::string id, city, supplyType;
        int amount, emergencyLevel;
        iss >> id >> city >> supplyType >> amount >> emergencyLevel;
        handleAddSupply(id, city, supplyType, amount, emergencyLevel);
    }else if (cmd == "ADD_RESCUE") {
        std::string id, city, riskStr;
        int numPeople, emergencyLevel;
        iss>> id >> city>> numPeople>> riskStr >> emergencyLevel;
        handleAddRescue(id, city, numPeople, riskStr, emergencyLevel);

    } else if (cmd == "REMOVE_REQUEST"){
        std::string id;
        iss >> id;
        handleRemoveRequest(id);
    }else if (cmd == "HANDLE_EMERGENCY"){
        int teamId, k;
        iss >> teamId >> k;
        handleHandleEmergency(teamId, k);
    } else if ( cmd == "DISPATCH_TEAM"){
        int teamId;
        iss >> teamId;
        handleDispatchTeam(teamId);
    } else if ( cmd == "PRINT_QUEUES") {
        printQueues();
    }else if ( cmd == "PRINT_TEAM") {
        int teamId;
        iss >> teamId;
        printTeam(teamId);

    }else if (cmd == "CLEAR"){
        clear();
    } else {
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl;
    }
    return true;

}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    //return true;

    if (numTeams<0){
        return false;
    }
    delete[] teams;
    teams = nullptr;
    teamCount = 0;

    if (numTeams == 0){
        return true;
    }
    Team* newTeams = new Team [numTeams];
    if (!newTeams){
        return false;

    }

    teams = newTeams;
    teamCount = numTeams;

    for (int i =0; i < teamCount; ++i){
        teams[i].setId(i);
        teams[i].setMaxLoadCapacity(0);
    }
    return true;

}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.
    //return -1;
    for (int i=0; i< teamCount; ++i){
        if(teams[i].getId() == teamId){
            return i;
        }
    }
    return -1;
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    //return true;
    int idx = findTeamIndexById(teamId);
    if (idx == -1){
        std::cout<< "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    teams[idx].setMaxLoadCapacity(capacity);
    std::cout << "Team " << teamId << " capacity set to " << capacity << "." << std::endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.

    Request req (id, cityStr, supplyTypeStr, amount, emergencyLevel);
    supplyQueue.enqueue(req);
    std::cout << "Request " << id << " added to SUPPLY queue." << std::endl;
    return true;

}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request req (id, cityStr, numPeople, riskStr, emergencyLevel);
    rescueQueue.enqueue(req);
    std::cout << "Request " <<id << " added to RESCUE queue." << std::endl;

    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue. 
    //return true;
    bool removed = false;
    if (supplyQueue.removeById(id)){
        removed = true;
    }

    if (rescueQueue.removeById(id)){
        removed = true;
    }

    if ( removed) {
        std::cout << "Request " << id << " removed from queues." << std::endl;

    }else {
        std::cout << "Error: Request " <<id<< " not found." << std::endl;
    }
    return true;
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    //(void)teamId;
    //(void)k;
    //return true;

    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& team = teams[idx];

    int assignedTotal = 0;
    int assignedSupply = 0;
    int assignedRescue = 0;
    bool overload = false;

    for (int step = 0; step < k; ++step){
        bool hasSupply = !supplyQueue.isEmpty();
        bool hasRescue = !rescueQueue.isEmpty();


        if (!hasSupply && !hasRescue) {
            break;
        }

        Request supplyFront;
        Request rescueFront;
        bool useSupply = false;


        if(hasSupply && !hasRescue) {
            useSupply = true;
        } else if (!hasSupply && hasRescue) {
            useSupply = false;
        } else {
            supplyQueue.peek(supplyFront);
            rescueQueue.peek(rescueFront);


            int supplyScore = supplyFront.computeEmergencyScore();
            int rescueScore = rescueFront.computeEmergencyScore();

            if (rescueScore > supplyScore){
                useSupply = false;
            }else if (supplyScore > rescueScore) {
                useSupply = true;
            } else {
                useSupply = false;
            }
        }


        Request chosen;
        if (useSupply) {
            if (!hasSupply) {
                break;
            }
            supplyQueue.peek(supplyFront);
            chosen = supplyFront;
        } else{
            if (!hasRescue) {
                break;
            }
            rescueQueue.peek(rescueFront);
            chosen = rescueFront;
        }

        if(!team.tryAssignRequest(chosen)) {
            std::cout << "Overload on Team " << teamId << ": rolling back mission." << std::endl;
            team.rollbackMission(supplyQueue, rescueQueue);
            overload = true;
            break;
        }

        if (useSupply) {
            Request tmp;
            supplyQueue.dequeue(tmp);
            ++assignedSupply;
        } else {
            Request tmp;
            rescueQueue.dequeue(tmp);
            ++assignedRescue;

        }

        ++assignedTotal;
    }

    if (!overload && assignedTotal > 0){
        int workload = team.getCurrentWorkload();
        std::cout << "Team " << teamId << " assigned "
            << assignedTotal << " requests ("
            << assignedSupply << " SUPPLY, "
            << assignedRescue << " RESCUE), total workload "
            << workload << "." << std::endl;
    }

    return true;

}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.

    
    std::cout << "SUPPLY QUEUE:" << std::endl;

    Request* sData = supplyQueue.getData();
    int sCap = supplyQueue.getCapacity();
    int sCount = supplyQueue.getCount();
    int sFront = supplyQueue.getFrontIndex();
    for (int i = 0; i < sCount; ++i){
        int idx = (sFront + i) % sCap;
        const Request& r = sData[idx];
        if (r.getType() == "SUPPLY") {
            std::cout << r.getId() << " "
                      << r.getCity() << " "
                      << r.getSupplyType() << " "
                      << r.getAmount() << " "
                      << r.getEmergencyLevel() << std::endl;
        }
    }
    std::cout << "RESCUE QUEUE:" << std::endl;
    Request* rData = rescueQueue.getData();
    int rCap = rescueQueue.getCapacity();
    int rCount = rescueQueue.getCount();
    int rFront = rescueQueue.getFrontIndex();

    for (int i = 0; i < rCount; ++i) {
        int idx = (rFront + i) % rCap;
        const Request& r = rData[idx];
        if (r.getType() == "RESCUE"){
            std::cout << r.getId() << " "
                      << r.getCity() << " "
                      << r.getNumPeople() << " "
                      << r.getRisk() << " "
                      << r.getEmergencyLevel() << std::endl;
        }
    }
    
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.

    int idx = findTeamIndexById(teamId);
    if (idx == -1){
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }



    const Team& t = teams[idx];
    const MissionStack& stack = t.getMissionStack();

    std::cout << "TEAM " << teamId << " STACK:" << std::endl;

    int top = stack.getTopIndex();
    if (top < 0) {
        return;
    }

    Request* data = stack.getData();
    for (int i = top; i >= 0; --i){
        const Request& r = data[i];
        if (r.getType() == "SUPPLY") {
            std::cout << r.getId() << " "
                      << r.getCity() << " "
                      << r.getSupplyType() << " "
                      << r.getAmount() << " "
                      << r.getEmergencyLevel() << std::endl;
        } else if (r.getType() == "RESCUE") {
            std::cout << r.getId() << " "
                      << r.getCity() << " "
                      << r.getNumPeople() << " "
                      << r.getRisk() << " "
                      << r.getEmergencyLevel() << std::endl;
        }
    }

}

void QuakeAssistController::clear() {
    //Clear data.

    supplyQueue.clear();
    rescueQueue.clear();
    for(int i = 0; i < teamCount; ++i){
        teams[i].clearMission();
    }

    std::cout << "System cleared." << std::endl;
}


bool QuakeAssistController::handleInitTeams(int numTeams) {
    if (!initializeTeams(numTeams)) {
        return false;
    }
    std::cout << "Initialized " << numTeams << " teams." << std::endl;
    return true;
}

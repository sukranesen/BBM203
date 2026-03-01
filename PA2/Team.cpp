#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    //(void)req;
    //return false;
    int contribution = req.computeWorkloadContribution();

    int predictidWorkload= currentWorkload + contribution;

    if (predictidWorkload > maxLoadCapacity) {
        return false;
    }
    missionStack.push(req);

    currentWorkload = predictidWorkload;

    return true;

}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.
    //(void)supplyQueue;
    //(void)rescueQueue;

    int n = missionStack.size();

    if ( n==0) {
        currentWorkload = 0;
        return;
    }
    Request *temp = new Request[n];

    for ( int i =0; i<n; ++i) {
        missionStack.pop(temp[i]);
    }

    int supplyCount = 0;
    int rescueCount = 0;

    for (int i =0; i<n; ++i){
        if (temp[i].getType() == "SUPPLY") {
            ++supplyCount;
        }else if (temp[i].getType() == "RESCUE") {
            ++rescueCount;
        }
    }

    Request* supplyArr = nullptr;
    Request* rescueArr = nullptr;
    if (supplyCount>0) {
        supplyArr = new Request[supplyCount];
    }
    if (rescueCount>0) {
        rescueArr = new Request[rescueCount];
    }
    int sIdx = 0;
    int rIdx = 0;
    for (int i = n-1; i >=0; --i){
        if (temp[i].getType()== "SUPPLY") {
            supplyArr[sIdx] = temp [i];
            sIdx++;
        } else if (temp[i].getType() == "RESCUE") {
            rescueArr[rIdx] = temp[i];
            rIdx++;
        }
    }
    int oldSupplyCount = supplyQueue.size();
    Request *oldSupply = nullptr;
    if ( oldSupplyCount >0) {
        oldSupply = new Request[oldSupplyCount];
        for (int i = 0; i< oldSupplyCount; ++i){
            supplyQueue.dequeue(oldSupply[i]);
        }
    }
    for (int i = 0; i<supplyCount; ++i) {
        supplyQueue.enqueue(supplyArr[i]);
    }

    for (int i = 0; i<oldSupplyCount; ++i){
        supplyQueue.enqueue(oldSupply[i]);
    }
    int oldRescueCount = rescueQueue.size();
    Request *oldRescue = nullptr;
    if (oldRescueCount >0) {
        oldRescue = new Request[oldRescueCount];
        for (int i = 0; i< oldRescueCount; ++i){
            rescueQueue.dequeue(oldRescue[i]);
        }
    }
    for (int i = 0; i< rescueCount; ++i) {
        rescueQueue.enqueue(rescueArr[i]);
    }
    for (int i = 0; i<oldRescueCount; ++i) {
        rescueQueue.enqueue(oldRescue[i]);
    }

    delete[] temp;
    delete[] supplyArr;
    delete[] rescueArr;
    delete[] oldSupply;
    delete[] oldRescue;

    currentWorkload = 0;

}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}


#include "ArtifactManager.h"
#include <iostream>
#include <sstream>
#include "Artifact.h"


ArtifactManager::ArtifactManager()
{
}
//cleanupArtifactUsageTable();
ArtifactManager::~ArtifactManager()
{
    cleanupArtifactUsageTable();
}
// memory leak
int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known

    if (line.size() == 0) return;

    std::string tokens[10];
    int count = tokenize(line, tokens, 10);

    if (count == 0) return;

    if (tokens[0].size() > 0 && tokens[0][0] == '#') return;

    std::string cmd = tokens[0];

    if (cmd == "ADD_ARTIFACT") handleAddArtifact(tokens, count);
    else if (cmd == "REMOVE_ARTIFACT") handleRemoveArtifact(tokens, count);
    else if (cmd == "HIRE_RESEARCHER") handleHireResearcher(tokens, count);
    else if (cmd == "FIRE_RESEARCHER") handleFireResearcher(tokens, count);
    else if (cmd == "REQUEST") handleRequest(tokens, count);
    else if (cmd == "RETURN") handleReturn(tokens, count);
    else if (cmd == "RETURN_ALL") handleReturnAll(tokens, count);
    else if (cmd == "RESEARCHER_LOAD") handleResearcherLoad(tokens, count);
    else if (cmd == "MATCH_RARITY") handleMatchRarity(tokens, count);
    else if (cmd == "PRINT_UNASSIGNED") handlePrintUnassigned(tokens, count);
    else if (cmd == "PRINT_STATS") handlePrintStats(tokens, count);
    else if (cmd == "CLEAR") handleClear(tokens, count);
    else
    {
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl;
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).

    if (count != 5)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    int id, rarity, value;
    std::stringstream ss1(tokens[1]); ss1 >> id;
    std::stringstream ss2(tokens[3]); ss2 >> rarity;
    std::stringstream ss3(tokens[4]); ss3 >> value;

    Artifact a(id, tokens[2], rarity, value);

    bool ok = artifactTree.insertArtifact(a);
    if (ok)
        std::cout << "Artifact " << id << " added." << std::endl;
    else
        std::cout << "Error: Artifact " << id << " already exists." << std::endl;
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.

    if (count != 2)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    int aid;
    std::stringstream ss(tokens[1]);
    ss >> aid;

    ArtifactNode *anode = artifactTree.findArtifact(aid);
    if (anode == nullptr)
    {
        std::cout << "Error: Artifact " << aid << " not found." << std::endl;
        return;
    }

    std::string owner = anode->data.assignedToName;
    if (owner != "")
    {
        ResearcherNode *rnode = researcherTree.findResearcher(owner);
        if (rnode != nullptr)
            rnode->data.removeArtifact(aid);

        artifactTree.clearAssignedTo(aid);
    }

    bool ok = artifactTree.removeArtifact(aid);
    if (ok)
        std::cout << "Artifact " << aid << " removed." << std::endl;
    else
        std::cout << "Error: Artifact " << aid << " not found." << std::endl;
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.

    if (count != 3)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    int cap;
    std::stringstream ss(tokens[2]);
    ss >> cap;

    Researcher r(tokens[1], cap);

    bool ok = researcherTree.insertResearcher(r);
    if (ok)
        std::cout << "Researcher " << tokens[1] << " hired." << std::endl;
    else
        std::cout << "Error: Researcher " << tokens[1] << " already exists." << std::endl;
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.

    if (count != 2)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    std::string rname = tokens[1];

    ResearcherNode *rnode = researcherTree.findResearcher(rname);
    if (rnode == nullptr)
    {
        std::cout << "Error: Researcher " << rname << " not found." << std::endl;
        return;
    }

    int i = 0;
    while (i < rnode->data.numAssigned)
    {
        int aid = rnode->data.assignedArtifacts[i];
        artifactTree.clearAssignedTo(aid);
        i++;
    }

    bool ok = researcherTree.removeResearcher(rname);
    if (ok)
        std::cout << "Researcher " << rname << " fired." << std::endl;
    else
        std::cout << "Error: Researcher " << rname << " not found." << std::endl;
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."

    if (count != 3)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    std::string rname = tokens[1];
    int aid;
    std::stringstream ss(tokens[2]);
    ss >> aid;

    ResearcherNode *rnode = researcherTree.findResearcher(rname);
    if (rnode == nullptr)
    {
        std::cout << "Error: Researcher " << rname << " not found." << std::endl;
        return;
    }

    ArtifactNode *anode = artifactTree.findArtifact(aid);
    if (anode == nullptr)
    {
        std::cout << "Error: Artifact " << aid << " not found." << std::endl;
        return;
    }

    if (anode->data.assignedToName != "")
    {
        std::cout << "Error: Artifact " << aid << " is already assigned." << std::endl;
        return;
    }

    if (rnode->data.numAssigned >= rnode->data.capacity)
    {
        std::cout << "Error: Researcher " << rname << " is at full capacity." << std::endl;
        return;
    }

    bool ok = rnode->data.addArtifact(aid);
    if (!ok)
    {
        std::cout << "Error: Researcher " << rname << " is at full capacity." << std::endl;
        return;
    }

    artifactTree.setAssignedTo(aid, rname);

    anode = artifactTree.findArtifact(aid);
    if (anode != nullptr)
        anode->data.updateValueBasedOnUsage();

    std::cout << "Artifact " << aid << " assigned to " << rname << "." << std::endl;
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."

    
    if (count != 3)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    std::string rname = tokens[1];
    int aid;
    std::stringstream ss(tokens[2]);
    ss >> aid;

    ResearcherNode *rnode = researcherTree.findResearcher(rname);
    if (rnode == nullptr)
    {
        std::cout << "Error: Researcher " << rname << " not found." << std::endl;
        return;
    }

    ArtifactNode *anode = artifactTree.findArtifact(aid);
    if (anode == nullptr)
    {
        std::cout << "Error: Artifact " << aid << " not found." << std::endl;
        return;
    }

    if (anode->data.assignedToName != rname)
    {
        std::cout << "Error: Artifact " << aid << " is not assigned to " << rname << "." << std::endl;
        return;
    }

    rnode->data.removeArtifact(aid);
    artifactTree.clearAssignedTo(aid);

    std::cout << "Artifact " << aid << " returned by " << rname << "." << std::endl;
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.

    if (count != 2)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    std::string rname = tokens[1];

    ResearcherNode *rnode = researcherTree.findResearcher(rname);
    if (rnode == nullptr)
    {
        std::cout << "Error: Researcher " << rname << " not found." << std::endl;
        return;
    }

    int i = 0;
    while (i < rnode->data.numAssigned)
    {
        int aid = rnode->data.assignedArtifacts[i];
        artifactTree.clearAssignedTo(aid);
        i++;
    }

    rnode->data.removeAllArtifacts();

    std::cout << "All artifacts returned by " << rname << "." << std::endl;
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.

    if (count != 2)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    std::string rname = tokens[1];
    ResearcherNode *rnode = researcherTree.findResearcher(rname);

    if (rnode == nullptr)
    {
        std::cout << "Error: Researcher " << rname << " not found." << std::endl;
        return;
    }

    std::cout << "Researcher " << rname << " load: " << rnode->data.numAssigned << std::endl;
}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.

    //bos

    if (count != 2) return;

    int minR;
    std::stringstream ss(tokens[1]);
    ss >> minR;

    std::cout << "=== MATCH_RARITY " << minR << " ===" << std::endl;

    // AVLTree içinde bunu yazman gerekiyor (aşağıda)
    artifactTree.printMatchRarity(minR);
}

void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().

    if (count != 1) return;

    std::cout << "Unassigned artifacts:" << std::endl;
    artifactTree.printUnassigned();
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)

    if (count != 1) return;

    int totalA = artifactTree.getArtifactCount();
    int totalR = researcherTree.getResearcherCount();

    int avgRarity = 0;
    if (totalA > 0) avgRarity = artifactTree.getTotalRarity() / totalA;

    int avgLoad = 0;
    if (totalR > 0) avgLoad = researcherTree.getTotalLoad() / totalR;

    std::cout << "=== SYSTEM STATISTICS ===" << std::endl;
    std::cout << "Artifacts: " << totalA << std::endl;
    std::cout << "Researchers: " << totalR << std::endl;
    std::cout << "Average rarity: " << avgRarity << std::endl;
    std::cout << "Average load: " << avgLoad << std::endl;

    std::cout << "Researchers:" << std::endl;
    researcherTree.traversePreOrderForStats();

    std::cout << "Artifacts:" << std::endl;
    artifactTree.traversePostOrderForStats();
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    //artifactTree.clear();
    //researcherTree.clear();
    // e.g. "All data cleared."

    if (count != 1)
    {
        std::cout << "Error: Unknown command '" << tokens[0] << "'." << std::endl;
        return;
    }

    artifactTree.clear();
    researcherTree.clear();

    cleanupArtifactUsageTable();


    std::cout << "All data cleared." << std::endl;
}

#include "Artifact.h"

// memory leak 
static int *g_ids = nullptr;
static int *g_cnt = nullptr;
static int g_sz = 0;
static int g_cp = 0;

void cleanupArtifactUsageTable()
{
    delete[] g_ids;
    delete[] g_cnt;
    g_ids = nullptr;
    g_cnt = nullptr;
    g_sz = 0;
    g_cp = 0;
}
//

Artifact::Artifact()
    : artifactID(-1),
      name(""),
      rarityLevel(1),
      researchValue(0),
      assignedToName("")
{
}


Artifact::Artifact(int id, const std::string &n, int rarity, int value)
    : artifactID(id),
      name(n),
      rarityLevel(rarity),
      researchValue(value),
      assignedToName("")
{
}


void Artifact::updateValueBasedOnUsage()
{
//TODO
if (g_cp < g_sz + 1)
    {
        int newCap = (g_cp == 0) ? 8 : g_cp;
        while (newCap < g_sz + 1)
            newCap *= 2;

        int *newIds = new int[newCap];
        int *newCnt = new int[newCap];

        for (int i = 0; i < g_sz; i++)
        {
            newIds[i] = g_ids[i];
            newCnt[i] = g_cnt[i];
        }

        delete[] g_ids;
        delete[] g_cnt;

        g_ids = newIds;
        g_cnt = newCnt;
        g_cp = newCap;
    }

    int index = -1;
    for (int i = 0; i < g_sz; i++)
    {
        if (g_ids[i] == artifactID)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        g_ids[g_sz] = artifactID;
        g_cnt[g_sz] = 1;
        index = g_sz;
        g_sz++;
    }
    else
    {
        g_cnt[index]++;
    }

    researchValue += rarityLevel * g_cnt[index];
}

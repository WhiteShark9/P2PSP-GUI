#include "rundialogstorage.h"

runDialogStorage::runDialogStorage()
{

}

unsigned int runDialogStorage::getPlayerPort() const
{
    return playerPort;
}

void runDialogStorage::setPlayerPort(unsigned int value)
{
    playerPort = value;
}

unsigned int runDialogStorage::getSplitterPort() const
{
    return splitterPort;
}

void runDialogStorage::setSplitterPort(unsigned int value)
{
    splitterPort = value;
}

string runDialogStorage::getSplitterAddress() const
{
    return splitterAddress;
}

void runDialogStorage::setSplitterAddress(const string &value)
{
    splitterAddress = value;
}

int runDialogStorage::getMaxChunk() const
{
    return maxChunkDebt;
}

void runDialogStorage::setMaxChunk(int value)
{
    maxChunkDebt = value;
}

unsigned int runDialogStorage::getTeamPort() const
{
    return teamPort;
}

void runDialogStorage::setTeamPort(unsigned int value)
{
    teamPort = value;
}

bool runDialogStorage::getLocalHost() const
{
    return useLocalHost;
}

void runDialogStorage::setLocalHost(bool value)
{
    useLocalHost = value;
}

int runDialogStorage::getSourcePortStep() const
{
    return sourcePortStep;
}

void runDialogStorage::setSourcePortStep(int value)
{
    sourcePortStep = value;
}






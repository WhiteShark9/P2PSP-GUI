#ifndef RUNDIALOGSTORAGE_H
#define RUNDIALOGSTORAGE_H

#include <string>
#include <iostream>

using namespace std;

class runDialogStorage
{
public:
    runDialogStorage();

    unsigned int getPlayerPort() const;
    void setPlayerPort(unsigned int value);

    unsigned int getSplitterPort() const;
    void setSplitterPort(unsigned int value);

    string getSplitterAddress() const;
    void setSplitterAddress(const string &value);

    int getMaxChunk() const;
    void setMaxChunk(int value);

    unsigned int getTeamPort() const;
    void setTeamPort(unsigned int value);

    bool getLocalHost() const;
    void setLocalHost(bool value);

    int getSourcePortStep() const;
    void setSourcePortStep(int value);

private:
    unsigned int playerPort;
    unsigned int splitterPort;
    string splitterAddress;
    int maxChunkDebt;
    unsigned int teamPort;
    bool useLocalHost;
    int sourcePortStep;
};

#endif // RUNDIALOGSTORAGE_H

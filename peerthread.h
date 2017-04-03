#ifndef PEERTHREAD_H
#define PEERTHREAD_H

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <peer.h>
#include <globalheader.h>
#include <rundialogstorage.h>
//#include <SimplePlayer.h>

//class peer;

using namespace std;

//class SimplePlayer;

class PeerThread : public QThread
{
    Q_OBJECT

private:
    bool stop1=true;
    QMutex mutex;
    QWaitCondition condition;

    p2psp::peer *peerobj;
    //SimplePlayer *simpleplayerobj;
    runDialogStorage runtimeParameters;

    /*Run Method Variables*/
    uint16_t player_port;
    string splitter_addr;
    uint16_t splitter_port;
#if not defined __IMS__
    int max_chunk_debt;
    uint16_t team_port;
#endif
#if defined __NTS__ && not defined __monitor__
    int source_port_step;
#endif

    //local Variables used in Algorithm Run Method
    int kbps_recvfrom;
    int kbps_sendto;
    int last_sendto_counter;
    int last_recvfrom_counter;
#ifndef __IMS__
    int last_chunk_number;
    int kbps_expected_recv;
    float team_ratio;
    int kbps_expected_sent;
    int counter;
#endif

    //Used in Algorithm Run Method
    void usingHeaders();
    void setParameters();
    void peerInitialization();
    void calcBufferingTime();
    void setLocalVariables();
    void printCurrentTime();
    void process();
    void delay(int msecs);
protected:
    void run();
    void msleep(int ms);

public:
    explicit PeerThread(QObject *parent = 0);
    ~PeerThread();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;

//    QString filename;
signals:
    void stats(int download,int upload,int totalPeers);
private slots:
    //Play the Stream
    void Play(runDialogStorage runParameters);
};

#endif // PEERTHREAD_H

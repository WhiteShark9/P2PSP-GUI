#ifndef SIMPLEPLAYER_H_
#define SIMPLEPLAYER_H_

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QApplication>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Enums.h>
#include <add_dialog.h>
#include <addchannels.h>
#include <rundialog.h>
#include <helper.h>
#include <iostream>
#include "peerthread.h"
#include "rundialogstorage.h"

namespace Ui {
    class SimplePlayer;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcEnums;

//class EqualizerDialog;
class runDialog;
class PeerThread;

class SimplePlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimplePlayer(QWidget *parent = 0);
    ~SimplePlayer();
    void closeWindow();
private slots:
//    void openLocal();
//    void openUrl();

    void on_checkBox_clicked();


    void on_fullscreen_clicked();

/*    void on_fullscreen_clicked();*/

    void on_pause_clicked();

    void addChannelSlot(addChannels);

    void runPeerClicked();

    void on_volume_valueChanged(int value);

    void on_volumeButton_clicked();

    void stats(int download,int upload,int peerSize);

    void receivePeerParameters(runDialogStorage runParameters);

signals:

    void sendChannels(vector<addChannels>);

    void runPeer(runDialogStorage runParameters);
private:
    Ui::SimplePlayer *ui;
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

//    QString downloadSpeed;
//    QString uploadSpeed;
//    QString peerSize;
    helper *help1;
    add_dialog *channel1;
    runDialog *rundialogobj;
    PeerThread *peerthreadobj;
    vector<addChannels> channels;

    //For playing the vlc stream
    void openUrl();
};

#endif // SIMPLEPLAYER_H_

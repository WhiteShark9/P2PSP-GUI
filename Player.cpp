#include "Player.h"
#include "ui_SimplePlayer.h"
//#include "EqualizerDialog.h"

SimplePlayer::SimplePlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimplePlayer),
      _media(0)
{
    ui->setupUi(this);
    help1 = new helper();
    channel1 = new add_dialog();
    rundialogobj = new runDialog();
    peerthreadobj = new PeerThread();

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);

    ui->video->setMediaPlayer(_player);
    ui->volume->setMediaPlayer(_player);
    ui->volume->setVolume(50);
    ui->volume->hide();

//    ui->seek->setMediaPlayer(_player);
//    connect(ui->actionOpenLocal, &QAction::triggered, this, &SimplePlayer::openLocal);
//    connect(ui->actionOpenUrl, &QAction::triggered, this, &SimplePlayer::openUrl);
//    connect(ui->actionPause, &QAction::toggled, _player, &VlcMediaPlayer::togglePause);
//    connect(ui->openUrl, &QPushButton::clicked, this, &SimplePlayer::openUrl);
//    connect(ui->stop, &QPushButton::clicked, _player, &VlcMediaPlayer::stop);
//    QIcon icon;
//    icon.addPixmap(QPixmap("://icons/play.png"),QIcon::Normal,QIcon::On);
//    icon.addPixmap(QPixmap("://icons/pause.png"),QIcon::Normal,QIcon::Off);
//    ui->pause->setIcon(icon);
//    ui->pause->setCheckable(true);

    connect(ui->pause, &QPushButton::toggled, _player, &VlcMediaPlayer::togglePause);
    connect(ui->stop, &QPushButton::clicked, _player, &VlcMediaPlayer::stop);
    connect(ui->openUrl, &QPushButton::clicked, this, &SimplePlayer::openUrl);

    ui->groupBox->setVisible(false);
    ui->checkBox->hide();//hidden for this version1 will be back in v2
    ui->openUrl->hide();
    QObject::connect(ui->actionHelp,SIGNAL(triggered()),help1,SLOT(exec()));
    QObject::connect(ui->checkBox, SIGNAL(clicked(bool)), ui->groupBox, SLOT(setVisible(bool)));
    QObject::connect(ui->actionAdd,SIGNAL(triggered()),channel1,SLOT(exec()));
    QObject::connect(ui->actionRunPeer,SIGNAL(triggered()),this,SLOT(runPeerClicked()));
    QObject::connect(this,SIGNAL(sendChannels(vector<addChannels>)),rundialogobj,SLOT(receiveChannels(vector<addChannels>)));
    QObject::connect(rundialogobj,SIGNAL(runPeer(runDialogStorage)),this,SLOT(receivePeerParameters(runDialogStorage)));
    QObject::connect(this,SIGNAL(runPeer(runDialogStorage)),peerthreadobj,SLOT(Play(runDialogStorage)));
    QObject::connect(peerthreadobj,SIGNAL(stats(int,int,int)),this,SLOT(stats(int,int,int)));
    QObject::connect(channel1,SIGNAL(addChannelSignal(addChannels)),this,SLOT(addChannelSlot(addChannels)));
}
SimplePlayer::~SimplePlayer()
{
    delete _player;
    delete _media;
    delete _instance;
    delete ui;
}

void SimplePlayer::stats(int download, int upload, int peerSize)
{
    ui->downloadSpeedValue->setText(QString::number(download));
    ui->uploadSpeedValue->setText(QString::number(upload));
    ui->usersOnlineValue->setText(QString::number(peerSize));
}

void SimplePlayer::closeWindow()
{
    this->window()->close();
}

void SimplePlayer::runPeerClicked()
{
    rundialogobj->show();
    emit sendChannels(channels);
}

void SimplePlayer::receivePeerParameters(runDialogStorage runParameters)
{
    emit runPeer(runParameters);
    QString url = "http://localhost:";
    if(runParameters.getPlayerPort()!=0)
    {
        url+=QString::number(runParameters.getPlayerPort());
    }
    else
    {
        url+="9999";
    }
    qDebug()<<url;
    _media = new VlcMedia(url, _instance);

    _player->open(_media);
}

void SimplePlayer::addChannelSlot(addChannels chnnl)
{
    channels.push_back(chnnl);
}

//void SimplePlayer::openLocal()
//{
//    QString file =
//            QFileDialog::getOpenFileName(this, tr("Open file"),
//                                         QDir::homePath(),
//                                         tr("Multimedia files(*)"));

//    if (file.isEmpty())
//        return;

//    _media = new VlcMedia(file, true, _instance);

//    _player->open(_media);
//}

void SimplePlayer::openUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));

    if (url.isEmpty())
        return;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);
}

void VlcWidgetVolumeSlider::mousePressEvent(QMouseEvent *event)
{
   // ...
    QSlider::mousePressEvent(event);
   // ...
}
void VlcWidgetVolumeSlider::mouseReleaseEvent(QMouseEvent *event)
{
    // ...
    QSlider::mouseReleaseEvent(event);
   // ...
}

void SimplePlayer::on_checkBox_clicked()
{
    ui->checkBox->setCheckable(true);
    if(ui->checkBox->isChecked()){
        ui->checkBox->setText("Hide Channels");
    }
    else{
        ui->checkBox->setText("Show Channels");
    }

}

void SimplePlayer::on_fullscreen_clicked()
{

    isFullScreen() ? showNormal() : showFullScreen();
    QIcon ico;
    ico.addPixmap(QPixmap("://icons/Fit to Width-50.png"),QIcon::Normal,QIcon::On);
    ico.addPixmap(QPixmap("://icons/Normal Screen Filled-50.png"),QIcon::Normal,QIcon::Off);
    ui->fullscreen->setIcon(ico);
    ui->fullscreen->setCheckable(true);
}

void SimplePlayer::on_pause_clicked()
{
//    QIcon icon;
//    icon.addPixmap(QPixmap("://icons/play.png"),QIcon::Normal,QIcon::On);
//    icon.addPixmap(QPixmap("://icons/pause.png"),QIcon::Normal,QIcon::Off);
//    ui->pause->setIcon(icon);
//    ui->pause->setCheckable(true);
    if(ui->pause->isChecked()){
         _player->pause();
        }
        else{
        _player->play();
        }
}

//void SimplePlayer::playPause()
//{

//    switch (mediaObject->state()){
//        case Phonon::PlayingState:
//            mediaObject->pause();
//            ui->pushButtonPlay->setChecked(false);
//            break;
//        case Phonon::PausedState:
//            _player->&VlcMediaPlayer::togglePause;
//            break;
//        case Phonon::StoppedState:
//            mediaObject->play();
//            break;
//        case Phonon::LoadingState:
//            ui->pushButtonPlay->setChecked(false);
//            break;
//    }

//    qDebug()<<"playPause";
//    qDebug()<<mediaObject->state();
//}



void SimplePlayer::on_volume_valueChanged(int value)
{
    QIcon icon;
    if(value>=1&&value<=30)
    {
        icon.addPixmap(QPixmap("://icons/Low Volume-64.png"),QIcon::Normal,QIcon::On);
    }
    else if(value>30&&value<60)
    {
        icon.addPixmap(QPixmap("://icons/Medium Volume-64.png"),QIcon::Normal,QIcon::On);
    }
    else if(value>=60)
    {
        icon.addPixmap(QPixmap("://icons/High Volume-64.png"),QIcon::Normal,QIcon::On);
    }
    else if(value==0)
    {
        icon.addPixmap(QPixmap("://icons/Mute-64.png"),QIcon::Normal,QIcon::On);
    }
    ui->volumeButton->setIcon(icon);
}

void SimplePlayer::on_volumeButton_clicked()
{
    if(ui->volume->isHidden())
    {
        ui->volume->show();
    }
    else
    {
        ui->volume->hide();
    }
}

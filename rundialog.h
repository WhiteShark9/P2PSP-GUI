#ifndef RUNDIALOG_H
#define RUNDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <addchannels.h>
#include <globalheader.h>
#include <QDebug>
#include "rundialogstorage.h"
//#include "SimplePlayer.h"
using namespace std;

namespace Ui {
class runDialog;
}

//class PeerThread;

class runDialog : public QDialog
{
    Q_OBJECT

public:
    explicit runDialog(QWidget *parent = 0);
    ~runDialog();

private slots:

    void runClicked();
    void receiveChannels(vector<addChannels> channels);

signals:
    void runPeer(runDialogStorage runParameters);

private:
    //For Storing Channels - used to get Splitter Port and Address
    vector<addChannels> runDialogChannels;

    //For UI Creation
    void buildUi();
    void clearUiElements();

    int labelPos;
    QComboBox *channelsComboBox;
    QLabel *playerPortLabel;
    QLineEdit *playerPortLineEdit;
    QLabel *channelsLabel;
    QLabel *maxChunkLabel;
    QLineEdit *maxChunkLineEdit;
    QLabel *teamPortLabel;
    QSpinBox *teamPortSpinBox;
    QLabel *localHostLabel;
    QCheckBox *localHostCheckBox;
    QLabel *sourcePortStepLabel;
    QLineEdit *sourcePortLineEdit;
    QPushButton *runPushButton;
    QPushButton *cancelPushButton;
    QGridLayout *glayout;

//    PeerThread *peerthreadobj; //used for calling the run method of peerThread
    Ui::runDialog *ui;
};

#endif // RUNDIALOG_H

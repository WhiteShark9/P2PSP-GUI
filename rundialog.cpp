#include "rundialog.h"
#include "ui_rundialog.h"

runDialog::runDialog(QWidget *parent) :
    QDialog(parent),labelPos(2),
    ui(new Ui::runDialog)
{
    ui->setupUi(this);
    buildUi();
}

runDialog::~runDialog()
{
    delete ui;
}

void runDialog::runClicked()
{
    runDialogStorage runParameters;
    runParameters.setPlayerPort(playerPortLineEdit->text().toUInt());
    //cout<<"runDialog Player Port"<<runParameters.getPlayerPort();
    //cout<<"runDialog Player Port"<<runParameters.getPlayerPort();
    if(channelsComboBox->currentText()!="--Select Channel--")
    {
        unsigned int channelSplitterPort=runDialogChannels[channelsComboBox->currentIndex()-1].getSplitterPort();
        runParameters.setSplitterPort(channelSplitterPort);
        string channelSplitterAddress=runDialogChannels[channelsComboBox->currentIndex()-1].getSplitterAddress();
        runParameters.setSplitterAddress(channelSplitterAddress);
    }
#ifndef __IMS__
    runParameters.setMaxChunk(maxChunkLineEdit->text().toInt());
    runParameters.setTeamPort(teamPortSpinBox->text().toUInt());
    runParameters.setLocalHost(localHostCheckBox->isChecked());
#endif
#ifdef __NTS__
    #ifndef __monitor__
    runParameters.setSourcePortStep(sourcePortLineEdit->text().toInt());
    #endif
#endif
    this->hide();
    clearUiElements();
    emit runPeer(runParameters);

}
void runDialog::clearUiElements()
{
    playerPortLineEdit->clear();
    channelsComboBox->clear();
#ifndef __IMS__
    maxChunkLineEdit->clear();
    teamPortSpinBox->clear();
    localHostCheckBox->setChecked(false);
#endif
#ifdef __NTS__
    #ifndef __monitor__
    sourcePortLineEdit->clear();
    #endif
#endif
}

void runDialog::receiveChannels(vector<addChannels> channels)
{
    for(auto chnnl:channels)
    {
        QIcon icon;
        if(chnnl.getThumbnail().isNull())
        {
            icon.addPixmap(QPixmap("://icons/p2psp.png"));
        }
        else
        {
            icon.addPixmap(chnnl.getThumbnail());
        }
        channelsComboBox->addItem(icon,QString::fromStdString(chnnl.getName()));
        runDialogChannels.push_back(chnnl); //used in runClicked Method for retrieving splitter port and address
    }
}

void runDialog::buildUi()
{
    glayout = new QGridLayout(this);
    playerPortLabel = new QLabel();
    playerPortLabel->setText("Player Port");
    glayout->addWidget(playerPortLabel,0,0);
    playerPortLineEdit = new QLineEdit();
    glayout->addWidget(playerPortLineEdit,0,1,1,3);
    channelsLabel = new QLabel();
    channelsLabel->setText("Channels");
    glayout->addWidget(channelsLabel,1,0);
    channelsComboBox=new QComboBox();
    glayout->addWidget(channelsComboBox,1,1,1,3);
    channelsComboBox->addItem("--Select Channel--");
#ifndef __IMS__
    maxChunkLabel=new QLabel();
    maxChunkLabel->setText("Max Chunk Debt");
    glayout->addWidget(maxChunkLabel,labelPos,0);
    maxChunkLineEdit = new QLineEdit();
    glayout->addWidget(maxChunkLineEdit,labelPos,1,1,3);
    labelPos++;
    teamPortLabel=new QLabel();
    teamPortLabel->setText("Team Port");
    glayout->addWidget(teamPortLabel,labelPos,0);
    teamPortSpinBox = new QSpinBox();
    teamPortSpinBox->setMaximum(65536);
    glayout->addWidget(teamPortSpinBox,labelPos,1,1,3);
    labelPos++;
    localHostLabel=new QLabel();
    localHostLabel->setText("Local Host");
    glayout->addWidget(localHostLabel,labelPos,0);
    localHostCheckBox = new QCheckBox();
    glayout->addWidget(localHostCheckBox,labelPos,1,1,3);
    labelPos++;
#endif

#ifdef __NTS__
    #ifndef __monitor__
        sourcePortStepLabel=new QLabel();
        sourcePortStepLabel->setText("Source Port Step");
        glayout->addWidget(sourcePortStepLabel,labelPos,0);
        sourcePortLineEdit = new QLineEdit();
        glayout->addWidget(sourcePortLineEdit,labelPos,1,1,3);
        labelPos++;
    #endif
#endif
    runPushButton = new QPushButton();
    runPushButton->setText("Run");
    glayout->addWidget(runPushButton,labelPos,2);
    cancelPushButton = new QPushButton();
    cancelPushButton->setText("Cancel");
    glayout->addWidget(cancelPushButton,labelPos,3);
    QObject::connect(cancelPushButton,SIGNAL(pressed()),this,SLOT(hide()));
    QObject::connect(runPushButton,SIGNAL(pressed()),this,SLOT(runClicked()));
}


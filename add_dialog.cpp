#include "add_dialog.h"
#include "ui_add_dialog.h"

add_dialog::add_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_dialog)
{
    ui->setupUi(this);
}

add_dialog::~add_dialog()
{
    delete ui;
}

void add_dialog::GetImage(QString url)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    QUrl url1 = QUrl(url);

    QNetworkRequest request(url1);

    manager->get(request);
}

void add_dialog::replyFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        cout<<"Error: "+ reply->errorString().toStdString();
       // ui->textBrowser->setText("Error: " +  reply->errorString());
    }
    else
    {
        QByteArray responseData = reply->readAll();
        pixi.loadFromData(responseData.data());
    }
}

void add_dialog::on_addButton_clicked()
{
    addChannels chnnl;
    chnnl.setName(ui->name->text().toStdString());
    chnnl.setDescription(ui->description->text().toStdString());
    chnnl.setThumbnailUrl(ui->thumbnail->text().toStdString());
    if(chnnl.getThumbnailUrl()!="")
    {
        GetImage(QString::fromStdString(chnnl.getThumbnailUrl()));
        if(!pixi.isNull())
        {
            chnnl.setThumbnail(pixi);
        }
    }
    chnnl.setSplitterAddress(ui->splitteraddress->text().toStdString());
    chnnl.setSplitterPort(ui->splitterport->text().toUInt());
    clearUiElements();
    this->window()->hide();
    emit addChannelSignal(chnnl);
}
void add_dialog::clearUiElements()
{
    ui->name->clear();
    ui->description->clear();
    ui->thumbnail->clear();
    ui->splitteraddress->clear();
    ui->splitterport->setValue(0);
}

void add_dialog::on_cancelButton_clicked()
{
    clearUiElements();
    this->window()->hide();
}

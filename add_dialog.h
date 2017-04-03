#ifndef ADD_DIALOG_H
#define ADD_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <iostream>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include "addchannels.h"

using namespace std;

namespace Ui {
class add_dialog;
}

class add_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit add_dialog(QWidget *parent = 0);
    ~add_dialog();

private slots:

    void on_addButton_clicked();

    void on_cancelButton_clicked();

    void replyFinished(QNetworkReply *reply);
signals:
    void addChannelSignal(addChannels);

private:
    QPixmap pixi; //used for storing the Thumbnail

    void clearUiElements();
    void GetImage(QString url);

    Ui::add_dialog *ui;
};

#endif // ADD_DIALOG_H

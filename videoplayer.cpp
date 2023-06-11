

#include "videoplayer.h"
#include "ui_videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent, QString *videoOne, QString *videoTwo) :
    QMainWindow(parent),
    ui(new Ui::VideoPlayer)
{
    ui->setupUi(this);

    this->installEventFilter(this);

    this->video[0] = *videoOne;
    this->video[1] = *videoTwo;

    convertBase64ToMp4();

    player = new QMediaPlayer;

    QPalette palette = ui->videoWidget->palette();
    palette.setColor(QPalette::Window, Qt::black);
    ui->videoWidget->setPalette(palette);

    player->setVideoOutput(ui->videoWidget);

    audioOutput = new QAudioOutput();

    ui->videoWidget->setAutoFillBackground(true);
    player->setSource(QUrl::fromLocalFile("videoOne.mp4"));

    player->setAudioOutput(audioOutput);
    player->play();

    ui->playVideo->setDisabled(true);
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

bool VideoPlayer::eventFilter(QObject *obj, QEvent *event)
{
    {
        if (event->type() == QEvent::Close) {
            player->stop();
            delete player;
            player = nullptr;
            delete audioOutput;
            audioOutput = nullptr;
        }
        return false;
    }
}

void VideoPlayer::convertBase64ToMp4()
{
    QFile::remove("videoOne.mp4");
    QFile::remove("videoTwo.mp4");

    QByteArray videoDataOne = QByteArray::fromBase64(video[0].toUtf8());

    QBuffer bufferOne(&videoDataOne);
    bufferOne.open(QIODevice::ReadOnly);

    QFile videoOne("videoOne.mp4");
    if (videoOne.open(QIODevice::WriteOnly))
    {
        videoOne.write(videoDataOne);
        videoOne.close();
    }

    QByteArray videoDataTwo = QByteArray::fromBase64(video[1].toUtf8());

    QBuffer bufferTwo(&videoDataTwo);
    bufferTwo.open(QIODevice::ReadOnly);

    QFile videoTwo("videoTwo.mp4");
    if (videoTwo.open(QIODevice::WriteOnly))
    {
        videoTwo.write(videoDataTwo);
        videoTwo.close();
    }
}

void VideoPlayer::on_stopVideo_clicked()
{
    player->stop();

    ui->stopVideo->setDisabled(true);
    ui->playVideo->setDisabled(false);
    ui->pause->setDisabled(false);
}


void VideoPlayer::on_playVideo_clicked()
{
    player->play();

    ui->stopVideo->setDisabled(false);
    ui->playVideo->setDisabled(true);
    ui->pause->setDisabled(false);
}


void VideoPlayer::on_pause_clicked()
{
    player->pause();

    ui->stopVideo->setDisabled(false);
    ui->playVideo->setDisabled(false);
    ui->pause->setDisabled(true);
}


void VideoPlayer::on_next_clicked()
{
    if (currentVideo == 0)
    {
        player->stop();

        player->setSource(QUrl::fromLocalFile("videoTwo.mp4"));

        player->setAudioOutput(audioOutput);
        player->play();

        ui->playVideo->setDisabled(true);
        currentVideo++;
    }
    else
    {
        player->stop();

        player->setSource(QUrl::fromLocalFile("videoOne.mp4"));

        player->setAudioOutput(audioOutput);
        player->play();

        ui->playVideo->setDisabled(true);
        currentVideo--;
    }

    ui->stopVideo->setDisabled(false);
    ui->playVideo->setDisabled(true);
    ui->pause->setDisabled(false);
}


void VideoPlayer::on_previous_clicked()
{
    if (currentVideo == 0)
    {
        player->stop();

        player->setSource(QUrl::fromLocalFile("videoTwo.mp4"));

        player->setAudioOutput(audioOutput);
        player->play();

        ui->playVideo->setDisabled(true);
        currentVideo++;
    }
    else
    {
        player->stop();

        player->setSource(QUrl::fromLocalFile("videoOne.mp4"));

        player->setAudioOutput(audioOutput);
        player->play();

        ui->playVideo->setDisabled(true);
        currentVideo--;
    }

    ui->stopVideo->setDisabled(false);
    ui->playVideo->setDisabled(true);
    ui->pause->setDisabled(false);
}


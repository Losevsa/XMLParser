#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QByteArray>
#include <QFile>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVBoxLayout>
#include <QWidget>
#include <QMediaPlayer>
#include <QBuffer>
#include <QVideoFrame>
#include <QtWidgets>
#include <QVideoWidget>
#include <QAudioOutput>

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr, QString *videoOne = nullptr, QString *videoTwo = nullptr);
    ~VideoPlayer();

    bool eventFilter(QObject *obj, QEvent *event) override;

    void convertBase64ToMp4();

private slots:
    void on_stopVideo_clicked();
    void on_playVideo_clicked();
    void on_pause_clicked();
    void on_next_clicked();

    void on_previous_clicked();

private:
    Ui::VideoPlayer *ui;
    QString video[2];
    QMediaPlayer *player;
    QAudioOutput* audioOutput;
    int currentVideo = 0;
};

#endif // VIDEOPLAYER_H

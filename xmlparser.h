#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QClipboard>
#include <QMainWindow>
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QMap>
#include <QImage>
#include <QByteArray>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QBuffer>
#include <QTemporaryFile>
#include <QMediaPlayer>
#include <QStringView>

#include "videoplayer.h"

enum violations
{

};

namespace Ui {
class XMLParser;
}

class XMLParser : public QMainWindow
{
    Q_OBJECT

public:
    explicit XMLParser(QWidget *parent = nullptr);
    ~XMLParser();    

    void saveSettings();
    void loadSettings();

private slots:
    void on_choseXmlFile_clicked();
    void on_next_clicked();
    void on_prev_clicked();

    void on_playVideo_clicked();

private:
    Ui::XMLParser *ui;
    QImage images[10];
    QString video[2];
    QString picFiles[10];
    int currentFile = 0;
    QString pathToFile = "";
    QClipboard* clipboard;

    //VideoPlayer* videoTwo;
};

#endif // XMLPARSER_H

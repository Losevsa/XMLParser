#include "xmlparser.h"
#include "ui_xmlparser.h"


XMLParser::XMLParser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::XMLParser)
{
    ui->setupUi(this);
    loadSettings();
    ui->pic->setScaledContents(true);
    ui->playVideo->setDisabled(true);
    ui->prev->setDisabled(true);
    ui->next->setDisabled(true);
}

XMLParser::~XMLParser()
{
    delete ui;
    //delete videoOne;
    //delete videoTwo;
}

void XMLParser::saveSettings()
{
    QSettings settings("Losevsa", "XmlParser");
    settings.setValue("Settings/lastOpenedDir", pathToFile);
}

void XMLParser::loadSettings()
{
    QSettings settings("Losevsa", "XmlParser");
    pathToFile = settings.value("Settings/lastOpenedDir").toString();
}

void XMLParser::on_choseXmlFile_clicked()
{
    //=====================================================================================
    //очищаем пути до фоток, обнуляем порядок, очищаем лог
    //=====================================================================================
    for(int i = 0; i < 10; i++)
    {
        picFiles[i] = "";
        images[i] = QImage();
    }
    video[0].clear();
    video[1].clear();

    ui->progress->clear();
    currentFile = 0;
    ui->fileSize->setStyleSheet("background-color : white;");

    //=====================================================================================
    //запоминаем путь до файла и сам файл
    //=====================================================================================
    QString xmlFilePath =  QFileDialog::getOpenFileName( this , tr( "Открыть файл" ) , pathToFile ,
                                                tr( "Xml файлы (*.xml)" ));
    ui->filePathXml->setText(xmlFilePath);

    //=====================================================================================
    //если файл выбрали
    //=====================================================================================
    if(xmlFilePath != "")
    {
        //=====================================================================================
        //открыли файл
        //=====================================================================================
        QFile* file = new QFile(xmlFilePath);

        //=====================================================================================
        //отдельно выберем путь до файла
        //=====================================================================================

        QFileInfo fileInfo(file->fileName());
        double fileSize = (file->size());
        fileSize = fileSize / 1024 / 1024;
        ui->fileSize->setText(QString::number(fileSize));
        if (fileSize > 25)
        {
            ui->fileSize->setStyleSheet("background-color : red;");
        }

        QString filename(fileInfo.fileName());

        pathToFile.clear();
        for(int i = 0; i < xmlFilePath.size() - filename.size(); i++)
        {
            pathToFile += xmlFilePath[i];
        }

        //=====================================================================================
        //пробуем открыть файл
        //=====================================================================================
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui->statusbar->showMessage("Не могу открыть файл");
            return;
        }
        else
        {
            ui->statusbar->showMessage("Файл открыт");
        }

        //=====================================================================================
        //Открываем файл через qtxml и создаем bytearray
        //=====================================================================================
        QXmlStreamReader xmlDoc(file);

        QByteArray base64Data;
        int extraPhotoCount = 1;
        int videoCount = 0;

        QString latitude = "";
        QString longitude = "";

        //=====================================================================================
        //парсим xml файл
        //=====================================================================================
        while (!xmlDoc.atEnd() && !xmlDoc.hasError())
        {
            QXmlStreamReader::TokenType token = xmlDoc.readNext();

            if (token == QXmlStreamReader::StartElement)
            {
                //=====================================================================================
                //скорость
                //=====================================================================================
                QStringView v_speed(QString::fromUtf8("v_speed"));
                if (xmlDoc.name() == v_speed)
                {
                    ui->speed->setText(xmlDoc.readElementText());
                    qDebug() << "v_speed = " + xmlDoc.readElementText();
                    continue;
                }

                //=====================================================================================
                //код нарушения
                //=====================================================================================
                QStringView v_pr_viol(QString::fromUtf8("v_pr_viol"));
                if (xmlDoc.name() == v_pr_viol)
                {
                    ui->violation->setText(xmlDoc.readElementText());
                    continue;
                }

                //=====================================================================================
                //номер
                //=====================================================================================
                QStringView v_regno(QString::fromUtf8("v_regno"));
                if (xmlDoc.name() == v_regno)
                {
                    ui->number->setText(xmlDoc.readElementText());
                    continue;
                }

                //=====================================================================================
                //дата
                //=====================================================================================
                QStringView v_time_check(QString::fromUtf8("v_time_check"));
                if (xmlDoc.name() == v_time_check)
                {
                    ui->date->setText(xmlDoc.readElementText());
                    continue;
                }

                //=====================================================================================
                //адрес
                //=====================================================================================
                QStringView v_camera_place(QString::fromUtf8("v_camera_place"));
                if (xmlDoc.name() == v_camera_place)
                {
                    ui->address->setText(xmlDoc.readElementText());
                    continue;
                }

                //=====================================================================================
                //широта
                //=====================================================================================
                QStringView v_gps_y(QString::fromUtf8("v_gps_y"));
                if (xmlDoc.name() == v_gps_y)
                {
                    latitude = xmlDoc.readElementText();
                    continue;
                }

                //=====================================================================================
                //долгота
                //=====================================================================================
                QStringView v_gps_x(QString::fromUtf8("v_gps_x"));
                if (xmlDoc.name() == v_gps_x)
                {
                    longitude = xmlDoc.readElementText() ;
                    continue;
                }

                //=====================================================================================
                //идентификатор камеры
                //=====================================================================================
                QStringView v_camera(QString::fromUtf8("v_camera"));
                if (xmlDoc.name() == v_camera)
                {
                    ui->cameraIdentif->setText(xmlDoc.readElementText());
                    continue;
                }

                //=====================================================================================
                //фото номер
                //=====================================================================================
                QStringView v_photo_grz(QString::fromUtf8("v_photo_grz"));
                if (xmlDoc.name() == v_photo_grz)
                {
                    base64Data = xmlDoc.readElementText().toUtf8();

                    //QImage image;
                    images[currentFile].loadFromData(QByteArray::fromBase64(base64Data), "jpg");
                    ui->progress->append("Фото номера - ок");
                    /*
                    if(image.save(pathToFile + "plate.jpg", "jpg"))
                    {
                        ui->progressInformation->append( "plate.jpg - создан");
                    }
                    else
                    {
                        ui->progressInformation->append( "plate.jpg - проблемы");
                    }
                    */
                    //picFiles[currentFile] = pathToFile + "plate.jpg";
                    ui->pic->setPixmap(QPixmap::fromImage(images[currentFile]));
                    currentFile++;
                    //ui->pic->setPixmap(pathToFile + "plate.jpg");
                    continue;
                }

                //=====================================================================================
                //фото ТС main
                //=====================================================================================
                QStringView v_photo_ts(QString::fromUtf8("v_photo_ts"));
                if (xmlDoc.name() == v_photo_ts)
                {
                    base64Data = xmlDoc.readElementText().toUtf8();

                    //QImage image;
                    images[currentFile].loadFromData(QByteArray::fromBase64(base64Data), "jpg");
                    ui->progress->append("Фото вырез тс - ок");
                    /*
                    if(image.save(pathToFile + "car.jpg", "jpg"))
                    {
                        ui->progressInformation->append( "car.jpg - создан");
                    }
                    else
                    {
                        ui->progressInformation->append( "car.jpg - проблемы");
                    }
                    */
                    //picFiles[currentFile] = pathToFile + "car.jpg";
                    currentFile++;
                    continue;
                }

                //=====================================================================================
                //поочереди открываем extra фото и смотрим что там за тип, после чего создаем фото
                //=====================================================================================
                QStringView v_photo_extra(QString::fromUtf8("v_photo_extra"));
                if(xmlDoc.name() == v_photo_extra)
                {
                    base64Data = xmlDoc.readElementText().toUtf8();
                    continue;
                }

                //=====================================================================================
                //проверяем что за тип
                //=====================================================================================
                QStringView v_type_photo(QString::fromUtf8("v_type_photo"));
                if(xmlDoc.name() == v_type_photo)
                {
                    QString element = xmlDoc.readElementText();

                    //=====================================================================================
                    //1 = просто доп фото(?)
                    //=====================================================================================
                    if(element == "1")
                    {
                        //QImage image;
                        images[currentFile].loadFromData(QByteArray::fromBase64(base64Data), "jpg");
                        ui->progress->append("Обзорное фото тс - ок");
                        /*
                        if(image.save(pathToFile + "overview" + QString::number(extraPhotoCount) + ".jpg", "jpg"))
                        {
                            ui->progressInformation->append( "overview.jpg - создан");
                        }
                        else
                        {
                            ui->progressInformation->append( "overview.jpg - проблемы");
                        }
                        */
                        //picFiles[currentFile] = pathToFile + "overview" + QString::number(extraPhotoCount) + ".jpg";
                        currentFile++;
                        extraPhotoCount++;
                    }

                    //=====================================================================================
                    //3 = видео, надо добавить поддержку ffmpeg и выгружать видос через него
                    //=====================================================================================
                    if(element == "3" || element == "3 ")
                    {
                        //ui->progressInformation->append( "Создание видео не поддерживается, создан файл с Base64");

                        //QFile file(pathToFile + "video" + QString::number(videoCount) + ".txt");
                        /*
                        if (file.open(QIODevice::WriteOnly))
                        {
                            QTextStream stream( &file );
                            stream << base64Data;
                        }
                        */
                        if(videoCount == 0)
                        {
                            video[0] = base64Data;
                            ui->progress->append("Видео 1 - ок");

                            //ui->video0->append(base64Data);
                        }
                        else
                        {
                            video[1] = base64Data;
                            ui->progress->append("Видео 2 - ок");
                            //ui->video1->append(base64Data);
                        }

                        videoCount++;
                    }

                    //=====================================================================================
                    //а = коллаж
                    //=====================================================================================
                    if(element == "a")
                    {
                        //QImage image;
                        images[currentFile].loadFromData(QByteArray::fromBase64(base64Data), "jpg");
                        ui->progress->append("Коллаж - ок");
                        /*
                        if(image.save(pathToFile + "collage.jpg", "jpg"))
                        {
                            ui->progressInformation->append( "collage.jpg - создан");
                        }
                        else
                        {
                            ui->progressInformation->append( "collage.jpg - проблемы");
                        }
                        */
                        //picFiles[currentFile] = pathToFile + "collage.jpg";
                        currentFile++;
                    }
                }
            }
        }
        ui->longiLati->setText(latitude + ", " + longitude);
    }
    currentFile = 0;
    saveSettings();

    if(!video[0].isNull())
    {
        ui->playVideo->setDisabled(false);
    }
    else
    {
        ui->playVideo->setDisabled(true);
    }

    ui->prev->setDisabled(true);
    ui->next->setDisabled(false);

}


void XMLParser::on_next_clicked()
{
    if(currentFile >= 0 && currentFile < 9)
    {
        if(!images[currentFile + 1].isNull())
        {
            ui->prev->setDisabled(false);
            ui->next->setDisabled(false);
            currentFile++;
            ui->pic->clear();
            ui->pic->setPixmap(QPixmap::fromImage(images[currentFile]));
            //ui->pic->setPixmap(picFiles[currentFile]);
        }
        else
        {
            ui->next->setDisabled(true);
        }
    }
}


void XMLParser::on_prev_clicked()
{
    if(currentFile > 0 && currentFile <= 9)
    {
        if(!images[currentFile - 1].isNull())
        {
            ui->prev->setDisabled(false);
            ui->next->setDisabled(false);
            currentFile--;
            ui->pic->clear();
            ui->pic->setPixmap(QPixmap::fromImage(images[currentFile]));
            //ui->pic->setPixmap(picFiles[currentFile]);
        }
        else
        {
            ui->prev->setDisabled(false);
        }
    }
}

void XMLParser::on_playVideo_clicked()
{
    VideoPlayer* videoOne = new VideoPlayer(this, &video[0], &video[1]);
    videoOne->setWindowTitle("Воспроизведение видео");
    videoOne->show();
}


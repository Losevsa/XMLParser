#include "xmlparser.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XMLParser parser;
    parser.show();
    parser.setWindowTitle("XmlParser");
    return a.exec();
}

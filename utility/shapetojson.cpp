#include "shapetojson.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRect>
#include <QTextStream>


ShapeToJson::ShapeToJson()
{
}


void ShapeToJson::saveRect(QRect rect, QString file_path)
{
    QJsonObject root;
    root["version"] = "0.0.0";

    QJsonObject roiRect;
    roiRect["x"] = rect.x();
    roiRect["y"] = rect.y();
    roiRect["width"] = rect.width();
    roiRect["height"] = rect.height();
    root["roi"] = roiRect;

    QJsonDocument doc;
    doc.setObject(root);

    QFile file(file_path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << doc.toJson().data();
}

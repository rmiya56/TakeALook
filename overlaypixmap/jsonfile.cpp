#include "jsonfile.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPolygonF>
#include <QTextStream>
#include <QDebug>



JsonFile::JsonFile()
{
}

void JsonFile::save(QVector<QPolygonF> polygons)
{
        QJsonArray shapes;
        for(auto& poly : polygons) {

            QJsonObject poly_json;
            poly_json["label"] = "";
            poly_json["type"] = "polygon";

            QJsonArray arr_x, arr_y;
            for(auto& p : poly) {
                arr_x.append(p.x());
                arr_y.append(p.y());
            }
            poly_json["x"] = arr_x;
            poly_json["y"] = arr_y;

            shapes.append(poly_json);
        }
        QJsonObject root;
        root["version"] = "0.0.0";
        root["shapes"] = shapes;

        QJsonDocument doc;
        doc.setObject(root);

        QFile file("./hoge.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        //out << doc.toJson(QJsonDocument::Compact).data();
        out << doc.toJson().data();
}

QVector<QPolygonF> JsonFile::read()
{
    QFile file("./hoge.json");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject obj(doc.object());

    QJsonArray shapes = obj.value("shapes").toArray();

    QVector<QPolygonF> polygons;

    for (auto shape : shapes)
    {
        QJsonObject arrObj =  shape.toObject();
        QJsonArray arr_x =  arrObj.value("x").toArray();
        QJsonArray arr_y =  arrObj.value("y").toArray();

        QPolygonF poly;
        for (int i=0; i<arr_x.size(); i++)
        {
            poly << QPointF(arr_x[i].toDouble(), arr_y[i].toDouble());
        }
        polygons.append(poly);
    }
    return polygons;
}

#ifndef JSONFILE_H
#define JSONFILE_H

#include <QPolygonF>
#include <QVector>


class JsonFile
{
public:
    JsonFile();
    static void savePolygons(QVector<QPolygonF> polygons, QString file_path);
    static QVector<QPolygonF> readPolygons(QString file_path);
};

#endif // JSONFILE_H

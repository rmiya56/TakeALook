#ifndef JSONFILE_H
#define JSONFILE_H

#include <QPolygonF>
#include <QVector>


class JsonFile
{
public:
    JsonFile();
    static void save(QVector<QPolygonF> polygons);
    QVector<QPolygonF> read();
};

#endif // JSONFILE_H

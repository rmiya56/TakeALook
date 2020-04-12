#ifndef SHAPETOJSON_H
#define SHAPETOJSON_H

#include <QRect>



class ShapeToJson
{
public:
    ShapeToJson();
    static void saveRect(QRect rect, QString file_path);
};

#endif // SHAPETOJSON_H

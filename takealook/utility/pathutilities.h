#ifndef PATHUTILITIES_H
#define PATHUTILITIES_H

#include <QFileInfo>
#include <QString>



class PathUtilities
{
public:
    PathUtilities();
    static QString replaceSuffix(QFileInfo file_info, QString suffix);

};

#endif //  PATHUTILITIES_H

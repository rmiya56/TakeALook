#include "../utility/pathutilities.h"
#include <QDir>


PathUtilities::PathUtilities()
{
}

QString PathUtilities::replaceSuffix(QFileInfo file_info, QString suffix)
{
    return file_info.absolutePath() + QDir::separator() + file_info.baseName() + "." + suffix;
}

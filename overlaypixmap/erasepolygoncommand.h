#ifndef ERASEPOLYGONCOMMAND_H
#define ERASEPOLYGONCOMMAND_H

#include "contouritem.h"
#include <QUndoCommand>



class ErasePolygonCommand : public QUndoCommand
{
public:
    ErasePolygonCommand(ContourItem *cont, QVector<ContourItem*> *cont_items, QUndoCommand *parent = nullptr);
    virtual void undo() override;
    virtual void redo() override;

private:
    ContourItem *cont;
    QVector<ContourItem*> *contItems;
    int lastIndex = -1;
};

#endif // ERASEPOLYGONCOMMAND_H

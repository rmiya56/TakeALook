#ifndef ADDPOLYGONCOMMAND_H
#define ADDPOLYGONCOMMAND_H

#include "contouritem.h"
#include <QGraphicsRectItem>
#include <QPolygonF>
#include <QUndoCommand>



class AddPolygonCommand : public QUndoCommand
{
public:
    AddPolygonCommand(ContourItem *cont, QVector<ContourItem*> *cont_items, QUndoCommand *parent = nullptr);
    void undo() override;
    void redo() override;

private:
    ContourItem *cont;
    QVector<ContourItem*> *contItems;
};

#endif // ADDPOLYGONCOMMAND_H

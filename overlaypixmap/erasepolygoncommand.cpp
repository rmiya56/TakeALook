#include "erasepolygoncommand.h"


ErasePolygonCommand::ErasePolygonCommand(ContourItem *cont, QVector<ContourItem *> *cont_items, QUndoCommand *parent)
    : QUndoCommand(parent),
      cont(cont),
      contItems(cont_items)
{
}


void ErasePolygonCommand::undo()
{
    if (contItems->isEmpty()) return;
    contItems->insert(lastIndex, cont);
}

void ErasePolygonCommand::redo()
{
    lastIndex = contItems->indexOf(cont);
    contItems->removeOne(cont);
}

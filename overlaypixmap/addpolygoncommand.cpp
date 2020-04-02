#include "addpolygoncommand.h"



AddPolygonCommand::AddPolygonCommand(ContourItem *cont, QVector<ContourItem *> *cont_items, QUndoCommand *parent)
    : QUndoCommand(parent),
      cont(cont),
      contItems(cont_items)
{
}

void AddPolygonCommand::undo()
{
   if (contItems->isEmpty()) return;
   contItems->takeLast();
}

void AddPolygonCommand::redo()
{
    contItems->push_back(cont);
}




/*
  Copyright (C) 2014 Callum James

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __NODEEDIT_H__
#define __NODEEDIT_H__

#include <QDialog>
#include <QAbstractButton>
#include <QKeyEvent>
#include "GraphNode.h"

/// @file NodeEdit.h
/// @brief Simple UI window to edit the name and short name of a node
/// @author Callum James
/// @version 1.0
/// @date 24/03/2014
/// Revision History:
/// Initial Version 12/03/2014
/// @class NodeEdit
/// @brief Simple UI window to edit the name and short name of a node

namespace Ui
{
    class NodeEdit;
}

class NodeEdit : public QDialog
{
    Q_OBJECT

public:
    /// @brief Ctr for NodeEdit
    /// @param [in] parent QWidget* - the parent of the node
    explicit NodeEdit(QWidget *parent = 0);
    /// @brief dtr
    ~NodeEdit();
    /// @brief Key press event
    /// @param _event QKeyEvent* - the event triggered
    void keyPressEvent(QKeyEvent *_event);
    /// @brief Set the node to edit
    /// @param [in] _node GraphNode* - the node to set
    void setNodeToEdit(GraphNode *_node);

private slots:
    /// @brief Set the node name
    void setNodeName();
    /// @brief Set the ndoe short name
    void setNodeShortName();

private:
    /// @brief The UI for the edit
    Ui::NodeEdit *m_ui;
    /// @brief Connect all signals and init
    void connectAndInit();
    /// @brief The node to edit
    GraphNode *m_nodeToEdit;
};

#endif /* __NODEEDIT_H__ */

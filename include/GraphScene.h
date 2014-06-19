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

#ifndef __GRAPHSCENE_H__
#define __GRAPHSCENE_H__

#include "GraphNode.h"
#include "GraphEdge.h"
#include "NodeEdit.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QLayout>

/// @file GraphScene.h
/// @brief Scene class that si the entire graph area, handles drawing and is a manager for all nodes
/// @author Callum James
/// @version 1.0
/// @date 24/03/2014
/// Revision History:
/// Initial Version 12/03/2014
/// @class GraphScene
/// @brief The main graph scene class. This contains all nodes, edges and sockets and all connections.
/// It also controls all of the reading of the information from the nodes and exports it currently as a string
/// as this is the format expected by the Wizard

/**
 * @mainpage Nodegraph Library
 * @section intro_sec Introduction
 * Welcome to the Nodegraph library. This custom library developed for the MayaAPIWizard allows you to define objects and command line arguments
 * by creating and joining nodes within the Wizard itself.
 * When the nodegraph is asked to return all data, it reads from the single end node, up to all connected nodes. It then returns as a single
 * string each node, its argument and its type, long and short name. Each node is delimited by a #_# and each attribute of a node delimited by a ;.
 * All node type declarations will be delimited by a -- so it is easy to see when a new node starts.
 *
 * @section build_sec Building the Nodegraph
 * To build the nodegraph, you will only need the Qt library. The required Qt version if 5 or higher. Thats it.
 * In the root directory, run qmake (assuming qmake has been setup in your path correctly) and then run make.
 *
 * @section usage_sec Usage
 * Usage of the Nodegrpah library is simple. When included into the application, the nodegraph is a widget that can be added to any container within the UI.
 * Once added, an end node will need to be added. This is the target node for all other nodes. When using the graph within the application:
 *  - Zoom in and out with the mouse wheel
 *  - Right click and drag a node to move it around
 *  - Left click a red outbound socket and drag to a green inbound socket to join the nodes
 *  - Press space and select the node you want to add a new node
 *  - Select a node with left click and press space or delete to remove a node and all connections to that node
 *  - double click a node to enter node edit mode to change its name and short name. Press enter to save the edit
 *
 * @section limitations_sec Current Limitations
 * The nodegraph, in its current iteration, lacks some functionality that needs to be added. Currently for example
 * chained nodes have no effect. The nodegraph is still being developed to rectify such limitations.
 **/

class GraphScene : public QGraphicsView
{
    Q_OBJECT

public:
    /// @brief explicit ctr for the GraphScene
    /// @param [in] parent QWidget* - the parent for the scene
    explicit GraphScene(QWidget *parent = 0);
    /// @brief defualt clas dtr
    ~GraphScene();

    // run through the nodes in the scene and build up a return string to pass back
    // return string is being used here as this is how the wizard reads data
    // it is not universal and would need to be changed for more generic use
    /// @brief Collect information to return
    /// @param [in] _string std::string* - the string to write the result to
    /// @returns bool
    bool collectInformation(std::string *_string); // this function will only work if an end node exists

    // this is simply a test debug function to print out information on each node
    /// @brief Print all node information in the scene
    void printAllNodes();
    /// @brief Init the scene
    void init();
    /// @brief Resize the scene
    /// @param [in] _x float - x position to start at
    /// @param [in] _y float - y position to start at
    /// @param [in] _w float - width
    /// @param [in] _h float - height
    void resize(float _x, float _y, float _w, float _h);
    /// @brief Show the graph view
    void showView();
    /// @brief Add an end node to the scene
    /// @param [in] _title std::string - the name of the end node
    void addEndNode(std::string _title);
    /// @brief Returns if a node is at a point
    /// @param [in] _point QPoint - the point to check
    /// @returns bool
    bool nodeAtPoint(QPoint _point);
    /// @brief Returns if a node is at a point
    /// @param [in] _x qreal - the x point to check
    /// @param [in] _y qreal - the y point to check
    /// @returns bool
    bool nodeAtPoint(qreal _x, qreal _y);
    /// @brief A key press event function
    /// @param [in] _event QKeyEvent* - the event triggered
    void keyPressEvent(QKeyEvent *_event);
    /// @brief A key release event function
    /// @param [in] _event QKeyEvent* - the event triggered
    void keyReleaseEvent(QKeyEvent *_event);
    /// @brief A mouse move event function
    /// @param [in] _event QMouseEvent* - the event triggered
    void mouseMoveEvent(QMouseEvent *_event);
    /// @brief A mouse press event function
    /// @param [in] _event QMouseEvent* - the event triggered
    void mousePressEvent(QMouseEvent *_event);
    /// @brief A mouse double click event function
    /// @param [in] _event QMouseEvent* - the event triggered
    void mouseDoubleClickEvent(QMouseEvent *_event);
    /// @brief A mouse release event function
    /// @param [in] _event QMouseEvent* - the event triggered
    void mouseReleaseEvent(QMouseEvent *_event);
    /// @brief A mouse wheel event function
    /// @param [in] _event QWheelEvent* - the event triggered
    void wheelEvent(QWheelEvent *_event);
    /// @brief Resize event for the scene
    /// @param [in] event QResizeEvent* - the event triggered
    void resizeEvent(QResizeEvent *event);
    /// @brief Add a node to the scene
    /// @param [in] _valueTy VALUE_TYPE - top level type of the node
    /// @param [in] _type NODE_TYPE - bottom level type of the node
    /// @param [in] _point QPointF - the point to add the node at
    /// @param [in] _parent GraphScene* - the parent scene for the node
    /// @param [in] _inboundSK int - number of inbound sockets on the node
    /// @param [in] _outboundSK int - number of outbound sockets on the node
    /// @param [in] _editable bool - whether the node is editable or not
    /// @param [in] _deletable bool - whether the node is deletable or not
    void addNodeToScene(VALUE_TYPE _valueTy, NODE_TYPE _type, QPointF _point, GraphScene *_parent, int _inboundSK=1, int _outboundSK=1, bool _editable=true, bool _deletable=true);
    /// @brief Add an edge to the scene
    /// @param [in] _edge GraphEdge* - the edge to add
    void addEdgeToScene(GraphEdge *_edge);
    /// @brief Add a socket to the scene
    /// @param [in] _socket NodeSocket* - the socket to add
    void addSocketToScene(NodeSocket *_socket);
    /// @brief Remove an item from the scene
    /// @param [in] _item QGraphicsItem* - the item to remove
    void removeFromScene(QGraphicsItem *_item);

signals:
    /// @brief Show the node selection menu
    void nodeMenuRequested(const QPoint&);

public slots:
    // I am making the zoom  and translate functions public slots in case it needs to be updated from somewhere
    // other than this class
    // these can also then easily be called from the mouse wheel event
    /// @brief Zoom into the scene
    void zoomIn();
    /// @brief Zoom out of the scene
    void zoomOut();
    /// @brief Translate across the scene - currently not working due to bug in Qt library
    /// @param [in] _x qreal - move by this in the x
    /// @param [in] _y qreal - move by this in the y
    void navScene(qreal _x, qreal _y) {translate(_x,_y);}

private slots:
    /// @brief Create an object node
    /// @param [in] _type int - the type of node to make
    void createObjectNode(int _type);
    /// @brief Create an argument node
    /// @param [in] _type int - the type of node to make
    void createArgumentNode(int _type);
    /// @brief Create an member node
    /// @param [in] _type int - the type of node to make
    void createMemberNode(int _type);

private:
    /// @brief If the scene is being navigated or not
    bool m_pan;
    /// @brief If a node is being moved
    bool m_moveNode;
    /// @brief If a node is being edited
    bool m_editingNode;
    /// @brief In an edge is being created
    bool m_creatingEdge;
    /// @brief If the edge being created is visible
    bool m_tempEdgeVisible;
    /// @brief If the mouse cursor is over an outbound socket somewhere in the scene
    bool m_cursorOverOutboundSocket;
    /// @brief If the mouse cursor is over an inbound socket somewhere in the scene
    bool m_cursorOverInboundSocket;

    /// @brief The scene itself
    QGraphicsScene *m_scene;
    /// @brief The editing UI used to edit nodes
    NodeEdit *m_nodeEdit;

    /// @brief Vector of all nodes in the scene
    std::vector<GraphNode*> *m_nodesInScene;
    /// @brief The number of nodes in the scene
    int m_numNodesInScene;

    /// @brief Socket used for drawing an edge from a socket to the mouse cursor
    NodeSocket *m_tempSocketForEdgeDrawing;
    /// @brief Temporary edge used when clicking and dragging to create an edge
    GraphEdge *m_tempEdgeForEdgeDrawing;

    /// @brief The active outbound socket
    NodeSocket *m_activeOutboundSocket;
    /// @brief The active inbound socket
    NodeSocket *m_activeInboundSocket;
    /// @brief The active selected node
    GraphNode *m_activeSelectedNode; // this is the node that is currently being interacted with by the user
    // for example if the user is a moving a node, this is the node they will move
    /// @brief If an node is active
    bool m_nodeActive; // boolean flag for if a node is active or not
    /// @brief If a node has been selected
    bool m_nodeSelected;

    /// @brief The menu used for creating a new node
    QMenu *m_nodeSelectMenu;
    /// @brief The menu for all objects
    QMenu* m_objectMenus;
    /// @brief All actions corresponding to the object menu
    std::vector<QAction*> m_objectMenuActions; // vector to hold all possible actions from the object menu
    /// @brief Menu used for all arguments
    QMenu* m_argumentMenus;
    /// @brief All actions corresponding to the the argument menu
    std::vector<QAction*> m_argumentMenuActions; // vector to hold all possible actions from the argument menu
    /// @brief The menu for all members
    QMenu* m_memberMenus;
    /// @brief All actions corresponding to the member menu
    std::vector<QAction*> m_memberMenuActions; // vector to hold all possible actions from the member menu

    // need a global point to be able to create a ndoe at this point
    /// @brief Point to create a nide at
    QPointF m_createNodeAt;
    /// @brief The previous X value
    int m_pastX;
    /// @brief The previous Y value
    int m_pastY;
    /// @brief Remove a node from the scene
    /// @param [in] _nodeToRemove GraphNode* - the node to remove
    /// @returns bool
    bool removeNode(GraphNode *_nodeToRemove);
    /// @brief Remove all nodes
    void removeAllNodes();
    /// @brief Select the active node
    /// @param [in] _select bool - whether to select it or not
    void activeNodeSelected(bool _select);
    /// @brief Find the index of the node
    /// @param [in] _node GraphNode* - the node to find
    /// @param [out] _index int* - the index to write to
    /// @returns bool
    bool findNodeIndex(GraphNode *_node, int *_index);
    /// @brief Populate the node selection menu
    void populateNodeSelectionMenu();
    /// @brief If an end zone is in the scene
    bool m_endNodeInScene; // can only have one end node

private slots:
    /// @brief Show node creation menu
    /// @param _pos QPoint - the point to create the menu at
    void showNodeMenu(const QPoint&_pos);
};

#endif /* __GRAPHSCENE_H__ */

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

#include "GraphScene.h"
#include <QEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QScrollBar>
#include <QResizeEvent>
#include <QDialog>
#include <QSignalMapper>

#include "Utilities.h"

#include<iostream>

GraphScene::GraphScene(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = NULL;
    m_nodeSelectMenu = NULL;
    m_nodesInScene = NULL;
    m_activeSelectedNode = NULL; // this will be NULL unless a node is active
    m_activeOutboundSocket = NULL; // NULL unless a socket is being used
    m_activeInboundSocket = NULL; // NULL unless a socket is being used
    m_nodeEdit = NULL;

    m_scene = new QGraphicsScene(parent);

    setScene(m_scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeAnchor(AnchorViewCenter);
    setInteractive(true);

    setTransformationAnchor(AnchorViewCenter);
}

GraphScene::~GraphScene()
{
    delete m_tempSocketForEdgeDrawing;
    delete m_tempEdgeForEdgeDrawing;

    if (m_nodeSelectMenu)
    {
        delete m_nodeSelectMenu;
    }

    if (m_nodesInScene)
    {
        for (int i = 0; i < m_numNodesInScene; i++)
        {
            delete m_nodesInScene->at(i);
        }
        m_nodesInScene->clear();
        delete m_nodesInScene;
    }
    if (m_scene)
    {
        delete m_scene;
    }
    if (m_nodeEdit != NULL)
    {
        delete m_nodeEdit;
    }
}

bool GraphScene::collectInformation(std::string *_string)
{
    if (m_endNodeInScene)
    {
        std::vector<std::string> gatherVector;
        gatherVector.clear();
        // as the wizard parses the results string on ;, I will put in some recognisable characters to
        // detect when the string is reading passed attributes and when it is finished
        std::string returnString = "#_#;";// #_# will be the recognisable token used to detect when attributes begin in the string and when they end
        // find the end node and work from there
        int endNodeIndex = 0;
        bool found = false;

        while (!found && endNodeIndex < m_numNodesInScene)
        {
            if (m_nodesInScene->at(endNodeIndex)->endNode())
            {
                found = true;
            }
            else
            {
                endNodeIndex++;
            }
        }

        if (found)
        {
            // run through all connected edges on the end node, visit the connected nodes and read their information
            // as this is an end node, it will only have input sockets
            std::vector<std::string> temp;
            temp.clear();
            // for now I will just ignore any chaining and only look at the ones connected up to the end node
            // and not anything connected to them as this at the moment does not mean anything
            // i will also look for any member nodes
            for (int i = 0; i < m_numNodesInScene; i++)
            {
                if (m_nodesInScene->at(i)->valueType() == VT_MEMBER)
                {
                    // ok so this current node is a member node and so we must simply record its information
                    gatherVector.push_back(m_nodesInScene->at(i)->getNodeInfo());
                }
            }

            m_nodesInScene->at(endNodeIndex)->gatherInboundSocketInfo(&temp);
            for (int j =0; j < int(temp.size()); j++)
            {
                gatherVector.push_back(temp.at(j));
            }
            temp.clear();


            // now we have all the information, need to check none of it is repeated
            // I know that node types will always follow a -- so when one of these is found, skip the next
            bool valid = true;
            int count = 0;
            int countj = 0;

            while (countj < int(gatherVector.size()))
            {
                if (gatherVector.at(countj) == "--;")
                {
                    countj +=2;
                }
                else
                {
                    while (valid && count < int(gatherVector.size()))
                    {

                        if (countj != count)
                        {
                            if (gatherVector.at(count) == "--;")
                            {
                                count += 2;
                            }
                            else
                            {
                                if (gatherVector.at(countj) == gatherVector.at(count))
                                {
                                    valid = false;
                                }
                                else
                                {
                                    count++;
                                }
                            }
                        }
                        else
                        {
                            count++;
                        }
                    }
                    if (!valid)
                    {
                        return false;
                    }
                    count = 0;
                    countj++;
                }
            }
        }
        else
        {
            return false;
        }

        // now we have got this far, everything should be groovy, so flatten out the vector to a string
        for (int k = 0; k < int(gatherVector.size()); k++)
        {
            returnString += gatherVector.at(k);
        }

        // finish off the string the same way it started
        returnString += "#_#;";
        *_string = returnString;
        return true;
    }
    return false;
}

void GraphScene::printAllNodes()
{
    std::cout<<"####################################################################"<<std::endl;
    for (int i = 0; i < m_numNodesInScene; i++)
    {
        m_nodesInScene->at(i)->printNodeInfo();
    }
    std::cout<<"####################################################################"<<std::endl;
}

void GraphScene::init()
{
    m_pan = false;
    m_moveNode = false;

    setMouseTracking(true);
    m_creatingEdge = false;
    m_tempEdgeVisible = false;

    m_nodeSelected = false;
    m_editingNode = false;

    m_endNodeInScene = false;

    m_cursorOverOutboundSocket = false;
    m_cursorOverInboundSocket = false;

    m_tempSocketForEdgeDrawing = new NodeSocket(SK_INBOUND,0.0,0.0);
    m_tempSocketForEdgeDrawing->init();
    m_tempSocketForEdgeDrawing->setParentScene(this);
    m_tempEdgeForEdgeDrawing = new GraphEdge();

    m_nodeEdit = new NodeEdit(this);

    m_numNodesInScene = 0;

    m_nodeActive = false; // no node is currently active

    QSizePolicy sizePol(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setSizePolicy(sizePol);

    m_nodesInScene = new std::vector<GraphNode*>;
    m_nodesInScene->clear();

    // set up the ability to open my own context menu when the space bar is pressed
    setContextMenuPolicy(Qt::CustomContextMenu);
    // now connect up the signal to the slot
    connect(this,SIGNAL(nodeMenuRequested(QPoint)),this,SLOT(showNodeMenu(QPoint)));

    // now init the menu and fill it with all available node possibilities
    m_nodeSelectMenu = new QMenu();
    m_objectMenus = new QMenu("Objects");
    m_argumentMenus = new QMenu("Arguments");
    m_memberMenus = new QMenu("Members");

    // clear all action menus
    m_objectMenuActions.clear();
    m_argumentMenuActions.clear();
    m_memberMenuActions.clear();

    m_createNodeAt = QPointF(0.0,0.0);

    populateNodeSelectionMenu();
}

void GraphScene::showView()
{
    show();
}

void GraphScene::addEndNode(std::string _title)
{
    if (!m_endNodeInScene)
    {
        addNodeToScene(VT_END,NT_ENDNODE,QPointF(0.0,0.0),this,1,0,false,false);
        // this will now be the last one in the node list so can access it there
        m_nodesInScene->at(m_numNodesInScene-1)->setNodeTitle(_title);
        m_nodesInScene->at(m_numNodesInScene-1)->setEndNode(true);
        // lets also make this node a little wider to make it noticeable
        m_nodesInScene->at(m_numNodesInScene-1)->setWidth(250.0);
        m_endNodeInScene = true;
    }
}

bool GraphScene::nodeAtPoint(QPoint _point)
{
    return nodeAtPoint(_point.x(),_point.y());
}

bool GraphScene::nodeAtPoint(qreal _x, qreal _y)
{
    // loop through all nodes in the scene and check if they exist over the given point
    std::vector<GraphNode*> nodesFound; // vector of possible nodes at point if more than one are at that point
    // if more than one, then it will select the node with the greatest zDepth value
    // a node is found only if the vector length is 0
    nodesFound.clear();

    int numFound = 0;

    for (int i = 0; i < m_numNodesInScene; i++)
    {
        if (m_nodesInScene->at(i)->nodeOverPoint(_x,_y))
        {
            nodesFound.push_back(m_nodesInScene->at(i));
            numFound++;
        }
    }

    // check if any have been found, if they have not then we can simply return false
    if (numFound == 0)
    {
        activeNodeSelected(false);
        m_activeSelectedNode = NULL;
        return false;
    }

    // now we need to check if there is mroe than 1 node. If there is only one node, we can set the active
    // node and then return true
    if (numFound == 1)
    {
        m_activeSelectedNode = nodesFound.at(0);
        return true;
    }

    // if we have got this far then there are multiple nodes at this point so we need to check their zDepth
    // the one with the highest value will be selected
    int z = 0;
    int indexToUse = 0;

    for (int n = 0; n < numFound; n++)
    {
        int depth = nodesFound.at(n)->zDepth();
        if (depth > z)
        {
            z = depth;
            indexToUse = n;
        }
    }

    // now set the active node to the top node and return true
    m_activeSelectedNode = nodesFound.at(indexToUse);
    return true;
}

void GraphScene::keyPressEvent(QKeyEvent *_event)
{
    if (_event->key() == Qt::Key_Space)
    {
        // here is where the node creation menu should pop up
        // need to get the position of the cursor first
        QPoint p = mapFromGlobal(QCursor::pos());
        int x = p.x();
        int y = p.y();
        // make sure it is within the widget
        if ((x >= 0 && x <= size().width()) && (y >= 0 && y <= size().height()))
        {
            // pop up the selection menu
            emit nodeMenuRequested(p);
        }
    }
    else if (_event->key() == Qt::Key_Backspace || _event->key() == Qt::Key_Delete)
    {
        if (m_nodeSelected)
        {
            m_activeInboundSocket = NULL;
            m_activeOutboundSocket = NULL;
            // now need to delete the selected node
            removeNode(m_activeSelectedNode);
        }
    }
    update();
    viewport()->update();
}

void GraphScene::keyReleaseEvent(QKeyEvent *_event)
{

}

void GraphScene::mouseMoveEvent(QMouseEvent *_event)
{
    if (m_pan)
    {
        // i know this is wrong for now, i just want to prove it will do something first
        // UPDATE: IT DOESNT DO ANYTHING!!!!
        int diffX = _event->x() - (int)m_pastX;
        int diffY = _event->y() - (int)m_pastY;

        //std::cout<<"[ "<<diffX<<" , "<<diffY<<" ]"<<std::endl;

        setDragMode(QGraphicsView::ScrollHandDrag);
        setInteractive(false);

        navScene(diffX,diffY);
    }
    else if (m_moveNode)
    {
        QPointF conv = mapToScene(_event->x(),_event->y());
        // need to add the offset of the cursor within the nodes area to stop it jumping to the mouse cursor
        conv += m_activeSelectedNode->getOffsetToCursor();
        m_activeSelectedNode->setPoint(conv);
        m_activeSelectedNode->updateSockets();
    }
    else if (m_creatingEdge)
    {
        QPointF conv = mapToScene(_event->x(),_event->y());
        m_cursorOverInboundSocket = false;

        int index = 0;
        bool found = false;

        m_tempSocketForEdgeDrawing->setPosition(conv.x(),conv.y());
        m_tempEdgeForEdgeDrawing->updateEdge();
        m_tempEdgeVisible = true;

        while (!found && index < m_numNodesInScene)
        {
            m_activeInboundSocket = m_nodesInScene->at(index)->cursorOverSocket(conv.x(),conv.y(),SK_INBOUND);
            if (m_activeInboundSocket != NULL)
            {
                m_cursorOverInboundSocket = true;
                found = true;
            }
            index++;
        }
    }
    else // else user is just moving the mouse so we can test for inbound sockets
    {
        m_cursorOverOutboundSocket = false;
        QPointF conv = mapToScene(_event->x(),_event->y());

        int index = 0;
        bool found = false;

        while (!found && index < m_numNodesInScene)
        {
            m_activeOutboundSocket = m_nodesInScene->at(index)->cursorOverSocket(conv.x(),conv.y(),SK_OUTBOUND);
            if (m_activeOutboundSocket != NULL)
            {
                m_cursorOverOutboundSocket = true;
                found = true;
            }
            index++;
        }
    }

    m_pastX = _event->x();
    m_pastY = _event->y();

    update();
    viewport()->update();
}

void GraphScene::mousePressEvent(QMouseEvent *_event)
{
    setMouseTracking(false);
    QPointF conv = mapToScene(_event->x(),_event->y());
    if (_event->buttons() == Qt::MiddleButton)
    {
        m_pastX = _event->x();
        m_pastY = _event->y();
        m_pan = true;
    }
    if (_event->buttons() == Qt::RightButton)
    {
        // check to see if this click is over a node
        // only if it is over a node will be set move to true, otherwise there is no point
        if (nodeAtPoint(conv.x(),conv.y()))
        {
            m_moveNode = true;
        }
    }
    if (_event->buttons() ==Qt::LeftButton)
    {
        if (m_cursorOverOutboundSocket)
        {
            // set the position of the dummy socket
            m_tempSocketForEdgeDrawing->setPosition(conv.x(),conv.y());
            m_tempEdgeForEdgeDrawing->setSourceDestinationSockets(m_activeOutboundSocket,m_tempSocketForEdgeDrawing);
            m_tempEdgeForEdgeDrawing->updateEdge();
            addEdgeToScene(m_tempEdgeForEdgeDrawing);
            m_creatingEdge = true;
        }
        else
        {
            activeNodeSelected(false);
            if (nodeAtPoint(conv.x(),conv.y()))
            {
                activeNodeSelected(true);
            }
        }
    }
    update();
    viewport()->update();
}

void GraphScene::mouseDoubleClickEvent(QMouseEvent *_event)
{
    QPointF conv = mapToScene(_event->x(),_event->y());
    if (_event->buttons() == Qt::LeftButton)
    {
        if (nodeAtPoint(conv.x(),conv.y()))
        {
            if (m_activeSelectedNode->editable())
            {
                // flag that a node is being modified
                m_editingNode = true;
                // bring up window here to modify the active node
                // will be my own made ui
                m_nodeEdit->setNodeToEdit(m_activeSelectedNode);
                m_nodeEdit->show();
                // editing is now finished
                m_editingNode = false;
            }
        }
    }
}

void GraphScene::mouseReleaseEvent(QMouseEvent *_event)
{
    // just reset everything as nothing will be happening now
    if (m_pan)
    {
        m_pan = false;
        setDragMode(QGraphicsView::NoDrag);
        setInteractive(true);
    }

    if (m_moveNode)
    {
        m_moveNode = false;
        m_activeSelectedNode = NULL;
    }

    if (m_cursorOverInboundSocket)
    {
        m_activeOutboundSocket->addEdge(m_activeInboundSocket);
    }

    m_creatingEdge = false;
    m_cursorOverOutboundSocket = false;
    m_cursorOverInboundSocket = false;

    if (m_tempEdgeVisible) // a temporary edge will be being drawn if in this state
    {
        removeFromScene(m_tempEdgeForEdgeDrawing);
        m_tempEdgeVisible = false;
    }

    setMouseTracking(true);

    update();
    viewport()->update();
}

void GraphScene::wheelEvent(QWheelEvent *_event)
{
    if (_event->delta() > 0)
    {
        zoomIn();
    }
    else if (_event->delta() < 0)
    {
        zoomOut();
    }
}

void GraphScene::resizeEvent(QResizeEvent *event)
{
    viewport()->update();
}

void GraphScene::addNodeToScene(VALUE_TYPE _valueTy, NODE_TYPE _type, QPointF _point, GraphScene *_parent, int _inboundSK, int _outboundSK, bool _editable, bool _deletable)
{
    if (m_scene != NULL)
    {
        m_nodesInScene->push_back(new GraphNode(_point, _valueTy, _type));
        m_nodesInScene->at(m_numNodesInScene)->setParentScene(_parent);
        m_nodesInScene->at(m_numNodesInScene)->setDeletable(_deletable);
        m_nodesInScene->at(m_numNodesInScene)->setEditable(_editable);
        if (_valueTy == VT_OBJECT)
        {
            // if it is an object, as some of the names are longer, need to widen it a little
            m_nodesInScene->at(m_numNodesInScene)->setWidth(200.0f);
            m_nodesInScene->at(m_numNodesInScene)->setBaseWidth(200.0f);
        }

        for (int i =0 ; i < _inboundSK; i++)
        {
            m_nodesInScene->at(m_numNodesInScene)->addSocket(SK_INBOUND);
        }
        for (int j =0 ; j < _outboundSK; j++)
        {
            m_nodesInScene->at(m_numNodesInScene)->addSocket(SK_OUTBOUND);
        }

        m_scene->addItem(m_nodesInScene->at(m_numNodesInScene));
        m_numNodesInScene++;
    }
    viewport()->update();
}

void GraphScene::addEdgeToScene(GraphEdge *_edge)
{
    if (m_scene && _edge)
    {
        m_scene->addItem(_edge);
    }
    viewport()->update();
}

void GraphScene::addSocketToScene(NodeSocket *_socket)
{
    if (m_scene && _socket)
    {
        m_scene->addItem(_socket);
    }
    viewport()->update();
}

void GraphScene::removeFromScene(QGraphicsItem *_item)
{
    m_scene->removeItem(_item);
    viewport()->update();
}

void GraphScene::zoomIn()
{
    setTransformationAnchor(AnchorUnderMouse);
    scale(1.08, 1.08);
    setTransformationAnchor(AnchorViewCenter);
}

void GraphScene::zoomOut()
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(0.92, 0.92);
    setTransformationAnchor(AnchorViewCenter);
}

void GraphScene::createObjectNode(int _type)
{
    NODE_TYPE temp = static_cast<NODE_TYPE>(_type);
    addNodeToScene(VT_OBJECT,temp,m_createNodeAt,this,0,1);
}

void GraphScene::createArgumentNode(int _type)
{
    NODE_TYPE temp = static_cast<NODE_TYPE>(_type);
    addNodeToScene(VT_ARGUMENTS,temp,m_createNodeAt,this,0,1);
}

void GraphScene::createMemberNode(int _type)
{
    NODE_TYPE temp = static_cast<NODE_TYPE>(_type);
    addNodeToScene(VT_MEMBER,temp,m_createNodeAt,this,0,0);
}

bool GraphScene::removeNode(GraphNode *_nodeToRemove)
{
    int indexToRemove = 0;

    if (findNodeIndex(_nodeToRemove,&indexToRemove))
    {
        if (m_nodesInScene->at(indexToRemove)->deletable())
        {
            activeNodeSelected(false);
            m_activeSelectedNode = NULL;
            removeFromScene(m_nodesInScene->at(indexToRemove));
            delete m_nodesInScene->at(indexToRemove);

            for (int i = indexToRemove; i < m_numNodesInScene - 1; i++)
            {
                m_nodesInScene->at(i) = m_nodesInScene->at(i+1);
            }

            m_numNodesInScene--;
            m_nodesInScene->resize(m_numNodesInScene);
            return true;
        }
        return false;
    }
    return false;
}

void GraphScene::removeAllNodes()
{
//    // need to copy across the number of nodes as m_numNodesInScene is modified
    int controlCount = m_numNodesInScene-1;

    for (int i = controlCount; i > -1; i--)
    {
        removeNode(m_nodesInScene->at(i));
    }
}

void GraphScene::activeNodeSelected(bool _select)
{
    if (m_activeSelectedNode != NULL)
    {
        m_nodeSelected = _select;
        m_activeSelectedNode->setSelectedNode(_select);
        m_activeSelectedNode->update();
    }
}

bool GraphScene::findNodeIndex(GraphNode *_node, int *_index)
{
    bool found = false;

    int indexToRemove = 0;

    if (_node == NULL) return false;

    while (!found && indexToRemove < m_numNodesInScene)
    {
        if (m_nodesInScene->at(indexToRemove) == _node)
        {
            found = true;
        }
        else
        {
            indexToRemove++;
        }
    }
    *_index = indexToRemove;
    return found;
}

void GraphScene::populateNodeSelectionMenu()
{
    m_nodeSelectMenu->addMenu(m_objectMenus);
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ANGLE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_DISTANCE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_TIME).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_LAST).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_COMPOUND).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ENUM).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_GENERIC).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_MATRIX).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_MESSAGE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_BOOLEAN).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONEBYTE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONECHAR).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONESHORT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_TWOSHORT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_THREESHORT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONELONG).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONEINT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_TWOLONG).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_TWOINT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_THREELONG).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_THREEINT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONEFLOAT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_TWOFLOAT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_THREEFLOAT).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ONEDOUBLE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_TWODOUBLE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_THREEDOUBLE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_FOURDOUBLE).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_ADDRESS).c_str()));
        m_objectMenuActions.push_back(m_objectMenus->addAction(GenUtils::nodeTypeToString(NT_OBJ_NUM_LAST).c_str()));
    m_nodeSelectMenu->addMenu(m_argumentMenus);
        m_argumentMenuActions.push_back(m_argumentMenus->addAction(GenUtils::nodeTypeToString(NT_BOOLEAN).c_str()));
        m_argumentMenuActions.push_back(m_argumentMenus->addAction(GenUtils::nodeTypeToString(NT_CHAR).c_str()));
        m_argumentMenuActions.push_back(m_argumentMenus->addAction(GenUtils::nodeTypeToString(NT_DOUBLE).c_str()));
        m_argumentMenuActions.push_back(m_argumentMenus->addAction(GenUtils::nodeTypeToString(NT_FLOAT).c_str()));
        m_argumentMenuActions.push_back(m_argumentMenus->addAction(GenUtils::nodeTypeToString(NT_INT).c_str()));
        m_argumentMenuActions.push_back(m_argumentMenus->addAction(GenUtils::nodeTypeToString(NT_STRING).c_str()));
    m_nodeSelectMenu->addMenu(m_memberMenus);
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_BOOLEAN).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_CHAR).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_DOUBLE).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_FLOAT).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_INT).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_STRING).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_VECTOR).c_str()));
        m_memberMenuActions.push_back(m_memberMenus->addAction(GenUtils::nodeTypeToString(NT_MATRIX).c_str()));

    QSignalMapper *objMapper = new QSignalMapper(this);
    QSignalMapper *argMapper = new QSignalMapper(this);
    QSignalMapper *memMapper = new QSignalMapper(this);

    // now all the actions have been added, need to manually connect up if they are triggered to thr eright node creation function
    connect(m_objectMenuActions.at(0),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(0),NT_OBJ_ANGLE);
    connect(m_objectMenuActions.at(1),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(1),NT_OBJ_DISTANCE);
    connect(m_objectMenuActions.at(2),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(2),NT_OBJ_TIME);
    connect(m_objectMenuActions.at(3),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(3),NT_OBJ_LAST);
    connect(m_objectMenuActions.at(4),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(4),NT_OBJ_COMPOUND);
    connect(m_objectMenuActions.at(5),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(5),NT_OBJ_ENUM);
    connect(m_objectMenuActions.at(6),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(6),NT_OBJ_GENERIC);
    connect(m_objectMenuActions.at(7),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(7),NT_OBJ_MATRIX);
    connect(m_objectMenuActions.at(8),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(8),NT_OBJ_MESSAGE);
    connect(m_objectMenuActions.at(9),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(9),NT_OBJ_BOOLEAN);
    connect(m_objectMenuActions.at(10),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(10),NT_OBJ_ONEBYTE);
    connect(m_objectMenuActions.at(11),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(11),NT_OBJ_ONECHAR);
    connect(m_objectMenuActions.at(12),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(12),NT_OBJ_ONESHORT);
    connect(m_objectMenuActions.at(13),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(13),NT_OBJ_TWOSHORT);
    connect(m_objectMenuActions.at(14),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(14),NT_OBJ_THREESHORT);
    connect(m_objectMenuActions.at(15),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(15),NT_OBJ_ONELONG);
    connect(m_objectMenuActions.at(16),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(16),NT_OBJ_ONEINT);
    connect(m_objectMenuActions.at(17),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(17),NT_OBJ_TWOLONG);
    connect(m_objectMenuActions.at(18),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(18),NT_OBJ_TWOINT);
    connect(m_objectMenuActions.at(19),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(19),NT_OBJ_THREELONG);
    connect(m_objectMenuActions.at(20),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(20),NT_OBJ_THREEINT);
    connect(m_objectMenuActions.at(21),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(21),NT_OBJ_ONEFLOAT);
    connect(m_objectMenuActions.at(22),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(22),NT_OBJ_TWOFLOAT);
    connect(m_objectMenuActions.at(23),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(23),NT_OBJ_THREEFLOAT);
    connect(m_objectMenuActions.at(24),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(24),NT_OBJ_ONEDOUBLE);
    connect(m_objectMenuActions.at(25),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(25),NT_OBJ_TWODOUBLE);
    connect(m_objectMenuActions.at(26),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(26),NT_OBJ_THREEDOUBLE);
    connect(m_objectMenuActions.at(27),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(27),NT_OBJ_FOURDOUBLE);
    connect(m_objectMenuActions.at(28),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(28),NT_OBJ_ADDRESS);
    connect(m_objectMenuActions.at(29),SIGNAL(triggered()),objMapper,SLOT(map()));
    objMapper->setMapping(m_objectMenuActions.at(29),NT_OBJ_NUM_LAST);

    connect(objMapper,SIGNAL(mapped(int)),this,SLOT(createObjectNode(int)));
    //======================================================================================================
    connect(m_argumentMenuActions.at(0),SIGNAL(triggered()),argMapper,SLOT(map()));
    argMapper->setMapping(m_argumentMenuActions.at(0),NT_BOOLEAN);
    connect(m_argumentMenuActions.at(1),SIGNAL(triggered()),argMapper,SLOT(map()));
    argMapper->setMapping(m_argumentMenuActions.at(1),NT_CHAR);
    connect(m_argumentMenuActions.at(2),SIGNAL(triggered()),argMapper,SLOT(map()));
    argMapper->setMapping(m_argumentMenuActions.at(2),NT_DOUBLE);
    connect(m_argumentMenuActions.at(3),SIGNAL(triggered()),argMapper,SLOT(map()));
    argMapper->setMapping(m_argumentMenuActions.at(3),NT_FLOAT);
    connect(m_argumentMenuActions.at(4),SIGNAL(triggered()),argMapper,SLOT(map()));
    argMapper->setMapping(m_argumentMenuActions.at(4),NT_INT);
    connect(m_argumentMenuActions.at(5),SIGNAL(triggered()),argMapper,SLOT(map()));
    argMapper->setMapping(m_argumentMenuActions.at(5),NT_STRING);

    connect(argMapper,SIGNAL(mapped(int)),this,SLOT(createArgumentNode(int)));
    //======================================================================================================
    connect(m_memberMenuActions.at(0),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(0),NT_BOOLEAN);
    connect(m_memberMenuActions.at(1),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(1),NT_CHAR);
    connect(m_memberMenuActions.at(2),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(2),NT_DOUBLE);
    connect(m_memberMenuActions.at(3),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(3),NT_FLOAT);
    connect(m_memberMenuActions.at(4),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(4),NT_INT);
    connect(m_memberMenuActions.at(5),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(5),NT_STRING);
    connect(m_memberMenuActions.at(6),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(6),NT_VECTOR);
    connect(m_memberMenuActions.at(7),SIGNAL(triggered()),memMapper,SLOT(map()));
    memMapper->setMapping(m_memberMenuActions.at(7),NT_MATRIX);

    connect(memMapper,SIGNAL(mapped(int)),this,SLOT(createMemberNode(int)));
    //======================================================================================================
}

void GraphScene::showNodeMenu(const QPoint &_pos)
{
    QPoint global = mapToGlobal(_pos);
    m_createNodeAt = mapToScene(_pos);
    m_nodeSelectMenu->exec(global);
}

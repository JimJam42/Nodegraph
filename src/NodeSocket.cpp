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

#include "NodeSocket.h"
#include "Utilities.h"

#include <iostream>

#include "GraphScene.h"
#include "GraphNode.h"
#include "GraphEdge.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#define LINE_EDGE_WIDTH 0

NodeSocket::NodeSocket(SOCKET_TYPE _type)
{
    m_socketType = _type;
}

NodeSocket::NodeSocket(SOCKET_TYPE _type, qreal _x, qreal _y, float _w, float _h)
{
    m_position = QPointF(_x,_y);
    m_width = _w;
    m_height = _h;

    m_socketType = _type;
}

NodeSocket::NodeSocket(SOCKET_TYPE _type, QPointF _point,  float _w, float _h)
{
    m_position = _point;
    m_width = _w;
    m_height = _h;

    m_socketType = _type;
}

NodeSocket::~NodeSocket()
{
    // if there are any edges left on this socket remove them
    //removeAllEdges();
    // now there are no edges left on this socket, delete it and remove itself from the scene
    m_edges->clear();
    delete m_edges;

    m_parentNode = NULL;
}

void NodeSocket::init()
{
    m_parentNode = NULL;

    m_edges = new std::vector<GraphEdge*>;
    m_edges->clear();
    m_numEdges = 0;

    m_position = QPointF(0.0,0.0);
    m_colour = QColor(102,102,102);
    m_width = 10.0f;
    m_height = 10.0f;
}

void NodeSocket::addEdge(NodeSocket *_dest)
{
    if (_dest)
    {
        GraphEdge *temp = new GraphEdge(this,_dest);
        m_edges->push_back(temp);
        m_numEdges++;
        _dest->addEdgeReference(temp);
        // now need to add this edge to the scene
        m_parentScene->addEdgeToScene(temp);
    }
}

void NodeSocket::addEdgeReference(GraphEdge *_edge)
{
    m_edges->push_back(_edge);
    m_numEdges++;
}

void NodeSocket::updateEdges()
{
    for (int  i =0 ; i < m_numEdges; i++)
    {
        m_edges->at(i)->updateEdge();
    }
}

bool NodeSocket::removeEdge(GraphEdge *_edge)
{
    int indexToRemove = 0;

    m_parentScene->removeFromScene(_edge);

    NodeSocket *other;
    switch (m_socketType)
    {
        case(SK_INBOUND): other = _edge->sourceSocket(); break;
        case(SK_OUTBOUND): other = _edge->destinationSocket(); break;
    }

    if (!other->removeEdgeReference(_edge))
    {
        return false;
    }

    // if found then we need to remove it, tell the other socket this edge is connected to to remove the reference to it
    // and then condense the edge list back down so it does not include this edge
    if (findEdgeIndex(_edge,&indexToRemove))
    {
        for (int i = indexToRemove; i < m_numEdges-1; i++)
        {
            m_edges->at(i) = m_edges->at(i+1);
        }

        // now it has been removed from this socket, tell the other socket to remove the reference

        m_numEdges--;
        m_edges->resize(m_numEdges);

        delete _edge;
        return true;
    }
    return false;
}

bool NodeSocket::removeEdgeReference(GraphEdge *_edge)
{
    // this function does not remove or delete anything
    // instead it removes the reference to an edge from this socket
    // this is called when an edge if removed from another connected node
    int indexToRemove = 0;

    if (findEdgeIndex(_edge,&indexToRemove))
    {
        // do not need to remove from the scene as this will already have been done
        for (int i = indexToRemove; i < m_numEdges-1; i++)
        {
            m_edges->at(i) = m_edges->at(i+1);
        }
        m_numEdges--;
        m_edges->resize(m_numEdges);
        return true;
    }
    return false;
}

void NodeSocket::removeAllEdges()
{
    // loop through all edges connected to this socket and remove them and the reference on the other node
    // need to copy the value of m_numValues to another int as this value is modified during removal
    int controlCount = m_numEdges-1;
    for (int i = controlCount; i > -1; i--)
    {
        removeEdge(m_edges->at(i));
    }
}

bool NodeSocket::socketOverPoint(QPointF _point)
{
    // call the other node over point implementation
    return socketOverPoint(_point.x(),_point.y());
}

bool NodeSocket::socketOverPoint(qreal _x, qreal _y)
{
    return contains(QPointF(_x,_y));
}

void NodeSocket::setPosition(QPointF _position)
{
    prepareGeometryChange();
    m_position = _position;
}

void NodeSocket::setPosition(qreal _x, qreal _y)
{
    prepareGeometryChange();
    m_position = QPointF(_x,_y);
}

void NodeSocket::setColour(QColor _colour)
{
    m_colour = _colour;
}

void NodeSocket::setColour(int _r, int _g, int _b, int _a)
{
    m_colour = QColor(_r,_g,_b,_a);
}

void NodeSocket::setWidth(float _w)
{
    if (_w > 0)
    {
        m_width = _w;
    }
    else
    {
#ifdef DEBUG
        std::cout<<"Warning, attempting to set a 0 or negative width, ignoring!"<<std::endl;
#endif
    }
}

void NodeSocket::setHeight(float _h)
{
    if (_h > 0)
    {
        m_height = _h;
    }
    else
    {
#ifdef DEBUG
        std::cout<<"Warning, attempting to set a 0 or negative width, ignoring!"<<std::endl;
#endif
    }
}

void NodeSocket::setParentNode(GraphNode *_parent)
{
    if (_parent)
    {
        m_parentNode = _parent;
    }
}

void NodeSocket::setParentScene(GraphScene *_scene)
{
    if (_scene)
    {
        m_parentScene = _scene;
    }
}

QPointF NodeSocket::centre()
{
    return QPointF(m_position.x() + m_width*0.5,m_position.y() + m_height*0.5);
}

QRectF NodeSocket::boundingRect() const
{
    return QRectF(m_position.x() - LINE_EDGE_WIDTH, m_position.y() - LINE_EDGE_WIDTH,
                 m_width + LINE_EDGE_WIDTH, m_height + LINE_EDGE_WIDTH);
}

QPainterPath NodeSocket::shape() const
{
    QPainterPath path;
    path.addRect(m_position.x(),m_position.y(),m_width,m_height);
    return path;
}

void NodeSocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);

    QLinearGradient gradient(0,0,1,1);

    gradient.setColorAt(0, m_colour);

    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawRect(m_position.x(),m_position.y(),m_width,m_height);
}

void NodeSocket::printSocketInfo()
{
    switch (m_socketType)
    {
        case(SK_INBOUND): std::cout<<"\n------------------------------------ Inbound Socket --------------------------------------"<<std::endl;break;
        case(SK_OUTBOUND): std::cout<<"\n------------------------------------ Outbound Socket --------------------------------------"<<std::endl;break;
    }

    std::cout<<"Number of edges: "<<m_numEdges<<std::endl;

    std::cout<<"\n------------------------------------------------------------------------------------------"<<std::endl;
}

bool NodeSocket::getConnectedNodeDetails(std::vector<std::string> *_vec)
{
    // return a vector to be able to detect if there are empty fields and any duplications
    std::vector<std::string> returnString;
    returnString.clear();

    GraphNode* other;

    for (int i = 0; i < m_numEdges; i++)
    {
        switch(m_socketType)
        {
            case(SK_INBOUND):
            {
                other = m_edges->at(i)->sourceNode();
            }break;
            case(SK_OUTBOUND):
            {
                other = m_edges->at(i)->destinationNode();
            }break;
        }
        if (GenUtils::nodeTypeToString(other->nodeType()) == "" || other->name() == "" || other->shortName() == "")
        {
            return false;
        }
        returnString.push_back(std::string(GenUtils::valueTypeToString(other->valueType())+";"));
        returnString.push_back(std::string(GenUtils::nodeTypeToString(other->nodeType())+";"));
        returnString.push_back(std::string(other->name()+";"));
        returnString.push_back(std::string(other->shortName()+";"));
        returnString.push_back("--;"); // escape character for me to see where one edge ends and another begins
    }
    for (int j = 0; j < int(returnString.size()); j++)
    {
        _vec->push_back(returnString.at(j));
    }
    return true;
}

bool NodeSocket::findEdgeIndex(GraphEdge *_edge, int *_index)
{
    int indexToRemove = 0;

    bool found = false;
    // first we need to find if the edge is contained in this sockets list of edges
    while (!found && indexToRemove < m_numEdges)
    {
        if (m_edges->at(indexToRemove) == _edge)
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

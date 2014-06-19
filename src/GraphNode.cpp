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

#include "GraphNode.h"
#include "GraphScene.h"
#include "GraphEdge.h"
#include "NodeSocket.h"
#include "Utilities.h"

#include <iostream>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QFormLayout>
#include <QGraphicsGridLayout>
#include <QString>
#include <QFontMetrics>

#define LINE_EDGE_WIDTH 0

GraphNode::GraphNode(VALUE_TYPE _vType, NODE_TYPE _type, QGraphicsItem *_parent) : QGraphicsItem(_parent)
{
    init(_vType,_type);
}

GraphNode::GraphNode(QPointF _point, VALUE_TYPE _vType, NODE_TYPE _type, QGraphicsItem *_parent) : QGraphicsItem(_parent)
{
    init(_vType, _type,_point);
}

GraphNode::GraphNode(qreal _x, qreal _y, VALUE_TYPE _vType, NODE_TYPE _type, QGraphicsItem *_parent) : QGraphicsItem(_parent)
{
    init(_vType, _type,QPointF(_x,_y));
}

GraphNode::~GraphNode()
{
    // take care of the inbound and outbound sockets
    if (m_inboundSockets)
    {
        for (int i = 0; i < m_numInboundSockets; i++)
        {
            m_inboundSockets->at(i)->removeAllEdges();
            m_parentScene->removeFromScene(m_inboundSockets->at(i));
            delete m_inboundSockets->at(i);
        }
        m_inboundSockets->clear();
        delete m_inboundSockets;
    }

    if (m_outboundSockets)
    {
        for (int i = 0; i < m_numOutboundSockets; i++)
        {
            m_outboundSockets->at(i)->removeAllEdges();
            m_parentScene->removeFromScene(m_outboundSockets->at(i));
            delete m_outboundSockets->at(i);
        }
        m_outboundSockets->clear();
        delete m_outboundSockets;
    }

    delete m_nodeName;
    delete m_nodeShortName;
    delete m_nodeTypeText;
}

void GraphNode::updateSockets()
{
    for (int i = 0; i < m_numOutboundSockets; i++)
    {
        m_outboundSockets->at(i)->updateEdges();
    }
    for (int i = 0; i < m_numInboundSockets; i++)
    {
        m_inboundSockets->at(i)->updateEdges();
    }
}

void GraphNode::setParentScene(GraphScene *_scene)
{
    if (_scene)
    {
        m_parentScene = _scene;
    }
}

void GraphNode::addSocket(SOCKET_TYPE _type)
{
    switch(_type)
    {
        case(SK_INBOUND): addInboundSocket(); break;
        case(SK_OUTBOUND): addOutboundSocket(); break;
    }
}

bool GraphNode::nodeOverPoint(QPointF _point)
{
    // call the other node over point implementation
    return nodeOverPoint(_point.x(),_point.y());
}

bool GraphNode::nodeOverPoint(qreal _x, qreal _y)
{
    bool active = contains(QPointF(_x,_y));

    // set the offset to 0 so if the cursor is not within, it will not affect any previous offsets
    m_offsetToCursor = QPointF(0.0,0.0);

    if (active)
    {
        QPointF tl = boundingRect().topLeft();
        m_offsetToCursor = QPointF(tl - QPointF(_x,_y));
    }
    return active;
}

NodeSocket *GraphNode::cursorOverSocket(QPointF _point, SOCKET_TYPE _type)
{
    return cursorOverSocket(_point.x(),_point.y(), _type);
}

NodeSocket *GraphNode::cursorOverSocket(qreal _x, qreal _y, SOCKET_TYPE _type)
{
    // this function will go through all of the attached sockets and see if the cursor is over any of them
    // this will be used for clicking and dragging the outbound nodes to connect them up
    // or it will be used when trying to connect up a node an inbound node
    // the nodes to look at will be determined by the socket type passed

    switch(_type)
    {
        case(SK_INBOUND):
        {
            int index = 0;
            bool active = false;
            while (!active && index < m_numInboundSockets)
            {
                // for now lets just detect a mouse over, will later extend to figuring out which one
                active = m_inboundSockets->at(index)->socketOverPoint(_x,_y);
                index++;
            }
            if (active)
            {
                return m_inboundSockets->at(index-1);
            }
            return NULL;
        }break;
        case(SK_OUTBOUND):
        {
            int index = 0;
            bool active = false;
            while (!active && index < m_numOutboundSockets)
            {
                // for now lets just detect a mouse over, will later extend to figuring out which one
                active = m_outboundSockets->at(index)->socketOverPoint(_x,_y);
                index++;
            }
            if (active)
            {
                return m_outboundSockets->at(index-1);
            }
            return NULL;
        }break;
    }
}

QRectF GraphNode::boundingRect() const
{
    return QRectF( m_nodePoint.x() - LINE_EDGE_WIDTH, m_nodePoint.y() - LINE_EDGE_WIDTH,
                 m_width + LINE_EDGE_WIDTH, m_height + LINE_EDGE_WIDTH);
}

QPainterPath GraphNode::shape() const
{
    QPainterPath path;
    path.addRoundRect(m_nodePoint.x(),m_nodePoint.y(),m_width,m_height,25,25);
    return path;
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // this is currently just a copy and paste job and simply needs to be changed for my needs
    // just want to test this works first
    painter->setPen(Qt::NoPen);

    QLinearGradient gradient(0,0,1,1);

    switch(m_valueType)
    {
        case(VT_OBJECT):
        {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::yellow);
        }break;
        case(VT_ARGUMENTS):
        {
            gradient.setColorAt(0, Qt::lightGray);
            gradient.setColorAt(1, Qt::lightGray);
        }break;
        case(VT_MEMBER):
        {
            gradient.setColorAt(0, Qt::cyan);
            gradient.setColorAt(1, Qt::cyan);
        }break;
        case(VT_END):
        {
            gradient.setColorAt(0, Qt::green);
            gradient.setColorAt(1, Qt::green);
        }break;
        default:
        {
            gradient.setColorAt(0, Qt::blue);
            gradient.setColorAt(1, Qt::blue);
        }break;
    }

    painter->setBrush(gradient);

    // switch on if is is selected or not, if it is then draw outline
    // else do not
    if (m_selected)
    {
        painter->setPen(QPen(m_highlightEdgeColour, m_highlightEdgeThickness));
    }
    else
    {
        painter->setPen(QPen(Qt::black, 0));
    }
    painter->drawRoundRect(m_nodePoint.x(),m_nodePoint.y(),m_width,m_height,25,25);
}

void GraphNode::setWidth(qreal _w)
{
    if (_w > 0.0)
    {
        prepareGeometryChange();
        m_width = _w;
        calculateSocketPositions();
    }
    else
    {
#ifdef DEBUG
     std::cout<<"Warning, attempting to set width to 0 or below. Ignoring."<<std::endl;
#endif
    }
    update();
}

void GraphNode::setHeight(qreal _h)
{
    if (_h > 0.0)
    {
        prepareGeometryChange();
        m_height = _h;
        calculateSocketPositions();
    }
    else
    {
#ifdef DEBUG
     std::cout<<"Warning, attempting to set height to 0 or below. Ignoring."<<std::endl;
#endif
    }
    update();
}

void GraphNode::setName(std::string _name)
{
    m_name = _name;
    m_nodeName->setPlainText(m_name.c_str());
    QFontMetrics met(m_nodeName->font());
    int width = met.width(QString(_name.c_str()));
    if (width >= m_baseWidth - 20.0)
    {
        setWidth(width + 25.0f);
    }
    else
    {
        setWidth(m_baseWidth);
    }
    update();
}

void GraphNode::setShortName(std::string _name)
{
    m_shortName = _name;
    m_nodeShortName->setPlainText(m_shortName.c_str());
    update();
}

void GraphNode::setNodeTitle(std::string _title)
{
    if (m_type == NT_ENDNODE)
    {
        m_nodeTypeText->setPlainText(_title.c_str());
    }
}

void GraphNode::setPoint(QPointF _point)
{
     prepareGeometryChange();
     m_nodePoint = _point;
     m_nodeTypeText->setPos(getPoint().x()+15.0,getPoint().y()+10.0);
     m_nodeName->setPos(getPoint().x()+15.0,getPoint().y()+30.0);
     m_nodeShortName->setPos(getPoint().x()+15.0,getPoint().y()+50.0);
     calculateSocketPositions();
}

void GraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent *_event)
{

}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *_event)
{

}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *_event)
{

}

void GraphNode::printNodeInfo()
{
    std::cout<<"\n/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\"<<std::endl;
    std::cout<<"\nNode name: "<<name()<<std::endl;
    std::cout<<"\n****************************************************************************"<<std::endl;
    std::cout<<"\nNumber of inbound sockets: "<<numInboundSockets()<<std::endl;
    for (int i = 0; i < numInboundSockets(); i++)
    {
        m_inboundSockets->at(i)->printSocketInfo();
    }
    std::cout<<"\n****************************************************************************"<<std::endl;
    std::cout<<"Number of outbound sockets: "<<numOutboundSockets()<<std::endl;
    for (int i = 0; i < numOutboundSockets(); i++)
    {
        m_outboundSockets->at(i)->printSocketInfo();
    }
    std::cout<<"\n/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\"<<std::endl;
}

std::string GraphNode::getNodeInfo()
{
    std::string returnString = "";
    returnString += (std::string(GenUtils::valueTypeToString(valueType())+";"));
    returnString += (std::string(GenUtils::nodeTypeToString(nodeType())+";"));
    returnString += (std::string(name()+";"));
    returnString += (std::string(shortName()+";"));
    returnString += "--;";
    return returnString;
}

bool GraphNode::gatherInboundSocketInfo(std::vector<std::string> *_vec)
{
    std::vector<std::string> store;
    store.clear();
    std::vector<std::string> temp;
    temp.clear();
    for (int  i =0; i < m_numInboundSockets; i++)
    {
        if(!m_inboundSockets->at(i)->getConnectedNodeDetails(&temp))
        {
            return false;
        }

        for (int j =0; j < int(temp.size()); j++)
        {
            store.push_back(temp.at(j));
        }
        temp.clear();
    }
    for (int k = 0; k < int(store.size()); k++)
    {
        _vec->push_back(store.at(k));
    }
    return true;
}

QPointF GraphNode::centre()
{
    QRectF box = boundingRect();
    return box.center();
}

void GraphNode::setBaseWidth(qreal _width)
{
    m_baseWidth = _width;
}

void GraphNode::setBaseHeight(qreal _height)
{
    m_baseHeight = _height;
}

void GraphNode::addInboundSocket()
{
    if (m_socketClearence == IN_OUT || m_socketClearence == IN)
    {
        NodeSocket *temp = new NodeSocket(SK_INBOUND,0.0,0.0);
        temp->init();
        temp->setParentNode(this);
        temp->setParentScene(m_parentScene);
        temp->setColour(0,255,0);
        // will need to add the socket to the scene here
        m_parentScene->addSocketToScene(temp);
        m_inboundSockets->push_back(temp);
        m_numInboundSockets++;
        calculateInboundSocketsPositions();
    }
}

void GraphNode::addOutboundSocket()
{
    if (m_socketClearence == IN_OUT || m_socketClearence == OUT)
    {
        NodeSocket *temp = new NodeSocket(SK_OUTBOUND,0.0,0.0);
        temp->init();
        temp->setParentNode(this);
        temp->setParentScene(m_parentScene);
        temp->setColour(255,0,0);
        // will need to add the socket to the scene here
        m_parentScene->addSocketToScene(temp);
        m_outboundSockets->push_back(temp);
        m_numOutboundSockets++;
        calculateOutboundSocketsPositions();
    }
}

void GraphNode::calculateSocketPositions()
{
    calculateInboundSocketsPositions();
    calculateOutboundSocketsPositions();
}

void GraphNode::calculateInboundSocketsPositions()
{
    // calculate the positions for any inbound sockets
    qreal XDelta = m_width/qreal(m_numInboundSockets+1);
    qreal YPos = m_nodePoint.y();

    for (int i = 0; i < m_numInboundSockets; i++)
    {
        m_inboundSockets->at(i)->setPosition(m_nodePoint.x()-(m_inboundSockets->at(i)->width()*0.5) + qreal(i+1)*XDelta,YPos-m_inboundSockets->at(i)->height());
    }
}

void GraphNode::calculateOutboundSocketsPositions()
{
    // calculate for any outbound sockets
    qreal XDelta = m_width/qreal(m_numOutboundSockets+1);
    qreal YPos = m_nodePoint.y()+m_height;

    for (int i = 0; i < m_numOutboundSockets; i++)
    {
        m_outboundSockets->at(i)->setPosition(m_nodePoint.x()-(m_outboundSockets->at(i)->width()*0.5) + qreal(i+1)*XDelta,YPos);
    }
}

void GraphNode::init(VALUE_TYPE _vType, NODE_TYPE _type, QPointF _point)
{
    m_name = "";
    m_shortName = "";

    m_deletable = true;
    m_editable = true;
    m_endNode = false;

    // set some flags to allow the node to be interactable
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    m_type = _type;
    m_valueType = _vType;
    m_nodePoint = _point;

    m_zDepth = -1; // this means it has been defaulted and is not on top of another node
    m_move = false;

    m_socketClearence = IN_OUT;

    m_nodeTypeText = new QGraphicsTextItem(this);
    m_nodeName = new QGraphicsTextItem(this);
    m_nodeShortName = new QGraphicsTextItem(this);
    std::string title = GenUtils::nodeTypeToString(m_type);
    if (title == "__end_node__")
    {
        title = "";
    }
    m_nodeTypeText->setPlainText(title.c_str());
    QFont titleFont;
    titleFont.setBold(true);
    m_nodeTypeText->setFont(titleFont);
    m_nodeTypeText->setPos(getPoint().x()+15.0,getPoint().y()+10.0);
    m_nodeName->setPlainText(m_name.c_str());
    m_nodeName->setPos(getPoint().x()+15.0,getPoint().y()+30.0);
    m_nodeShortName->setPlainText(m_shortName.c_str());
    m_nodeShortName->setPos(getPoint().x()+15.0,getPoint().y()+50.0);

    m_numInboundSockets = 0;
    m_numOutboundSockets = 0;

    m_highlightEdgeColour = QColor(255,165,0);
    m_highlightEdgeThickness = 5;
    m_selected = false;

    m_width = 160.0;
    m_baseWidth = 160.0;
    m_height = 80.0;
    m_baseHeight = 80.0;

    m_offsetToCursor = QPointF(0.0,0.0);

    m_inboundSockets = new std::vector<NodeSocket*>;
    m_inboundSockets->clear();
    m_outboundSockets = new std::vector<NodeSocket*>;
    m_outboundSockets->clear();

    // set no sockets then test what type of sockets this node will accept
    m_numInboundSockets = 0;
    m_numOutboundSockets = 0;
}

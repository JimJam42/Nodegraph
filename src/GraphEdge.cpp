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

#include "GraphEdge.h"

#include <QPainter>

#include <iostream>

#include <math.h>

#define DEFAULT_ARROW_SIZE 7.5

#define PI 3.14159265359
#define TWO_PI 6.28318530718

GraphEdge::GraphEdge()
{
    m_sourceNode = NULL;
    m_sourceSocket = NULL;
    m_destinationNode = NULL;
    m_destinationSocket = NULL;

#ifdef DEBUG
    std::cout<<"Warning, creating edge without passing a source and destination. This will need to be done manually!"<<std::endl;
#endif

    init();
}

GraphEdge::GraphEdge(NodeSocket *_source, NodeSocket *_destination)
{
    m_sourceNode = _source->getParentNode();
    m_sourceSocket = _source;
    m_destinationNode = _destination->getParentNode();
    m_destinationSocket = _destination;

    updateEdge();
    init();
}

GraphEdge::~GraphEdge()
{
    // none of the pointers are deleted in here as they should be deleted elsewhere
    // for example the ndoes should be deleted from the scene
    // sockets should be deleted from the nodes along with the edges
}

void GraphEdge::init()
{
    m_arrowSize = DEFAULT_ARROW_SIZE;
}

void GraphEdge::updateEdge() // update the line start and end positions if the nodes have now moved
{
    // first check for validity on the end and start nodes
    if (!m_sourceSocket || !m_destinationSocket) return;

    prepareGeometryChange();

    //    // for now naiively just set source point to start point and destination point to end point
    // a source node will come out fo the bottom of the node for now

    m_sourcePoint = m_sourceSocket->centre();
    m_destinationPoint = m_destinationSocket->centre();
}

void GraphEdge::setSourceDestinationSockets(NodeSocket *_source, NodeSocket *_destination)
{
    setSourceSocket(_source);

    setDestinationSocket(_destination);
}

void GraphEdge::setSourceSocket(NodeSocket *_source)
{
    if (_source)
    {
        m_sourceNode = _source->getParentNode();
        m_sourceSocket = _source;
        updateEdge();
    }
    else
    {
#ifdef DEBUG
        std::cerr<<"Passing a null pointer to set as source socket for edge"<<std::endl;
#endif
    }
}

void GraphEdge::setDestinationSocket(NodeSocket *_destination)
{
    if (_destination)
    {
        m_destinationNode = _destination->getParentNode();
        m_destinationSocket = _destination;
        updateEdge();
    }
    else
    {
#ifdef DEBUG
        std::cerr<<"Passing a null pointer to set as destination socket for edge"<<std::endl;
#endif
    }
}

void GraphEdge::setArrowSize(float _size)
{
    if (_size >= 0.0f)
    {
        m_arrowSize = _size;
    }
    else
    {
#ifdef DEBUG
      std::cout<<"Attempted to set the arrow size to a negative number. Defaulting size"<<std::endl;
#endif
        m_arrowSize = DEFAULT_ARROW_SIZE;
    }
}

QRectF GraphEdge::boundingRect() const
{
    if (!m_sourceNode || !m_destinationNode)
    {
        // one of the nodes is not valid so must just return a dummy rectangle
        return QRectF();
    }

    // structure of this taken from edge example found in QT Docs
    // http://qt-project.org/doc/qt-4.8/graphicsview-elasticnodes.html
    float compensate = m_arrowSize / 2.0f;
    return QRectF(m_sourcePoint,QSizeF(m_destinationPoint.x() - m_sourcePoint.x(),m_destinationPoint.y() - m_sourcePoint.y())).normalized()
            .adjusted(-compensate,-compensate,compensate,compensate);
}

void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!m_sourceSocket || !m_destinationSocket)
    {
        // one of the nodes is not valid so must just return
        return;
    }

    QLineF line(m_sourcePoint, m_destinationPoint);

    // check that the line length isnt 0
    // if it is then simply dont draw it by returning
    if (line.length() <= 0.0f) return;

    // draw the line
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // this is now where the drawing of the arrows must take place
    // will implement this at a later date, for now lets just get stuff drawing
    double angle = acos(line.dx() / line.length());
    if (line.dy() >= 0)
    {
        angle = TWO_PI - angle;
    }

    QPointF midPoint = (m_destinationPoint+m_sourcePoint)*0.5;

    QPointF midArrowP1 = midPoint + QPointF(sin(angle - PI / 3) * m_arrowSize,
                                               cos(angle - PI / 3) * m_arrowSize);
    QPointF midArrowP2 = midPoint + QPointF(sin(angle - PI + PI / 3) * m_arrowSize,
                                               cos(angle - PI + PI / 3) * m_arrowSize);
    QPointF destArrowP1 = m_destinationPoint + QPointF(sin(angle - PI / 3) * m_arrowSize,
                                           cos(angle - PI / 3) * m_arrowSize);
    QPointF destArrowP2 = m_destinationPoint + QPointF(sin(angle - PI + PI / 3) * m_arrowSize,
                                           cos(angle - PI + PI / 3) * m_arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << midPoint << midArrowP1 << midArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

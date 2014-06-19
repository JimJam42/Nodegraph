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

#ifndef __CONNECTINGEDGE_H__
#define __CONNECTINGEDGE_H__

#include <QGraphicsItem>
#include "GraphNode.h"

/// @file GraphEdge.h
/// @brief Edge class to control the drawing and positons of an edge in the graph
/// @author Callum James
/// @version 1.0
/// @date 24/03/2014
/// Revision History:
/// Initial Version 12/03/2014
/// @class GraphEdge
/// @brief A class to control the state, drawing and position of any edge within
/// the graph that connects between two nodes. This also keeps track of sockets connected to the edge.

class GraphEdge : public QGraphicsItem
{
public:
    /// @brief Default ctr
    GraphEdge();
    /// @brief alternative ctr
    /// @param [in] _source NodeSocket* - the source socket
    /// @param [in] _destination NodeSocket* - the destination socket
    GraphEdge(NodeSocket *_source, NodeSocket *_destination);

    /// @brief default dtr
    ~GraphEdge();

    /// @brief Init function to initialise this edge
    void init();
    /// @brief Update this edge such as start and end point
    void updateEdge();
    /// @brief Set the source and destination sockets for the edge
    /// @param [in] _source NodeSocket* - the source socket
    /// @param [in] _destination NodeSocket* - the destination socket
    void setSourceDestinationSockets(NodeSocket *_source, NodeSocket *_destination);
    /// @brief Set the source socket of the edge
    /// @param [in] _source NodeSocket* - the socket to set as the source
    void setSourceSocket(NodeSocket *_source);
    /// @brief Set the destination socket of the edge
    /// @param [in] _destination NodeSocket* - the socket to set as the destination
    void setDestinationSocket(NodeSocket *_destination);
    /// @brief Set the size of the arrow on the edge
    /// @param [in] _size float - the size of the arrow
    void setArrowSize(float _size);
    /// @brief Get pointer to the source node
    /// @returns GraphNode*
    GraphNode *sourceNode() {return m_sourceNode;}
    /// @brief Get pointer to the source socket
    /// @returns NodeSocket*
    NodeSocket *sourceSocket() {return m_sourceSocket;}
    /// @brief Get pointer to the destination node
    /// @returns GraphNode*
    GraphNode *destinationNode() {return m_destinationNode;}
    /// @brief Get pointer to the destination socket
    /// @returns NodeSocket*
    NodeSocket *destinationSocket() {return m_destinationSocket;}
    /// @brief Get the arrow size
    /// @returns float
    float arrowSize() {return m_arrowSize;}

protected:
    /// @brief Function to get the bounding rectangle of the object
    /// @returns QRectF
    QRectF boundingRect() const;
    /// @brief Paint function that is overridden from the virtual
    /// @param [in] painter QPainter* - the painter object to use
    /// @param [in] option QStyleOptionGraphicsItem* - options for the drawing style
    /// @param [in] widget QWidget* - the widget to draw
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    /// @brief The source node of the edge
    GraphNode *m_sourceNode;
    /// @brief The destination node of the edge
    GraphNode *m_destinationNode;
    /// @brief The source socket of the edge
    NodeSocket *m_sourceSocket;
    /// @brief The destination socket of the edge
    NodeSocket *m_destinationSocket;

    /// @brief The source point of the edge
    QPointF m_sourcePoint; // these are both stored within the ndoes but as item space
    /// @brief The destination point of the edge
    QPointF m_destinationPoint; // these two attributes are the conversion from this
    /// @brief The size of the arrow to draw
    float m_arrowSize; // will be defaulted to 10
};

#endif /* __CONNECTINGEDGE_H__ */

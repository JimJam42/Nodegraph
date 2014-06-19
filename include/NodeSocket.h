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

#ifndef __NODESOCKET_H__
#define __NODESOCKET_H__

#include <QGraphicsItem>

/// @file NodeSocket.h
/// @brief A class for a single socket, this class knows which sockets and edges are connected
/// @author Callum James
/// @version 1.0
/// @date 24/03/2014
/// Revision History:
/// Initial Version 12/03/2014
/// @class NodeSocket
/// @brief A class for a single socket, this class knows which sockets and edges are connected
/// It is also able to access any connected nodes to this socket

/// @enum SOCKET_TYPE
/// @brief Whether it is inbound or outbound
enum SOCKET_TYPE
{
    SK_INBOUND,
    SK_OUTBOUND
};

class GraphNode;
class GraphEdge;
class GraphScene;

class NodeSocket : public QGraphicsItem
{
public:
    /// @brief ctr
    /// @param [in] _type SOCKET_TYPE - the type of socket to create
    NodeSocket(SOCKET_TYPE _type);
    /// @brief ctr
    /// @param [in] _type SOCKET_TYPE - the type of socket to create
    /// @param _x qreal - x position to create at
    /// @param _y qreal - y position to create at
    /// @param _w float - width of node
    /// @param _h float - height of node
    NodeSocket( SOCKET_TYPE _type, qreal _x, qreal _y, float _w = 10.0f, float _h = 10.0f);
    /// @brief ctr
    /// @param [in] _type SOCKET_TYPE - the type of socket to create
    /// @param _point QPointF - the point to create the node socket at
    /// @param _w float - width of node
    /// @param _h float - height of node
    NodeSocket(SOCKET_TYPE _type, QPointF _point, float _w = 10.0f, float _h = 10.0f);
    /// @brief dtr
    ~NodeSocket();
    /// @brief initialise the class
    void init();
    /// @brief Add an edge to the socket
    /// @param [in] _dest NodeSocket* - the destination socket
    void addEdge(NodeSocket *_dest);
    /// @brief Add an edge reference without creating a new edge
    /// @param [in] _edge GraphEdge* - the edge to reference
    void addEdgeReference(GraphEdge *_edge);
    /// @brief Update all connected edges
    void updateEdges();
    /// @brief Remove an edge
    /// @param [in] _edge GraphEdge* - the edge to remove
    /// @returns bool
    bool removeEdge(GraphEdge *_edge);
    /// @brief Remove an edge reference
    /// @param [in] _edge GraphEdge* - the reference to remove
    /// @returns bool
    bool removeEdgeReference(GraphEdge *_edge);
    /// @brief remove all edges
    void removeAllEdges();

    /// @brief Test if a socket is over a point
    /// @param _point QPointF - the point to test
    /// @returns bool
    bool socketOverPoint(QPointF _point);
    /// @brief Test if a socket is over a point
    /// @param [in] _x qreal - the x point to test
    /// @param [in] _y qreal - the y point to test
    /// @returns bool
    bool socketOverPoint(qreal _x, qreal _y);
    /// @brief Set position of the socket
    /// @param [in] _position QPointF - the position to set to
    void setPosition(QPointF _position);
    /// @brief Set position of the socket
    /// @param [in] _x qreal - the x position to set to
    /// @param [in] _y qreal - the y position to set to
    void setPosition(qreal _x, qreal _y);
    /// @brief Set the colour of the socket
    /// @param [in] _colour QColor - the colour to set to
    void setColour(QColor _colour);
    /// @brief Set the colour of the socket
    /// @param [in] _r int - the red channel
    /// @param [in] _g int - the green channel
    /// @param [in] _b int - the blue channel
    /// @param [in] _a int - the alpha channel
    void setColour(int _r, int _g, int _b, int _a=255);
    /// @brief Set the width of the socket
    /// @param [in] _w float - width
    void setWidth(float _w);
    /// @brief Set the height of the socket
    /// @param [in] _h float - height
    void setHeight(float _h);
    /// @brief Set the parent node
    /// @param [in] _parent GraphNode* - the parent node
    void setParentNode(GraphNode *_parent);
    /// @brief Set the parent scene
    /// @param [in] _scene GraphScene* - the parent scene
    void setParentScene(GraphScene *_scene);
    /// @brief get the position of the socket
    /// @returns QPointF
    QPointF position() {return m_position;}
    /// @brief get the centre of the socket
    /// @returns QPointF
    QPointF centre();
    /// @brief Get the colour of the socket
    /// @returns QColor
    QColor colour() {return m_colour;}
    /// @brief Get the width of the socket
    /// @returns float
    float width() {return m_width;}
    /// @brief Get the height of the socket
    /// @returns float
    float height() {return m_height;}
    /// @brief Get the bounding rectangle of the socket
    /// @returns QRectF
    QRectF boundingRect() const;
    /// @brief Get the shape of the object
    /// @returns QPainterPath
    QPainterPath shape() const;
    /// @brief Paint function that is overridden from the virtual
    /// @param [in] painter QPainter* - the painter object to use
    /// @param [in] option QStyleOptionGraphicsItem* - options for the drawing style
    /// @param [in] widget QWidget* - the widget to draw
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /// @brief Get the parent node
    /// @returns GraphNode*
    GraphNode *getParentNode() {return m_parentNode;}
    /// @brief Get the parent scene
    /// @returns GraphScene*
    GraphScene *getParentScene() {return m_parentScene;}
    /// @brief Print the socket information to console
    void printSocketInfo();
    /// @brief Get all connected node details
    /// @param [out] _vec std::vector<std::string>* - the vector to write information to
    /// @returns bool
    bool getConnectedNodeDetails(std::vector<std::string> *_vec);

private:
    /// @brief The parent node
    GraphNode *m_parentNode;
    /// @brief The parent scene
    GraphScene *m_parentScene;
    /// @brief All edges connected to this socket
    std::vector<GraphEdge*> *m_edges;
    /// @brief Number of edges connected to the socket
    int m_numEdges;
    /// @brief The type of socket
    SOCKET_TYPE m_socketType;
    /// @brief Position of the socket
    QPointF m_position;
    /// @brief Colour of the socket
    QColor m_colour;
    /// @brief Width of the socket
    float m_width;
    /// @brief Height of the socket
    float m_height;
    /// @brief Find the index of an edge on the socket
    /// @param [in] _edge GraphEdge* - edge to find
    /// @param [out] _index int* - index to write to
    /// @returns bool
    bool findEdgeIndex(GraphEdge *_edge, int *_index);

};

#endif /* __NODESOCKET_H__ */

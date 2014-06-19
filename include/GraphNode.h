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

#ifndef __GRAPHNODE_H__
#define __GRAPHNODE_H__

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsTextItem>

#include "NodeSocket.h"

/// @file GraphNode.h
/// @brief Node class to control and manage a single node
/// @author Callum James
/// @version 1.0
/// @date 24/03/2014
/// Revision History:
/// Initial Version 12/03/2014
/// @class GraphNode
/// @brief Class to handle a single node. It handles the type and state of the node as well as
/// any connected sockets and allows access to these sockets for finding connected edges and nodes

class GraphEdge;
class GraphScene;

/// @enum SOCKET_CLEARENCE
/// @brief identifier of whether the node allows in, out or both connections
enum SOCKET_CLEARENCE
{
    IN_OUT = 0,
    IN,
    OUT,
    NONE
};

/// @enum VALUE_TYPE
/// @brief idenfitifer of what type of node it is on a top level
enum VALUE_TYPE
{
    VT_NOTYPE = 0,
    VT_OBJECT,
    VT_ARGUMENTS,
    VT_MEMBER,
    VT_END
};

/// @enum NODE_TYPE
/// @brief idenfitifer of what type of node it is on a bottom level
enum NODE_TYPE
{ // keeping it to just 3 types at the moment for testing and development
    // more can then be added later
    NT_NOTYPE = 0,
    NT_STRING,
    NT_INT,
    NT_FLOAT,
    NT_DOUBLE,
    NT_BOOLEAN,
    NT_CHAR,
    NT_VECTOR,
    NT_MATRIX,
    NT_OBJ_ANGLE,
    NT_OBJ_DISTANCE,
    NT_OBJ_TIME,
    NT_OBJ_LAST,
    NT_OBJ_COMPOUND,
    NT_OBJ_ENUM,
    NT_OBJ_GENERIC,
    NT_OBJ_MATRIX,
    NT_OBJ_MESSAGE,
    NT_OBJ_BOOLEAN,
    NT_OBJ_ONEBYTE,
    NT_OBJ_ONECHAR,
    NT_OBJ_ONESHORT,
    NT_OBJ_TWOSHORT,
    NT_OBJ_THREESHORT,
    NT_OBJ_ONELONG,
    NT_OBJ_ONEINT,
    NT_OBJ_TWOLONG,
    NT_OBJ_TWOINT,
    NT_OBJ_THREELONG,
    NT_OBJ_THREEINT,
    NT_OBJ_ONEFLOAT,
    NT_OBJ_TWOFLOAT,
    NT_OBJ_THREEFLOAT,
    NT_OBJ_ONEDOUBLE,
    NT_OBJ_TWODOUBLE,
    NT_OBJ_THREEDOUBLE,
    NT_OBJ_FOURDOUBLE,
    NT_OBJ_ADDRESS,
    NT_OBJ_NUM_LAST,
    NT_ENDNODE
};

class GraphNode : public QGraphicsItem
{
public:
    /// @brief Ctr for the GraphNode
    /// @param [in] _vType VALUE_TYPE - the top level type of the node
    /// @param [in] _type NODE_TYPE - the bottom level type of the node
    /// @param [in] _parent QWidget* - the parent widget of the node
    GraphNode(VALUE_TYPE _vType=VT_NOTYPE, NODE_TYPE _type=NT_NOTYPE, QGraphicsItem *_parent=0);
    /// @brief Ctr for the GraphNode
    /// @param [in] _point QPointF - the point to position the node at
    /// @param [in] _vType VALUE_TYPE - the top level type of the node
    /// @param [in] _type NODE_TYPE - the bottom level type of the node
    /// @param [in] _parent QWidget* - the parent widget of the node
    GraphNode(QPointF _point=QPointF(0.0,0.0), VALUE_TYPE _vType=VT_NOTYPE, NODE_TYPE _type=NT_NOTYPE, QGraphicsItem *_parent=0);
    /// @brief Ctr for the GraphNode
    /// @param [in] _x qreal - the x position to position the node at
    /// @param [in] _y qreal - the y position to position the node at
    /// @param [in] _vType VALUE_TYPE - the top level type of the node
    /// @param [in] _type NODE_TYPE - the bottom level type of the node
    /// @param [in] _parent QWidget* - the parent widget of the node
    GraphNode(qreal _x, qreal _y, VALUE_TYPE _vType=VT_NOTYPE, NODE_TYPE _type=NT_NOTYPE, QGraphicsItem *_parent=0);
    /// @brief dtr
    ~GraphNode();
    /// @brief Set the node as selected
    /// @param [in] _selected bool - whether to set it as selected or not
    void setSelectedNode(bool _selected) {prepareGeometryChange(); m_selected = _selected;}
    /// @brief Update all sockets on the node
    void updateSockets();
    /// @brief Set the parent scene of the node
    /// @param [in] _scene GraphScene* - the scene to set as parent
    void setParentScene(GraphScene *_scene);
    /// @brief Add a socket to the node
    /// @param [in] _type SOCKET_TYPE - the type of socket to add
    void addSocket(SOCKET_TYPE _type);
    /// @brief Get the name of the node
    /// @returns std::string
    std::string name() {return m_name;}
    /// @brief Get the short name of the node
    /// @returns std::string
    std::string shortName() {return m_shortName;}

    /// @brief Get the bottom level type of the node
    /// @returns NODE_TYPE
    NODE_TYPE nodeType() {return m_type;}
    /// @brief Get the top level type of the node
    /// @returns VALUE_TYPE
    VALUE_TYPE valueType() {return m_valueType;}

    /// @brief Returns if a node sits over a point
    /// @param [in] _point QPointF - the point to test against
    bool nodeOverPoint(QPointF _point);
    /// @brief Returns if a node sits over a point
    /// @param [in] _x qreal - the x point to test against
    /// @param [in] _y qreal - the y point to test against
    bool nodeOverPoint(qreal _x, qreal _y);

    /// @brief Function to get any node socket at a given point
    /// @param [in] _point QPointF - the point to test against
    /// @param [in] _type SOCKET_TYPE - socket type to find
    /// @returns NodeSocket*
    NodeSocket *cursorOverSocket(QPointF _point, SOCKET_TYPE _type);
    /// @brief Function to get any node socket at a given point
    /// @param [in] _x qreal - the x point to test against
    /// @param [in] _y qreal - the y point to test against
    /// @param [in] _type SOCKET_TYPE - socket type to find
    /// @returns NodeSocket*
    NodeSocket *cursorOverSocket(qreal _x, qreal _y, SOCKET_TYPE _type);
    /// @brief Get the bounding rectangle of the object
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
    /// @brief Function to set the width of the node
    /// @param [in] _w qreal - width to set to
    void setWidth(qreal _w);
    /// @brief Function to set the height of the node
    /// @param [in] _h qreal - height to set to
    void setHeight(qreal _h);
    /// @brief Set the name of the node
    /// @param [in] _name std::string - the name to set
    void setName(std::string _name);
    /// @brief Set the short name of the node
    /// @param [in] _name std::string - the short name to set
    void setShortName(std::string _name);

    /// @brief Set the node to be deletable
    /// @param [in] _del bool - whether deletable or not
    void setDeletable(bool _del) {m_deletable = _del;}
    /// @brief Set the node to be editable
    /// @param [in] _edi bool - whether editable or not
    void setEditable(bool _edi) {m_editable = _edi;}
    /// @brief Set the node to be an end node
    /// @param [in] _end bool - whether end node or not
    void setEndNode(bool _end) {m_endNode = _end;}
    /// @brief Set the title of the node
    /// @param [in] _title std::string - the title of the node
    void setNodeTitle(std::string _title);// this will only be possible if the type is EndNode
    /// @brief Set the clearence of the socket
    /// @param [in] _clearence SOCKET_CLEARENCE - whether the node can accept in, out or both types
    void setSocketClearence(SOCKET_CLEARENCE _clearence) {m_socketClearence = _clearence;}
    /// @brief Get the z depth of the node in the scene - currently not affecting results
    /// @returns int
    int zDepth() {return m_zDepth;}
    /// @brief Get width of node
    /// @returns qreal
    qreal getWidth() {return m_width;}
    /// @brief Get height of node
    /// @returns qreal
    qreal getHeight() {return m_height;}
    /// @brief Set point of the node
    /// @param [in] _point QPointF - the point to set the node to
    void setPoint(QPointF _point);
    /// @brief Get the point that the node is located at
    /// @returns QPointF
    QPointF getPoint() {return m_nodePoint;}
    /// @brief Get the offset within the node to the mouse cursor
    /// @returns QPointF
    QPointF getOffsetToCursor() {return m_offsetToCursor;}
    /// @brief Get the clearence of the node
    /// @returns SOCKET_CLEARENCE
    SOCKET_CLEARENCE getSocketClearence() {return m_socketClearence;}
    /// @brief Get if the node is selected or not
    /// @returns bool
    bool getIfSelected() {return m_selected;}
    /// @brief Get the parent scene of the node
    /// @returns GraphScene*
    GraphScene* parentScene() {return m_parentScene;}

    /// @brief the event triggered when the mouse is moved
    /// @param [in] _event QGraphicsSceneMouseEvent* - the event triggered
    void mouseMoveEvent(QGraphicsSceneMouseEvent *_event);
    /// @brief the event triggered when the mouse is clicked
    /// @param [in] _event QGraphicsSceneMouseEvent* - the event triggered
    void mousePressEvent(QGraphicsSceneMouseEvent *_event);
    /// @brief the event triggered when the mouse is released
    /// @param [in] _event QGraphicsSceneMouseEvent* - the event triggered
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *_event);

    /// @brief Get number of outbound sockets on the node
    /// @returns int
    int numOutboundSockets() {return m_numOutboundSockets;}
    /// @brief Get number of inbound sockets on the node
    /// @returns int
    int numInboundSockets() {return m_numInboundSockets;}

    /// @brief Returns whether the node is deletable or not
    /// @returns bool
    bool deletable() {return m_deletable;}
    /// @brief Returns whether the node is editable or not
    /// @returns bool
    bool editable() {return m_editable;}
    /// @brief Returns whether the node is the end node or not
    /// @returns bool
    bool endNode() {return m_endNode;}

    /// @brief Simple function to print to console the node information
    void printNodeInfo();
    /// @brief Get the node information as a string
    /// @returns std::string
    std::string getNodeInfo();
    /// @brief get the all inbound socket information - store to passed in vec
    /// @param [in] _vec std::vector<std::string>* - vector to write result to
    /// @returns bool
    bool gatherInboundSocketInfo(std::vector<std::string> *_vec);
    /// @brief Get the centre of the node
    /// @returns QPointF
    QPointF centre();
    /// @brief set the base width, the width the node will return to
    /// @param [in] _width qreal - the width to set the base width to
    void setBaseWidth(qreal _width);
    /// @brief set the base height, the height the node will return to
    /// @param [in] _height qreal - the height to set the base height to
    void setBaseHeight(qreal _height);
    /// @brief get the base width
    /// @returns qreal
    qreal getBaseWidth() {return m_baseWidth;}
    /// @brief get the base height
    /// @returns qreal
    qreal getBaseHeight() {return m_baseHeight;}

private:
    /// @brief Clearence of the node
    SOCKET_CLEARENCE m_socketClearence;
    /// @brief Name of the node
    std::string m_name;
    /// @brief Short name of the node
    std::string m_shortName;
    /// @brief the base width of the node
    qreal m_baseWidth;
    /// @brief Width of the node
    qreal m_width;
    /// @brief the base height of the node
    qreal m_baseHeight;
    /// @brief Height of the node
    qreal m_height;
    /// @brief Highlight colour for the node
    QColor m_highlightEdgeColour;
    /// @brief thickness of the highlight edge
    int m_highlightEdgeThickness;
    /// @brief If the ndoe is selected or not
    bool m_selected;
    /// @brief Item to hold the node type name
    QGraphicsTextItem *m_nodeTypeText;
    /// @brief Item to hold the node name
    QGraphicsTextItem *m_nodeName;
    /// @brief Item to hold the node short name
    QGraphicsTextItem *m_nodeShortName;
    /// @brief The parent scene of the node
    GraphScene *m_parentScene; // the scene within which the node exists - used for adding of edges

    /// @brief Vector of outbound sockets on the node
    std::vector<NodeSocket*> *m_outboundSockets;
    /// @brief Vector of inbound sockets on the node
    std::vector<NodeSocket*> *m_inboundSockets;
    /// @brief Number of outbound sockets
    int m_numOutboundSockets;
    /// @brief Number of inbound sockets
    int m_numInboundSockets;
    /// @brief The poit the node is located at
    QPointF m_nodePoint;
    /// @brief Offset of the ndoe poiint to the mouse cursor
    QPointF m_offsetToCursor;// used to determine the offset to where the user has clicked
    /// @brief Z depth of the node in the scene
    int m_zDepth; // used when two ndoes overlap each other, this value will be assigned
    // when it is not equal to -1, it means there are some nodes overlapping and each node will have its own value
    // when this occurs, when selecting the node, the node at that point with the highest zDepth will be selected
    /// @brief Wether the node is being moved or not
    bool m_move;
    /// @brief If the node is deletable or not
    bool m_deletable;
    /// @brief If the node is editable or not
    bool m_editable;
    /// @brief If the node is an end node or not
    bool m_endNode;
    /// @brief Top level type of the node
    VALUE_TYPE m_valueType;
    /// @brief Botoom level type of the node
    NODE_TYPE m_type;
    /// @brief Function to add an inbound socket
    void addInboundSocket();
    /// @brief Function to add an outbound socket
    void addOutboundSocket();
    /// @brief Function to calculate all socket positions
    void calculateSocketPositions();
    /// @brief Function to calculate inbound socket positions
    void calculateInboundSocketsPositions();
    /// @brief Function to calculate outbound socket positions
    void calculateOutboundSocketsPositions();
    /// @brief Function to initialise the node
    /// @param [in] _vType VALUE_TYPE - top level type for the node
    /// @param [in] _type NODE_TYPE - bottom level type for the node
    /// @param [in] _point QPointF - point to create the node at
    void init(VALUE_TYPE _vType, NODE_TYPE _type=NT_NOTYPE, QPointF _point=QPointF(0.0,0.0));
};

#endif /* __GRAPHNODE_H__ */

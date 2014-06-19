NodeGraph Library

Introduction

Welcome to the Nodegraph library. This custom library developed for the 
MayaAPIWizard allows you to define objects and command line arguments by
 creating and joining nodes within the Wizard itself. When the nodegraph is 
asked to return all data, it reads from the single end node, up to all 
connected nodes. It then returns as a single string each node, its argument 
and its type, long and short name. Each node is delimited by a _# and each 
attribute of a node delimited by a ;. All node type declarations will be 
delimited by a -- so it is easy to see when a new node starts.

Building the Nodegraph

To build the nodegraph, you will only need the Qt library. The required Qt 
version if 5 or higher. Thats it. In the root directory, run qmake (assuming 
qmake has been setup in your path correctly) and then run make.

Usage

Usage of the Nodegrpah library is simple. When included into the application, 
the nodegraph is a widget that can be added to any container within the UI. 
Once added, an end node will need to be added. This is the target node for all 
other nodes. When using the graph within the application:

Zoom in and out with the mouse wheel
	-Right click and drag a node to move it around
	-Left click a red outbound socket and drag to a green inbound socket to 
	 join the nodes
	-Press space and select the node you want to add a new node
	-Select a node with left click and press space or delete to remove a node 
	 and all connections to that node
	-double click a node to enter node edit mode to change its name and short 
	 name. Press enter to save the edit

Current Limitations

The nodegraph, in its current iteration, lacks some functionality that needs to
be added. Currently for example chained nodes have no effect. The nodegraph is
still being developed to rectify such limitations.

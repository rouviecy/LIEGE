#ifndef NODE_H
#define NODE_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Edge;

/// <summary>Node of the graph</summary>
class Node{

public:

	/// <summary>Default constructor (unnamed and free node)</summary>
	Node();

	/// <summary>Constructor (free node)</summary>
	/// <param name="name">Label of the node</param>
	Node(std::string name);

	/// <summary>Constructor (fixed node)</summary>
	/// <param name="name">Label of the node</param>
	/// <param name="x">X-position</param>
	/// <param name="y">Y-position</param>
	Node(std::string name, float x, float y);

	/// <summary>Destructor : call destruction of incoming and outgoing edges</summary>
	/// <returns>List of created edges</returns>
	~Node();

	/// <summary>Create an edge between this node and another node (the edge will be unnamed and without reverse edge)</summary>
	/// <param name="neighbourNode">The other node to link</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectToNode(Node* neighbourNode);

	/// <summary>Create an edge between this node and another node (the edge will be unnamed)</summary>
	/// <param name="neighbourNode">The other node to link</param>
	/// <param name="isBidirectional">If another reverse edge has to be created</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectToNode(Node* neighbourNode, bool isBidirectional);

	/// <summary>Create an edge between this node and another node</summary>
	/// <param name="neighbourNode">The other node to link</param>
	/// <param name="isBidirectional">If another reverse edge has to be created</param>
	/// <param name="edgeName">Label of the new edge</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectToNode(Node* neighbourNode, bool isBidirectional, std::string edgeName);

	/// <summary>Create an edge between this node and another node (the edge will be without reverse)</summary>
	/// <param name="neighbourNode">The other node to link</param>
	/// <param name="edgeName">Label of the new edge</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectToNode(Node* neighbourNode, std::string edgeName);

	/// <summary>Delete all edges between this node and another node</summary>
	/// <param name="notNeighbourAnymore">The other node to unlink</param>
	void DeleteEdgesToNode(Node* notNeighbourAnymore);

	/// <summary>Delete an edge of this node (reverse edges will be kept)</summary>
	/// <param name="notEdgeAnymore">The edge to remove</param>
	void DeleteEdge(Edge* notEdgeAnymore);

	/// <summary>Delete an edge of this node</summary>
	/// <param name="notEdgeAnymore">The edge to remove</param>
	/// <param name="removeAllReverses">If reverse edges to this node has to be deleted</param>
	void DeleteEdge(Edge* notEdgeAnymore, bool removeAllReverses);

	/// <summary>To string (edges will be only light-described)</summary>
	/// <returns>Text</returns>
	std::string ToString();

	/// <summary>To string</summary>
	/// <param name="printEdges">If edges has to be entirely described</param>
	/// <returns>Text</returns>
	std::string ToString(bool printEdges);

	/// <summary>Convert to DOT text</summary>
	/// <param name="i">Node index (should be unique for each node)</param>
	/// <param name="scale">Multiplier-scale for position, 0 if free position</param>
	/// <returns>DOT text</returns>
	std::string ToDot(int i, float scale);

	/// <summary>Name getter</summary>
	/// <returns>Label of the node</returns>
	std::string GetName();

	/// <summary>X getter</summary>
	/// <returns>X-position</returns>
	float GetX();

	/// <summary>Y getter</summary>
	/// <returns>Y-position</returns>
	float GetY();

	/// <summary>Edges getter</summary>
	/// <returns>List of edges outgoing from this node. If Graph-object is used, do not try to modify theses edges manually ! (use Graph-object's functions)</returns>
	std::vector<Edge*> GetEdges();

	/// <summary>IsFixedPos getter</summary>
	/// <returns>If X-Y-position is fixed</returns>
	bool IsFixedPos();

private:

	/// <summary>Remove an edge from the list of incoming edges to this node. This edge will not be destroyed here ; it should be destroyed by the other node from where this edge come from.</summary>
	/// <param name="edge">The edge to unregister</param>
	void UnregisterIncomingEdge(Edge* edge);

	/// <summary>Delete all edges to going to a node, and remove all edges from these node going to the actual node.</summary>
	/// <param name="notNeighbourAnymore">The node to which edges have to be removed</param>
	/// <param name="unregisterReverseIncomingEdge">If reverse edges going from this node to the actual node have to be unregistered from list of incoming edges of the actual node</param>
	void DeleteEdgesToNode(Node* notNeighbourAnymore, bool unregisterReverseIncomingEdge);

	/// <summary>Label of the node</summary>
	std::string name;

	/// <summary>X-position</summary>
	float x;

	/// <summary>Y-position</summary>
	float y;

	/// <summary>If X-Y-position is fixed</summary>
	bool fixedPos;

	/// <summary>List of edges outgoing from this node</summary>
	std::vector<Edge*> edges;

	/// <summary>List of edges ingoing to this node (only for internal use)</summary>
	std::vector<Edge*> incomingEdges;

};

#endif
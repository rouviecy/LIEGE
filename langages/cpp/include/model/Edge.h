#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>

class Node;

/// <summary>Edge of the graph</summary>
class Edge{

public:

	/// <summary>Default constructor (only for STD containers, do not use it)</summary>
	Edge();

	/// <summary>Constructor (unnamed edge)</summary>
	/// <param name="fromNode">Source of the edge</param>
	/// <param name="toNode">Target of the edge</param>
	Edge(Node* fromNode, Node* toNode);

	/// <summary>Constructor</summary>
	/// <param name="fromNode">Source of the edge</param>
	/// <param name="toNode">Target of the edge</param>
	/// <param name="name">Label of the edge</param>
	Edge(Node* fromNode, Node* toNode, std::string name);

	/// <summary>To string (source and target nodes will be only light-described)</summary>
	/// <returns>Text</returns>
	std::string ToString();

	/// <summary>To string</summary>
	/// <param name="printNodes">If source and target nodes have to be entirely described</param>
	/// <returns>Text</returns>
	std::string ToString(bool printNodes);

	/// <summary>To string</summary>
	/// <param name="nodeToIndex">Dictionary giving unique a unique index for each node</param>
	/// <returns>DOT text</returns>
	std::string ToDot(std::map<Node*, int> nodeToIndex);

	/// <summary>Name getter</summary>
	/// <returns>Label of the edge</returns>
	std::string GetName();

	/// <summary>FromNode getter</summary>
	/// <returns>Source of the edge</returns>
	Node* GetFromNode();

	/// <summary>ToNode getter</summary>
	/// <returns>Target of the edge</returns>
	Node* GetToNode();

	/// <summary>IsValid getter</summary>
	/// <returns>If this edge is valid (source and target nodes are not null)</returns>
	bool IsValid();

private:

	/// <summary>Label of the edge</summary>
	std::string name;

	/// <summary>Source of the edge</summary>
	Node* fromNode;

	/// <summary>Target of the edge</summary>
	Node* toNode;

	/// <summary>If this edge is valid (source and target nodes are not null)</summary>
	bool isValid;

};

#endif
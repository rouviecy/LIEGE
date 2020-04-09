#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Edge.h"

#include <map>
#include <fstream>
#include <stdlib.h>

#include "../TinyXML/tinyxml.h"

/// <summary>Collections of nodes and edges</summary>
class Graph{

public:

	/// <summary>Default constructor</summary>
	Graph();

	/// <summary>Destructor : call destruction of all nodes and edges</summary>
	~Graph();

	/// <summary>Call destruction of all nodes and edges</summary>
	void Clear();

	/// <summary>Add a new node (unnamed and free node)</summary>
	/// <returns>Created node</returns>
	Node* CreateNode();

	/// <summary>Add a new node (free node)</summary>
	/// <param name="name">Label of the node</param>
	/// <returns>Created node</returns>
	Node* CreateNode(std::string name);

	/// <summary>Add a new node</summary>
	/// <param name="name">Label of the node</param>
	/// <param name="x">X-position</param>
	/// <param name="y">Y-position</param>
	/// <returns>Created node</returns>
	Node* CreateNode(std::string name, float x, float y);


	/// <summary>Create an edge between two nodes (the edge will be unnamed and without reverse edge)</summary>
	/// <param name="fromNode">The source node of the new edge</param>
	/// <param name="toNode">The target node of the new edge</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectNodes(Node* fromNode, Node* toNode);

	/// <summary>Create an edge between two nodes (he edge will be unnamed)</summary>
	/// <param name="fromNode">The source node of the new edge</param>
	/// <param name="toNode">The target node of the new edge</param>
	/// <param name="isBidirectional">If another reverse edge has to be created</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectNodes(Node* fromNode, Node* toNode, bool isBidirectional);

	/// <summary>Create an edge between two nodes</summary>
	/// <param name="fromNode">The source node of the new edge</param>
	/// <param name="toNode">The target node of the new edge</param>
	/// <param name="isBidirectional">If another reverse edge has to be created</param>
	/// <param name="edgeName">Label of the new edge</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectNodes(Node* fromNode, Node* toNode, bool isBidirectional, std::string edgeName);

	/// <summary>Create an edge between two nodes (the edge will be without reverse)</summary>
	/// <param name="fromNode">The source node of the new edge</param>
	/// <param name="toNode">The target node of the new edge</param>
	/// <param name="edgeName">Label of the new edge</param>
	/// <returns>List of created edges</returns>
	std::vector<Edge*> ConnectNodes(Node* fromNode, Node* toNode, std::string edgeName);

	/// <summary>Delete a node, incoming and outcoming edges</summary>
	/// <param name="node">The node to delete</param>
	void DeleteNode(Node* node);

	/// <summary>Delete an edge (reverse edges will be kept)</summary>
	/// <param name="edge">The edge to delete</param>
	void DeleteEdge(Edge* edge);

	/// <summary>Delete an edge</summary>
	/// <param name="edge">The edge to delete</param>
	/// <param name="removeAllReverses">If all reverse edges have to be also removed</param>
	void DeleteEdge(Edge* edge, bool removeAllReverses);

	/// <summary>To string</summary>
	/// <returns>Text</returns>
	std::string ToString();

	/// <summary>Nodes getter</summary>
	/// <returns>List of nodes. Do not try to modify theses nodes manually ! (use Graph-object's functions)</returns>
	std::vector<Node*> GetNodes();


	/// <summary>Convert C++ instances to DOT text (nodes will have free positions)</summary>
	/// <param name="pathDot">The source node of the new edge</param>
	void ExportDot(std::string pathDot);

	/// <summary>Convert C++ instances to DOT text</summary>
	/// <param name="pathDot">The source node of the new edge</param>
	/// <param name="scale">Multiplier-scale for positions, 0 if free positions</param>
	void ExportDot(std::string pathDot, float scale);

	/// <summary>Convert C++ instances to DOT text (nodes will have free positions)</summary>
	/// <param name="pathDot">The source node of the new edge</param>
	/// <param name="pathPng">Path of PNG file to draw</param>
	void ExportDot(std::string pathDot, std::string pathPng);

	/// <summary>Convert C++ instances to DOT text (nodes will have free positions)</summary>
	/// <param name="pathDot">The source node of the new edge</param>
	/// <param name="pathPng">Path of PNG file to draw</param>
	/// <param name="pathSvg">Path of SVG file to draw</param>
	void ExportDot(std::string pathDot, std::string pathPng, std::string pathSvg);

	/// <summary>Convert C++ instances to DOT text</summary>
	/// <param name="pathDot">The source node of the new edge</param>
	/// <param name="pathPng">Path of PNG file to draw</param>
	/// <param name="scale">Multiplier-scale for positions, 0 if free positions</param>
	void ExportDot(std::string pathDot, std::string pathPng, float scale);

	/// <summary>Convert C++ instances to DOT text</summary>
	/// <param name="pathDot">The source node of the new edge</param>
	/// <param name="pathPng">Path of PNG file to draw</param>
	/// <param name="pathSvg">Path of SVG file to draw</param>
	/// <param name="scale">Multiplier-scale for positions, 0 if free positions</param>
	void ExportDot(std::string pathDot, std::string pathPng, std::string pathSvg, float scale);

	/// <summary>Read GraphML file to instanciate C++ objects</summary>
	/// <param name="path">Path of GraphML file to read</param>
	void ImportGraphml(std::string path);

private:

	/// <summary>List of nodes</summary>
	std::vector<Node*> nodes;

	/// <summary>Find a child with given tag from an XML element</summary>
	/// <param name="xmlSuper">XML element to analyse</param>
	/// <param name="childTag">Child tag to find</param>
	/// <returns>Found child (null if no child has been found with this tag)</returns>
	TiXmlElement* FindXmlChild(TiXmlElement* xmlSuper, const char* childTag);

	/// <summary>Find all children with given tag from an XML element</summary>
	/// <param name="xmlSuper">XML element to analyse</param>
	/// <param name="childsTag">Child tag to find</param>
	/// <returns>List of found child (empty if no child has been found with this tag)</returns>
	std::vector<TiXmlElement*> FindXmlChilds(TiXmlElement* xmlSuper, const char* childsTag);

};

#endif
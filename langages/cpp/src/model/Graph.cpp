#include "../../include/model/Graph.h"

using namespace std;

Graph::Graph(){
	Clear();
}

Graph::~Graph(){
	Clear();
}

void Graph::Clear(){
	// Clearing list of nodes calls destruction of the nodes, but we have to call manually shrink to reduce memory used by the empty list
	nodes.clear();
	nodes.shrink_to_fit();
}

Node* Graph::CreateNode(){
	Node* newNode = new Node();
	nodes.push_back(newNode);
	return newNode;
}

Node* Graph::CreateNode(string name){
	Node* newNode = new Node(name);
	nodes.push_back(newNode);
	return newNode;
}

Node* Graph::CreateNode(string name, float x, float y){
	Node* newNode = new Node(name, x, y);
	nodes.push_back(newNode);
	return newNode;
}

vector<Edge*> Graph::ConnectNodes(Node* fromNode, Node* toNode){
	return fromNode->ConnectToNode(toNode);
}

vector<Edge*> Graph::ConnectNodes(Node* fromNode, Node* toNode, bool isBidirectional){
	return fromNode->ConnectToNode(toNode, isBidirectional);
}

vector<Edge*> Graph::ConnectNodes(Node* fromNode, Node* toNode, string edgeName){
	return fromNode->ConnectToNode(toNode, edgeName);
}

vector<Edge*> Graph::ConnectNodes(Node* fromNode, Node* toNode, bool isBidirectional, string edgeName){
	return fromNode->ConnectToNode(toNode, isBidirectional, edgeName);
}

void Graph::DeleteNode(Node* node){
	if(node == NULL){
		cout << "[WARNING] Trying to delete a null node ; it has been ignored" << endl;
		return;
	}
	vector<Node*>::iterator nodeInCollection = find(nodes.begin(), nodes.end(), node);
	if(nodeInCollection == nodes.end()){
		cout << "[WARNING] Trying to delete node \"" << node->GetName() << "\" which is not in graph collection ; this node has not been removed" << endl;
		return;
	}
	// Nodes are created in Graph context. They have to be deleted here. Deletion call destruction of the node, which call the destruction of all edges from and to this node.
	delete node;
	nodes.erase(nodeInCollection);
}

void Graph::DeleteEdge(Edge* edge){
	DeleteEdge(edge, false);
}

void Graph::DeleteEdge(Edge* edge, bool removeAllReverses){
	// Edges are created in Node context. They have to be deleted by the node.
	edge->GetFromNode()->DeleteEdge(edge, removeAllReverses);
}

string Graph::ToString(){
	// Print all nodes with details
	stringstream ss;
	for(vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it){
		ss << (*it)->ToString(true) << endl;
	}
	return ss.str();
}

vector<Node*> Graph::GetNodes(){
	return nodes;
}

void Graph::ExportDot(string pathDot){
	ExportDot(pathDot, "", "");
}

void Graph::ExportDot(string pathDot, string pathPng){
	ExportDot(pathDot, pathPng, "");
}

void Graph::ExportDot(string pathDot, string pathPng, string pathSvg){
	ExportDot(pathDot, pathPng, pathSvg, 0);
}

void Graph::ExportDot(string pathDot, float scale){
	ExportDot(pathDot, "", "", scale);
}

void Graph::ExportDot(string pathDot, string pathPng, float scale){
	ExportDot(pathDot, pathPng, "", scale);
}

void Graph::ExportDot(string pathDot, string pathPng, string pathSvg, float scale){
	map<Node*, vector<Edge*> > nodeToEdges;		// Discovered edges for each node
	map<Node*, int> nodeToIndex;			// Unique index for each node
	int i = 0;					// Unique index to increment
	bool fixedPos = false;				// If any node has a fixed position
	ofstream fichier(pathDot);			// DOT file to write
	fichier << "digraph LIEGE{" << endl;		// DOT header. TODO : add metadata for nodes and edges
	// Iterate over the nodes ...
	for(vector<Node*>::iterator node = nodes.begin(); node != nodes.end(); ++node){
		// Grab DOT text for this node
		fichier << (*node)->ToDot(i, scale) << endl;
		// If this node is fixed, all the graph will be fixed
		fixedPos = fixedPos || (*node)->IsFixedPos();
		// Discover all edges from this node
		nodeToEdges[*node] = (*node)->GetEdges();
		// Give the unique index to this node
		nodeToIndex[*node] = i++;
	}
	// ... and iterate over the discovered edges for each node
	for(map<Node*, vector<Edge*> >::iterator node = nodeToEdges.begin(); node != nodeToEdges.end(); ++node){
		for(vector<Edge*>::iterator edge = node->second.begin(); edge != node->second.end(); ++edge){
			// Grab DOT text for this edge
			fichier << (*edge)->ToDot(nodeToIndex) << endl;
		}
	}
	// DOT footer and close
	fichier << "}" << endl;
	fichier.close();
	stringstream ss;
	// Generate PNG file if path is given
	if(!pathPng.empty()){ //TODO : verif si le chemin est possible
		ss << "dot ";
		if(fixedPos){
			ss << "-Kfdp ";
		}
		ss << "-Tpng " << pathDot << " > " << pathPng;
		system(ss.str().c_str());
	}
	// Generate SVG file if path is given
	ss.str(string());
	if(!pathSvg.empty()){ //idem
		ss << "dot ";
		if(fixedPos){
			ss << "-Kfdp ";
		}
		ss << "-Tsvg " << pathDot << " > " << pathSvg;
		system(ss.str().c_str());
	}
}

void Graph::ImportGraphml(string path){
	map<string, Node*> iToNode;	// Dictionary of found nodes, key is unique index from GraphML
	TiXmlDocument file;		// XML file to read
	// Try to open XML file
	if(!file.LoadFile(path.c_str())){
		cout << "[ERROR] Failed to open GraphML file \"" << path << "\" ; no node and edge as been imported" << endl;
		return;
	}
	// Grab the root-element
	TiXmlElement* xmlRoot = file.FirstChildElement();
	if(xmlRoot == NULL){
		cout << "[ERROR] Failed to load root element from GraphML file \"" << path << "\" ; no node and edge as been imported" << endl;
		return;
	}
	// Grab the graph-element in the root-element
	TiXmlElement* xmlGraph = FindXmlChild(xmlRoot, "graph");
	if(xmlGraph == NULL){
		cout << "[ERROR] Failed to load root/graph element from GraphML file \"" << path << "\" ; no node and edge as been imported" << endl;
		return;
	}
	// Grab the node-elements in the graph-element
	vector<TiXmlElement*> xmlNodes = FindXmlChilds(xmlGraph, "node");
	if(xmlNodes.empty()){
		cout << "[ERROR] Failed to load root/graph/node[] elements from GraphML file \"" << path << "\" ; no node and edge as been imported" << endl;
		return;
	}
	// Iterate over the node-elements
	for(vector<TiXmlElement*>::iterator xmlNode = xmlNodes.begin(); xmlNode != xmlNodes.end(); ++xmlNode){
		// Read the unique index for this node
		const char* i = (*xmlNode)->Attribute("id");
		if(i == NULL){
			cout << "[WARNING] Failed to read id attribute of a root/graph/node from GraphML file \"" << path << "\" ; this node has been ignored" << endl;
			continue;
		}
		// Geometry-element and label-element not found yet
		TiXmlElement* xmlGeometry = NULL;
		TiXmlElement* xmlLabel = NULL;
		// Iterate the data-elements in the node-element
		vector<TiXmlElement*> xmlDatas = FindXmlChilds(*xmlNode, NULL);
		for(vector<TiXmlElement*>::iterator xmlData = xmlDatas.begin(); xmlData != xmlDatas.end(); ++xmlData){
			// Iterate the data-elements in the node-element
			vector<TiXmlElement*> xmlShapes = FindXmlChilds(*xmlData, "y:ShapeNode");
			for(vector<TiXmlElement*>::iterator xmlShape = xmlShapes.begin(); xmlShape != xmlShapes.end(); ++xmlShape){
				// Grab the geometry-element and label-element in the shape-element
				xmlGeometry = FindXmlChild(*xmlShape, "y:Geometry");
				xmlLabel = FindXmlChild(*xmlShape, "y:NodeLabel");
			}
		}
		// If geometry-element or the label-element are not found, ignore this node
		if(xmlGeometry == NULL){
			cout << "[WARNING] Failed to load root/graph/node/data/y:ShapeNode/y:Geometry for node \"" << i << "\" from GraphML file \"" << path << "\" ; this node has been ignored" << endl;
			continue;
		}
		if(xmlLabel == NULL){
			cout << "[WARNING] Failed to load root/graph/node/data/y:ShapeNode/y:NodeLabel for node \"" << i << "\" from GraphML file \"" << path << "\" ; this node has been ignored" << endl;
			continue;
		}
		// Read the name and the position of this node
		const char* name = xmlLabel->GetText();
		const char* x = xmlGeometry->Attribute("x");
		const char* y = xmlGeometry->Attribute("y");
		if(name == NULL){
			cout << "[WARNING] Failed to read name value of root/graph/node/data/y:ShapeNode/y:NodeLabel for node \"" << i << "\" from GraphML file \"" << path << "\" ; this node has been ignored" << endl;
			continue;
		}
		if(x == NULL){
			cout << "[WARNING] Failed to read x attribute of root/graph/node/data/y:ShapeNode/y:Geometry for node \"" << i << "\" from GraphML file \"" << path << "\" ; this node has been ignored" << endl;
			continue;
		}
		if(y == NULL){
			cout << "[WARNING] Failed to read x attribute of root/graph/node/data/y:ShapeNode/y:Geometry for node \"" << i << "\" from GraphML file \"" << path << "\" ; this node has been ignored" << endl;
			continue;
		}
		// Create the node and put it in the dictionary
		iToNode[string(i)] = CreateNode(string(name), atof(x), atof(y));
	}
	// Grab the edge-elements in the graph-element
	vector<TiXmlElement*> xmlEdges = FindXmlChilds(xmlGraph, "edge");
	if(xmlEdges.empty()){
		cout << "[WARNING] Failed to load root/graph/edge[] elements from GraphML file \"" << path << "\" ; no edge as been imported" << endl;
		return;
	}
	// Iterate over the edge-elements
	for(vector<TiXmlElement*>::iterator xmlEdge = xmlEdges.begin(); xmlEdge != xmlEdges.end(); ++xmlEdge){
		// Read the unique id of this edge, of the source node and of the target node
		const char* iEdge = (*xmlEdge)->Attribute("id");
		const char* iSource = (*xmlEdge)->Attribute("source");
		const char* iTarget = (*xmlEdge)->Attribute("target");
		if(iEdge == NULL){
			cout << "[WARNING] Failed to read id attribute of a root/graph/edge[] from GraphML file \"" << path << "\" ; this edge has been ignored" << endl;
			continue;
		}
		if(iSource == NULL){
			cout << "[WARNING] Failed to read source attribute of root/graph/edge for edge \"" << iEdge << "\" from GraphML file \"" << path << "\" ; this edge has been ignored" << endl;
			continue;
		}
		if(iTarget == NULL){
			cout << "[WARNING] Failed to read target attribute of root/graph/edge for edge \"" << iEdge << "\" from GraphML file \"" << path << "\" ; this edge has been ignored" << endl;
			continue;
		}
		// Find source and target node in the dictionary with their unique id
		Node* nodeSource = iToNode[string(iSource)];
		Node* nodeTarget = iToNode[string(iTarget)];
		// Arrow-elements not found yet
		TiXmlElement* xmlArrows = NULL;
		// Grab the data-elements in the edge-element
		vector<TiXmlElement*> xmlDatas = FindXmlChilds(*xmlEdge, "data");
		if(xmlDatas.size() == 0){
			cout << "[WARNING] Failed to load /root/graph/edge/data[] for edge \"" << iEdge << "\" from GraphML file \"" << path << "\" ; this edge will be bidirectional" << endl;
		}
		// Iterate over the data-elements
		for(vector<TiXmlElement*>::iterator xmlData = xmlDatas.begin(); xmlData != xmlDatas.end(); ++xmlData){
			// Grab the unknown-elements in the data-element
			vector<TiXmlElement*> xmlUnknowns = FindXmlChilds(*xmlData, NULL);
			if(xmlUnknowns.size() == 0){
				cout << "[WARNING] Failed to load /root/graph/edge/data/?[] for edge \"" << iEdge << "\" from GraphML file \"" << path << "\" ; this edge will be bidirectional" << endl;
			}
			// Iterate over the unknown-elements
			for(vector<TiXmlElement*>::iterator xmlUnknown = xmlUnknowns.begin(); xmlUnknown != xmlUnknowns.end(); ++xmlUnknown){
				// Grab the arrows-element in the unknown-element
				xmlArrows = FindXmlChild(*xmlUnknown, "y:Arrows");
			}
		}
		bool sourceToTarget = true;
		bool targetToSource = true;
		// If no arrows-element has been found, the edge will be bidirectional
		if(xmlArrows == NULL){
			cout << "[WARNING] Failed to load /root/graph/edge/data/?/Arrows for edge \"" << iEdge << "\" from GraphML file \"" << path << "\" ; this edge will be bidirectional" << endl;
		}
		else{
			// If source-attribute of the arrows-element is 'none', consider the edge not oriented from source to target
			const char* target = xmlArrows->Attribute("target");
			sourceToTarget = (target == NULL || strcmp(target, "none") != 0);
			// If target-attribute of the arrows-element is 'none', consider the edge not oriented from target to source
			const char* source = xmlArrows->Attribute("source");
			targetToSource = (source == NULL || strcmp(source, "none") != 0);
		}
		// If no orientation is found of two orientations are found, create a bidirectional edge and save it
		if(!(sourceToTarget ^ targetToSource)){
			ConnectNodes(nodeSource, nodeTarget, true);
		}
		// If only an orientation from source to target is found, create a unidirectional edge from source to target and save it
		else if(sourceToTarget){
			ConnectNodes(nodeSource, nodeTarget, false);
		}
		// If only an orientation from target to source is found, create a unidirectional edge from target to source and save it (possible if user create a link in yEd from a node to another and change arrows style after)
		else if(targetToSource){
			ConnectNodes(nodeTarget, nodeSource, false);
		}
		// Else, there is a bug in the matrix
		else{}
	}
}

TiXmlElement* Graph::FindXmlChild(TiXmlElement* xmlSuper, const char* childTag){
	for(TiXmlElement* child = xmlSuper->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
		if(childTag == NULL || strcmp(child->Value(), childTag) == 0){
			return child;
		}
	}
	return NULL;
}

vector<TiXmlElement*> Graph::FindXmlChilds(TiXmlElement* xmlSuper, const char* childsTag){
	vector<TiXmlElement*> resultat;
	for(TiXmlElement* child = xmlSuper->FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
		if(childsTag == NULL || strcmp(child->Value(), childsTag) == 0){
			resultat.push_back(child);
		}
	}
	return resultat;
}

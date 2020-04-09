#include "../../include/model/Node.h"
#include "../../include/model/Edge.h"

using namespace std;

Node::Node() : Node(""){}

Node::Node(string name){
	this->name = name;
	this->fixedPos = false;
}

Node::Node(string name, float x, float y): Node(name){
	this->x = x;
	this->y = y;
	this->fixedPos = true;
}

Node::~Node(){
	// Iterate other the outgoing edges to find all neighbours
	for(vector<Edge*>::iterator edge = edges.begin(); edge != edges.end(); ++edge){
		// Ask neighbour to delete all edges to me and to ask me to unregister these edges from my incoming edges list
		(*edge)->GetToNode()->DeleteEdgesToNode(this, true);
	}
	// Iterate other remaining incoming edges (from nodes I have not outgoing edge to)
	for(vector<Edge*>::iterator edge = incomingEdges.begin(); edge != incomingEdges.end(); ++edge){
		// Ask neighbour to delete all edges to me without asking the to unregister theses edges from my incoming edges list because I am actually iterating over them !
		(*edge)->GetFromNode()->DeleteEdgesToNode(this, false);
	}
	// Now the only remaining edges are mine
	// Clearing list of edges calls destruction of the edges, but we have to call manually shrink to reduce memory used by the empty list
	edges.clear();
	edges.shrink_to_fit();
	// Clearing list of edges calls destruction of the edges, but we have to call manually shrink to reduce memory used by the empty list
	incomingEdges.clear();
	incomingEdges.shrink_to_fit();
}

vector<Edge*> Node::ConnectToNode(Node* neighbourNode){
	return ConnectToNode(neighbourNode, "");
}

vector<Edge*> Node::ConnectToNode(Node* neighbourNode, string edgeName){
	return ConnectToNode(neighbourNode, false, name);
}

vector<Edge*> Node::ConnectToNode(Node* neighbourNode, bool isBidirectional){
	return ConnectToNode(neighbourNode, isBidirectional, "");
}

vector<Edge*> Node::ConnectToNode(Node* neighbourNode, bool isBidirectional, string edgeName){
	// List of created edges
	vector<Edge*> resultat;
	if(neighbourNode == NULL){
		cout << "[WARNING] A null-pointer for neighbourNode as been provided to connect the node \"" << name << "\" ; this connection will be ignored" << endl;
		return resultat;
	}
	// Create an edge from me to the other node
	Edge* newEdge = new Edge(this, neighbourNode, edgeName);
	edges.push_back(newEdge);
	// Register this edge to the list of incoming edges from the point of view of the other node
	neighbourNode->incomingEdges.push_back(newEdge);
	resultat.push_back(newEdge);
	if(!isBidirectional){
		return resultat;
	}
	// If it is bidirectional, ask the other node to create an edge to me (this one will not be bidirectional to avoid infinite recursivity)
	resultat = neighbourNode->ConnectToNode(this, false, edgeName);
	resultat.push_back(newEdge);
	return resultat;
}

void Node::DeleteEdgesToNode(Node* notNeighbourAnymore){
	DeleteEdgesToNode(notNeighbourAnymore, true);
}

void Node::DeleteEdgesToNode(Node* notNeighbourAnymore, bool unregisterReverseIncomingEdge){
	// Iterate over outgoing edges
	for(vector<Edge*>::iterator edge = edges.begin(); edge != edges.end();){
		// Ignore edges with other target node
		if((*edge)->GetToNode() != notNeighbourAnymore){
			++edge;
			continue;
		}
		// Ask target node to unregister this edge from its incoming edges list
		if(unregisterReverseIncomingEdge){
			(*edge)->GetToNode()->UnregisterIncomingEdge(*edge);
		}
		// Edges are created in node context. They have to be deleted here.
		delete *edge;
		edge = edges.erase(edge);
	}
	// Iterate over incoming edges
	for(vector<Edge*>::iterator edge = incomingEdges.begin(); edge != incomingEdges.end();){
		// Ignore edges with other source node
		if((*edge)->GetFromNode() != notNeighbourAnymore){
			++edge;
			continue;
		}
		// Do not delete incoming edges here : they are created in the other node context. Let them delete these edges theirselves.
		edge = incomingEdges.erase(edge);
	}
}

void Node::DeleteEdge(Edge* notEdgeAnymore, bool removeAllReverses){
	// Try to find this edge in my list of outgoing edges
	vector<Edge*>::iterator edge = find(edges.begin(), edges.end(), notEdgeAnymore);
	if(edge == edges.end()){
		cout << "[WARNING] Trying to delete edge \"" << notEdgeAnymore->GetName() << "\" from node \"" << name << "\" which does not contain this edge ; this edge has been kept" << endl;
		return;
	}
	// If needed, ask neighbour to delete all edges to me and to ask me to unregister these edges from my incoming edges list
	if(removeAllReverses){
		notEdgeAnymore->GetToNode()->DeleteEdgesToNode(this, true);
	}
	else{
		// Else, only ask him to unregister this edge from his incoming edge list
		notEdgeAnymore->GetToNode()->UnregisterIncomingEdge(notEdgeAnymore);
	}
	// Edges are created in node context. They have to be deleted here.
	delete notEdgeAnymore;
	edges.erase(edge);
}

void Node::DeleteEdge(Edge* notEdgeAnymore){
	DeleteEdge(notEdgeAnymore, false);
}

void Node::UnregisterIncomingEdge(Edge* edge){
	// Try to find this edge in my incoming edges list
	vector<Edge*>::iterator incomingEdge = find(incomingEdges.begin(), incomingEdges.end(), edge);
	if(incomingEdge == incomingEdges.end()){
		cout << "[WARNING] Trying to unregister incoming edge \"" << edge->GetName() << "\" from node \"" << name << "\" which does not have registered this incoming edge" << endl;
		return;
	}
	// Do not delete incoming edges here : they are created in the other node context. Let them delete these edges theirselves.
	incomingEdges.erase(incomingEdge);
}

string Node::ToString(){
	return ToString(false);
}

string Node::ToString(bool printEdges){
	stringstream ss;
	ss << "Node " << name << " [x=";
	if(fixedPos){ss << x;}
	else{ss << "?";}
	ss << " ; y=";
	if(fixedPos){ss << y;}
	else{ss << "?";}
	ss << "]";
	// Never ask edges to print details to avoid ungly recursivity
	if(printEdges){
		for(vector<Edge*>::iterator edge = edges.begin(); edge != edges.end(); ++edge){
			ss << "\n\tOUT : " << (*edge)->ToString(false);
		}
		for(vector<Edge*>::iterator edge = incomingEdges.begin(); edge != incomingEdges.end(); ++edge){
			ss << "\n\tIN : " << (*edge)->ToString(false);
		}
	}
	return ss.str();
}

string Node::ToDot(int i, float scale){
	stringstream ss;
	ss << "\t" << i << "[\n\t\tlabel = \"" << name << "\"\n";
	if(fixedPos && scale > 0){
		ss << "\t\tpos = \"" << x * scale << "," << -y * scale << "\"\n";
	}
	ss << "\t]";
	return ss.str();
}

string Node::GetName(){return name;}
float Node::GetX(){return x;}
float Node::GetY(){return y;}
vector<Edge*> Node::GetEdges(){return edges;}
bool Node::IsFixedPos(){return fixedPos;}
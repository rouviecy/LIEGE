#include "../../include/model/Edge.h"
#include "../../include/model/Node.h"

using namespace std;

Edge::Edge() : Edge(NULL, NULL){}

Edge::Edge(Node* fromNode, Node* toNode) : Edge(fromNode, toNode, ""){}

Edge::Edge(Node* fromNode, Node* toNode, string name){
	this->fromNode = fromNode;
	this->toNode = toNode;
	this->name = name;
	this->isValid = true;
	// This edge will not be valid if source node or target node is null
	if(fromNode == NULL){
		this->isValid = false;
		cout << "[WARNING] A null-pointer for fromNode has been provided to create edge \"" << name << "\" ; this edge could be ignored later" << endl;
	}
	if(toNode == NULL){
		this->isValid = false;
		cout << "[WARNING] A null-pointer for toNode has been provided to create edge \"" << name << "\" ; this edge could be ignored later" << endl;
	}
}

string Edge::ToString(){
	return ToString(false);
}

string Edge::ToString(bool printNodes){
	if(!isValid){
		return "This edge is not valid";
	}
	stringstream ss;
	// If source and target nodes have to be printed, do not print their edges to avoid ugly recursivity !
	if(printNodes){
		ss << "Edge \"" << name << "\"\n\tfrom " << fromNode->ToString(false) << "\n\tto " << toNode->ToString(false);
	}
	else{
		ss << "Edge \"" << name << "\" from \"" << fromNode->GetName() << "\" to \"" << toNode->GetName() << "\"";
	}
	return ss.str();
}

string Edge::ToDot(map<Node*, int> nodeToIndex){
	stringstream ss;
	ss << "\t" << nodeToIndex[fromNode] << " -> " << nodeToIndex[toNode] << " [label = \"" << name << "\"]";
	return ss.str();
}

string Edge::GetName(){return name;}
Node* Edge::GetFromNode(){return fromNode;}
Node* Edge::GetToNode(){return toNode;}
bool Edge::IsValid(){return isValid;}
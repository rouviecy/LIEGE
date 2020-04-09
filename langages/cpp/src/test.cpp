#include "../include/model/Graph.h"

#include <filesystem>

using namespace std;

string ForgePath(filesystem::path path, string filename, string extension){
	return (path / filename).string() + "." + extension;
}

void ExportFreePng(Graph graph, filesystem::path path, string filename){
	graph.ExportDot(ForgePath(path, filename, "dot"), ForgePath(path, filename, "png"), 0);
}

int main(int argc, char* argv[]){
	filesystem::path pathIn = "../tests/in";
	filesystem::path pathOut = "../tests/out";

	Graph graph;
	graph.ImportGraphml(ForgePath(pathIn, "simple", "graphml"));
	graph.ExportDot(ForgePath(pathOut, "simpleFixed", "dot"), ForgePath(pathOut, "simpleFixed", "png"), ForgePath(pathOut, "simpleFixed", "svg"), 0.02);
	graph.ExportDot(ForgePath(pathOut, "simpleFree", "dot"), ForgePath(pathOut, "simpleFree", "png"), ForgePath(pathOut, "simpleFree", "svg"), 0);

	graph.Clear();
	ExportFreePng(graph, pathOut, "scratch00");
	Node* nodeA = graph.CreateNode("A");
	ExportFreePng(graph, pathOut, "scratch01");
	Node* nodeB = graph.CreateNode("B");
	ExportFreePng(graph, pathOut, "scratch02");
	Edge* edgeAB = graph.ConnectNodes(nodeA, nodeB, false, "A to B")[0];
	ExportFreePng(graph, pathOut, "scratch03");
	Node* nodeC = graph.CreateNode("C");
	ExportFreePng(graph, pathOut, "scratch04");
	Edge* edgeBC = graph.ConnectNodes(nodeB, nodeC, false, "B to C")[0];
	ExportFreePng(graph, pathOut, "scratch05");
	Node* nodeD = graph.CreateNode("D");
	ExportFreePng(graph, pathOut, "scratch06");
	Edge* edgeCD = graph.ConnectNodes(nodeC, nodeD, false, "C to D")[0];
	ExportFreePng(graph, pathOut, "scratch07");
	Edge* edgeDA = graph.ConnectNodes(nodeD, nodeA, false, "D to A")[0];
	ExportFreePng(graph, pathOut, "scratch08");
	Node* nodeE = graph.CreateNode("E");
	ExportFreePng(graph, pathOut, "scratch09");
	vector<Edge*> edgesAE = graph.ConnectNodes(nodeA, nodeE, true, "A - E");
	ExportFreePng(graph, pathOut, "scratch10");
	vector<Edge*> edgesDE = graph.ConnectNodes(nodeD, nodeE, true, "D - E");
	ExportFreePng(graph, pathOut, "scratch11");
	graph.DeleteNode(nodeA);
	ExportFreePng(graph, pathOut, "scratch12");
	graph.DeleteEdge(edgesDE[0], false);
	ExportFreePng(graph, pathOut, "scratch13");
	graph.DeleteEdge(edgeCD, false);
	ExportFreePng(graph, pathOut, "scratch14");
	vector<Edge*> edgesBE = graph.ConnectNodes(nodeB, nodeE, true, "B - E");
	ExportFreePng(graph, pathOut, "scratch15");
	vector<Edge*> edgesCD = graph.ConnectNodes(nodeC, nodeD, true, "C - D");
	ExportFreePng(graph, pathOut, "scratch16");
	graph.DeleteNode(nodeE);
	ExportFreePng(graph, pathOut, "scratch17");
	graph.DeleteEdge(edgesCD[0], true);
	ExportFreePng(graph, pathOut, "scratch18");
	graph.DeleteNode(nodeD);
	ExportFreePng(graph, pathOut, "scratch19");
	graph.DeleteNode(nodeB);
	ExportFreePng(graph, pathOut, "scratch20");
	graph.DeleteNode(nodeC);
	ExportFreePng(graph, pathOut, "scratch21");
	return 0;
}
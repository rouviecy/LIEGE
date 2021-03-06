import LIEGE
import os

# OS-independant way to access to input and output test files
pathIn = os.path.join('tests', 'in')
pathOut = os.path.join('tests', 'out')

# Fonction to export a graph to PNG with free positions
def ExportFreePng(graph, filename):
	graph.ExportDot(os.path.join(pathOut, filename + '.dot'), pathPng = os.path.join(pathOut, filename + '.png'), scale = 0)

# Create a collection of nodes and edges
graph = LIEGE.Graph()

# Import nodes and edges from GraphML file generated by yEd
graph.ImportGraphml(os.path.join(pathIn, 'simple.graphml'))

# Print all nodes
for node in graph.nodes:
	print(node)

# Print all edges
for edge in graph.edges:
	print(edge)

# Generate DOT, PNG and SVG file with GraphViz with positions from GraphML file
graph.ExportDot(os.path.join(pathOut, 'simpleFixed.dot'), pathPng = os.path.join(pathOut, 'simpleFixed.png'), pathSvg = os.path.join(pathOut, 'simpleFixed.svg'), scale = 0.02)

# Generate DOT, PNG and SVG file with GraphViz with free positions
graph.ExportDot(os.path.join(pathOut, 'simpleFree.dot'), pathPng = os.path.join(pathOut, 'simpleFree.png'), pathSvg = os.path.join(pathOut, 'simpleFree.svg'), scale = 0)

# Clear and create step-by-step an new graph from scratch
graph.Clear()
ExportFreePng(graph, 'scratch00')
nodeA = graph.CreateNode('A')
ExportFreePng(graph, 'scratch01')
nodeB = graph.CreateNode('B')
ExportFreePng(graph, 'scratch02')
edgeAB = graph.ConnectNodes(nodeA, nodeB, isBidirectional = False, edgeName = 'A to B')[0]
ExportFreePng(graph, 'scratch03')
nodeC = graph.CreateNode('C')
ExportFreePng(graph, 'scratch04')
edgeBC = graph.ConnectNodes(nodeB, nodeC, isBidirectional = False, edgeName = 'B to C')[0]
ExportFreePng(graph, 'scratch05')
nodeD = graph.CreateNode('D')
ExportFreePng(graph, 'scratch06')
edgeCD = graph.ConnectNodes(nodeC, nodeD, isBidirectional = False, edgeName = 'C to D')[0]
ExportFreePng(graph, 'scratch07')
edgeDA = graph.ConnectNodes(nodeD, nodeA, isBidirectional = False, edgeName = 'D to A')[0]
ExportFreePng(graph, 'scratch08')
nodeE = graph.CreateNode('E')
ExportFreePng(graph, 'scratch09')
edgeAE, edgeEA = graph.ConnectNodes(nodeA, nodeE, isBidirectional = True, edgeName = 'A - E')
ExportFreePng(graph, 'scratch10')
edgeDE, edgeED = graph.ConnectNodes(nodeD, nodeE, isBidirectional = True, edgeName = 'D - E')
ExportFreePng(graph, 'scratch11')
graph.DeleteNode(nodeA)
ExportFreePng(graph, 'scratch12')
graph.DeleteEdge(edgeED, removeAllReverses = False)
ExportFreePng(graph, 'scratch13')
graph.DeleteEdge(edgeCD, removeAllReverses = False)
ExportFreePng(graph, 'scratch14')
edgeBE, edgeEB = graph.ConnectNodes(nodeB, nodeE, isBidirectional = True, edgeName = 'B - E')
ExportFreePng(graph, 'scratch15')
edgeCD, edgeDC = graph.ConnectNodes(nodeC, nodeD, isBidirectional = True, edgeName = 'C - D')
ExportFreePng(graph, 'scratch16')
graph.DeleteNode(nodeE)
ExportFreePng(graph, 'scratch17')
graph.DeleteEdge(edgeCD, removeAllReverses = True)
ExportFreePng(graph, 'scratch18')
graph.DeleteNode(nodeD)
ExportFreePng(graph, 'scratch19')
graph.DeleteNode(nodeB)
ExportFreePng(graph, 'scratch20')
graph.DeleteNode(nodeC)
ExportFreePng(graph, 'scratch21')

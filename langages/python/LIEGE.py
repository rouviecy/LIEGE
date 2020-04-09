## @package LIEGE
#  Librairie d'Import Export de Graphes Electroniques

import os
import xml.etree.ElementTree as ET

## Node of the graph
class Node:

	## Constructor
	#  @param name Label of the node
	#  @param x X-position if given
	#  @param y Y-position if given
	def __init__(self, name, x = None, y = None):
		self.name = name
		self.x = x
		self.y = y
		self.edges = []

	## Create an edge between this node and another node
	#  @param neighbourNode The other node to link
	#  @param isBidirectional If another reverse edge has to be created
	#  @param edgeName Label of the new edge
	#  @return List of created edges
	def ConnectToNode(self, neighbourNode, isBidirectional = False, edgeName = ''):
		# Create forward edge
		newEdge = Edge(self, neighbourNode, edgeName)
		self.edges.append(newEdge)
		if not isBidirectional:
			return [newEdge]
		# Create backward edge
		return [newEdge, neighbourNode.ConnectToNode(self, False, edgeName)[0]]

	## To string
	#  @return Text
	def __str__(self):
		return 'Node %s [x=%s ; y=%s]' % (self.name, '?' if self.x == None else self.x, '?' if self.y == None else self.y)

	## Convert to DOT text
	#  @param i Node index (should be unique for each node)
	#  @param scale Multiplier-scale for position, 0 if free position
	#  @return DOT text
	def ToDot(self, i, scale):
		fixedPos = scale > 0 and (not self.x == None) and (not self.y == None)
		return '\t%s[\n\t\tlabel = \"%s\"%s\n\t]' % (str(i), self.name, '\n\t\tpos = \"%f,%f!\"' % (self.x * scale, -self.y * scale) if fixedPos else ''), fixedPos # y = -y because GraphML and DOT are horizontal-mirrored

## Edge of the graph
class Edge:

	## Constructor
	#  @param fromNode Source of the edge
	#  @param toNode Target of the edge
	#  @param name Label of the edge
	def __init__(self, fromNode, toNode, name = ''):
		self.fromNode = fromNode
		self.toNode = toNode
		self.name = name

	## To string
	#  @return Text
	def __str__(self):
		return 'Edge %s from %s to %s' % (self.name, str(self.fromNode), str(self.toNode))

	## Convert to DOT text
	#  @param nodeToIndex Dictionary<Node*,int> giving unique a unique index for each node
	#  @return DOT text
	def ToDot(self, nodeToIndex):
		return '\t%d -> %d [label = \"%s\"]' % (nodeToIndex[self.fromNode], nodeToIndex[self.toNode], self.name)

## Collections of nodes and edges
class Graph:

	## Constructor
	def __init__(self):
		self.nodes = []
		self.edges = []

	## To string
	#  @return Text
	def __str__(self):
		resultat = ''
		for node in self.nodes:
			resultat += str(node) + '\n'
		for edge in self.edges:
			resultat += str(edge) + '\n'
		return resultat

	## Remove all nodes and edges
	def Clear(self):
		self.nodes = []
		self.edges = []

	## Add a new node
	#  @param name Label of the node
	#  @param x X-position if given
	#  @param y Y-position if given
	#  @return Created node
	def CreateNode(self, name = '', x = None, y = None):
		newNode = Node(name, x, y)
		self.nodes.append(newNode)
		return newNode

	## Create an edge between two nodes
	#  @param fromNode The source node of the new edge
	#  @param toNode The target node of the new edge
	#  @param isBidirectional If another reverse edge has to be created
	#  @param edgeName Label of the new edge
	#  @return List of created edges
	def ConnectNodes(self, fromNode, toNode, isBidirectional = False, edgeName = ''):
		newEdges = fromNode.ConnectToNode(toNode, isBidirectional, edgeName)
		self.edges += newEdges
		return newEdges

	## Delete a node, incoming and outcoming edges
	#  @param node The node to delete
	def DeleteNode(self, node):
		connectedNodes = [edge.toNode for edge in self.edges]
		for connectedNode in connectedNodes:
			connectedNode.edges = [reverseEdge for reverseEdge in connectedNode.edges if not reverseEdge == node]
		self.edges = [edge for edge in self.edges if not edge.fromNode == node and not edge.toNode == node]
		self.nodes.remove(node)

	## Delete an edge
	#  @param edge The edge to delete
	#  @param removeAllReverses If all reverse edges have to be also removed
	def DeleteEdge(self, edge, removeAllReverses = False):
		edge.fromNode.edges.remove(edge)
		if removeAllReverses:
			reverseEdges = [reverseEdge for reverseEdge in edge.toNode.edges if reverseEdge.toNode == edge.fromNode]
			for reverseEdge in reverseEdges:
				edge.toNode.edges.remove(reverseEdge)
				self.edges.remove(reverseEdge)
		self.edges.remove(edge)

	## Convert Python instances to DOT text
	#  @param pathDot Path of DOT file to write
	#  @param pathPng Path of PNG file to draw (can be None to skip PNG drawing)
	#  @param pathSvg Path of SVG file to draw (can be None to skip SVG drawing)
	#  @param scale Multiplier-scale for positions, 0 if free positions
	def ExportDot(self, pathDot, pathPng = None, pathSvg = None, scale = 0):
		nodeToIndex = {}			# Unique index for each node
		fixedPos = False			# If any node has a fixed position
		i = 0					# Unique index to increment
		fichier = open(pathDot, 'w')		# DOT file to write
		fichier.write('digraph LIEGE{\n')	# DOT header. TODO : add metadata for nodes and edges
		# Iterate over the nodes
		for node in self.nodes:
			# Grab DOT text for this node
			nodeDot, isNodeFixed = node.ToDot(i, scale)
			fichier.write(nodeDot + '\n')
			# If this node is fixed, all the graph will be fixed
			if isNodeFixed:
				fixedPos = True
			# Give the unique index to this node
			nodeToIndex[node] = i
			i += 1
		# Iterate over the edges
		for edge in self.edges:
			# Grab DOT text for this edge
			fichier.write(edge.ToDot(nodeToIndex) + '\n')
		# DOT footer and write
		fichier.write('}')
		fichier.close()
		# Generate PNG file if path is given
		if not pathPng == None:
			os.system('dot %s-Tpng %s > %s' % ('-Kfdp ' if fixedPos else '', pathDot, pathPng))
		# Generate SVG file if path is given
		if not pathSvg == None:
			os.system('dot %s-Tsvg %s > %s' % ('-Kfdp ' if fixedPos else '', pathDot, pathSvg))

	## Read GraphML file to instanciate Python objects
	#  @path Path of GraphML file to read
	def ImportGraphml(self, path):
		nodes = {} # Dictionary of found nodes, key is unique index from GraphML
		# Grab the root-element, and iterate over node-elements in graph-element
		for xmlNode in ET.parse(path).getroot().find('{http://graphml.graphdrawing.org/xmlns}graph').findall('{http://graphml.graphdrawing.org/xmlns}node'):
			# Read the unique index for this node
			i = xmlNode.attrib['id']
			# Geometry-element and label-element not found yet
			xmlGeometry = None
			xmlLabel = None
			# Iterate over the data-elements in the node-element
			for xmlData in xmlNode:
				# Skip if this data-element do not contains any shape-element
				xmlShape = xmlData.find('{http://www.yworks.com/xml/graphml}ShapeNode')
				if xmlShape == None:
					continue
				# If a shape-element is found, grab the geometry-element and the label-element
				xmlGeometry = xmlShape.find('{http://www.yworks.com/xml/graphml}Geometry')
				xmlLabel = xmlShape.find('{http://www.yworks.com/xml/graphml}NodeLabel')
			# If geometry-element or the label-element are not found, ignore this node
			if xmlGeometry == None or xmlLabel == None:
				print('[WARNING] Unable to decode node with id=\"%s\" which has been ignored' % (i))
			# Read the name and the position of this node
			name = xmlLabel.text
			x = xmlGeometry.attrib['x']
			y = xmlGeometry.attrib['y']
			# Create the node and put it in the dictionary
			nodes[i] = self.CreateNode(name = name, x = float(x), y = float(y))
		# Grab the root-element, and iterate over edge-elements in graph-element
		for xmlEdge in ET.parse(path).getroot().find('{http://graphml.graphdrawing.org/xmlns}graph').findall('{http://graphml.graphdrawing.org/xmlns}edge'):
			# Read the unique id of this edge, of the source node and of the target node
			iEdge = xmlEdge.attrib['id']
			iSource = xmlEdge.attrib['source']
			iTarget = xmlEdge.attrib['target']
			# Find source and target node in the dictionary with their unique id
			nodeSource = nodes[iSource]
			nodeTarget = nodes[iTarget]
			# Arrow-elements not found yet
			xmlArrows = None
			# Iterate over data-element in unknown-element in this edge-element to find any arrows-element
			for xmlData in xmlEdge:
				for xmlUnknown in xmlData:
					xmlArrows = xmlUnknown.find('{http://www.yworks.com/xml/graphml}Arrows')
					if xmlArrows == None:
						continue
			# If no arrows-element has been found, the edge will be bidirectional
			if xmlArrows == None:
				print('[WARNING] Unable to find direction of edge with id=\"%s\" which as been added as bidirectional between node with id=\"%s\" and node with id=\"%s\"' % (i, iSource, iTarget))
				sourceToTarget = True
				targetToSource = True
			# If target-attribute of the arrows-element is 'none', consider the edge not oriented from source to target
			sourceToTarget = not xmlArrows.attrib['target'] == 'none'
			# If source-attribute of the arrows-element is 'none', consider the edge not oriented from target to source
			targetToSource = not xmlArrows.attrib['source'] == 'none'
			# If no orientation is found of two orientations are found, create a bidirectional edge and save it
			if not sourceToTarget ^ targetToSource:
				self.ConnectNodes(nodeSource, nodeTarget, isBidirectional = True)
			# If only an orientation from source to target is found, create a unidirectional edge from source to target and save it
			elif sourceToTarget:
				self.ConnectNodes(nodeSource, nodeTarget, isBidirectional = False)
			# If only an orientation from target to source is found, create a unidirectional edge from target to source and save it (possible if user create a link in yEd from a node to another and change arrows style after)
			elif targetToSource:
				self.ConnectNodes(nodeTarget, nodeSource, isBidirectional = False)
			# Else, there is a bug in the matrix
			else:
				pass
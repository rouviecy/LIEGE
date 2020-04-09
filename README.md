# LIEGE

Librairie d'Import Export de Graphes Électroniques

## Dépendances

### Version Python

* Interpréteur Python
* Générateur de documentation Doxygen facultatif
* Module xml (si pas déjà embarqué dans l'interpréteur)
* GraphViz (uniquement le binaire appelable depuis la console, pas le module Python)

### Version C++

* CMake
* Compileur compatible minimum C++27
* Générateur de documentation Doxygen facultatif
* Librairie TinyXML embarquée
* GraphViz (uniquement le binaire appelable depuis la console, pas la librairie C++)

### Version C#

* à venir ...

### Version Java

* à venir ...

## Utilisation

### Notions à connaître

* Les noeuds sont matérialisés par des objets `Node`
* Les noeuds sont reliés via des objets `Edge`
* Chaque `Node` possède une liste de `Edge` correspondant aux liens sortants. Si un lien est bidirectionnel, alors il y a un autre `Edge` dans le sens opposé dans la liste des `Edge` de l'autre `Node`.
* En C++, si on ne souhaite pas gérer manuellement l'allocation mémoire et `Node` et `Edge`, il faut utiliser un objet `Graph` qui gère la mémoire. Pour l'instant pas besoin en Python, mais la version Python va être refaite pour proposer les mêmes fonctions que la version C++.
* Un `Node` peut être localisé via des coordonnées [X;Y] ou pas. Lors de l'export, on dit qu'un `Graph` est `free` si la position des noeuds est ignorée (GraphViz va les positionner lui-même à l'export) ou `fixed` si la position compte (par exemple donnée par yEd). Pour combiner les capacités de dessin de GraphViz avec le placement intelligent par yEd, on peut par exemple :
    1. Créer un graph manuellement par le code
    2. L'exporter en DOT sans les positions
    3. Convertir le fichier DOT en GraphML (il existe d'autres outils pour ça)
    4. Ouvrir le fichier GraphML dans yEd
    5. Demander à yEd de placer intelligemment les noeuds et exporter dans un nouveau fichier GraphML
    6. Importer le fichier GraphML et générer un fichier DOT/PNG/SVG avec les positions données par yEd

### Version Python

* Exemple d'utilisation dans `test.py`
    * Importer `LIEGE.py`
    * Instancier un objet `graph` depuis la classe `LIEGE.Graph`
    * Importer un fichier GraphML (par exemple généré par yEd) via `graph.ImportGraphml(...)`
    * Exporter vers un fichier DOT, une image PNG ou SVG via `graph.ExportDot(...)`
    * Manipuler le graphe depuis les fonctions disponibles sur l'objet `graph` (création/suppression d'un noeud/arrête, récupération de tous les noeuds, etc.)

* Fichiers exemples à lire ou générés dans `tests/in/` et `tests/out/`

### Version C++

* Exemple d'utilisation dans `src/test.cpp`
    * Importer `include/model/Graph.h`
    * Instancier un objet `graph` depuis la classe `Graph`
    * Importer un fichier GraphML (par exemple généré par yEd) via `graph.ImportGraphml(...)`
    * Exporter vers un fichier DOT, une image PNG ou SVG via `graph.ExportDot(...)`
    * Manipuler le graphe depuis les fonctions disponibles sur l'objet `graph` (création/suppression d'un noeud/arrête, récupération de tous les noeuds, etc.)

### Version C#

* à venir ...

### Version Java

* à venir ...

## TODO

* Versions C# et Java
* Refactoring de la versio Python pour imiter l'API C++
* Système d'héritage ou d'interface pour enrichir les `Node` et `Edge` par d'autres classes
* Exemple d'enrichissement par génération de graphes UML, etc.
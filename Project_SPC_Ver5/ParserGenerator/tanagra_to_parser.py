import sys
from os.path import exists
import functools
import tana2tree as t2t
import pandas as pd
import openpyxl
import xlrd


# Generates a list of all decision paths
def generatePathsRecursively(node, path, length, path_list):
    """
    Generates a list of the shortest paths from the root node to all leaves
    :param node: Root node of tree
    :param path: Current path being constructed
    :param length: Length of the current path
    :param path_list: A list of the shortest paths from root to leaves
    :return: Returns the list of paths
    """
    # Base case
    if node is None:
        return
    path[length] = node
    length += 1

    # DFS implementation
    if node.l_branch is None and node.r_branch is None:
        path_list.append(path.copy()[:length])
    else:
        generatePathsRecursively(node.l_branch, path, length, path_list)
        generatePathsRecursively(node.r_branch, path, length, path_list)

    return path_list


# Returns a list of all decision paths
def getPaths(node, numElements):
    """
    Driver function for generating all paths from root to leaves
    :param node: The root of the tree
    :return: Returns the list of paths
    """
    # initialize array equal to the length of all elements in tree as an upper bound to the length of paths
    path = [0] * numElements
    path_list = []
    path_list = generatePathsRecursively(node, path, 0, path_list)
    return path_list


# Custom comparator for sorting based on path length
def compare(path1, path2):
    """
    Custom comparator for sorting paths based on their length
    :param path1: First path to be compared
    :param path2: Second path to be compared
    :return: Returns the difference between the two.
    """
    return len(path1) - len(path2)


# Write the parser to a file
def writeParser(parser_contents, output_file):
    """
    Steps through the calculated parser elements and writes them to the outputfile
    :param parser_contents: Generated parser elements to be written
    :param output_file: The filepath of the output file
    :return:
    """
    parser_file = open(output_file, 'w', encoding="utf-8")
    for parser_el in parser_contents:
        parserStr = parser_el.toString()
        string_to_write = str(parserStr).strip("[]") + "\n"
        parser_file.write(string_to_write)


class ParserElement:
    """
    Class representing an element of a parser
    """

    def __init__(self, orig_labels=None):
        if orig_labels is None:
            orig_labels = dict()
        self.x1 = 0
        self.y1 = 0
        self.x2 = 1
        self.y2 = 1
        self.graphNum = -1
        self.classNum = -1
        self.x_attribute = ""
        self.y_attribute = ""
        self.orig_labels = orig_labels

    def copy(self):
        copyParser = ParserElement(self.orig_labels)
        copyParser.x1 = self.x1
        copyParser.y1 = self.y1
        copyParser.x2 = self.x2
        copyParser.y2 = self.y2
        copyParser.graphNum = self.graphNum
        copyParser.classNum = self.classNum
        copyParser.x_attribute = self.x_attribute
        copyParser.y_attribute = self.y_attribute
        return copyParser

    def getOrigXLabel(self):
        return self.orig_labels[self.x_attribute]

    def getOrigYLabel(self):
        return self.orig_labels[self.y_attribute]

    def toString(self):
        return str(self.x1) + ", " + str(self.y1) + ", " + str(self.x2) + ", " + str(self.y2) + ", " + \
               str(self.graphNum) + ", " + str(self.classNum) + ", " + self.orig_labels[self.x_attribute].replace(" ", "-") + ", " + \
               self.orig_labels[self.y_attribute].replace(" ", "-")


class ContinueElement(ParserElement):
    """
    A type of parser element that includes a destination plot.
    """

    def __init__(self):
        super().__init__()
        self.destinationPlotId = -1

    def copy(self):
        copy = super().copy()
        copyContEl = ContinueElement()
        copyContEl.x1 = copy.x1
        copyContEl.y1 = copy.y1
        copyContEl.x2 = copy.x2
        copyContEl.y2 = copy.y2
        copyContEl.graphNum = copy.graphNum
        copyContEl.classNum = copy.classNum
        copyContEl.x_attribute = copy.x_attribute
        copyContEl.y_attribute = copy.y_attribute
        copyContEl.orig_labels = copy.orig_labels
        return copyContEl

    def toString(self):
        return super().toString() + ", " + str(self.destinationPlotId)


def findPairs(path_list, classes, orig_labels):
    """
    Computes the optimal attribute pairs for use in parser generation
    :param path_list: The list of paths from root to leaves
    :param classes: A dictionary that maps class strings to class integers
    :param orig_labels: A dictionary of original labels used in the tree and their original values
    :return: A list of pairs, a dictionary of attribute pairs, a list of attributes that are paired
    """
    pairs = []
    pairMap = dict()
    usedAttributes = set()
    isAttributePaired = dict()
    for label in orig_labels:
        isAttributePaired[label] = False

    for path in path_list:
        # If path length is less than three, there is only one attribute: path[0]
        if len(path) < 3:
            pair = (path[0].attr, path[0].attr)
            pairs.append(pair)
            addOrAppend(path[0].attr, pair, pairMap)
            usedAttributes.add(path[0].attr)
            continue

        # Step through each pair of adjacent nodes from root
        for i in range(len(path) - 1):
            # Grab two adjacent nodes in path
            node1 = path[i]
            node2 = path[i + 1]

            # If both nodes are not class nodes
            if node1.attr not in classes and node2.attr not in classes:
                pair = (node1.attr, node2.attr)
                # If adjacent nodes are technically the same
                if orig_labels[pair[0]] == orig_labels[pair[1]]:
                    continue

                # if node1/node2 attributes are not paired
                if pair not in pairs and (node2.attr, node1.attr) not in pairs:
                    # node1 or node2 attributes have pairs
                    if isAttributePaired[pair[0]] or isAttributePaired[pair[1]]:
                        continue
                    pairs.append(pair)
                    addOrAppend(pair[0], pair, pairMap)
                    addOrAppend(pair[1], pair, pairMap)
                    isAttributePaired[node1.attr] = True
                    isAttributePaired[node2.attr] = True

    # For every unpaired attribute, pair it with itself.
    for attr in isAttributePaired:
        if not isAttributePaired[attr]:
            pair = (attr, attr)
            pairs.append(pair)
            addOrAppend(attr, pair, pairMap)
            isAttributePaired[attr] = True

    return pairs, pairMap, isAttributePaired


def addOrAppend(attr, pair, pairMap):
    """
    A helper function that appends a pair to the dictionary of pairs if it doesnt exist,
    and adds it to the list of pairs if it doesn't.
    :param attr: Attribute name
    :param pair: Pair belonging to attr
    :param pairMap: A dictionary of pairs. Format is dict[attribute_name] -> [pair1, pair2, ...]
    :return:
    """
    if attr in pairMap:
        if pair not in pairMap[attr]:
            pairMap[attr].append(pair)
    else:
        pairMap[attr] = [pair]


def generateAllDecisionElements(path_list, pairMap, node_map, plotIdMap, orig_labels,
                                classes, rootNode, maxMap):
    """
    Generate all decision elements present in current pair configuration.
    :param path_list: A list of all paths from root to leaves.
    :param pairMap: A dictionary of attribute names to pairs they belong to.
    :param node_map: A dictionary of all nodes in the tree.
    :param plotIdMap: A map holding the id for each paired coordinate plots.
    :param orig_labels: A dictionary holding the original label names.
    :param classes: A dictionary mapping class strings to class integers.
    :param rootNode: Root of the tree.
    :return: A list of decision elements.
    """

    # idea: for each path, take the last two elements and name them classNode and parentNode
    # find the pair of parentNode
    # this paired can include being paired to itself
    # create a parser local to that pair
    # adjust parser to any duplicates found higher in the tree
    # Special case : path < 3 elements long
    # this means that there is only one attribute. Find it's pair, then create a single attribute parserElement

    decisionElements = []
    for path in path_list:
        if len(decisionElements) == 5:
            print()
        # init variables that we will need to construct parser
        classNode = path[len(path) - 1]
        parentNode = path[len(path) - 2]

        # If the path is only two attributes long
        if len(path) < 3:
            # Generate single attribute parser elements
            singleAttributeElements = generateSingleAttributeParserElements(parentNode, classNode, classes, orig_labels,
                                                                            plotIdMap)
            # Adjust single attribute parser elements to fit within bounds
            for parser in singleAttributeElements:
                adjustedParserElements = adjustSingleAttributeParserElementBounds(parser, path, orig_labels, pairMap,
                                                                                  rootNode, plotIdMap)
                for adjustedParser in adjustedParserElements:
                    decisionElements.append(adjustedParser)
            continue

        # The attribute pair that the parent node belongs to
        parentNodePair = pairMap[parentNode.attr][0]
        # The node that the parent node is paired to
        parentNodePairedNode = node_map[parentNodePair[0]] if parentNode.attr != parentNodePair[0] \
            else node_map[parentNodePair[1]]
        # Is the parent node the X axis attribute in the pair
        isParentNodeXAxis = parentNode.attr == parentNodePair[0]
        parserElement = ParserElement(orig_labels)
        parserElement.classNum = classes[classNode.attr]
        parserElement.graphNum = getPlotId(parentNode.attr, parentNodePairedNode.attr, plotIdMap)
        parserElement.x_attribute = parentNodePair[0]
        parserElement.y_attribute = parentNodePair[1]

        # If the parent node and its pair have the same attribute, generate and adjust single attribute parser element
        if orig_labels[parentNode.attr] == orig_labels[parentNodePairedNode.attr]:
            singleAttributeElements = generateSingleAttributeParserElements(parentNode, classNode, classes, orig_labels,
                                                                            plotIdMap)
            for parser in singleAttributeElements:
                adjustedParserElements = adjustSingleAttributeParserElementBounds(parser, path, orig_labels, pairMap,
                                                                                  rootNode, plotIdMap)
                for adjustedParser in adjustedParserElements:
                    decisionElements.append(adjustedParser)
            continue

        # If parent node is on the X axis of its pair
        # The idea of this step is to adjust the bounds of the parser element space so that it fits with the parentNode and its pair
        if isParentNodeXAxis:
            if '<' in classNode.parent_op:
                # parserElement.x2 = parentNode.value / 10  # todo
                # parserElement.x2 = parentNode.value / maxMap[parserElement.getOrigXLabel()]  # todo
                parserElement.x2 = normalize(parentNode.value, parserElement.getOrigXLabel(), minMap, maxMap)
            else:
                # parserElement.x1 = parentNode.value / 10  # todo
                # parserElement.x1 = parentNode.value / maxMap[parserElement.getOrigXLabel()]  # todo
                parserElement.x1 = normalize(parentNode.value, parserElement.getOrigXLabel(), minMap, maxMap)
        else:
            if '<' in classNode.parent_op:
                # parserElement.y2 = parentNode.value / 10  # todo
                tmp = maxMap[parserElement.getOrigYLabel()]
                # parserElement.y2 = parentNode.value / maxMap[parserElement.getOrigYLabel()]  # todo
                parserElement.y2 = normalize(parentNode.value, parserElement.getOrigYLabel(), minMap, maxMap)
            else:
                # parserElement.y1 = parentNode.value / 10  # todo
                # parserElement.y1 = parentNode.value / maxMap[parserElement.getOrigYLabel()]  # todo
                parserElement.y1 = normalize(parentNode.value, parserElement.getOrigYLabel(), minMap, maxMap)
            if '<' in parentNode.parent_op:
                # parserElement.x2 = parentNodePairedNode.value / 10  # todo
                # parserElement.x2 = parentNodePairedNode.value / maxMap[parserElement.getOrigXLabel()]  # todo
                parserElement.x2 = normalize(parentNodePairedNode.value, parserElement.getOrigXLabel(), minMap, maxMap)
            else:
                # parserElement.x1 = parentNodePairedNode.value / 10  # todo
                # parserElement.x1 = parentNodePairedNode.value / maxMap[parserElement.getOrigXLabel()]  # todo
                parserElement.x1 = normalize(parentNodePairedNode.value, parserElement.getOrigXLabel(), minMap, maxMap)
        parserElement = adjustParserElementBounds(parserElement, path, orig_labels, node_map, pairMap)
        decisionElements.append(parserElement)
    return decisionElements


def generateSingleAttributeParserElements(parentNode, classNode, classes, orig_labels, plotIdMap):
    """
    Helper function to generate single attribute parser elements.
    :param parentNode: The node penultimate node of the path.
    :param classNode: The end node of the path which represented which class the path belongs to.
    :param classes: A dictionary of class name string to class integers.
    :param orig_labels: A diction of labels used during processing and original labels
    :param plotIdMap: A dictionary containing which pairs map to which plots
    :return: A list containing the single attribute parser elements.
    """
    singleAttributeParserElements = []
    parserElement = ParserElement(orig_labels)
    parserElement.x_attribute = parentNode.attr
    parserElement.y_attribute = parentNode.attr
    parserElement.classNum = classes[classNode.attr]
    parserElement.graphNum = getPlotId(parentNode.attr, parentNode.attr, plotIdMap)

    # There are two options for single attribute parsers. If the parser element calls for less than an attribute value,
    # or greater than.
    if '<' in classNode.parent_op:
        parserElement.x1 = 0
        parserElement.y1 = 0
        # parserElement.x2 = parentNode.value / maxMap[parserElement.getOrigXLabel()]  # todo
        parserElement.x2 = normalize(parentNode.value, parserElement.getOrigXLabel(), minMap, maxMap)
        # parserElement.y2 = parentNode.value / maxMap[parserElement.getOrigYLabel()]  # todo
        parserElement.y2 = normalize(parentNode.value, parserElement.getOrigYLabel(), minMap, maxMap)
        singleAttributeParserElements.append(parserElement)
    elif '>' in classNode.parent_op:  # todo: problem here somewhere
        parserElement.x1 = 0
        # parserElement.y1 = parentNode.value / maxMap[parserElement.getOrigYLabel()]  # todo
        parserElement.y1 = normalize(parentNode.value, parserElement.getOrigYLabel(), minMap, maxMap)
        parserElement.x2 = 1
        parserElement.y2 = 1

        parserElement2 = ParserElement(orig_labels)
        parserElement2.x_attribute = parentNode.attr
        parserElement2.y_attribute = parentNode.attr
        parserElement2.classNum = classes[classNode.attr]
        parserElement2.graphNum = getPlotId(parentNode.attr, parentNode.attr, plotIdMap)
        # parserElement2.x1 = parentNode.value / maxMap[parserElement.getOrigXLabel()]  # todo
        parserElement2.x1 = normalize(parentNode.value, parserElement2.getOrigXLabel(), minMap, maxMap)
        parserElement2.y1 = 0
        parserElement2.x2 = 1
        # parserElement2.y2 = parentNode.value / maxMap[parserElement.getOrigYLabel()]  # todo
        parserElement2.y2 = normalize(parentNode.value, parserElement2.getOrigYLabel(), minMap, maxMap)
        singleAttributeParserElements.append(parserElement)
        singleAttributeParserElements.append(parserElement2)

    return singleAttributeParserElements


def adjustSingleAttributeParserElementBounds(parserElement, path, orig_labels, pairMap, rootNode, plotIdMap):
    """
    Adjusts single attribute parser values so that they are within bounds of previous instances of attributein same path
    :param parserElement: The parser element to be adjusted.
    :param path: The path that the parser element belongs to.
    :param orig_labels: A dictionary of labels used during processing and original labels
    :param pairMap: A dictionary of attribute names to pairs they belong to
    :param rootNode: Root node of the tree.
    :param plotIdMap: A dictionary containing which pairs map to which plots.
    :return: The adjusted single attribute parsers
    """
    classNode = path[len(path) - 1]
    parentNode = path[len(path) - 2]
    returnParserElements = []
    # Step through the path in reverse starting at the parent node
    for i in reversed(range(len(path) - 2)):
        node2 = path[i]
        node1 = path[i + 1]
        if orig_labels[node2.attr] == orig_labels[parentNode.attr]:  # if node2 and the parent node are the same
            if parserElement.x1 == 0 and parserElement.y1 == 0:  # parserElement is a less than element
                # parserElement.y1 = node2.value / maxMap[parserElement.getOrigYLabel()]  # todo
                parserElement.y1 = normalize(node2.value, parserElement.getOrigYLabel(), minMap, maxMap)
                newParserEl = parserElement.copy()
                # newParserEl.x1 = node2.value / maxMap[newParserEl.getOrigXLabel()]  # todo
                newParserEl.x1 = normalize(node2.value, parserElement.getOrigXLabel(), minMap, maxMap)
                newParserEl.y1 = 0
                # newParserEl.y2 = node2.value / maxMap[newParserEl.getOrigYLabel()] # todo
                newParserEl.y2 = normalize(node2.value, parserElement.getOrigYLabel(), minMap, maxMap)
                returnParserElements.append(newParserEl)
                break
            # if the classNode and node1 are on opposite sides of the same parent attribute
            if '<' in classNode.parent_op and '<' not in node1.parent_op or '>' in classNode.parent_op and '>' not in node1.parent_op:
                if '<' in node1.parent_op and '<' not in classNode.parent_op:
                    # parserElement.x2 = node2.value / maxMap[parserElement.getOrigXLabel()]  # todo
                    parserElement.x2 = normalize(node2.value, parserElement.getOrigXLabel(), minMap, maxMap)
                    # parserElement.y2 = node2.value / maxMap[parserElement.getOrigYLabel()]  # todo
                    parserElement.y2 = normalize(node2.value, parserElement.getOrigYLabel(), minMap, maxMap)
                    break
                elif '>' in node1.parent_op and '<' not in classNode.parent_op:
                    # parserElement.x1 = node2.value / maxMap[parserElement.getOrigXLabel()]  # todo
                    parserElement.x1 = normalize(node2.value, parserElement.getOrigXLabel(), minMap, maxMap)
                    # parserElement.y1 = node2.value / maxMap[parserElement.getOrigYLabel()]  # todo
                    parserElement.y1 = normalize(node2.value, parserElement.getOrigYLabel(), minMap, maxMap)
                    break

    # if the node that is paired to itself is the root node, combine it with the root node
    if parserElement.x_attribute == rootNode.attr and len(pairMap) > 1:
        rootPair = pairMap[rootNode.attr][0]
        parserElement.x_attribute = rootPair[0]
        parserElement.y_attribute = rootPair[1]
        parserElement.graphNum = getPlotId(rootPair[0], rootPair[1], plotIdMap)
        # if root attr is X axis
        if rootNode.attr == rootPair[0]:
            # greater than. Parser element with y1 of 1 will always be a greater than element
            if parserElement.y1 == 1:
                parserElement.x1 = parserElement.y2
                parserElement.y1 = 0
                parserElement.x2 = 1
                parserElement.y2 = 1
            else:  # less than
                parserElement.x1 = 0
                parserElement.y1 = 0
                parserElement.x2 = parserElement.y2
                parserElement.y2 = 1
        else:  # if root attr is X axis
            if parserElement.y1 == 1:  # greater than
                parserElement.x1 = 0
                parserElement.y1 = parserElement.y2
                parserElement.x2 = 1
                parserElement.y2 = 1
            else:  # less than
                parserElement.x1 = 0
                parserElement.y1 = 0
                parserElement.x2 = 1
    returnParserElements.append(parserElement)
    return returnParserElements


def adjustParserElementBounds(parserElement, path, orig_labels, node_map, pairMap):
    """
    Adjusts parser values so that they are within bounds of previous instances of attributes in the same path.
    :param parserElement: The parser element to adjust.
    :param path: The path that the current parser element belongs to.
    :param orig_labels: A dictionary of labels used during processing and original labels.
    :param node_map: A dictionary of node attributes to their corresponding node in the tree.
    :param pairMap: A dictionary of attribute names to pairs they belong to.
    :return: The adjusted parser.
    """
    # if the parser element is a single attribute parser element
    if parserElement.x_attribute == parserElement.y_attribute:
        return adjustParserElementBounds(parserElement, path, orig_labels, node_map, pairMap)

    child = path[len(path) - 1]
    parent = path[len(path) - 2]
    parentPair = pairMap[parent.attr][0]
    isParentXAxis = parent.attr == parentPair[0]
    parentPairedNode = node_map[parentPair[0]] if parent.attr != parentPair[0] else node_map[parentPair[1]]

    #
    # if parent.attr == parentPairedNode.attr:  # todo : consider making this its own method
    #     # Special case, only one attribute pair
    #     child = parent
    #     for i in reversed(range(len(path) - 2)):
    #         node2 = path[i]
    #         node1 = path[i + 1]
    #         if orig_labels[node2.attr] == orig_labels[parent.attr]:
    #             if '<' in node1.parent_op and '<' not in child.parent_op:
    #                 parserElement.y2 = node2.value / 10  # todo
    #                 parserElement.x2 = node2.value / 10  # todo
    #                 break
    #             elif '>' in node1.parent_op and '>' not in child.parent_op:
    #                 parserElement.y1 = node1.value / 10  # todo
    #                 parserElement.x1 = node1.value / 10  # todo
    #                 break
    #     return parserElement

    # case: if parent is X of its pair
    if isParentXAxis:
        # step backwards through path starting from parent node
        for i in reversed(range(len(path) - 2)):
            node2 = path[i]
            node1 = path[i + 1]
            # If parent and node2 have the same attribute name
            if orig_labels[node2.attr] == orig_labels[parent.attr]:
                if '<' in node1.parent_op and '<' not in child.parent_op:
                    # parserElement.x2 = node2.value / maxMap[parserElement.getOrigXLabel()]  # todo
                    parserElement.x2 = normalize(node2.value, parserElement.getOrigXLabel(), minMap, maxMap)
                    break
                elif '>' in node1.parent_op and '>' not in child.parent_op:
                    # parserElement.x1 = node2.value / maxMap[parserElement.getOrigXLabel()]  # todo
                    parserElement.x1 = normalize(node2.value, parserElement.getOrigXLabel(), minMap, maxMap)
                    break
        return parserElement

    # since we caught one attribute pairs already, then the previous two before class node must be paired
    for i in reversed(range(len(path) - 2)):
        node2 = path[i]
        node1 = path[i + 1]
        # If parent and node2 have the same attribute name
        if orig_labels[node2.attr] == orig_labels[parent.attr]:
            if '<' in node1.parent_op and '<' not in child.parent_op:
                # parserElement.y2 = node2.value / maxMap[parserElement.getOrigYLabel()]  # todo
                parserElement.y2 = normalize(node2.value, parserElement.getOrigYLabel(), minMap, maxMap)
                break
            elif '>' in node1.parent_op and '>' not in child.parent_op:
                # parserElement.y1 = node1.value / maxMap[parserElement.getOrigYLabel()]  # todo
                parserElement.y1 = normalize(node1.value, parserElement.getOrigYLabel(), minMap, maxMap)
                break

    # do the previous step with the other attribute in the pair
    child = parent
    parent = path[len(path) - 3]
    for i in reversed(range(len(path) - 2)):
        node2 = path[i]
        node1 = path[i + 1]
        if orig_labels[node2.attr] == orig_labels[parent.attr]:
            if '<' in node1.parent_op and '<' not in child.parent_op:
                # parserElement.x2 = node2.value / maxMap[parserElement.getOrigXLabel()]  # todo
                parserElement.x2 = normalize(node2.value, parserElement.getOrigXLabel(), minMap, maxMap)
                break
            elif '>' in node1.parent_op and '>' not in child.parent_op:
                # parserElement.x1 = node1.value / maxMap[parserElement.getOrigXLabel()]  # todo
                parserElement.x1 = normalize(node1.value, parserElement.getOrigXLabel(), minMap, maxMap)
                break
    return parserElement


def adjustContinueParserElementBounds(parserElement, path, orig_labels, node_map):
    """
    A special case of the parser adjustment process. Adjusts continue elements specficially.
    :param parserElement: The original continue parser element to be adjusted.
    :param path: The path the parser element belongs to.
    :param orig_labels: A dictionary of labels used during processing and original labels.
    :param node_map: A dictionary of node attributes to their corresponding node in the tree.
    :return:
    """
    node1 = node_map[parserElement.x_attribute]
    node2 = node_map[parserElement.y_attribute]
    # check x attribute
    for i in reversed(range(len(path) - 1)):
        node3 = path[i]
        node4 = path[i + 1]
        if node3.attr == node1.attr:
            for j in range(i):
                node5 = path[j]
                if orig_labels[node5.attr] == orig_labels[node1.attr]:
                    if '>' in node3.parent_op and '>' not in node4.parent_op:
                        # parserElement.x1 = node5.value / maxMap[parserElement.getOrigXLabel()]  # todo
                        parserElement.x1 = normalize(node5.value, parserElement.getOrigXLabel(), minMap, maxMap)
                        break
                    elif '<' in node3.parent_op and '<' not in node4.parent_op:
                        # parserElement.x2 = node5.value / maxMap[parserElement.getOrigXLabel()]  # todo
                        parserElement.x2 = normalize(node5.value, parserElement.getOrigXLabel(), minMap, maxMap)
                        break

    # check y attribute
    for i in reversed(range(len(path) - 1)):
        node3 = path[i]
        node4 = path[i + 1]
        if node3.attr == node2.attr:
            for j in range(i):
                node5 = path[j]
                if orig_labels[node5.attr] == orig_labels[node2.attr]:
                    if '>' in node3.parent_op and '<' not in node4.parent_op:
                        # parserElement.y1 = node5.value / maxMap[parserElement.getOrigYLabel()]  # todo
                        parserElement.y1 = normalize(node5.value, parserElement.getOrigYLabel(), minMap, maxMap)
                        break
                    elif '>' in node3.parent_op and '>' not in node4.parent_op:
                        # parserElement.y2 = node5.value / maxMap[parserElement.getOrigYLabel()]  # todo
                        parserElement.y2 = normalize(node5.value, parserElement.getOrigYLabel(), minMap, maxMap)
                        break
    return parserElement


def generateAllContinueElements(path_list, pairMap, plotIdMap, orig_labels, node_map, maxMap):
    """
    Function that generates all continue elements present given the pairings in pairMap
    :param path_list: List of all paths from root to leaves.
    :param pairMap: A dictionary of attribute names to pairs they belong to.
    :param plotIdMap: A dictionary containing which pairs map to which plots.
    :param orig_labels: A dictionary of labels used during processing and original labels.
    :param node_map: A dictionary of node attributes to their corresponding node in the tree.
    :return: Generates a list of all continue parser elements.
    """
    # traverse through longest path
    # any pair that has memebers of two different pairs, we add a decision node
    continueElements = []
    seen_decisions = []
    parentPairMap = dict()
    for j in range(len(path_list)):
        path = path_list[j]
        for i in range(0, len(path) - 2, 1):
            node1 = path[i]
            node2 = path[i + 1]
            pair = (node1.attr, node2.attr)

            # if nodes are not paired together, add a decision zone
            if not attrInPairList(pair[0], pairMap[pair[1]]) and not attrInPairList(pair[1], pairMap[pair[0]]):
                parserElement = ContinueElement()
                parserElement.orig_labels = orig_labels

                # todo: there is room for optimization here
                node1PairedAttr = pairMap[node1.attr][0][1] if pairMap[node1.attr][0][0] == node1.attr else \
                    pairMap[node1.attr][0][0]
                isNode1AttrX = True
                if node1.attr == pairMap[pair[0]][0][1]:
                    isNode1AttrX = False
                    node1PairedAttr = pairMap[node1.attr][0][0]
                # end optimization zone

                if isNode1AttrX:
                    parserElement.x_attribute = node1.attr
                    parserElement.y_attribute = node1PairedAttr
                else:
                    parserElement.y_attribute = node1.attr
                    parserElement.x_attribute = node1PairedAttr

                if isNode1AttrX:
                    if "<" in node2.parent_op:
                        # parserElement.x2 = node1.value /  maxMap[parserElement.getOrigXLabel()]  # TODO: parameterize
                        parserElement.x2 = normalize(node1.value, parserElement.getOrigXLabel(), minMap, maxMap)
                    elif ">" in node2.parent_op:
                        # parserElement.x1 = node1.value / maxMap[parserElement.getOrigXLabel()]
                        parserElement.x1 = normalize(node1.value, parserElement.getOrigXLabel(), minMap, maxMap)
                else:
                    if "<" in node2.parent_op:
                        # parserElement.y2 = node1.value / maxMap[parserElement.getOrigYLabel()]  # TODO: parameterize
                        parserElement.y2 = normalize(node1.value, parserElement.getOrigYLabel(), minMap, maxMap)
                    elif ">" in node2.parent_op:
                        # parserElement.y1 = node1.value / maxMap[parserElement.getOrigYLabel()]
                        parserElement.y1 = normalize(node1.value, parserElement.getOrigYLabel(), minMap, maxMap)
                    # special case, we need to include X of node1 pair as well
                    # debug for wine data
                    if i > 0:
                        # root -> ... -> node3 -> node1 -> node2 -> .. -> classNode
                        node3 = path[i - 1]
                        if "<" in node1.parent_op:
                            # parserElement.x2 = node3.value / maxMap[parserElement.getOrigXLabel()]
                            parserElement.x2 = normalize(node3.value, parserElement.getOrigXLabel(), minMap, maxMap)
                        elif ">" in node1.parent_op:
                            # parserElement.x1 = node3.value / maxMap[parserElement.getOrigXLabel()]
                            parserElement.x1 = normalize(node3.value, parserElement.getOrigXLabel(), minMap, maxMap)
                        else:
                            print("debug")  # todo
                graphNum = getPlotId(node1.attr, node1PairedAttr, plotIdMap)
                # if isNode1AttrX:
                #     parserElement.x_attribute = node1.attr
                #     parserElement.y_attribute = node1PairedAttr
                # else:
                #     parserElement.y_attribute = node1.attr
                #     parserElement.x_attribute = node1PairedAttr

                parserElement.graphNum = graphNum

                parentPair = (parserElement.x_attribute, parserElement.y_attribute)

                if parentPair not in parentPairMap:
                    parentPairMap[parentPair] = {pair: -1}
                elif pair not in parentPairMap[parentPair]:
                    parentPairMap[parentPair][pair] = -len(parentPairMap[parentPair]) - 1

                parserElement.classNum = parentPairMap[parentPair][pair]
                destinationPair = pairMap[node2.attr][0]
                destinationGraphId = getPlotId(destinationPair[0], destinationPair[1], plotIdMap)
                parserElement.destinationPlotId = destinationGraphId

                if parserElement.x1 > 1 or parserElement.y1 > 1 or parserElement.x2 > 1 or parserElement.y2 > 1:
                    print("debug")

                parserElement = adjustContinueParserElementBounds(parserElement, path, orig_labels, node_map)

                if parserElement.x1 > 1 or parserElement.y1 > 1 or parserElement.x2 > 1 or parserElement.y2 > 1:
                    print("debug")

                if parserElement.toString() not in seen_decisions:
                    continueElements.append(parserElement)
                    seen_decisions.append(parserElement.toString())

    return continueElements


def attrInPairList(attr, pair_list):
    """
    Helper function that tests if a attribute exists as part of a pair.
    :param attr: The attribute being tested.
    :param pair_list: The list of all attribute pairs.
    :return: A boolean representing whether or not an attribute is in the pair_list.
    """
    for pair in pair_list:
        if attr in pair:
            return True
    return False


def getGraphIdMap(pair_list, root):
    """
    Assigns each pair to a plot number.
    :param pair_list: The list of all pairs.
    :param root: The root node of the tree.
    :return: A dictionary of attribute pairs to their plot id.
    """
    graphId = 1
    # TODO: Kind of funny, think about optimizing
    plotIdMap = dict()
    for pair in pair_list:
        if root in pair:
            plotIdMap[pair] = 0
        elif pair in plotIdMap:
            graphId += 1
        else:
            plotIdMap[pair] = graphId
            graphId += 1
    return plotIdMap


def getPlotId(attr1, attr2, plotIdMap):
    """
    Gets the plot id for two attributes in a pair, regardless of their order.
    :param attr1: The first attribute pair memember.
    :param attr2: The first attribute pair memember.
    :param plotIdMap: A dictionary of attribute pairs to their plot id.
    :return: The plot id of the given pair
    """
    plotId = -1
    if (attr1, attr2) in plotIdMap:
        plotId = plotIdMap[(attr1, attr2)]
    elif (attr2, attr1) in plotIdMap:
        plotId = plotIdMap[(attr2, attr1)]
    else:
        print("undefined")
    return plotId


def replaceAttributeNames(node_list, orig_labels):
    """
    Helper function to replace all attribute names in node_list with those of the original labels.
    :param node_list: A list of nodes in the tree.
    :param orig_labels: A dictionary of labels used during processing and original labels.
    :return: The list of nodes that have had their attributes renamed.
    """
    for node in node_list:
        node.attr = orig_labels[node.attr]
    return node_list

def normalize(value, attr, minMap, maxMap):
    result = (value - minMap[attr]) / (maxMap[attr] - minMap[attr])
    if result > 1:
        print("debug")
    return result

# debug
class Node:
    def __init__(self, attr):
        self.attr = attr
        self.l_branch = None
        self.op = None
        self.parent_op = '>='
        self.r_branch = None
        self.value = None


def generateParser(input_file, output_file, minMap, maxMap, classList):
    """
    Main function. Returns the parser elements based on the input file.
    :param input_file: Filepath of input file.
    :param output_file: Filepath of output file.
    :return:
    """
    if not exists(input_file):
        print("Error. Could not find file:", input_file, "in project directory")
        exit()

    tree = t2t.Tanagra_Parser()
    tree.parse(input_file)  # todo : parameterize
    node_list_before = tree.traverse()
    # debug for iris
    # node = tree.get_node("sw")
    # addNode = t2t.t.Tree_Node(node[0].attr, ">=", "Iris-versicolor_1", None, None)
    # node[0].r_branch = addNode
    # end debug
    # tree.print_tree()
    # TODO: Why is tree.parse() not getting all nodes?
        # TODO: Figured it out!! Separate attributes CANNOT begin with the same two letters
    node_list = tree.traverse()
    tree.print_tree()

    # debug for breast training
    # node = tree.get_node("uc")
    # new0 = t2t.t.Tree_Node(node[0].attr, ">=", "uc_1", "<", 4.5)
    # new1 = t2t.t.Tree_Node(new0.attr, "<", "bn_1", "<", 3.5)
    # new0.l_branch = new1
    # new2 = t2t.t.Tree_Node(node[0].attr, ">=", "cl_2", "<", 6.5)
    # new0.r_branch = new2
    # new3 = t2t.t.Tree_Node(new1.attr, "<", "no_2", "<", 2.5)
    # new1.l_branch = new3
    # new4 = t2t.t.Tree_Node(new1.attr, ">=", "")





    # TODO: Determine number of classes
    for node in node_list:
        for className in classList:
            if className in node.attr:
                node.attr = className
                break

    strippedLabels = tree.orig_labels.copy()
    for label in tree.orig_labels:
        for className in classList:
            if className in label:
                strippedLabels.pop(label)
    tree.orig_labels = strippedLabels

    classes = dict()
    counter = 0
    for node in node_list:
        if node.op is None and node.attr not in classes:
            classes[node.attr] = counter
            counter += 1

    # build node map
    nodeMap = dict()
    for node in node_list:
        if node.attr not in classes:
            nodeMap[node.attr] = node

    path_list = getPaths(tree.root, len(node_list))
    path_list = sorted(path_list, key=functools.cmp_to_key(compare), reverse=True)
    parserElements = []

    # pairs = findPairs(path_list, classes, tree.orig_labels)
    pairs, pairMap, isAttributePairedList = findPairs(path_list, classes, tree.orig_labels)
    graphIdMap = getGraphIdMap(pairs, tree.root.attr)

    continueElements = generateAllContinueElements(path_list, pairMap, graphIdMap, tree.orig_labels, nodeMap, maxMap)
    decisionElements = generateAllDecisionElements(path_list, pairMap, nodeMap,
                                                   graphIdMap,
                                                   tree.orig_labels, classes, tree.root, maxMap)

    for el in continueElements:
        parserElements.append(el)

    for el in decisionElements:
        parserElements.append(el)

    # ensure that plot nums are sequential
    seenPlotIds = []
    for el in parserElements:
        plotId = el.graphNum
        if plotId not in seenPlotIds:
            seenPlotIds.append(plotId)
    plotMap = dict()
    for i in range(len(seenPlotIds)):
        plotMap[seenPlotIds[i]] = i
    for el in parserElements:
        el.graphNum = plotMap[el.graphNum]
        if hasattr(el, "destinationPlotId"):
            el.destinationPlotId = plotMap[el.destinationPlotId]

    print(plotMap)

    writeParser(parserElements, output_file)
    stringToReturn = ""
    for parserElement in parserElements:
        stringToReturn += parserElement.toString() + '\n'
    return stringToReturn


def tests(fileName, output):
    """
    Tests for a selection of decision trees.
    :param fileName: The name of the file the parser generated belongs to.
    :param output: The output of the parser generation process.
    :return: None
    """
    print()
    # print("Testing:", file, "with output:\n", output)
    if fileName == "./TestTanagraOutputs/two_attribute_tree.txt":
        correctAnswer = ["0.25, 0, 1, 0.65, 0, 1, ucellsize, clump", "0.25, 0.65, 1, 1, 0, 1, ucellsize, clump",
                         "0, 0, 0.25, 1, 0, 0, ucellsize, clump"]
        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(fileName, "TEST PASSED")
        else:
            print(fileName, "NOT PASSED")
    elif fileName == "./TestTanagraOutputs/one_attribute_tree.txt":
        correctAnswer = [
            "0, 0, 0.25, 0.25, 0, 0, ucellsize, ucellsize",
            "0, 0.25, 1, 1, 0, 1, ucellsize, ucellsize",
            "0.25, 0, 1, 0.25, 0, 1, ucellsize, ucellsize"
        ]
        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(fileName, "TEST PASSED")
        else:
            print(fileName, "NOT PASSED")
    elif fileName == "./TestTanagraOutputs/two_attribute_tree_2.txt":
        correctAnswer = [
            "0.25, 0, 1, 1, 0, -1, ucellsize, sepics, 1",
            "0, 0, 0.25, 0.25, 0, 0, ucellsize, sepics",
            "0, 0.25, 0.25, 1, 0, 0, ucellsize, sepics",
            "0.25, 0, 0.45, 0.25, 1, 1, ucellsize, ucellsize",
            "0, 0.25, 0.45, 0.45, 1, 1, ucellsize, ucellsize",
            "0, 0.45, 1, 1, 1, 1, ucellsize, ucellsize",
            "0.45, 0, 1, 0.45, 1, 1, ucellsize, ucellsize"
        ]
        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(fileName, "TEST PASSED")
        else:
            print(fileName, "NOT PASSED")
    elif fileName == "./TestTanagraOutputs/three_attribute_tree_two_duplicates.txt":
        correctAnswer = [
            "0, 0, 0.25, 0.45, 0, -1, ucellsize, bnuclei, 2",
            "0.25, 0, 1, 1, 0, -2, ucellsize, bnuclei, 1",
            "0, 0, 0.65, 0.65, 2, 0, clump, clump",
            "0.65, 0, 1, 0.65, 2, 1, clump, clump",
            "0, 0.65, 1, 1, 2, 1, clump, clump",
            "0.45, 0, 1, 0.65, 1, 1, ucellsize, clump",
            "0.45, 0.65, 1, 1, 1, 1, ucellsize, clump",
            "0, 0.45, 0.25, 1, 0, 1, ucellsize, bnuclei",
            "0.25, 0, 0.45, 1, 1, 1, ucellsize, clump"
        ]

        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(fileName, "TEST PASSED")
        else:
            print(fileName, "NOT PASSED")
    elif fileName == "./TestTanagraOutputs/three_attribute_tree_one_duplicate.txt":
        correctAnswer = [
            "0, 0, 0.25, 0.45, 0, -1, ucellsize, bnuclei, 1",
            "0.25, 0, 1, 1, 0, -2, ucellsize, bnuclei, 2",
            "0, 0, 0.65, 0.65, 1, 0, clump, clump",
            "0.65, 0, 1, 0.65, 1, 1, clump, clump",
            "0, 0.65, 1, 1, 1, 1, clump, clump",
            "0, 0.45, 0.25, 1, 0, 1, ucellsize, bnuclei",
            "0.25, 0, 0.45, 0.25, 2, 1, ucellsize, ucellsize",
            "0, 0.25, 0.45, 0.45, 2, 1, ucellsize, ucellsize",
            "0.45, 0, 1, 0.45, 2, 1, ucellsize, ucellsize",
            "0, 0.45, 1, 1, 2, 1, ucellsize, ucellsize"
        ]
        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(fileName, "TEST PASSED")
        else:
            print(fileName, "NOT PASSED")

    elif fileName == "./TestTanagraOutputs/medium_tree.txt":
        correctAnswer = [
            "0.25, 0, 1, 1, 0, -1, ucellsize, bnuclei, 1",
            "0.45, 0, 1, 0.65, 1, -1, ucellsize, clump, 2",
            "0, 0.45, 0.85, 1, 2, -1, bnuclei, bchromatin, 8",
            "0.25, 0, 0.45, 1, 1, -2, ucellsize, clump, 3",
            "0.25, 0, 1, 0.65, 3, -1, bnuclei, clump, 6",
            "0.25, 0.65, 1, 1, 3, -2, bnuclei, clump, 7",
            "0, 0, 0.25, 1, 3, -3, bnuclei, clump, 5",
            "0, 0, 0.25, 0.45, 0, -2, ucellsize, bnuclei, 4",
            "0, 0, 0.85, 0.85, 8, 1, mgadhesion, mgadhesion",
            "0, 0.85, 1, 1, 8, 1, mgadhesion, mgadhesion",
            "0.85, 0, 1, 0.85, 8, 1, mgadhesion, mgadhesion",
            "0, 0, 0.35, 0.35, 6, 0, bchromatin, bchromatin",
            "0, 0.35, 1, 1, 6, 1, bchromatin, bchromatin",
            "0.35, 0, 1, 0.35, 6, 1, bchromatin, bchromatin",
            "0, 0, 0.7, 0.7, 7, 1, normnucl, normnucl",
            "0, 0.7, 1, 1, 7, 1, normnucl, normnucl",
            "0.7, 0, 1, 0.7, 7, 1, normnucl, normnucl",
            "0, 0, 0.85, 0.45, 2, 0, bnuclei, bchromatin",
            "0, 0, 0.25, 0.25, 5, 0, normnucl, normnucl",
            "0, 0.25, 1, 1, 5, 0, normnucl, normnucl",
            "0.25, 0, 1, 0.25, 5, 0, normnucl, normnucl",
            "0.85, 0, 1, 1, 2, 1, bnuclei, bchromatin",
            "0, 0, 0.65, 0.65, 4, 0, clump, clump",
            "0, 0.65, 1, 1, 4, 1, clump, clump",
            "0.65, 0, 1, 0.65, 4, 1, clump, clump",
            "0.45, 0.65, 1, 1, 1, 1, ucellsize, clump",
            "0, 0.45, 0.25, 1, 0, 1, ucellsize, bnuclei"
        ]
        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(fileName, "TEST PASSED")
        else:
            print(fileName, "NOT PASSED")

def computeMinMax(data):
    minMap = dict()
    maxMap = dict()
    for columnName in data.columns:
        for value in data[columnName]:
            if columnName not in minMap:
                minMap[columnName] = value
            else:
                if value < minMap[columnName]:
                    minMap[columnName] = value
            if columnName not in maxMap:
                maxMap[columnName] = value
            else:
                if value > maxMap[columnName]:
                    maxMap[columnName] = value
    return minMap, maxMap

if __name__ == '__main__':
    """
    Main function
    """
    # test mode
    isTestingMode = False
    if isTestingMode:
        filesToTest = [
            "./TestTanagraOutputs/two_attribute_tree.txt",
            "./TestTanagraOutputs/one_attribute_tree.txt",
            "./TestTanagraOutputs/two_attribute_tree_2.txt",
            "./TestTanagraOutputs/three_attribute_tree_two_duplicates.txt",
            "./TestTanagraOutputs/three_attribute_tree_one_duplicate.txt",
            "./TestTanagraOutputs/medium_tree.txt"
        ]
        sys.argv.append("")
        sys.argv.append("debug.txt")
        for file in filesToTest:
            sys.argv[1] = file
            if len(sys.argv) < 3:
                print("Please call this script with the following syntax: python tanagra_to_parser.py <input_file> "
                      "<output_file> <dataset>")
            else:
                output = generateParser(sys.argv[1], sys.argv[2])
                tests(file, output)
    else:
        if len(sys.argv) < 4:
            print("Please call this script with the following syntax: python tanagra_to_parser.py <input_file> "
                  "<output_file> <dataset>")
        else:
            data = pd.read_excel(sys.argv[3])
            className = data.columns[len(data.columns) - 1]
            classes = data[className]
            classList = []
            for classVal in classes:
                if classVal not in classList:
                    classList.append(classVal)
            # compute min/Max of each column in dataset

            tree = t2t.Tanagra_Parser()
            tree.parse(sys.argv[1])
            node_list_debug = tree.traverse()
            orig_labels = tree.orig_labels
            short_labels = dict()
            minMap, maxMap = computeMinMax(data)
            output = generateParser(sys.argv[1], sys.argv[2], minMap, maxMap, classList)

import copy
import sys
from os.path import exists
import functools
import tana2tree as t2t


# Generates a list of all decision paths
def generatePathsRecursively(node, path, length, path_list):
    if node is None:
        return
    path[length] = node
    length += 1

    if node.l_branch is None and node.r_branch is None:
        path_list.append(path.copy()[:length])
    else:
        generatePathsRecursively(node.l_branch, path, length, path_list)
        generatePathsRecursively(node.r_branch, path, length, path_list)

    return path_list


# Returns a list of all decision paths
def printPaths(node):
    path = [0] * 1000
    path_list = []
    path_list = generatePathsRecursively(node, path, 0, path_list)
    return path_list


# Custom comparator for sorting based on path length
def compare(item1, item2):
    return len(item1) - len(item2)


# Write the parser to a file
def writeParser(parser_contents, orig_labels, output_file):
    parser_file = open(output_file, 'w', encoding="utf-8")
    for parser_el in parser_contents:
        parserStr = parser_el.toString()
        string_to_write = str(parserStr).strip("[]") + "\n"
        parser_file.write(string_to_write)


class ParserElement:

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

    def toString(self):
        return str(self.x1) + ", " + str(self.y1) + ", " + str(self.x2) + ", " + str(self.y2) + ", " + \
               str(self.graphNum) + ", " + str(self.classNum) + ", " + self.orig_labels[self.x_attribute] + ", " + \
               self.orig_labels[self.y_attribute]


class ContinueElement(ParserElement):
    def __init__(self):
        super().__init__()
        self.destinationGraphId = -1

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
        return super().toString() + ", " + str(self.destinationGraphId)


def findPairs(path_list, classes, orig_labels):
    # pair is (x_attr, y_attr)
    pairs = dict()
    path_list = sorted(path_list, key=functools.cmp_to_key(compare), reverse=True)
    attributeSet = set()
    usedAttributeSet = set()

    debugPairList = []

    for path in path_list:
        # special case: path is only two nodes long
        # pair up the attribute in path[0] with itself
        if len(path) < 3 and len(pairs) == 0:
            pair = (path[0].attr, path[0].attr)
            pairs[path[0].attr] = pair
            usedAttributeSet.add(pair[0])
            debugPairList.append(pair)
            continue

        # for j in range(len(path) - 1, 0, -1):
        for j in range(len(path) - 1):
            node1 = path[j]
            node2 = path[j + 1]
            # node2 = path[j - 1]

            # Don't pair up similar nodes unless at the end
            if orig_labels[node1.attr] == orig_labels[node2.attr]:
                continue

            if node1.attr not in classes and node2.attr not in classes:
                if node1.attr not in attributeSet:
                    attributeSet.add(node1.attr)
                if node2.attr not in attributeSet:
                    attributeSet.add(node2.attr)
                if node1.attr not in pairs and node2.attr not in pairs:
                    pair = (node1.attr, node2.attr)
                    pairs[node1.attr] = pair
                    pairs[node2.attr] = pair
                    debugPairList.append(pair)
                    # pairs[node1.attr] = node2.attr
                    # pairs[node2.attr] = node1.attr
                    usedAttributeSet.add(node1.attr)
                    usedAttributeSet.add(node2.attr)
                else:
                    print("debug")
                if node1.attr in attributeSet or node2.attr in attributeSet:
                    continue
    # if there are unpaired attributes, pair with itself
    if len(attributeSet) != len(usedAttributeSet):
        unusedAttributes = attributeSet.difference(usedAttributeSet)  # todo: pair these up with themselves
        for attribute in unusedAttributes:
            pair = (attribute, attribute)
            pairs[attribute] = pair

    return pairs


def findPairsV2(path_list, classes, orig_labels):
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

                if pair not in pairs and (node2.attr, node1.attr) not in pairs:
                    if isAttributePaired[pair[0]] or isAttributePaired[pair[1]]:
                        continue

                    pairs.append(pair)
                    addOrAppend(pair[0], pair, pairMap)
                    addOrAppend(pair[1], pair, pairMap)
                    isAttributePaired[node1.attr] = True
                    isAttributePaired[node2.attr] = True
                # else:
                #     # print(pair)

    for attr in isAttributePaired:
        if not isAttributePaired[attr]:
            pair = (attr, attr)
            pairs.append(pair)
            addOrAppend(attr, pair, pairMap)
            isAttributePaired[attr] = True

    # print(pairs)
    return pairs, pairMap, isAttributePaired


def addOrAppend(attr, pair, pairMap):
    if attr in pairMap:
        if pair not in pairMap[attr]:
            pairMap[attr].append(pair)
    else:
        pairMap[attr] = [pair]


def generatePairs(path_list, classes, orig_labels):
    path_list = sorted(path_list, key=functools.cmp_to_key(compare), reverse=True)


#
# def generateAllDecisionElements(path_list, pairs, pairMap, node_map, isAttributePairedList, graphIdMap, orig_labels,
#                                 classes):
#     decision_elements = []
#     pathDebug = False
#     for i in range(len(path_list)):
#         path = path_list[i]
#         if pathDebug: print("\t\tstart path")
#         # special case, if the path is less than three nodes long
#         if len(path) < 3:
#             if pathDebug: print("path < 3")
#             # TODO
#             node1 = path[0]
#             node2 = path[1]
#             pair = pairMap[node1.attr][0]  # TODO: will this ever be larger than one?
#             graphId = graphIdMap[pair]
#             classNum = classes[node2.attr]
#             if pair[0] == pair[1]:
#                 if pathDebug: print("Path pair[0] == [1]")
#                 if '<' in node2.parent_op:
#                     if pathDebug: print("path < in node2")
#                     parserElement1 = ParserElement()
#                     parserElement1.orig_labels = orig_labels
#                     parserElement1.x2 = node1.value / 10.0  # todo: parameterize
#                     parserElement1.y2 = node1.value / 10.0  # todo: parameterize
#                     parserElement1.x_attribute = pair[0]
#                     parserElement1.y_attribute = pair[1]
#                     parserElement1.graphNum = graphId
#                     parserElement1.classNum = classNum
#
#                     parserElement2 = ParserElement()
#                     parserElement2.orig_labels = orig_labels
#                     parserElement2.x1 = node1.value / 10.0
#                     parserElement2.y2 = node1.value / 10.0
#                     parserElement2.x_attribute = pair[0]
#                     parserElement2.y_attribute = pair[1]
#                     parserElement2.graphNum = graphId
#                     parserElement2.classNum = classNum
#
#                     parserElement3 = ParserElement()
#                     parserElement3.orig_labels = orig_labels
#                     parserElement3.y1 = node1.value / 10.0
#                     parserElement3.x2 = node1.value / 10.0
#                     parserElement3.x_attribute = pair[0]
#                     parserElement3.y_attribute = pair[1]
#                     parserElement3.graphNum = graphId
#                     parserElement3.classNum = classNum
#
#                     parserElement1 = adjustParserElementBounds(parserElement1, path, orig_labels, node_map)
#                     parserElement2 = adjustParserElementBounds(parserElement2, path, orig_labels, node_map)
#                     parserElement3 = adjustParserElementBounds(parserElement3, path, orig_labels, node_map)
#                     decision_elements.append(parserElement1)
#                     decision_elements.append(parserElement2)
#                     decision_elements.append(parserElement3)
#                     if pathDebug: print("append:", parserElement1.toString())
#                     if pathDebug: print("append:", parserElement2.toString())
#                     if pathDebug: print("append:", parserElement3.toString())
#                 elif '>' in node2.parent_op:
#                     if pathDebug: print("path > in node2")
#                     parserElement = ParserElement()
#                     parserElement.orig_labels = orig_labels
#                     parserElement.x1 = node1.value / 10.0  # todo: parameterize
#                     parserElement.y1 = node1.value / 10.0  # todo: parameterize
#                     parserElement.x_attribute = pair[0]
#                     parserElement.y_attribute = pair[1]
#                     parserElement.y_attribute = pair[1]
#                     parserElement.graphNum = graphId
#                     parserElement.classNum = classNum
#                     parserElement = adjustParserElementBounds(parserElement, path, orig_labels, node_map)
#                     decision_elements.append(parserElement)
#                     if pathDebug: print("append:", parserElement.toString())
#             else:
#                 if pathDebug: print("path pair[0] != pair[1]")
#                 pair = pairMap[node1.attr][0]
#                 isNode1X = (node1.attr == pair[0])
#                 parserElement = ParserElement()
#                 parserElement.orig_labels = orig_labels
#                 parserElement.classNum = classes[node2.attr]
#                 parserElement.graphNum = graphIdMap[pair]
#                 parserElement.x_attribute = pair[0]
#                 parserElement.y_attribute = pair[1]
#                 if '<' in node2.parent_op:
#                     if pathDebug: print("path < in node2")
#                     if isNode1X:
#                         parserElement.x2 = node1.value / 10
#                     else:
#                         parserElement.y2 = node1.value / 10  # todo : parameterize
#                 elif '>' in node2.parent_op:
#                     if pathDebug: print("path > in node2")
#                     if isNode1X:
#                         parserElement.x1 = node1.value / 10
#                     else:
#                         parserElement.y1 = node1.value / 10  # todo : parameterize
#                 parserElement = adjustParserElementBounds(parserElement, path, orig_labels, node_map)
#                 decision_elements.append(parserElement)
#                 if pathDebug: print("append:", parserElement.toString())
#             if pathDebug: print("End path")
#             continue
#
#         # root -> ... -> node2 -> node1 -> classNode
#         classNode = path[len(path) - 1]
#         node1 = path[len(path) - 2]
#         node2 = path[len(path) - 3]
#         parserElement = ParserElement()
#         parserElement.orig_labels = orig_labels
#
#         if 'uc' in node1.attr and 'uc' in node2.attr:
#             if pathDebug: print("debug")
#
#         # Case 1: node1 and node2 are the same attribute
#         if orig_labels[node1.attr] == orig_labels[node2.attr]:
#             if pathDebug: print("node1 attr == node2 attr names")
#             pair = pairMap[node1.attr][0]
#             isNode1X = node1.attr == pair[0]
#             parserElement.x_attribute = pair[0]
#             parserElement.y_attribute = pair[1]
#             parserElement.graphNum = getGraphNum(parserElement.x_attribute, parserElement.y_attribute, graphIdMap)
#             parserElement.classNum = classes[classNode.attr]
#
#             # # if attributes are not paired (but are still the same)
#             # if (node1.attr, node2.attr) not in pairMap[node1.attr] and (node2.attr, node1.attr) not in pairMap[node1.attr]:
#             #     if pathDebug: print("attributes the same, but not paired together")
#             #     if '<' in classNode.parent_op:
#             #         parserElement.x1 = node2.value / 10 # todo
#             #         parserElement.y1 = 0
#             #         parserElement.x2 = node1.value / 10 # todo
#             #         parserElement.y2 = 1
#             #     #elif '>' in classNode.parent_op
#
#             # if isNode1X:
#             # if classNode parent operation is different from node1 parent op
#             if node1.parent_op not in classNode.parent_op or classNode.parent_op not in node1.parent_op:
#                 if pathDebug: print("classNode parent op and node1 parent op not the same")
#                 # if these attributes are paired together, then we possibly need two parserElements
#                 if orig_labels[node2.attr] not in map(lambda x: orig_labels[x], pair):  # if they are paired
#                     # todo
#                     if pathDebug: print("node1 and node2 are the same but not paired")
#                     if '>' in node1.parent_op:
#                         parserElement.x1 = node2.value / 10  # todo
#                         parserElement.y1 = 0
#                         if '<' in classNode.parent_op:
#                             parserElement.x2 = node1.value / 10  # todo
#                             parserElement.y2 = 1
#                         elif '>' in classNode.parent_op:
#                             print("I think already covered debug")
#                 # if they are not paired together,
#                 # TODO: may need another case here
#                 elif '>' in node1.parent_op:
#                     if pathDebug: print("node1 parent op >")
#                     node1PairedNode = node_map[pair[0]] if node1.attr == pair[1] else node_map[pair[1]]
#                     parserElement.x1 = node2.value / 10  # todo
#                     parserElement.y1 = 0  # todo
#                     parserElement.x2 = node1.value / 10  # todo
#                     if not isNode1X:
#                         parserElement.y2 = node1PairedNode.value / 10  # todo
#                     # parserElement.x1 = 0 # todo : tentative accept
#                     # parserElement.y1 = node2.value / 10  # todo
#                     # # parserElement.x1 = node2.value
#                     # # parserElement.y1 = 0
#                     # parserElement.x2 = node1.value / 10  # todo
#                     # parserElement.y2 = node1.value / 10  # todo
#                     # # parser will be appended at the end
#                     # parserElement2 = ParserElement(orig_labels)
#                     # parserElement2.x_attribute = pair[0]
#                     # parserElement2.y_attribute = pair[1]
#                     # parserElement2.graphNum = getGraphNum(parserElement.x_attribute, parserElement.y_attribute,
#                     #                                       graphIdMap)
#                     # parserElement2.classNum = classes[classNode.attr]
#                     # parserElement2.x1 = node2.value / 10  # todo
#                     # parserElement2.y1 = 0
#                     # parserElement2.x2 = node1.value / 10  # todo
#                     # parserElement2.y2 = node2.value / 10  # todo
#                     # decision_elements.append(parserElement2)
#                     # if pathDebug: print("append:", parserElement2.toString())
#                 elif '<' in node1.parent_op:
#                     if pathDebug: print("node1 parent op <")
#                     # parserElement.x1 = 0
#                     # parserElement.y2 = node1.value / 10  # todo
#                     # parserElement.x2 = node2.value / 10  # todo
#                     # parserElement.y2 = node2.value / 10  # todo
#                     # # parser will be appended at the end
#                     # parserElement2 = ParserElement(orig_labels)
#                     # parserElement2.x_attribute = pair[0]
#                     # parserElement2.y_attribute = pair[1]
#                     # parserElement2.graphNum = getGraphNum(parserElement.x_attribute, parserElement.y_attribute,
#                     #                                       graphIdMap)
#                     # parserElement2.classNum = classes[classNode.attr]
#                     # parserElement2.x1 = node1.value / 10  # todo
#                     # parserElement2.y1 = 0
#                     # parserElement2.x2 = node2.value / 10  # todo
#                     # parserElement2.y2 = node1.value / 10  # todo
#
#                     # decision_elements.append(parserElement2)
#                     # if pathDebug: print("append:", parserElement2.toString())
#             else:
#                 if pathDebug: print("classNode parent op and node1 parent op are the same")
#                 parserElement.x1 = 0
#                 parserElement.y1 = node1.value / 10  # todo
#                 parserElement.x2 = 1
#                 parserElement.y2 = 1
#                 # will be added at the end of loop
#                 parserElement2 = ParserElement(orig_labels)
#                 parserElement2.x_attribute = pair[0]
#                 parserElement2.y_attribute = pair[1]
#                 parserElement2.graphNum = getGraphNum(parserElement.x_attribute, parserElement.y_attribute, graphIdMap)
#                 parserElement2.classNum = classes[classNode.attr]
#                 parserElement2.x1 = node1.value / 10  # todo
#                 parserElement2.y1 = 0
#                 parserElement2.x2 = 1
#                 parserElement2.y2 = node1.value / 10  # todo
#                 parserElement2 = adjustParserElementBounds(parserElement2, path, orig_labels, node_map)
#                 decision_elements.append(parserElement2)
#                 if pathDebug: print("append:", parserElement2.toString())
#                 #
#
#                 # if '<' in classNode.parent_op:
#                 #     parserElement.x2 = node1.value / 10.0  # todo parameterize
#                 # elif '>' in classNode.parent_op:
#                 #     parserElement.x1 = node1.value / 10.0
#                 # if '<' in node1.parent_op:
#                 #     parserElement.x2 = node2.value / 10.0
#                 # elif '>' in node1.parent_op:
#                 #     parserElement.x1 = node2.value / 10.0
#             # else:
#             #     if '<' in classNode.parent_op:
#             #         parserElement.y2 = node1.value / 10.0  # todo parameterize
#             #     elif '>' in classNode.parent_op:
#             #         parserElement.y1 = node1.value / 10.0
#             #     if '<' in node1.parent_op:
#             #         parserElement.y2 = node2.value / 10.0
#             #     elif '>' in node1.parent_op:
#             #         parserElement.y1 = node2.value / 10.0
#         # Case 2: node1 and node2 are paired together
#         elif (node1.attr, node2.attr) in pairs or (node2.attr, node1.attr) in pairs:
#             if pathDebug: print("node1 and node2 are paired together")
#             pair = pairMap[node1.attr][0]  # todo: check if this length is ever greater than 1
#             attributeX = pair[0]
#             attributeY = pair[1]
#             isNode1X = (node1.attr == pair[0])
#
#             parserElement.x_attribute = attributeX
#             parserElement.y_attribute = attributeY
#             parserElement.graphNum = getGraphNum(attributeX, attributeY, graphIdMap)
#             parserElement.classNum = classes[classNode.attr]
#
#             # get first entry
#             if "<" in classNode.parent_op:
#                 if pathDebug: print("classNode parent op is <")
#                 if isNode1X:
#                     parserElement.x2 = node1.value / 10  # todo: parameterize
#                 else:
#                     parserElement.y2 = node1.value / 10
#             elif ">" in classNode.parent_op:
#                 if pathDebug: print("classNode parent op is >")
#                 if isNode1X:
#                     parserElement.x1 = node1.value / 10  # todo: parameterize
#                 else:
#                     parserElement.y1 = node1.value / 10
#
#             # get second entry
#             if "<" in node1.parent_op:
#                 if pathDebug: print("node1 parent op is <")
#                 if isNode1X:
#                     parserElement.y2 = node2.value / 10
#                 else:
#                     parserElement.x2 = node2.value / 10
#             elif ">" in node1.parent_op:
#                 if pathDebug: print("node1 parent op is >")
#                 if isNode1X:
#                     parserElement.y1 = node2.value / 10
#                 else:
#                     parserElement.x1 = node2.value / 10
#         # Case 3: node1 and node2 are not paired, node 1 is a single attribute pair
#         elif pairMap[node1.attr][0][0] == pairMap[node1.attr][0][1]:
#             if pathDebug: print("node1 and node2 are not paired and node1 is a single attribute pair")
#             pair = pairMap[node1.attr][0]
#             parserElement = ParserElement(orig_labels)
#             parserElement.x_attribute = pair[0]
#             parserElement.y_attribute = pair[1]
#             parserElement.graphNum = getGraphNum(pair[0], pair[1], graphIdMap)
#             parserElement.classNum = classes[classNode.attr]
#             if '<' in classNode.parent_op:
#                 if pathDebug: print("classNode parent op is <")
#                 parserElement.x1 = 0
#                 parserElement.y1 = 0
#                 parserElement.x2 = node1.value / 10  # TODO: Parameterize
#                 parserElement.y2 = node1.value / 10
#             elif '>' in classNode.parent_op:
#                 if pathDebug: print("classNode parent op is >")
#                 parserElement.x1 = 0
#                 parserElement.y1 = node1.value / 10  # todo
#                 parserElement.x2 = 1
#                 parserElement.y2 = 1
#                 # Will be added at the end of this loop
#
#                 parserElement2 = ParserElement(orig_labels)
#                 parserElement2.x_attribute = pair[0]
#                 parserElement2.y_attribute = pair[1]
#                 parserElement2.graphNum = getGraphNum(pair[0], pair[1], graphIdMap)
#                 parserElement2.classNum = classes[classNode.attr]
#                 parserElement2.x1 = node1.value / 10  # todo
#                 parserElement2.y1 = 0
#                 parserElement2.x2 = 1
#                 parserElement2.y2 = node1.value / 10  # todo
#                 parserElement2 = adjustParserElementBounds(parserElement2, path, orig_labels, node_map)
#                 decision_elements.append(parserElement2)
#                 if pathDebug: print("append:", parserElement2.toString())
#
#         # Case 4: node1 and node2 are not paired
#         else:
#             if pathDebug: print("node1 and node2 are not paired")
#             pair = pairMap[node1.attr][0]
#             attributeX = pair[0]
#             attributeY = pair[1]
#             isNode1X = (node1.attr == pair[0])
#
#             if "<" in classNode.parent_op:
#                 if isNode1X:
#                     parserElement.x2 = node1.value / 10  # todo: param
#                 else:
#                     parserElement.y2 = node1.value / 10
#             elif ">" in classNode.parent_op:
#                 if isNode1X:
#                     parserElement.x1 = node1.value / 10
#                 else:
#                     parserElement.y1 = node1.value / 10
#
#             parserElement.graphNum = getGraphNum(attributeX, attributeY, graphIdMap)
#             parserElement.classNum = classes[classNode.attr]
#             parserElement.x_attribute = attributeX
#             parserElement.y_attribute = attributeY
#
#         if parserElement.x_attribute == "bc" and parserElement.y_attribute == "cl":
#             if pathDebug: print("debug")
#
#         parserElement = adjustParserElementBounds(parserElement, path, orig_labels, node_map)
#
#         decision_elements.append(parserElement)
#         if pathDebug: print("append:", parserElement.toString())
#         if pathDebug: print("end path")
#     return decision_elements


def generateAllDecisionElementsV2(path_list, pairs, pairMap, node_map, isAttributePairedList, graphIdMap, orig_labels,
                                  classes, rootNode):
    decisionElements = []
    # idea: for each path, take the last two elements and name them classNode and parentNode
    # find the pair of parentNode
    # this paired can include being paired to itself
    # create a parser local to that pair
    # adjust parser to any duplicates found higher in the tree

    # We need a method for generating single attribute parserElements

    # Special case : path < 3 elements long
    # this means that there is only one attribute. Find it's pair, then create a single attribute parserElement
    for path in path_list:

        # init variables that we will need to construct parser
        classNode = path[len(path) - 1]
        parentNode = path[len(path) - 2]

        if len(path) < 3:
            singleAttributeElements = generateSingleAttributeParserElements(parentNode, classNode, classes, orig_labels,
                                                                            graphIdMap)
            for parser in singleAttributeElements:
                adjustedParserElements = adjustSingleAttributeParserElementBounds(parser, path, orig_labels, pairMap,
                                                                                  rootNode, graphIdMap)
                for adjustedParser in adjustedParserElements:
                    decisionElements.append(adjustedParser)
            continue

        parentNodePair = pairMap[parentNode.attr][0]
        parentNodePairedNode = node_map[parentNodePair[0]] if parentNode.attr != parentNodePair[0] \
            else node_map[parentNodePair[1]]
        isParentNodeXAxis = parentNode.attr == parentNodePair[0]
        parserElement = ParserElement(orig_labels)
        parserElement.classNum = classes[classNode.attr]
        parserElement.graphNum = getGraphNum(parentNode.attr, parentNodePairedNode.attr, graphIdMap)
        parserElement.x_attribute = parentNodePair[0]
        parserElement.y_attribute = parentNodePair[1]

        if orig_labels[parentNode.attr] == orig_labels[parentNodePairedNode.attr]:
            singleAttributeElements = generateSingleAttributeParserElements(parentNode, classNode, classes, orig_labels,
                                                                            graphIdMap)
            for parser in singleAttributeElements:
                adjustedParserElements = adjustSingleAttributeParserElementBounds(parser, path, orig_labels, pairMap,
                                                                                  rootNode, graphIdMap)
                for adjustedParser in adjustedParserElements:
                    decisionElements.append(adjustedParser)
            continue

        if isParentNodeXAxis:
            if '<' in classNode.parent_op:
                parserElement.x2 = parentNode.value / 10  # todo
            else:
                parserElement.x1 = parentNode.value / 10  # todo
        else:
            if '<' in classNode.parent_op:
                parserElement.y2 = parentNode.value / 10  # todo
            else:
                parserElement.y1 = parentNode.value / 10  # todo
            if '<' in parentNode.parent_op:
                parserElement.x2 = parentNodePairedNode.value / 10  # todo
            else:
                parserElement.x1 = parentNodePairedNode.value / 10  # todo

        parserElement = adjustParserElementBounds(parserElement, path, orig_labels, node_map, pairMap)
        decisionElements.append(parserElement)
    return decisionElements


def generateSingleAttributeParserElements(parentNode, classNode, classes, orig_labels, graphIdMap):
    singleAttributeParserElements = []
    parserElement = ParserElement(orig_labels)
    parserElement.x_attribute = parentNode.attr
    parserElement.y_attribute = parentNode.attr
    parserElement.classNum = classes[classNode.attr]
    parserElement.graphNum = getGraphNum(parentNode.attr, parentNode.attr, graphIdMap)

    if '<' in classNode.parent_op:
        parserElement.x1 = 0
        parserElement.y1 = 0
        parserElement.x2 = parentNode.value / 10  # todo
        parserElement.y2 = parentNode.value / 10  # todo
        singleAttributeParserElements.append(parserElement)
    elif '>' in classNode.parent_op:  # todo: problem here somewhere
        parserElement.x1 = 0
        parserElement.y1 = parentNode.value / 10  # todo
        parserElement.x2 = 1
        parserElement.y2 = 1

        parserElement2 = ParserElement(orig_labels)
        parserElement2.x_attribute = parentNode.attr
        parserElement2.y_attribute = parentNode.attr
        parserElement2.classNum = classes[classNode.attr]
        parserElement2.graphNum = getGraphNum(parentNode.attr, parentNode.attr, graphIdMap)
        parserElement2.x1 = parentNode.value / 10  # todo
        parserElement2.y1 = 0
        parserElement2.x2 = 1
        parserElement2.y2 = parentNode.value / 10  # todo
        singleAttributeParserElements.append(parserElement)
        singleAttributeParserElements.append(parserElement2)

    return singleAttributeParserElements


def adjustSingleAttributeParserElementBounds(parserElement, path, orig_labels, pairMap, rootNode, graphIdMap):
    classNode = path[len(path) - 1]
    parentNode = path[len(path) - 2]
    returnParserElements = []
    for i in reversed(range(len(path) - 2)):
        node2 = path[i]
        node1 = path[i + 1]
        if orig_labels[node2.attr] == orig_labels[parentNode.attr]:  # if node2 and the parent node are the same
            if parserElement.x1 == 0 and parserElement.y1 == 0:  # parserElement is a less than element
                parserElement.y1 = node2.value / 10  # todo
                newParserEl = parserElement.copy()
                newParserEl.x1 = node2.value / 10  # todo
                newParserEl.y1 = 0
                newParserEl.y2 = node2.value / 10  # todo
                returnParserElements.append(newParserEl)
                break
            if '<' in classNode.parent_op and '<' not in node1.parent_op or '>' in classNode.parent_op and '>' not in node1.parent_op:
                if '<' in node1.parent_op and '<' not in classNode.parent_op:
                    parserElement.x2 = node2.value / 10  # todo
                    parserElement.y2 = node2.value / 10  # todo
                    break
                elif '>' in node1.parent_op and '<' not in classNode.parent_op:
                    parserElement.x1 = node2.value / 10  # todo
                    parserElement.y1 = node2.value / 10  # todo
                    break

    # if the node that is paired to itself is the root node, combine it with the root node
    if parserElement.x_attribute == rootNode.attr and len(pairMap) > 1:
        rootPair = pairMap[rootNode.attr][0]
        parserElement.x_attribute = rootPair[0]
        parserElement.y_attribute = rootPair[1]
        parserElement.graphNum = getGraphNum(rootPair[0], rootPair[1], graphIdMap)
        if rootNode.attr == rootPair[0]:  # if root attr is X axis
            if parserElement.y1 == 1:  # greater than
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
    if parserElement.x_attribute == parserElement.y_attribute:
        return adjustParserElementBounds(parserElement, path, orig_labels, node_map, pairMap)

    child = path[len(path) - 1]
    parent = path[len(path) - 2]
    parentPair = pairMap[parent.attr][0]
    isParentXAxis = parent.attr == parentPair[0]
    parentPairedNode = node_map[parentPair[0]] if parent.attr != parentPair[0] else node_map[parentPair[1]]
    if parent.attr == parentPairedNode.attr:  # todo : consider making this its own method
        # Special case, only one attribute pair
        child = parent
        for i in reversed(range(len(path) - 2)):
            node2 = path[i]
            node1 = path[i + 1]
            if orig_labels[node2.attr] == orig_labels[parent.attr]:
                if '<' in node1.parent_op and '<' not in child.parent_op:
                    parserElement.y2 = node2.value / 10  # todo
                    parserElement.x2 = node2.value / 10  # todo
                    break
                elif '>' in node1.parent_op and '>' not in child.parent_op:
                    parserElement.y1 = node1.value / 10  # todo
                    parserElement.x1 = node1.value / 10  # todo
                    break
        return parserElement

    # case: if parent is X of its pair
    if isParentXAxis:
        for i in reversed(range(len(path) - 2)):
            node2 = path[i]
            node1 = path[i + 1]
            if orig_labels[node2.attr] == orig_labels[parent.attr]:
                if '<' in node1.parent_op and '<' not in child.parent_op:
                    parserElement.x2 = node2.value / 10  # todo
                    break
                elif '>' in node1.parent_op and '>' not in child.parent_op:
                    parserElement.x1 = node2.value / 10  # todo
                    break
        return parserElement

    # since we caught one attribute pairs already, then the previous two before class node must be paired
    for i in reversed(range(len(path) - 2)):
        node2 = path[i]
        node1 = path[i + 1]
        if orig_labels[node2.attr] == orig_labels[parent.attr]:
            if '<' in node1.parent_op and '<' not in child.parent_op:
                parserElement.y2 = node2.value / 10  # todo
                break
            elif '>' in node1.parent_op and '>' not in child.parent_op:
                parserElement.y1 = node1.value / 10  # todo
                break

    child = parent
    parent = path[len(path) - 3]
    for i in reversed(range(len(path) - 2)):
        node2 = path[i]
        node1 = path[i + 1]
        if orig_labels[node2.attr] == orig_labels[parent.attr]:
            if '<' in node1.parent_op and '<' not in child.parent_op:
                parserElement.x2 = node2.value / 10  # todo
                break
            elif '>' in node1.parent_op and '>' not in child.parent_op:
                parserElement.x1 = node1.value / 10  # todo
                break
    return parserElement


def adjustContinueParserElementBounds(parserElement, path, orig_labels, node_map):
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
                        parserElement.x1 = node5.value / 10  # todo
                        break
                    elif '<' in node3.parent_op and '<' not in node4.parent_op:
                        parserElement.x2 = node5.value / 10  # todo
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
                        parserElement.y1 = node5.value / 10  # todo
                        break
                    elif '>' in node3.parent_op and '>' not in node4.parent_op:
                        parserElement.y2 = node5.value / 10  # todo
                        break
    return parserElement


def generateAllContinueElements(path_list, pairs, pairMap, graphIdMap, orig_labels, node_map):
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
            # if node1.attr != pairs[node2.attr][0] and node1.attr != pairs[node2.attr][1]:
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
                    if "<" in node2.parent_op:
                        parserElement.x2 = node1.value / 10.0  # TODO: parameterize
                    elif ">" in node2.parent_op:
                        parserElement.x1 = node1.value / 10.0
                    # special case, we need to include X of node1 pair as well
                    # if i > 0:
                    #     # root -> ... -> node3 -> node1 -> node2 -> .. -> classNode
                    #     node3 = path[i - 1]
                    #     if "<" in node1.parent_op:
                    #         parserElement.y2 = node3.value / 10.0
                    #     elif ">" in node1.parent_op:
                    #         parserElement.y1 = node3.value / 10.0
                    #     else:
                    #         print("debug")  # todo
                else:
                    if "<" in node2.parent_op:
                        parserElement.y2 = node1.value / 10.0  # TODO: parameterize
                    elif ">" in node2.parent_op:
                        parserElement.y1 = node1.value / 10.0
                    # special case, we need to include X of node1 pair as well
                    if i > 0:
                        # root -> ... -> node3 -> node1 -> node2 -> .. -> classNode
                        node3 = path[i - 1]
                        if "<" in node1.parent_op:
                            parserElement.x2 = node3.value / 10.0
                        elif ">" in node1.parent_op:
                            parserElement.x1 = node3.value / 10.0
                        else:
                            print("debug")  # todo
                graphNum = getGraphNum(node1.attr, node1PairedAttr, graphIdMap)
                if isNode1AttrX:
                    parserElement.x_attribute = node1.attr
                    parserElement.y_attribute = node1PairedAttr
                else:
                    parserElement.y_attribute = node1.attr
                    parserElement.x_attribute = node1PairedAttr

                parserElement.graphNum = graphNum

                parentPair = (parserElement.x_attribute, parserElement.y_attribute)

                if parentPair not in parentPairMap:
                    parentPairMap[parentPair] = {pair: -1}
                elif pair not in parentPairMap[parentPair]:
                    parentPairMap[parentPair][pair] = -len(parentPairMap[parentPair]) - 1

                parserElement.classNum = parentPairMap[parentPair][pair]
                destinationPair = pairMap[node2.attr][0]
                destinationGraphId = getGraphNum(destinationPair[0], destinationPair[1], graphIdMap)
                parserElement.destinationGraphId = destinationGraphId

                parserElement = adjustContinueParserElementBounds(parserElement, path, orig_labels, node_map)

                if parserElement.toString() not in seen_decisions:
                    continueElements.append(parserElement)
                    seen_decisions.append(parserElement.toString())

    return continueElements


def attrInPairList(attr, pair_list):
    for pair in pair_list:
        if attr in pair:
            return True
    return False


def getGraphIdMap(pairs, root):
    # special case, only one pair
    # if len(pairs) == 1:
    #     graphId = 0
    # else:
    #     graphId = (len(pairs) // 2) - 1

    graphId = 1

    # TODO: Kind of funny, think about optimizing
    graphIdMap = dict()
    for pair in pairs:
        if root in pair:
            graphIdMap[pair] = 0
        elif pair in graphIdMap:
            graphId += 1
        else:
            graphIdMap[pair] = graphId
            graphId += 1
    return graphIdMap


def getGraphNum(attr1, attr2, graphIdMap):
    graphNum = -1
    if (attr1, attr2) in graphIdMap:
        graphNum = graphIdMap[(attr1, attr2)]
    elif (attr2, attr1) in graphIdMap:
        graphNum = graphIdMap[(attr2, attr1)]
    else:
        print("undefined")
    return graphNum


def replaceAttributeNames(node_list, orig_labels):
    for node in node_list:
        node.attr = orig_labels[node.attr]
    return node_list


def generateParser(input_file, output_file):
    if not exists(input_file):
        print("Error. Could not find file:", input_file, "in project directory")
        exit()

    tree = t2t.Tanagra_Parser()
    tree.parse(input_file)  # todo : parameterize
    # tree.print_tree()
    node_list = tree.traverse()
    # node_list = replaceAttributeNames(node_list, tree.orig_labels)
    # newLabels = dict()
    # for label in tree.orig_labels:
    #     name = tree.orig_labels[label]
    #     newLabels[label] = name
    #     newLabels[name] = name
    # tree.orig_labels = newLabels

    # TODO: Determine number of classes
    for node in node_list:
        if 'malignant' in node.attr:
            node.attr = "malignant"
        elif 'begnin' in node.attr:
            node.attr = "begnin"

    strippedLabels = tree.orig_labels.copy()
    for label in tree.orig_labels:
        if 'malignant' in label or 'begnin' in label:
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

    path_list = printPaths(tree.root)
    path_list = sorted(path_list, key=functools.cmp_to_key(compare), reverse=True)
    parserElements = []
    attribute_pairs = dict()
    pair_to_graphNum = dict()
    graphNum = 0

    # pairs = findPairs(path_list, classes, tree.orig_labels)
    pairs, pairMap, isAttributePairedList = findPairsV2(path_list, classes, tree.orig_labels)
    graphIdMap = getGraphIdMap(pairs, tree.root.attr)
    seen_continue_elements = []

    continueElements = generateAllContinueElements(path_list, pairs, pairMap, graphIdMap, tree.orig_labels, nodeMap)
    # decisionElements = generateAllDecisionElements(path_list, pairs, pairMap, nodeMap, isAttributePairedList,
    #                                                graphIdMap,
    #                                                tree.orig_labels, classes)
    decisionElements = generateAllDecisionElementsV2(path_list, pairs, pairMap, nodeMap, isAttributePairedList,
                                                     graphIdMap,
                                                     tree.orig_labels, classes, tree.root)

    for el in continueElements:
        parserElements.append(el)

    for el in decisionElements:
        parserElements.append(el)

    writeParser(parserElements, tree.orig_labels, output_file)
    stringToReturn = ""
    for parserElement in parserElements:
        stringToReturn += parserElement.toString() + '\n'
    return stringToReturn


def tests(file, output):
    print()
    # print("Testing:", file, "with output:\n", output)
    if file == "./TestTanagraOutputs/two_attribute_tree.txt":
        correctAnswer = ["0.25, 0, 1, 0.65, 0, 1, ucellsize, clump", "0.25, 0.65, 1, 1, 0, 1, ucellsize, clump",
                         "0, 0, 0.25, 1, 0, 0, ucellsize, clump"]
        counter = -1
        for row in correctAnswer:
            if row not in output:
                break
            counter += 1
        if counter == len(correctAnswer) - 1:
            print(file, "TEST PASSED")
        else:
            print(file, "NOT PASSED")
    elif file == "./TestTanagraOutputs/one_attribute_tree.txt":
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
            print(file, "TEST PASSED")
        else:
            print(file, "NOT PASSED")
    elif file == "./TestTanagraOutputs/two_attribute_tree_2.txt":
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
            print(file, "TEST PASSED")
        else:
            print(file, "NOT PASSED")
    elif file == "./TestTanagraOutputs/three_attribute_tree_two_duplicates.txt":
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
            print(file, "TEST PASSED")
        else:
            print(file, "NOT PASSED")
    elif file == "./TestTanagraOutputs/three_attribute_tree_one_duplicate.txt":
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
            print(file, "TEST PASSED")
        else:
            print(file, "NOT PASSED")

    elif file == "./TestTanagraOutputs/medium_tree.txt":
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
            print(file, "TEST PASSED")
        else:
            print(file, "NOT PASSED")


if __name__ == '__main__':
    # debug / test
    isDebugMode = False
    if isDebugMode:
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
                      "<output_file>")
            else:
                output = generateParser(sys.argv[1], sys.argv[2])
                tests(file, output)
    else:
        if len(sys.argv) < 3:
            print("Please call this script with the following syntax: python tanagra_to_parser.py <input_file> "
                  "<output_file>")
        else:
            output = generateParser(sys.argv[1], sys.argv[2])

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
def writeParser(parser_contents, orig_labels):
    parser_file = open("generatedParser.txt", 'w', encoding="utf-8")
    for parser_el in parser_contents:
        parserStr = parser_el.toString()
        string_to_write = str(parserStr).strip("[]") + "\n"
        # for label in orig_labels:
        #    if label in string_to_write:
        #        string_to_write = string_to_write.replace("'" + label + "'", orig_labels[label])
        parser_file.write(string_to_write)


class ParserElement:
    def __init__(self):
        self.x1 = 0
        self.y1 = 0
        self.x2 = 1
        self.y2 = 1
        self.graphNum = -1
        self.classNum = -1
        self.x_attribute = ""
        self.y_attribute = ""
        self.orig_labels = ""

    def toString(self):
        return str(self.x1) + ", " + str(self.y1) + ", " + str(self.x2) + ", " + str(self.y2) + ", " + \
               str(self.graphNum) + ", " + str(self.classNum) + ", " + self.orig_labels[self.x_attribute] + ", " + \
               self.orig_labels[self.y_attribute]


def findPairs(path_list, classes):
    # pair is (x_attr, y_attr)
    pairs = dict()
    path_list = sorted(path_list, key=functools.cmp_to_key(compare), reverse=True)
    attributeList = []
    usedAttributeList = []
    for path in path_list:
        for j in range(len(path) - 1, 0, -1):
            node1 = path[j]
            node2 = path[j - 1]
            if node1.attr not in classes and node2.attr not in classes:
                if node1.attr not in attributeList:
                    attributeList.append(node1.attr)
                if node2.attr not in attributeList:
                    attributeList.append(node2.attr)
                if node1.attr not in pairs and node2.attr not in pairs:
                    pair = (node1.attr, node2.attr)
                    pairs[node1.attr] = pair
                    pairs[node2.attr] = pair
                    # pairs[node1.attr] = node2.attr
                    # pairs[node2.attr] = node1.attr
                    usedAttributeList.append(node1.attr)
                    usedAttributeList.append(node2.attr)
    if len(attributeList) != len(usedAttributeList):
        print("debug, this will be used later")

    return pairs

# def generateParserElementFromClassNode(node1, classNode, node1PairedAttr, graphNum, classes, orig_labels, pairs):
#     # node1 -> class
#     # X or Y         classNide
#     parserElement = ParserElement()
#     parserElement.orig_labels = orig_labels
#
#     # need to determine if node1.attr is X or Y in its pair
#     node1AttrIsX = True
#     if pairs[node1.attr][0] == node1.attr:
#         node1AttrIsX = False
#
#     if node1AttrIsX:
#         if "<" in classNode.parent_op:
#             parserElement.x2 = node1.value / 10.0 # todo : paramterize
#         elif ">" in classNode.parent_op:
#             parserElement.x1 = node1.value / 10.0
#     else:
#         if "<" in classNode.parent_op:
#             parserElement.y2 = node1.value / 10.0 # todo : paramterize
#         elif ">" in classNode.parent_op:
#             parserElement.y1 = node1.value / 10.0
#
#     parserElement.graphNum = graphNum
#     if classNode.attr not in classes:
#         print("Debug")
#     parserElement.classNum = classes[classNode.attr]
#     if node1AttrIsX:
#         parserElement.x_attribute = node1.attr
#         parserElement.y_attribute = node1PairedAttr
#     else:
#         parserElement.y_attribute = node1.attr
#         parserElement.x_attribute = node1PairedAttr
#     return parserElement


# def generateParserDecisionParserElement(node1, node2, classNode, graphNum, classes, orig_labels):
#     # node1 -> node2 -> classNode
#     #  X         Y
#     parserElement = ParserElement()
#     parserElement.orig_labels = orig_labels
#
#     if isinstance(node2, str):
#         parserElement.graphNum = graphNum
#         parserElement.classNum = classes
#
#
#     if node1.value is not None:
#         if "<" in node2.parent_op:
#             parserElement.y2 = node1.value / 10.0 # TODO: Parameterize
#         elif ">" in node2.parent_op:
#             parserElement.y1 = node1.value / 10.0
#
#     if node2.value is not None:
#         if "<" in classNode.parent_op:
#             parserElement.x2 = node2.value / 10.0
#         elif ">" in classNode.parent_op:
#             parserElement.x1 = node2.value / 10.0
#
#     parserElement.graphNum = graphNum
#     if classNode.attr not in classes:
#         print("Debug")
#     parserElement.classNum = classes[classNode.attr]
#     parserElement.y_attribute = node1.attr
#     parserElement.x_attribute = node2.attr
#     return parserElement

def generateAllDecisionElements(path_list, pairs, graphIdMap, orig_labels, classes):
    decision_elements = []
    for i in range(len(path_list)):
        path = path_list[i]
        if len(path) < 3:
            # TODO
            continue

        # root -> ... -> node2 -> node1 -> classNode
        classNode = path[len(path) - 1]
        node1 = path[len(path) - 2]
        node2 = path[len(path) - 3]
        parserElement = ParserElement()
        parserElement.orig_labels = orig_labels

        # Case 1: node1 and node2 are paired
        if node1.attr == pairs[node2.attr][0] or node1.attr == pairs[node2.attr][1]:
            pair = pairs[node1.attr]
            # attributeX = node2.attr
            # attributeY = node1.attr
            # isNode1X = False
            # if node1.attr == pair[0]:
            #     isNode1X = True
            #     attributeX = node1.attr
            #     attributeY = node2.attr
            attributeX = pair[0]
            attributeY = pair[1]
            isNode1X = (node1.attr == pair[0])

            parserElement.x_attribute = attributeX
            parserElement.y_attribute = attributeY
            parserElement.graphNum = getGraphNum(attributeX, attributeY, graphIdMap)
            parserElement.classNum = classes[classNode.attr]

            # get first entry
            if "<" in classNode.parent_op:
                if isNode1X:
                    parserElement.x2 = node1.value / 10 # todo: parameterize
                else:
                    parserElement.y2 = node1.value
            elif ">" in classNode.parent_op:
                if isNode1X:
                    parserElement.x1 = node1.value / 10 # todo: parameterize
                else:
                    parserElement.y1 = node1.value

            # get second entry
            if "<" in node1.parent_op:
                if isNode1X:
                    parserElement.y2 = node2.value / 10
                else:
                    parserElement.x2 = node2.value / 10
            elif ">" in node1.parent_op:
                if isNode1X:
                    parserElement.y1 = node2.value / 10
                else:
                    parserElement.x1 = node2.value / 10

        # Case 2: node1 and node2 are not paired
        else:
            pair = pairs[node1.attr]
            attributeX = pair[0]
            attributeY = pair[1]
            isNode1X = (node1.attr == pair[0])

            if "<" in classNode.parent_op:
                if isNode1X:
                    parserElement.x2 = node1.value / 10 # todo: param
                else:
                    parserElement.y2 = node1.value / 10
            elif ">" in classNode.parent_op:
                if isNode1X:
                    parserElement.x1 = node1.value / 10
                else:
                    parserElement.y1 = node1.value / 10

            parserElement.graphNum = getGraphNum(attributeX, attributeY, graphIdMap)
            parserElement.classNum = classes[classNode.attr]
            parserElement.x_attribute = attributeX
            parserElement.y_attribute = attributeY

        if parserElement.x_attribute == "bc" and parserElement.y_attribute == "cl":
            print("debug")
        decision_elements.append(parserElement)
    return decision_elements


def generateAllContinueElements(path_list, pairs, graphIdMap, orig_labels):
    # traverse through longest path
    # any pair that has memebers of two different pairs, we add a decision node
    decisionElements = []
    seen_decisions = []
    for j in range(len(path_list)):
        path = path_list[j]
        for i in range(0, len(path) - 2, 1):
            node1 = path[i]
            node2 = path[i + 1]
            # if nodes are not paired, add a decision zone
            if node1.attr != pairs[node2.attr][0] and node1.attr != pairs[node2.attr][1]:
                parserElement = ParserElement()
                parserElement.orig_labels = orig_labels
                node1PairedAttr = pairs[node1.attr][1]
                isNode1AttrX = True
                if node1.attr == pairs[node1.attr][1]:
                    isNode1AttrX = False
                    node1PairedAttr = pairs[node1.attr][0]

                if isNode1AttrX:
                    if "<" in node2.parent_op:
                        parserElement.x2 = node1.value / 10.0  # TODO: parameterize
                    elif ">" in node2.parent_op:
                        parserElement.x1 = node1.value / 10.0
                    # special case, we need to include X of node1 pair as well
                    if i > 0:
                        # root -> ... -> node3 -> node1 -> node2 -> .. -> classNode
                        node3 = path[i - 1]
                        if "<" in node1.parent_op:
                            parserElement.y2 = node3.value / 10.0
                        elif ">" in node1.parent_op:
                            parserElement.y1 = node3.value / 10.0
                        else:
                            print("debug")  # todo
                else:
                        if "<" in node2.parent_op:
                            parserElement.y2 = node1.value / 10.0  # TODO: parameterize
                        elif ">" in node2.parent_op:
                            parserElement.y1 = node1.value / 10.0
                graphNum = getGraphNum(node1.attr, node1PairedAttr, graphIdMap)
                parserElement.graphNum = graphNum
                parserElement.classNum =  -1
                if isNode1AttrX:
                    parserElement.x_attribute = node1.attr
                    parserElement.y_attribute = node1PairedAttr
                else:
                    parserElement.y_attribute = node1.attr
                    parserElement.x_attribute = node1PairedAttr
                if parserElement.toString() not in seen_decisions:
                    decisionElements.append(parserElement)
                    seen_decisions.append(parserElement.toString())
    return decisionElements

# def generateParserContinueElement(node1, node2, pairedNodeAttr, graphNum, orig_labels, pairs):
#     # node1 -> node2 -> ... -> class
#     # node is paired to some different node : pairedNode
#
#     parserElement = ParserElement()
#     parserElement.orig_labels = orig_labels
#
#     # determine if node1 is X or Y in it's pair
#     # need to determine if node1.attr is X or Y in its pair
#     node1AttrIsX = True
#     if pairs[node1.attr][0] == node1.attr:
#         node1AttrIsX = False
#
#     if node1AttrIsX:
#         if "<" in node2.parent_op:
#             parserElement.x2 = node1.value / 10.0 # TODO: parameterize
#         elif ">" in node2.parent_op:
#             parserElement.x1 = node1.value / 10.0
#     else:
#         if "<" in node2.parent_op:
#             parserElement.y2 = node1.value / 10.0 # TODO: parameterize
#         elif ">" in node2.parent_op:
#             parserElement.y1 = node1.value / 10.0
#
#     parserElement.classNum = -1
#     if node1AttrIsX:
#         parserElement.x_attribute = node1.attr
#         parserElement.y_attribute = pairedNodeAttr
#     else:
#         parserElement.y_attribute = node1.attr
#         parserElement.x_attribute = pairedNodeAttr
#     parserElement.graphNum = graphNum
#     return parserElement

def getGraphIdMap(pairs):
    graphId = int(len(pairs) / 2) - 1
    graphIdMap = dict()
    isReciprocal = False # every pair will be represented twice, so we use this to keep track of that
    for pair in pairs:
        graphIdMap[pairs[pair]] = graphId
        if isReciprocal is True:
            graphId -= 1
        isReciprocal = not isReciprocal
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



def main():

    tree = t2t.Tanagra_Parser()
    tree.parse("tanagra_output.txt")
    tree.print_tree()
    node_list = tree.traverse()

    # Determine number of classes
    for node in node_list:
        if 'malignant' in node.attr:
            node.attr = "malignant"
        elif 'begnin' in node.attr:
            node.attr = "begnin"

    classes = dict()
    counter = 0
    for node in node_list:
        if node.op is None and node.attr not in classes:
            classes[node.attr] = counter
            counter += 1

    path_list = printPaths(tree.root)
    path_list = sorted(path_list, key=functools.cmp_to_key(compare))
    parserElements = []
    attribute_pairs = dict()
    pair_to_graphNum = dict()
    graphNum = 0

    pairs = findPairs(path_list, classes)
    graphIdMap = getGraphIdMap(pairs)
    seen_continue_elements = []

    continueElements = generateAllContinueElements(path_list, pairs, graphIdMap, tree.orig_labels)
    decisionElements = generateAllDecisionElements(path_list, pairs, graphIdMap, tree.orig_labels, classes)

    for el in continueElements:
        parserElements.append(el)

    for el in decisionElements:
        parserElements.append(el)

    # for path in path_list:
    #     index = 0
    #     while index < len(path):
    #         current = path[index]
    #
    #         if current.attr in classes:
    #             if len(path) < 3:
    #                 node1 = path[index - 1]
    #                 node1Pair = pairs[node1.attr][0]
    #                 if node1.attr == node1Pair[0]:
    #                     node1Pair = pairs[node1.attr][1]
    #                 node2 = current
    #                 graphNum = getGraphNum(node1.attr, node2.attr, graphIdMap)
    #                 generateParserDecisionParserElement(node1, node2, current, graphNum, classes, tree.orig_labels)
    #                 break
    #             else:
    #                 node1 = path[index - 2]
    #                 node2 = path[index - 1]
    #                 graphNum = getGraphNum(node1.attr, node2.attr, graphIdMap)
    #                 parserElement = generateParserDecisionParserElement(node1, node2, current, graphNum, classes, tree.orig_labels)
    #                 parserElements.append(parserElement)
    #                 break
    #
    #         # is current element paired to next element?
    #         if index + 1 < len(path) and (pairs[current.attr][0] == path[index + 1].attr or pairs[current.attr][1] == path[index + 1].attr):
    #             index += 2
    #             continue
    #         # if index is the second to last element of a path
    #         elif index == len(path) - 2:
    #             node1 = current
    #             node2 = path[index + 1] # node2 becomes class node
    #             node1PairedAttr = pairs[node1.attr][0] # find the attribute that node1 is paired to
    #             if node1PairedAttr == node1.attr:
    #                 node1PairedAttr = pairs[node1.attr][1]
    #             graphNum = getGraphNum(node1.attr, node1PairedAttr, graphIdMap)
    #             parserElement = generateParserElementFromClassNode(node1, node2, node1PairedAttr, graphNum, classes, tree.orig_labels, pairs)
    #             parserElements.append(parserElement)
    #             break
    #         # if node1 and node2 are not paired and we are not at the last two elements
    #         elif index + 1 < len(path):
    #             # make a decision element in node 1's pair, then increment index
    #             index += 1
    #             continue
    #             node1 = current
    #             node2 = path[index + 1]
    #
    #             # debug
    #             if node1.attr == "bn_1" and node2.attr == "cl":
    #                 print("debug")
    #
    #             pair = pairs[node1.attr]
    #             pairAttr = pair[0]
    #             if pair[0] == node1.attr:
    #                 pairAttr = pair[1]
    #             graphNum = getGraphNum(node1.attr, pairAttr, graphIdMap)
    #             parserElement = generateParserContinueElement(node1, node2, pairAttr, graphNum, tree.orig_labels, pairs)
    #             if parserElement.toString() not in seen_continue_elements:
    #                 parserElements.append(parserElement)
    #                 seen_continue_elements.append(parserElement.toString())
    #             # index += 1
    #             # continue
    #         else:
    #             print("debug")
    # # for path in path_list:
    # #     index = 0
    # #     parserIsGenerated = False
    # #     while index < len(path):
    # #         current = path[index]
    # #         # is current node part of a class
    # #         if current.attr in classes:
    # #             if len(path) > 2:
    # #                 node1 = path[len(path) - 3]
    # #                 node2 = path[len(path) - 2]
    # #                 graphId = -1
    # #                 if (node1.attr, node2.attr) in graphIdMap:
    # #                     graphId = graphIdMap[(node1.attr, node2.attr)]
    # #                 elif (node2.attr, node1.attr) in graphIdMap:
    # #                     graphId = graphIdMap[(node2.attr, node1.attr)]
    # #                 else:
    # #                     print("Undefined") # todo
    # #                     break
    # #                 parserElement = generateParserDecisionParserElement(node1, node2, current, graphId, classes, tree)
    # #                 parserElements.append(parserElement)
    # #                 break
    # #             else:
    # #                 print("Special case") # TODO
    # #
    # #         # is current node paired to the next node
    # #         if pairs[current.attr][0] == path[index + 1].attr or pairs[current.attr][1] == path[index + 1].attr:
    # #             if index == len(path) - 3:
    # #                 node1 = current
    # #                 node2 = path[index + 1]
    # #                 graphId = -1
    # #                 if (node1.attr, node2.attr) in graphIdMap:
    # #                     graphId = graphIdMap[(node1.attr, node2.attr)]
    # #                 elif (node2.attr, node1.attr) in graphIdMap:
    # #                     graphId = graphIdMap[(node2.attr, node1.attr)]
    # #                 else:
    # #                     print("Undefined") # todo
    # #                 parserElement = generateParserDecisionParserElement(node1, node2, path[len(path) - 1], graphId, classes, tree)
    # #                 parserElements.append(parserElement)
    # #                 break
    # #             else:
    # #                 print("Special Case") # TODO
    # #         else:
    # #             node1 = current
    # #             node2 = path[index + 1]
    # #             graphId = -1
    # #             if (node1.attr, pairs[node1.attr][0]) in graphIdMap:
    # #                 graphId = graphIdMap[(node1.attr, pairs[node1.attr][0])]
    # #             elif (node1.attr, pairs[node1.attr][1]) in graphIdMap:
    # #                 graphId = graphIdMap[(node1.attr, pairs[node1.attr][1])]
    # #             elif (pairs[node1.attr][0], node1.attr) in graphIdMap:
    # #                 graphId = graphIdMap[(pairs[node1.attr][0], node1.attr)]
    # #             elif (pairs[node1.attr][1], node1.attr) in graphIdMap:
    # #                 graphId = graphIdMap[(pairs[node1.attr][1], node1.attr)]
    # #             else:
    # #                 print("Undefined")  # todo
    # #             parserElement = generateParserContinueElement(node1, node2, pairs[node1.attr], graphId, tree)
    # #             parserElements.append(parserElement)
    # #         index += 1
    #
    # #
    # #     i = 0
    # #     while i < len(path):
    # #         pathElement = path[i]
    # #
    # #         # is pathElement a class?
    # #         if pathElement.attr in classes:
    # #             if len(path) < 3:
    # #                 #TODO: special case
    # #             else:
    # #                 node1 = path[i - 2]
    # #                 node2 = path[i - 1]
    # #                 graphNum = 0 # tmp
    # #                 generateParserDecisionParserElement(node1, node2, pathElement, graphNum)
    # #             break
    # #
    # #         # is pathElement paired to the next element?
    # #         if pairs[pathElement] == path[i+1]:
    # #             # is pair last pair before class?
    # #             if path[i + 1] == len(path) - 2:
    # #                 node1 = pathElement
    # #                 node2 = path[i + 1]
    # #                 generateParserDecisionParserElement(node1, node2, )
    # #             break
    # #         else:
    # #             cont = generateContinueElement()
    # #             if cont in continueElements:
    # #                 continue
    # #             else:
    # #                 continueElements.insert(cont)
    # #     i += 1
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # # graphNumId = dict()
    # # graphNumPair = dict()
    # # seenAttributes = []
    # # graphNum = 0
    # # for attribute in tree.orig_labels:
    # #     if "malignant" in attribute or "begnin" in attribute:
    # #         continue
    # #     if attribute not in seenAttributes and pairs[attribute] not in seenAttributes:
    # #         seenAttributes.append(pairs[attribute][0])
    # #         seenAttributes.append(pairs[attribute][1])
    # #         graphNumId[(pairs[attribute][0], pairs[attribute][1])] = graphNum
    # #         graphNumPair[graphNum] = (pairs[attribute][0], pairs[attribute][1])
    # #         graphNum += 1
    # #
    # # # step through each path
    # # for path in path_list:
    # #     is_x_axis = True
    # #     class_node = path[len(path) - 1]
    # #     classNum = classes[class_node.attr]
    # #     parserElement = ParserElement()
    # #
    # #     for j in range(len(path) - 1, 0, -1):
    # #         node1 = path[j]
    # #         node2 = path[j - 1]
    # #         parserElement.classNum = classNum
    # #
    # #         if node1.attr not in classes:
    # #             if pairs[node1.attr][0] == node2.attr or pairs[node1.attr][1] == node2.attr:
    # #                 if '<' in node1.parent_op:
    # #                     parserElement.y2 = node2.value / 10.0 #TODO parameterize
    # #                 elif '>' in node1.parent_op:
    # #                     parserElement.y1 = node2.value / 10.0 #TODO paramterize
    # #                 parserElements.append(parserElement)
    # #                 parserElement = ParserElement()
    # #             else:
    # #                 print()
    # #         else:
    # #             parserElement.x_attribute = pairs[node2.attr][0]
    # #             parserElement.y_attribute = pairs[node2.attr][1]
    # #             parserElement.graphNum = graphNumId[(parserElement.x_attribute, parserElement.y_attribute)]
    # #             if node1.parent_op == '<' or node1.parent_op == '<=':
    # #                 if node2.attr == pairs[node2.attr][0]:
    # #                     parserElement.x2 = node2.value / 10.0 # TODO Parameterize
    # #                 elif node2.attr == pairs[node2.attr][1]:
    # #                     parserElement.y2 = node2.value / 10.0 # Todo parameterize
    # #                     parserElements.append(parserElement)
    # #                     parserElement = ParserElement()
    # #                 else:
    # #                     print()
    # #             elif '>' in node1.parent_op:
    # #                 if node2.attr == pairs[node2.attr][0]:
    # #                     parserElement.x1 = node2.value / 10.0 # TODO parameterize
    # #                 elif node2.attr == pairs[node2.attr][1]:
    # #                     parserElement.y1 = node2.value / 10.0 # todo parameterize
    # #                     parserElements.append(parserElement)
    # #                     parserElement = ParserElement()
    # #                 else:
    # #                     print()
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    # #
    #
    #
    #
    #
    #
    #         #
    #         # if node1.attr not in classes and node2.attr not in classes:
    #         #     if pairs[node1.attr][0] == node2.attr or pairs[node1.attr][1] == node2.attr:
    #         #         parserElement.x_attribute = pairs[node1.attr][0]
    #         #         parserElement.y_attribute = pairs[node1.attr][1]
    #         #     else:
    #         #         parserElement.x_attribute = pairs[node2.attr][0]
    #         #         parserElement.y_attribute = pairs[node2.attr][1]
    #         #         parserElement.classNum = -1
    #         # else:
    #         #     parserElement.x_attribute = pairs[node2.attr][0]
    #         #     parserElement.y_attribute = pairs[node2.attr][1]
    #         #
    #         # if (parserElement.x_attribute, parserElement.y_attribute) in graphNumId:
    #         #     parserElement.graphNum = graphNumId[(parserElement.x_attribute, parserElement.y_attribute)]
    #         # else:
    #         #     parserElement.graphNum = graphNumId[(parserElement.y_attribute, parserElement.x_attribute)]
    #         #
    #         # if is_x_axis:
    #         #     if node1.parent_op == '<' or node1.parent_op == '<=':
    #         #         parserElement.x2 = node2.value / 10.0  # TODO: parameterize
    #         #     elif node1.parent_op == '>' or node1.parent_op == '>=':
    #         #         parserElement.x1 = node2.value / 10.0
    #         # else:
    #         #     if node1.parent_op == '<' or node1.parent_op == '<=':
    #         #         parserElement.y2 = node2.value / 10.0  # TODO: parameterize
    #         #     elif node1.parent_op == '>' or node1.parent_op == '>=':
    #         #         parserElement.y1 = node2.value / 10.0
    #         #     parserElements.append(parserElement)
    #         #     parserElement = ParserElement()
    #         # is_x_axis = not is_x_axis
    writeParser(parserElements, tree.orig_labels)
    for parserElement in parserElements:
        print(parserElement.toString())


if __name__ == '__main__':
    main()

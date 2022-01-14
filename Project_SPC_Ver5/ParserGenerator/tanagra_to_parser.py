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
    attributeSet = set()
    usedAttributeSet = set()

    for path in path_list:
        # special case: path is only two nodes long
        # pair up the attribute in path[0] with itself
        if len(path) < 3 and len(pairs) == 0:
            pair = (path[0].attr, path[0].attr)
            pairs[path[0].attr] = pair
            usedAttributeSet.add(pair[0])
            continue

        for j in range(len(path) - 1, 0, -1):
            node1 = path[j]
            node2 = path[j - 1]
            if node1.attr not in classes and node2.attr not in classes:
                if node1.attr not in attributeSet:
                    attributeSet.add(node1.attr)
                if node2.attr not in attributeSet:
                    attributeSet.add(node2.attr)
                if node1.attr not in pairs and node2.attr not in pairs:
                    pair = (node1.attr, node2.attr)
                    pairs[node1.attr] = pair
                    pairs[node2.attr] = pair
                    # pairs[node1.attr] = node2.attr
                    # pairs[node2.attr] = node1.attr
                    usedAttributeSet.add(node1.attr)
                    usedAttributeSet.add(node2.attr)

    # if there are unpaired attributes, pair with itself
    if len(attributeSet) != len(usedAttributeSet):
        for attribute in attributeSet.difference(usedAttributeSet):
            pairs[attribute] = (attribute, attribute)

    return pairs

def generateAllDecisionElements(path_list, pairs, graphIdMap, orig_labels, classes):
    decision_elements = []
    for i in range(len(path_list)):
        path = path_list[i]
        # special case, if the path is less than three nodes long
        if len(path) < 3:
            # TODO
            node1 = path[0]
            node2 = path[1]
            pair = pairs[node1.attr]
            graphId = graphIdMap[pair]
            classNum = classes[node2.attr]
            if pair[0] == pair[1]:
                if '<' in node2.parent_op:
                    parserElement1 = ParserElement()
                    parserElement1.orig_labels = orig_labels
                    parserElement1.x2 = node1.value / 10.0 # todo: parameterize
                    parserElement1.y2 = node1.value / 10.0  # todo: parameterize
                    parserElement1.x_attribute = pair[0]
                    parserElement1.y_attribute = pair[1]
                    parserElement1.graphNum = graphId
                    parserElement1.classNum = classNum

                    parserElement2 = ParserElement()
                    parserElement2.orig_labels = orig_labels
                    parserElement2.x1 = node1.value / 10.0
                    parserElement2.y2 = node1.value / 10.0
                    parserElement2.x_attribute = pair[0]
                    parserElement2.y_attribute = pair[1]
                    parserElement2.graphNum = graphId
                    parserElement2.classNum = classNum

                    parserElement3 = ParserElement()
                    parserElement3.orig_labels = orig_labels
                    parserElement3.y1 = node1.value / 10.0
                    parserElement3.x2 = node1.value / 10.0
                    parserElement3.x_attribute = pair[0]
                    parserElement3.y_attribute = pair[1]
                    parserElement3.graphNum = graphId
                    parserElement3.classNum = classNum

                    decision_elements.append(parserElement1)
                    decision_elements.append(parserElement2)
                    decision_elements.append(parserElement3)
                elif '>' in node2.parent_op:
                    parserElement = ParserElement()
                    parserElement.orig_labels = orig_labels
                    parserElement.x1 = node1.value / 10.0  # todo: parameterize
                    parserElement.y1 = node1.value / 10.0  # todo: parameterize
                    parserElement.x_attribute = pair[0]
                    parserElement.y_attribute = pair[1]
                    parserElement.graphNum = graphId
                    parserElement.classNum = classNum
                    decision_elements.append(parserElement)
            else:
                pair = pairs[node1.attr]
                isNode1X = (node1.attr == pair[0])
                parserElement = ParserElement()
                parserElement.orig_labels = orig_labels
                parserElement.classNum = classes[node2.attr]
                parserElement.graphNum = graphIdMap[pair]
                parserElement.x_attribute = pair[0]
                parserElement.y_attribute = pair[1]
                if '<' in node2.parent_op:
                    if isNode1X:
                        parserElement.x2 = node1.value / 10
                    else:
                        parserElement.y2 = node1.value / 10 #todo : parameterize
                elif '>' in node2.parent_op:
                    if isNode1X:
                        parserElement.x1 = node1.value / 10
                    else:
                        parserElement.y1 = node1.value / 10 #todo : parameterize
                decision_elements.append(parserElement)
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
                    parserElement.x2 = node1.value / 10  # todo: parameterize
                else:
                    parserElement.y2 = node1.value
            elif ">" in classNode.parent_op:
                if isNode1X:
                    parserElement.x1 = node1.value / 10  # todo: parameterize
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
                    parserElement.x2 = node1.value / 10  # todo: param
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
    continueElements = []
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
                parserElement.classNum = -1
                if isNode1AttrX:
                    parserElement.x_attribute = node1.attr
                    parserElement.y_attribute = node1PairedAttr
                else:
                    parserElement.y_attribute = node1.attr
                    parserElement.x_attribute = node1PairedAttr
                if parserElement.toString() not in seen_decisions:
                    continueElements.append(parserElement)
                    seen_decisions.append(parserElement.toString())
    return continueElements


def getGraphIdMap(pairs):
    # special case, only one pair
    # if len(pairs) == 1:
    #     graphId = 0
    # else:
    #     graphId = (len(pairs) // 2) - 1

    graphId = 0

    graphIdMap = dict()
    isReciprocal = False  # every pair will be represented twice, so we use this to keep track of that
    for pair in pairs:
        if pairs[pair] in graphIdMap:
            graphId += 1
        else:
            graphIdMap[pairs[pair]] = graphId
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


def generateParser(input_file, output_file):
    if not exists(input_file):
        print("Error. Could not find file:", input_file, "in project directory")
        exit()

    tree = t2t.Tanagra_Parser()
    tree.parse(input_file)  # todo : parameterize
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

    writeParser(parserElements, tree.orig_labels, output_file)
    for parserElement in parserElements:
        print(parserElement.toString())


if __name__ == '__main__':
    # debug / test
    sys.argv.append("two_attribute_tree_2.txt")
    sys.argv.append("two_attribute_tree_2_parser.txt")

    if len(sys.argv) < 3:
        print("Please call this script with the following syntax: python tanagra_to_parser.py <input_file> "
              "<output_file>")
    else:
        generateParser(sys.argv[1], sys.argv[2])

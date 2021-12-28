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
        for label in orig_labels:
            if label in string_to_write:
                string_to_write = string_to_write.replace("'" + label + "'", orig_labels[label])
        parser_file.write(string_to_write)


class ParserElement:
    def __init__(self):
        self.x1 = 0
        self.y1 = 0
        self.x2 = 1
        self.y2 = 1
        self.graphNum = -1
        self.classNum = -1
        self.x_attribute = None
        self.y_attribute = None

    def toString(self):
        return str(self.x1) + ", " + str(self.y1) + ", " + str(self.x2) + ", " + str(self.y2) + ", " + \
               str(self.graphNum) + ", " + str(self.classNum) + ", " + tree.orig_labels[self.x_attribute] + ", " + \
               tree.orig_labels[self.y_attribute]


def findPairs(path_list):
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
                    pairs[node1.attr] = node2.attr
                    pairs[node2.attr] = node1.attr
                    usedAttributeList.append(node1.attr)
                    usedAttributeList.append(node2.attr)
    if len(attributeList) != len(usedAttributeList):
        print("debug, this will be used later")

    return pairs


if __name__ == '__main__':

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

    pairs = findPairs(path_list)
    graphNumId = dict()
    graphNumPair = dict()
    seenAttributes = []
    graphNum = 0
    for attribute in tree.orig_labels:
        if "malignant" in attribute or "begnin" in attribute:
            continue
        if attribute not in seenAttributes and pairs[attribute] not in seenAttributes:
            seenAttributes.append(attribute)
            seenAttributes.append(pairs[attribute])
            graphNumId[(attribute, pairs[attribute])] = graphNum
            graphNumPair[graphNum] = (attribute, pairs[attribute])
            graphNum += 1

    # step through each path
    for path in path_list:
        is_x_axis = True
        class_node = path[len(path) - 1]
        classNum = classes[class_node.attr]
        parserElement = ParserElement()

        for j in range(len(path) - 1, 0, -1):
            node1 = path[j]
            node2 = path[j - 1]
            parserElement.classNum = classNum
            if node1.attr not in classes and node2.attr not in classes:
                if pairs[node1.attr] == node2.attr:
                    parserElement.x_attribute = node2.attr
                    parserElement.y_attribute = node1.attr
                else:
                    parserElement.x_attribute = node2.attr
                    parserElement.y_attribute = pairs[node2.attr]
                    parserElement.classNum = -1
            else:
                parserElement.x_attribute = node2.attr
                parserElement.y_attribute = pairs[node2.attr]

            if (parserElement.x_attribute, parserElement.y_attribute) in graphNumId:
                parserElement.graphNum = graphNumId[(parserElement.x_attribute, parserElement.y_attribute)]
            else:
                parserElement.graphNum = graphNumId[(parserElement.y_attribute, parserElement.x_attribute)]

            parserPairOrdered = graphNumPair[parserElement.graphNum]
            parserElement.x_attribute = parserPairOrdered[0]
            parserElement.y_attribute = parserPairOrdered[1]

            if is_x_axis:
                if node1.parent_op == '<' or node1.parent_op == '<=':
                    parserElement.x2 = node2.value / 10.0  # TODO: parameterize
                elif node1.parent_op == '>' or node1.parent_op == '>=':
                    parserElement.x1 = node2.value / 10.0
            else:
                if node1.parent_op == '<' or node1.parent_op == '<=':
                    parserElement.y2 = node2.value / 10.0  # TODO: parameterize
                elif node1.parent_op == '>' or node1.parent_op == '>=':
                    parserElement.y1 = node2.value / 10.0
                parserElements.append(parserElement)
                parserElement = ParserElement()
            is_x_axis = not is_x_axis
    writeParser(parserElements, tree.orig_labels)

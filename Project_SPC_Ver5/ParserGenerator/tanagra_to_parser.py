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
        string_to_write = str(parser_el).strip("[]") + "\n"
        for label in orig_labels:
            if label in string_to_write:
                string_to_write = string_to_write.replace("'" + label + "'", orig_labels[label])
        parser_file.write(string_to_write)


if __name__ == '__main__':

    # look at notes for updated parser strategy




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
    graphAttributePairs = []
    parserElements = []
    used_attributes = []
    graphNum = -1
    for i in range(len(path_list)):
        parserElement = [0, 0, 1, 1, 0, -1, None, None]
        path = path_list[i]
        node_class = path[len(path) - 1].attr  # class is determined by end of path
        parserElement[5] = classes[node_class]
        node1 = None
        node2 = None
        is_x_axis = True
        for j in range(len(path) - 1, 0, -1):
            node1 = path[j]
            node2 = path[j - 1]

            # Determine what the coordinates are for a given decision node
            if is_x_axis:
                is_x_axis = not is_x_axis
                if node1.parent_op is not None and (node1.parent_op == '<' or node1.parent_op == '<='):
                    parserElement[2] = node2.value / 10.0
                elif node1.parent_op is not None and (node1.parent_op == '>' or node1.parent_op == '>='):
                    parserElement[0] = node2.value / 10.0
            else:
                is_x_axis = not is_x_axis
                if node1.parent_op is not None and (node1.parent_op == '<' or node1.parent_op == '<='):
                    parserElement[3] = node2.value / 10.0
                elif node1.parent_op is not None and (node1.parent_op == '>' or node1.parent_op == '>='):
                    parserElement[1] = node2.value / 10.0

            # Make and add pairings of attributes to be displayed
            if node1 is not None and node2 is not None:
                if node1.attr not in classes and node2.attr not in classes:
                    if (node1.attr, node2.attr) not in graphAttributePairs:
                        graphAttributePairs.append((node1.attr, node2.attr))
                        graphNum += 1
            elif (node1.attr, graphAttributePairs[len(graphAttributePairs) - 1][0]) not in graphAttributePairs and \
                    (node1.attr not in used_attributes and
                     graphAttributePairs[len(graphAttributePairs) - 1][0] not in used_attributes):
                if node1.attr not in classes and node2.attr not in classes:
                    graphAttributePairs.append((node1.attr, graphAttributePairs[len(graphAttributePairs) - 1][0]))
                    graphNum += 1

        parserElement[4] = graphNum
        parserElement[6] = graphAttributePairs[graphNum][0]
        parserElement[7] = graphAttributePairs[graphNum][1]
        parserElements.append(parserElement.copy())
    writeParser(parserElements, tree.orig_labels)

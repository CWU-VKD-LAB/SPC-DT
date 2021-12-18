import json

import tana2tree as t2t

### we need to know how many graphs to make
# Maybe number of nodes that have a decision / 2 ?
# Rational being that each pair constitutes two decision barriers

### we need to know which attributes make each graph
# Maybe adjacent nodes with decisions operators?
from typing import Tuple

if __name__ == '__main__':

    tree = t2t.Tanagra_Parser()
    tree.parse("tanagra_output.txt")
    tree.print_tree()
    node_list = tree.traverse()
    dict = tree.make_dict()

    counter = 0
    isEven = 0
    attributePairs = []
    decisionNodes = []
    for i in range(len(node_list)):
        node = node_list[i]
        if node.op is not None:
            print("node:", counter, "value:", node.op, "attribute:", node.attr)
            decisionNodes.append(node)

    for i in range(len(decisionNodes)):
        node = decisionNodes[i]
        if i % 2 > 0:
            attributePairs.append((node.attr, decisionNodes[i-1].attr))
    if len(decisionNodes) % 2 > 0:
        attributePairs.append((decisionNodes[len(decisionNodes) - 1], decisionNodes[len(decisionNodes) - 2]))

    print(attributePairs)

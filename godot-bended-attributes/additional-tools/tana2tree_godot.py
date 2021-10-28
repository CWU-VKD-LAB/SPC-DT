import tana2tree as t2t
import json
import sys
import os

def is_leaf(node_dict):
    if node_dict == None:
        return False
    else:
        return "leaf_class" in node_dict

def convert_node(node_dict, optimize = False):
    if not "l_child" in node_dict and not "r_child" in node_dict:
        return {"leaf_class" : node_dict["attr"]}

    new_node = {"attribute" : node_dict["attr"], "threshold": node_dict["value"], "operator": "<"}

    left_child_dict = None
    right_child_dict = None
    if "l_child" in node_dict:
        left_child_dict = node_dict["l_child"]
    if "r_child" in node_dict:
        right_child_dict = node_dict["r_child"]  

    left_child_node = convert_node(left_child_dict, optimize) if left_child_dict != None else None
    right_child_node = convert_node(right_child_dict, optimize) if right_child_dict != None else None
    new_node["child_left"] = left_child_node
    new_node["child_right"] = right_child_node

    if optimize and is_leaf(left_child_node) and is_leaf(right_child_node) and left_child_node["leaf_class"] == right_child_node["leaf_class"]:
        print("Optimizing a node ...")
        return {"leaf_class" : left_child_node["leaf_class"]}

    return new_node

def convert_dict(src_dict, optimize = False):
    if "tree_node" in src_dict:
        converted_node = convert_node(src_dict["tree_node"], optimize)
        return {"tree_root" : converted_node}
    else:
        print("Error: Tree dictionary is missing tree_node key.")
        return None

def main(input_file, output_file, optimize_tree = False):
    # build the tree, returns root node
    tree = t2t.Tanagra_Parser()
    print("Tree: ")
    print(tree.parse(input_file))

    print("\n")

    # print tree in readable format
    print("Pretty print tree: ")
    tree.print_tree()

    print("\n")

    # return tree as dict
    print("Tree formatted as dict:")
    d = tree.make_dict()
    print(d)

    converted_dict = convert_dict(d, optimize_tree)
    if converted_dict != None:
        with open(output_file, "w") as outfile:  
            json.dump(converted_dict, outfile)
            print("\n\nGodot tree outputted to " + output_file)

# https://stackoverflow.com/questions/15008758/parsing-boolean-values-with-argparse
def str2bool(v):
    if isinstance(v, bool):
       return v
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected.')

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Error: Missing command line arguments.")
        print("Usage: python " + os.path.basename(__file__) + " [tanagra input text file] [output json file name] [optimize tree]")
        print("Example: python " + os.path.basename(__file__) + " tanagra_result.txt output_tree.json true")
    else:
        optimize_tree = False
        if len(sys.argv) > 3:
            optimize_tree = str2bool(sys.argv[3])
        main(sys.argv[1], sys.argv[2], optimize_tree)
        
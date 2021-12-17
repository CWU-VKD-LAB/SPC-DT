import tana2tree
import tana2tree as t2t


if __name__ == '__main__':
    input_file = "tanagra_output.txt"
    tree = t2t.Tanagra_Parser()
    tree.parse(input_file)
    node_list = tree.traverse()
    dict = tree.make_dict(unique_values=True)
    print(dict)

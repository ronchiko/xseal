import os
import os.path
import argparse

from blob import Blob
from query import query_all_files
from cfile import create_c_embedded_file


# File extensions to treat as text files
TEXT_FILE_EXTENSIONS = ['.txt', '.glsl', '.vert', '.frag', '.md', '.json', '.yml', '.yaml']


def __unpack_list_arguments(lst):
    return [item[0] for item in lst]


def _get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', action='append', type=str, dest='directories', nargs='+', help='The directory to search for the embedded files in.', default=None)
    parser.add_argument('-o', action='store', type=str, help='Where to put the result file.', dest='output', default='embed.cpp')
    parser.add_argument('--recurse', action='store_true', dest='recurse', default=False, help='Search recursively')
    parser.add_argument('-s', action='store', type=str, dest='subpath', default='', help='The directory to embed into.')

    return parser.parse_args()

def _make_internal_name(path, start=""):
    full_path = os.path.join(start, path)
    if full_path.startswith('\\') or full_path.startswith("/"):
        full_path = full_path[1:]
    
    return full_path


def _get_file_open_mode(file_name: str):
    _, ext = os.path.splitext(file_name)
    return 'r' if ext in TEXT_FILE_EXTENSIONS else 'rb'


def main():
    args = _get_arguments()

    # Generate the blob
    blob, parts = Blob(), {}
    for (file, relative_path) in query_all_files(__unpack_list_arguments(args.directories), args.recurse):
        with open(file, _get_file_open_mode(file)) as opened_file:
            parts[_make_internal_name(relative_path, start=args.subpath)] = blob.add_item(opened_file.read())

    # Write the output to a file.
    embedded_content = create_c_embedded_file(blob, parts)
    with open(args.output, "wb+") as output_file:
        output_file.write(bytes(embedded_content, encoding='ascii'))

if __name__ == '__main__':
    main()

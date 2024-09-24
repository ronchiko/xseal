import os
import os.path
from typing import List


def files_in_directory(directory: str, recurse: bool):
    for file in os.listdir(directory):
        path = os.path.join(directory, file)
        if os.path.isdir(path):
            if not recurse:
                continue
            
            for i in files_in_directory(path, recurse=recurse):
                yield i
        else:
            yield path

def query_all_files(roots: List[str], recurse: bool):
    for directory in (roots or []):
       for i in files_in_directory(directory, recurse=recurse):
           yield i, i.replace(directory, "")

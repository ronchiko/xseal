from typing import Dict, Tuple

from blob import Blob

DATA_VARAIBLE_NAME = "_EMBEDDED_DATA"

def __create_c_array(blob: Blob) -> str:
    return "static char {}[] = {{{}}}".format(DATA_VARAIBLE_NAME, ','.join([str(int(value)) for value in blob.data]))


def __create_embed_function(parts: Dict[str, Tuple[int]]):
    insertions = ['    seal_verify_result(seal::add_embedded_resource(\"{}\", {}, {}));'.\
                          format(part.replace("\\", "\\\\"), '{} + {}'.format(DATA_VARAIBLE_NAME, start), end - start)
                          for part, (start, end) in parts.items()]
    return """
seal::result<void> register_all_embedded_resources() {{
{}

    return {{}};
}}
""".format('\n'.join(insertions))


def create_c_embedded_file(blob: Blob, parts: Dict[str, Tuple[int]]) -> bytes:
    return """
/// ------------------------------------------------
/// This file was auto generated by XSeal
/// ------------------------------------------------

#include "seal/api/resource.hpp"
#include "seal/types/result.hpp"

{};

{}
""".format(__create_c_array(blob), __create_embed_function(parts))

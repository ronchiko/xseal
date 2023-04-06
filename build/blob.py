
class Blob:
    """
    A collection of binary data.
    """
    def __init__(self) -> None:
        self._data = bytes()

    def add_item(self, item) -> None:
        """
        Adds an item to the blob

        :param item: The item to add 
        """
        bytes_object, begin = self.__covert_to_bytes(item), len(self._data)
        self._data += bytes_object

        return (begin, len(self._data))

    @property
    def data(self) -> bytes:
        """
        The entire data of the blob object
        """
        return self._data

    @staticmethod
    def __covert_to_bytes(item):
        if isinstance(item, bytes):
            return item
        elif isinstance(item, str):
            return bytes(item, encoding='utf8') + bytearray([0])
        
        raise NotImplementedError(f"Cannot convert object of type {type(item).__name__} to bytes")
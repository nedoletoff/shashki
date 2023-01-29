import ctypes

libc = ctypes.CDLL("model/func.dll")


# board.c structure
class BoardSt(ctypes.Structure):
    _fields_ = [('grid', ctypes.c_char * 8 * 8),
                ('white_num', ctypes.c_char),
                ('grey_num', ctypes.c_char),
                ('turn', ctypes.c_char),
                ('no_eat_counter', ctypes.c_int),
                ('has_moves', ctypes.c_int)]


# coordinates_array.c structure

class CoordinatesSt(ctypes.Structure):
    _fields_ = [('height', ctypes.c_int),
                ('width', ctypes.c_int)]


class NodeSt(ctypes.Structure):
    pass


NodeSt._fields_ = [('coordinates', CoordinatesSt),
                   ('prev', ctypes.POINTER(NodeSt)),
                   ('next', ctypes.POINTER(NodeSt))]


class CoordinatesListSt(ctypes.Structure):
    _fields_ = [('head', ctypes.POINTER(NodeSt)),
                ('tail', ctypes.POINTER(NodeSt)),
                ('size', ctypes.c_int)]


'''class CoordinatesArraySt(ctypes.Structure):
    _fields_ = [('heights', ctypes.c_int * 16),
                ('widths', ctypes.c_int * 16),
                ('size', ctypes.c_int)]

'''
# board.c functions
libc.initBoard.restype = BoardSt
libc.is_game_over.restype = ctypes.c_char
libc.get_white_num.restype = ctypes.c_int
libc.get_grey_num.restype = ctypes.c_int
libc.move_cat.restype = ctypes.c_int

# coordinates_list functions
# libc.initCoordinatesArray.restype = ctypes.POINTER(CoordinatesArraySt)
libc.initCoordinatesArray.restype = CoordinatesArraySt
libc.get_movable_cat.restype = CoordinatesArraySt


def struct_list_to_python_list(cords_list: CoordinatesArraySt) -> list:
    res_list = list()
    print(cords_list.size)
    while cords_list.size > 0:
        cur = CoordinatesSt()
        cur_p = ctypes.pointer(cur)
        libc.pop_back(cords_list, cur_p)
        res_list.append(Cords(cur))
    return res_list


class Cords:
    def __init__(self, coordinates_st=None):
        self.height = -2
        self.width = -2
        if coordinates_st is not None:
            self.height = coordinates_st.height
            self.width = coordinates_st.width

    def to_text(self) -> str:
        res = "height - " + str(self.height) + ": width - " + str(self.width)
        return res

    def to_CoordinatesSt(self) -> CoordinatesSt:
        cur = CoordinatesSt()
        cur.height = self.height
        cur.width = self.width
        return cur


class Board:
    def __init__(self):
        self.board = libc.initBoard()
        self.movable = list()
        self.libc_arr = libc.initCoordinatesArray()

    def get_grid(self) -> list:
        grid = [[0 for i in range(8)] for j in range(8)]
        for i in range(8):
            for j in range(8):
                grid[i][j] = self.board.grid[i][j].decode("utf-8")
        return grid

    def is_game_ended(self) -> str:
        r = libc.is_game_over(self.board)
        return r.decode("utf-8")

    def get_movable(self) -> list:
        libc.destroy(self.libc_arr)
        self.movable = struct_list_to_python_list(libc.get_movable_cat(self.board, self.libc_arr))
        return self.movable

    def get_moves(self, cords: Cords) -> list:
        res = list()
        if cords in self.movable:
            libc.get_moves(self.board, cords.to_CoordinatesSt(), self.libc_arr)
            res = struct_list_to_python_list(self.libc_arr)
        return res


if __name__ == "__main__":
    b = Board()
    print(b.get_movable())

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


class CoordinatesArraySt(ctypes.Structure):
    _fields_ = [('heights', ctypes.c_int * 16),
                ('widths', ctypes.c_int * 16),
                ('size', ctypes.c_int)]


# board.c functions
libc.initBoard.restype = ctypes.POINTER(BoardSt)
libc.is_game_over.restype = ctypes.c_char
libc.get_white_num.restype = ctypes.c_int
libc.get_grey_num.restype = ctypes.c_int
libc.move_cat.restype = ctypes.c_int

# coordinates_list functions
#libc.initCoordinatesArray.restype = ctypes.POINTER(CoordinatesArraySt)


# libc.initCoordinatesList.restype = CoordinatesListSt


def struct_list_to_python_list(cords_list: CoordinatesArraySt) -> list:
    res_list = list()
    print(cords_list.size)
    while cords_list.size > 0:
        libc.print(cords_list)
        cur = CoordinatesSt()
        libc.pop_back(cords_list, cur)
        res_list.append(Cords(cur))
    return res_list


class Cords:
    def __init__(self, coordinates_st=None):
        self.height = -2
        self.width = -2
        if coordinates_st is not None:
            self.height = coordinates_st.height
            self.width = coordinates_st.width


class Board:
    def __init__(self):
        self.board = libc.initBoard()

        #libc.print(libc.initCoordinatesArray())
        #self.moves = CoordinatesArraySt()
        #self.eats = CoordinatesArraySt()
        print("Damn")
        self.moves = libc.initCoordinatesArray()
        print("Damn")
        self.eats = libc.initCoordinatesArray()

    def get_grid(self) -> list:
        grid = [[0 for i in range(8)] for j in range(8)]
        for i in range(8):
            for j in range(8):
                grid[i][j] = self.board.contents.grid[i][j].decode("utf-8")
        return grid

    def is_game_ended(self) -> str:
        r = libc.is_game_over(self.board)
        return r.decode("utf-8")

    def get_movable(self) -> list:
        # self.moves = CoordinatesListSt()
        libc.destroy(self.moves)
        libc.init(self.moves)
        #libc.push_back(self.moves, CoordinatesSt(13, 13))
        # self.eats = CoordinatesListSt()
        libc.destroy(self.eats)
        libc.init(self.eats)
        libc.get_movable_cat(self.board, self.moves, self.eats)

        print(self.moves)
        return struct_list_to_python_list(self.moves)


print("Hello world")
b = Board()
print(b.get_grid())
print(b.get_movable())
print("Hello world")
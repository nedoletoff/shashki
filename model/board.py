import ctypes
import datetime
import os
import pickle

path = "saves/"
libc = ctypes.CDLL("model/board.dll")


# board.c structure
class BoardSt(ctypes.Structure):
    _fields_ = [('grid', ctypes.c_char * 8 * 8),
                ('white_num', ctypes.c_char),
                ('grey_num', ctypes.c_char),
                ('turn', ctypes.c_char),
                ('eaten', ctypes.c_char),
                ('no_eat_counter', ctypes.c_int),
                ('has_moves', ctypes.c_int)]


# coordinates_list.c structure

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


class NodeBSt(ctypes.Structure):
    pass


NodeBSt._fields_ = [('value', BoardSt),
                    ('prev', ctypes.POINTER(NodeSt)),
                    ('next', ctypes.POINTER(NodeSt))]


class BoardsListSt(ctypes.Structure):
    _fields_ = [('head', ctypes.POINTER(NodeSt)),
                ('tail', ctypes.POINTER(NodeSt)),
                ('size', ctypes.c_int)]


# board.c functions
libc.initBoard.restype = BoardSt
libc.get_Board.restype = BoardSt
libc.is_game_over.restype = ctypes.c_char
libc.get_white_num.restype = ctypes.c_int
libc.get_grey_num.restype = ctypes.c_int
libc.move_cat.restype = ctypes.c_int

# coordinates_list functions
libc.initCoordinatesList.restype = CoordinatesListSt


class Cords:
    def __init__(self, coordinates_st=None, height=-2, width=-2):
        self.height = height
        self.width = width
        if coordinates_st is not None:
            self.height = coordinates_st.height
            self.width = coordinates_st.width

    def to_text(self) -> str:
        res = "height - " + str(self.height) + ": width - " + str(self.width)
        return res

    def to_symbolic_literal(self) -> str:
        literals = ('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H')
        nums = ('8', '7', '6', '5', '4', '3', '2', '1')
        res = literals[self.width] + nums[self.width]
        return res

    def to_CoordinatesSt(self) -> CoordinatesSt:
        if self is None:
            raise Exception("Cords are None")
        cur = CoordinatesSt()
        cur.height = self.height
        cur.width = self.width
        return cur

    def in_list(self, list_s) -> bool:
        for e in list_s:
            if e.height == self.height and e.width == self.width:
                return True
        return False


def c_list_p_to_python_list(list_p: ctypes.pointer) -> list:
    res = list()
    temp = ctypes.pointer(CoordinatesSt())
    while list_p.contents.size > 0:
        libc.pop_back(list_p, temp)
        res.append(Cords(temp.contents))
    return res


def create_savefile() -> str:
    if not os.path.exists(path):
        os.mkdir(path)
    filename = str(datetime.datetime.now().strftime('%Y_%m_%d_%H-%M-%S')) + '.txt'
    file = open(path + filename, 'w')
    file.close()
    filename = path + filename
    return filename


class Board:
    def __init__(self):
        self.board_p = ctypes.pointer(BoardSt())
        libc.initB(self.board_p)
        self.movable_list = list()
        self.list_p1 = ctypes.pointer(CoordinatesListSt())
        self.list_p2 = ctypes.pointer(CoordinatesListSt())
        self.has_eats = False
        self.savefile = create_savefile()
        self.move_num = 1
        self.lines_num = 0
        self.list_boards = ctypes.pointer(BoardsListSt())
        self.list_boards = list()
        self.save_board()

    def write_move(self, c1: Cords, c2: Cords):
        with open(self.savefile, 'a+') as saves:
            if self.lines_num != self.move_num:
                saves.write(str(self.move_num) + ':\t' + c1.to_symbolic_literal() + ':' + c2.to_symbolic_literal())
                self.lines_num += 1
            else:
                saves.write(':' + c2.to_symbolic_literal())

    def write_win(self, win: str):
        with open(self.savefile, 'a+') as saves:
            if win == 'w':
                saves.write("White wins\n")
            elif win == 'g':
                saves.write("Grey wins\n")
            elif win == 'o':
                saves.write("Draw\n")

    def write_newline(self):
        with open(self.savefile, 'a+') as saves:
            saves.write('\n')

    def delete_move(self):
        with open(self.savefile, 'r+') as saves:
            saves.seek(0, os.SEEK_END)
            pos = saves.tell() - 1

            while pos > 0 and (saves.read(1) != "\n" or saves.read(1) != ':'):
                pos -= 1
                saves.seek(pos, os.SEEK_SET)

            if pos > 0:
                saves.seek(pos, os.SEEK_SET)
                saves.truncate()

    def save_board(self):
        self.list_boards.append(pickle.dumps(self.board_p.contents))

    def change_board_to_prev(self):
        cur = self.list_boards[-1]
        self.list_boards.pop(-1)
        self.board_p.contents = pickle.loads(cur)

    def get_grid(self) -> list:
        grid = [[0 for i in range(8)] for j in range(8)]
        for i in range(8):
            for j in range(8):
                grid[i][j] = self.board_p.contents.grid[i][j].decode("utf-8")
        return grid

    def get_eaten(self) -> bool:
        return self.board_p.contents.eaten > bytes(1)

    def is_game_ended(self) -> str:
        r = libc.is_game_over(self.board_p)
        return r.decode("utf-8")

    def change_turn(self):
        libc.change_turn_and_finally_eat(self.board_p)
        self.move_num += 1
        self.write_newline()
        self.save_board()

    def get_turn(self) -> str:
        return self.board_p.contents.turn.decode("utf-8")

    def update_get_movable(self) -> list:
        libc.get_movable_cat(self.board_p, self.list_p1, self.list_p2)
        self.movable_list = c_list_p_to_python_list(self.list_p1)
        if len(self.movable_list) == 0:
            self.has_eats = True
            self.movable_list = c_list_p_to_python_list(self.list_p2)
        else:
            self.has_eats = False
        return self.movable_list

    def get_moves(self, cords: Cords) -> list:
        res = list()
        if cords.in_list(self.movable_list):
            libc.get_moves(self.board_p, cords.to_CoordinatesSt(), self.list_p1)
            res = c_list_p_to_python_list(self.list_p1)
        return res

    def move_cat(self, c1: Cords, c2: Cords) -> bool:
        if c2.in_list(self.get_moves(c1)):
            libc.move(self.board_p, c1.to_CoordinatesSt(), c2.to_CoordinatesSt())
            self.write_move(c1, c2)
            return True
        return False

    def _move_ai_cat(self, c1: Cords, c2: Cords):
        libc.move(self.board_p, c1.to_CoordinatesSt(), c2.to_CoordinatesSt())
        self.write_move(c1, c2)


    def can_eat(self, cell: Cords) -> bool:
        cell_st = cell.to_CoordinatesSt()
        return libc.is_able_to_move(self.board_p, cell_st) == 1

    def print_text(self):
        grid = self.get_grid()
        for i in grid:
            for j in i:
                print(j, end='')
            print()

    def make_ai_move(self):
        list_p = ctypes.pointer(CoordinatesListSt())
        libc.initCL(list_p)
        cur_cat = ctypes.pointer(CoordinatesSt())

        libc.get_ai_move(self.board_p, list_p, cur_cat)
        moves = c_list_p_to_python_list(list_p)
        cur = Cords(cur_cat.contents)
        libc.destroy(list_p)
        while len(moves) > 0:
            self._move_ai_cat(cur, moves[-1])
            cur = moves[-1]
            moves.pop(-1)


if __name__ == "__main__":
    b = Board()
    print(b.get_eaten())
    for e in b.update_get_movable():
        print(e.to_text())
        pass
    for e in b.get_moves(b.movable_list[0]):
        # print(e.to_text())
        pass
    c1 = Cords()
    c1.height = 5
    c1.width = 7
    c2 = Cords()
    c2.height = 4
    c2.width = 6
    print(b.move_cat(c1, c2))
    libc.print_board(b.board_p)
    print(b.is_game_ended())
    print(b.board_p.contents.turn)
    print(b.board_p.contents.no_eat_counter)

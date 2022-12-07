import ctypes
import struct

libc = ctypes.CDLL("./board.so")


class Board_st(ctypes.Structure):
    _fields_ = [('grid', ctypes.c_char[8][8]),
                ('white_num', ctypes.c_char),
                ('grey_num', ctypes.c_char),
                ('turn', ctypes.c_char),
                ('no_eat_counter', ctypes.c_int),
                ('has_moves', ctypes.c_int)]


class Coordinates_st(ctypes.Structure):
    _fields_ = [('height', ctypes.c_int),
                ('width', ctypes.c_int)]


class Node_t_st(ctypes.Structure):
    _fields_ = [('value', Coordinates_st),
                ('prev', ctypes.POINTER(Node_t_st)),
                ('next', ctypes.POINTER(Node_t_st))]


class Coordinates_list_st(ctypes.Structure):
    _fields_ = [('head', ctypes.POINTER(Node_t_st)),
                ('tail', ctypes.POINTER(Node_t_st)),
                ('size', ctypes.c_int)]

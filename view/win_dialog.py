import win32ui # если убрать перестает работать
from tkinter import *
import tkinter.messagebox
import ctypes

ctypes.windll.shcore.SetProcessDpiAwareness(True)


class WinDialog:
    def __init__(self, cat: str):
        message = ""
        if cat == 'w':
            message = "White wins"
        if cat == 'g':
            message = "Grey wins"
        if cat == 'o':
            message = "Draw"

        w = tkinter.messagebox.showinfo(title="Coshashki", message=message) # если убрать w = перестает работать


def open_win_dialog(cat: str):
    WinDialog(cat)


if __name__ == "__main__":
    p = WinDialog('w')

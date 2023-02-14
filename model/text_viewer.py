import win32ui # если убрать перестает работать
import logging

from tkinter import *
from tkinter import scrolledtext
from tkinter import filedialog

import ctypes

logging.basicConfig(
    filename="logs.log",
    format='%(asctime)s %(levelname)s %(message)s',
    level=logging.INFO,
    datefmt='%Y-%m-%d %H:%M:%S'
)
log = logging.getLogger("ex")
file_to_open = ''
file_types = [("Text Files", "*.txt"), ("Markdown", "*.md")]


def open_file():
    global file_to_open
    try:
        file_to_open = filedialog.askopenfilename(filetypes=file_types, initialdir="saves/")
    except Exception as e:
        log.exception(e)
    if file_to_open.__len__() == 0:
        log.exception(Exception("File name is null"))


def update(txt: scrolledtext.ScrolledText):
    global file_to_open
    with open(file_to_open) as f:
        txt.delete(1.0, END)
        txt.insert(1.0, f.read())


def open_file_and_update(txt: scrolledtext.ScrolledText):
    open_file()
    update(txt)


def open_list_files():
    ctypes.windll.shcore.SetProcessDpiAwareness(True)
    app_name = 'Просмотр игр'

    open_file()
    if file_to_open.__len__() == 0:
        return

    window = Tk()
    window.grid_columnconfigure(0, weight=1)
    window.title(app_name)
    window.geometry('300x600')

    txt = scrolledtext.ScrolledText(window, height=999)
    txt.grid(row=1, sticky=N + S + E + W)

    menu = Menu(window)

    file_dropdown = Menu(menu, tearoff=False)
    file_dropdown.add_command(label='Open', command=lambda: open_file_and_update(txt))
    menu.add_cascade(label='Another', menu=file_dropdown)
    window.config(menu=menu)

    update(txt)

    window.mainloop()
    return True


if __name__ == '__main__':
    open_list_files()

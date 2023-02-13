import tkinter.messagebox


class WinDialog():
    def __init__(self, cat: str):
        message = ""
        if cat == 'w':
            message = "White wins"
        if cat == 'g':
            message = "Grey wins"

        dialog = tkinter.messagebox.showinfo(title="Coshashki", message=message)


if __name__ == "__main__":
    p = WinDialog('w')

from PyQt6.QtWidgets import QApplication
import window
import sys


def open_window():
    app = QApplication(sys.argv)
    main_window = window.QMainWindow()
    main_window.show()


if __name__ == "__main__":
    open_window()

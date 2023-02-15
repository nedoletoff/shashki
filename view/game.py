import time

import pygame
import model.board
from view.text_viewer import *
from view.win_dialog import *
import logging

logging.basicConfig(
    filename="logs.log",
    format='%(asctime)s %(levelname)s %(message)s',
    level=logging.INFO,
    datefmt='%Y-%m-%d %H:%M:%S'
)
log = logging.getLogger("ex")


def is_in_rect(rect: list, mouse: tuple) -> bool:
    return rect[0] <= mouse[0] <= rect[0] + rect[2] and rect[1] <= mouse[1] <= rect[1] + rect[3]


def what_cell(mouse: tuple, size: int) -> model.board.Cords:
    res = model.board.Cords()
    gap = int(size / 8)
    res.width = -1
    res.height = -1
    if 0 < mouse[0] <= size and 0 < mouse[1] <= size:
        res.width = int(mouse[0] / gap)
        res.height = int(mouse[1] / gap)
    return res


class Game:
    def __init__(self):
        self.window_size = 768
        self.button_w = 40
        self.width = self.window_size
        self.height = self.window_size + self.button_w
        self.res = (self.width, self.height)

        pygame.init()
        pygame.display.set_caption("Coshashki")
        # load images
        pygame.display.set_icon(pygame.image.load("images/cotologo.png"))
        self.field = pygame.image.load("images/field.png")
        self.grey_cat = pygame.image.load("images/grey.png")
        self.white_cat = pygame.image.load("images/white.png")
        self.grey_king = pygame.image.load("images/grey_king.png")
        self.white_king = pygame.image.load("images/white_king.png")

        # white color
        self.color_white = (255, 255, 255)
        # light shade of the button
        self.color_light = (170, 170, 170)
        # dark shade of the button
        self.color_dark = (100, 100, 100)
        # light color for cells
        self.color_tan = (255, 243, 146)
        # dark color for cells
        self.color_brown = (121, 65, 0)
        # cell that you chose
        self.color_orange = (235, 168, 52)
        # cells suitable for the move
        self.color_blue = (76, 252, 241)
        # defining a fonts
        self.font_menu = pygame.font.SysFont('segoeui', 40)
        self.font_button = pygame.font.SysFont('segoeui', 20)
        self.screen = pygame.display.set_mode(self.res)

        self.rect_size_h = 55
        self.rect_size_w = int(self.window_size / 4)

        self.rect_width_sizes = [360, 308, 361, 106]
        self.rect_height_sizes = [int(2 * self.window_size / 8), int(3 * self.window_size / 8),
                                  int(4 * self.window_size / 8), int(5 * self.window_size / 8)]

    def draw_buttons(self):
        mouse = pygame.mouse.get_pos()
        give_up_text = self.font_button.render('Сдаться', False, self.color_white)
        back_text = self.font_button.render('Отмена хода', False, self.color_white)
        if is_in_rect(l := [0, self.window_size, int(self.window_size / 2), self.button_w], mouse):
            pygame.draw.rect(self.screen, self.color_light, l)
        else:
            pygame.draw.rect(self.screen, self.color_dark, l)

        if is_in_rect(l := [w := int(self.window_size / 2), self.window_size, w, self.button_w], mouse):
            pygame.draw.rect(self.screen, self.color_light, l)
        else:
            pygame.draw.rect(self.screen, self.color_dark, l)

        self.screen.blit(give_up_text,
                         (self.width / 8 + 68, self.window_size + 5))
        self.screen.blit(back_text,
                         (5 * self.width / 8 + 55, self.window_size + 5))

    def start_menu(self):
        # rendering a text written in
        # this font
        start_human_text = self.font_menu.render('Игра с человеком', True, self.color_white)
        start_bot_text = self.font_menu.render('Игра с компьютером', True, self.color_white)
        story_text = self.font_menu.render('Предыдущие партии', True, self.color_white)
        quit_text = self.font_menu.render('Выход', True, self.color_white)
        while True:
            mouse = pygame.mouse.get_pos()
            self.screen.blit(self.field, (0, 0))
            for ev in pygame.event.get():

                if ev.type == pygame.QUIT:
                    pygame.quit()
                    return 'quit'

                    # checks if a mouse is clicked
                if ev.type == pygame.MOUSEBUTTONDOWN:
                    # if the mouse is clicked on the
                    for i in range(4):
                        if is_in_rect([self.rect_size_w, self.rect_height_sizes[i],
                                       self.rect_width_sizes[i], self.rect_size_h], mouse):
                            # button the game is terminated
                            if i == 0:
                                return "bot_game"
                            if i == 1:
                                return "human_game"
                            if i == 2:
                                return "story"
                            if i == 3:
                                pygame.quit()
                                return 'quit'

            # fills the screen with a color
            for i in range(4):
                if is_in_rect(l := [self.rect_size_w, self.rect_height_sizes[i],
                                    self.rect_width_sizes[i], self.rect_size_h], mouse):
                    pygame.draw.rect(self.screen, self.color_light, l)
                else:
                    pygame.draw.rect(self.screen, self.color_dark, l)

            pygame.draw.rect(self.screen, self.color_dark, [0, self.window_size,
                                                            self.window_size, self.button_w])
            self.screen.blit(start_bot_text,
                             (self.rect_size_w, self.rect_height_sizes[0]))
            self.screen.blit(start_human_text,
                             (self.rect_size_w, self.rect_height_sizes[1]))
            self.screen.blit(story_text,
                             (self.rect_size_w, self.rect_height_sizes[2]))
            self.screen.blit(quit_text,
                             (self.rect_size_w, self.rect_height_sizes[3]))
            # updates the frames of the game
            pygame.display.update()

    def draw_game(self, mode):
        state = "not selected, move has not done"
        cur = None
        selected = None
        board = model.board.Board()
        movable_cats = board.update_get_movable()
        moves = list()
        field_list = board.get_grid()
        has_eat = board.get_eaten()

        def draw_cats(f_list: list):
            for i, ipos in enumerate(range(0, self.window_size, self.window_size // 8)):
                for j, jpos in enumerate(range(0, self.window_size, self.window_size // 8)):
                    if f_list[i][j] == 'w':
                        self.screen.blit(self.white_cat, (jpos, ipos))
                    if f_list[i][j] == 'W':
                        self.screen.blit(self.white_king, (jpos, ipos))
                    if f_list[i][j] == 'g':
                        self.screen.blit(self.grey_cat, (jpos, ipos))
                    if f_list[i][j] == 'G':
                        self.screen.blit(self.grey_king, (jpos, ipos))

        def draw_highlighted(clicked: model.board.Cords, mode: bool):
            global moves, movable_cat
            gap = self.window_size // 8
            if mode or clicked is None:  # highlight movable cats
                for cat in movable_cats:
                    pygame.draw.rect(self.screen, self.color_blue,
                                     (cat.width * gap, cat.height * gap, gap, gap))
            else:
                pygame.draw.rect(self.screen, self.color_orange,
                                 (clicked.width * gap, clicked.height * gap, gap, gap))
                moves = board.get_moves(clicked)
                for cell in moves:
                    pygame.draw.rect(self.screen, self.color_blue,
                                     (cell.width * gap, cell.height * gap, gap, gap))

        while True:
            if mode == 'bot' and board.get_turn() == 'g' and board.is_game_ended() == 'n':
                time.sleep(0.03)
                board.make_ai_move()
                board.change_turn()
                state = "not selected, move has not done"
                cur = None
                selected = None
                movable_cats = board.update_get_movable()
                moves = list()
                field_list = board.get_grid()
                has_eat = board.get_eaten()
                if (cat := board.is_game_ended()) != 'n':
                    board.write_win(cat)
                    open_win_dialog(cat)
                    break
            time.sleep(0.1)
            mouse = pygame.mouse.get_pos()
            self.draw_buttons()
            self.screen.blit(self.field, (0, 0))
            for ev in pygame.event.get():
                if ev.type == pygame.QUIT:
                    pygame.quit()
                    return
                if ev.type == pygame.MOUSEBUTTONDOWN:
                    if is_in_rect(l := [0, self.window_size, int(self.window_size / 2), self.button_w], mouse):
                        if board.get_turn() == 'w':
                            open_win_dialog('g')
                            board.write_win('g')
                            return
                        else:
                            open_win_dialog('w')
                            board.write_win('w')
                            return

                    if is_in_rect(l := [w := int(self.window_size / 2), self.window_size, w, self.button_w], mouse):
                        board.change_board_to_prev()
                        state = "not selected, move has not done"
                        selected = None
                        movable_cats = board.update_get_movable()
                        moves = list()
                        field_list = board.get_grid()
                        has_eat = board.get_eaten()

                    cur = what_cell(mouse, self.window_size)
                    if state == "not selected, move has not done":  # cell unselected, move have not done
                        if cur.in_list(movable_cats):
                            state = "selected, move has not done"
                            selected = cur
                        else:
                            state = "not selected, move has not done"
                            cur = None
                            selected = None
                    elif state == "selected, move has not done":
                        if cur.in_list(board.get_moves(selected)):
                            board.move_cat(selected, cur)
                            field_list = board.get_grid()
                            state = "selected, move has done"
                            has_eat = board.get_eaten()
                            selected = cur
                        else:
                            selected = None
                            cur = None
                            state = "not selected, move has not done"
                    if state == "selected, move has done":
                        # board.print_text()
                        if has_eat and board.can_eat(selected):
                            if cur.in_list(board.get_moves(selected)):
                                board.move_cat(selected, cur)
                                field_list = board.get_grid()
                                state = "selected, move has done"
                                has_eat = True
                                selected = cur
                        else:
                            board.change_turn()
                            state = "not selected, move has not done"
                            cur = None
                            selected = None
                            movable_cats = board.update_get_movable()
                            moves = list()
                            field_list = board.get_grid()
                            has_eat = board.get_eaten()

            movable_cats = board.update_get_movable()
            draw_highlighted(cur, state == "not selected, move has not done")
            draw_cats(field_list)
            pygame.display.update()

            if (cat := board.is_game_ended()) != 'n':
                board.write_win(cat)
                open_win_dialog(cat)
                print(board.is_game_ended(), " win")
                break


def main():
    game = Game()
    mode = game.start_menu()
    while True:
        if mode == 'bot_game':
            game.draw_game('bot')
            mode = game.start_menu()
        if mode == 'human_game':
            game.draw_game('human')
            mode = game.start_menu()
        if mode == 'story':
            try:
                open_list_files()
            except Exception as e:
                log.exception(e)
            mode = game.start_menu()
        if mode == 'quit':
            break


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        log.exception(e)

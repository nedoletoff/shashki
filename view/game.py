import pygame
import model.board
import sys
import subprocess


def is_in_rect(rect: list, mouse: tuple) -> bool:
    return rect[0] <= mouse[0] <= rect[0] + rect[2] and rect[1] <= mouse[1] <= rect[1] + rect[3]


class Game:
    def __init__(self):
        self.window_size = 768
        self.button_w = 40
        self.width = self.window_size
        self.height = self.window_size + self.button_w
        self.res = (self.width, self.height)

        pygame.init()
        pygame.display.set_caption("Кошашки")
        pygame.display.set_icon(pygame.image.load("images/cotologo.png"))
        self.field = pygame.image.load("images/field.png")

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
                    return

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
                                return

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


def main():
    game = Game()
    mode = game.start_menu()
    print(mode)


if __name__ == '__main__':
    main()

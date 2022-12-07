import pygame
import sys
import subprocess

pygame.init()
SIZE = 768
res = (SIZE, SIZE)
screen = pygame.display.set_mode(res)
pygame.display.set_caption("Кошашки")
pygame.display.set_icon(pygame.image.load("../images/cotologo.png"))
field = pygame.image.load("../images/field.png")

# white color
color = (255, 255, 255)

# light shade of the button
color_light = (170, 170, 170)

# dark shade of the button
color_dark = (100, 100, 100)
TAN = (255, 243, 146)
BROWN = (121, 65, 0)

width = screen.get_width()
height = screen.get_height()

# defining a font
smallfont = pygame.font.SysFont('Calibri', 40)

# rendering a text written in
# this font
start_human_text = smallfont.render('Игра с человеком', True, color)
start_bot_text = smallfont.render('Игра с компьютером', True, color)
story_text = smallfont.render('Предыдущие партии', True, color)
quit_text = smallfont.render('Выход', True, color)


def is_in_rect(rect: list) -> bool:
    return rect[0] <= mouse[0] <= rect[0] + rect[2] and rect[1] <= mouse[1] <= rect[1] + rect[3]


while True:
    for ev in pygame.event.get():

        if ev.type == pygame.QUIT:
            pygame.quit()

            # checks if a mouse is clicked
        if ev.type == pygame.MOUSEBUTTONDOWN:

            # if the mouse is clicked on the
            # button the game is terminated
            if is_in_rect([width / 4, 2 * height / 8, 349, 40]):
                print("Start game with bot")

            if is_in_rect([width / 4, 3 * height / 8, 297, 40]):
                print("Start game with human")

            if is_in_rect([width / 4, 4 * height / 8, 351, 40]):
                print("Show story")

            if is_in_rect([width / 4, 5 * height / 8, 107, 40]):
                pygame.quit()
                # fills the screen with a color
    screen.blit(field, (0, 0))
    mouse = pygame.mouse.get_pos()

    if is_in_rect(l := [width / 4, 2 * height / 8, 349, 40]):
        pygame.draw.rect(screen, color_light, l)
    else:
        pygame.draw.rect(screen, color_dark, l)

    if is_in_rect(l := [width / 4, 3 * height / 8, 297, 40]):
        pygame.draw.rect(screen, color_light, l)
    else:
        pygame.draw.rect(screen, color_dark, l)

    if is_in_rect(l := [width / 4, 4 * height / 8, 351, 40]):
        pygame.draw.rect(screen, color_light, l)
    else:
        pygame.draw.rect(screen, color_dark, l)

    if is_in_rect(l := [width / 4, 5 * height / 8, 107, 40]):
        pygame.draw.rect(screen, color_light, l)
    else:
        pygame.draw.rect(screen, color_dark, l)

    start_bot_size = screen.blit(start_bot_text, (width / 4, 2 * height / 8))
    start_human_size = screen.blit(start_human_text, (width / 4, 3 * height / 8))
    story_size = screen.blit(story_text, (width / 4, 4 * height / 8))
    quit_size = screen.blit(quit_text, (width / 4, 5 * height / 8))

    # updates the frames of the game
    pygame.display.update()

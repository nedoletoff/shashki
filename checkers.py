import pygame
import random
import sys
from itertools import combinations

SIZE = 800
ROWS = 8

WHITE_CAT = pygame.image.load('.\\images\\white.png')
GREY_CAT = pygame.image.load('.\\images\\grey.png')

WHITE_KING = pygame.image.load('.\\images\\white_king.png')
GRAY_KING = pygame.image.load('.\\images\\grey_king.png')

TAN = (255, 243, 146)
BROWN = (121, 65, 0)
ORANGE = (235, 168, 52)
BLUE = (76, 252, 241)

pygame.init()
WIN = pygame.display.set_mode((SIZE, SIZE))
pygame.display.set_caption('Кошашки')
pygame.display.set_icon(pygame.image.load(".\\images\\cotologo.png"))

priorMoves = []


class Node:
    def __init__(self, row, col, width):
        self.row = row
        self.col = col
        self.x = int(row * width)
        self.y = int(col * width)
        self.colour = TAN
        self.piece = None

    def draw(self, win):
        pygame.draw.rect(win, self.colour, (self.x, self.y, SIZE / ROWS, SIZE / ROWS))
        if self.piece:
            win.blit(self.piece.image, (self.x, self.y))


def update_display(win, grid, rows, width):
    for row in grid:
        for spot in row:
            spot.draw(win)
    draw_grid(win, rows, width)
    pygame.display.update()


def make_grid(rows, width):
    grid = []
    gap = width // rows
    count = 0
    for i in range(rows):
        grid.append([])
        for j in range(rows):
            node = Node(j, i, gap)
            if abs(i - j) % 2 == 0:
                node.colour = BROWN
            if (abs(i + j) % 2 == 0) and (i < 3):
                node.piece = Piece('R')
            elif (abs(i + j) % 2 == 0) and i > 4:
                node.piece = Piece('G')
            count += 1
            grid[i].append(node)
    return grid


def draw_grid(win, rows, width):
    gap = width // ROWS
    for i in range(rows):
        pygame.draw.line(win, BROWN, (0, i * gap), (width, i * gap))
        for j in range(rows):
            pygame.draw.line(win, BROWN, (j * gap, 0), (j * gap, width))


class Piece:
    def __init__(self, team):
        self.team = team
        self.image = WHITE_CAT if self.team == 'R' else GREY_CAT
        self.type = None

    def draw(self, x, y):
        WIN.blit(self.image, (x, y))


def get_node(rows, width):
    gap = width // rows
    row_x, row_y = pygame.mouse.get_pos()
    row = row_x // gap
    col = row_y // gap
    return col, row


def reset_colours(grid, node):
    positions = generate_potential_moves(node, grid)
    positions.append(node)

    for colouredNodes in positions:
        node_x, node_y = colouredNodes
        grid[node_x][node_y].colour = BROWN if abs(node_x - node_y) % 2 == 0 else TAN


def highlight_potential_moves(piece_position, grid):
    positions = generate_potential_moves(piece_position, grid)
    for position in positions:
        column, row = position
        grid[column][row].colour = BLUE


def opposite(team):
    return "R" if team == "G" else "G"


def generate_potential_moves(node_position, grid):
    checker = lambda x, y: 0 <= x + y < 8
    positions = []
    column, row = node_position
    if grid[column][row].piece:
        vectors = [[1, -1], [1, 1]] if grid[column][row].piece.team == "R" else [[-1, -1], [-1, 1]]
        if grid[column][row].piece.type == 'KING':
            vectors = [[1, -1], [1, 1], [-1, -1], [-1, 1]]
        for vector in vectors:
            column_vector, row_vector = vector
            if checker(column_vector, column) and checker(row_vector, row):
                # grid[(column+column_vector)][(row+row_vector)].colour=ORANGE
                if not grid[(column + column_vector)][(row + row_vector)].piece:
                    positions.append((column + column_vector, row + row_vector))
                elif grid[column + column_vector][row + row_vector].piece and \
                        grid[column + column_vector][row + row_vector].piece.team == opposite(
                    grid[column][row].piece.team):

                    if checker((2 * column_vector), column) and checker((2 * row_vector), row) \
                            and not grid[(2 * column_vector) + column][(2 * row_vector) + row].piece:
                        positions.append((2 * column_vector + column, 2 * row_vector + row))

    return positions


def highlight(clicked_node, grid, old_highlight):
    column, row = clicked_node
    grid[column][row].colour = ORANGE
    if old_highlight:
        reset_colours(grid, old_highlight)
    highlight_potential_moves(clicked_node, grid)
    return column, row


def move(grid, piece_position, new_position):
    reset_colours(grid, piece_position)
    new_column, new_row = new_position
    old_column, old_row = piece_position

    piece = grid[old_column][old_row].piece
    grid[new_column][new_row].piece = piece
    grid[old_column][old_row].piece = None

    if new_column == 7 and grid[new_column][new_row].piece.team == 'R':
        grid[new_column][new_row].piece.type = 'KING'
        grid[new_column][new_row].piece.image = WHITE_KING
    if new_column == 0 and grid[new_column][new_row].piece.team == 'G':
        grid[new_column][new_row].piece.type = 'KING'
        grid[new_column][new_row].piece.image = GRAY_KING
    if abs(new_column - old_column) == 2 or abs(new_row - old_row) == 2:
        grid[int((new_column + old_column) / 2)][int((new_row + old_row) / 2)].piece = None
        return grid[new_column][new_row].piece.team
    return opposite(grid[new_column][new_row].piece.team)


def main(size, rows):
    global piece_column, piece_row
    grid = make_grid(rows, size)
    highlighted_piece = None
    curr_move = 'G'

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                print('EXIT SUCCESSFUL')
                pygame.quit()
                sys.exit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                clicked_node = get_node(rows, size)
                clicked_position_column, clicked_position_row = clicked_node
                if grid[clicked_position_column][clicked_position_row].colour == BLUE:
                    if highlighted_piece:
                        piece_column, piece_row = highlighted_piece
                    if curr_move == grid[piece_column][piece_row].piece.team:
                        reset_colours(grid, highlighted_piece)
                        curr_move = move(grid, highlighted_piece, clicked_node)
                elif highlighted_piece == clicked_node:
                    pass
                else:
                    if grid[clicked_position_column][clicked_position_row].piece:
                        if curr_move == grid[clicked_position_column][clicked_position_row].piece.team:
                            highlighted_piece = highlight(clicked_node, grid, highlighted_piece)

        update_display(WIN, grid, rows, size)


if __name__ == "__main__":
    main(SIZE, ROWS)

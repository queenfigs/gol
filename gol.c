#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// globals
int size = 0;        // for an nxn board
char **board = NULL; // as a 2D array
char **tmp = NULL;   // copy of the board for updating

void init_board()
{
  size_t s = (size_t)size;
  char *board_arry = (char *)calloc(s * s, sizeof(char)); // the actual data
  char *tmp_arry = (char *)calloc(s * s, sizeof(char));   // the actual data
  board = (char **)malloc(s * sizeof(char *));
  tmp = (char **)malloc(s * sizeof(char *));
  // initialize the row indices
  for (int r = 0; r < size; r++)
  {
    board[r] = board_arry + r * size; // &board_arry[r * size]
    tmp[r] = tmp_arry + r * size;
  }

  // R: e.g. 4x4
  // 0: 0 0 1 0
  // 1: 0 0 0 1
  // 2: 0 1 1 1
  // 3: 0 0 0 0

  if (size > 3)
  {
    board[0][2] = 1;
    board[1][3] = 1;
    board[2][1] = 1;
    board[2][2] = 1;
    board[2][3] = 1;
  }
}

/*
Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void update_square(int row, int col)
{
  int sum = 0;
  char alive = board[row][col];
  for (int r = row - 1; r <= row + 1; r++)
  {
    for (int c = col - 1; c <= col + 1; c++)
    {
      if (r >= 0 && r < size &&    // don't go off left or right
          c >= 0 && c < size &&    // don't go off top or bottom
          !(r == row && c == col)) // don't count the square itself
      {
        sum += board[r][c]; // count the neighbors
      }
    }
  }
  if (alive)
  {
    if (sum < 2 || sum > 3)
    {
      alive = 0;
    }
  }
  else
  {
    if (sum == 3)
    {
      alive = 1;
    }
  }
  tmp[row][col] = alive;
}

void update_board()
{
  for (int r = 0; r < size; r++)
  {
    for (int c = 0; c < size; c++)
    {
      update_square(r, c);
    }
  }
  // copy the tmp board to the real board
  for (int r = 0; r < size; r++)
  {
    for (int c = 0; c < size; c++)
    {
      board[r][c] = tmp[r][c];
    }
  }
}

void console_cls()
{
  // Clear the console screen
  printf("\033[H\033[J");
}

void print_board()
{
  int r, c;
  console_cls();
  for (r = 0; r < size; r++)
  {
    for (c = 0; c < size; c++)
    {
      if (board[r][c])
      {
        printf("X  ");
      }
      else
      {
        printf("   ");
      }
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  size = 16; // default
  if (argc == 2)
  {
    size = (int)strtol(argv[1], NULL, 10);
  }

  printf("gameOfLife %d x %d\n", size, size);
  init_board();
  for (int i = 0; i < 53; i++)
  {
    print_board();
    update_board();
    sleep(1);
  }

  return 0;
}

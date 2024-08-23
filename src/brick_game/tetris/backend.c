#include "backend.h"

GameInfo_t *getGameInfo_t() {
  static GameInfo_t *stats = NULL;
  if (stats == NULL) {
    stats = malloc(sizeof(GameInfo_t));
    if (stats != NULL) {
      stats->field = (int **)malloc(HEIGHT * sizeof(int *));
      if (stats->field != NULL) {
        for (int i = 0; i < HEIGHT; i++) {
          stats->field[i] = (int *)malloc(WIDTH * sizeof(int));
        }
      } else {
        errorQuit("couldn't manage to allocate memory");
      }

      stats->falling = malloc(CELLS * sizeof(Tblock));
      stats->next = malloc(CELLS * sizeof(Tblock));
      if (stats->next == NULL || stats->falling == NULL) {
        errorQuit("couldn't manage to allocate memory");
      }
      stats->pause = 0;
      stats->started = 0;
      stats->stop = 0;
      stats->score = 0;
      stats->high_score = 0;
      stats->ticks = 1;
      stats->level = 1;

    } else {
      errorQuit("couldn't manage to allocate memory");
    }
  }
  return stats;
}

int initGame() {
  GameInfo_t *stats = getGameInfo_t();
  while (1) {
    bool hold = stats->started;

    int key = getch();
    UserAction_t action;
    parseKey(key, &action);
    userInput(action, hold);
  }
  return EXIT_SUCCESS;
}

void startGame() {
  GameInfo_t *stats = getGameInfo_t();
  FILE *file = fopen("record.txt", "r+");
  int hi_score = 0;
  if (file != NULL) {
    fscanf(file, "%d", &hi_score);
  }
  fclose(file);
  stats->high_score = hi_score;
  stats->started = 1;
  srand((unsigned)time(NULL));

  stats->next[0].type = rand() % 7;
  initFigure();
}

void playAgain() {
  GameInfo_t *stats = getGameInfo_t();
  stats->score = 0;
  stats->level = 1;
  initFigure();
  stats->stop = 0;
}

void initFigure() {
  GameInfo_t *stats = getGameInfo_t();
  stats->falling[0].type = stats->next[0].type;
  stats->next[0].type = rand() % 7;
  int type_falling = 0;
  int type_next = 0;
  type_falling = stats->falling[0].type;
  type_next = stats->next[0].type;

  for (int i = 0; i < CELLS; i++) {
    stats->falling[i].orintation = 0;
    stats->falling[i].type = stats->falling[0].type;

    Tlocation cell_f = getTetr(type_falling, 0, i);
    stats->falling[i].loc.row = cell_f.row;
    stats->falling[i].loc.col = cell_f.col;

    stats->next[i].orintation = stats->next[0].type;
    Tlocation cell_n = getTetr(type_next, 0, i);
    stats->next[i].loc.row = cell_n.row;
    stats->next[i].loc.col = cell_n.col;
  }
  putFigureOnField();
}

void putFigureOnField() {
  GameInfo_t *stats = getGameInfo_t();
  Tblock *temp = stats->falling;
  for (int i = 0; i < CELLS; i++) {
    if (stats->started && stats->stop == 0) {
      if (stats->field[temp[i].loc.row][temp[i].loc.col] == 1) {
        TerminatePushed();
      } else {
        stats->field[temp[i].loc.row][temp[i].loc.col] = 1;
      }
    }
  }
}

void moveDown() {
  GameInfo_t *stats = getGameInfo_t();
  int x, y;

  if (mayItGoDown()) {
    rmTetraminoFromField();
    for (int i = 0; i < CELLS; i++) {
      if (stats->falling[i].loc.row < HEIGHT - 1) {
        x = stats->falling[i].loc.col;
        y = stats->falling[i].loc.row;
        stats->falling[i].loc.row = y + 1;
        stats->field[y + 1][x] = 1;
      }
    }
  } else {
    initFigure();
  }
}

void rmTetraminoFromField() {
  GameInfo_t *stats = getGameInfo_t();
  for (int i = 0; i < CELLS; i++) {
    int x = stats->falling[i].loc.col;
    int y = stats->falling[i].loc.row;
    stats->field[y][x] = 0;
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t *stats = getGameInfo_t();
  return *stats;
}

void userInput(UserAction_t action, bool hold) {
  if (!hold && action == Start) {
    startGame();
  }
  if (hold) {
    switch (action) {
      case Start:
        break;
      case Pause:
        pauseGame();
        break;
      case Action:
        rotateFigure();
        break;
      case Down:
        moveFigure(action);
        break;
      case Left:
        moveFigure(action);
        break;
      case Right:
        moveFigure(action);
        break;
      case Terminate:
        TerminatePushed();
        break;
      case Up:
        break;
      default:
        break;
    }
  }
}

void pauseGame() {
  GameInfo_t *stats = getGameInfo_t();
  if (stats->pause == 1) {
    stats->pause = 0;
  } else {
    stats->pause = 1;
  }
}

int fullLine(int line) {
  int res = 1;
  GameInfo_t *stats = getGameInfo_t();
  for (int i = 0; i < WIDTH; i++) {
    if (stats->field[line][i] != 1) {
      res = 0;
    }
  }
  if (res) {
  }
  return res;
}

int getGravity(int index) {
  const int speed[11] = {// 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10
                         0, 17, 16, 15, 14, 13, 12, 11, 9, 8, 7};

  return speed[index];
}

void parseKey(int key, UserAction_t *action) {
  GameInfo_t *stats = getGameInfo_t();
  switch (key) {
    case 's':
    case 'S':
      if (stats->started) {
        *action = Pause;
      } else
        *action = Start;
      break;
    case 'r':
    case 'R':
      *action = Action;
      break;
    case KEY_DOWN:
      *action = Down;
      break;
    case KEY_LEFT:
      *action = Left;
      break;
    case KEY_RIGHT:
      *action = Right;
      break;
    case 'Q':
    case 'q':
      *action = Terminate;
      break;
    case 'N':
    case 'n':
      if (stats->stop == 1) {
        quit();
      }
      break;
    case 'Y':
    case 'y':
      if (stats->stop == 1) {
        playAgain();
      }
      break;
    default:
      *action = Up;
  }
}

void quit() {
  GameInfo_t *stats = getGameInfo_t();
  FILE *file = fopen("record.txt", "w");
  fprintf(file, "%d", stats->high_score);
  fclose(file);
  quitNcurses();
  freeMap();
  exit(EXIT_SUCCESS);
}

void TerminatePushed() {
  GameInfo_t *stats = getGameInfo_t();
  stats->stop = 1;

  for (int j = 0; j < CELLS; j++) {
    stats->next[j].loc.row = 0;
    stats->next[j].loc.col = 0;
    stats->next[j].type = rand() % 7;

    stats->falling[j].loc.row = 0;
    stats->falling[j].loc.col = 0;
  }
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      stats->field[i][j] = 0;
    }
  }
}

void freeMap() {
  GameInfo_t *stats = getGameInfo_t();
  for (int i = 0; i < HEIGHT; i++) {
    free(stats->field[i]);
  }
  free(stats->falling);
  free(stats->field);
}

void errorQuit(char *msg) {
  quitNcurses();
  perror(msg);
  exit(EXIT_FAILURE);
}

void quitNcurses() {
  delwin(stdscr);
  curs_set(0);
  endwin();
  refresh();
}
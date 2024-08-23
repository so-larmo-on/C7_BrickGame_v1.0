/**
 * @file frontend.h
 * @brief Файл содержит функции отрисовки игры
 */

#ifndef FRONTEND_H
#define FRONTEND_H

#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>

#include "../../brick_game/tetris/backend.h"

/**
 * @brief Макрос на инициализацию ncurses
 */
#define CURSINIT()                          \
  {                                         \
    if (initscr() == NULL) {                \
      perror("error initialising ncurses"); \
      exit(EXIT_FAILURE);                   \
    }                                       \
    cbreak();                               \
    noecho();                               \
    keypad(stdscr, TRUE);                   \
    curs_set(0);                            \
    timeout(0);                             \
  }

/**
 * @brief значение для отрисовки поля
 */
#define HUD_WIDTH 12

/**
 * @brief значение для отрисовки поля
 */
#define BOARDS_BEGIN 2

/**
 * @brief сообщения в начале
 */
#define INTRO_MESSAGE "TO START PRESS S"

/**
 * @brief длина соощения в начале
 */
#define INTRO_MESSAGE_LEN (int)strlen(INTRO_MESSAGE)

/**
 * @brief сообщение в конце
 */
#define FINAL_MESSAGE_1 "GAME OVER"

/**
 * @brief сообщение в конце
 */
#define FINAL_MESSAGE_2 "Your score is %d"

/**
 * @brief сообщение в конце
 */
#define FINAL_MESSAGE_3 "Sour highscore is %d"

/**
 * @brief сообщение в конце
 */
#define FINAL_MESSAGE_4 "Play over? [y/n]"

/**
 * @brief Макрос на отрисовку заполненного блока
 */
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

/**
 * @brief Макрос на отрисовку частей поля
 */
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)

/**
 * @brief Функция отрисовки прямоугольников
 */
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Функция обновления поля
 */
void updateField();

/**
 * @brief Функция прекращения работы библиотеки ncurses
 */
void quitNcurses();

/**
 * @brief Функция рисования начального сообщения
 */
void drawStartMessage();

/**
 * @brief Функция отрисовки стартового поля
 */
void drawRecs();

/**
 * @brief Функция отрисовки превью фигуры
 */
void DrawNext();

/**
 * @brief Функция отрисовки финальных сообщений
 */
void DrawAgain();

/**
 * @brief Функция инициализации цветов
 */
void init_colors();
#endif
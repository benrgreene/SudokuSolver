extern int board[9][9];
extern int chunk;

#ifndef MAIN_
#define MAIN_

void print_board();

#endif

#ifndef HELP_
#define HELP_

int board_complete();
int num_available(int pos_x, int pos_y);
int is_available(int pos_x, int pos_y, int num);
int break_candidate();
int break_rows();
int break_cols();
int last_chance();
void set_chunk(int c);

#endif
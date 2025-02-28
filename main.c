#include <conio.h>
#include <stdio.h>
#include <windows.h>

#define TRUE        1
#define FALSE       0
#define UP          'w'
#define DOWN        's'
#define LEFT        'a'
#define RIGHT       'd'
#define SELECT      '\r'    // Enter key
#define QUIT        '\e'    // Esc key
#define RESET       'r'
#define coord(c, r) (COORD){c, r}

#define MIN_HEIGHT  3
#define MAX_HEIGHT  15
#define MIN_WIDTH   3
#define MAX_WIDTH   15
#define MIN_PLAYERS 2
#define MAX_PLAYERS 4

#define VOID_CHAR   ' '
#define PLAYER_1_CHAR 'O'
#define PLAYER_2_CHAR 'X'
#define PLAYER_3_CHAR 'T'
#define PLAYER_4_CHAR 'Z'
#define DRAW        'D'

// General variables
char running = TRUE;
char input;
char arrow;
char *arrow_txt[2];
char arrow_len[2];

// Console managment variables
HWND window_handle;
HANDLE console_handle;
CONSOLE_CURSOR_INFO cursor_info;
CONSOLE_FONT_INFO font_info;
CONSOLE_SCREEN_BUFFER_INFO console_info;
DWORD characters;
DWORD style;
const char *space = " ";
const int CONSOLE_WIDTH = 60;
const int CONSOLE_HEIGHT = 30;

// Scene variables
enum scene {
    SCENE_MENU,
    SCENE_LOCAL_GAME,
};
enum scene actual_scene = SCENE_MENU;

enum popup {
    POPUP_EXIT_PROGRAM,
    POPUP_LEFT_GAME,
    POPUP_RESET_GAME,

    POPUP_COUNT
};
char *popup_txt[POPUP_COUNT];
char popup_len[POPUP_COUNT];

char *no_txt;
char no_len;
char *yes_txt;
char yes_len;

char *menu_hint_txt[3];
char menu_hint_len[3];

char *play_txt;
char play_len;
char *back_txt;
char back_len;

char *customize_game_hint_txt[3];
char customize_game_hint_len[3];

char *player_turn_txt;
char player_turn_len;

char *draw_txt;
char draw_len;
char *winner_txt[2];
char winner_len[2];

// Menu variables
enum menu_options {
    PLAY_LOCAL,
    EXIT,

    OPTIONS_COUNT
};
char *option_txt[OPTIONS_COUNT];
char option_len[OPTIONS_COUNT];

// Game variables
char tile[MAX_WIDTH][MAX_HEIGHT];
char tiles_occupied;
enum game_custom_options {
    GAME_WIDTH,
    GAME_HEIGHT,
    GAME_SEQUENCE,
    GAME_PLAYERS,

    GAME_CUSTOM_COUNT
};
char *game_custom_txt[GAME_CUSTOM_COUNT];
char game_custom_len[GAME_CUSTOM_COUNT];
unsigned char game_variable[GAME_CUSTOM_COUNT];
unsigned char game_variable_min[GAME_CUSTOM_COUNT];
unsigned char game_variable_max[GAME_CUSTOM_COUNT];

const char player_char[MAX_PLAYERS] = {PLAYER_1_CHAR, PLAYER_2_CHAR, PLAYER_3_CHAR, PLAYER_4_CHAR};

const char number[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

// Functions
void resize_console(int, int);
void clear_line(char);
void clear_screen();
void cursor_visibility(int);
char get_user_input();
int popup(enum popup);
void menu();
void draw_menu();
int customize_game();
void draw_customize_game(char);
void game();
void draw_game(int, int, char);
char check_game_over(char, char);
void game_over_screen(char);

int main()
{
    // Declarations
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(console_handle, &cursor_info);
    GetConsoleScreenBufferInfo(console_handle, &console_info);
    GetCurrentConsoleFont(console_handle, FALSE, &font_info);
    
    // Update the console size (based in the characters)
    resize_console(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    
    // Setting window as insizeble
    window_handle = GetConsoleWindow();
    style = GetWindowLong(window_handle, GWL_STYLE);
    style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
    SetWindowLong(window_handle, GWL_STYLE, style);

    // Set application name
    SetConsoleTitle("Ultimate TicTacToe");

    // Clear the screen
    clear_screen();

    // Makes cursor invisible
    cursor_visibility(FALSE);

    // Logic and string variables
    input = 0;

    arrow_txt[FALSE] = "   ";
    arrow_txt[TRUE]  = ">> ";
    arrow_len[FALSE]  = strlen(arrow_txt[FALSE]);
    arrow_len[TRUE]   = strlen(arrow_txt[TRUE]);

    option_txt[PLAY_LOCAL] = "Play";
    option_txt[EXIT] = "Exit";
    for (int i = 0; i < OPTIONS_COUNT; i++) option_len[i] = strlen(option_txt[i]);

    popup_txt[POPUP_EXIT_PROGRAM] = "Sure want to quit?";
    popup_txt[POPUP_LEFT_GAME] = "Come back to menu?";
    popup_txt[POPUP_RESET_GAME] = "Sure want to reset the game?";
    for (int i = 0; i < POPUP_COUNT; i++) popup_len[i] = strlen(popup_txt[i]);

    game_custom_txt[GAME_WIDTH] = "Width of grid";
    game_custom_txt[GAME_HEIGHT] = "Height of grid";
    game_custom_txt[GAME_SEQUENCE] = "Sequence to win";
    game_custom_txt[GAME_PLAYERS] = "Number of players";
    for (int i = 0; i < GAME_CUSTOM_COUNT; i++) game_custom_len[i] = strlen(game_custom_txt[i]);
    game_variable[GAME_WIDTH] = game_variable_min[GAME_WIDTH] = MIN_WIDTH;
    game_variable_max[GAME_WIDTH] = MAX_WIDTH;
    game_variable[GAME_HEIGHT] = game_variable_min[GAME_HEIGHT] = MIN_HEIGHT;
    game_variable_max[GAME_HEIGHT] = MAX_HEIGHT;
    game_variable[GAME_SEQUENCE] = game_variable_min[GAME_SEQUENCE] = game_variable_max[GAME_SEQUENCE] = 3;
    game_variable[GAME_PLAYERS] = game_variable_min[GAME_PLAYERS] = MIN_PLAYERS;
    game_variable_max[GAME_PLAYERS] = MAX_PLAYERS;

    no_txt = "No";
    no_len = strlen(no_txt);
    yes_txt = "Yes";
    yes_len = strlen(yes_txt);

    menu_hint_txt[0] = " W and S to navigate through menu";
    menu_hint_len[0] = strlen(menu_hint_txt[0]);
    menu_hint_txt[1] = " ENTER to select your option";
    menu_hint_len[1] = strlen(menu_hint_txt[1]);
    menu_hint_txt[2] = " ESCAPE to come back or quit";
    menu_hint_len[2] = strlen(menu_hint_txt[2]);

    play_txt = "Play";
    play_len = strlen(play_txt);
    back_txt = "Back";
    back_len = strlen(back_txt);

    customize_game_hint_txt[0] = " W and S to navigate through the options";
    customize_game_hint_len[0] = strlen(customize_game_hint_txt[0]);
    customize_game_hint_txt[1] = " A and D to change values";
    customize_game_hint_len[1] = strlen(customize_game_hint_txt[1]);
    customize_game_hint_txt[2] = " Q to come back";
    customize_game_hint_len[2] = strlen(customize_game_hint_txt[2]);

    player_turn_txt = "Player Turn:";
    player_turn_len = strlen(player_turn_txt);

    draw_txt = "DRAW!";
    draw_len = strlen(draw_txt);
    winner_txt[0] = "PLAYER";
    winner_txt[1] = "WON!";
    winner_len[0] = strlen(winner_txt[0]);
    winner_len[1] = strlen(winner_txt[1]);

    // Main loop
    while (running)
    {
        switch (actual_scene)
        {
            case SCENE_MENU:
                menu();            
                break;
            case SCENE_LOCAL_GAME:
                game();
                break;
            default:
                return 1;
        }
    }        
    return 0;
}

void resize_console(int width, int height)
{
    // Variable with the console dimensions {left, top, right, bottom}
    SMALL_RECT rect = {0};
    // Temporary resize window to minimum, because of buffer limitations
    rect.Right = 1;
    rect.Bottom = 1;
    SetConsoleWindowInfo(console_handle, TRUE, &rect);
    // Get the real values
    rect.Right = width-1;
    rect.Bottom = height-1;
    // Resize buffer
    SetConsoleScreenBufferSize(console_handle, coord(width, height));
    // Truly resize window
    SetConsoleWindowInfo(console_handle, TRUE, &rect);
}

void clear_line(char line)
{
    // Make a clear string
    char clean_line[CONSOLE_WIDTH + 1];
    for (int i = 0; i < CONSOLE_WIDTH; i++) clean_line[i] = VOID_CHAR;
    clean_line[CONSOLE_WIDTH] = '\0';

    WriteConsoleOutputCharacter(console_handle, clean_line, CONSOLE_WIDTH, coord(0, line), &characters);
}

void clear_screen()
{
    // Apply it in every line of console
    for (int i = 0; i < CONSOLE_HEIGHT; i++)
    {
        clear_line(i);
    }
}

void cursor_visibility(int set)
{
    cursor_info.bVisible = set;
    SetConsoleCursorInfo(console_handle, &cursor_info);
}

char get_user_input()
{
    // Ignore the caps lock effect
    return tolower(getch());
}

int popup(enum popup question)
{
    clear_screen();

    int temporary_arrow = FALSE;

    while (TRUE)
    {
        // Print question
        WriteConsoleOutputCharacter(console_handle, popup_txt[question], popup_len[question], coord(0, 0), &characters);
        // Print option 0 (no)
        WriteConsoleOutputCharacter(console_handle, arrow_txt[!temporary_arrow], arrow_len[!temporary_arrow], coord(0, 1), &characters);
        WriteConsoleOutputCharacter(console_handle, no_txt, no_len, coord(arrow_len[temporary_arrow], 1), &characters);
        // Print option 1 (yes)
        WriteConsoleOutputCharacter(console_handle, arrow_txt[temporary_arrow], arrow_len[temporary_arrow], coord(0, 2), &characters);
        WriteConsoleOutputCharacter(console_handle, yes_txt, yes_len, coord(arrow_len[temporary_arrow], 2), &characters);
        // Input logic
        input = get_user_input();
        switch (input)
        {
            case UP:
            case DOWN:
                temporary_arrow = !temporary_arrow;
                break;
            case SELECT:
                if (temporary_arrow == TRUE)
                {
                    clear_screen();
                    return TRUE;
                }
            case QUIT:
                clear_screen();
                return FALSE;
        }
    }
}

void menu()
{
    draw_menu();
    
    input = get_user_input();
    switch (input)
    {
        case UP:
            if (arrow > 0) arrow--;
            else arrow = OPTIONS_COUNT-1;
            break;
        case DOWN:
            if (arrow < OPTIONS_COUNT-1) arrow++;
            else arrow = 0;
            break;
        case SELECT:
            switch (arrow)
            {
                case PLAY_LOCAL:
                    clear_screen();
                    if (customize_game()) actual_scene = SCENE_LOCAL_GAME;
                    break;
                case EXIT:
                    // If answer is Yes, close the program
                    if (popup(POPUP_EXIT_PROGRAM)) running = FALSE;
                    break;
            }
            break;
        case QUIT:
            running = FALSE;
            break;
    }   
}

void draw_menu()
{
    char is_arrow_in_this_line;
    int len_already_written;

    for (int i = 0; i < OPTIONS_COUNT; i++)
    {
        // Check if the arrow is in this line
        is_arrow_in_this_line = (i == arrow);
        // Draw the arrow or it's absence
        WriteConsoleOutputCharacter(console_handle, arrow_txt[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the actual menu line
        WriteConsoleOutputCharacter(console_handle, option_txt[i], option_len[i], coord(arrow_len[is_arrow_in_this_line], i), &characters);
        // Clear rest of the line
        len_already_written = arrow_len[is_arrow_in_this_line] + option_len[i];
        for (int j = len_already_written; j < CONSOLE_WIDTH; j++) WriteConsoleOutputCharacter(console_handle, space, 1, coord(j, i), &characters);
    }
    WriteConsoleOutputCharacter(console_handle, menu_hint_txt[0], menu_hint_len[0], coord(0, CONSOLE_HEIGHT-4), &characters);
    WriteConsoleOutputCharacter(console_handle, menu_hint_txt[1], menu_hint_len[1], coord(0, CONSOLE_HEIGHT-3), &characters);
    WriteConsoleOutputCharacter(console_handle, menu_hint_txt[2], menu_hint_len[2], coord(0, CONSOLE_HEIGHT-2), &characters);
}

int customize_game()
{
    char new_arrow = GAME_CUSTOM_COUNT;
    char custom_options = GAME_CUSTOM_COUNT + 2;

    while (TRUE)
    {
        draw_customize_game(new_arrow);
        input = get_user_input();
        switch (input)
        {
            case UP:
                if (new_arrow > 0) new_arrow--;
                else new_arrow = custom_options - 1;
                break;
            case DOWN:
                if (new_arrow < custom_options - 1) new_arrow++;
                else new_arrow = 0;
                break;
            case LEFT:
                if (new_arrow <= GAME_CUSTOM_COUNT && game_variable[new_arrow] > game_variable_min[new_arrow])
                    {
                        game_variable[new_arrow]--;
                        game_variable_max[GAME_SEQUENCE] = game_variable[GAME_WIDTH] > game_variable[GAME_HEIGHT] ? game_variable[GAME_HEIGHT] : game_variable[GAME_WIDTH];
                        if (game_variable[GAME_SEQUENCE] > game_variable_max[GAME_SEQUENCE]) game_variable[GAME_SEQUENCE] = game_variable_max[GAME_SEQUENCE];
                    }
                break;
            case RIGHT:
                if (new_arrow <= GAME_CUSTOM_COUNT && game_variable[new_arrow] < game_variable_max[new_arrow])
                    {
                        game_variable[new_arrow]++;
                        game_variable_max[GAME_SEQUENCE] = game_variable[GAME_WIDTH] > game_variable[GAME_HEIGHT] ? game_variable[GAME_HEIGHT] : game_variable[GAME_WIDTH];
                        if (game_variable[GAME_SEQUENCE] > game_variable_max[GAME_SEQUENCE]) game_variable[GAME_SEQUENCE] = game_variable_max[GAME_SEQUENCE];
                    }
                break;
            case SELECT:
                clear_screen();
                switch (new_arrow)
                {
                    case GAME_CUSTOM_COUNT:
                        return TRUE;
                    case GAME_CUSTOM_COUNT + 1:
                        return FALSE;
                }
                break;
            case QUIT:
                clear_screen();
                return FALSE;
        }
    }
}

void draw_customize_game(char arrow)
{
    char is_arrow_in_this_line;
    int len_already_written;

    for (int i = 0; i < GAME_CUSTOM_COUNT; i++)
    {
        // Check if the arrow is in this line
        is_arrow_in_this_line = (i == arrow);
        // Draw the arrow or it's absence
        WriteConsoleOutputCharacter(console_handle, arrow_txt[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the custom game line
        WriteConsoleOutputCharacter(console_handle, game_custom_txt[i], game_custom_len[i], coord(arrow_len[is_arrow_in_this_line], i), &characters);
        // Draw the number
        len_already_written = arrow_len[is_arrow_in_this_line] + game_custom_len[GAME_PLAYERS] + 1;
        if (game_variable[i] > game_variable_min[i]) WriteConsoleOutputCharacter(console_handle, "<", 1, coord(len_already_written, i), &characters);
        else                                         WriteConsoleOutputCharacter(console_handle, " ", 1, coord(len_already_written, i), &characters);
        if (game_variable[i] > 9)
        {
            char variable_txt[] = "??";
            variable_txt[0] = number[game_variable[i]/10];
            variable_txt[1] = number[game_variable[i]%10];
            WriteConsoleOutputCharacter(console_handle, variable_txt, 2, coord(len_already_written + 2, i), &characters);
            if (game_variable[i] < game_variable_max[i]) WriteConsoleOutputCharacter(console_handle, ">", 1, coord(len_already_written + 4, i), &characters);
            else                                         WriteConsoleOutputCharacter(console_handle, " ", 1, coord(len_already_written + 4, i), &characters);
        }
        else
        {
            char variable_txt[] = "?";
            variable_txt[0] = number[game_variable[i]];
            WriteConsoleOutputCharacter(console_handle, variable_txt, 1, coord(len_already_written + 2, i), &characters);
            if (game_variable[i] < game_variable_max[i]) WriteConsoleOutputCharacter(console_handle, " >", 2, coord(len_already_written + 3, i), &characters);
            else                                         WriteConsoleOutputCharacter(console_handle, "  ", 2, coord(len_already_written + 3, i), &characters);
        }
    }

    for (int i = GAME_CUSTOM_COUNT + 1; i <= GAME_CUSTOM_COUNT + 2; i++)
    {
        // Check if the arrow is in this line
        is_arrow_in_this_line = (i == arrow + 1);
        // Draw the arrow or it's absence
        WriteConsoleOutputCharacter(console_handle, arrow_txt[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the line
        if (i == GAME_CUSTOM_COUNT + 1) WriteConsoleOutputCharacter(console_handle, play_txt, play_len, coord(arrow_len[is_arrow_in_this_line], i), &characters);
        else                            WriteConsoleOutputCharacter(console_handle, back_txt, back_len, coord(arrow_len[is_arrow_in_this_line], i), &characters);
    }

    WriteConsoleOutputCharacter(console_handle, customize_game_hint_txt[0], customize_game_hint_len[0], coord(0, CONSOLE_HEIGHT-4), &characters);
    WriteConsoleOutputCharacter(console_handle, customize_game_hint_txt[1], customize_game_hint_len[1], coord(0, CONSOLE_HEIGHT-3), &characters);
    WriteConsoleOutputCharacter(console_handle, customize_game_hint_txt[2], customize_game_hint_len[2], coord(0, CONSOLE_HEIGHT-2), &characters);
}

void game()
{
    int grid_width_count = game_variable[GAME_WIDTH];
    int grid_height_count = game_variable[GAME_HEIGHT];
    int last_player_index = game_variable[GAME_PLAYERS] - 1;

    // Grid variables
    int grid_total_size = grid_height_count * grid_width_count;
    int grid_string_lenght = (3 * grid_total_size) + (grid_total_size - 1) + 1;
    int grid_height_total_count = (2 * grid_height_count) - 1;
    int grid_width_total_count = (2 * grid_width_count) - 1;
    // Tile selected position
    COORD tile_selected = coord(0, 0);
    // Clean the tiles
    for (int i = 0; i < grid_height_count; i++)
    {
        for (int j = 0; j < grid_width_count; j++) tile[j][i] = VOID_CHAR;
    }
    tiles_occupied = 0;

    // Players
    char player_turn = 0;

    cursor_visibility(TRUE);
    
    
    SetConsoleCursorPosition(console_handle, coord(1, 0));
    while (actual_scene == SCENE_LOCAL_GAME)
    {
        draw_game(grid_width_total_count, grid_height_total_count, player_turn);
        input = get_user_input();
        switch (input)
        {
            case UP:
                if (tile_selected.Y > 0) tile_selected.Y--;
                break;
            case DOWN:
                if (tile_selected.Y < grid_height_count-1) tile_selected.Y++;
                break;
            case LEFT:
                if (tile_selected.X > 0) tile_selected.X--;
                break;
            case RIGHT:
                if (tile_selected.X < grid_width_count-1) tile_selected.X++;
                break;
            case SELECT:
                if (tile[tile_selected.X][tile_selected.Y] == VOID_CHAR)
                {
                    tile[tile_selected.X][tile_selected.Y] = player_char[player_turn];
                    // Check if someone won
                    if (check_game_over(tile_selected.X, tile_selected.Y))
                    {
                        draw_game(grid_width_total_count, grid_height_total_count, player_turn);
                        game_over_screen(player_char[player_turn]);
                        return;
                    }
                    // Check if is draw
                    tiles_occupied++;
                    if (tiles_occupied == grid_total_size)
                    {
                        draw_game(grid_width_total_count, grid_height_total_count, player_turn);
                        game_over_screen(DRAW);
                        return;
                    }
                    // Change the turn owner
                    if (player_turn != last_player_index) player_turn++;
                    else player_turn = 0;

                    draw_game(grid_width_total_count, grid_height_total_count, player_turn);
                }
                break;
            case RESET:
                cursor_visibility(FALSE);
                if (popup(POPUP_RESET_GAME))
                {
                    // Reset
                    return;
                }
                cursor_visibility(TRUE);
                break;
            case QUIT:
                cursor_visibility(FALSE);
                if (popup(POPUP_LEFT_GAME))
                {
                    // Go to menu
                    actual_scene = SCENE_MENU;
                    return;
                }
                cursor_visibility(TRUE);
            }
            int x = (tile_selected.X * 4) + 1;
            int y = (tile_selected.Y * 2);
            SetConsoleCursorPosition(console_handle, coord(x, y));
    }
}

void draw_game(int grid_width, int grid_height, char player_turn)
{
    char tile_x = 0;
    char tile_y = 0;
    
    char that_tile[4] = "   ";
    for (int i = 0; i < grid_height; i++)
    {
        for (int j = 0; j < grid_width; j++)
        {
            // If row is even, draw the tiles
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    that_tile[1] = tile[tile_x][tile_y];
                    tile_x++;
                    WriteConsoleOutputCharacter(console_handle, that_tile, 3, coord(j * 2, i), &characters);
                }
                else
                {
                    WriteConsoleOutputCharacter(console_handle, "|", 1, coord((j * 2) + 1, i), &characters);
                }
            }
            // Else (is odd), draw the lines
            else
            {
                if (j % 2 == 0)
                {
                    WriteConsoleOutputCharacter(console_handle, "---", 3, coord(j * 2, i), &characters);
                }
                else
                {
                    WriteConsoleOutputCharacter(console_handle, "|", 1, coord((j * 2) + 1, i), &characters);
                }
            }
        }
        if (i % 2 == 0)
        {
            tile_x = 0;
            tile_y++;
        }
    }

    
    WriteConsoleOutputCharacter(console_handle, player_turn_txt, player_turn_len, coord(0, CONSOLE_HEIGHT-1), &characters);
    char player[] = " ? ";
    player[1] = player_char[player_turn];
    WriteConsoleOutputCharacter(console_handle, player, 3, coord(player_turn_len, CONSOLE_HEIGHT-1), &characters);
}

char check_game_over(char last_tile_setted_x, char last_tile_setted_y)
{
    int counter = 0;
    int sequence = game_variable[GAME_SEQUENCE];
    COORD max = {game_variable[GAME_WIDTH], game_variable[GAME_HEIGHT]};
    char horizontal_constant_value;
    char vertical_constant_value;
    COORD tile_buffer = {0};
    for (int i = 0; i < sequence; i++)
    {
        // Horizontal Sequence "—"
        for (int j = 0; j < (sequence - 1); j++)
        {
            horizontal_constant_value = (sequence - 1) - i - j;
            // Set the tile that will be analyzed
            tile_buffer.X = last_tile_setted_x - horizontal_constant_value;
            tile_buffer.Y = last_tile_setted_y;
            if (tile_buffer.X >= last_tile_setted_x) tile_buffer.X++;
            // Check if the tile being analyzed is valid
            if (tile_buffer.X < 0 || tile_buffer.X > max.X) break;
            // Increment the counter in case the tile analyzed and the last tile setted have the same char
            if (tile[tile_buffer.X][tile_buffer.Y] == tile[last_tile_setted_x][last_tile_setted_y]) counter++;
            else break;
        }
        if (counter == sequence - 1) return TRUE;
        counter = 0;

        // Vertical Sequence "|"
        for (int j = 0; j < (sequence - 1); j++)
        {
            vertical_constant_value = (sequence - 1) - i - j;
            // Set the tile that will be analyzed
            tile_buffer.X = last_tile_setted_x;
            tile_buffer.Y = last_tile_setted_y - vertical_constant_value;
            if (tile_buffer.Y >= last_tile_setted_y) tile_buffer.Y++;
            // Check if the tile being analyzed is valid
            if (tile_buffer.Y < 0 || tile_buffer.Y > max.Y) break;
            // Increment the counter in case the tile analyzed and the last tile setted have the same char
            if (tile[tile_buffer.X][tile_buffer.Y] == tile[last_tile_setted_x][last_tile_setted_y]) counter++;
            else break;
        }
        if (counter == sequence - 1) return TRUE;
        counter = 0;

        // Diagonal Sequence "\"
        for (int j = 0; j < (sequence - 1); j++)
        {
            horizontal_constant_value = (sequence - 1) - i - j;
            vertical_constant_value   = (sequence - 1) - i - j;
            
            tile_buffer.X = last_tile_setted_x - horizontal_constant_value;
            if (tile_buffer.X >= last_tile_setted_x) tile_buffer.X++;
            if (tile_buffer.X < 0 || tile_buffer.X > max.X) break;
            
            
            tile_buffer.Y = last_tile_setted_y - vertical_constant_value;
            if (tile_buffer.Y >= last_tile_setted_y) tile_buffer.Y++;
            if (tile_buffer.Y < 0 || tile_buffer.Y > max.Y) break;

            // Increment the counter in case the tile analyzed and the last tile setted have the same char
            if (tile[tile_buffer.X][tile_buffer.Y] == tile[last_tile_setted_x][last_tile_setted_y]) counter++;
            else break;
        }
        if (counter == sequence - 1) return TRUE;
        counter = 0;

        // Diagonal Sequence "/"
        for (int j = 0; j < (sequence - 1); j++)
        {
            horizontal_constant_value = (sequence - 1) - i - j;
            vertical_constant_value   = (sequence - 1) - i - j;
            
            tile_buffer.X = last_tile_setted_x - horizontal_constant_value;
            if (tile_buffer.X >= last_tile_setted_x) tile_buffer.X++;
            if (tile_buffer.X < 0 || tile_buffer.X > max.X) break;
            
            
            tile_buffer.Y = last_tile_setted_y + vertical_constant_value;
            if (tile_buffer.Y <= last_tile_setted_y) tile_buffer.Y--;
            if (tile_buffer.Y < 0 || tile_buffer.Y > max.Y) break;

            // Increment the counter in case the tile analyzed and the last tile setted have the same char
            if (tile[tile_buffer.X][tile_buffer.Y] == tile[last_tile_setted_x][last_tile_setted_y]) counter++;
            else break;
        }
        if (counter == sequence - 1) return TRUE;
        counter = 0;
    }

    return FALSE;
}

void game_over_screen(char winner_char)
{
    clear_line(CONSOLE_HEIGHT-1);

    // Draw
    if (winner_char == DRAW)
    {
        WriteConsoleOutputCharacter(console_handle, draw_txt, draw_len, coord((CONSOLE_WIDTH - draw_len) / 2, CONSOLE_HEIGHT-1), &characters);
    }
    // Have a winner
    else
    {
        char entire_lenght = winner_len[0] + 3 + winner_len[1];
        char winner_txt_x[2] = { ((CONSOLE_WIDTH - entire_lenght) / 2), ((CONSOLE_WIDTH - entire_lenght) / 2)};
        WriteConsoleOutputCharacter(console_handle, winner_txt[0], winner_len[0], coord(0, CONSOLE_HEIGHT-1), &characters);
        char player[] = " ? ";
        player[1] = winner_char;
        WriteConsoleOutputCharacter(console_handle, player, 3, coord(winner_len[0], CONSOLE_HEIGHT-1), &characters);
        WriteConsoleOutputCharacter(console_handle, winner_txt[1], winner_len[1], coord(winner_len[0] + 3, CONSOLE_HEIGHT-1), &characters);
    }

    get_user_input();
    cursor_visibility(FALSE);
    clear_screen();
}

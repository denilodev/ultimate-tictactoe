#include <conio.h>
#include <stdio.h>
#include <windows.h>

#define TRUE        1
#define FALSE       0
#define UP          'w'
#define DOWN        's'
#define LEFT        'a'
#define RIGHT       'd'
#define SELECT      ' '
#define QUIT        'q'
#define coord(c, r) (COORD){c, r}

#define MIN_ROWS    3
#define MAX_ROWS    10
#define MIN_COLS    3
#define MAX_COLS    10
#define MIN_PLAYERS 2
#define MAX_PLAYERS 4

#define VOID_CHAR   ' '
#define PLAYER_1_CHAR 'O'
#define PLAYER_2_CHAR 'X'
#define PLAYER_3_CHAR 'A'
#define PLAYER_4_CHAR 'Z'

// General variables
char running = TRUE;
char input;
char arrow;
char *arrow_text[2];
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
const int CONSOLE_WIDTH = 40;
const int CONSOLE_HEIGHT = 20;

// Scene variables
enum scene {
    SCENE_MENU,
    SCENE_LOCAL_GAME,
    SCENE_MULTI_GAME
};
enum scene actual_scene = SCENE_MENU;

enum popup {
    POPUP_EXIT_PROGRAM,
    POPUP_HOST_OR_CONNECT,
    POPUP_LEFT_GAME,

    POPUP_COUNT
};
char *popup_txt[POPUP_COUNT];
char popup_len[POPUP_COUNT];

// Menu variables
enum menu_options {
    PLAY_LOCAL,
    PLAY_MULTI,
    EXIT,

    OPTIONS_COUNT
};
char *option_txt[OPTIONS_COUNT];
char option_len[OPTIONS_COUNT];

// Game variables
char tile[MAX_ROWS * MAX_COLS];
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

// Functions
void resize_console(int, int);
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
char check_game_over(char);

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

    arrow_text[FALSE] = "   ";
    arrow_text[TRUE]  = ">> ";
    arrow_len[FALSE]  = strlen(arrow_text[FALSE]);
    arrow_len[TRUE]   = strlen(arrow_text[TRUE]);

    option_txt[PLAY_LOCAL] = "Play offline";
    option_txt[PLAY_MULTI] = "Play online (LAN)";
    option_txt[EXIT] = "Exit the game";
    for (int i = 0; i < OPTIONS_COUNT; i++) option_len[i] = strlen(option_txt[i]);

    popup_txt[POPUP_EXIT_PROGRAM] = "Sure want to quit?";
    popup_txt[POPUP_HOST_OR_CONNECT] = "Want to host or connect a game?";
    popup_txt[POPUP_LEFT_GAME] = "Sure want to left the game?";
    for (int i = 0; i < POPUP_COUNT; i++) popup_len[i] = strlen(popup_txt[i]);

    game_custom_txt[GAME_WIDTH] = "Width of grid";
    game_custom_txt[GAME_HEIGHT] = "Height of grid";
    game_custom_txt[GAME_SEQUENCE] = "Sequence to win";
    game_custom_txt[GAME_PLAYERS] = "Number of players";
    for (int i = 0; i < GAME_CUSTOM_COUNT; i++) game_custom_len[i] = strlen(game_custom_txt[i]);
    game_variable[GAME_WIDTH] = game_variable_min[GAME_WIDTH] = MIN_COLS;
    game_variable_max[GAME_WIDTH] = MAX_COLS;
    game_variable[GAME_HEIGHT] = game_variable_min[GAME_HEIGHT] = MIN_ROWS;
    game_variable_max[GAME_HEIGHT] = MAX_ROWS;
    game_variable[GAME_SEQUENCE] = game_variable_min[GAME_SEQUENCE] = game_variable_max[GAME_SEQUENCE] = 3;
    game_variable[GAME_PLAYERS] = game_variable_min[GAME_PLAYERS] = MIN_PLAYERS;
    game_variable_max[GAME_PLAYERS] = MAX_PLAYERS;

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
                // Show TTT
                // TTT logic
                // If game is over
                    // Show winner
                    // Ask if want to replay or go back to MENU
                        // If replay, re-loop
                        // If go back
                            // Set MENU
                            // Re-loop
                break;
            case SCENE_MULTI_GAME:
                // Ask if host or connect game
                    // If host
                        // Build server socket
                        // Show IP and PORT to connect
                        // Wait for connection
                        // If connection
                            // Show TTT
                            // TTT logic
                            // If game is over
                                // Show winner
                                // Ask if want to replay or go back to MENU
                                    // If replay, re-loop
                                    // If go back
                                        // Set MENU
                                        // Re-loop
                    // If connect
                        // Build client socket
                        // Ask for IP Addres to connect
                        // Ask for PORT to connect
                        // If connection is not successful
                            // Show that connection failed and return to ask again the Address and PORT
                        // If connection is successuful
                            // Show TTT
                            // TTT logic
                            // If game is over
                                // Show winner
                                // Ask if want to replay or go back to MENU
                                    // If replay, re-loop
                                    // If go back
                                        // Set MENU
                                        // Re-loop
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

void clear_screen()
{
    // Make a clear string
    char clean_line[CONSOLE_WIDTH + 1];
    for (int i = 0; i < CONSOLE_WIDTH; i++) clean_line[i] = VOID_CHAR;
    clean_line[CONSOLE_WIDTH] = '\0';

    // Apply it in every line of console
    for (int i = 0; i < CONSOLE_HEIGHT; i++)
    {
        WriteConsoleOutputCharacter(console_handle, clean_line, CONSOLE_WIDTH, coord(0, i), &characters);
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

    char *no_txt = "No";
    char no_len = strlen(no_txt);
    char *yes_txt = "Yes";
    char yes_len = strlen(yes_txt);
    char *host_txt = "Host";
    char host_len = strlen(host_txt);
    char *connect_txt = "Connect";
    char connect_len = strlen(connect_txt);

    switch (question)
    {
        case POPUP_EXIT_PROGRAM:
            while (TRUE)
            {
                // Print question
                WriteConsoleOutputCharacter(console_handle, popup_txt[question], popup_len[question], coord(0, 0), &characters);
                // Print option 0 (no)
                WriteConsoleOutputCharacter(console_handle, arrow_text[!temporary_arrow], arrow_len[!temporary_arrow], coord(0, 1), &characters);
                WriteConsoleOutputCharacter(console_handle, no_txt, no_len, coord(arrow_len[temporary_arrow], 1), &characters);
                // Print option 1 (yes)
                WriteConsoleOutputCharacter(console_handle, arrow_text[temporary_arrow], arrow_len[temporary_arrow], coord(0, 2), &characters);
                WriteConsoleOutputCharacter(console_handle, yes_txt, yes_len, coord(arrow_len[temporary_arrow], 2), &characters);
                // Input logic
                input = getch();
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

        case POPUP_HOST_OR_CONNECT:
            while (TRUE)
            {
                // Print question
                WriteConsoleOutputCharacter(console_handle, popup_txt[question], popup_len[question], coord(0, 0), &characters);
                // Print option 0 (no)
                WriteConsoleOutputCharacter(console_handle, arrow_text[!temporary_arrow], arrow_len[!temporary_arrow], coord(0, 1), &characters);
                WriteConsoleOutputCharacter(console_handle, connect_txt, connect_len, coord(arrow_len[temporary_arrow], 1), &characters);
                // Print option 1 (yes)
                WriteConsoleOutputCharacter(console_handle, arrow_text[temporary_arrow], arrow_len[temporary_arrow], coord(0, 2), &characters);
                WriteConsoleOutputCharacter(console_handle, host_txt, host_len, coord(arrow_len[temporary_arrow], 2), &characters);
                // Input logic
                input = getch();
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

        case POPUP_LEFT_GAME:
            while (TRUE)
            {
                // Print question
                WriteConsoleOutputCharacter(console_handle, popup_txt[question], popup_len[question], coord(0, 0), &characters);
                // Print option 0 (no)
                WriteConsoleOutputCharacter(console_handle, arrow_text[!temporary_arrow], arrow_len[!temporary_arrow], coord(0, 1), &characters);
                WriteConsoleOutputCharacter(console_handle, no_txt, no_len, coord(arrow_len[temporary_arrow], 1), &characters);
                // Print option 1 (yes)
                WriteConsoleOutputCharacter(console_handle, arrow_text[temporary_arrow], arrow_len[temporary_arrow], coord(0, 2), &characters);
                WriteConsoleOutputCharacter(console_handle, yes_txt, yes_len, coord(arrow_len[temporary_arrow], 2), &characters);
                // Input logic
                input = getch();
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

        default:
            return -1;
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
                case PLAY_MULTI:
                    popup(POPUP_HOST_OR_CONNECT);
                    // clear_screen();
                    // actual_scene = SCENE_MULTI_GAME;
                    break;
                case EXIT:
                    // If answer is Yes, close the program
                    if (popup(POPUP_EXIT_PROGRAM)) running = FALSE;
                    break;
            }
            break;
        case QUIT:
            if (popup(POPUP_EXIT_PROGRAM)) running = FALSE;
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
        WriteConsoleOutputCharacter(console_handle, arrow_text[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the actual menu line
        WriteConsoleOutputCharacter(console_handle, option_txt[i], option_len[i], coord(arrow_len[is_arrow_in_this_line], i), &characters);
        // Clear rest of the line
        len_already_written = arrow_len[is_arrow_in_this_line] + option_len[i];
        for (int j = len_already_written; j < CONSOLE_WIDTH; j++) WriteConsoleOutputCharacter(console_handle, space, 1, coord(j, i), &characters);
    }
    char *hint_1 = " W and S to navigate through menu";
    char hint_1_len = strlen(hint_1);
    char *hint_2 = " SPACE to select your option";
    char hint_2_len = strlen(hint_2);
    char *hint_3 = " Q to come back or quit";
    char hint_3_len = strlen(hint_3);
    WriteConsoleOutputCharacter(console_handle, hint_1, hint_1_len, coord(0, CONSOLE_HEIGHT-4), &characters);
    WriteConsoleOutputCharacter(console_handle, hint_2, hint_2_len, coord(0, CONSOLE_HEIGHT-3), &characters);
    WriteConsoleOutputCharacter(console_handle, hint_3, hint_3_len, coord(0, CONSOLE_HEIGHT-2), &characters);
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
        WriteConsoleOutputCharacter(console_handle, arrow_text[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the custom game line
        WriteConsoleOutputCharacter(console_handle, game_custom_txt[i], game_custom_len[i], coord(arrow_len[is_arrow_in_this_line], i), &characters);
        // Draw the number
        len_already_written = arrow_len[is_arrow_in_this_line] + game_custom_len[GAME_PLAYERS] + 1;
        WriteConsoleOutputCharacter(console_handle, "<", 1, coord(len_already_written, i), &characters);
        char number[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        if (game_variable[i] > 9)
        {
            char variable_txt[3] = "  ";
            variable_txt[0] = number[game_variable[i]/10];
            variable_txt[1] = number[game_variable[i]%10];
            WriteConsoleOutputCharacter(console_handle, variable_txt, 2, coord(len_already_written + 1, i), &characters);
            WriteConsoleOutputCharacter(console_handle, ">", 1, coord(len_already_written + 3, i), &characters);
        }
        else
        {
            char variable_txt[2] = " ";
            variable_txt[0] = number[game_variable[i]];
            WriteConsoleOutputCharacter(console_handle, variable_txt, 1, coord(len_already_written + 1, i), &characters);
            WriteConsoleOutputCharacter(console_handle, "> ", 2, coord(len_already_written + 2, i), &characters);
        }
    }

    for (int i = GAME_CUSTOM_COUNT + 1; i <= GAME_CUSTOM_COUNT + 2; i++)
    {
        // Check if the arrow is in this line
        is_arrow_in_this_line = (i == arrow + 1);
        // Draw the arrow or it's absence
        WriteConsoleOutputCharacter(console_handle, arrow_text[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the line
        if (i == GAME_CUSTOM_COUNT + 1) WriteConsoleOutputCharacter(console_handle, "Play", 4, coord(arrow_len[is_arrow_in_this_line], i), &characters);
        else                            WriteConsoleOutputCharacter(console_handle, "Back", 4, coord(arrow_len[is_arrow_in_this_line], i), &characters);
    }

    char *hint_1 = " W and S to navigate through the options";
    char hint_1_len = strlen(hint_1);
    char *hint_2 = " A and D to change values";
    char hint_2_len = strlen(hint_2);
    char *hint_3 = " Q to come back";
    char hint_3_len = strlen(hint_3);
    WriteConsoleOutputCharacter(console_handle, hint_1, hint_1_len, coord(0, CONSOLE_HEIGHT-4), &characters);
    WriteConsoleOutputCharacter(console_handle, hint_2, hint_2_len, coord(0, CONSOLE_HEIGHT-3), &characters);
    WriteConsoleOutputCharacter(console_handle, hint_3, hint_3_len, coord(0, CONSOLE_HEIGHT-2), &characters);
}

void game()
{
    int grid_col_count = game_variable[GAME_WIDTH];
    int grid_row_count = game_variable[GAME_HEIGHT];
    int last_player_index = game_variable[GAME_PLAYERS] - 1;

    // Clamp the value of rows and columns beetween the minimum and maximum values
    if (grid_row_count < MIN_ROWS) grid_row_count = MIN_ROWS;
    else if (grid_row_count > MAX_ROWS) grid_row_count = MAX_ROWS;
    if (grid_col_count < MIN_COLS) grid_col_count = MIN_COLS;
    else if (grid_col_count > MAX_COLS) grid_col_count = MAX_COLS;

    // Grid variables
    int grid_total_size = grid_row_count * grid_col_count;
    int grid_string_lenght = (3 * grid_total_size) + (grid_total_size - 1) + 1;
    int grid_rows_total_count = (2 * grid_row_count) - 1;
    int grid_cols_total_count = (2 * grid_col_count) - 1;
    // Tile selected position
    COORD tile_selected = coord(0, 0);
    // Clean the tiles
    for (int i = 0; i < grid_total_size; i++) tile[i] = VOID_CHAR;

    // Players
    char player_turn = 0;

    cursor_visibility(TRUE);
    
    draw_game(grid_rows_total_count, grid_cols_total_count, player_turn);

    SetConsoleCursorPosition(console_handle, coord(1, 0));
    while (actual_scene == SCENE_LOCAL_GAME || actual_scene == SCENE_MULTI_GAME)
    {
        input = get_user_input();
        switch (input)
        {
            case UP:
                if (tile_selected.Y > 0) tile_selected.Y--;
                break;
            case DOWN:
                if (tile_selected.Y < grid_row_count-1) tile_selected.Y++;
                break;
            case LEFT:
                if (tile_selected.X > 0) tile_selected.X--;
                break;
            case RIGHT:
                if (tile_selected.X < grid_col_count-1) tile_selected.X++;
                break;
            case SELECT:
                int tile_to_place = (tile_selected.Y * grid_col_count) + tile_selected.X;
                if (tile[tile_to_place] == VOID_CHAR)
                {
                    tile[tile_to_place] = player_char[player_turn];
                    if (check_game_over(tile_to_place))
                    {
                        actual_scene = SCENE_MENU;
                        return;
                    }
                    if (player_turn != last_player_index) player_turn++;
                    else player_turn = 0;

                    draw_game(grid_rows_total_count, grid_cols_total_count, player_turn);
                }
                break;
            case QUIT:
                cursor_visibility(FALSE);
                if (popup(POPUP_LEFT_GAME))
                {
                    // Go to menu
                    actual_scene = SCENE_MENU;
                    return;
                }
                // Need to redraw the game, once the popup clear everything
                draw_game(grid_rows_total_count, grid_cols_total_count, player_turn);
                cursor_visibility(TRUE);
            }
            int x = (tile_selected.X * 4) + 1;
            int y = (tile_selected.Y * 2);
            SetConsoleCursorPosition(console_handle, coord(x, y));
    }
}

void draw_game(int rows, int cols, char player_turn)
{
    char that_tile[4] = "   ";
    int tile_number = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // If row is even, draw the tiles
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    that_tile[1] = tile[tile_number];
                    WriteConsoleOutputCharacter(console_handle, that_tile, 3, coord(j * 2, i), &characters);
                    tile_number++;
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
    }

    char player_turn_txt[15] = "Player Turn: ?";
    player_turn_txt[13] = player_char[player_turn];
    char player_turn_len = strlen(player_turn_txt);
    // WriteConsoleOutputCharacter(console_handle, hint_3,       hint_3_len,      coord(0, CONSOLE_HEIGHT-2), &characters);
    WriteConsoleOutputCharacter(console_handle, player_turn_txt, player_turn_len, coord(0, CONSOLE_HEIGHT-1), &characters);
}

char check_game_over(char last_tile_setted)
{
    return FALSE;
}
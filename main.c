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
#define coord(c, r) (COORD){c, r}
#define MIN_ROWS    3
#define MAX_ROWS    10
#define MIN_COLS    3
#define MAX_COLS    10

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
    MENU,
    LOCAL_GAME,
    MULTI_GAME,
    EXIT_GAME
};
enum scene actual_scene = MENU;

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

// Functions
void resize_console(int, int);
void clear_screen();
void draw_menu();
void ask_if_want_exit();
void game(int, int);
void draw_game(int, int);

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

    // Clear the screen
    clear_screen();

    // Make cursor invisible
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(console_handle, &cursor_info);

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

    // Main loop
    while (running)
    {
        switch (actual_scene)
        {
            case MENU:
                draw_menu(arrow);
                //game(8, 8);
                input = getch();
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
                        case LEFT:
                            break;
                        case RIGHT:
                            break;
                        case SELECT:
                            switch (arrow)
                            {
                                case PLAY_LOCAL:
                                    actual_scene = PLAY_LOCAL;
                                    break;
                                case PLAY_MULTI:
                                    actual_scene = PLAY_LOCAL;
                                    break;
                                case EXIT:
                                    running = FALSE;
                                    break;
                            }
                            break;
                        case 'c':
                            
                            break;
                    }               
                break;
            case LOCAL_GAME:
                    game(MIN_ROWS, MIN_COLS);
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
            case MULTI_GAME:
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
            case EXIT_GAME:
                ask_if_want_exit();
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
    for (int i = 0; i < CONSOLE_WIDTH; i++) clean_line[i] = space[0];
    clean_line[CONSOLE_WIDTH] = '\0';

    // Apply it in every line of console
    for (int i = 0; i < CONSOLE_HEIGHT; i++)
    {
        WriteConsoleOutputCharacter(console_handle, clean_line, CONSOLE_WIDTH, coord(0, i), &characters);
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
        // Draw the arrow of it's absence
        WriteConsoleOutputCharacter(console_handle, arrow_text[is_arrow_in_this_line], arrow_len[is_arrow_in_this_line], coord(0, i), &characters);
        // Draw the actual menu line
        WriteConsoleOutputCharacter(console_handle, option_txt[i], option_len[i], coord(arrow_len[is_arrow_in_this_line], i), &characters);
        // Clear rest of the line
        len_already_written = arrow_len[is_arrow_in_this_line] + option_len[i];
        for (int j = len_already_written; j < CONSOLE_WIDTH; j++) WriteConsoleOutputCharacter(console_handle, space, 1, coord(j, i), &characters);
    }
}

void game(int grid_row_count, int grid_col_count)
{
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
    arrow = -1;
    // Clean the tiles
    for (int i = 0; i < grid_total_size; i++) tile[i] = ' ';
    // Clean input
    input = 0;

    draw_game(grid_rows_total_count, grid_cols_total_count);
    while (running)
    {
        input = getch();
        switch (input)
        {
        case 'q':
            return;
        case UP:
            if (tile_selected.Y > 0) tile_selected.Y--;
            break;
        case DOWN:
            if (tile_selected.Y < grid_row_count-1) tile_selected.Y++;
            //if (cursor_position.Y == grid_row_count)
            break;
        case LEFT:
            if (tile_selected.X > 0) tile_selected.X--;
            break;
        case RIGHT:
            if (tile_selected.X < grid_col_count-1) tile_selected.X++;
            break;
        default:
            break;
        }
        int x = 1 + (tile_selected.X * 4);
        int y = 1 + (tile_selected.Y * 2);
        SetConsoleCursorPosition(console_handle, coord(x, y));
    }
}

void draw_game(int rows, int cols)
{
    int tile_number = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // If column is even, draw the tiles
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    printf(" %c ", tile[tile_number]);
                    tile_number++;
                    //snprintf(grid, grid_string_lenght, " %c ", tile[0], tile[1], tile[2]);
                }
                else
                {
                    printf("|");
                }
            }
            // Else (is odd), draw the lines
            else
            {
                if (j % 2 == 0)
                {
                    printf("---");
                }
                else
                {
                    printf("|");
                }
                //snprintf(grid, grid_string_lenght, "═══╬═══╬═══");
            }
        }
        printf("\n");
    }
}
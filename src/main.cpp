#include "bz_base_coord.h"
#include "bz_clock.h"
#include "bz_color.h"
#include "bz_coord.h"
#include "bz_food.h"
#include "bz_keys.h"
#include "bz_snake.h"
#include "bz_win.h"
using namespace std;

namespace game_status
{
// initialize config arguments
short _max_x = 0, _max_y = 0;
int _food_cnt = 0;
int _default_time = 0, _min_time = 0, _max_time = 0;
short max_x, max_y;

int welcome_default_x = 2, welcome_default_y = 2;

int food_cnt;
int default_time;
int min_time, max_time;

short default_x, default_y, default_y_p2;

//initialize the waiting time after game-over
const int waiting_time = 5000;

//initialize the default welcome_choice
int welcome_choice = 0;

//2P
bool double_game = 0;

//create a container to store Foods
set<Food> foods;

//initialize scores
int score = 0;
int score_p2 = 0;

//initialize the interval time (speed / difficulty)
int interval_time;
int dtime = 0;

//initialize heads of the snakes
Snake *head, *head_p2;

//initialize move_directions
bz_coord::Arrow cur_direction;
bz_coord::Arrow cur_direction_p2;

//initialize game clock
Clock game_clock;
clock_t start_time, cur_time;

//for (bool)is_p2
const bool P1 = false;
const bool P2 = true;

//initialize coords
bz_coord default_coord;
bz_coord game_coord;
bz_coord time_coord;
bz_coord food_coord;

//initialize strings to print
const char *square_str = "■";
const char *food_str = "♣";

//special status
int max_speed_remaining_tick;
int last_interval_time;
int green_remaining_tick;
int green_remaining_tick_p2;
int reverse_remaining_tick;
int reverse_remaining_tick_p2;

//loop times
int tick_cnt;
} // namespace game_status

using namespace game_status;

void show_gameui(bz_coord &c)
{
    set_color(black, white);
    cls();

    set_color(136);
    c.move_to(0, 0);
    for (int i = 0; i < max_x / 2; ++i)
        printf(square_str);
    for (int i = 1; i < max_y - 1; ++i)
    {
        c.move_to(0, i);
        printf(square_str);
        c.move_to(max_x - 2, i);
        printf(square_str);
    }
    c.move_to(0, max_y - 1);
    for (int i = 0; i < max_x / 2; ++i)
        printf(square_str);

    set_color(black, white);
}

void show_about(bz_coord &c)
{
    cls();
    set_color(white, 1, 1);
    c.move_to(0, 2);
    printf("  bz' X-Snake 游戏说明\n\n");
    printf("  单人模式：经典的贪食蛇玩法。\n");
    printf("  双人模式：在经典贪食蛇玩法之上，避免撞到敌人，同时运用道具和技巧，\n");
    printf("            引诱敌人撞向自己。\n\n");
    printf("  各色食物：\n");
    printf("      黑色：一般的食物。\n");
    printf("      红色：蛇的移动速度提升。\n");
    printf("      蓝色：蛇的移动速度下降。\n");
    printf("      紫色：蛇在一段时间内进入『癫疯模式』。\n");
    printf("      黄色：让敌人或自己（单人模式）的操作反转一段时间。\n");
    printf("      绿色：让敌人或自己（单人模式）选择原谅。\n\n");
    printf("  自定玩法：\n");
    printf("      根据提示修改该目录下的 config.txt 文件以配置自定玩法。\n");
    c.move_to(0, 20);
    set_color(magenta, white);
    printf("  BugenZhao, SJTU, Jan 2019.");
    c.move_to(2, 24);
    set_color(yellow, 1, 1);
    printf("       Back       ");

    int key = 0;
    while (true)
    {
        key = getch();
        if (key == keys::key_enter)
            break;
    }
}

int show_welcome(bz_coord &c)
{
    for (int k : std::initializer_list<int>{0x25, 0x26, 0x27, 0x28, 'W', 'A', 'S', 'D'})
        GetAsyncKeyState(k);

    int key = 0, &now = welcome_choice;
    int &default_x = welcome_default_x,
        &default_y = welcome_default_y;

    set_color(magenta, white);
    cls();
    //set_color(white);

    c.move_to(default_x - 28, default_y - 10);
    cout << "___.          /\\  ____  ___           _________              __           ";
    c.move_to(default_x - 28, default_y - 9);
    cout << "\\_ |__ _______)/  \\   \\/  /          /   _____/ ____ _____  |  | __ ____  ";
    c.move_to(default_x - 28, default_y - 8);
    cout << " | __ \\\\___   /    \\     /   ______  \\_____  \\ /    \\\\__  \\ |  |/ // __ \\ ";
    c.move_to(default_x - 28, default_y - 7);
    cout << " | \\_\\ \\/    /     /     \\  /_____/  /        \\   |  \\/ __ \\|    <\\  ___/ ";
    c.move_to(default_x - 28, default_y - 6);
    cout << " |___  /_____ \\   /___/\\  \\         /_______  /___|  (____  /__|_ \\\\___  >";
    c.move_to(default_x - 28, default_y - 5);
    cout << "     \\/      \\/         \\_/                 \\/     \\/     \\/     \\/    \\/ ";

    while (true)
    {
        set_color(white, 1, 1);

        c.move_to(default_x, default_y);
        if (now == 0)
            set_color(yellow, 1, 1);
        printf("     Single Game    ");
        set_color(white, 1, 1);
        c.move_to(default_x, default_y + 2);
        if (now == 1)
            set_color(yellow, 1, 1);
        printf("      2 Players     ");
        set_color(white, 1, 1);
        c.move_to(default_x, default_y + 4);
        if (now == 2)
            set_color(yellow, 1, 1);
        printf("    Introductions   ");
        set_color(white, 1, 1);
        c.move_to(default_x, default_y + 6);
        if (now == 3)
            set_color(yellow, 1, 1);
        printf("        Exit        ");

        set_color(magenta, white);
        c.move_to(default_x, default_y + 10);
        printf("/* BugenZhao 2019 */");

        key = getch();
        if (key == keys::key_enter)
            return now;
        if (key != 224)
            continue;
        key = getch();
        if (key == keys::key_up)
            now = (now - 1 >= 0) ? now - 1 : now;
        if (key == keys::key_down)
            now = (now + 1 <= 3) ? now + 1 : now;
    }
}

void generate_foods(int cnt, bz_coord &c)
{
    int x, y;
    while (cnt)
    {
        Snake *p = head;
        Snake *q = head_p2;
        bool ok = 1;
        x = 2 * (rand() % (max_x / 2 - 2)) + 2;
        y = rand() % (max_y - 2) + 1;
        while (p && ok)
        {
            if (p->coord.X == x && p->coord.Y == y)
                ok = 0;
            p = p->next;
        }
        if (double_game)
        {
            while (q && ok)
            {
                if (q->coord.X == x && q->coord.Y == y)
                    ok = 0;
                q = q->next;
            }
        }
        if (!ok)
            continue;
        int color_num = rand() % 12;
        Color color;
        if (color_num <= 1)
            color = red; // red boost
        else if (color_num == 2)
            color = blue; // blue cool
        else if (color_num == 3)
            color = green; // +2
        else if (color_num == 4)
            color = yellow; //reverse
        else if (color_num == 5)
            color = magenta; // boost!!!!!!
        else
            color = black; // normal
        Food food(x, y, color);
        //foods.push_back(food);
        foods.insert(food);
        c.move_to(x, y);
        set_color(color, white);
        printf(food_str);
        set_color(white, 1, 1);
        --cnt;
    }
}

void show_snake(bz_coord &c, bool is_p2)
{
    auto p = is_p2 ? head_p2 : head;
    while (p)
    {
        c.move_to(p->coord);
        if (is_p2)
        {
            if (green_remaining_tick_p2)
                set_color(green, white, 1, 1);
            else if (reverse_remaining_tick_p2)
                set_color(yellow, white, 1, 1);
            else
                set_color(p->color, white, 1, 1);
        }
        else
        {
            if (green_remaining_tick)
                set_color(green, white);
            else if (reverse_remaining_tick)
                set_color(yellow, white);
            else
                set_color(p->color, white);
        }
        printf(square_str);
        p = p->next;
    }
    set_color(white, 1, 1);
}

base_coord get_snake_tail(Snake *head)
{
    while (head->next)
        head = head->next;
    return base_coord(head->coord.X, head->coord.Y);
}

Snake *extend_snake(Snake *head, base_coord new_head_pos)
{
    auto *new_head = new Snake(new_head_pos);
    new_head->next = head;
    return new_head;
}

bz_coord::Arrow do_get_direction(bz_coord::Arrow cur_direction,
                                 int up, int down, int left, int right)
{
    bz_coord::Arrow new_direction = cur_direction;
    if (GetAsyncKeyState(up) && cur_direction != bz_coord::Arrow::down)
        new_direction = bz_coord::Arrow::up;
    else if (GetAsyncKeyState(down) && cur_direction != bz_coord::Arrow::up)
        new_direction = bz_coord::Arrow::down;
    else if (GetAsyncKeyState(left) && cur_direction != bz_coord::Arrow::right)
        new_direction = bz_coord::Arrow::left;
    else if (GetAsyncKeyState(right) && cur_direction != bz_coord::Arrow::left)
        new_direction = bz_coord::Arrow::right;
    return new_direction;
}

bz_coord::Arrow get_direction(bool is_p2)
{
    if (!is_p2)
    {
        if (reverse_remaining_tick)
            return do_get_direction(cur_direction, VK_DOWN, VK_UP, VK_RIGHT, VK_LEFT);
        else
            return do_get_direction(cur_direction, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
    }
    else
    {
        if (reverse_remaining_tick_p2)
            return do_get_direction(cur_direction_p2, 'S', 'W', 'D', 'A');
        else
            return do_get_direction(cur_direction_p2, 'W', 'S', 'A', 'D');
    }
}

Snake *move_snake(bz_coord &c, bool is_p2)
{
    Snake *&head = is_p2 ? game_status::head_p2 : game_status::head;
    bz_coord::Arrow &direction = is_p2 ? game_status::cur_direction_p2 : game_status::cur_direction;
    int &score = is_p2 ? game_status::score_p2 : game_status::score;

    auto it = foods.find(Food(head->coord.X, head->coord.Y, white));
    bool eat = (it != foods.end());

    if (eat)
    {
        if (it->color == green)
            score += 4;
        else if (it->color == white)
            score += 1;
        else if (it->color == blue)
            score += 1;
        else if (it->color == red)
            score += 3;
        else
            score += 2;
    }

    c.move_to(head->coord);
    bool move_ok = c.move(direction);
    if (!move_ok)
        return nullptr;

    Snake *new_head;

    if (!eat)
    {
        base_coord fore_coord = head->coord;
        head->coord = {c.x(), c.y()};
        Snake *p = head->next;
        while (p->next)
        {
            swap(p->coord, fore_coord);
            p = p->next;
        }
        Snake *tail = p;
        c.move_to(tail->coord);
        printf("  ");
        swap(p->coord, fore_coord);
        new_head = head;
    }
    else
    {
        new_head = new Snake(c);
        new_head->next = head;
        new_head->color = it->color;
        if (it->color == red)
            dtime = -30;
        if (it->color == blue)
            dtime = 30;
        if (it->color == magenta)
            max_speed_remaining_tick = 30, last_interval_time = interval_time;
        foods.erase(it);
    }
    if (eat && it->color == green)
    {
        if (!double_game)
            green_remaining_tick = 20;
        else if (is_p2)
            green_remaining_tick = 20;
        else
            green_remaining_tick_p2 = 20;
    }
    else if (eat && it->color == yellow)
    {
        if (!double_game)
            reverse_remaining_tick = 15;
        else if (is_p2)
            reverse_remaining_tick = 7;
        else
            reverse_remaining_tick_p2 = 7;
    }

    return new_head;
}

bool check_bite(Snake *head, Snake *head_p2)
{
    auto p = head_p2;
    bool ret = 0;
    while (p->next && !ret)
    {
        p = p->next;
        if (p->coord == head->coord)
            ret = 1;
    }
    return ret;
}

void env_initialize()
{
    SetLayeredWindowAttributes(GetConsoleWindow(), 0, 240, LWA_ALPHA);
    set_color(white, 1, 1);
    cls();

    //try to read the config file
    ifstream fin("config.txt");
    fin >> _max_x >> _max_y;
    fin >> _food_cnt;
    fin >> _default_time >> _min_time >> _max_time;

    max_x = _max_x ? _max_x : 80;
    max_y = _max_y ? _max_y : 30;
    food_cnt = _food_cnt ? _food_cnt : 6;
    default_time = _default_time ? _default_time : 130;
    min_time = _min_time ? _min_time : 40;
    max_time = _max_time ? _max_time : 160;

    //set the default positions of two snakes
    default_x = max_x / 4 * 2 - 6;
    default_y = max_y / 6 * 2;
    default_y_p2 = default_y * 2;

    welcome_default_x = max_x / 2 + 5;
    welcome_default_y = max_y / 2;

    //fix encoding problem on non-utf8 devices
    utf8_fix();

    //hide the cursor
    hide_cursor();

    //set the window
    set_window_status("bz' X-Snake");

    //initialize random numbers
    srand(time(nullptr) + 1018);

    //set the size of the window
    string resize_cmd = "mode con cols=" + to_string(max_x + 30) + " lines=" + to_string(max_y);
    system(resize_cmd.c_str());

    //set the interval time
    interval_time = default_time;

    //configure bz_coords
    game_coord = bz_coord(1, 1, 1, 1, max_x - 2, max_y - 1);
    game_coord.set_dx(2);
    game_coord.set_dy(1);
}

void game_initialize()
{
    cls();

    show_gameui(default_coord);

    //make snakes
    head = new Snake(default_x, default_y);
    head_p2 = new Snake(default_x, default_y_p2);
    head->next = nullptr;
    head_p2->next = nullptr;
    for (short x = default_x + 2; x < default_x + 8; x += 2)
        head = extend_snake(head, {x, default_y});
    if (double_game)
        for (short x = default_x + 2; x < default_x + 8; x += 2)
            head_p2 = extend_snake(head_p2, {x, default_y_p2});

    //generate foods
    foods.clear();
    generate_foods(food_cnt + double_game * food_cnt, food_coord);

    //set default directions
    cur_direction = bz_coord::Arrow::right;
    cur_direction_p2 = bz_coord::Arrow::right;

    //clear the scores
    score = 0;
    score_p2 = 0;

    //set the interval time
    interval_time = default_time;
    dtime = 0;

    //print Score UI
    default_coord.move_to(max_x + 8, default_y);
    if (!double_game)
        default_coord.fill_in("Score: ");
    else
    {
        default_coord.fill_in("Score P1: ");
        default_coord.move_to(max_x + 8, default_y + 2);
        default_coord.fill_in("Score P2: ");
    }

    //set the start time
    start_time = clock();
    game_clock.clear();

    //set special status
    max_speed_remaining_tick = 0;
    last_interval_time = 0;
    green_remaining_tick = 0;
    green_remaining_tick_p2 = 0;
    reverse_remaining_tick = 0;
    reverse_remaining_tick_p2 = 0;

    for (int k : std::initializer_list<int>{0x25, 0x26, 0x27, 0x28, 'W', 'A', 'S', 'D'})
        GetAsyncKeyState(k);

    tick_cnt = 0;
}

inline bool game_over()
{
    //judge game-over (hitting the walls)
    if (!double_game && head == nullptr)
    {
        default_coord.move_to(max_x + 9, default_y_p2 + 5);
        set_color(yellow, 1, 1);
        default_coord.fill_in(" Game Over. ");
        return true;
    }
    else if (double_game)
    {
        if (head == nullptr && head_p2 == nullptr)
        {
            default_coord.move_to(max_x + 9, default_y_p2 + 5);
            set_color(yellow, 1, 1);
            default_coord.fill_in(" Draw. ");
            return true;
        }
        else if (head == nullptr)
        {
            default_coord.move_to(max_x + 9, default_y_p2 + 5);
            set_color(yellow, 1, 1);
            default_coord.fill_in(" P2 Win. ");
            return true;
        }
        else if (head_p2 == nullptr)
        {
            default_coord.move_to(max_x + 9, default_y_p2 + 5);
            set_color(yellow, 1, 1);
            default_coord.fill_in(" P1 Win. ");
            return true;
        }
    }

    //judge game-over (bite)
    if (!double_game && check_bite(head, head))
    {
        default_coord.move_to(max_x + 9, default_y_p2 + 5);
        set_color(yellow, 1, 1);
        default_coord.fill_in(" Game Over. ");
        return true;
    }
    else if (double_game)
    {
        bool p1_lose = check_bite(head, head_p2) || check_bite(head, head);
        bool p2_lose = check_bite(head_p2, head) || check_bite(head_p2, head_p2);
        if (p1_lose && p2_lose)
        {
            default_coord.move_to(max_x + 9, default_y_p2 + 5);
            set_color(yellow, 1, 1);
            default_coord.fill_in(" Draw. ");
            return true;
        }
        else if (p1_lose)
        {
            default_coord.move_to(max_x + 9, default_y_p2 + 5);
            set_color(yellow, 1, 1);
            default_coord.fill_in(" P2 Win. ");
            return true;
        }
        else if (p2_lose)
        {
            default_coord.move_to(max_x + 9, default_y_p2 + 5);
            set_color(yellow, 1, 1);
            default_coord.fill_in(" P1 Win. ");
            return true;
        }
    }
    return false;
}

inline void update_special_status()
{
    if (max_speed_remaining_tick)
        --max_speed_remaining_tick;
    if (green_remaining_tick)
        --green_remaining_tick;
    if (green_remaining_tick_p2)
        --green_remaining_tick_p2;
    if (reverse_remaining_tick)
        --reverse_remaining_tick;
    if (reverse_remaining_tick_p2)
        --reverse_remaining_tick_p2;
}

int main()
{
    env_initialize();

main_begin:
    //menu

    while (true)
    {
        welcome_choice = show_welcome(default_coord);
        if (welcome_choice == 0)
        {
            double_game = 0;
            break;
        }
        else if (welcome_choice == 1)
        {
            double_game = 1;
            break;
        }
        else if (welcome_choice == 2)
            show_about(default_coord);
        else
            return 0;
    }

    set_color(white, 1, 1);
    cls();

    game_initialize();

    //game start
    while (true)
    {
        ++tick_cnt;

        update_special_status();

        //show snakes
        show_snake(game_coord, P1);
        if (double_game)
            show_snake(game_coord, P2);

        //double game indicator
        if (tick_cnt == 1 && double_game)
        {
            set_color(black, white);
            default_coord.move_to(default_x + 9, default_y);
            default_coord.fill_in("P1");
            default_coord.move_to(default_x + 9, default_y_p2);
            set_color(black, white, 1, 1);
            default_coord.fill_in("P2");
            Sleep(2000);
            default_coord.move_to(default_x + 9, default_y);
            default_coord.fill_in("  ");
            default_coord.move_to(default_x + 9, default_y_p2);
            default_coord.fill_in("  ");
        }

        //print scores
        default_coord.move_to(max_x + 20, default_y);
        printf("%d", score);
        if (double_game)
        {
            default_coord.move_to(max_x + 20, default_y + 2);
            printf("%d", score_p2);
        }

        //show current time
        time_coord.move_to(max_x + 13, default_y - 5);
        cur_time = clock();
        game_clock.set_time((cur_time - start_time) / 1000);
        game_clock.print();

        //SLEEP!!!
        Sleep(interval_time);
        dtime = 0;

        //get directions, move snakes and process foods
        cur_direction = get_direction(P1);
        head = move_snake(game_coord, P1);
        if (double_game)
        {
            cur_direction_p2 = get_direction(P2);
            head_p2 = move_snake(game_coord, P2);
        }

        //adjust interval time
        if (max_speed_remaining_tick)
        {
            if (max_speed_remaining_tick >= 20)
                dtime = min_time - interval_time;
            else if (max_speed_remaining_tick >= 7)
                dtime = default_time - interval_time - 30;
            if (max_speed_remaining_tick == 1)
                dtime = default_time - interval_time;
        }
        interval_time += dtime;
        if (interval_time > max_time)
            interval_time = max_time;
        if (interval_time < min_time)
            interval_time = min_time;

        //show speed status
        if (dtime)
        {
            default_coord.move_to(max_x + 5, default_y_p2 - 4);
            if (interval_time < 0.4 * default_time)
            {
                set_color(red, white, 1, 1);
                printf("            ._._._.");
                default_coord.move(bz_coord::down);
                printf(" __ ________| | | |  ");
                default_coord.move(bz_coord::down);
                printf("|  |  \\____ \\ | | |  ");
                default_coord.move(bz_coord::down);
                printf("|  |  /  |_> >|\\|\\|  ");
                default_coord.move(bz_coord::down);
                printf("|____/|   __/______  ");
                default_coord.move(bz_coord::down);
                printf("      |__|   \\/\\/\\/  ");
                set_color(white, 1, 1);
            }
            else if (interval_time < 0.7 * default_time)
            {
                set_color(red, white);
                printf("             ._._.   ");
                default_coord.move(bz_coord::down);
                printf("  __ ________| | |   ");
                default_coord.move(bz_coord::down);
                printf(" |  |  \\____ \\ | |   ");
                default_coord.move(bz_coord::down);
                printf(" |  |  /  |_> >|\\|   ");
                default_coord.move(bz_coord::down);
                printf(" |____/|   __/____   ");
                default_coord.move(bz_coord::down);
                printf("       |__|   \\/\\/   ");
                set_color(white, 1, 1);
            }
            else if (interval_time < default_time)
            {
                set_color(red, white);
                printf("              ._.    ");
                default_coord.move(bz_coord::down);
                printf("   __ ________| |    ");
                default_coord.move(bz_coord::down);
                printf("  |  |  \\____ \\ |    ");
                default_coord.move(bz_coord::down);
                printf("  |  |  /  |_> >|    ");
                default_coord.move(bz_coord::down);
                printf("  |____/|   __/__    ");
                default_coord.move(bz_coord::down);
                printf("        |__|   \\/    ");
                set_color(white, 1, 1);
            }
            else if (interval_time > default_time)
            {
                set_color(blue, white);
                printf("     __              ");
                default_coord.move(bz_coord::down);
                printf(" ___/ /__ _    _____ ");
                default_coord.move(bz_coord::down);
                printf("/ _  / _ \\ |/|/ / _ \\");
                default_coord.move(bz_coord::down);
                printf("\\_,_/\\___/__,__/_\/\/_/");
                default_coord.move(bz_coord::down);
                printf("                     ");
                default_coord.move(bz_coord::down);
                printf("                     ");
                set_color(white, 1, 1);
            }
            else
            {
                printf("                     ");
                default_coord.move(bz_coord::down);
                printf("                     ");
                default_coord.move(bz_coord::down);
                printf("                     ");
                default_coord.move(bz_coord::down);
                printf("                     ");
                default_coord.move(bz_coord::down);
                printf("                     ");
                default_coord.move(bz_coord::down);
                printf("                     ");
            }
        }

        bool is_over = game_over();
        if (is_over)
        {
            Snake *p = head, *q;
            while (p)
            {
                q = p;
                p = p->next;
                delete q;
            }
            if (double_game)
            {
                p = head_p2;
                while (p)
                {
                    q = p;
                    p = p->next;
                    delete q;
                }
            }
            Sleep(waiting_time);
            goto main_begin;
        }

        //generate foods
        if (foods.size() <= (unsigned)food_cnt / 2 + double_game * food_cnt / 2)
            generate_foods(food_cnt / 2 + double_game * food_cnt / 2, game_coord);
    }
    cls();
    return 0;
}

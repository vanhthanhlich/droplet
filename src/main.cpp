#include <projectsetting.hpp>

#define SLEEP(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
typedef boost::coroutines2::asymmetric_coroutine<int> coro_t;

const int TARGET_FPS = 24;
const int RADIUS = 8;
std::list<coro_t::pull_type> animations;
PointsInCircle pic(RADIUS);

#define BLUE_PAIR 8
#define YELLOW_PAIR 9
#define GREEN_PAIR 10
#define WHITE_PAIR 11

void Setup() {
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    curs_set(0);

    keypad(stdscr, true);

    mmask_t mask = BUTTON1_CLICKED, _;
    mousemask(mask, &_);

    mouseinterval(20);  
    
    start_color();
    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);  
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);  
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);  
    init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);  
    
}

float lerp(float x) {
    if(x > 1) x = 1;
    if(x < 0) x = 0;

    if(x <= 0.678) return 1.4 * x * x + 0.3;
    return 4 * (x - 1) * (x - 1) + 0.5;

}

void Animation(coro_t::push_type& yield, int my, int mx, std::string s = "omaiga") {
    std::string Gradient = " .:-=+*#%@$#";
    std::vector<int> startPoint(RADIUS + 1, 0);

    assert(startPoint.size() == pic.Points.size());

    for(int i = 0; i < startPoint.size(); i ++) {
        startPoint[i] = lerp((float)i / RADIUS) * (Gradient.size() - 1);
    }

    for(int time = 0; time <= Gradient.size() * RADIUS; time ++) {

        for(int i = 0; i < pic.Points.size(); i ++) {
            if(startPoint[i] < 0) continue;
            if(i > time) break;

            char c = Gradient[startPoint[i] --];
            for(auto [dx, dy] : pic.Points[i]) {
                mvprintw(my + dy, mx + dx, "%c", c);
            }
        }
        yield(0);

    }
    


}

void mouse_handling() {
    MEVENT event;
    if(getmouse(&event) != OK) return;
    animations.emplace_back([y = event.y, x = event.x](coro_t::push_type& yield){
        Animation(yield, y, x);
    });
}

int input_handling() {

    int event = getch();
    switch (event)
    {
    case 'q':
        return -1;
    case KEY_MOUSE:
        mouse_handling();
        break;
    default:
        break;
    }
    
    return 0;
}

int arg_handling(int argc, char *argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    for(int i = 1; i < args.size(); i ++) {
        
        if(args[i] == "-h" || args[i] == "--hel[]") {
            printf("Usage: %s [optinal] -C color\n", argv[0]);
            return -1;
        }
        else if(args[i] == "-C" && i + 1 < args.size()) {
            short color_pair = WHITE_PAIR;
            
            if(args[i + 1] == "blue") color_pair = BLUE_PAIR;
            else if(args[i + 1] == "yellow") color_pair = YELLOW_PAIR;
            else if(args[i + 1] == "green") color_pair = GREEN_PAIR;

            attron(COLOR_PAIR(color_pair));
        }

    }
    return 0;
}

int main(int argc, char *argv[]) {

    Setup();
    if(arg_handling(argc, argv) == -1) {
        endwin();
        return 0;
    }

    int running = 1;
    while(running) {
        if(input_handling() == -1) break;
        
        for(auto it = animations.begin(); it != animations.end(); ) {
            (*it)();
            if(!(*it)) it = animations.erase(it);
            else it ++;
        }
        
        refresh();
        SLEEP(1000 / TARGET_FPS);
    }


    endwin();

    return 0;    
}
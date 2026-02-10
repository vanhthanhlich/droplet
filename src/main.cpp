#include <ncurses.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>
#include <list>
#include <boost/coroutine2/all.hpp>
#include <algorithm>
#include "PointsInCircle.hpp"

#define SLEEP(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
typedef boost::coroutines2::asymmetric_coroutine<int> coro_t;

const int TARGET_FPS = 24;
const int RADIUS = 10;
std::list<coro_t::pull_type> animations;
PointsInCircle pic(RADIUS);

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
}

void Animation(coro_t::push_type& yield, int my, int mx, std::string s = "omaiga") {
    int time = 0;
    std::string Gradient = "#&$@%#*+=-:. ";

    for(; time <= RADIUS * Gradient.size(); time++) {

        for(int i = 0; i < pic.Points.size() - 1; i ++) {
            int state = time - i;
            if(state >= Gradient.size()) continue;
            if(state < 0) break;

            char c = Gradient[state];
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


int main(int argc, char **argv) {

    Setup();

    int running = 1;
    while(running) {
        if(input_handling() == -1) break;
        
        for(auto it = animations.begin(); it != animations.end(); ) {
            (*it)();
            if(!(*it)) it = animations.erase(it);
            else it ++;
        }
        
        // fprintf(stderr, "%d\n", animations.size());
        refresh();
        SLEEP(1000 / TARGET_FPS);
    }


    endwin();

    return 0;    
}
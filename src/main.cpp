#include <SFML/System.hpp>
#include "game.hpp"
#include <gtkmm.h>

//Create a separate global thread for our game, so that the gtk frontend and the game
//run in different threads

class launcher
{
private:
    Gtk::Window &window;
    Gtk::SpinButton &winsize_spin;
    static sf::Thread game;
public:
    void startgame();
    launcher(Gtk::Window &window, Gtk::SpinButton &winsize_spin) : window(window), winsize_spin(winsize_spin){};
};

sf::Thread launcher::game(&game::init);

void launcher::startgame()
{
    window.iconify();
    int winsize_int = winsize_spin.get_value_as_int();
    game::setwindowsize(winsize_int);
    game.launch();
}

int main(int argc, char **argv) {
    Gtk::Main kit(argc, argv);

    Gtk::Window window;

    Gtk::SpinButton winsize_spin;
	
    launcher gnarrom(window, winsize_spin);

    Gtk::VBox mainbox;
    Gtk::HBox controlbox;
    Gtk::HBox winsizebox;
    
    Gtk::Button start("Launch");
    start.show();
    start.signal_clicked().connect(sigc::mem_fun(&gnarrom, &launcher::startgame));

    Gtk::Button stop("Quit");
    stop.show();
    stop.signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));
    
    Gtk::Label winsizelabel("Window Size");
    winsizelabel.show();

    winsize_spin.set_range(400, 1200);
    winsize_spin.set_increments(1, 10);
    winsize_spin.set_value(700);
    winsize_spin.show();

    controlbox.pack_start(start);
    controlbox.pack_end(stop);
    controlbox.show();
    
    winsizebox.pack_start(winsizelabel);
    winsizebox.pack_end(winsize_spin);
    winsizebox.show();

    mainbox.pack_start(winsizebox);
    mainbox.pack_end(controlbox);
    mainbox.show();
    
    window.add(mainbox);

    Gtk::Main::run(window);
    
    return 0;
}

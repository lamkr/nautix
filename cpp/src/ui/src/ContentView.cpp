#include "ui/ContentView.h"
namespace nautix::ui {
    ContentView::ContentView() {
        auto label = Gtk::make_managed<Gtk::Label>("Content View");
        label->set_halign(Gtk::Align::CENTER);
        label->set_valign(Gtk::Align::CENTER);
        append(*label);

        set_vexpand(true);
    }
}
#include "ui/NavigationPanel.h"

namespace nautix::ui {
    NavigationPanel::NavigationPanel() {
        // Um Gtk::Box não tem um "label", então adicionamos um Gtk::Label dentro dele
        auto label = Gtk::make_managed<Gtk::Label>("Navigation Panel");
        label->set_halign(Gtk::Align::CENTER); // Centraliza o texto
        label->set_valign(Gtk::Align::CENTER);
        append(*label); // Define o label como o conteúdo do Box

        set_vexpand(true);
    }
}
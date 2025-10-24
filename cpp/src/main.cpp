#include <gtkmm/application.h>
#include <gtkmm/builder.h>

#include "adw/Application.h"

class MyWindow : public Gtk::Window {
public:
    MyWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& p_builder);
    ~MyWindow() override;

protected:
    void on_button_clicked();

private:
    Glib::RefPtr<Gtk::Button> m_button;
};

int main(int argc, char* argv[]) {
    adw_init();

    GtkBuilder* builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "main.ui", NULL) == 0) {
        g_printerr("Error loading UI file: builder.ui\n");
        g_object_unref(builder);
        return 1;
    }

    // Get the main window object by its ID
    auto windows = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (windows) {
        g_printerr("Error: Could not get 'main_window' object.\n");
        g_object_unref(builder);
        return 1;
    }

    auto adw_app = adw_application_new("blaal", G_APPLICATION_DEFAULT_FLAGS);

    adw_gtk_application_add_window(adw_app, GTK_WINDOW(windows));

    /*auto gtkapp = Gtk::Application::create("org.nautix.app");
    gtkapp->signal_startup().connect([]() {
        //load_css_data();
    });
    gtkapp->run(argc, argv);

    auto app = adw::Application::create("org.nautix.app");
    return app->run(argc, argv);
    auto app = adw::Application::create("org.nautix.app");

    app->signal_startup().connect([]() {
    });

    return app->run(argc, argv);*/

}

/*#include <gtkmm/application.h>
#include <adwaita.h>
#include <iostream>
#include "ui/MainWindow.h"

void load_css_data() {
    const auto css_provider = Gtk::CssProvider::create();

    css_provider->load_from_data(
        ".status-bar {"
        "   border-radius: 0;"
        "}"
        ".solid-csd windowcontrols, "
        ".solid-csd headerbar {"
        "   border-radius: 0;"
        "   box-shadow: none;"
        "}"
        "window.solid-csd {"
        "  border-radius: 0;"
        "}"
    );

    // Adiciona o provedor ao display padrão para que ele se aplique a todas as janelas.
    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

int main(const int argc, char* argv[]) {
    adw_init();
    const auto app = Gtk::Application::create("org.nautix.Nautix", Gio::Application::Flags::DEFAULT_FLAGS);

    app->signal_startup().connect([]() {
        load_css_data();
    });

    return app->make_window_and_run<nautix::ui::MainWindow>(argc, argv);
}
*/
/*#include <libintl.h>
#include <clocale>

#include "common/include/common/Logger.h"

int main() {
    // Inicializa o logger como a primeira coisa a ser feita
    Logger::init();
    Logger::get()->info("Bem-vindo ao Nautix!");

    // Setup locale and i18n
    setlocale(LC_ALL, "");
    bindtextdomain("nautix", "/usr/share/locale"); // Ou outro diretório de instalação
    textdomain("nautix");
    return 0;
}

*/

#include <gtk/gtk.h>
#include <adwaita.h>
#include <string>
#include <print>
#include <memory>
#include <gtkmm/button.h>

static void add_new_tab(AdwTabView *tab_view) {
    static int tab_count = 0;
    tab_count++;

    // Create the child widget for the new tab.
    auto* label = gtk_label_new(("Hello from tab " + std::to_string(tab_count)).c_str());
    gtk_widget_set_hexpand(GTK_WIDGET(label), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(label), TRUE);

    // Append the new tab and get the page object.
    auto* page = adw_tab_view_append(tab_view, GTK_WIDGET(label));

    // Set the title on the page object.
    adw_tab_page_set_title(page, ("Tab " + std::to_string(tab_count)).c_str());

    // Switch to the newly created tab.
    adw_tab_view_set_selected_page(tab_view, page);
}

static void close_selected_tab(AdwTabView *tab_view) {
    if (adw_tab_view_get_n_pages(tab_view) > 0) {
        adw_tab_view_close_page(tab_view, adw_tab_view_get_selected_page(tab_view));
    }
}

static void on_new_tab_clicked(Gtk::Button& , AdwTabView *tab_view) {
    add_new_tab(tab_view);
}

static void on_close_tab_clicked(Gtk::Button& , AdwTabView *tab_view) {
    close_selected_tab(tab_view);
}

static void on_activate(GtkApplication *app) {
    // Adw is initialized implicitly when using AdwApplication,
    // but if using a regular GtkApplication, you would call:
    // adw_init();

    // Create the main application window.
    auto* window = adw_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "C++23 AdwTabView Demo");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create the TabView and TabBar.
    AdwTabView* tab_view = adw_tab_view_new();
    AdwTabBar* tab_bar = adw_tab_bar_new();

    // Attach the TabBar to the TabView.
    adw_tab_bar_set_view(tab_bar, tab_view);

    // Create a header bar with buttons.
    GtkWidget* header_bar = adw_header_bar_new();
    auto* new_tab_button = gtk_button_new_from_icon_name("tab-new-symbolic");
    auto* close_tab_button = gtk_button_new_from_icon_name("window-close-symbolic");

    adw_header_bar_pack_start(reinterpret_cast<AdwHeaderBar*>(header_bar), new_tab_button);
    adw_header_bar_pack_end(reinterpret_cast<AdwHeaderBar*>(header_bar), close_tab_button);

    // Connect button signals.
    g_signal_connect(new_tab_button, "clicked", G_CALLBACK(on_new_tab_clicked), tab_view);
    g_signal_connect(close_tab_button, "clicked", G_CALLBACK(on_close_tab_clicked), tab_view);

    // Arrange widgets in a main content box.
    auto* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(content_box), header_bar);
    gtk_box_append(GTK_BOX(content_box), reinterpret_cast<GtkWidget*>(tab_bar));
    gtk_box_append(GTK_BOX(content_box), reinterpret_cast<GtkWidget*>(tab_view));

    // Set the content of the window.
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), content_box);

    // Add an initial tab.
    add_new_tab(tab_view);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    // The C++23 std::print function can be used for logging.
    std::print("Starting application...\n");

    // Create an AdwApplication, which automatically handles
    // libadwaita initialization and stylesheet.
    auto* app = adw_application_new("org.example.adwtabviewdemo", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), nullptr);

    auto status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

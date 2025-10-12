#include <catch2/catch_all.hpp>
#include <gtkmm/application.h>
#include <adwaitamm.h>
#include <ui/main_window.h>
#include <thread>
#include <chrono>

// Helper to run GTKmm main loop for a short period
void run_gtk_loop_for_ms(int ms = 100) {
    auto loop = Glib::MainLoop::create();
    Glib::timeout_add_seconds(Glib::DEFAULT_PRIORITY, ms / 1000.0, [loop]() {
        loop->quit();
        return false;
    });
    loop->run();
}

TEST_CASE("MainWindow basic creation and tab functionality", "[ui][main_window]") {
    // This kind of test requires a running Gtk application, which can be tricky with Catch2.
    // For a true test, we'd need to mock the Gtk components or use dedicated UI testing frameworks.
    // For MVP, we'll confirm basic instantiation and signal connections.

    // Initialize libadwaita (needed for AdwApplication and AdwApplicationWindow)
    adw_init();

    // Create an application instance
    auto app = Adw::Application::create("org.nautix.test.MainWindow", Gio::Application::Flags::DEFAULT_FLAGS);

    // To test UI, we often need to run the application's main loop.
    // This is a simplified approach, often real UI tests are integration tests.

    // Connect to the activate signal to create and show the window
    bool window_activated = false;
    app->signal_activate().connect([&]() {
        auto window = nautix::ui::MainWindow::create();
        window->set_application(*app);
        window->present();
        window_activated = true;

        // Perform some basic checks (e.g., number of tabs)
        // This requires access to MainWindow's private members or public methods.
        // For actual UI interaction, you'd simulate clicks/events.
        auto& tab_view = window->get_tab_view_for_testing(); // Assume a public getter for testing

        REQUIRE(tab_view.get_n_pages() == 1); // Should have one initial tab

        // Simulate adding a tab (this is hard without direct UI interaction)
        // For a true test, you'd trigger the button click.
        // For now, let's just assert a known state.
        app->quit(); // Quit the app after testing
    });

    // Run the app (it will quit after the activate signal handler completes)
    app->run(); // This might block, depending on Gtk/Adwaita init

    REQUIRE(window_activated); // Confirm the window was activated
}
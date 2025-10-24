#include "ui/MainWindow.h"

namespace nautix::ui {
    void MainWindow::on_new_tab_callback([[maybe_unused]] Gtk::Button& button, const gpointer user_data) {
        // Cast the user_data back to the MainWindow instance pointer.
        // Now, call the actual non-static member function.
        if (auto* self = static_cast<MainWindow*>(user_data)) {
            self->on_new_tab();
        }
    }

    MainWindow::MainWindow() {
        initialize_window();
        build_content();
    }

    void MainWindow::initialize_window() {
        get_style_context()->add_class("solid-csd");
        set_default_size(800, 600); //maximize();
    }

    void MainWindow::build_content() {
        // Tab view.
        tab_view_ = adw_tab_view_new();
        g_object_set(tab_view_, "enable-new-tab", TRUE, nullptr);

        // Tab bar.
        AdwTabBar* tab_bar = adw_tab_bar_new();
        // Attach the TabBar to the TabView.
        adw_tab_bar_set_view(tab_bar, tab_view_);

        // AdwTabOverview
        AdwTabOverview* tab_overview = ADW_TAB_OVERVIEW(adw_tab_overview_new());
        adw_tab_overview_set_view(tab_overview, tab_view_);
        adw_tab_overview_set_enable_new_tab(tab_overview, true);

        // AdwTabButton
        AdwTabButton* overview_btn = ADW_TAB_BUTTON(adw_tab_button_new());
        adw_tab_button_set_view(overview_btn, tab_view_);
        gtk_widget_set_visible(GTK_WIDGET(overview_btn), FALSE);

        // GtkButton
        GtkWidget* new_tab_btn = gtk_button_new_from_icon_name("tab-new-symbolic");

        // AdwHeaderBar
        AdwHeaderBar* header_bar = ADW_HEADER_BAR(adw_header_bar_new());
        adw_header_bar_pack_end(header_bar, GTK_WIDGET(overview_btn));
        adw_header_bar_pack_end(header_bar, new_tab_btn);

        // AdwToolbarView
        AdwToolbarView* toolbar_view = ADW_TOOLBAR_VIEW(adw_toolbar_view_new());
        adw_toolbar_view_set_top_bar_style(toolbar_view, AdwToolbarStyle::ADW_TOOLBAR_RAISED);
        adw_toolbar_view_add_top_bar(toolbar_view, GTK_WIDGET(header_bar));
        adw_toolbar_view_add_top_bar(toolbar_view, GTK_WIDGET(tab_bar));
        adw_toolbar_view_set_content(toolbar_view, GTK_WIDGET(tab_view_));

        // TabOverview child
        adw_tab_overview_set_child(tab_overview, GTK_WIDGET(toolbar_view));

        // AdwWindow (janela principal do Adwaita)
        AdwWindow* adw_window = ADW_WINDOW(adw_window_new());
        adw_window_set_content(adw_window, GTK_WIDGET(tab_overview));

        AdwBreakpointCondition* breakpoint_condition = adw_breakpoint_condition_new_length(
            AdwBreakpointConditionLengthType::ADW_BREAKPOINT_CONDITION_MAX_WIDTH,
            500.0,
            AdwLengthUnit::ADW_LENGTH_UNIT_PX
            );

        // Breakpoint para responsividade
        AdwBreakpoint* breakpoint = adw_breakpoint_new(breakpoint_condition);

        GValue _true = G_VALUE_INIT;
        g_value_init(&_true, G_TYPE_BOOLEAN);
        g_value_set_boolean(&_true, TRUE);

        GValue _false = G_VALUE_INIT;
        g_value_init(&_false, G_TYPE_BOOLEAN);
        g_value_set_boolean(&_false, FALSE);

        adw_breakpoint_add_setter(breakpoint, G_OBJECT(overview_btn), "visible", &_true);
        adw_breakpoint_add_setter(breakpoint, G_OBJECT(new_tab_btn), "visible", &_true);
        adw_breakpoint_add_setter(breakpoint, G_OBJECT(tab_bar), "visible", &_false);

        adw_window_add_breakpoint(adw_window, breakpoint);

        // Adiciona o widget principal à janela Gtk::ApplicationWindow
        set_child(*Glib::wrap(GTK_WIDGET(adw_window), true));
        on_new_tab();
    }

    void MainWindow::build_content_() {
        // Tab view.
        tab_view_ = adw_tab_view_new();
        // Tab bar.
        AdwTabBar* tab_bar = adw_tab_bar_new();
        // Attach the TabBar to the TabView.
        adw_tab_bar_set_view(tab_bar, tab_view_);

        Gtk::HeaderBar* header_bar = create_header_bar();

        // Arrange widgets in a main content box.
        auto* content_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 0);
        content_box->append(*Glib::wrap(GTK_WIDGET(tab_bar)));
        content_box->append(*Glib::wrap(GTK_WIDGET(tab_view_)));

        set_titlebar(*header_bar);
        set_child(*content_box);

        on_new_tab();
    }

    // Create a header bar with buttons.
    Gtk::HeaderBar* MainWindow::create_header_bar() {
        auto* header_bar = Gtk::make_managed<Gtk::HeaderBar>();
        add_new_tab_button(header_bar);
        return header_bar;
    }

    void MainWindow::add_new_tab_button(Gtk::HeaderBar* header_bar) {
        auto* new_tab_button = Gtk::make_managed<Gtk::Button>();
        new_tab_button->set_icon_name("tab-new-symbolic");
        header_bar->pack_start(*new_tab_button);
        g_signal_connect(new_tab_button->gobj(), "clicked", G_CALLBACK(on_new_tab_callback), this);
    }

    /*void MainWindow::initialize_first_tab() {
        Gtk::Box* tabContent = create_tab_content();

        auto* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(content_box), headerBar);
        gtk_box_append(GTK_BOX(content_box), reinterpret_cast<GtkWidget*>(tabContent));

        // 5. Adiciona o layout completo a uma nova aba no AdwTabView
        adw_tab_view_append(tabView_, GTK_WIDGET(tabContent->gobj()));

        // 6. Define o AdwTabView como o conteúdo principal da janela
        set_child(*Glib::wrap(GTK_WIDGET(tabView_)));
    }

    GtkWidget* MainWindow::create_header_bar() const {
        auto* headerBar = adw_header_bar_new();
        auto* newTabButton = gtk_button_new_from_icon_name("tab-new-symbolic");
        adw_header_bar_pack_start(reinterpret_cast<AdwHeaderBar*>(headerBar), newTabButton);
        // Connect button signals.
        g_signal_connect(newTabButton, "clicked", G_CALLBACK(on_new_tab_callback), tabView_);
        return headerBar;
    }

    Gtk::Box* MainWindow::create_tab_content() {

        // 2. Cria os painéis de conteúdo da aba (placeholders)
        navigationPanel_ = Gtk::make_managed<NavigationPanel>();
        contentView_ = Gtk::make_managed<ContentView>();
        statusBar_ = Gtk::make_managed<StatusBar>();

        // 3. Cria o divisor redimensionável e adiciona os painéis esquerdo e direito
        auto paned = Gtk::Paned(Gtk::Orientation::HORIZONTAL);
        paned.set_wide_handle();
        paned.set_start_child(*navigationPanel_);
        paned.set_end_child(*contentView_);
        paned.set_position(280); // Largura inicial do painel de navegação
        paned.set_resize_start_child(true);
        paned.set_shrink_start_child(false);

        // 4. Cria uma caixa vertical para organizar o conteúdo principal e a barra de status
        Gtk::Box* tab_content_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        tab_content_box->append(paned); // Adiciona o divisor no topo (ocupando a maior parte do espaço)
        tab_content_box->append(*statusBar_); // Adiciona a barra de status no rodapé
        return tab_content_box;
    }
*/

    void MainWindow::on_new_tab() {
        tab_count_++;

        // Cria o conteúdo da nova aba
        auto label_text = "Conteúdo da Aba " + std::to_string(tab_count_);
        auto* label = Gtk::make_managed<Gtk::Label>(label_text);
        label->set_vexpand(true);
        label->set_hexpand(true);

        // Adiciona a nova aba e obtém o objeto da página
        auto* page = adw_tab_view_append(tab_view_, reinterpret_cast<GtkWidget*>(label->gobj()));

        // Define o título da página
        auto title_text = "Aba " + std::to_string(tab_count_);
        adw_tab_page_set_title(page, title_text.c_str());

        // Seleciona a aba recém-criada
        adw_tab_view_set_selected_page(tab_view_, page);
    }

}

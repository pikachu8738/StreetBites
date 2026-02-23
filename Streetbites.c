#include <gtk/gtk.h>
#include <string.h>
#include <curl/curl.h>

/* ===== GOOGLE GEMINI API KEY ===== */
#define GEMINI_API_KEY "Your API Key"


/* ---------- GLOBAL ---------- */
GtkWidget *stack;
GtkWidget *meme_label;
GtkWidget *ai_spinner;


/* ---------- NAV ---------- */
void go_screen(const char *name){
    gtk_stack_set_visible_child_name(GTK_STACK(stack), name);
}

/* ---------- POPUP ---------- */
void popup(GtkWindow *w,const char *msg){
    GtkAlertDialog *d = gtk_alert_dialog_new("%s", msg);
    gtk_alert_dialog_show(d, w);
}

/* ---------- MEME ROTATION ---------- */
gboolean rotate_meme(gpointer d){
    static int i=0;
    const char *memes[]={
        "Mood: Pani Puri > Problems 😭🔥",
        "Diet kal se pakka 🤡",
        "Street food is my love language 💘",
        "Aaj cheat day, kal bhi 😌"
    };
    gtk_label_set_text(GTK_LABEL(meme_label), memes[i%4]);
    i++;
    return TRUE;
}
/* ===== CURL RESPONSE BUFFER ===== */

struct Memory {
    char *data;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    return realsize;
}


/* ---------- CHAT ---------- */
GtkWidget* chat_screen(){
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,8);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroll, TRUE);

    GtkWidget *chat = gtk_label_new(
        "👤 Amit: 7 baje milte 😄\n"
        "👤 Sneha: Blue cap pehni hu 💙\n"
        "👤 Rahul: Pani puri first 😭🔥\n\n"
        "⚠️ Group ends at midnight");

    gtk_label_set_wrap(GTK_LABEL(chat), TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), chat);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry),"Type message...");

    GtkWidget *back = gtk_button_new_with_label("⬅ Back");
    g_signal_connect_swapped(back,"clicked",G_CALLBACK(go_screen),"user");

    gtk_box_append(GTK_BOX(box), scroll);
    gtk_box_append(GTK_BOX(box), entry);
    gtk_box_append(GTK_BOX(box), back);
    return box;
}

/* ---------- STALL CARD ---------- */
GtkWidget* stall_card(GtkWindow *w,const char *name,const char *price){
    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL,6);
    gtk_widget_add_css_class(card,"card");

    gtk_box_append(GTK_BOX(card), gtk_label_new(name));
    GtkWidget *map = gtk_image_new_from_file("map.png");
gtk_widget_set_size_request(map, -1, 120);
gtk_box_append(GTK_BOX(card), map);

    gtk_box_append(GTK_BOX(card), gtk_label_new(price));

    GtkWidget *join = gtk_button_new_with_label("Join Group Chat 💬");
    g_signal_connect_swapped(join,"clicked",G_CALLBACK(go_screen),"chat");

    gtk_box_append(GTK_BOX(card), join);
    return card;
}

/* ---------- GROCERY ---------- */
GtkWidget* grocery_screen(){
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroll, TRUE);

    GtkWidget *list = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), list);

    gtk_box_append(GTK_BOX(list), gtk_label_new("🍅 Tomatoes ₹40/kg\n\n"));
    gtk_box_append(GTK_BOX(list), gtk_label_new("🥔 Potatoes ₹30/kg\n\n"));
    gtk_box_append(GTK_BOX(list), gtk_label_new("🥭 Mangoes ₹80/kg\n\n"));

    GtkWidget *back = gtk_button_new_with_label("⬅ Back");
    g_signal_connect_swapped(back,"clicked",G_CALLBACK(go_screen),"user");

    gtk_box_append(GTK_BOX(box), gtk_label_new("Local Grocery"));
    gtk_box_append(GTK_BOX(box), scroll);
    gtk_box_append(GTK_BOX(box), back);
    return box;
}

/* ---------- USER ---------- */
GtkWidget* user_screen(GtkWindow *w){
    GtkWidget *main = gtk_box_new(GTK_ORIENTATION_VERTICAL,8);

    meme_label = gtk_label_new("Mood: Pani Puri > Problems 😭🔥");
gtk_widget_set_margin_top(meme_label, 10);
gtk_widget_set_margin_bottom(meme_label, 10);
gtk_widget_add_css_class(meme_label, "meme");

gtk_box_append(GTK_BOX(main), meme_label);
g_timeout_add(2500, rotate_meme, NULL);


    GtkWidget *tabs = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,6);
    GtkWidget *t1 = gtk_button_new_with_label("Street Food");
    GtkWidget *t2 = gtk_button_new_with_label("Groceries");
    GtkWidget *t3 = gtk_button_new_with_label("Trending");

    g_signal_connect_swapped(t2,"clicked",G_CALLBACK(go_screen),"grocery");

    gtk_box_append(GTK_BOX(tabs), t1);
    gtk_box_append(GTK_BOX(tabs), t2);
    gtk_box_append(GTK_BOX(tabs), t3);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroll, TRUE);

    GtkWidget *list = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), list);

    gtk_box_append(GTK_BOX(list), stall_card(w,"🔥 Rajesh Chaat - MG Road","Pani Puri ₹30"));
    gtk_box_append(GTK_BOX(list), stall_card(w,"🧀 Cheese Dosa Hub","Dosa ₹50"));
    gtk_box_append(GTK_BOX(list), stall_card(w,"🥟 Amit Momos","Momos ₹40"));

    GtkWidget *back = gtk_button_new_with_label("⬅ Back");
    g_signal_connect_swapped(back,"clicked",G_CALLBACK(go_screen),"role");

    gtk_box_append(GTK_BOX(main), gtk_label_new("Explore Nearby"));
    gtk_box_append(GTK_BOX(main), tabs);
    gtk_box_append(GTK_BOX(main), scroll);
    gtk_box_append(GTK_BOX(main), back);
    return main;
}
char* extract_gemini_text(const char *json) {
    const char *p = strstr(json, "\"text\":");
    if (!p) return g_strdup("No suggestion received.");

    p = strchr(p, ':');
    if (!p) return g_strdup("No suggestion received.");
    p += 2; // skip :"

    const char *end = strchr(p, '"');
    if (!end) return g_strdup("No suggestion received.");

    return g_strndup(p, end - p);
}


/* ---------- VENDOR ---------- */
void call_gemini(GtkButton *b, gpointer win) {

    gtk_spinner_start(GTK_SPINNER(ai_spinner));
   
    CURL *curl = curl_easy_init();
    if (!curl) return;

    struct Memory chunk;
    chunk.data = malloc(1);
    chunk.size = 0;

    const char *url =
        "https://generativelanguage.googleapis.com/v1/models/"
        "gemini-1.5-flash:generateContent?key=" GEMINI_API_KEY;

    const char *json =
        "{"
        " \"contents\": ["
        "   {"
        "     \"role\": \"user\","
        "     \"parts\": ["
        "       { \"text\": \"Suggest best stall location and time today for street food vendor in Delhi.\" }"
        "     ]"
        "   }"
        " ]"
        "}";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    CURLcode res = curl_easy_perform(curl);

    gtk_spinner_stop(GTK_SPINNER(ai_spinner));

    if (res != CURLE_OK) {
        popup(GTK_WINDOW(win), "Gemini API Error ❌");
    } else {
        char *msg = extract_gemini_text(chunk.data);
        popup(GTK_WINDOW(win), msg);
        g_free(msg);
    }

    curl_easy_cleanup(curl);
    free(chunk.data);
}

void reminder_popup(GtkButton *b, gpointer win) {
    popup(GTK_WINDOW(win), "🔔 Please update today's stall location 📍");
}

GtkWidget* vendor_screen(GtkWindow *win){
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,8);

    GtkWidget *stall = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(stall),"Stall Name");

    GtkWidget *menu = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(menu),"Today's Menu");

    GtkWidget *open = gtk_check_button_new_with_label("Open Today");

    GtkWidget *ai = gtk_button_new_with_label("🤖 AI Location Suggestion");
    ai_spinner = gtk_spinner_new();
    gtk_spinner_stop(GTK_SPINNER(ai_spinner));

    GtkWidget *rem = gtk_button_new_with_label("🔔 Reminder Test");

    g_signal_connect(ai, "clicked", G_CALLBACK(call_gemini), win);
    g_signal_connect(rem, "clicked", G_CALLBACK(reminder_popup), win);

    GtkWidget *back = gtk_button_new_with_label("⬅ Back");
    g_signal_connect_swapped(back,"clicked",G_CALLBACK(go_screen),"role");

    gtk_box_append(GTK_BOX(box), gtk_label_new("Vendor Dashboard"));
    gtk_box_append(GTK_BOX(box), stall);
    gtk_box_append(GTK_BOX(box), menu);
    gtk_box_append(GTK_BOX(box), open);
    gtk_box_append(GTK_BOX(box), ai);
    gtk_box_append(GTK_BOX(box), ai_spinner);
    gtk_box_append(GTK_BOX(box), rem);
    gtk_box_append(GTK_BOX(box), back);

    return box;
}


/* ---------- ROLE ---------- */
GtkWidget* role_screen(){
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

    GtkWidget *title = gtk_label_new("🍴StreetBite🍴");
    gtk_widget_add_css_class(title,"title");

    GtkWidget *name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name),"Your Name");

    GtkWidget *u = gtk_button_new_with_label("I am User");
    GtkWidget *v = gtk_button_new_with_label("I am Vendor");

    g_signal_connect_swapped(u,"clicked",G_CALLBACK(go_screen),"user");
    g_signal_connect_swapped(v,"clicked",G_CALLBACK(go_screen),"vendor");

    gtk_box_append(GTK_BOX(box), title);
    gtk_box_append(GTK_BOX(box), name);
    gtk_box_append(GTK_BOX(box), u);
    gtk_box_append(GTK_BOX(box), v);
    return box;
}

/* ---------- CSS ---------- */
void load_css(){
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css,
        "window{background:#e8f7ff;}"
        "button{background:#7ddfc3;color:#003;padding:10px;border-radius:14px;}"
        ".card{background:white;padding:12px;border-radius:14px;}"
        "entry{border-radius:12px;padding:8px;}"
        ".title{font-size:26px;color:#2a5fff;font-weight:bold;}"
    );
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

/* ---------- APP ---------- */
static void activate(GtkApplication *app,gpointer d){
    load_css();

    GtkWidget *win = gtk_application_window_new(app);
    
    gtk_window_set_default_size(GTK_WINDOW(win),420,700);
    gtk_window_set_title(GTK_WINDOW(win),"StreetBite");
    GtkCssProvider *provider = gtk_css_provider_new();

gtk_css_provider_load_from_string(provider,
    ".meme {"
    "  font-size: 16px;"
    "  color: #ff4d6d;"
    "  font-weight: bold;"
    "}"
    ".title {"
    "  font-size: 28px;"
    "  font-weight: bold;"
    "  color: #4dabf7;"
    "}"
);

gtk_style_context_add_provider_for_display(
    gdk_display_get_default(),
    GTK_STYLE_PROVIDER(provider),
    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
);


    stack = gtk_stack_new();

    gtk_stack_add_named(GTK_STACK(stack), role_screen(),"role");
    gtk_stack_add_named(GTK_STACK(stack), vendor_screen(GTK_WINDOW(win)), "vendor");
    gtk_stack_add_named(GTK_STACK(stack), user_screen(GTK_WINDOW(win)),"user");
    gtk_stack_add_named(GTK_STACK(stack), chat_screen(),"chat");
    gtk_stack_add_named(GTK_STACK(stack), grocery_screen(),"grocery");

    gtk_stack_set_visible_child_name(GTK_STACK(stack),"role");

    gtk_window_set_child(GTK_WINDOW(win), stack);
    gtk_window_present(GTK_WINDOW(win));
}

int main(int argc,char **argv){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    GtkApplication *app = gtk_application_new("com.streetbite.app",G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
    int status = g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);
    curl_global_cleanup();
    return status;
}
#include <gtk-3.0/gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sqlite3.h"

GtkWidget *window, *window2, *window3, *window4, *addbutton,
    *button1, *button2, *buttonok, *btupdate,
    *fixed1, *fixed2, *fixed3, *fixed4, *label1, *notification,
    *birthdate, *phonenum, *full, *closebt, *notification1, *scrollwindow1;
GtkBuilder *builder, *builder2, *builder3, *builder4;
GtkEntry *entry1, *entryname, *entryfullname, *entryphone,
    *entrydate, *entryname1, *entryfullname1, *entryphone1, *entrydate1;
GtkListStore *store;
GtkTreeIter iter;
GtkTreeView *list1;
GtkTreeSelection *selection;

sqlite3 *db;
char *err_msg = 0;
sqlite3_stmt *res;
int rc, cur, rowNbr;
typedef struct
{
    gchar *phone;
    gchar *name;
    gchar *fullname;
    gchar *date;

} Contact;
enum
{
    COLUMN_PHONE,
    COLUMN_NAME,
    NUM_COLUMNS
};
Contact data[1555];
Contact selectData;

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    NotUsed = 0;
    data[cur].phone = (char *)malloc(strlen(argv[0]) * sizeof(char));
    strcpy(data[cur].phone, argv[0]);
    data[cur].fullname = (char *)malloc(strlen(argv[1]) * sizeof(char) + 5);
    strcpy(data[cur].fullname, argv[1]);
    data[cur].name = (char *)malloc(strlen(argv[2]) * sizeof(char));
    strcpy(data[cur].name, argv[2]);
    data[cur].date = (char *)malloc(strlen(argv[3]) * sizeof(char));
    strcpy(data[cur].date, argv[3]);
    ++cur;
    return 0;
}
int callback2(void *NotUsed, int argc, char **argv, char **azColName)
{
    NotUsed = 0;
    selectData.phone = (char *)malloc(strlen(argv[0]) * sizeof(char));
    strcpy(selectData.phone, argv[0]);
    selectData.fullname = (char *)malloc(strlen(argv[1]) * sizeof(char) + 5);
    strcpy(selectData.fullname, argv[1]);
    selectData.name = (char *)malloc(strlen(argv[2]) * sizeof(char));
    strcpy(selectData.name, argv[2]);
    selectData.date = (char *)malloc(strlen(argv[3]) * sizeof(char));
    strcpy(selectData.date, argv[3]);
    return 0;
}
void selectall()
{
    char *sql3 = "SELECT * FROM Phones ORDER BY Name";
    rc = sqlite3_exec(db, sql3, callback, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
}
int select_db(const char *phone)
{
    char sql1[1222];
    sprintf(sql1, "SELECT * FROM Phones WHERE Phone = '%s'", phone);
    rc = sqlite3_exec(db, sql1, callback2, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 1;
}
int insert(const char *phone, const char *fullname,
           const char *name, const char *date) //insert into db
{

    char sql1[1222];
    sprintf(sql1, "INSERT INTO Phones VALUES('%s','%s','%s','%s');", phone, fullname, name, date);
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(err_msg);
        return -1;
    }
    return 1;
}
int update(const char *phone, const char *fullname,
           const char *name, const char *date,
           const char *ophone) //update db
{
    char sql1[1222];
    sprintf(sql1, "UPDATE Phones SET Phone='%s', FullName = '%s', Name='%s',Birthdate='%s' WHERE Phone = '%s' ;", phone, fullname, name, date, ophone);
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(err_msg);
        return -1;
    }
    return 1;
}
int delete_phone(const char *phone) //delete from db
{
    char sql1[1222];
    sprintf(sql1, "DELETE FROM Phones WHERE Phone='%s';", phone);
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error:%s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 1;
}

void on_destroy()
{
    gtk_main_quit();
}
GtkTreeModel *create_model()
{
    gint i = 0;
    store = gtk_list_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
    for (i = 0; i < cur; ++i)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COLUMN_PHONE, data[i].phone,
                           COLUMN_NAME, data[i].name,
                           -1);
    }
    return GTK_TREE_MODEL(store);
}
void add_book_columns(GtkTreeView *treeview)
{
    GtkTreeViewColumn *column1, *column2;
    GtkCellRenderer *renderer;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    renderer = gtk_cell_renderer_text_new();
    column1 = gtk_tree_view_column_new_with_attributes("Phone number", renderer, "text",
                                                       COLUMN_PHONE, NULL);
    column2 = gtk_tree_view_column_new_with_attributes("Name", renderer, "text",
                                                       COLUMN_NAME, NULL);
    gtk_tree_view_append_column(treeview, column1);
    gtk_tree_view_append_column(treeview, column2);
}
int findRow1(GtkTreeModel *model, GtkTreeIter iter, const gchar *srch)
{
    const gchar *text;
    GtkTreePath *path;
    GtkTreeIter iterChild;

    while (1)
    {
        gtk_tree_model_get(model, &iter, 1, &text, -1); //get pointer to column 1 name
        rowNbr++;
        if (strncasecmp(text, srch, strlen(srch)) == 0)
        {
            path = gtk_tree_model_get_path(model, &iter);
            gtk_tree_view_expand_all(list1);
            gtk_tree_view_scroll_to_cell(list1, path, NULL, TRUE, 0.5, 0.0);
            gtk_tree_view_set_cursor_on_cell(list1, path, NULL, NULL, FALSE);
            return 1;
        }
        if (gtk_tree_model_iter_has_child(model, &iter))
        {
            gtk_tree_model_iter_children(model, &iterChild, &iter);
            if (findRow1(model, iterChild, srch))
                return 1;
        }
        if (!gtk_tree_model_iter_next(model, &iter))
        {
            return 0;
        }
    }
}
int findRow2(GtkTreeModel *model, GtkTreeIter iter, const gchar *srch)
{
    const gchar *text;
    GtkTreePath *path;
    GtkTreeIter iterChild;

    while (1)
    {
        gtk_tree_model_get(model, &iter, 0, &text, -1); //get pointer to column  phone
        rowNbr++;
        if (strncasecmp(text, srch, strlen(srch)) == 0)
        {
            path = gtk_tree_model_get_path(model, &iter);
            gtk_tree_view_expand_all(list1);
            gtk_tree_view_scroll_to_cell(list1, path, NULL, TRUE, 0.5, 0.0);
            gtk_tree_view_set_cursor_on_cell(list1, path, NULL, NULL, FALSE);
            return 1;
        }
        if (gtk_tree_model_iter_has_child(model, &iter))
        {
            gtk_tree_model_iter_children(model, &iterChild, &iter);
            if (findRow2(model, iterChild, srch))
                return 1;
        }
        if (!gtk_tree_model_iter_next(model, &iter))
        {
            return 0;
        }
    }
}
void on_entry1_search_changed(GtkSearchEntry *s) //find
{
    const gchar *srch;
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(list1);
    srch = gtk_entry_get_text(GTK_ENTRY(s));
    if (strlen(srch) == 0)
    {
        return;
    }
    gtk_tree_model_get_iter_first(model, &iter);
    rowNbr = 0;
    findRow1(model, iter, srch);
    findRow2(model, iter, srch);
}
void insert_text_handler(GtkEntry *entry, const gchar *text, gint length, gint *position, gpointer data)
{
    GtkEditable *editable = GTK_EDITABLE(entry);
    int i, count = 0;
    gchar *res = g_new(gchar, length);
    for (i = 0; i < length; i++)
    {
        if (!isdigit(text[i]))
            continue;
        res[count++] = text[i];
    }
    if (count > 0)
    {
        g_signal_handlers_block_by_func(G_OBJECT(editable), G_CALLBACK(insert_text_handler), data);
        gtk_editable_insert_text(editable, res, count, position);
        g_signal_handlers_unblock_by_func(G_OBJECT(editable), G_CALLBACK(insert_text_handler), data);
    }
    g_signal_stop_emission_by_name(G_OBJECT(editable), "insert_text");
    g_free(res);
}
void first_dialog(const char *phone)
{
    GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "Phone number already exists!");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(message),
                                             "You might want to update data for phone number %s\n Please try again!", phone);
    gtk_window_set_transient_for(GTK_WINDOW(message), GTK_WINDOW(window));
    int response = gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
}
void second_dialog(const char *phone)
{
    GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "Update failed!");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(message),
                                             "Phone number %s already exists in the list", phone);
    gtk_window_set_transient_for(GTK_WINDOW(message), GTK_WINDOW(window));
    int response = gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
}
void third_dialog()
{
    GtkWidget *message = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "Adding new contact failed");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(message),
                                             "Field Name or Phone must be filled in!");
    gtk_window_set_transient_for(GTK_WINDOW(message), GTK_WINDOW(window));
    int response = gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
}

void on_addbutton_clicked(GtkButton *b) //show window2 insert
{
    builder2 = gtk_builder_new_from_file("My-contact.glade");
    window2 = GTK_WIDGET(gtk_builder_get_object(builder2, "window2"));
    g_signal_connect(window2, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_builder_connect_signals(builder2, NULL);

    fixed2 = GTK_WIDGET(gtk_builder_get_object(builder2, "fixed2"));
    entryphone = GTK_ENTRY(gtk_builder_get_object(builder2, "entryphone"));
    entryname = GTK_ENTRY(gtk_builder_get_object(builder2, "entryname"));
    entryfullname = GTK_ENTRY(gtk_builder_get_object(builder2, "entryfullname"));
    entrydate = GTK_ENTRY(gtk_builder_get_object(builder2, "entrydate"));
    notification = GTK_WIDGET(gtk_builder_get_object(builder2, "notification"));
    buttonok = GTK_WIDGET(gtk_builder_get_object(builder2, "buttonok"));
    gtk_window_set_title(GTK_WINDOW(window2), "Add Contact");
    gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER_ALWAYS);
    g_signal_connect(G_OBJECT(entryphone), "insert_text", G_CALLBACK(insert_text_handler), NULL);

    gtk_widget_show(window2);
    gtk_main();
}
void on_buttonok_clicked(GtkButton *b) //insert close window 2
{
    const gchar *phone, *name, *fullname, *date;
    name = gtk_entry_get_text(entryname);
    fullname = gtk_entry_get_text(entryfullname);
    phone = gtk_entry_get_text(entryphone);
    date = gtk_entry_get_text(entrydate);
    if (strlen(phone) == 0 || strlen(name) == 0)
    {
        gtk_label_set_text(GTK_LABEL(notification), "Field Name and Phone number must be filled in!");
    }
    else
    {
        int a = insert(phone, fullname, name, date);
        if (a == -1)
        {
            first_dialog(phone);
            gtk_window_close(GTK_WINDOW(window2));
            return;
        }
        else
        {
            GtkTreeIter iter;
            GtkTreeModel *model = gtk_tree_view_get_model(list1);
            int a = 0;
            const gchar *text;
            gtk_tree_model_get_iter_first(model, &iter);
            while (1)
            {
                gtk_tree_model_get(model, &iter, 1, &text, -1);
                if (strcasecmp(text, name) > 0)
                {
                    gtk_list_store_insert(store, &iter, (gint)a);
                    gtk_list_store_set(store, &iter, COLUMN_PHONE, phone, COLUMN_NAME, name, -1);
                    break;
                }
                a++;
                if (!gtk_tree_model_iter_next(model, &iter))
                {
                    break;
                }
            }
            findRow2(model, iter, phone);
        }
        gtk_window_close(GTK_WINDOW(window2));
    }
}
void on_button1_clicked(GtkButton *b) //update clicked show window4
{
    builder4 = gtk_builder_new_from_file("My-contact.glade");
    window4 = GTK_WIDGET(gtk_builder_get_object(builder4, "window4"));
    g_signal_connect(window4, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_builder_connect_signals(builder4, NULL);
    fixed4 = GTK_WIDGET(gtk_builder_get_object(builder4, "fixed4"));
    entryphone1 = GTK_ENTRY(gtk_builder_get_object(builder4, "entryphone1"));
    entryname1 = GTK_ENTRY(gtk_builder_get_object(builder4, "entryname1"));
    entryfullname1 = GTK_ENTRY(gtk_builder_get_object(builder4, "entryfullname1"));
    entrydate1 = GTK_ENTRY(gtk_builder_get_object(builder4, "entrydate1"));
    notification1 = GTK_WIDGET(gtk_builder_get_object(builder4, "notification1"));
    btupdate = GTK_WIDGET(gtk_builder_get_object(builder4, "btupdate"));
    g_signal_connect(G_OBJECT(entryphone1), "insert_text", G_CALLBACK(insert_text_handler), NULL);
    gtk_window_set_title(GTK_WINDOW(window4), "Update Info");
    gtk_window_set_position(GTK_WINDOW(window4), GTK_WIN_POS_CENTER_ALWAYS);

    const gchar *ophone;
    GtkTreeModel *model = gtk_tree_view_get_model(list1);
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter) == FALSE)
    {
        return;
    }
    gtk_tree_model_get(model, &iter, 0, &ophone, -1);

    if (select_db(ophone))
    {
        gtk_entry_set_text(entryname1, (const gchar *)selectData.name);
        gtk_entry_set_text(entryphone1, (const gchar *)selectData.phone);
        gtk_entry_set_text(entryfullname1, (const gchar *)selectData.fullname);
        gtk_entry_set_text(entrydate1, (const gchar *)selectData.date);
        gtk_widget_show(window4);
        gtk_main();
    }
}
void on_update_clicked() //click update button close window4
{

    const gchar *ophone, *srch, *name, *fullname, *date, *phone;
    GtkTreeModel *model = gtk_tree_view_get_model(list1);
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter) == FALSE)
    {
        return;
    }
    gtk_tree_model_get(model, &iter, 0, &ophone, -1);
    name = gtk_entry_get_text(GTK_ENTRY(entryname1));
    fullname = gtk_entry_get_text(GTK_ENTRY(entryfullname1));
    phone = gtk_entry_get_text(GTK_ENTRY(entryphone1));
    date = gtk_entry_get_text(GTK_ENTRY(entrydate1));
    if (strlen(phone) == 0 || strlen(name) == 0)
    {
        gtk_label_set_text(GTK_LABEL(notification1), (const gchar *)"Field Name and Phone number must be filled in!");
    }
    else
    {
        int a = update(phone, fullname, name, date, ophone);
        if (a == -1)
        {
            second_dialog(phone);
            gtk_window_close(GTK_WINDOW(window4));
            return;
        }
        else
        {
            gtk_list_store_set(store, &iter, COLUMN_PHONE, phone, COLUMN_NAME, name, -1);
            findRow2(model, iter, phone);
        }
    }
    gtk_window_close(GTK_WINDOW(window4));
}
void on_button2_clicked(GtkButton *b) //delete clicked
{
    gchar *phone;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter) == FALSE)
    {
        return;
    }
    gtk_tree_model_get(model, &iter, 0, &phone, -1);
    delete_phone(phone);
    gtk_list_store_remove(store, &iter);
    g_free(phone);
}
void on_list1_row_activated() //show window 3
{
    builder3 = gtk_builder_new_from_file("My-contact.glade");
    window3 = GTK_WIDGET(gtk_builder_get_object(builder3, "window3"));
    g_signal_connect(window3, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_builder_connect_signals(builder3, NULL);
    fixed3 = GTK_WIDGET(gtk_builder_get_object(builder3, "fixed3"));
    birthdate = GTK_WIDGET(gtk_builder_get_object(builder3, "birthdate"));
    full = GTK_WIDGET(gtk_builder_get_object(builder3, "full"));
    phonenum = GTK_WIDGET(gtk_builder_get_object(builder3, "phonenum"));
    closebt = GTK_WIDGET(gtk_builder_get_object(builder3, "closebt"));

    gtk_window_set_position(GTK_WINDOW(window3), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title(GTK_WINDOW(window3), "Detailed Info");

    GtkTreeModel *model = gtk_tree_view_get_model(list1);
    GtkTreeIter iter;
    gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter);
    gchar *phone;
    gtk_tree_model_get(model, &iter, 0, &phone, -1);
    if (select_db(phone))
    {
        gtk_label_set_text(GTK_LABEL(phonenum), (const gchar *)selectData.phone);
        gtk_label_set_text(GTK_LABEL(full), (const gchar *)selectData.fullname);
        gtk_label_set_text(GTK_LABEL(birthdate), (const gchar *)selectData.date);
        gtk_widget_show(window3);
        gtk_main();
    }
    g_free(phone);
}
void on_close_clicked()
{
    gtk_window_close(GTK_WINDOW(window3));
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        g_print("Usage: ./contact My-contact.glade\n");
    }
    rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    selectall();

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("My-contact.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_builder_connect_signals(builder, NULL);

    list1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "list1"));
    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    entry1 = GTK_ENTRY(gtk_builder_get_object(builder, "entry1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    addbutton = GTK_WIDGET(gtk_builder_get_object(builder, "addbutton"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    button2 = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
    scrollwindow1 = GTK_WIDGET(gtk_builder_get_object(builder, "scrollwindow1"));
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list1));

    gtk_tree_view_set_model(GTK_TREE_VIEW(list1), GTK_TREE_MODEL(create_model()));
    add_book_columns(GTK_TREE_VIEW(list1));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title(GTK_WINDOW(window), "Phone Book");
    gtk_widget_show(window);
    gtk_main();
    sqlite3_close(db);
    return 0;
}

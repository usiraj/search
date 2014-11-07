//============================================================================
// Name        : MapRoutePlanner.cpp
// Author      : Usama Siraj
// Version     :
// Copyright   : AIResearch
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <gtk/gtk.h>
#include <pthread.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>
#include "usjMapProblem.h"
#include "usjAILSearches.h"
//FUNCTION DECLARATIONS
void setUpGUI();
void buttonHandler(GtkWidget *widget,GdkEvent *event,gpointer data);
typedef void(*DrawingFunc)(cairo_t*,gint,gint);
void drawMap(cairo_t*,gint,gint);
static void loadMap(GtkWidget *w,GtkFileSelection *fs);
gboolean drawingStandard(GtkWidget *widget,GdkEvent *event,gpointer data);
void paintName(cairo_t*,gint x,gint y,std::string city,double r,double g,double b,double,double,double);
// FILE Scope Variables
GtkWidget* mainWindow;
GtkWidget* dArea;
GtkWidget *searchMethod,*search,*iState,*gState;
usjAIL::MapProblem *problem;
usjAIL::Stack *solution;
//FUNCTION DEFINATIONS
int main(int argc,char **argv) {
	usjAIL::Search searcher;
	gtk_init(&argc,&argv);
	setUpGUI();
	return 0;
}
void setUpGUI(){
	//local variables
	GtkWidget *table = gtk_table_new(6,6,FALSE);
	std::vector<GtkWidget*> items;
	GtkWidget *vbox = gtk_vbox_new(TRUE,3);
	dArea = gtk_drawing_area_new();
	//start of function
	mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainWindow),"Map Routing Problem");
	gtk_container_set_border_width(GTK_CONTAINER(mainWindow),5);
	// add event listeners
	g_signal_connect(mainWindow,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(mainWindow,"delete-event",G_CALLBACK(gtk_main_quit),NULL);
	// add window children
	items.push_back( gtk_button_new_with_label("Load Map") );
	items.back()->name = (gchar*)"Load Map";
	g_signal_connect(items.back(),"clicked",G_CALLBACK(buttonHandler),NULL);
	items.push_back( gtk_label_new("Initial State"));
	items.back()->name = (gchar*)"IState";
	iState = gtk_combo_new();
	gtk_widget_set_sensitive(iState,false);
	items.push_back(iState);
	items.back()->name = (gchar*)"Initial State";
	items.push_back( gtk_label_new("Goal State"));
	items.back()->name = (gchar*)"GState";
	gState = gtk_combo_new();
	gtk_widget_set_sensitive(gState,false);
	items.push_back( gState);
	items.back()->name = (gchar*)"Goal State";
	search = gtk_button_new_with_label("Search");
	gtk_widget_set_sensitive(search,false);
	items.push_back( search );
	g_signal_connect(items.back(),"clicked",G_CALLBACK(buttonHandler),NULL);
	items.back()->name = (gchar*)"Search";
	items.push_back( gtk_label_new("Search Method"));
	items.back()->name = (gchar*)"smethod";
	searchMethod = gtk_combo_new();
	gtk_widget_set_sensitive(searchMethod,false);
	GList* smlist = 0;
	smlist = g_list_append(smlist,(gpointer)"Breadth First Search");
	smlist = g_list_append(smlist,(gpointer)"Uniform Cost Search");
	smlist = g_list_append(smlist,(gpointer)"Depth First Search");
	smlist = g_list_append(smlist,(gpointer)"Iterative Deepening Search");
	smlist = g_list_append(smlist,(gpointer)"Greedy Search");
	smlist = g_list_append(smlist,(gpointer)"A* Search");
	gtk_combo_set_popdown_strings(GTK_COMBO(searchMethod),smlist);
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(searchMethod)->entry),"A* Search");
	items.push_back( searchMethod );
	items.back()->name = (gchar*)"Search Method";
	items.push_back( gtk_button_new_with_label("Quit") );
	items.back()->name = (gchar*)"Quit";
	g_signal_connect(items.back(),"clicked",G_CALLBACK(buttonHandler),NULL);
	// adding signals
	for ( unsigned int i = 0; i < items.size() ; ++i){
		gtk_widget_show(items[i]);
		gtk_box_pack_start(GTK_BOX(vbox),items[i],TRUE,TRUE,3);
	}
	// adding signal for drawing
	g_signal_connect(dArea,"expose-event",G_CALLBACK(drawingStandard),(gpointer)drawMap);
	gtk_widget_show(vbox);
	gtk_table_attach_defaults(GTK_TABLE(table),vbox,5,6,0,6);
	// adding drawing board
	items.push_back( dArea );
	gtk_widget_set_size_request(items.back(),640,480);
	gtk_widget_show(items.back());
	gtk_table_attach_defaults(GTK_TABLE(table),items.back(),0,5,0,5);
	// setting table
	gtk_table_set_row_spacings(GTK_TABLE(table),3);
	gtk_table_set_col_spacings(GTK_TABLE(table),3);
	gtk_container_add(GTK_CONTAINER(mainWindow),table);
	gtk_widget_show(table);
	// show window
	gtk_widget_show(mainWindow);
	return gtk_main();
}
void buttonHandler(GtkWidget *widget,GdkEvent *event,gpointer data){
	if ( usjAIL::Data::compareSTLStrings(widget->name,"Quit")){
		gtk_main_quit();
	} else if( usjAIL::Data::compareSTLStrings(widget->name,"Search")){
		// do search
		usjAIL::Search SEARCH;
		std::string is,fs,sm;
		is = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(iState)->entry));
		fs = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(gState)->entry));
		sm = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(searchMethod)->entry));
		if ( solution != 0) delete solution;
		solution = new usjAIL::Stack;
		if ( problem != 0){
			problem->defineInitialState(problem->getStateWithName(is));
			problem->defineGoalState(problem->getStateWithName(fs));
			if ( usjAIL::Data::compareSTLStrings(sm,"A* Search") ){
				*solution = SEARCH.AStarSearch(*problem);
			} else if ( usjAIL::Data::compareSTLStrings(sm,"Greedy Search") ){
				*solution = SEARCH.GreedySearch(*problem);
			} else if ( usjAIL::Data::compareSTLStrings(sm,"Iterative Deepening Search") ){
				*solution = SEARCH.IterativeDeepeningSearch(*problem);
			} else if ( usjAIL::Data::compareSTLStrings(sm,"Depth First Search") ){
				*solution = SEARCH.DepthFirstSerach(*problem);
			} else if ( usjAIL::Data::compareSTLStrings(sm,"Uniform Cost Search") ){
				*solution = SEARCH.UniformCostSearch(*problem);
			} else if ( usjAIL::Data::compareSTLStrings(sm,"Breadth First Search") ){
				*solution = SEARCH.BreadthFirstSearch(*problem);
			} else {
				*solution = SEARCH.DepthFirstSerach(*problem);
			}
		} else {
			delete solution;
			solution = 0;
		}
		drawingStandard(dArea,0,(gpointer)drawMap);	// update map
	} else if ( usjAIL::Data::compareSTLStrings(widget->name,"Load Map")){
		// load map
		GtkWidget* fileopen;
		fileopen = gtk_file_selection_new("Map to open");
		g_signal_connect(GTK_FILE_SELECTION(fileopen)->ok_button,
				"clicked",G_CALLBACK(loadMap),gpointer(fileopen));
		g_signal_connect_swapped(GTK_FILE_SELECTION(fileopen)->cancel_button,
				"clicked",G_CALLBACK(gtk_widget_destroy),fileopen);
		gtk_file_selection_complete(GTK_FILE_SELECTION(fileopen),"*.map");
		gtk_widget_show(fileopen);
	} else{
	}
}
void loadMap(GtkWidget *w,GtkFileSelection *fs){
	// file opened
	std::string filename = gtk_file_selection_get_filename(fs);
	std::string title;
	if ( !filename.empty() ){
		if ( problem != 0) delete problem;
		problem = new usjAIL::MapProblem(filename);
		if ( problem->getActions().getSize() == 0){
			delete problem;
			problem = 0;
			gtk_window_set_title(GTK_WINDOW(mainWindow),"Map Routing Problem" );
			gtk_widget_set_sensitive(search,false);
			gtk_widget_set_sensitive(searchMethod,false);
			gtk_widget_set_sensitive(gState,false);
			gtk_widget_set_sensitive(iState,false);
		}
	}
	gtk_widget_destroy(GTK_WIDGET(fs));
	if ( problem != 0){
		// problem loaded successfully update other things
		// redraw map
		problem->getStateWithName("Map").get("Map",&title);
		title = "Map Routing Problem : " + title;
		gtk_window_set_title(GTK_WINDOW(mainWindow),title.c_str() );
		gtk_widget_set_sensitive(searchMethod,true);
		// populate initial states and final state
		usjAIL::Actions *acts = new usjAIL::Actions;
		*acts = problem->getActions();
		GList* sslist = 0;
		for ( unsigned int i = 1; i < acts->getSize(); ++i){
			if ( i == 1 ){
				gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(iState)->entry),
						acts->getState(i).getStateName().c_str());
				gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(gState)->entry),
						acts->getState(i).getStateName().c_str());
			}
			sslist = g_list_append(sslist,(gpointer)acts->getState(i).getStateName().c_str());
		}
		gtk_combo_set_popdown_strings(GTK_COMBO(iState),sslist);
		gtk_combo_set_popdown_strings(GTK_COMBO(gState),sslist);
		gtk_widget_set_sensitive(iState,true);
		gtk_widget_set_sensitive(gState,true);
		gtk_widget_set_sensitive(search,true);
	}
	drawingStandard(dArea,0,(gpointer)drawMap);
}
gboolean drawingStandard(GtkWidget *widget,GdkEvent *event,gpointer data){
	cairo_t *cr = gdk_cairo_create(widget->window);
	gint width,height;
	width = gdk_window_get_width(widget->window);
	height = gdk_window_get_height(widget->window);
	// draw border
	cairo_save(cr);
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint (cr);
	cairo_restore(cr);
	cairo_set_source_rgb(cr,0,0,0);
	cairo_set_line_width(cr,3);
	cairo_move_to(cr,0,0);
	cairo_line_to(cr,width,0);
	cairo_line_to(cr,width,height);
	cairo_line_to(cr,0,height);
	cairo_line_to(cr,0,0);
	cairo_stroke_preserve(cr);
	// setting background;
	cairo_set_source_rgba(cr,0.6,0.6,0.6,0.9);
	cairo_fill(cr);
	// if function in data run that
	if ( data != NULL){
		DrawingFunc func = (DrawingFunc)data;
		func(cr,width,height);
	}
	//
	cairo_destroy(cr);
	return TRUE;
}
void drawMap(cairo_t* cr,gint width,gint height){
	std::ostringstream stream;
	cairo_save(cr);
	// set font and size and starting location
	if ( problem == 0){
		cairo_set_source_rgb(cr,0,0,0);
		cairo_select_font_face(cr,"Times New Roman",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size(cr,24);
		cairo_move_to(cr,width/2-100,height/2);
		cairo_show_text(cr,"Map not Loaded");
	} else {
		usjAIL::Actions *acts = new usjAIL::Actions;
		usjAIL::Action *act = new usjAIL::Action;
		usjAIL::State curState;
		*acts = problem->getActions();
		// calculate bounds
		double minx = 10000,miny = 10000,maxx = 0,maxy = 0;
		double scalex,scaley;
		double dval1,dval2;
		for (unsigned int i = 1 ; i < acts->getSize(); ++i){
			acts->getState(i).get("Longitude",&dval1);
			if ( dval1 < minx)	minx = dval1;
			if ( dval1 > maxx)	maxx = dval1;
			acts->getState(i).get("Latitude",&dval2);
			if ( dval2 < miny)	miny = dval2;
			if ( dval2 > maxy)	maxy = dval2;
		}
		scalex = 0.9*width/(maxx - minx);
		scaley = 0.9*height/(maxy - miny);
		for (unsigned int i = 1; i < acts->getSize(); ++i){
			curState = acts->getState(i);
			curState.get("Longitude",&dval1);
			curState.get("Latitude",&dval2);
			dval1 -= minx;	dval1*=scalex;
			dval2 = maxy - dval2;	dval2*=scaley;
			// drawing paths
			*act = acts->getActions(i);
			for ( int j = 1; j <= act->getTotalActions() ; ++j){
				double val1,val2;

				usjAIL::Action curact;
				curact.add(act->getAction(j),act->getStepCost(j));
				usjAIL::State dest = problem->RESULT(curState,curact);
				dest.get("Longitude",&val1);	dest.get("Latitude",&val2);
				val1 -= minx;	val1 *=scalex;
				val2 = maxy - val2;	val2 *=scaley;
				cairo_save(cr);
				cairo_move_to(cr,dval1+15,dval2+15);
				cairo_line_to(cr,val1+15,val2+15);
				cairo_set_source_rgb(cr,0,0,0);
				cairo_stroke(cr);
			}
		}
		for (unsigned int i = 1; i < acts->getSize(); ++i){
			curState = acts->getState(i);
			curState.get("Longitude",&dval1);
			curState.get("Latitude",&dval2);
			dval1 -= minx;	dval1*=scalex;
			dval2 = maxy - dval2;	dval2*=scaley;
			paintName(cr,dval1+15,dval2+15,acts->getState(i).getStateName(),
												1,1,1,0,0,1);
		}
		// now showing result
		if ( solution != 0){
			usjAIL::Stack answer = *solution;
			answer.setStackType(usjAIL::Stack::LIFO);
			if ( answer.ISEMPTY() ){
				std::cout << "Showing Solution " << answer.ISEMPTY() << std::endl;
				cairo_save(cr);
				cairo_move_to(cr,20,15);
				cairo_set_source_rgb(cr,1,1,0);
				cairo_select_font_face(cr,"Times New Roman",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
				cairo_set_font_size(cr,14);
				cairo_show_text(cr,"No Solution Found");
				cairo_restore(cr);
			} else {
				usjAIL::Node node = answer.POP();
				if ( usjAIL::Data::compareSTLStrings(node.STATE().getStateName(),"FAILED") ){
					cairo_save(cr);
					cairo_move_to(cr,20,15);
					cairo_set_source_rgb(cr,1,1,0);
					cairo_select_font_face(cr,"Times New Roman",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
					cairo_set_font_size(cr,14);
					cairo_show_text(cr,"Search Failed");
					cairo_restore(cr);
				} else {
					node.STATE().get("Longitude",&dval1);
					node.STATE().get("Latitude",&dval2);
					dval1 -= minx;	dval1*=scalex;
					dval2 = maxy - dval2;	dval2*=scaley;
					// make it a yellow square
					while ( !answer.ISEMPTY() ){
						double val1,val2;
						node = answer.POP();
						node.STATE().get("Longitude",&val1);	node.STATE().get("Latitude",&val2);
						val1 -= minx;	val1 *= scalex;
						val2 = maxy - val2;	val2 *= scaley;
						// draw path
						cairo_save(cr);
						cairo_move_to(cr,dval1+15,dval2+15);
						cairo_line_to(cr,val1+15,val2+15);
						cairo_set_source_rgb(cr,1,0,0);
						cairo_stroke(cr);
						dval1 = val1;	dval2 = val2;
					}
					// paint city blocks
					answer = *solution;
					answer.setStackType(usjAIL::Stack::LIFO);
					node = answer.POP();
					node.STATE().get("Longitude",&dval1);
					node.STATE().get("Latitude",&dval2);
					dval1 -= minx;	dval1*=scalex;
					dval2 = maxy - dval2;	dval2*=scaley;
					// make it a yellow square
					paintName(cr,dval1+15,dval2+15,node.STATE().getStateName(),
							1,1,0,1,1,0);
					while ( !answer.ISEMPTY() ){
						node = answer.POP();
						node.STATE().get("Longitude",&dval1);	node.STATE().get("Latitude",&dval2);
						dval1 -= minx;	dval1 *= scalex;
						dval2 = maxy - dval2;	dval2 *= scaley;
						if ( answer.ISEMPTY() ){ // goal state
							paintName(cr,dval1+15,dval2+15,node.STATE().getStateName(),
														0,1,0,0,1,0);
							// print total cost
							stream.str("");
							stream << "Toatal Path Cost is : " << node.PATH_COST();
							cairo_save(cr);
							cairo_move_to(cr,20,15);
							cairo_set_source_rgb(cr,1,1,0);
							cairo_select_font_face(cr,"Times New Roman",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
							cairo_set_font_size(cr,14);
							cairo_show_text(cr,stream.str().c_str());
							cairo_restore(cr);
						} else {
							paintName(cr,dval1+15,dval2+15,node.STATE().getStateName(),
														1,1,1,0,0,1);
						}
					}
				}
			}
		}
		// painting step costs
		for (unsigned int i = 1; i < acts->getSize(); ++i){
			curState = acts->getState(i);
			curState.get("Longitude",&dval1);
			curState.get("Latitude",&dval2);
			dval1 -= minx;	dval1*=scalex;
			dval2 = maxy - dval2;	dval2*=scaley;
			// drawing paths
			*act = acts->getActions(i);
			for ( int j = 1; j <= act->getTotalActions() ; ++j){
				double val1,val2;
				usjAIL::Action curact;
				curact.add(act->getAction(j),act->getStepCost(j));
				usjAIL::State dest = problem->RESULT(curState,curact);
				dest.get("Longitude",&val1);	dest.get("Latitude",&val2);
				val1 -= minx;	val1 *=scalex;
				val2 = maxy - val2;	val2 *=scaley;
				cairo_save(cr);
				// write step cost
				stream.str("");
				stream << act->getStepCost(j);
				cairo_move_to(cr,dval1+15+(val1-dval1)/2,dval2+15+(val2-dval2)/2);
				cairo_set_source_rgb(cr,0,1,0);
				cairo_show_text(cr,stream.str().c_str());
				cairo_restore(cr);
			}
		}
		delete acts;
		delete act;
	}
	cairo_restore(cr);
}
void paintName(cairo_t* cr,gint x,gint y,std::string city,double r,double g,double b,double rb,double gb,double bb){
	cairo_save(cr);
	cairo_translate(cr,x,y);
	cairo_set_source_rgb(cr,rb,gb,bb);
	cairo_rectangle(cr,-5,-5,10,10);
	cairo_stroke(cr);
	cairo_set_source_rgb(cr,r,g,b);
	cairo_select_font_face(cr,"Times New Roman",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr,12);
	cairo_move_to(cr,10,0);
	cairo_show_text(cr,city.c_str());
	cairo_restore(cr);
}

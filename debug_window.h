#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

#include <fox/fx.h>
#include "ref.h"

class FXHTMLWindow;
class HTMLObject;

class DebugWindow : public FXDialogBox
{
    FXDECLARE( DebugWindow );

 public:
    enum {
	ID_RELOAD = FXDialogBox::ID_LAST,
	ID_TREE,
	ID_TIMEOUT,
	ID_LAST
    };

 private:

    FXHTMLWindow    *html;
    FXTreeList      *tree;
    Ref<HTMLObject>  root;
    HTMLObject      *marked_up;

    DebugWindow();

    FXIcon *foldericon;
    FXIcon *infoicon;
    FXIcon *contenticon;
    FXIcon *nodeicon;

    FXTimer *timer;

 public:
    DebugWindow( FXApp *app, FXHTMLWindow *html );
    ~DebugWindow();

    long onReload( FXObject *obj, FXSelector sel, void *ptr );
    long onItemSelected( FXObject *obj, FXSelector sel, void *ptr );
    long onMarkupDone( FXObject *obj, FXSelector sel, void *ptr );

    void create();

 private:
    void build_tree( Ref<HTMLObject> node, FXTreeItem *item );
};

#endif

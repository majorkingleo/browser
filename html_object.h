#ifndef HTML_OBJECT_H
#define HTML_OBJECT_H

#include <fox/fx.h>
#include <vector>
#include "ref.h"

#include "html_dc.h"

class Node;
class Image;

class HTMLObject
{
 public:
    enum {
	LAYOUT_NO_HINT = 0,
	LAYOUT_FORCE_LNBR_AFTER,
	LAYOUT_FORCE_LNBR_BEFORE,
	LAYOUT_AUTOSPACE,	
	LAYOUT_FIX_SIZE,
	LAYOUT_MANAGER,
	LAYOUT_ADOPT_MY_CHILDS,
	LAYOUT_DEAMONIZE,
	LAYOUT_WIDTH_PERCENTAGE,
	LAYOUT_HEIGHT_PERCENTAGE,
	LAYOUT_LAST
    };

    enum {
	FONT_DECORE_NONE = 0,
	FONT_DECORE_UNDERLINE,
	FONT_DECORE_STROKE
    };

 protected:
    Ref<HTMLObject> parent;

    std::vector< Ref<HTMLObject> > children;

    int width;
    int height;
    int min_width;
    int min_height;
    int max_width;
    int pwidth;
    int pheight;

    struct Style
    {
	FXString name;
	FXString value;

	Style( const FXString & name, const FXString & value ) : name( name ), value( value ) {}
    };

    std::vector<Style> styles;

    int   layout_hints[LAYOUT_LAST];

    int current_font;

    static int object_type_id_count;
    int object_type_id;

    FXString classname;

    Ref<Node> node;

    bool layouted;
    bool markup;
    bool nolayout;

    HTMLDC dc;

 public:
    HTMLObject( const Ref<HTMLObject> & parent );
    HTMLObject( const Ref<HTMLObject> & parent, const Ref<Node> & node );
    virtual ~HTMLObject();
    
    int getObjectType() { return object_type_id; }
    FXString getClassName() { return classname; }
    bool hasNode() { return node.valid(); }
    Ref<Node> getNode();

    int getWidth() { return width; }
    int getHeight() { return height; }

    virtual void setWidth( int w );
    virtual void setHeight( int h );

    int getMinWidth() { return min_width; }
    int getMinHeight() { return min_height; }

    int getPWidth() { return pwidth; }
    int getPHeight() { return pheight; }

    int getMaxWidth() { return max_width; } ///< returns maximum useful width (not maximum possible width)

    bool relayout() { return !nolayout; }

    virtual void layout();
    virtual void paint();

    Ref<HTMLObject> &  getParent() { return parent; }
    int                getChildNum() const { return children.size(); }
    Ref<HTMLObject> &  getChild( int i ) { return children[i]; }

    void insertChild( Ref<HTMLObject> c, int where );
    void addChield( const Ref<HTMLObject> & c ) { children.push_back( c ); }

    FXString getStyle( const FXString & name, const FXString & default_style = "" );
    FXString getStyle( const FXString & name, int default_style ) 
	{ return getStyle( name, FXString().format( "%d", default_style ) ); }
    FXString getStyle( const FXString & name, unsigned default_style ) 
	{ return getStyle( name, FXString().format( "%d", default_style ) ); }

    int getLocalStylesNum() { return styles.size(); }
    void getLocalStyle( int i, FXString &name, FXString &value ) { name = styles[i].name; value = styles[i].value; }

    void setStyle( const FXString & name, const FXString & value );
    void setStyle( const FXString & name, int value ) { setStyle( name, FXString().format( "%d", value ) ); }
    void setStyle( const FXString & name, unsigned value ) { setStyle( name, FXString().format( "%d", value ) ); }

    bool getLayoutHint( int hint ) const { return layout_hints[hint]; }
    void setLayoutHint( int hint );
    void delLayoutHint( int hint );

    int getCurrentFont() const { return current_font; }

    FXFont* getFont() { return getFont( current_font ); }
    virtual FXFont* getFont( int font );
    virtual FXFont* getFont( const FXString & fontface, unsigned size, unsigned font_weight, unsigned font_slant,
			     unsigned font_encoding, unsigned font_width, unsigned hints,
			     int & id ); 

    virtual void loadURL( const FXString &url );

    virtual void loadImage( const FXString &file );
    virtual void getImage( const FXString &file, Ref<Image> &image );

    virtual void beginLinkCursor( const FXString &url );
    virtual void endLinkCursor();

    virtual void onMouseEnter( int x, int y );
    virtual void onMouseLeave( int x, int y );
    virtual void onMouseClick( int x, int y );
    virtual void onMouseDoubleClick( int x, int y );
    virtual void onMouseDown( int x, int y );
    virtual void onMouseUp( int x, int y );
    virtual void onMouseMove( int x, int y );

    virtual void doMarkup();
    virtual void clearMarkup();

    /// forces children to recache theire stuff (such as fonts, colors). You must call layout at next.
    void recache();

    HTMLDC* getDC() { return &dc; }
};


#endif

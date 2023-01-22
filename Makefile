# this is a comment
SRC=main.cpp fxhtml.cpp \
	node.cpp \
	html_dc.cpp \
	html_object.cpp \
	object_root.cpp \
	object_body.cpp \
	object_text.cpp \
	object_layout.cpp \
	object_hr.cpp \
	object_table.cpp \
	object_row.cpp \
	object_td.cpp \
	object_ul.cpp \
	object_image.cpp \
	string_utils.cpp \
	object_a.cpp \
	cppdir.cpp \
	debug_window.cpp \
	build_tree.cpp \
	url_loader.cpp \
	thread.cpp \
	fxurl2.cpp \
	image.cpp \
	image_size.cpp

OBJ=$(SRC:.cpp=.o) # replaces the .c from SRC with .o
EXE=browser

CC=g++
CFLAGS=-Wall -g -pthread -DUSE_LIBTIDY # -O3 -pg -DUSE_LIBTIDY
LDFLAGS= -lFOX -lpthread #-ltidy # -lefence #-lsgml
RM=rm -f

%.o: %.cpp         # combined w/ next line will compile recently changed .c files
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY : all     # .PHONY ignores files named all
all: $(EXE)      # all is dependent on $(EXE) to be complete

.PHONY: depend
depend:
	$(CC) -MM $(SRC) >.dependencies

.dependencies: $(SRC)
	$(CC) -MM $(SRC) >$@

include .dependencies

$(EXE): $(OBJ)   # $(EXE) is dependent on all of the files in $(OBJ) to exist
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $@

.PHONY : clean   # .PHONY ignores files named clean
clean:
	$(RM) $(OBJ) core

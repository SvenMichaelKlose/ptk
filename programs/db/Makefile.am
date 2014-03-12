bin_PROGRAMS=	ptkdb
ptkdb_SOURCES=	db_editor.cc db_entry_list.cc db_entry_view.cc db_list.cc \
		db_row_list.cc db_table_list.cc dbi.cc login.cc main.cc
ptkdb_LDADD=	-lptk -lptktoolkit -lmysqlclient -lX11 -L/usr/local/lib -L/usr/X11R6/lib -L/usr/local/lib/mysql
INCLUDES= 	-I. -I../../src/ptk -I../../src/ptktoolkit -I/usr/local/include
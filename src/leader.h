/* data */
#define LINK(link, first, last, next, prev)			\
do								\
{								\
    if ( !(first) )						\
      (first)			= (link);			\
    else							\
      (last)->next		= (link);			\
    (link)->next		= NULL;				\
    (link)->prev		= (last);			\
    (last)			= (link);			\
} while(0)

#define INSERT(link, insert, first, next, prev)			\
do								\
{								\
    (link)->prev		= (insert)->prev;		\
    if ( !(insert)->prev )					\
      (first)			= (link);			\
    else							\
      (insert)->prev->next	= (link);			\
    (insert)->prev		= (link);			\
    (link)->next		= (insert);			\
} while(0)

#define UNLINK(link, first, last, next, prev)			\
do								\
{								\
    if ( !(link)->prev )					\
      (first)			= (link)->next;			\
    else							\
      (link)->prev->next	= (link)->next;			\
    if ( !(link)->next )					\
      (last)			= (link)->prev;			\
    else							\
      (link)->next->prev	= (link)->prev;			\
} while(0)


typedef struct hiscore_entry HISCORE_ENTRY;
typedef struct hiscore HISCORE;
struct hiscore_entry
{
	char *name;
	int score;

	HISCORE_ENTRY *next;
	HISCORE_ENTRY *prev;
};
struct hiscore
{
	char *keyword;
	char *desc;
	int vnum;
	sh_int max_length;
	sh_int length;
	HISCORE_ENTRY *first_entry; /* first entry is number 1, highest
score */
	HISCORE_ENTRY *last_entry; /* last entry is number 10 or last,
lowest */

	HISCORE		  *next;
	HISCORE		  *prev;
};

HISCORE *first_table;
HISCORE *last_table;

/*
void do_hiscoset( CHAR_DATA *ch, char *argument );
void do_hiscore( CHAR_DATA *ch, char *argument );

char *is_hiscore_obj( OBJ_DATA *obj );
void show_hiscore( char *keyword, CHAR_DATA *victim );

void adjust_hiscore( char *keyword, CHAR_DATA *ch, int score );
*/

/* local function declarations */
bool add_hiscore( char *keyword, char *name, int score );
sh_int get_position( char *keyword, char *name );

void fix_table_length( HISCORE *table );
void create_hitable( char *keyword, char *desc );
bool destroy_hitable( char *keyword );
void save_hiscores( void );
void load_hiscores( void );
HISCORE *find_table( char *keyword );
HISCORE *load_hiscore_table( FILE *fp );

bool clear_hitable( char *keyword );

char *is_hiscore_obj( OBJ_DATA *obj );
void show_hiscore( char *keyword, CHAR_DATA *victim );

void adjust_hiscore( char *keyword, CHAR_DATA *ch, int score );


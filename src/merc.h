/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.-doc' as well the Merc
*
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#include "warlock.h"
#include "clans.h"
#include "arena.h"
#include "monk.h"
#include "garou.h"
#include "dragon.h"
#include "wraith.h"
#include "hunter.h"
#include "dragon.h"
/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#endif



/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef short    int			sh_int;
typedef unsigned char			bool;
#endif
#include "player.h"
#include "old.h"
//#include "hiscore.h" 

/*
 * Structure types.
 */
typedef struct  clans_data		CLAN_DATA;
typedef struct  leader_data		LEADER_DATA;
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct	char_data		CHAR_DATA;
typedef struct  config_data		CONFIG_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct  project_data            PROJECT_DATA;
typedef struct	kill_data		KILL_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct  kingdom_data		KINGDOM_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct  room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
typedef struct  editing_data            EDITING_DATA;	/* OLC */
typedef struct  system_data		SYSTEM_DATA;

/* Added Toaster 24/03/00 for System data stuff */

typedef struct  disabled_data                 DISABLED_DATA;

typedef struct  magic_shield		MAGIC_SHIELD;
// Added by Toaster 05/05/00

/* one disabled command */
struct disabled_data
{
      DISABLED_DATA *next; /* pointer to next node */
      struct cmd_type const *command; /* pointer to the command struct*/
      char *disabled_by; /* name of disabler */
      sh_int level; /* level of disabler */
};
extern                        DISABLED_DATA     *disabled_first;
extern				BAN_DATA	*ban_first;
/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch ) );
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo ));

/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 4096
#define MAX_INPUT_LENGTH	  160
#define MSL			(MAX_STRING_LENGTH)
#define MIL			(MAX_INPUT_LENGTH)


#define SCREEN_WIDTH		79

#define HELP_FILE       "help.txt"   /* For undefined helps */
#define PLAYER_LOG	"player.log" /* Not used yet 	    */
#define CHANGES_FILE    "../data/changes.dat"
#define PROJECTS_FILE   "../data/projects.txt"
#define MEMLOG_FILE     "../data/memory.txt"
/*
 * Colour stuff by Lope of Loping Through The MUD
 */
#define CLEAR		"[0m"		/* Resets Colour	*/
//#define CLEAR		"[0;37m"
#define C_RED		"[0;31m"	/* Normal Colours	*/
#define C_GREEN		"[0;32m"
#define C_YELLOW	"[0;33m"
#define C_BLUE		"[0;34m"
#define C_MAGENTA	"[0;35m"
#define C_CYAN		"[0;36m"
#define C_WHITE		"[0;37m"
#define C_D_GREY	"[1;30m"  	/* Light Colors		*/
#define C_B_RED		"[1;31m"
#define C_B_GREEN	"[1;32m"
#define C_B_YELLOW	"[1;33m"
#define C_B_BLUE	"[1;34m"
#define C_B_MAGENTA	"[1;35m"
#define C_B_CYAN	"[1;36m"
#define C_B_WHITE	"[1;37m"

/*
 * Rotains Gobal Procedures
 */
void clear_stats    args( (CHAR_DATA *ch) );
void room_is_total_darkness args( (ROOM_INDEX_DATA *pRoomIndex) );
void improve_wpn    args( (CHAR_DATA *ch,int dtype, bool right_hand) );
void improve_stance args( (CHAR_DATA *ch) );
void skillstance    args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void show_spell     args( (CHAR_DATA *ch, int dtype) );
void fightaction    args( (CHAR_DATA *ch, CHAR_DATA *victim, int actype , \
int dtype, int wpntype) );
void crack_head     args( (CHAR_DATA *ch, OBJ_DATA *obj, char *argument) );
void critical_hit   args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam) );

void take_item      args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void raw_kill       args( (CHAR_DATA *victim, CHAR_DATA *ch) );
void trip           args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void disarm         args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void make_corpse    args( (CHAR_DATA *ch) );
void one_hit        args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, \
				int handtype) );
void make_part      args( (CHAR_DATA *ch, char *argument) );

#include "board.h"

/*void clan_table_read        args( ( ) );
void artifact_table_read    args( ( ) );
void clan_table_write       args( ( ) );
void artifact_table_write   args( ( ) );
void home_write             args( ( ) );
void clan_table_powerselect args( (CHAR_DATA *ch, char *power_name) );
void clan_table_namefill    args( (CHAR_DATA *ch) );
void clan_table_bite        args( (CHAR_DATA *ch) );
void clan_table_dec         args( (CHAR_DATA *ch, CHAR_DATA *victim) );
*/
void behead                 args( (CHAR_DATA *victim) );
void paradox                args( (CHAR_DATA *ch) );


void load_bans	args( (void) );
void save_bans	args( (void) );
void load_kingdoms args( (void) );
void save_kingdoms args( (void) );
/*
 * Godwars Game Parameters
 * By Rotain
 */

#define SKILL_ADEPT               100
/*This is minor bug Toast Changed*/
#define SKILL_THAC0_32             6
#define SKILL_THAC0_00              18
#define VERSION_NUMBER              2
#define DONATION_ROOM_WEAPON     6641
#define DONATION_ROOM_ARMOR      6643
#define DONATION_ROOM_REST       6644
#define MAX_VAMPIRE_POWER           3
#define MAX_CLAN                   12
#define MAX_ART                    12
#define MAX_KINGDOM		   20
#define MAX_FORGET		   10

#define EDIT_HELP(ch, help)	(help = HELP_DATA*)ch->desc->pEdit )

/*CLAN LIST STUFF FROM KaVIr! Once again he comes through :)*/
#define CLAN_NONE		      0
#define CLAN_CAITIFF		      0
#define CLAN_LASOMBRA		      1
#define CLAN_TZIMISCE		      2
#define CLAN_GANGREL		      3
#define CLAN_VENTRUE		      4
#define CLAN_TREMERE		      5
#define CLAN_ASSAMITE		      6
#define CLAN_MALKAVIAN		      7
#define CLAN_CAPPADOCIAN	      8
#define CLAN_TOREADOR		      9
#define CLAN_NOSFERATU		     10
#define CLAN_UNBOUND		     11
/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 * Changed MAX_SPELL TO 61 for SOULSTRIKE 12/02/00
 */

#define PARADOX_TICK               30
#define MAX_SKILL	 	  200
#define MAX_SPELL		   61
#define MAX_LEVEL		   12
#define L_ANG		   MAX_LEVEL
/*Changed by Toaster 08/02/00 to only allow lvl 12 to use imc set
commands*/
#define NO_WATCH		   10
#define NO_GODLESS		   (MAX_LEVEL - 1)
#define LEVEL_HERO		   (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 5)

#define LEVEL_MORTAL		   (MAX_LEVEL - 10)
#define LEVEL_AVATAR		   (MAX_LEVEL - 9)
#define LEVEL_APPRENTICE	   (MAX_LEVEL - 8)
#define LEVEL_MAGE		   (MAX_LEVEL - 7)
#define LEVEL_ARCHMAGE		   (MAX_LEVEL - 6)
#define LEVEL_NINJA                (MAX_LEVEL - 6)
#define LEVEL_MONK                 (MAX_LEVEL - 6)
#define LEVEL_BUILDER		   (MAX_LEVEL - 5)
#define LEVEL_QUESTMAKER	   (MAX_LEVEL - 4)
#define LEVEL_ENFORCER		   (MAX_LEVEL - 3)
#define LEVEL_JUDGE		   (MAX_LEVEL - 2)
#define LEVEL_HIGHJUDGE		   (MAX_LEVEL - 1)
#define LEVEL_IMPLEMENTOR	   (MAX_LEVEL)


#define L_UNK	0
#define L_CIT	1
#define L_LEG	2
#define L_MYT	3
#define L_TIT	4
#define L_ORA	5
#define L_IMM	6
#define L_DEM	7
#define L_DEI	8
#define L_ALM	9


#define PULSE_PER_SECOND	    4
#define PULSE_GAIN_EXP		  ( 1 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE		  ( 3 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  ( 4 * PULSE_PER_SECOND)
#define PULSE_TICK		  (30 * PULSE_PER_SECOND)
#define PULSE_AREA		  (60 * PULSE_PER_SECOND)
#define PULSE_WW                  ( 4 * PULSE_PER_SECOND)


#define A			 1
#define B			 2
#define C 			 4
#define D	 		 8
#define E		        16
#define F		        32
#define G		        64
#define H		       128
#define I		       256
#define J		       512
#define K		      1024
#define L		      2048
#define M		      4096
#define N      		      8192
#define O      		     16384
#define P     	 	     32768
#define Q      		     65536
#define R      		    131072
#define S      		    262144
#define T      		    524288
#define U      		   1048576
#define V      		   2097152
#define W      		   4194304
#define X    	  	   8388608
#define Y    	  	  16777216
#define Z     	 	  33554432
#define aa     		  67108864
#define bb    	 	 134217728
#define cc     		 268435456
#define dd     		 536870912
#define ee		1073741824

/*
 * Clan structure.
 */
struct clans_data
{
    int         members;        /* Number of members */
    char *      clan_head;      /* Name of clan head */
    char *      primogen;       /* Name of primogen */
};

struct  project_data
{
    PROJECT_DATA * next;                /* Next project in list*/
    PROJECT_DATA * prev;                /* Previous project in list*/
    NOTE_DATA *  first_log;             /* First log on project*/
    NOTE_DATA *  last_log;              /* Last log  on project*/
    char *name;
    char *owner;
    char *coder;
    char *status;
    char *date;
    char *description;
    bool taken;                         /* Has someone taken project?*/
};


/*
 * Rotains Clan Table Structure
 *
struct clan_table_type
{
  char * clan_name;
  char * clan_leader;
  char * clan_powers_1;
  char * clan_powers_2;
  char * clan_powers_3;
  int    clan_kills [MAX_CLAN];
  int    lvnum;
  int    uvnum;
  int    clan_recall;
};
*/
struct artifact_type
{
  char * player_name;
  int    object_vnum;
};

struct dgain_type
{
    char *name;
    int bit;
    int req;
    int cost;
};

struct dage_type
{
    char *name;
    int  bit;
    int  cost;
};

/*
 * Site ban structure.
 */
struct	ban_data
{
/*
    BAN_DATA *	next;
    char *	name;
*/
/* For new ban code -Tijer */
    BAN_DATA    *next;
    char        *name;
    char        *by_who;
    sh_int      level;
    bool        newbie_banned;
};



/*
 * Store types of char logged in.
 */

#define LOGIN_NONE		    0
#define LOGIN_NEWBIE		    1
#define LOGIN_MORTAL		    2
#define LOGIN_GOD		    4
#define LOGIN_VAMPIRE		    8
#define LOGIN_WEREWOLF		   16
#define LOGIN_MAGE		   32
#define LOGIN_DEMON		   64
#define LOGIN_HIGHLANDER	   128
#define LOGIN_DROW		   256
#define LOGIN_NINJA		   512
#define LOGIN_MONK		   1024
#define LOGIN_ABOMINATION	   2048
/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define MOON_RISE		    0
#define MOON_UP			    1
#define MOON_SET		    2
#define MOON_DOWN	   	    3

#define MORNING			    0
#define DAY			    1
#define EVENING			    2
#define NIGHT			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3

#define MOON_NEW		    0
#define MOON_CRESCENT		    1
#define MOON_HALF		    2
#define MOON_GIBBOUS		    3
#define MOON_FULL		    4
#define MOON_WANING		    5

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
    int		moonlight;
    int		moonphase;
    int		time_of_day;
};



/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_SEX			 6
#define CON_GET_NEW_CLASS		 7
#define CON_READ_MOTD			 8
#define CON_NOT_PLAYING			 9
#define CON_GET_NEW_ANSI                 10
#define CON_COPYOVER_RECOVER            11
#define CON_EDITING			12

/*
 * Character substates
 */
typedef enum
{
  SUB_NONE, SUB_PAUSE, SUB_PERSONAL_DESC, SUB_OBJ_SHORT, SUB_OBJ_LONG,
  SUB_OBJ_EXTRA, SUB_MOB_LONG, SUB_MOB_DESC, SUB_ROOM_DESC, SUB_ROOM_EXTRA,
  SUB_ROOM_EXIT_DESC, SUB_WRITING_NOTE, SUB_MPROG_EDIT, SUB_HELP_EDIT,
  SUB_WRITING_MAP, SUB_PERSONAL_BIO, SUB_REPEATCMD, SUB_RESTRICTED,
  SUB_DEITYDESC,
  /* timer types ONLY below this point */
  SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;



/*
 * Connected states for OLC.
 *
#define CON_AEDITOR                     -1
#define CON_REDITOR                     -2
#define CON_MEDITOR                     -3
#define CON_OEDITOR                     -4
*/
/*
 * Timers
 */

#define MAX_TIMER 10
#define TIMER(ch, tmr)		((ch)->pcdata->tick_timer[(tmr)])
#define SET_TIMER(ch, tmr, tm)  ((ch)->pcdata->tick_timer[(tmr)] = (tm))
#define ADD_TIMER(ch, tmr, tm)  ((ch)->pcdata->tick_timer[(tmr)] += (tm))
#define SUB_TIMER(ch, tmr, tm)  ((ch)->pcdata->tick_timer[(tmr)] -= (tm))
#define TIME_UP(ch, tmr)	((ch)->pcdata->tick_timer[(tmr)] == 0 ? TRUE : FALSE )



// add timers here.. #define TIMER_WHATEVER  0...

#define TIMER_THIRD_ARM_GOING	0
#define TIMER_FOURTH_ARM_GOING	1
/*
 * Connected states for OLC.
 */
#define CON_AEDITOR                     -1
#define CON_REDITOR                     -2
#define CON_MEDITOR                     -3
#define CON_OEDITOR                     -4
/*
Extra global's done by KaVir
*/
extern		int			world_affects;
/* Double exp global */
extern     bool     global_exp;
/*
 * Online editing structure.
 */
struct  editing_data	/* OLC */
{
    void *              pIndex;
    int                 vnum;
    int                 type;
    int                 command;
    char *              text;
    char **             destination;
};




/*
 * Area flags. OLC
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define			AREA_UNLINKED	8	// Sage- unlinked zone


/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    char *		host;
    sh_int		descriptor;
    sh_int		connected;
    sh_int		idle;
//Changed Toaster 06/03/00 cos i want idle
    bool		fcommand;
    bool 		ansi;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    char *		outbuf;
    int			outsize;
    int			outtop;
    EDITING_DATA        editing;	/* OLC */
    void *              pEdit;		/* OLC */
    char **             pString;	/* OLC */
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3



/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    sh_int	level;
    char *	keyword;
    char *	text;
};



/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    sh_int	keeper;			/* Vnum of shop keeper mob	*/
    sh_int	buy_type [MAX_TRADE];	/* Item types shop will buy	*/
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
};



/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA * next;
    char *      sender;
    char *      date;
    char *      to_list;
    char *      subject;
    char *      text;
    time_t      date_stamp;
    time_t      expire;
};


/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
};



/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    sh_int		number;
    sh_int		killed;
};



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	   3060
#define MOB_VNUM_VAMPIRE	   3404
#define MOB_VNUM_CANNIBAL	   30069

/*
 * Immunities, for players.  KaVir.
 */
#define IMM_SLASH	      1	/* Resistance to slash, slice. 		*/
#define IMM_STAB	      2	/* Resistance to stab, pierce. 		*/
#define IMM_SMASH	      4	/* Resistance to blast, pound, crush. 	*/
#define IMM_ANIMAL	      8	/* Resistance to bite, claw. 		*/
#define IMM_MISC	     16	/* Resistance to grep, suck, whip. 	*/
#define IMM_CHARM	     32	/* Immune to charm spell. 		*/
#define IMM_HEAT	     64	/* Immune to fire/heat spells. 		*/
#define IMM_COLD	    128	/* Immune to frost/cold spells.		*/
#define IMM_LIGHTNING	    256	/* Immune to lightning spells.		*/
#define IMM_ACID	    512	/* Immune to acid spells.		*/
#define IMM_SUMMON	   1024	/* Immune to being summoned.		*/
#define IMM_VOODOO	   2048	/* Immune to voodoo magic.		*/
#define IMM_VAMPIRE	   4096	/* Allow yourself to become a vampire.	*/
#define IMM_STAKE	   8192	/* Immune to being staked (vamps only). */
#define IMM_SUNLIGHT	  16384	/* Immune to sunlight (vamps only).	*/
#define IMM_SHIELDED	  32768 /* For Obfuscate. Block scry, etc.	*/
#define IMM_HURL	  65536 /* Cannot be hurled.			*/
#define IMM_BACKSTAB	 131072 /* Cannot be backstabbed.		*/
#define IMM_KICK	 262144 /* Cannot be kicked.			*/
#define IMM_DISARM	 524288 /* Cannot be disarmed.			*/
#define IMM_STEAL	1048576 /* Cannot have stuff stolen.		*/
#define IMM_SLEEP	2097152 /* Immune to sleep spell.		*/
#define IMM_DRAIN	4194304 /* Immune to energy drain.		*/
#define IMM_DEMON	8388608 /* Allow yourself to become a demon.	*/
#define IMM_TRANSPORT  16777216 /* Objects can't be transported to you.	*/
#define IMM_TRAVEL     33554432 /*so demons can turn travel off*/

/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		      1		/* Auto set for mobs	*/
#define ACT_SENTINEL		      2		/* Stays in one room	*/
#define ACT_SCAVENGER		      4		/* Picks up objects	*/
#define ACT_AGGRESSIVE		     32		/* Attacks PC's		*/
#define ACT_STAY_AREA		     64		/* Won't leave area	*/
#define ACT_WIMPY		    128		/* Flees when hurt	*/
#define ACT_PET			    256		/* Auto set for pets	*/
#define ACT_TRAIN		    512		/* Can train PC's	*/
#define ACT_PRACTICE		   1024		/* Can practice PC's	*/
#define ACT_MOUNT		   2048		/* Can be mounted	*/
#define ACT_NOPARTS		   4096		/* Dead = no body parts	*/
#define ACT_NOEXP		   8192		/* No exp for killing	*/
#define ACT_NOAUTOKILL		  16384
#define ACT_SKILLED		  32768
#define ACT_COMMANDED		  65536		/* Can't attack newbies */

#define KITH_BOGGAN		1
#define KITH_SIDHE		2
#define KITH_NOCKER		4
#define KITH_POOKA		32
#define KITH_REDCAP		64
#define KITH_SLUAGH		128
#define KITH_ESHU		256
#define KITH_SATYR		512
#define KITH_TROLL		1024

/*
 * Thingers for Demon Warps
 */

#define WARP_CBODY				1
#define WARP_SBODY				2
#define WARP_STRONGARMS				4
#define WARP_STRONGLEGS				8
#define WARP_VENOMTONG				16
#define WARP_SPIKETAIL				32
#define WARP_BADBREATH				64
#define WARP_QUICKNESS				128
#define WARP_STAMINA				256
#define WARP_HUNT				512
#define WARP_DAMCAP				1024
#define WARP_TERROR				2048
#define WARP_REGEN				4096
#define WARP_STEED				8192
#define WARP_WEAPON				16384
#define WARP_INFIRMITY				32768
#define WARP_GBODY				65536
#define WARP_SCARED				131072
#define WARP_MAGMA				262144
#define WARP_WEAK				524288
#define WARP_SLOW				1048576
#define WARP_VULNER				2097152
#define WARP_SHARDS				4194304
#define WARP_WINGS				8388608
#define WARP_CLUMSY				16777216
#define WARP_STUPID				33554432
#define WARP_SPOON				67108864
#define WARP_FORK				134217728
#define WARP_KNIFE				268435456
#define WARP_SALADBOWL				536870912
//#define WARP_HITROLL				1073741828
//#define WARP_DAMROLL				2147483648



/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		      1
#define AFF_INVISIBLE		      2
#define AFF_DETECT_EVIL		      4
#define AFF_DETECT_INVIS	      8
#define AFF_DETECT_MAGIC	     16
#define AFF_DETECT_HIDDEN	     32
#define AFF_SHADOWPLANE		     64 /* Creatures in shadow plane - KaVir */
#define AFF_SANCTUARY		    128
#define AFF_FAERIE_FIRE		    256
#define AFF_INFRARED		    512
#define AFF_CURSE		   1024
#define AFF_FLAMING		   2048 /* For burning creatures - KaVir */
#define AFF_POISON		   4096
#define AFF_PROTECT		   8192
#define AFF_ETHEREAL		  16384 /* For ethereal creatures - KaVir */
#define AFF_SNEAK		  32768
#define AFF_HIDE		  65536
#define AFF_SLEEP		 131072
#define AFF_CHARM		 262144
#define AFF_FLYING		 524288
#define AFF_PASS_DOOR		1048576
#define AFF_POLYMORPH		2097152 /* For polymorphed creatures - KaVir */
#define AFF_SHADOWSIGHT		4194304 /* Can see between planes - KaVir */
#define AFF_WEBBED		8388608 /* Cannot move - KaVir */
#define AFF_TOTALBLIND      16777216 /* Cannot get pregnant - KaVir */
#define AFF_DARKNESS           33554432 /* Drow Darkness - Rotain */
#define AFF_PROTECTG	       67108864
#define AFF_SAFE              134217728
#define AFF_SHIFT	      268435456
#define AFF_INFIRMITY          536870912 /* Monk and Paladin Bless */
#define AFF_STEELSHIELD	      1073741824
//#define AFF_DROWFIRE	      2147483648 Was Protectg above.


/*
 * The Affs Strike Back
 * 'flag2'
 */
#define AFF2_BLINK_1ST_RD		(A)
#define AFF2_BLINK_2ND_RD		(B)
#define AFF2_SPIRITGUARD                (C)
#define VAMP_ASHES			(D)
#define VAMP_CLONE			(E)
#define VAMP_OBJMASK			(F)
#define AFF2_ROT			(G)
#define AFF2_CONTRACEPTION	        (H)
#define AFF2_TENDRILS		        (I)
#define EXTRA_AFK		        (J)
#define AFF2_AFK		        (K)
#define AFF2_REIMB		        (L)
#define AFF2_CHALLENGED			(M)
#define AFF2_CHALLENGER			(N)
#define AFF2_INARENA			(O)
#define AFF2_BALOR			(P)
#define AFF2_IMP                        (Q)
#define AFF2_SIXTHSENSE                 (R)
#define AFF2_BEASTIE			(S)

/*
 * Bits for 'itemaffect'.
 * Used in #MOBILES.
 */
#define ITEMA_SHOCKSHIELD	      1
#define ITEMA_FIRESHIELD	      2
#define ITEMA_ICESHIELD		      4
#define ITEMA_ACIDSHIELD	      8
#define ITEMA_DBPASS                 16
#define ITEMA_CHAOSSHIELD            32
#define ITEMA_ARTIFACT               64
#define ITEMA_REGENERATE            128
#define ITEMA_SPEED                 256
#define ITEMA_VORPAL                512
#define ITEMA_PEACE                1024
#define ITEMA_RIGHT_SILVER         2048
#define ITEMA_LEFT_SILVER          4096
#define ITEMA_REFLECT              8192
#define ITEMA_RESISTANCE          16384
#define ITEMA_VISION              32768
#define ITEMA_STALKER             65536
#define ITEMA_VANISH             131072
#define ITEMA_RAGER              262144
#define ITEMA_HIGHLANDER         524288
#define ITEMA_PALADIN           1048576
#define ITEMA_SUNSHIELD         2097152
#define ITEMA_MASTERY		4194304

/*
 * Rune, Glyph and Sigil bits.
 */
#define RUNE_FIRE	      1
#define RUNE_AIR	      2
#define RUNE_EARTH	      4
#define RUNE_WATER	      8
#define RUNE_DARK	      16
#define RUNE_LIGHT	      32
#define RUNE_LIFE	      64
#define RUNE_DEATH	      128
#define RUNE_MIND	      256
#define RUNE_SPIRIT	      512
#define RUNE_MASTER	      1024
#define GLYPH_CREATION	      1
#define GLYPH_DESTRUCTION     2
#define GLYPH_SUMMONING	      4
#define GLYPH_TRANSFORMATION  8
#define GLYPH_TRANSPORTATION  16
#define GLYPH_ENHANCEMENT     32
#define GLYPH_REDUCTION	      64
#define GLYPH_CONTROL	      128
#define GLYPH_PROTECTION      256
#define GLYPH_INFORMATION     512
#define SIGIL_SELF	      1
#define SIGIL_TARGETING       2
#define SIGIL_AREA	      4
#define SIGIL_OBJECT	      8


/*
 * Advanced spells.
 */
#define ADV_DAMAGE	      1
#define ADV_AFFECT	      2
#define ADV_ACTION	      4
#define ADV_AREA_AFFECT       8
#define ADV_VICTIM_TARGET    16
#define ADV_OBJECT_TARGET    32
#define ADV_GLOBAL_TARGET    64
#define ADV_NEXT_PAGE       128
#define ADV_PARAMETER       256
#define ADV_SPELL_FIRST     512
#define ADV_NOT_CASTER     1024
#define ADV_NO_PLAYERS     2048
#define ADV_SECOND_VICTIM  4096
#define ADV_SECOND_OBJECT  8192
#define ADV_REVERSED      16384
#define ADV_STARTED	  32768
#define ADV_FINISHED	  65536
#define ADV_FAILED	 131072
#define ADV_MESSAGE_1	 262144
#define ADV_MESSAGE_2	 524288
#define ADV_MESSAGE_3	1048576
#define ADV_COLORED	    (V)

/*
 * Advanced spell actions.
 */
#define ACTION_NONE	      0
#define ACTION_MOVE	      1
#define ACTION_MOB	      2
#define ACTION_OBJECT	      3

/*
 * Advanced spell affects.
 */
#define ADV_STR			      1
#define ADV_DEX			      2
#define ADV_INT			      4
#define ADV_WIS			      8
#define ADV_CON			     16
#define ADV_SEX			     32
#define ADV_MANA		     64
#define ADV_HIT			    128
#define ADV_MOVE		    256
#define ADV_AC			    512
#define ADV_HITROLL		   1024
#define ADV_DAMROLL		   2048
#define ADV_SAVING_SPELL	   4096



/* Colour scale macros - added 12th Aug 1995 by Calamar */

#define NO_COLOUR	""		/* Blank */
#define GREY		"[0;1;30m"	/* Dark Grey */
#define D_RED		"[0;0;31m"	/* Dark Red */
#define L_RED		"[0;1;31m"	/* Light Red */
#define D_GREEN		"[0;0;32m"	/* Dark Green */
#define L_GREEN		"[0;1;32m"	/* Light Green */
#define BROWN		"[0;0;33m"	/* Brown */
#define YELLOW		"[0;1;33m"	/* Yellow */
#define D_BLUE		"[0;0;34m"	/* Dark Blue */
#define L_BLUE		"[0;1;34m"	/* Light Blue */
#define MAGENTA		"[0;0;35m"	/* Magenta */
#define PINK		"[0;1;35m"	/* Pink */
#define D_CYAN		"[0;0;36m"	/* Dark Cyan */
#define L_CYAN		"[0;1;36m"	/* Light Cyan */
#define NORMAL		"[0;0;37m"	/* Light Grey */
#define WHITE		"[0;0;38m"	/* White */

#define ADD_COLOUR(_player,_str,_col) {char swh_temp[255]; \
if (!IS_NPC(_player) && IS_SET(_player->act, PLR_ANSI)) {swh_temp[0] = '\0'; \
strcpy(swh_temp, _col); strcat(swh_temp, _str); strcat(swh_temp, NORMAL); \
strcpy(_str, swh_temp);}}

#define SCALE_COLS 4

#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) \
	ADD_COLOUR(_swh_ch, _swh_str, \
	(_swh_curr < 1) ? L_RED : \
	(_swh_curr < _swh_max) ? \
	scale[(SCALE_COLS * _swh_curr) / ((_swh_max > 0) \
	? _swh_max : 1)] : L_CYAN)

/*COmmented out by Sage to fix prompt stuff 12/23/99
#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) \
	ADD_COLOUR(_swh_ch, _swh_str, \
	(_swh_curr < 1) ? L_CYAN : \
	(_swh_curr < _swh_max) ? \
	scale[(SCALE_COLS * _swh_curr) / ((_swh_max > 0) \
	? _swh_max : 1)] : L_CYAN)
*/
extern char *scale[SCALE_COLS];

/*
 * Bits for 'vampire'.
 * Used for player vampires.
 */
#define VAM_FANGS		      1
#define VAM_CLAWS		      2
#define VAM_NIGHTSIGHT		      4
#define VAM_FLYING		      8 /* For flying creatures */
#define VAM_SONIC		     16 /* For creatures with full detect */
#define VAM_CHANGED		     32 /* Changed using a vampire power */

#define VAM_PROTEAN		     64 /* Claws, nightsight, and change */
#define VAM_CELERITY		    128 /* 66%/33% chance 1/2 extra attacks */
#define VAM_FORTITUDE		    256 /* 5 hp less per hit taken */
#define VAM_POTENCE		    512 /* Deal out 1.5 times normal damage */
#define VAM_OBFUSCATE		   1024 /* Disguise and invis */
#define VAM_AUSPEX		   2048 /* Truesight, etc */
#define VAM_OBTENEBRATION	   4096 /* Shadowplane/sight and shadowbody */
#define VAM_SERPENTIS		   8192 /* Eyes/serpent, heart/darkness, etc */

#define VAM_DISGUISED		  16384 /* For the Obfuscate disguise ability */
#define VAM_MORTAL		  32768 /* For Obfuscate mortal ability. */

#define VAM_DOMINATE		  65536 /* Evileye, command */

#define VAM_EVILEYE		 131072 /* Evileye, command */

#define VAM_PRESENCE		 262144 /* Presence discipline */

#define VAM_VICISSITUDE          524288

#define FAE_CHIC	0
#define FAE_LEGE	1
#define FAE_PRIM	2
#define FAE_SOOT	3
#define FAE_SOVE	4
#define FAE_WAYF	5



/* New vamp defines*/
#define VAM_ANIM	0
#define VAM_AUSP	1
#define VAM_DAIM	2
#define VAM_DOMI	3
#define VAM_FORT	4
#define VAM_OBFU	5
#define VAM_OBTE	6
#define VAM_PRES	7
#define VAM_PROT	8
#define VAM_QUIE	9
#define VAM_SERP	10
#define VAM_THAU	11
#define VAM_VICI	12
#define VAM_CELE	13
#define VAM_CHIM	14
#define VAM_POTE	15
#define VAM_OBEA	16
#define VAM_MELP	17
#define VAM_THAN 	18
#define VAM_NECR	19

#define DISC_MAX        19
/*end new vamp section*/


#define RES_ANIMALISM		      1
#define RES_AUSPEX		      2
#define RES_DOMINATE		      4
#define RES_PRESENCE		      8
#define RES_TOGGLE		     16
#define RES_CELERITY		     32

#define WILL_ANIMALISM		      1
#define WILL_AUSPEX		      2
#define WILL_DOMINATE		      3
#define WILL_PRESENCE		      4


/*Define new vamp powers*/

/*
 * Bits for 'polymorph'.
 * Used for players.
 */
#define POLY_BAT		      1
#define POLY_WOLF		      2
#define POLY_MIST		      4
#define POLY_SERPENT		      8
#define POLY_RAVEN		     16
#define POLY_FISH		     32
#define POLY_FROG		     64
#define POLY_ZULO                   128
#define POLY_SPIDERFORM		    256
#define POLY_HAGS		    512
#define POLY_FLESHCRAFT		    1024
#define POLY_NEWZULO		    2048
#define POLY_DRAGON		    4096

/*
 * Languages.
 */
#define LANG_COMMON		      0
#define DIA_OLDE		      1
#define DIA_BAD			      2
#define LANG_DARK		      4



/*
 * Score.
 */
#define SCORE_TOTAL_XP		      0
#define SCORE_HIGH_XP		      1
#define SCORE_TOTAL_LEVEL	      2
#define SCORE_HIGH_LEVEL	      3
#define SCORE_QUEST		      4
#define SCORE_NUM_QUEST		      5



/*
 * Zombie Lord.
 */
#define ZOMBIE_NOTHING		      0
#define ZOMBIE_TRACKING		      1
#define ZOMBIE_ANIMATE		      2
#define ZOMBIE_CAST		      3
#define ZOMBIE_REST		      4



/*
 * Damcap values.
 */
#define DAM_CAP		      0
#define DAM_CHANGE	      1

/*
 * Bits for Demonic Champions.
 *
 * new demon fields in player.h and old ones in old.h
 */


/*
 * Mounts
 */
#define IS_ON_FOOT		      0
#define IS_MOUNT		      1
#define IS_RIDING		      2
#define IS_CARRIED		      4
#define IS_CARRYING		      8

//Embrace shit to fix a bug! WHEEEEEEEE!

#define ARE_NONE		      0
#define ARE_EMBRACING		      1
#define ARE_EMBRACED		      2


/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2



/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3

#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_FINAL_TURD	     16
#define OBJ_VNUM_STAKE       18

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_BLOOD_SPRING	     23

#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716
#define OBJ_VNUM_KHRONE		   30055
/* For KaVir's stuff */
#define OBJ_VNUM_SOULBLADE	  30000
#define OBJ_VNUM_PORTAL		  30001
#define OBJ_VNUM_EGG		  30002
#define OBJ_VNUM_EMPTY_EGG	  30003
#define OBJ_VNUM_SPILLED_ENTRAILS 30004
#define OBJ_VNUM_QUIVERING_BRAIN  30005
#define OBJ_VNUM_SQUIDGY_EYEBALL  30006
#define OBJ_VNUM_SPILT_BLOOD      30007
#define OBJ_VNUM_VOODOO_DOLL      30010
#define OBJ_VNUM_RIPPED_FACE      30012
#define OBJ_VNUM_TORN_WINDPIPE    30013
#define OBJ_VNUM_CRACKED_HEAD     30014
#define OBJ_VNUM_SLICED_EAR	  30025
#define OBJ_VNUM_SLICED_NOSE	  30026
#define OBJ_VNUM_KNOCKED_TOOTH	  30027
#define OBJ_VNUM_TORN_TONGUE	  30028
#define OBJ_VNUM_SEVERED_HAND	  30029
#define OBJ_VNUM_SEVERED_FOOT	  30030
#define OBJ_VNUM_SEVERED_THUMB	  30031
#define OBJ_VNUM_SEVERED_INDEX	  30032
#define OBJ_VNUM_SEVERED_MIDDLE	  30033
#define OBJ_VNUM_SEVERED_RING	  30034
#define OBJ_VNUM_SEVERED_LITTLE	  30035
#define OBJ_VNUM_SEVERED_TOE	  30036
#define OBJ_VNUM_PROTOPLASM	  30037
#define OBJ_VNUM_QUESTCARD	  30039
#define OBJ_VNUM_QUESTMACHINE	  30040
#define OBJ_VNUM_GATE		  30053
#define OBJ_VNUM_MOONGATE	  30052
#define MOB_VNUM_GUARDIAN	  30001
#define MOB_VNUM_MOUNT		  30006
#define MOB_VNUM_FROG		  30007
#define MOB_VNUM_RAVEN		  30008
#define MOB_VNUM_CAT		  30009
#define MOB_VNUM_DOG		  30010
#define MOB_VNUM_EYE		  30012
#define MOB_VNUM_STEED		  30075


/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PORTAL		     27
#define ITEM_EGG		     28
#define ITEM_VOODOO		     29
#define ITEM_STAKE		     30
#define ITEM_MISSILE		     31 /* Ammo vnum, cur, max, type */
#define ITEM_AMMO		     32 /* ???, dam min, dam max, type */
#define ITEM_QUEST		     33
#define ITEM_QUESTCARD		     34
#define ITEM_QUESTMACHINE	     35
#define ITEM_SYMBOL		     36
#define ITEM_BOOK		     37
#define ITEM_PAGE		     38
#define ITEM_TOOL		     39
#define ITEM_DTOKEN		     40
#define ITEM_WALL		     41
#define ITEM_WGATE		     42
#define ITEM_WARD		     43
#define ITEM_RESOURCE		     44
#define ITEM_CREATURE		     45
#define ITEM_STOKEN		     46
#define ITEM_DRAGONGEM               47

/*
 * Weapon flags
 * field = 'weapflags'
 * For Flaming\Icing\Shitty weapons
 */
#define WEAPON_FLAMING		      1
#define WEAPON_POISON		      2
#define WEAPON_FROST		      4

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		    (A)
#define ITEM_HUM		    (B)
#define ITEM_THROWN		    (C)
#define ITEM_KEEP		    (D)
#define ITEM_VANISH		    (E)
#define ITEM_INVIS		    (F)
#define ITEM_MAGIC		    (G)
#define ITEM_NODROP		    (H)
#define ITEM_BLESS		    (I)
#define ITEM_ANTI_GOOD		    (J)
#define ITEM_ANTI_EVIL		    (K)
#define ITEM_ANTI_NEUTRAL	    (L)
#define ITEM_NOREMOVE		    (M)
#define ITEM_INVENTORY		    (N)
#define ITEM_LOYAL		    (O)
#define ITEM_SHADOWPLANE	    (P)
#define ITEM_ARM		    (Q)
#define ITEM_MENCHANT		    (R)
/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		      1
#define ITEM_WEAR_FINGER	      2
#define ITEM_WEAR_NECK		      4
#define ITEM_WEAR_BODY		      8
#define ITEM_WEAR_HEAD		     16
#define ITEM_WEAR_LEGS		     32
#define ITEM_WEAR_FEET		     64
#define ITEM_WEAR_HANDS		    128
#define ITEM_WEAR_ARMS		    256
#define ITEM_WEAR_SHIELD	    512
#define ITEM_WEAR_ABOUT		   1024
#define ITEM_WEAR_WAIST		   2048
#define ITEM_WEAR_WRIST		   4096
#define ITEM_WIELD		   8192
#define ITEM_HOLD		  16384
#define ITEM_WEAR_FACE		  32768
#define ITEM_WEAR_SPECIAL	  65536
#define ITEM_WEAR_BODYART 	 131072
#define ITEM_WEAR_MEDAL          262144
#define ITEM_WEAR_FLOAT          524288

/*
 * Special types.
 * Used in #OBJECTS for special items - KaVir.
 */
#define SITEM_ACTIVATE		      1
#define SITEM_TWIST		      2
#define SITEM_PRESS		      4
#define SITEM_PULL		      8
#define SITEM_TARGET		      16
#define SITEM_SPELL		      32
#define SITEM_TRANSPORTER	      64
#define SITEM_TELEPORTER	      128
#define SITEM_DELAY1		      256
#define SITEM_DELAY2		      512
#define SITEM_OBJECT		     1024
#define SITEM_MOBILE		     2048
#define SITEM_ACTION		     4096
#define SITEM_MORPH		     8192
#define SITEM_SILVER		    16384
#define SITEM_WOLFWEAPON	    32768
#define SITEM_DROWWEAPON	    65536
#define SITEM_CHAMPWEAPON	   131072
#define SITEM_DEMONIC		   262144
#define SITEM_HIGHLANDER	   524288
#define SITEM_NINJA               1048576
#define SITEM_MAGE		  2097152
#define SITEM_DROW		  4194304
#define C_TWITCHING		   8388608
#define SITEM_DRAGON		  16777216

/* Immunties on items - Tracker 03/18/03 */
#define IITEM_SLASH		(A)
#define IITEM_STAB              (B)            
#define IITEM_SMASH             (C)
#define IITEM_BEAST             (D)
#define IITEM_GRAB              (E)
#define IITEM_CHARM             (F)
#define IITEM_HEAT              (G)
#define IITEM_COLD              (H)
#define IITEM_LIGHTNING         (I)
#define IITEM_ACID              (J)
#define IITEM_SLEEP             (K)
#define IITEM_DRAIN             (L)
#define IITEM_HURL              (M)
#define IITEM_BACKSTAB          (N)
#define IITEM_KICK              (O)
#define IITEM_DISARM            (P)
#define IITEM_STEAL             (Q)

/*
 * Apply types (for quest affects).
 * Used in #OBJECTS.
 */
#define QUEST_STR		      1
#define QUEST_DEX		      2
#define QUEST_INT		      4
#define QUEST_WIS		      8
#define QUEST_CON		     16
#define QUEST_HITROLL		     32
#define QUEST_DAMROLL		     64
#define QUEST_HIT		    128
#define QUEST_MANA		    256
#define QUEST_MOVE		    512
#define QUEST_AC		   1024
#define QUEST_NAME		   2048
#define QUEST_SHORT		   4096
#define QUEST_LONG		   8192
#define QUEST_FREENAME		  16384
#define QUEST_ENCHANTED		  32768
#define QUEST_SPELLPROOF	  65536
#define QUEST_ARTIFACT		 131072
#define QUEST_IMPROVED		 262144
#define QUEST_MASTER_RUNE	 524288
#define QUEST_RELIC             1048576
#define QUEST_ZOMBIE		2097152
#define QUEST_CLONED     	4194304
#define QUEST_BLOODA		8388608
#define ITEM_EQUEST	       16777216
#define QUEST_MAGEENCHANT	    (Z)
#define QUEST_MALLEABLE		   (aa)
#define QUEST_MASTERY		   (bb)
#define QUEST_PRIZE		   (cc)
#define QUEST_IMBUE		   (dd)

/*
 * Tool types.
 */
#define TOOL_PEN		      1
#define TOOL_PLIERS		      2
#define TOOL_SCALPEL		      4

/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_POLY		     25
#define APPLY_BLOOD_MAX		     26
#define APPLY_BLOOD_POT		     27

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8



/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
/*Changed by Toaster cos CHAT room is now 10*/
#define ROOM_VNUM_CHAT		     10
#define ROOM_VNUM_TEMPLE	   3001
#define ROOM_VNUM_ALTAR		   3054
#define ROOM_VNUM_SCHOOL	   3700
#define ROOM_VNUM_HELL		  30000
#define ROOM_VNUM_CRYPT		  30001
#define ROOM_VNUM_DISCONNECTION	  30002
#define ROOM_VNUM_IN_OBJECT	  30008



/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		      1
#define ROOM_NO_OTRANS		      2
#define ROOM_NO_MOB		      4
#define ROOM_INDOORS		      8
#define ROOM_SEX		     16
#define ROOM_PRIVATE		    512
#define ROOM_SAFE		   1024
#define ROOM_SOLITARY		   2048
#define ROOM_PET_SHOP		   4096
#define ROOM_NO_RECALL		   8192
#define ROOM_NO_TELEPORT	  16384
#define ROOM_TOTAL_DARKNESS       32768
#define ROOM_BLADE_BARRIER        65536
#define ROOM_ARENA               131072
#define ROOM_FLAMING             262144
#define ROOM_SILENCE             524288
#define ROOM_ASTRAL		1048576
#define ROOM_PROTOTYPE		2097152
#define ROOM_ORDER		4194304
#define	ROOM_NO_CHANT		8388608


#define ROOM2_SILENCE		      1
/*
 * Room text flags (KaVir).
 * Used in #ROOMS.
 */
#define RT_LIGHTS		      1 /* Toggles lights on/off */
#define RT_SAY			      2 /* Use this if no others powers */
#define RT_ENTER		      4
#define RT_CAST			      8
#define RT_THROWOUT		     16 /* Erm...can't remember ;) */
#define RT_OBJECT		     32 /* Creates an object */
#define RT_MOBILE		     64 /* Creates a mobile */
#define RT_LIGHT		    128 /* Lights on ONLY */
#define RT_DARK			    256 /* Lights off ONLY */
#define RT_OPEN_LIFT		    512 /* Open lift */
#define RT_CLOSE_LIFT		   1024 /* Close lift */
#define RT_MOVE_LIFT		   2048 /* Move lift */
#define RT_SPELL		   4096 /* Cast a spell */
#define RT_PORTAL		   8192 /* Creates a one-way portal */
#define RT_TELEPORT		  16384 /* Teleport player to room */

#define RT_ACTION		  32768
#define RT_BLANK_1		  65536
#define RT_BLANK_2		 131072

#define RT_RETURN		1048576 /* Perform once */
#define RT_PERSONAL		2097152 /* Only shows message to char */
#define RT_TIMER		4194304 /* Sets object timer to 1 tick */



/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5
#define MAX_DIR			      6	/* OLC */



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      1
#define EX_CLOSED		      2
#define EX_LOCKED		      4
#define EX_PICKPROOF		     8
#define EX_BASHPROOF		      16 /* KaVir*/
#define EX_BASHED		     32 /* KaVir */
#define EX_PASSPROOF		     64 /* KaVir */


/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNDERWATER			      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_MAX		     11



/*
 * Equipment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FINGER_L		      1
#define WEAR_FINGER_R		      2
#define WEAR_NECK_1		      3
#define WEAR_NECK_2		      4
#define WEAR_BODY		      5
#define WEAR_HEAD		      6
#define WEAR_LEGS		      7
#define WEAR_FEET		      8
#define WEAR_HANDS		      9
#define WEAR_ARMS		     10
#define WEAR_SHIELD		     11
#define WEAR_ABOUT		     12
#define WEAR_WAIST		     13
#define WEAR_WRIST_L		     14
#define WEAR_WRIST_R		     15
#define WEAR_WIELD		     16
#define WEAR_HOLD		     17
#define WEAR_THIRD		     18
#define WEAR_FOURTH		     19
#define WEAR_FACE		     20
#define WEAR_SCABBARD_L		     21
#define WEAR_SCABBARD_R		     22
#define WEAR_SPECIAL                 23
#define WEAR_FLOAT                   24
#define WEAR_MEDAL                   25
#define WEAR_BODYART                 26
#define MAX_WEAR		     27

/*
 * Locations for damage.
 */
#define LOC_HEAD		      0
#define LOC_BODY		      1
#define LOC_ARM_L		      2
#define LOC_ARM_R		      3
#define LOC_LEG_L		      4
#define LOC_LEG_R		      5

/*
 * For Head
 */
#define LOST_EYE_L		       1
#define LOST_EYE_R		       2
#define LOST_EAR_L		       4
#define LOST_EAR_R		       8
#define LOST_NOSE		      16
#define BROKEN_NOSE		      32
#define BROKEN_JAW		      64
#define BROKEN_SKULL		     128
#define LOST_HEAD		     256
#define LOST_TOOTH_1		     512 /* These should be added..... */
#define LOST_TOOTH_2		    1024 /* ...together to caculate... */
#define LOST_TOOTH_4		    2048 /* ...the total number of.... */
#define LOST_TOOTH_8		    4096 /* ...teeth lost.  Total..... */
#define LOST_TOOTH_16		    8192 /* ...possible is 31 teeth.   */
#define LOST_TONGUE		   16384

/*
 * For Body
 */
#define BROKEN_RIBS_1		       1 /* Remember there are a total */
#define BROKEN_RIBS_2		       2 /* of 12 pairs of ribs in the */
#define BROKEN_RIBS_4		       4 /* human body, so not all of  */
#define BROKEN_RIBS_8		       8 /* these bits should be set   */
#define BROKEN_RIBS_16		      16 /* at the same time.          */
#define BROKEN_SPINE		      32
#define BROKEN_NECK		      64
#define CUT_THROAT		     128
#define CUT_STOMACH		     256
#define CUT_CHEST		     512

/*
 * For Arms
 */
#define BROKEN_ARM		       1
#define LOST_ARM		       2
#define LOST_HAND		       4
#define LOST_FINGER_I		       8 /* Index finger */
#define LOST_FINGER_M		      16 /* Middle finger */
#define LOST_FINGER_R		      32 /* Ring finger */
#define LOST_FINGER_L		      64 /* Little finger */
#define LOST_THUMB		     128
#define BROKEN_FINGER_I		     256 /* Index finger */
#define BROKEN_FINGER_M		     512 /* Middle finger */
#define BROKEN_FINGER_R		    1024 /* Ring finger */
#define BROKEN_FINGER_L		    2048 /* Little finger */
#define BROKEN_THUMB		    4096

/*
 * For Legs
 */
#define BROKEN_LEG		       1
#define LOST_LEG		       2
#define LOST_FOOT		       4
#define LOST_TOE_A		       8
#define LOST_TOE_B		      16
#define LOST_TOE_C		      32
#define LOST_TOE_D		      64 /* Smallest toe */
#define LOST_TOE_BIG		     128
#define BROKEN_TOE_A		     256
#define BROKEN_TOE_B		     512
#define BROKEN_TOE_C		    1024
#define BROKEN_TOE_D		    2048 /* Smallest toe */
#define BROKEN_TOE_BIG		    4096

/*
 * For Bleeding
 */
#define BLEEDING_HEAD		       1
#define BLEEDING_THROAT		       2
#define BLEEDING_ARM_L		       4
#define BLEEDING_ARM_R		       8
#define BLEEDING_HAND_L		      16
#define BLEEDING_HAND_R		      32
#define BLEEDING_LEG_L		      64
#define BLEEDING_LEG_R		     128
#define BLEEDING_FOOT_L		     256
#define BLEEDING_FOOT_R		     512


/*
 * For Spec powers on players
 */
#define EYE_SPELL		       1 /* Spell when they look at you */
#define EYE_SELFACTION		       2 /* You do action when they look */
#define EYE_ACTION		       4 /* Others do action when they look */




/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2



/*
 * Stats - KaVir.
 */
#define STAT_STR		      0
#define STAT_END		      1
#define STAT_REF		      2
#define STAT_FLE		      2



/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_MEDITATING		      5
#define POS_SITTING		      6
#define POS_RESTING		      7
#define POS_FIGHTING		      8
#define POS_STANDING		      9


// Bits for Areas ((area)->bits)
#define WASTE (A)
#define UPDATEWASTE (B)
// World bits (config_info->world)
#define WIZLOCKED (A)
/*
 * ACT bits for players.
 */
#define PLR_IS_NPC		      1		/* Don't EVER set.	*/
#define PLR_DEMON		      2
#define PLR_CHAMPION		      4
#define PLR_AUTOEXIT		      8
#define PLR_AUTOLOOT		     16
#define PLR_AUTOSAC                  32
#define PLR_BLANK		     64
#define PLR_BRIEF		    128
#define PLR_COMBINE		    512
#define PLR_PROMPT		   1024
#define PLR_TELNET_GA		   2048
#define PLR_HOLYLIGHT		   4096
#define PLR_WIZINVIS		   8192
#define PLR_ANSI		  16384
#define PLR_COLOUR		  16384
#define	PLR_SILENCE		  32768
#define PLR_NO_EMOTE		  65536
#define PLR_INCOG                131072
#define PLR_NO_TELL		 262144
#define PLR_LOG			 524288
#define PLR_DENY		1048576
#define PLR_FREEZE		2097152
#define PLR_GODLESS	       16777216
#define PLR_WATCHER	       33554432
#define PLR_DROWSIGHT	       67108864
#define PLR_EMBRACING         134217728
#define PLR_EMBRACED          268435456
#define PLR_ACID              536870912
#define PLR_LAST_AGG	     1073741824

/*NEWBITS FOR PLAYERS..(added by sage)10/17/98*/
#define NEWBIE_PACK	1
#define THIRD_HAND	2
#define FOURTH_HAND	4
#define NEW_DROWHATE    8
#define NEW_DARKNESS    16
#define NEW_TIDE	32
#define NEW_COIL	64
#define NEW_MONKFLAME	128
#define NEW_MONKCLOAK   256
#define NEW_MONKADAM    512
#define NEW_MONKSKIN    1024
#define NEW_MONKFAVOR   2048
#define NEW_POWER	4096
#define NEW_CLANNABLE   8192
#define NEW_NATURAL     16384
#define NEW_QUILLS      32768
#define NEW_OBEAH	65536
#define NEW_PAIN        131072
#define NEW_REINA       262144
#define NEW_NEUTRAL     524288
#define NEW_CALM	1048576
#define NEW_NOPAIN	2097152
#define NEW_SILENCE	4194304
#define NEW_COURAGE     8388608
#define NEW_VIOLATION  16777216
#define NEW_FTALK	33554432
#define NEW_VISCERAL	67108864
#define NEW_PURGE      134217728
#define STARE_DOWN     268435456
#define TEST_CHAR      536870912
#define PLR_AUTOCON   1073741824

// exactly like newbits wheeeeee!
#define MORE_ANIMAL_MASTER 1
#define MORE_SPIRIT	2
#define MORE_GUARDIAN   4
/* Changed by Kyrlin to add rsilence 27/01/00 */
#define RSILENCE        8
#define NOFOLLOW	16
/*
 * World affect bits.
 */
#define WORLD_FOG		      1
#define WORLD_RAIN		      2
#define WORLD_ECLIPSE		      4
#define WORLD_NICE		      8 // No swearing while frenzied

/*
 * EXTRA bits for players. (KaVir)
 */
				/*    1 */
				/*    2 */
#define EXTRA_TRUSTED		      4
#define EXTRA_NEWPASS		      8
#define EXTRA_OSWITCH		     16
#define EXTRA_SWITCH		     32
#define EXTRA_FAKE_CON		     64
#define TIED_UP			    128
#define GAGGED			    256
#define BLINDFOLDED		    512
				/* 1024 */
#define EXTRA_DONE		   2048
#define EXTRA_EXP		   4096
#define EXTRA_PREGNANT		   8192
#define EXTRA_LABOUR		  16384
#define EXTRA_BORN		  32768
#define EXTRA_PROMPT		  65536
#define EXTRA_MARRIED		 131072
		        /*	 262144 */
#define FROUND			 524288

#define EXTRA_CALL_ALL		1048576
#define EXTRA_ASH	        2097152
#define EXTRA_NOTE_TRUST       4194304
#define EXTRA_STANCE           8388608
#define EXTRA_ANTI_GODLESS    16777216
#define EXTRA_ZOMBIE	      33554432
#define EXTRA_POTENCY	      67108864
#define EXTRA_AWE	     134217728
#define EXTRA_DRAGON	     268435456
#define EXTRA_PLASMA	     536870912
#define EXTRA_EARTHMELD     1073741824
#define EXTRA_ROT	    2147483648
#define EXTRA_AXE	    4294967296
#define EXTRA_CAN_SEE	    8589934592 /* You can't see, but familiar can*/

/*
 * AGE Bits.
 */
#define AGE_CHILDE		      0
#define AGE_NEONATE		      1
#define AGE_ANCILLA		      2
#define AGE_ELDER		      3
#define AGE_METHUSELAH		      4


/*
 * Stances for combat
 */
#define STANCE_NONE		     -1
#define STANCE_NORMAL		      0
#define STANCE_VIPER		      1
#define STANCE_CRANE		      2
#define STANCE_CRAB		      3
#define STANCE_MONGOOSE		      4
#define STANCE_BULL		      5
#define STANCE_MANTIS		      6
#define STANCE_DRAGON		      7
#define STANCE_TIGER		      8
#define STANCE_MONKEY		      9
#define STANCE_SWALLOW		     10
#define STANCE_RABBIT		     11
#define STANCE_COBRA		     12
#define STANCE_BEAR		     13
#define STANCE_COUGAR		     14
#define STANCE_EAGLE		     15

/*
 * Obsolete bits.
 */
#if 0
#define PLR_AUCTION		      4	/* Obsolete	*/
#define PLR_CHAT		    256	/* Obsolete	*/
#define PLR_NO_SHOUT		 131072	/* Obsolete	*/
#endif



/*
 * Channel bits.
 */
#define	CHANNEL_AUCTION		      1
#define	CHANNEL_CHAT		      2
#define	CHANNEL_SYSTEM		      4
#define	CHANNEL_IMMTALK		      8
#define	CHANNEL_MUSIC		     16
#define CHANNEL_SOCIAL               17
#define	CHANNEL_QUESTION	     32
#define	CHANNEL_SHOUT		     64
#define	CHANNEL_YELL		    128
#define	CHANNEL_VAMPTALK	    256
#define	CHANNEL_HOWL		    512
#define	CHANNEL_LOG		   1024
#define	CHANNEL_PRAY		   2048
#define	CHANNEL_INFO		   4096
#define	CHANNEL_HINT		   8192
#define	CHANNEL_TELL		  16384
#define	CHANNEL_MAGETALK          32768
#define CHANNEL_HIGHTALK	  65536
#define CHANNEL_CLANTALK	 131072
#define CHANNEL_SIGN		262144
#define CHANNEL_MIKTALK        524288
#define CHANNEL_MIKT		(CHANNEL_MIKTALK)
#define	CHANNEL_BITCH       1048576
#define	CHANNEL_MONK        2097152
#define CHANNEL_KTALK       4194304
#define CHANNEL_BUILD       8388608
#define CHANNEL_FTALK	    16777216
#define CHANNEL_PINFO       335544328

struct clan_type
{
    int		members;	/* Number of members */
    char *	clan_head;	/* Name of clan head */
    char *	primogen;	/* Name of primogen */
};


struct kingdom_data
{
	int num;
	char *name;
	char *who_name;
	char *leader_name;
	int pk;
	int pd;
	int recall;
        int altar;
        int mem;
        int exp;
};


/*
 * Used to keep track of system settings and statistics         -Toaster
 */
struct  system_data
{
sh_int      log_level;              /* Level of log channel LEVEL LOG*/
};


/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    char *              spec_name;	/* OLC */
    SHOP_DATA *		pShop;
    AREA_DATA *         area;		/* OLC */
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    char *		hunting;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		clan;
    char *		morph;
    char *		createtime;
    char *		pload;
    char *		lasttime;
    char *		lasthost;
    char *		powertype;
    char *		poweraction;
    char *		prompt;
    char *		cprompt;
    sh_int		spectype;
    sh_int		specpower;
    int		        loc_hp	[7];
    sh_int		vnum;
    sh_int		count;
    sh_int		killed;
    sh_int		sex;
    sh_int		mounted;
    sh_int		home;
    int		        level;
    int			immune;
    int			polyaff;
    int			vampaff;
    int			itemaffect;
    int			vamppass;
    int			form;
    int			fatal;
    int			act;
    int			extra;
    //int	        	more;
    int			affected_by;
    sh_int		alignment;
    sh_int		hitroll;		/* Unused */
    sh_int		ac;			/* Unused */
    sh_int		hitnodice;		/* Unused */
    sh_int		hitsizedice;		/* Unused */
    sh_int		hitplus;		/* Unused */
    sh_int		damnodice;		/* Unused */
    sh_int		damsizedice;		/* Unused */
    sh_int		damplus;		/* Unused */
    int			gold;			/* Unused */
};

struct clan_data
{
    int	kills[6];
    int deaths[6];
};

struct leader_data
{
    char name[40];
    int kills;
    int deaths;
};


struct  config_data
{
    int bonus;
    int world;
    int might;
    int catchmode;
};


struct   form_type
{
    char *	desc;
    char *	name;
    bool        claws;
    bool	fangs;
    bool	tail;
    bool	wings;
    int         hitplus;
    int		damplus;
};



/*
 * One character (PC or NPC).
 */
struct	char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *		reply;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;
    NOTE_DATA *		pnote;
    OBJ_DATA *		carrying;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    PC_DATA *		pcdata;
    char *		hunting;
    char *		name;
    char *	        pload;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		clan;
    char *            kingdom;
    CHAR_DATA *		unveil;
    char *		objdesc;
    char *		morph;
    char *		createtime;
    char *		lasttime;
    char *		lasthost;
    char *		poweraction;
    char *		powertype;
    char *		prompt;
    char *		cprompt;
    char *		prefix;
    sh_int		sex;
    sh_int		race;
    sh_int		class,class_2,class_3;
    int			immune;
    int			polyaff;
    int			vampaff_a;
    int			warp;
    sh_int		warpcount;
    int			fight_timer;
    int			obeah_timer;
    int			reina_timer;
    int			safe_timer;
    int			silence_timer;
    int			darkness_timer;
    int			arena_timer;
    int			majesty_timer;
    int			cobra_timer;
    int			wyld_timer;
    int			timers[MAX_TIMER];
    CHAR_DATA *		embraced;
    CHAR_DATA *		embracing;
    int			itemaffect;
    int			itemaffects;
    int			vamppass_a;
/*    int			vampgen;*/
    int			form;
    int			fatal;
    sh_int		vampgen_a;
    sh_int		spectype;
    sh_int		specpower;
    sh_int		loc_hp	[7];
    sh_int		wpn	[13];
    sh_int		spl	[5];
    sh_int		cmbt	[8];
    sh_int		stance	[17];
    sh_int		beast;
    sh_int		mounted;
    sh_int		home;
    sh_int		level;
    sh_int		trust;
    int			played;
    time_t		logon;
    time_t		save_time;
    sh_int		timer;
    sh_int		wait;
    int			pkill;
    int			pdeath;
    int			mkill;
    int			mdeath;
     sh_int		agg;
    long		hit;
    long		max_hit;
    long		mana;
    long		max_mana;
    long		move;
    long		max_move;
    int			gold;
    int			exp;
    int			act;
    int			extra;
    int                 newbits;
    int                 special;
    int   		more;
    int			affected_by;
    sh_int		position;
    sh_int		practice;
    sh_int		carry_weight;
    sh_int		carry_number;
    sh_int		saving_throw;
    sh_int		alignment;
    sh_int		hitroll;
    sh_int		damroll;
    sh_int		armor;
    sh_int		wimpy;
    sh_int		deaf;
    sh_int		x;
    sh_int		y;
    sh_int		room;
    sh_int		paradox		[3];
    sh_int		damcap		[2];
    int			monkstuff;
    int			monkcrap;
    int			monkab [4];
    sh_int		chi [2];
    CHAR_DATA *		blinkykill;
    CHAR_DATA *		bridgekill;
    int			flag2;
    sh_int		spheres[9];
    int			mflags;
    sh_int		quint[3];
    sh_int		imms[3];
    CHAR_DATA *		challenged;
    int			awins;
    int			alosses;
    sh_int		monkblock;
    sh_int		focus[2];
    int                 gifts[21];
    int                 garou1;
    int                 garou2;
    sh_int              gnosis[2];
    sh_int		lstatus;
    int			version;
/*  MAGIC_SHIELD *	first_shield;
    MAGIC_SHIELD *	last_shield;
Changed Toaster 05/03/00 to make 1 shield affect at time.
*/
};



/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    CHAR_DATA *		familiar;
    CHAR_DATA *		partner;
    CHAR_DATA *		propose;
    OBJ_DATA *		chobj;
    OBJ_DATA *		memorised;
    BOARD_DATA *        board; /* The current board */
    time_t              last_note[MAX_BOARD]; /* last note for the boards */
    NOTE_DATA *         in_progress;
    char *              last_decap[2];
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
    char *		titlec;
    int			wizinvis;
    char *		conception;
    char *		parents;
    char *		cparents;
    char *		marriage;
    char *		email;
/*
    char *		mdecap;
*/
     char *		forget[MAX_FORGET];
     char *		love;
    sh_int		perm_str;
    sh_int		perm_int;
    sh_int		perm_wis;
    sh_int		perm_dex;
    sh_int		perm_con;
    sh_int		mod_str;
    sh_int		mod_int;
    sh_int		mod_wis;
    sh_int		mod_dex;
    sh_int		mod_con;
    sh_int		willpower[2];
    sh_int		resist[5];
    sh_int		atm;
    int			quest;
    int 		kingdom;
    int			bounty;
    int			powers[20];
    int                 powerss[20];
    int			powersm[20];
    int			powersd[20];
    int			powersde[20];
    int			powerst[20];
    int			principles[20]; // New Ninja Principles - Simian 9/6/2001
    char *              ns_target; // Ninja Scroll Target - Simian 24/07/2001
    int                 ns_plan[10]; // Ninja Scroll Plan Data - Simian 24/07/2001
    int 		stats[12];
    int			dragonaff;
    int			dragonage;
    int			hatch;
    char *		breath[5];
    sh_int		wolf;
    sh_int		rank;
    sh_int		demonic_a;
    sh_int		kith;
    int			language[2];
    sh_int		stage[3];
    sh_int		wolfform[2];
    int			score[6];
    sh_int		runes[4];
    sh_int		disc_a[11];
    int			genes[10];
    sh_int		fake_skill;
    sh_int		fake_stance;
    sh_int		fake_hit;
    sh_int		fake_dam;
    sh_int		fake_hp;
    sh_int		fake_mana;
    sh_int		fake_move;
    int			fake_ac;
    sh_int		obj_vnum;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int		stat_ability	[4];
    sh_int		stat_amount	[4];
    sh_int		stat_duration	[4];
    sh_int		exhaustion;
    sh_int              followers;
    sh_int		tick_timer[MAX_TIMER];
    int                 security;	/* OLC */
    sh_int		autostance;
    sh_int              pagelen;
    long                imc_deaf;    /* IMC channel def flags */
    long                imc_allow;   /* IMC channel allow flags */
    long                imc_deny;    /* IMC channel deny flags */
    char *              rreply;      /* IMC reply-to */
    char *              rreply_name; /* IMC reply-to shown to char */
    char *              ice_listen;  /* ICE channels */
    int 		war_bits;
};





/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		16

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};



/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    AREA_DATA *         area;	/* OLC */
    char *		name;
    char *		short_descr;
    char *		description;
    char *		real_name;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    sh_int		vnum;
    int			item_type;
    sh_int		extra_flags;
    sh_int		wear_flags;
    sh_int		count;
    sh_int		weight;
    int			spectype;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int			quest;
    sh_int		points;
    int			cost;			/* Unused */
    int			value [4];
};



/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    CHAR_DATA *		carried_by;
    CHAR_DATA *		chobj;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    char *		name;
    char *		short_descr;
    char *		real_name;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    sh_int		item_type;
    sh_int		extra_flags;
    sh_int		wear_flags;
    sh_int		wear_loc;
    sh_int		weight;
    int			spectype;
    int			immune;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int			quest;
    sh_int		points;
    int			cost;
    sh_int		level;
    sh_int		timer;
    int			value	[4];
    int			weapflags;
};



/*
 * Exit data.
 */
struct	exit_data
{
    ROOM_INDEX_DATA *	to_room;
    EXIT_DATA *         next;	/* OLC */
    sh_int		vnum;
    sh_int		exit_info;
    sh_int		key;
    char *		keyword;
    char *		description;
    int			rs_flags;	/* OLC */
};



/*
 * Room text checking data.
 */
typedef struct roomtext_data
{
    int				type;
    int				power;
    int				mob;
    char *			input;
    char *			output;
    char *			choutput;
    char *			name;
    struct roomtext_data	*next;
} ROOMTEXT_DATA;



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    sh_int		arg1;
    sh_int		arg2;
    sh_int		arg3;
};



/*
 * Area definition.
 */
struct	area_data
{
    AREA_DATA *		next;
    RESET_DATA *	reset_first;
    RESET_DATA *	reset_last;
    char *		name;
    sh_int		age;
    sh_int		x;
    sh_int		y;
    sh_int		start;
    sh_int		nplayer;
    char *		filename;	/* OLC */
    char *		builders;	/* OLC */ /* Listing of */
    int			security;	/* OLC */ /* Value 1-9  */
    int			lvnum;		/* OLC */ /* Lower vnum */
    int			uvnum;		/* OLC */ /* Upper vnum */
    int			vnum;		/* OLC */ /* Area vnum  */
    int			area_flags;	/* OLC */
    int			bits;
};


/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    CHAR_DATA *		people;
    OBJ_DATA *		contents;
    OBJ_DATA *		to_obj;
    EXTRA_DESCR_DATA *	extra_descr;
    AREA_DATA *		area;
    EXIT_DATA *		exit	  [6];
    ROOMTEXT_DATA *	roomtext;
    char *		track     [5];
    char *		name;
    char *		description;
    sh_int		vnum;
    sh_int		room_flags;
    int			added_flags;
    sh_int		light;
    sh_int		blood;
    sh_int		passed;
    sh_int		track_dir [5];
    sh_int		sector_type;
    sh_int		wood;
    sh_int		build;
    sh_int		ground;
    sh_int		x;
    sh_int		y;
    sh_int		room;
};


/* Magic Shield Types, Toaster 05/03/00*/
#define SHIELD_NONE     0
#define FLAME_SHIELD    1
#define SHOCK_SHIELD    2
#define CHAOS_SHIELD    3
#define ICE_SHIELD      4
#define ACID_SHIELD     5
#define DARK_SHIELD	6

struct magic_shield
{
    bool        is_free; /* Ramias:for run-time checks of LINK/UNLINK */
  MAGIC_SHIELD *next;
  MAGIC_SHIELD *prev;
  sh_int        type;           /* what kind is it? Electric, Fire, etc...*/
  bool          harmfull;       /* does the shield damage the attacker? */
  sh_int        attack_dam;     /* then hurt the attacker ;) */
  sh_int        percent;        /* percent of damage it absorbs per attack*/
  sh_int        hits;           /* Shield hitpoints */
  int           sn;
  char          * absorb_message_room;
  char          * absorb_message_victim;
  char          * absorb_message_self;
  char          * name;
  char          * wearoff_room;
  char          * wearoff_self;

};





/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000
#define TYPE_ADD_AGG                 3000
#define TYPE_AGG                     3000


/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4

#define TARGET_CHAR                 0
#define TARGET_OBJ                  1
#define TARGET_ROOM                 2
#define TARGET_NONE                 3

#define PURPLE_MAGIC		    0
#define RED_MAGIC		    1
#define BLUE_MAGIC		    2
#define GREEN_MAGIC		    3
#define YELLOW_MAGIC		    4



/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    sh_int	skill_level;	        /* Level needed by class	*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	slot;			/* Slot for #OBJECT loading	*/
    sh_int	min_mana;		/* Minimum mana used		*/
    sh_int	beats;			/* Waiting time after use	*/
    char *	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
};


/*
 * These are skill_lookup return values for common skills and spells.
 */
extern	sh_int	gsn_backstab;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_slow;
extern	sh_int	gsn_haste;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern  sh_int  gsn_inferno;
extern  sh_int  gsn_blinky;
extern	sh_int	gsn_fastdraw;
extern	sh_int	gsn_berserk;
extern	sh_int	gsn_punch;
extern	sh_int	gsn_elbow;
extern  sh_int  gsn_fangs;
extern  sh_int  gsn_rfangs;
extern  sh_int  gsn_spiket;
extern  sh_int  gsn_venomt;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_waspt;
extern  sh_int	gsn_claws;
extern  sh_int  gsn_shards;
extern	sh_int	gsn_headbutt;
extern  sh_int  gsn_quills;
extern  sh_int  gsn_buffet;
extern  sh_int  gsn_tail;
extern  sh_int  gsn_shiroken;
extern  sh_int  gsn_riposte;
extern  sh_int  gsn_joust;
extern	sh_int	gsn_sweep;
extern	sh_int	gsn_knee;
extern  sh_int  gsn_spinkick;
extern  sh_int  gsn_backfist;
extern  sh_int  gsn_jumpkick;
extern  sh_int  gsn_monksweep;
extern  sh_int  gsn_thrustkick;
extern  sh_int  gsn_spinkick;
extern  sh_int  gsn_elbow;
extern  sh_int  gsn_palmstrike;
extern  sh_int  gsn_shinkick;
extern  sh_int  gsn_lightningkick;
extern  sh_int  gsn_tornadokick;
extern  sh_int  gsn_spiderform;
extern  sh_int  gsn_garotte;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_hurl;
extern	sh_int	gsn_kick;
extern  sh_int  gsn_dfirebreath;
extern  sh_int  gsn_dfrostbreath;
extern  sh_int  gsn_dacidbreath;
extern  sh_int  gsn_dgasbreath;
extern  sh_int  gsn_dlightningbreath;
extern  sh_int  gsn_circle;
extern	sh_int	gsn_talon;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_track;
extern	sh_int	gsn_polymorph;
extern	sh_int	gsn_web;
extern sh_int	gsn_drowfire;
extern  sh_int	gsn_godbless;  /* Vic - Monks */
extern  sh_int  gsn_llothbless;
extern  sh_int	gsn_totalblind; /* Vic - Monks */
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_mass_invis;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;

extern	sh_int	gsn_paradox;
extern	sh_int	gsn_darkness;


/*
 * Utility macros.
 */
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))	/* OLC */
#define STR(dat, field)         (( (dat)->field != NULL                    \
                                             ? (dat)->field                \
                                             : (dat)->pIndexData->field ))	/* OLC */
                                /*
#define	FILL_STRING( string, text )					\
				if ( text != NULL )			\
				{					\
				    smash_tilde( text );		\
				    free_string( string );		\
				    string = str_dup( text );		\
				}					\
				else					\
				{					\
				    free_string( string );		\
				    string = str_dup( "(NULL-BUGGED)" );\
				}
				*/

#define IS_LEADER(ch)		(!str_cmp((ch)->name, leader[0]->name) || !str_cmp((ch)->name, leader[1]->name) || !str_cmp((ch)->name, leader[2]->name) )
#define IS_FODDER(ch)		(!str_cmp((ch)->name, leader[3]->name) || !str_cmp((ch)->name, leader[4]->name) || !str_cmp((ch)->name, leader[5]->name) )
#define IS_WEAP(obj,stat)	(IS_SET((obj)->weapflags, (stat)))

/*
 * Character macros.
 */
#define IS_BLACK_SPIRAL_DANCER(ch) ((ch)->class == CLASS_BLACK_SPIRAL_DANCER)
#define IS_ABOMINATION(ch)	((ch)->class == CLASS_ABOMINATION)
#define IS_VAMPIRE(ch)		( (ch)->class == CLASS_VAMPIRE || (ch)->class_2 == CLASS_VAMPIRE || (ch)->class_3 == CLASS_VAMPIRE)
#define IS_WEREWOLF(ch)		((ch)->class == CLASS_WEREWOLF || (ch)->class_2 == CLASS_WEREWOLF || (ch)->class_3 == CLASS_WEREWOLF)
#define IS_MAGE(ch)		((ch)->class == CLASS_MAGE || (ch)->class_2 == CLASS_MAGE || (ch)->class_3 == CLASS_MAGE)
#define IS_DEMON(ch)		((ch)->class == CLASS_DEMON || (ch)->class_2 == CLASS_DEMON || (ch)->class_3 == CLASS_DEMON)
#define IS_NINJA(ch)		((ch)->class == CLASS_NINJA || (ch)->class_2 == CLASS_NINJA || (ch)->class_3 == CLASS_NINJA)
#define IS_HUNTER(ch)		((ch)->class == CLASS_HUNTER)
#define IS_SHAKRIN(ch)		((ch)->class == CLASS_SHAKRIN)
#define IS_HIGHLANDER(ch)	((ch)->class == CLASS_HIGHLANDER || (ch)->class_2 == CLASS_HIGHLANDER || (ch)->class_3 == CLASS_HIGHLANDER)
#define IS_WRAITH(ch)		((ch)->class == CLASS_WRAITH)
#define IS_MONK(ch)		((ch)->class == CLASS_MONK || (ch)->class_2 == CLASS_MONK || (ch)->class_3 == CLASS_MONK)
#define IS_FAE(ch)		((ch)->class == CLASS_FAE)
#define IS_DEATHKNIGHT(ch)      ((ch)->class == CLASS_DEATHKNIGHT)
#define IS_SPIRIT(ch)		(IS_SET((ch)->more, MORE_SPIRIT))
#define IS_DROW(ch)		(IS_SET((ch)->class, CLASS_DROW) || (ch)->class_2 == CLASS_DROW || (ch)->class_3 == CLASS_DROW)
#define IS_CREATOR(ch)		(!str_cmp((ch)->name, "Tracker" ))
#define IS_COMB(ch, sn)		(IS_SET((ch)->monkcrap, (sn)))
#define IS_FS(ch, sn)		(IS_SET((ch)->monkstuff, (sn)))
#define IS_NEWFLAG(ch, sn)      (IS_SET((ch)->flag2, (sn)))
#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_JUDGE(ch)		(get_trust(ch) >= LEVEL_JUDGE)
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)		(get_trust(ch) >= LEVEL_HERO)
#define IN_TROOM(ch)		(ch->in_room->vnum == 3100 && ch->mkill == 1)
#define CAN_PK(ch)		(get_trust(ch) >= 3 && get_trust(ch) <= 6)
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
#define LINK(link, first, last, next, prev)
#define UNLINK(link, first, last, next, prev)
#define IS_SPEAKING(ch, sn)	(IS_SET((ch)->pcdata->language[0], (sn)))
#define CAN_SPEAK(ch, sn)	(IS_SET((ch)->pcdata->language[1], (sn)))
#define CAN_DANCE(ch, sn)	(IS_SET((ch)->pcdata->powers[1], (sn)))
#define IS_ITEMAFF(ch, sn)	(IS_SET((ch)->itemaffect, (sn)))
#define IS_ITEMAFFS(ch, sn)     (IS_SET((ch)->itemaffects, (sn)))
#define IS_WILLPOWER(ch, sn)	(IS_SET((ch)->pcdata->resist[0], (sn)))
#define IS_IMMUNE(ch, sn)	(IS_SET((ch)->immune, (sn)))
#define IS_VAMPAFF(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_AFF], (sn)))
#define IS_VAMPPASS(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_CURRENT], (sn)))
#define IS_FORM(ch, sn)		(IS_SET((ch)->form, (sn)))
#define IS_POLYAFF(ch, sn)	(IS_SET((ch)->polyaff, (sn)))
#define IS_EXTRA(ch, sn)	(IS_SET((ch)->extra, (sn)))
#define IS_MORE(ch, sn)		(IS_SET((ch)->more, (sn)))
#define IS_STANCE(ch, sn)	(ch->stance[0] == sn)
#define IS_DEMPOWER(ch, sn)	(IS_SET((ch)->pcdata->powers[DPOWER_FLAGS], (sn)))
#define IS_DRAPOWER(ch, sn)     (IS_SET((ch)->pcdata->dragonaff, (sn)))
#define IS_DEMAFF(ch, sn)	(IS_SET((ch)->pcdata->powers[DPOWER_CURRENT], (sn)))
#define IS_MPOWER(ch, sn)	(IS_SET((ch)->pcdata->powers[MPOWER_POWER], (sn)))
#define IS_PPOWER(ch, sn)	(IS_SET((ch)->pcdata->powers[PPOWER_FLAGS], (sn)))  /* Paladin - Loki */
#define IS_PAFF(ch, sn)	(IS_SET((ch)->pcdata->powers[PPOWER_CURRENT], (sn)))   /* Paladin - Loki */
//#define IS_CLASS(ch, CLASS)	(IS_SET((ch)->class, CLASS))
#define IS_CLASS(ch, CLASS)	( (ch)->class == CLASS || (ch)->class_2 == CLASS || (ch)->class_3 == CLASS)
#define IS_HEAD(ch, sn)		(IS_SET((ch)->loc_hp[0], (sn)))
#define IS_BODY(ch, sn)		(IS_SET((ch)->loc_hp[1], (sn)))
#define IS_ARM_L(ch, sn)	(IS_SET((ch)->loc_hp[2], (sn)))
#define IS_ARM_R(ch, sn)	(IS_SET((ch)->loc_hp[3], (sn)))
#define IS_LEG_L(ch, sn)	(IS_SET((ch)->loc_hp[4], (sn)))
#define IS_LEG_R(ch, sn)	(IS_SET((ch)->loc_hp[5], (sn)))
#define IS_BLEEDING(ch, sn)	(IS_SET((ch)->loc_hp[6], (sn)))

#define IN_CLAN( ch )           (IS_NPC(ch)?0:ch->clan)
#define IS_PLAYING( d )         (d->connected==CON_PLAYING)

#define IS_GOOD(ch)		(ch->alignment >= 350)
#define IS_EVIL(ch)		(ch->alignment <= -350)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define PC(ch,field)            ((ch)->pcdata->field)	/* OLC */
#define GET_PC(ch,field,nopc)   ((ch->pcdata != NULL ? ch->pcdata->field    \
                                                     : (nopc)) )	/* OLC */
#define GET_AC(ch)		((ch)->armor				    \
				    + ( IS_AWAKE(ch)			    \
				    ? dex_app[get_curr_dex(ch)].defensive   \
				    : 0 ))
#define GET_HITROLL(ch)		((ch)->hitroll+str_app[get_curr_str(ch)].tohit)
#define GET_DAMROLL(ch)		((ch)->damroll+str_app[get_curr_str(ch)].todam)

#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->room_flags,		    \
				    ROOM_INDOORS))


#define WAIT_STATE(ch, npulse)	((ch)->wait = UMAX((ch)->wait, (npulse)))



/* New timers command thingie to replace waitstate - Simian 25/06/2001 */

int delayPulse;

#define DELAY(ch, delayPulse)  ((ch)->timers[TIMER_GEN] = UMAX((ch)->timers[TIMER_GEN], (delayPulse)))

/* End new timers code */

#define CH(descriptor)  ((descriptor)->original ? \
(descriptor)->original : (descriptor)->character)

/*
 * Object Macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))



/*
 * Description macros.
 */


#define PERS(ch, looker)        ( can_see( looker, (ch) ) ?             \
                                ( IS_NPC(ch) ? (ch)->short_descr        \
                                : ( IS_AFFECTED( (ch), AFF_POLYMORPH) ?   \
                                (ch)->morph : (ch)->name ) )            \
                                : (IS_IMMORTAL(ch) ? "An Immortal" :    \
                                "Someone" ) )
/*
#define PERS(ch, looker)	( can_see( looker, (ch) ) ?		\
				( IS_NPC(ch) ? (ch)->short_descr	\
				: ( IS_AFFECTED( (ch), AFF_POLYMORPH) ?   \
				(ch)->morph : (ch)->name ) )		\
				: "someone" )


#define PERS(ch, looker)  ( can_see( (looker), (ch) ) ?         \
                                ( IS_NPC(ch) ? (ch)->short_descr        \
                                : ( IS_AFFECTED( (ch), AFF_POLYMORPH) ?   \
                                (ch)->morph : (ch)->name ) )            \
                                : ( IS_IMMORTAL(ch) ? "An Immortal" :  \
                                     "Someone" ) )
*/




/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    sh_int		level;
    sh_int		log;
};

/*
 * Bits for Demonic Armour.
 */
#define ARM_BLACK                     0
#define ARM_RED                       1
#define ARM_COPPER                    2
#define ARM_BRONZE                    3
#define ARM_BRASS                     4





/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
};


/*
 * Structure for an X-social in the socials table.
 */
struct	xsocial_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
    sh_int		gender;
    sh_int		stage;
    sh_int		position;
    sh_int		self;
    sh_int		other;
    sh_int              extra;
    bool		chance;
};

/*
 * Global constants.
 */
extern	const	struct	str_app_type	str_app		[26];
extern	const	struct	int_app_type	int_app		[26];
extern	const	struct	wis_app_type	wis_app		[26];
extern	const	struct	dex_app_type	dex_app		[26];
extern	const	struct	con_app_type	con_app		[26];

extern	const	struct	cmd_type	cmd_table	[];
extern	const	struct	liq_type	liq_table	[LIQ_MAX];
extern	const	struct	skill_type	skill_table	[MAX_SKILL];
extern		struct	artifact_type	artifact_table	[MAX_ART];
extern	const	struct	social_type	social_table	[];
extern	const	struct	xsocial_type	xsocial_table	[];
extern  const   struct  dage_type	dage_table	[];
extern  const   char              *     sphere_name     [9];
extern  const   char		  *     dcolor		[10];
extern  const   char		  *     dcolor2		[10];
extern  const   struct  form_type	form_data       [MAX_FORMS];
extern  const   char		  *     transformation_message [7];
extern	char *	const	dir_name        [];	/* OLC */
extern	const	int	rev_dir         [];	/* OLC */
extern	const	struct	spec_type	spec_table	[];
extern	char *	const	clan_table	[];		/* List of clans*/
/*
 * Global variables.
 */
extern		HELP_DATA	  *	help_first;
extern		SHOP_DATA	  *	shop_first;

extern		BAN_DATA	  *	ban_list;
extern		CHAR_DATA	  *	char_list;
extern		DESCRIPTOR_DATA   *	descriptor_list;
extern		NOTE_DATA	  *	note_list;
extern		OBJ_DATA	  *	object_list;
extern		LEADER_DATA	  *     leader[6];
extern		AFFECT_DATA	  *	affect_free;
extern		BAN_DATA	  *	ban_free;
extern		CHAR_DATA	  *	char_free;
extern		DESCRIPTOR_DATA	  *	descriptor_free;
extern		EXTRA_DESCR_DATA  *	extra_descr_free;
extern		ROOMTEXT_DATA     *	roomtext_free;
extern		NOTE_DATA	  *	note_free;
extern		OBJ_DATA	  *	obj_free;
extern		PC_DATA		  *	pcdata_free;
extern		CONFIG_DATA	  *     config_info;
extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern		FILE *			fpReserve;
extern		KILL_DATA		kill_table	[];
extern		char			log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA		weather_info;
extern          int			arena;
extern          CLAN_DATA         *     clans_data [MAX_CLAN];
/*
extern          int                     debug_counter;
extern          char              *     debug_procedure[20];
extern          char              *     debug_last_character;
extern          int                     debug_last_room;
extern          int                     debug_stage;
extern          char              *     debug_last_command;
extern          bool                    debug_dont_run;
*/
/*
 * online creation externals  OLC
 */
extern          AREA_DATA *             area_first;
extern          AREA_DATA *             area_last;
extern  	SHOP_DATA *             shop_last;

extern          int                     top_affect;
extern          int                     top_area;
extern          int                     top_ed;
extern          int                     top_exit;
extern          int                     top_help;
extern          int                     top_mob_index;
extern          int                     top_obj_index;
extern          int                     top_reset;
extern          int                     top_room;
extern          int                     top_shop;

extern          int                     top_vnum_mob;
extern          int                     top_vnum_obj;
extern          int                     top_vnum_room;

extern          char                    str_empty       [1];

extern  MOB_INDEX_DATA *        mob_index_hash  [MAX_KEY_HASH];
extern  OBJ_INDEX_DATA *        obj_index_hash  [MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *       room_index_hash [MAX_KEY_HASH];


//config.c
void save_config args(( void ));

/* LOW_MISC.c */
void alter_corpse(OBJ_DATA *obj);
void mageupkeep(CHAR_DATA *ch);
void monkupkeep(CHAR_DATA *ch);
void drowupkeep(CHAR_DATA *ch);
void wwupkeep(CHAR_DATA *ch);

/*
 * New Imm Commands
 * LOW_WIZ.c
 */
DECLARE_DO_FUN( do_paralyzing	);
DECLARE_DO_FUN( do_login	);
DECLARE_DO_FUN( do_pstat	);
DECLARE_DO_FUN( do_waitplayer	);
DECLARE_DO_FUN( do_pshow	);
DECLARE_DO_FUN( do_makepreg	);
DECLARE_DO_FUN( do_topxp	);
DECLARE_DO_FUN( do_reimb	);
DECLARE_DO_FUN( do_resetpassword);
DECLARE_DO_FUN( do_lord		);
DECLARE_DO_FUN( do_numlock	);
DECLARE_DO_FUN( do_newlock		);
DECLARE_DO_FUN( do_renamep	);
DECLARE_DO_FUN( do_dismember	);
DECLARE_DO_FUN( do_dele		);
DECLARE_DO_FUN( do_delete	);
DECLARE_DO_FUN( do_rolldirt	);
DECLARE_DO_FUN( do_version	);
DECLARE_DO_FUN( test_crap	);
//DECLARE_DO_FUN( do_katana	);
DECLARE_DO_FUN( do_forge	);
DECLARE_DO_FUN( do_blitzattack	);
DECLARE_DO_FUN( do_stare        );
DECLARE_DO_FUN( do_sixthsense   );
/*
 * Warlock Functions.
 * Warlock.c
 */
DECLARE_DO_FUN( do_bountyz	);
DECLARE_DO_FUN( do_bountylist   );
DECLARE_DO_FUN( do_banshee	);
DECLARE_DO_FUN( do_dimmak	);
DECLARE_DO_FUN( do_renew	);
DECLARE_DO_FUN( do_neutral	);
DECLARE_DO_FUN( do_anesthetic	);
DECLARE_DO_FUN( do_panacea	);
DECLARE_DO_FUN( do_obeah	);
DECLARE_DO_FUN( do_magi		);
DECLARE_DO_FUN( do_obtain	);
DECLARE_DO_FUN( do_redeem	);
DECLARE_DO_FUN( do_clearwarps	);
DECLARE_DO_FUN( do_warps	);
DECLARE_DO_FUN( do_wyld		);
DECLARE_DO_FUN( do_bridgegate	);
DECLARE_DO_FUN( do_eyeofcobra	);
DECLARE_DO_FUN( do_brew		);
DECLARE_DO_FUN( do_testdragon	  );
DECLARE_DO_FUN( do_testscore     );
DECLARE_DO_FUN( do_phase	  );
DECLARE_DO_FUN( do_spheres	  );
DECLARE_DO_FUN( do_steed	  );
DECLARE_DO_FUN( do_perception     );
DECLARE_DO_FUN( do_objectgate	  );
DECLARE_DO_FUN( do_mgate	  );
DECLARE_DO_FUN( do_polyappear	  );
DECLARE_DO_FUN( do_discharge	  );
DECLARE_DO_FUN( do_delevel	  );
DECLARE_DO_FUN( do_telekinetics   );
DECLARE_DO_FUN( do_call_lightning );
DECLARE_DO_FUN( do_controlweather );
DECLARE_DO_FUN( do_earthembrace	  );
DECLARE_DO_FUN( do_tempest	  );
DECLARE_DO_FUN( do_slowtime	  );
DECLARE_DO_FUN( do_speedtime	  );
DECLARE_DO_FUN( do_quintessence	  );
DECLARE_DO_FUN( do_enchant	  );
DECLARE_DO_FUN( do_imbue	  );
DECLARE_DO_FUN( do_rubbing	  );
DECLARE_DO_FUN( do_purification   );
DECLARE_DO_FUN( do_qblast	  );
DECLARE_DO_FUN( do_hotienchi	  );
DECLARE_DO_FUN(	do_fetishdoll	);
DECLARE_DO_FUN( do_littledeath    );
DECLARE_DO_FUN( do_love           );
DECLARE_DO_FUN( do_courage	);
DECLARE_DO_FUN( do_reina	);
DECLARE_DO_FUN( do_willpower      );
DECLARE_DO_FUN( do_breath	  );
DECLARE_DO_FUN( do_dbreath	);
DECLARE_DO_FUN( do_betterbody     );
DECLARE_DO_FUN( do_bounty	  );
DECLARE_DO_FUN( do_shapechange    );
DECLARE_DO_FUN( do_shroudnight	);
DECLARE_DO_FUN( do_layhands	  );
DECLARE_DO_FUN( do_analyze	  );
DECLARE_DO_FUN( do_alterstate	  );
DECLARE_DO_FUN( do_alterweight	  );
DECLARE_DO_FUN( do_enchantarmor   );
DECLARE_DO_FUN( do_hover	  );
DECLARE_DO_FUN( do_axe		  );
DECLARE_DO_FUN( do_mshadow	  );

DECLARE_DO_FUN( do_callspirit	  );
DECLARE_DO_FUN( do_spiritkiss	  );
DECLARE_DO_FUN( do_spiritblast	  );
DECLARE_DO_FUN( do_breach	  );
DECLARE_DO_FUN( do_rsilence       );
DECLARE_DO_FUN( do_gifts	  );
DECLARE_DO_FUN( do_giftreset	  );
DECLARE_DO_FUN( do_balorsgaze	);
DECLARE_DO_FUN( do_giant	);
DECLARE_DO_FUN( do_giftset		);
DECLARE_DO_FUN( do_discset	);
DECLARE_DO_FUN( do_sphereset    );
DECLARE_DO_FUN(do_mizotamouyo	);
DECLARE_DO_FUN(do_trip	);
DECLARE_DO_FUN(do_orimasu	);
DECLARE_DO_FUN(do_gunaya	);
DECLARE_DO_FUN( do_challenge	  );
DECLARE_DO_FUN( do_arenaagree	  );
DECLARE_DO_FUN( do_decline	  );
DECLARE_DO_FUN( do_configuration  );

// Garou Stuff
DECLARE_DO_FUN( do_staredown );
DECLARE_DO_FUN( do_disquiet  );
DECLARE_DO_FUN( do_reshape   );
DECLARE_DO_FUN( do_cocoon    );
DECLARE_DO_FUN( do_quills    );
DECLARE_DO_FUN( do_burrow    );
DECLARE_DO_FUN( do_nightsight);
DECLARE_DO_FUN( do_wither    );
DECLARE_DO_FUN( do_totemgift );
DECLARE_DO_FUN( do_heightened);
DECLARE_DO_FUN( do_gnaw      );
DECLARE_DO_FUN( do_elementalgift);
DECLARE_DO_FUN( do_songbeast );
DECLARE_DO_FUN( do_blissful  );
DECLARE_DO_FUN( do_lunasblessing);
DECLARE_DO_FUN( do_whelp     );
DECLARE_DO_FUN( do_chillfrost);
DECLARE_DO_FUN( do_heartofice);
DECLARE_DO_FUN( do_cannibalspirit);
DECLARE_DO_FUN( do_cuttingwind);
DECLARE_DO_FUN( do_invisibility);
DECLARE_DO_FUN( do_ignore);
DECLARE_DO_FUN( do_unignore);
DECLARE_DO_FUN( do_icychill	);
DECLARE_DO_FUN( do_flamespirit);
DECLARE_DO_FUN( do_darkshroud);
DECLARE_DO_FUN( do_merciful);
DECLARE_DO_FUN( do_sightfrombeyond);
DECLARE_DO_FUN( do_paws);
DECLARE_DO_FUN( do_lunasavenger);
DECLARE_DO_FUN( do_gatemoon);
DECLARE_DO_FUN( do_clapofthunder);
DECLARE_DO_FUN( do_vengeance);
DECLARE_DO_FUN( do_violation);
DECLARE_DO_FUN( do_barrage);
DECLARE_DO_FUN( do_avalanche);
DECLARE_DO_FUN( do_snarl);
DECLARE_DO_FUN( do_livingwood);
DECLARE_DO_FUN( do_garouserenity);
DECLARE_DO_FUN( do_dazzle);
DECLARE_DO_FUN( do_bodywrack);
DECLARE_DO_FUN( do_aeolus);
DECLARE_DO_FUN( do_visceral);
DECLARE_DO_FUN( do_clenched);
DECLARE_DO_FUN( do_wallofgranite);
DECLARE_DO_FUN( do_razorclaws);
DECLARE_DO_FUN( do_malleable);
DECLARE_DO_FUN( do_feral);
DECLARE_DO_FUN( do_spiritdrain);
DECLARE_DO_FUN( do_mothers);
DECLARE_DO_FUN( do_madness);
/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */

DECLARE_DO_FUN( do_edges	); /* Hunter commands */
DECLARE_DO_FUN( do_imbued	); /* Hunter commands */
DECLARE_DO_FUN( do_ihide        ); /* Hunter commands */
DECLARE_DO_FUN( do_torture      ); /* Hunter commands */
DECLARE_DO_FUN( do_project   );
DECLARE_DO_FUN( do_entomb          ); /* Demon command */
DECLARE_DO_FUN( do_dimp          ); /* Demon command */
DECLARE_DO_FUN( do_dimpsend      ); /* Demon command */

DECLARE_DO_FUN( do_hiscoset	);
DECLARE_DO_FUN( do_hiscore	);
DECLARE_DO_FUN( do_leaders	);
DECLARE_DO_FUN( do_racepower	);
DECLARE_DO_FUN( do_immolate	);
DECLARE_DO_FUN( do_wfreeze	);
DECLARE_DO_FUN( do_caust	);
DECLARE_DO_FUN( do_blink	);
DECLARE_DO_FUN( do_inferno	);
DECLARE_DO_FUN( do_leech	);
DECLARE_DO_FUN( do_unnerve	);

DECLARE_DO_FUN(	do_accept	);
DECLARE_DO_FUN(	do_activate	);
DECLARE_DO_FUN(	do_alignment	);
DECLARE_DO_FUN(	do_allow	);
DECLARE_DO_FUN( do_cloak	);
DECLARE_DO_FUN( do_almightyfavour );
DECLARE_DO_FUN( do_prayer       );
DECLARE_DO_FUN(	do_anarch	);
DECLARE_DO_FUN(	do_ansi		);
DECLARE_DO_FUN(	do_answer	);
DECLARE_DO_FUN(	do_areas	);
DECLARE_DO_FUN(	do_artifact	);
DECLARE_DO_FUN( do_priceless	);
DECLARE_DO_FUN(	do_at		);
DECLARE_DO_FUN(	do_auction	);
DECLARE_DO_FUN( do_aura     ); /* Loki */
DECLARE_DO_FUN(	do_autoexit	);
DECLARE_DO_FUN(	do_autoloot	);
DECLARE_DO_FUN(	do_autosac	);
DECLARE_DO_FUN(	do_autosave	);
DECLARE_DO_FUN( do_autostance);
DECLARE_DO_FUN(	do_backstab	);
DECLARE_DO_FUN(	do_bamfin	);
DECLARE_DO_FUN(	do_bamfout	);
DECLARE_DO_FUN(	do_ban		);
DECLARE_DO_FUN(	do_beastlike	);
DECLARE_DO_FUN(	do_berserk	);
DECLARE_DO_FUN(	do_bind		);
DECLARE_DO_FUN(	do_birth	);
DECLARE_DO_FUN(	do_bite		);
DECLARE_DO_FUN(	do_blank	);
DECLARE_DO_FUN(	do_blindfold	);
DECLARE_DO_FUN(	do_bloodline	);
DECLARE_DO_FUN( do_possession	);
DECLARE_DO_FUN( do_bomuzite );
DECLARE_DO_FUN(	do_brandish	);
DECLARE_DO_FUN(	do_breakup	);
DECLARE_DO_FUN(	do_brief	);
DECLARE_DO_FUN(	do_bug		);
DECLARE_DO_FUN( do_burrow       );
DECLARE_DO_FUN(	do_buy		);
DECLARE_DO_FUN(	do_call		);
DECLARE_DO_FUN(	do_calm		);
DECLARE_DO_FUN(	do_cast		);
DECLARE_DO_FUN( do_celestialpath );
DECLARE_DO_FUN(	do_champions	);
DECLARE_DO_FUN(	do_change	);
DECLARE_DO_FUN(	do_changelight	);
DECLARE_DO_FUN(	do_channels	);
DECLARE_DO_FUN(	do_chant	);
DECLARE_DO_FUN( do_chaosblast	);
DECLARE_DO_FUN(	do_chat		);
DECLARE_DO_FUN(	do_claim	);
DECLARE_DO_FUN( do_stat	);
DECLARE_DO_FUN(	do_clandisc	);
DECLARE_DO_FUN( do_krecall      );
DECLARE_DO_FUN( do_kinglist     );
DECLARE_DO_FUN( do_far	);
DECLARE_DO_FUN( do_roster);
DECLARE_DO_FUN( do_kinginfo     );
DECLARE_DO_FUN( do_clanlist     );
DECLARE_DO_FUN( do_claninfo     );
DECLARE_DO_FUN( do_ftalk       );
DECLARE_DO_FUN( do_induct       );
DECLARE_DO_FUN( do_clannable    );
DECLARE_DO_FUN( do_prince       );
DECLARE_DO_FUN( do_outcast      );
DECLARE_DO_FUN( do_clanname     );
DECLARE_DO_FUN( do_leaders      );
DECLARE_DO_FUN( do_kingdom      );
DECLARE_DO_FUN( do_ktalk        );
DECLARE_DO_FUN( do_clandisc     );
DECLARE_DO_FUN( do_clannum      );
DECLARE_DO_FUN( do_clanset      );
DECLARE_DO_FUN(	do_show_clan_info);
DECLARE_DO_FUN(	do_clanname	);
DECLARE_DO_FUN( do_kingname	);
DECLARE_DO_FUN(	do_claw		);
DECLARE_DO_FUN(	do_claws	);
DECLARE_DO_FUN( do_class        );
DECLARE_DO_FUN( do_autobalance	);
DECLARE_DO_FUN( do_selfclass	);
DECLARE_DO_FUN( do_unclass	);
DECLARE_DO_FUN( do_classset	);
DECLARE_DO_FUN(	do_clearstats	);
DECLARE_DO_FUN(	do_clearvam	);
DECLARE_DO_FUN(	do_clearvamp	);
DECLARE_DO_FUN(	do_close	);/*
DECLARE_DO_FUN( do_cockatrice	);*/
DECLARE_DO_FUN( do_color        );
DECLARE_DO_FUN(	do_command	);
DECLARE_DO_FUN(	do_commands	);
DECLARE_DO_FUN(	do_compare	);
DECLARE_DO_FUN(	do_complete	);
DECLARE_DO_FUN(	do_config	);
DECLARE_DO_FUN(	do_consent	);
DECLARE_DO_FUN(	do_consider	);
DECLARE_DO_FUN(	do_cprompt	);
DECLARE_DO_FUN(	do_crack	);
DECLARE_DO_FUN(	do_create	);
DECLARE_DO_FUN( do_copyover	);
DECLARE_DO_FUN( do_chardump	);
DECLARE_DO_FUN( do_rchat	);
DECLARE_DO_FUN( do_rimm	);
DECLARE_DO_FUN( do_realcopyover	);
DECLARE_DO_FUN(	do_credits	);
DECLARE_DO_FUN( do_darkblaze     );
DECLARE_DO_FUN( do_dances	);
DECLARE_DO_FUN(	do_darkheart	);/*
DECLARE_DO_FUN( do_darkness	);*/
DECLARE_DO_FUN(	do_decapitate	);
DECLARE_DO_FUN(	do_demonarmour	);
DECLARE_DO_FUN( do_discipline   );
DECLARE_DO_FUN(	do_deny		);
DECLARE_DO_FUN( do_undeny	);
DECLARE_DO_FUN(	do_description	);
DECLARE_DO_FUN(	do_diagnose	);
DECLARE_DO_FUN(	do_dismount	);
DECLARE_DO_FUN(do_disable);
DECLARE_DO_FUN(	do_disarm	);
DECLARE_DO_FUN(	do_disconnect	);
DECLARE_DO_FUN(	do_divorce	);
DECLARE_DO_FUN(	do_donate	);
DECLARE_DO_FUN(	do_down		);
DECLARE_DO_FUN(	do_draw		);
DECLARE_DO_FUN(	do_drink	);
DECLARE_DO_FUN(	do_drop		);
DECLARE_DO_FUN(do_drowcreate	);
DECLARE_DO_FUN( do_drowfire	);
DECLARE_DO_FUN( do_drows	);
DECLARE_DO_FUN( do_drowsight	);
DECLARE_DO_FUN(	do_ntrust	);
DECLARE_DO_FUN(	do_offer	);
DECLARE_DO_FUN(	do_offersoul	);
DECLARE_DO_FUN(	do_grant	);
DECLARE_DO_FUN( do_graft	);
DECLARE_DO_FUN(	do_east		);
DECLARE_DO_FUN(	do_eat		);
DECLARE_DO_FUN(	do_echo		);
DECLARE_DO_FUN(	do_empty	);
DECLARE_DO_FUN(	do_escape	);
DECLARE_DO_FUN(	do_email	);
DECLARE_DO_FUN(	do_emote	);
DECLARE_DO_FUN(	do_enter	);
DECLARE_DO_FUN(	do_equipment	);
DECLARE_DO_FUN(	do_evileye	);
DECLARE_DO_FUN(	do_examine	);
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_eyesofgod);
DECLARE_DO_FUN( do_exlist	);
DECLARE_DO_FUN(	do_familiar	);
DECLARE_DO_FUN(	do_fangs	);
DECLARE_DO_FUN( do_favor	);
DECLARE_DO_FUN(	do_favour	);
DECLARE_DO_FUN(	do_fcommand	);
DECLARE_DO_FUN(	do_feed		);
DECLARE_DO_FUN(	do_fightstyle	);
DECLARE_DO_FUN(	do_fileupdate	);
DECLARE_DO_FUN(	do_fill		);
DECLARE_DO_FUN(	do_finger	);
/*drow shit*/
DECLARE_DO_FUN( do_drowshield	);
DECLARE_DO_FUN( do_drowhate	);
DECLARE_DO_FUN( do_drowpowers	);
DECLARE_DO_FUN( do_earthshatter	);
DECLARE_DO_FUN( do_shadowwalk   );
DECLARE_DO_FUN( do_garotte	);
DECLARE_DO_FUN( do_spiderform   );
DECLARE_DO_FUN( do_dark_garotte );
DECLARE_DO_FUN( do_glamour      );
DECLARE_DO_FUN( do_confuse	);
DECLARE_DO_FUN( do_dconfuse     );
DECLARE_DO_FUN(	do_flee		);
DECLARE_DO_FUN(	do_flex		);
DECLARE_DO_FUN(	do_follow	);
DECLARE_DO_FUN( do_nofollow	);
DECLARE_DO_FUN(do_for);
DECLARE_DO_FUN(	do_force	);
DECLARE_DO_FUN(	do_forceauto	);
DECLARE_DO_FUN(	do_freeze	);
DECLARE_DO_FUN(	do_gag		);
DECLARE_DO_FUN( do_gaia         );
DECLARE_DO_FUN( do_generation   );
DECLARE_DO_FUN(	do_get		);
DECLARE_DO_FUN(	do_ghoul	);
DECLARE_DO_FUN(	do_gift		);
DECLARE_DO_FUN(	do_give		);
DECLARE_DO_FUN(	do_godless	);
DECLARE_DO_FUN(	do_goto		);
DECLARE_DO_FUN( do_grant	);
DECLARE_DO_FUN(	do_group	);
DECLARE_DO_FUN(	do_gtell	);/*
DECLARE_DO_FUN(do_heal	);*/
DECLARE_DO_FUN(	do_help		);
DECLARE_DO_FUN(	do_hide		);
DECLARE_DO_FUN( do_monktalk	);
DECLARE_DO_FUN( do_hightalk	);
DECLARE_DO_FUN( do_katana	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_stare        ); /*Stare for Highlanders -Leo*/
DECLARE_DO_FUN( do_nkatana	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_weaponteach	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_hclan	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_hlearn	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_nsword	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_lsword	); /*Katana for Highlanders*/
DECLARE_DO_FUN( do_ssword	); /*Katana for Highlanders*/
DECLARE_DO_FUN(	do_holylight	);
DECLARE_DO_FUN(	do_halosun	);
DECLARE_DO_FUN( do_holyvision );
DECLARE_DO_FUN(	do_home		);
DECLARE_DO_FUN(	do_horns	);
DECLARE_DO_FUN( do_tail        );
DECLARE_DO_FUN(	do_howl		);
DECLARE_DO_FUN(	do_huh		);
DECLARE_DO_FUN(	do_humanform	);
DECLARE_DO_FUN(	do_humanity	);
DECLARE_DO_FUN(	do_hunt		);
DECLARE_DO_FUN( do_knee		);
DECLARE_DO_FUN( do_elbow	);
DECLARE_DO_FUN( do_sweep	);
DECLARE_DO_FUN( do_palmstrike	);
DECLARE_DO_FUN( do_shinkick	);
DECLARE_DO_FUN( do_chi		);
DECLARE_DO_FUN( do_thrustkick	);
DECLARE_DO_FUN( do_spinkick	);
DECLARE_DO_FUN( do_backfist	);
DECLARE_DO_FUN( do_spiritpower	);
DECLARE_DO_FUN( do_healingtouch );
DECLARE_DO_FUN( do_deathtouch	);
DECLARE_DO_FUN( do_relax	);
DECLARE_DO_FUN( do_blinding	);
DECLARE_DO_FUN( do_mantra	);
DECLARE_DO_FUN( do_guide	);
DECLARE_DO_FUN( do_monk		);
DECLARE_DO_FUN( do_celestial    );
DECLARE_DO_FUN( do_godseye	);
DECLARE_DO_FUN( do_godsbless    );
DECLARE_DO_FUN( do_flaminghands );
DECLARE_DO_FUN( do_cloak	);
DECLARE_DO_FUN( do_prayofages   );
DECLARE_DO_FUN( do_adamantium	);
DECLARE_DO_FUN( do_steelskin	);
DECLARE_DO_FUN( do_godsheal	);
DECLARE_DO_FUN( do_ghold	);
DECLARE_DO_FUN( do_callgod	);
DECLARE_DO_FUN( do_learn	);
DECLARE_DO_FUN(	do_hurl		);
DECLARE_DO_FUN(	do_idea		);
DECLARE_DO_FUN(	do_immune	);
DECLARE_DO_FUN(	do_immtalk	);
DECLARE_DO_FUN(	do_idle		);
DECLARE_DO_FUN( do_incog        );
DECLARE_DO_FUN( do_cloak        );/*Old winvis, now cloak,*/
DECLARE_DO_FUN(	do_inconnu	);
DECLARE_DO_FUN(	do_info		);
DECLARE_DO_FUN(	do_fatality		);
DECLARE_DO_FUN(	do_inpart	);
DECLARE_DO_FUN(	do_dempowers	);
DECLARE_DO_FUN(	do_introduce	);
DECLARE_DO_FUN(	do_inventory	);
DECLARE_DO_FUN(	do_invis	);
DECLARE_DO_FUN( do_joust    );  /* Paladin Joust - Loki */
DECLARE_DO_FUN( do_kakusu   );  /* Enhanced Ninja Hide */
DECLARE_DO_FUN( do_kanzuite );  /* Ninja Truesight */
DECLARE_DO_FUN( do_mitsukeru	);
DECLARE_DO_FUN( do_koryou	);
DECLARE_DO_FUN( do_tsume	);
DECLARE_DO_FUN( do_circle	);
DECLARE_DO_FUN(	do_kick		);
DECLARE_DO_FUN( do_hara_kiri	);
/* DECLARE_DO_FUN( do_dagger	); */
/* DECLARE_DO_FUN( do_sword	); */
DECLARE_DO_FUN(	do_kill		);
DECLARE_DO_FUN(	do_killperson	);
DECLARE_DO_FUN( do_klaive       );
DECLARE_DO_FUN(	do_leap		);
DECLARE_DO_FUN( do_afk		);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_newbiepack	);
DECLARE_DO_FUN( do_itempurge	);
DECLARE_DO_FUN( do_demonform	);
DECLARE_DO_FUN( do_embrace	);
DECLARE_DO_FUN( do_reimburse	);
DECLARE_DO_FUN( do_level        );
DECLARE_DO_FUN(	do_lifespan	);
DECLARE_DO_FUN(	do_list		);
DECLARE_DO_FUN(	do_locate	);
DECLARE_DO_FUN(	do_lock		);
DECLARE_DO_FUN(	do_log		);
DECLARE_DO_FUN(	do_look		);
DECLARE_DO_FUN(	do_majesty	);
DECLARE_DO_FUN( do_mantra   );
DECLARE_DO_FUN( do_makepreg	);
DECLARE_DO_FUN(	do_marry	);
DECLARE_DO_FUN(	do_mask		);
DECLARE_DO_FUN( do_mclan    );
DECLARE_DO_FUN(	do_mclear	);
DECLARE_DO_FUN(	do_meditate	);
DECLARE_DO_FUN(	do_memory	);
DECLARE_DO_FUN(	do_mfind	);
DECLARE_DO_FUN( do_michi    ); /* Ninja michi - Loki */
DECLARE_DO_FUN( do_mienaku  ); /* Ninja Flee - Loki */
DECLARE_DO_FUN( do_miktalk     ); /* Ninja channel - Loki */
DECLARE_DO_FUN(	do_mload	);
DECLARE_DO_FUN( do_monk     );
DECLARE_DO_FUN(	do_mount	);
DECLARE_DO_FUN(	do_morph	);
DECLARE_DO_FUN(	do_mortal	);
DECLARE_DO_FUN(	do_mortalvamp	);
DECLARE_DO_FUN(	do_mset		);
DECLARE_DO_FUN(	do_mastery	);
DECLARE_DO_FUN(	do_mstat	);
DECLARE_DO_FUN(	do_mwhere	);
DECLARE_DO_FUN(	do_owhere	);
DECLARE_DO_FUN(	do_music	);
DECLARE_DO_FUN( do_nclan    );
DECLARE_DO_FUN(	do_nightsight	);
DECLARE_DO_FUN( do_ninjagear    );
DECLARE_DO_FUN( do_magecreate	);
DECLARE_DO_FUN( do_magi		);
DECLARE_DO_FUN( do_magearmor	);
DECLARE_DO_FUN(	do_noemote	);
DECLARE_DO_FUN(	do_north	);
DECLARE_DO_FUN(	do_note		);
DECLARE_DO_FUN( do_board	);
DECLARE_DO_FUN(	do_notell	);
DECLARE_DO_FUN(	do_oclone	);
DECLARE_DO_FUN(	do_ofind	);
DECLARE_DO_FUN(	do_oload	);
DECLARE_DO_FUN(	do_open		);
DECLARE_DO_FUN(	do_order	);
DECLARE_DO_FUN(	do_oreturn	);
DECLARE_DO_FUN(	do_oset		);
DECLARE_DO_FUN(	do_ostat	);
DECLARE_DO_FUN(	do_oswitch	);
DECLARE_DO_FUN(	do_otransfer	);
DECLARE_DO_FUN( do_outcast      );
DECLARE_DO_FUN(	do_pact		);
DECLARE_DO_FUN( do_palmthrust );
DECLARE_DO_FUN( do_paradox	);
DECLARE_DO_FUN( do_paladin  );  /* Paladin Channel - Loki */
DECLARE_DO_FUN(	do_password	);
DECLARE_DO_FUN( do_pclan    ); /* Paladin clan list - Loki */
DECLARE_DO_FUN(	do_peace	);
DECLARE_DO_FUN(	do_pick		);/*
DECLARE_DO_FUN( do_plasma	);*/
DECLARE_DO_FUN(	do_pload	);
DECLARE_DO_FUN(	do_pinfo	); /* Imm info channel - Kyrlin */
DECLARE_DO_FUN(   do_doublexp); /* Imm doublexp command - Kyrlin */
DECLARE_DO_FUN(   do_chaos); 	/* Imm chaos command - Toaster */
DECLARE_DO_FUN(   do_tscatter); /* Imm quest type command - Kyrlin */
DECLARE_DO_FUN(   do_doubletrain);
DECLARE_DO_FUN(   do_tick);
DECLARE_DO_FUN(   do_prep	); /* Imm prepare command - Kyrlin */
/* DECLARE_DO_FUN(   do_linkdead   ); * Imm command - Kyrlin */
DECLARE_DO_FUN( do_cset         ); /* Imm command - Kyrlin */
DECLARE_DO_FUN( do_preturn	);
DECLARE_DO_FUN(	do_poison	);
DECLARE_DO_FUN(	do_practice	);
DECLARE_DO_FUN(	do_plist	); //Simian 18-04-00
DECLARE_DO_FUN(	do_pray		);
DECLARE_DO_FUN(	do_press	);
DECLARE_DO_FUN( do_prefix	);
DECLARE_DO_FUN(	do_preturn	);
DECLARE_DO_FUN( do_principles ); /* Ninja Principles - Loki */
DECLARE_DO_FUN( do_newprinciples ); /* New Ninja Stuff - Simian */
DECLARE_DO_FUN(	do_prompt	);
DECLARE_DO_FUN( do_promote      );
DECLARE_DO_FUN(	do_propose	);
DECLARE_DO_FUN(	do_pset		);
DECLARE_DO_FUN(	do_pull		);
DECLARE_DO_FUN(	do_punch	);
DECLARE_DO_FUN(	do_purge	);
DECLARE_DO_FUN(	do_put		);
DECLARE_DO_FUN(	do_qmake	);
DECLARE_DO_FUN(	do_quaff	);
DECLARE_DO_FUN(	do_qset		);
DECLARE_DO_FUN(	do_qstat	);
DECLARE_DO_FUN(	do_qtrust	);
DECLARE_DO_FUN(	do_quest	);
DECLARE_DO_FUN(	do_question	);
DECLARE_DO_FUN(	do_qui		);
DECLARE_DO_FUN(	do_quit		);
DECLARE_DO_FUN(	do_rage		);
DECLARE_DO_FUN( do_ragesong     );
DECLARE_DO_FUN( do_lambent     );
DECLARE_DO_FUN( do_quicksand     );
DECLARE_DO_FUN( do_waste     );
DECLARE_DO_FUN(	do_read		);
DECLARE_DO_FUN(	do_readaura	);
DECLARE_DO_FUN(	do_reboo	);
DECLARE_DO_FUN(	do_reboot	);
DECLARE_DO_FUN(	do_recall	);
DECLARE_DO_FUN(	do_recharge	);
DECLARE_DO_FUN(	do_recho	);
DECLARE_DO_FUN(	do_recite	);
DECLARE_DO_FUN(	do_regenerate	);
DECLARE_DO_FUN(	do_release	);
DECLARE_DO_FUN(	do_relevel	);
DECLARE_DO_FUN(	do_reload	);
DECLARE_DO_FUN(	do_remove	);
DECLARE_DO_FUN(	do_rent		);
DECLARE_DO_FUN(	do_reply	);
DECLARE_DO_FUN(	do_report	);
DECLARE_DO_FUN(	do_rescue	);
DECLARE_DO_FUN(	do_rest		);
DECLARE_DO_FUN(	do_restore	);
DECLARE_DO_FUN(	do_retreat	); // Simian - 10/6/2001
DECLARE_DO_FUN(	do_return	);
DECLARE_DO_FUN(	do_roll		);
DECLARE_DO_FUN( do_rogue    );
DECLARE_DO_FUN(	do_rset		);
DECLARE_DO_FUN(	do_rstat	);
DECLARE_DO_FUN( do_sacredinvisibility );
DECLARE_DO_FUN(	do_sacrifice	);
DECLARE_DO_FUN(	do_safe		);
DECLARE_DO_FUN(	do_save		);
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN(	do_scan		);
DECLARE_DO_FUN(	do_score	);
DECLARE_DO_FUN( do_trait	);
DECLARE_DO_FUN( do_study	);
/* Dragon Stuff - tRacker */
DECLARE_DO_FUN( do_drackform );
DECLARE_DO_FUN( do_hatch     );
DECLARE_DO_FUN( do_dgain );
DECLARE_DO_FUN( do_ddbreath );
DECLARE_DO_FUN( do_lick );
DECLARE_DO_FUN( do_ddconfuse );
DECLARE_DO_FUN( do_dpolymorph );
DECLARE_DO_FUN( do_fly );
DECLARE_DO_FUN( do_peer );
DECLARE_DO_FUN( do_consume );
DECLARE_DO_FUN( do_age );
/* End Dragon */

DECLARE_DO_FUN( do_changes      );
DECLARE_DO_FUN( do_chsave       );
DECLARE_DO_FUN( do_addchange    );
DECLARE_DO_FUN(	do_scroll	); //Ninja Scroll Command - Simian 24/07/2001
DECLARE_DO_FUN(	do_scry		);
DECLARE_DO_FUN(	do_sell		);
DECLARE_DO_FUN(	do_serpent	);
DECLARE_DO_FUN(	do_shadowplane	);
DECLARE_DO_FUN(	do_shadowsight	);
DECLARE_DO_FUN(	do_sheath	);
DECLARE_DO_FUN(	do_shield	);
DECLARE_DO_FUN(	do_shoot	);
DECLARE_DO_FUN(	do_shout	);
DECLARE_DO_FUN(	do_shutdow	);
DECLARE_DO_FUN(	do_shutdown	);
DECLARE_DO_FUN(	do_crashcatcher	);
DECLARE_DO_FUN(	do_soulstrike	);
/*Changed by Toaster 12/02/00 To add Soulstrike command*/
DECLARE_DO_FUN(	do_show_artifacts);
DECLARE_DO_FUN(	do_side		);
DECLARE_DO_FUN( do_sign		);
DECLARE_DO_FUN(	do_silence	);
DECLARE_DO_FUN(	do_sit		);
DECLARE_DO_FUN(	do_skill	);
DECLARE_DO_FUN(	do_sla		);
DECLARE_DO_FUN(	do_slay		);
DECLARE_DO_FUN( do_toggle	);
DECLARE_DO_FUN(	do_sleep	);
DECLARE_DO_FUN(	do_slookup	);
DECLARE_DO_FUN(	do_speak	);
DECLARE_DO_FUN(	do_special	);
DECLARE_DO_FUN(	do_spell	);
DECLARE_DO_FUN(	do_stake	);
DECLARE_DO_FUN(	do_stance	);
DECLARE_DO_FUN(	do_smother	);
DECLARE_DO_FUN(	do_sneak	);
DECLARE_DO_FUN(	do_snoop	);
DECLARE_DO_FUN(	do_socials	);
DECLARE_DO_FUN( do_gsocial      );
DECLARE_DO_FUN(	do_south	);
DECLARE_DO_FUN(	do_split	);
DECLARE_DO_FUN(	do_spy		);
DECLARE_DO_FUN(	do_spydirection	);
DECLARE_DO_FUN(	do_sset		);
DECLARE_DO_FUN(	do_stand	);
DECLARE_DO_FUN(	do_statcap	);
DECLARE_DO_FUN(	do_steal	);
DECLARE_DO_FUN( do_steelshield  );
DECLARE_DO_FUN( do_summoning	);
DECLARE_DO_FUN(	do_summon	);/*
DECLARE_DO_FUN( do_sun_bath	);*/
DECLARE_DO_FUN(	do_switch	);
DECLARE_DO_FUN( do_talon        );
DECLARE_DO_FUN( do_target       );
DECLARE_DO_FUN(	do_teach	);
DECLARE_DO_FUN(	do_tear		);
DECLARE_DO_FUN(	do_tell		);
DECLARE_DO_FUN(	do_throw	);
DECLARE_DO_FUN(	do_tie		);
DECLARE_DO_FUN(	do_time		);
DECLARE_DO_FUN(	do_title	);
DECLARE_DO_FUN( do_classtitle   );
DECLARE_DO_FUN(	do_token	);
DECLARE_DO_FUN(	do_totems	);
DECLARE_DO_FUN(	do_track	);
DECLARE_DO_FUN(	do_tradition	);
DECLARE_DO_FUN(	do_train	);
DECLARE_DO_FUN(	do_transfer	);
DECLARE_DO_FUN(	do_transport	);
DECLARE_DO_FUN(	do_notravel	);
DECLARE_DO_FUN(	do_travel	);
DECLARE_DO_FUN( do_kwalk	);
DECLARE_DO_FUN(	do_tribe	);
DECLARE_DO_FUN(	do_truesight	);
DECLARE_DO_FUN(	do_trust	);
DECLARE_DO_FUN(	do_turn		);
DECLARE_DO_FUN(	do_twist	);
DECLARE_DO_FUN(	do_typo		);
DECLARE_DO_FUN(	do_unload	);
DECLARE_DO_FUN(	do_unlock	);
DECLARE_DO_FUN(	do_unpolymorph	);
DECLARE_DO_FUN(	do_untie	);
DECLARE_DO_FUN(	do_unwerewolf	);
DECLARE_DO_FUN(	do_up		);
DECLARE_DO_FUN(	do_upkeep	);
DECLARE_DO_FUN(	do_users	);
DECLARE_DO_FUN(	do_value	);
DECLARE_DO_FUN(	do_defences	);
DECLARE_DO_FUN( do_vampire	);
DECLARE_DO_FUN( do_clantalk	);
DECLARE_DO_FUN(	do_vamptalk	);
DECLARE_DO_FUN( do_bitch	);
DECLARE_DO_FUN(	do_hooves	);
DECLARE_DO_FUN(	do_magetalk	);
DECLARE_DO_FUN(	do_vanish	);
DECLARE_DO_FUN(	do_vclan	);
DECLARE_DO_FUN(	do_version	);
DECLARE_DO_FUN(	do_visible	);
DECLARE_DO_FUN( do_vlist	);
DECLARE_DO_FUN(	do_voodoo	);
DECLARE_DO_FUN(	do_web		);
DECLARE_DO_FUN(	do_wake		);
DECLARE_DO_FUN(	do_watcher	);
DECLARE_DO_FUN(	do_watching	);
DECLARE_DO_FUN(	do_weaponform	);
DECLARE_DO_FUN(	do_wear		);
DECLARE_DO_FUN(	do_wearaffect	);
DECLARE_DO_FUN(	do_weather	);
DECLARE_DO_FUN(	do_werewolf	);
DECLARE_DO_FUN(	do_west		);
DECLARE_DO_FUN(	do_where	);
DECLARE_DO_FUN(	do_whisper	);
DECLARE_DO_FUN(	do_who		);
DECLARE_DO_FUN(	do_wimpy	);/*
DECLARE_DO_FUN( do_wind_slash	);
DECLARE_DO_FUN( do_wind_song	);*/
DECLARE_DO_FUN(	do_wings	);
DECLARE_DO_FUN(	do_wizhelp	);
DECLARE_DO_FUN(	do_wizlist	);
DECLARE_DO_FUN(	do_wizlock	);
DECLARE_DO_FUN(	do_write	);
DECLARE_DO_FUN(	do_xemote	);
DECLARE_DO_FUN(	do_xsocials	);
DECLARE_DO_FUN(	do_yell		);
DECLARE_DO_FUN(	do_zap		);
DECLARE_DO_FUN( do_zuloform         );
/*new vamp stuff right here make sure commands dont conflict in interp.c*/
DECLARE_DO_FUN( do_theft	);
DECLARE_DO_FUN( do_diablerise	);
DECLARE_DO_FUN( do_spiritguard  );
DECLARE_DO_FUN( do_preserve	);
DECLARE_DO_FUN( do_spiritgate	);
DECLARE_DO_FUN( do_scream	);
DECLARE_DO_FUN( do_zombie	);
DECLARE_DO_FUN( do_bonemod	);
DECLARE_DO_FUN( do_fleshcraft	);
DECLARE_DO_FUN( do_serenity	);
DECLARE_DO_FUN( do_share	);
DECLARE_DO_FUN( do_frenzy	);
DECLARE_DO_FUN( do_pigeon	);
DECLARE_DO_FUN( do_beckon	);
DECLARE_DO_FUN( do_lamprey 	);
DECLARE_DO_FUN( do_shroud	);
DECLARE_DO_FUN( do_shadowstep	);
DECLARE_DO_FUN( do_mindblast	);
DECLARE_DO_FUN( do_entrance	);
DECLARE_DO_FUN( do_summon	);
DECLARE_DO_FUN( do_awe		);
DECLARE_DO_FUN( do_spit		);
DECLARE_DO_FUN( do_assassinate	);
DECLARE_DO_FUN( do_bloodagony	);
DECLARE_DO_FUN( do_infirmity	);
DECLARE_DO_FUN( do_silencedeath	);
DECLARE_DO_FUN( do_taste	);
DECLARE_DO_FUN( do_cauldron	);
DECLARE_DO_FUN( do_tide		);
DECLARE_DO_FUN( do_potency	);
DECLARE_DO_FUN( do_unveil	);
DECLARE_DO_FUN( do_astralwalk	);
DECLARE_DO_FUN( do_mesmerise	);
DECLARE_DO_FUN( do_acid		);
DECLARE_DO_FUN( do_conceal	);
DECLARE_DO_FUN( do_earthmeld	);
DECLARE_DO_FUN( do_flamehands	);
DECLARE_DO_FUN( do_tongue	);
DECLARE_DO_FUN( do_tendrils	);
DECLARE_DO_FUN( do_plasma	);
DECLARE_DO_FUN( do_dragonform	);
DECLARE_DO_FUN( do_dragonarmor );
DECLARE_DO_FUN( do_guardian	);
DECLARE_DO_FUN( do_fear		);
DECLARE_DO_FUN( do_gate		);
DECLARE_DO_FUN( do_vtwist	);
DECLARE_DO_FUN( do_bloodwall	);
DECLARE_DO_FUN( do_share	);
DECLARE_DO_FUN( do_pigeon	);
DECLARE_DO_FUN( do_frenzy	);
DECLARE_DO_FUN( do_beckon	);
DECLARE_DO_FUN( do_serenity	);
DECLARE_DO_FUN( do_mirror	);
DECLARE_DO_FUN( do_formillusion	);
DECLARE_DO_FUN( do_clone	);
DECLARE_DO_FUN( do_control	);
DECLARE_DO_FUN( do_objmask	);
DECLARE_DO_FUN( do_hagswrinkles	);
DECLARE_DO_FUN( do_putrefaction	);
DECLARE_DO_FUN( do_ashes	);
DECLARE_DO_FUN( do_withering 	);
DECLARE_DO_FUN( do_drain	);
DECLARE_DO_FUN( do_bloodwater	);
DECLARE_DO_FUN( do_coil		);
DECLARE_DO_FUN( do_gourge	);
DECLARE_DO_FUN( do_sharpen	);
DECLARE_DO_FUN( do_tongue	);
/*Wraith*/
DECLARE_DO_FUN( do_enshroud	);
DECLARE_DO_FUN( do_phantomwings	);
DECLARE_DO_FUN( do_flickering	);
DECLARE_DO_FUN( do_jump	);
DECLARE_DO_FUN( do_oubliette	);
DECLARE_DO_FUN( do_soulsight	);
DECLARE_DO_FUN( do_coax	);
DECLARE_DO_FUN( do_secrets	);
DECLARE_DO_FUN( do_purify	);
DECLARE_DO_FUN( do_phantommask	);
DECLARE_DO_FUN( do_statue	);
DECLARE_DO_FUN( do_materialize	);
DECLARE_DO_FUN( do_objectride	);
DECLARE_DO_FUN( do_itemjump	);
DECLARE_DO_FUN( do_dirge	);
DECLARE_DO_FUN( do_ballad	);
DECLARE_DO_FUN( do_crescendo	);
DECLARE_DO_FUN( do_requim	);
DECLARE_DO_FUN( do_imitate	);
DECLARE_DO_FUN( do_rend	);
DECLARE_DO_FUN( do_bodyshape	);
DECLARE_DO_FUN( do_pinging	);
DECLARE_DO_FUN( do_wraithgrasp	);
DECLARE_DO_FUN( do_wraithpowers	);
DECLARE_DO_FUN( do_arcanoi	);
DECLARE_DO_FUN( do_stonehand	);
DECLARE_DO_FUN( do_pyro	);
DECLARE_DO_FUN( do_obliviate	);
DECLARE_DO_FUN( do_wierdness	);
DECLARE_DO_FUN( do_befuddlement	);
DECLARE_DO_FUN( do_ether	);
DECLARE_DO_FUN( do_tempus	);
DECLARE_DO_FUN( do_dreams	);
DECLARE_DO_FUN( do_phantasmagoria);
DECLARE_DO_FUN( do_agon	);
DECLARE_DO_FUN( do_skinride	);

/*
 * Online creation commands  OLC
 */
DECLARE_DO_FUN( do_ashow        );
DECLARE_DO_FUN( do_rshow        );
DECLARE_DO_FUN( do_oshow        );
DECLARE_DO_FUN( do_mshow        );

DECLARE_DO_FUN( do_aedit        );
DECLARE_DO_FUN( do_redit        );
DECLARE_DO_FUN( do_oedit        );
DECLARE_DO_FUN( do_medit        );

DECLARE_DO_FUN( do_resetarea    );
DECLARE_DO_FUN( do_resets       );

DECLARE_DO_FUN( do_asave        );

DECLARE_DO_FUN( do_arealist     );
DECLARE_DO_FUN( do_buildtalk	);

/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(	spell_spiritkiss	);
DECLARE_SPELL_FUN(	spell_lunasblessing	);
DECLARE_SPELL_FUN(	spell_courage		);
DECLARE_SPELL_FUN(	spell_acid_blast	);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(	spell_darkness		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_drowfire	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_general_purpose	);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(      spell_godbless  );
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(	spell_haste		);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_high_explosive	);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(      spell_llothbless         );
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_protection_evil	);
DECLARE_SPELL_FUN(	spell_protection_good	);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);

DECLARE_SPELL_FUN(	spell_guardian		);
DECLARE_SPELL_FUN(	spell_soulblade		);
DECLARE_SPELL_FUN(	spell_soulstrike	);
DECLARE_SPELL_FUN(	spell_mana		);
DECLARE_SPELL_FUN(	spell_frenzy		);
DECLARE_SPELL_FUN(	spell_darkblessing	);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_energyflux	);
DECLARE_SPELL_FUN(	spell_voodoo		);
DECLARE_SPELL_FUN(	spell_transport		);
DECLARE_SPELL_FUN(	spell_regenerate	);
DECLARE_SPELL_FUN(	spell_clot		);
DECLARE_SPELL_FUN(	spell_mend		);
DECLARE_SPELL_FUN(	spell_quest		);
DECLARE_SPELL_FUN(	spell_minor_creation	);
DECLARE_SPELL_FUN(	spell_brew		);
DECLARE_SPELL_FUN(	spell_scribe		);
DECLARE_SPELL_FUN(	spell_carve		);
DECLARE_SPELL_FUN(	spell_engrave		);
DECLARE_SPELL_FUN(	spell_bake		);
DECLARE_SPELL_FUN(	spell_mount		);
DECLARE_SPELL_FUN(	spell_scan		);
DECLARE_SPELL_FUN(	spell_repair		);
DECLARE_SPELL_FUN(	spell_spellproof	);
DECLARE_SPELL_FUN(	spell_preserve		);
DECLARE_SPELL_FUN(	spell_major_creation	);
DECLARE_SPELL_FUN(	spell_copy		);
DECLARE_SPELL_FUN(	spell_insert_page	);
DECLARE_SPELL_FUN(	spell_chaos_blast	);
DECLARE_SPELL_FUN(	spell_resistance	);
DECLARE_SPELL_FUN(	spell_web		);
DECLARE_SPELL_FUN(	spell_polymorph		);
DECLARE_SPELL_FUN(	spell_contraception	);
DECLARE_SPELL_FUN(	spell_remove_page	);
DECLARE_SPELL_FUN(	spell_find_familiar	);
DECLARE_SPELL_FUN(	spell_improve		);
DECLARE_SPELL_FUN(	spell_infirmity		);

/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(_AIX)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(apollo)
int	atoi		args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(interactive)
#endif

#if	defined(linux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(macintosh)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
#if 	defined(SYSV)
size_t 	fread		args( ( void *ptr, size_t size, size_t n,
				FILE *stream ) );
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif



/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)
#define PLAYER_DIR	""		/* Player files			*/
#define BACKUP_DIR      "../player/backupinfo/"
#define NULL_FILE	"proto.are"	/* To reserve one stream	*/
#endif

#if defined(MSDOS)
#define PLAYER_DIR	""		/* Player files                 */
#define BACKUP_DIR      "../player/backupinfo/"
#define NULL_FILE	"null"		/* To reserve one stream	*/
#endif

#if defined(unix)
#define PLAYER_DIR	"../player/"	/* Player files			*/
#define NULL_FILE	"/dev/null"	/* To reserve one stream	*/
#endif

#define AREA_LIST	"area.lst"	/* List of areas		*/
#define CLAN_LIST	"clan1.txt"	/* List of clans		*/
#define ART_LIST	"art1.txt"	/* List of artifacts            */
#define HOME_AREA	"../area/homes.are" /* Mages towers, etc        */

#define BACKUP_DIR	"../player/backupinfo/"

#define BUG_FILE	"bugs.txt"      /* For 'bug' and bug( )		*/
#define IDEA_FILE	"ideas.txt"	/* For 'idea'			*/
#define TYPO_FILE	"typos.txt"     /* For 'typo'			*/
#define NOTE_FILE	"notes.txt"	/* For 'notes'			*/
#define SHUTDOWN_FILE	"shutdown.txt"	/* For 'shutdown'		*/
#define COPYOVER_FILE   "copyover.txt"  /* For copyover                 */
#define EXE_FILE        "../src/merc"
#define MAIN_PORT       1234 /*HERE TO CHANGE MAIN PORT*/
#define CLAN_DIR	"../area/clans.txt"	/* Clan file		*/
#define BAN_FILE        "../area/ban.txt"      /* For perm banned sites */
#define PLAYER_LOG_DIR  "../log/player.logs/"


/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN

void    mage_damage 	args( (CHAR_DATA *ch, CHAR_DATA *victim, int dam, const char *message, int type ) );

/* act_comm.c */
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void	room_text	args( ( CHAR_DATA *ch, char *argument ) );
char    *strlower       args( ( char * ip ) );
void	excessive_cpu	args( ( int blx ) );
bool	check_parse_name args( ( char *name ) );

void fire_effect  args(( void *vo, int level, int dam, int target));

/* arena.c */

void    undo_arena	args( ( CHAR_DATA *ch ) );
void    clean_arena	args( ( CHAR_DATA *loser, CHAR_DATA *winner ) );
bool    is_inarena	args( ( CHAR_DATA *ch ) );

/* act_info.c */
void	set_title	args( ( CHAR_DATA *ch, char *title ) );
void    stc	        args( ( const char *txt, CHAR_DATA *ch ) );
void sta args((const char *txt,AREA_DATA *area));
void    cent_to_char    args( ( char *txt, CHAR_DATA *ch ) );
void    banner_to_char  args( ( char *txt, CHAR_DATA *ch ) );
void    banner2_to_char args( ( char *txt, CHAR_DATA *ch ) );
void    divide_to_char  args( ( CHAR_DATA *ch ) );
void    divide2_to_char args( ( CHAR_DATA *ch ) );
void    divide3_to_char args( ( CHAR_DATA *ch ) );
void    divide4_to_char args( ( CHAR_DATA *ch ) );
void    divide5_to_char args( ( CHAR_DATA *ch ) );
void    divide6_to_char args( ( CHAR_DATA *ch ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
int	char_hitroll	args( ( CHAR_DATA *ch ) );
int	char_damroll	args( ( CHAR_DATA *ch ) );
int	char_ac		args( ( CHAR_DATA *ch ) );

/* act_move.c */
void	move_char	args( ( CHAR_DATA *ch, int door ) );
void	open_lift	args( ( CHAR_DATA *ch ) );
void	close_lift	args( ( CHAR_DATA *ch ) );
void	move_lift	args( ( CHAR_DATA *ch, int to_room ) );
void	move_door	args( ( CHAR_DATA *ch ) );
void	thru_door	args( ( CHAR_DATA *ch, int doorexit ) );
void	open_door	args( ( CHAR_DATA *ch, bool be_open ) );
bool	is_open		args( ( CHAR_DATA *ch ) );
bool	same_floor	args( ( CHAR_DATA *ch, int cmp_room ) );
void	check_hunt	args( ( CHAR_DATA *ch ) );

/* act_obj.c */
bool	is_ok_to_wear	args( ( CHAR_DATA *ch, bool wolf_ok, char *argument ) );
void	quest_object	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	remove_obj	args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );

/* act_wiz.c */
void	bind_char	args( ( CHAR_DATA *ch ) );
void	logchan		args( ( char *argument ) );
void	sysmes		args( ( char *argument ) );
ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg );	/* OLC */
void	call_all	args((CHAR_DATA *ch));

/* Ar_wiz.c */

bool 	in_war		args( (CHAR_DATA *ch ) );
int 	getMight        args (( CHAR_DATA * ch ));
int 	getMight        args (( CHAR_DATA * ch ));
int  	get_ratio     args (( CHAR_DATA * ch ));


/* config.c */
void    initialise_config args( ( void ) );

/* comm.c */
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );

//void	close_socket2	args( ( DESCRIPTOR_DATA *dclose, bool kickoff ) );
void	close_socket2	args( ( DESCRIPTOR_DATA *dclose ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void	send_to_all_char args( ( const char *text ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	act2		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	kavitem		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );


/* dragon.c */
char  * dragon_colour	      args( ( sh_int dragtype ) );
void    add_dragon_bonuses    args( ( CHAR_DATA *ch ) );
void    remove_dragon_bonuses args( ( CHAR_DATA *ch ) );
void    set_dragon_form	      args( ( CHAR_DATA *ch ) );
void    remove_dragon_form    args( ( CHAR_DATA *ch ) );


// Color stuff
int     colour	        args( ( char type, CHAR_DATA *ch, char *string ) );
void    colourconv      args( ( char *buffer, const char *txt , CHAR_DATA *ch ) );

#define DISABLED_FILE "disabled.txt"  /* disabled commands */

bool write_to_descriptor     args( ( int desc, char *txt, int length ) );
void    init_descriptor args( ( DESCRIPTOR_DATA *dnew, int desc ) );

/* dragon.c */
char  * dragon_colour	      args( ( sh_int dragtype ) );
void    add_dragon_bonuses    args( ( CHAR_DATA *ch ) );
void    remove_dragon_bonuses args( ( CHAR_DATA *ch ) );
void    set_dragon_form	      args( ( CHAR_DATA *ch ) );
void    remove_dragon_form    args( ( CHAR_DATA *ch ) );

/* prototypes from db.c */
void  load_disabled   args( ( void ) );
void  save_disabled   args( ( void ) );

KINGDOM_DATA king_table[MAX_KINGDOM];
/* db.c */
void	boot_db		args( ( bool fCopyOver ) );
//void	boot_db		args( ( void ) );
void	area_update	args( ( void ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
void	free_char	args( ( CHAR_DATA *ch, bool quiet ) );
char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
char *	get_roomtext	args( ( const char *name, ROOMTEXT_DATA    *rt ) );
MID *	get_mob_index	args( ( int vnum ) );
OID *	get_obj_index	args( ( int vnum ) );
RID *	get_room_index	args( ( int vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
void	free_mem	args( ( void *pMem, int sMem ) );
char *	str_dup		args( ( const char *str ) );
void	free_string	args( ( char *pstr ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
int	number_mm	args( ( void ) );
int	dice		args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
void	smash_tilde	args( ( char *str ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void	bug		args( ( const char *str, int param ) );
void	log_string	args( ( const char *str ) );
void 	log_string_quiet args(	( const char *str ) );
void	tail_chain	args( ( void ) );
void	reset_area      args( ( AREA_DATA * pArea ) );	/* OLC */
void	logf	        args( ( CHAR_DATA *ch, char *fmt, ... ) );


struct mal_war_type
{
    int timer;      // All Purpose Timer.
    int type;       // Is it LMS or TD?
    int bonus;      // Bonus for winning player/team.
    int reward;     // Reward given to each player for each kill.
    int stage;       // Bits. Yeah. Lotsa Bits.
}mal_war;

typedef struct war_data         WAR_DATA;
struct war_data
{
        sh_int  war;
        sh_int  wart;
};
 
extern  WAR_DATA        war_info;

typedef struct dxp_data         DXP_DATA;
struct dxp_data
{
	sh_int	dxp;
        sh_int  dxpt;
};

extern	DXP_DATA	dxp_info;

char *dxp_sprintf args ((bool pTime, bool reason));
char *dtr_sprintf args ((bool pTime, bool reason));
char *dqp_sprintf args ((bool pTime, bool reason));
char *chs_sprintf args ((bool pTime, bool reason));

typedef struct chao_data	CHAO_DATA;
struct chao_data
{
	sh_int  chs;
        sh_int  chst;
};

extern CHAO_DATA chao_info;

typedef struct dqp_data        DQP_DATA;
struct dqp_data
{
        sh_int  dqp;
        sh_int  dqpt;
};

extern DQP_DATA dqp_info;

typedef struct dtr_data        DTR_DATA;
struct dtr_data
{
        sh_int  dtr;
        sh_int  dtrt;
};

extern DTR_DATA dtr_info;

/* fight.c */
void	violence_update	args( ( void ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			    int dt ) );
void	adv_damage	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void    stop_embrace	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	no_attack	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	hurt_person	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
	bool agg_dam ) );
bool    has_timer	args( ( CHAR_DATA *ch ) );
void	killperson	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int  randomize_damage   args( ( CHAR_DATA *ch, int dam, int am ) );


/* garou.c */

bool   is_garou		args( ( CHAR_DATA *ch ) );

void    death_message   args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
/* handler.c */
int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_age		args( ( CHAR_DATA *ch ) );
int	get_curr_str	args( ( CHAR_DATA *ch ) );
int	get_curr_int	args( ( CHAR_DATA *ch ) );
int	get_curr_wis	args( ( CHAR_DATA *ch ) );
int	get_curr_dex	args( ( CHAR_DATA *ch ) );
int	get_curr_con	args( ( CHAR_DATA *ch ) );
int num_changes         args( ( void ) );   
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( const char *str, char *namelist ) );
#define is_full_name is_name
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
void	unequip_char2	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );

int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj2	args( ( OBJ_DATA *obj ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull, bool quiet ) );
CD *	get_char	args( ( CHAR_DATA *ch ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_in_obj	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_room	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	create_money	args( ( int amount ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
//char *	item_type_name	args( ( OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *	extra_bit_name	args( ( int extra_flags ) );
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
void	stage_update	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int stage ) );

/* magic.c */
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
int	sn_lookup	args( ( int sn ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );
void	enhance_stat	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, int apply_bit,
				    int bonuses, int affect_bit ) );

/* save.c */
void	save_char_obj		args( ( CHAR_DATA *ch ) );
void	save_char_obj_backup	args( ( CHAR_DATA *ch ) );
bool	load_char_obj		args( ( DESCRIPTOR_DATA *d, char *name ) );
bool	load_char_short		args( ( DESCRIPTOR_DATA *d, char *name ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );

/* update.c */
void    advance_level   args( ( CHAR_DATA *ch, bool hide ) );
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );

/* kav_fight.c */
void	special_move	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    beast_rage      args( ( CHAR_DATA *ch ) );
/* kav_info.c */
void	birth_date	args( ( CHAR_DATA *ch, bool is_self ) );
void	other_age	args( ( CHAR_DATA *ch, int extra, bool is_preg,
				char *argument ) );
int	years_old	args( ( CHAR_DATA *ch ) );
void	 centre_text	args( ( char *text, CHAR_DATA *ch ) );
void	 line2		args( ( CHAR_DATA *ch ) );
void	 line		args( ( CHAR_DATA *ch ) );

/* kav_wiz.c */
void oset_affect args( ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest) );

/* clan.c */
void    make_preg       args( ( CHAR_DATA *mother, CHAR_DATA *father ) );
void	werewolf_regen	args( ( CHAR_DATA *ch ) );
void	reg_mend	args( ( CHAR_DATA *ch ) );
void	vamp_rage	args( ( CHAR_DATA *ch ) );
bool	char_exists	args( ( bool backup, char *argument ) );
OD *	get_page	args( ( OBJ_DATA *book, int page_num ) );

/*
#define WRITE_STR(field, value); free_string(field); field = str_dup( value );
#define SS(stage)               debug_stage = (stage)
void  crash             args( (char *arg) );
void init_signals       args( ( void ) );
*/
/* clan_list.c (prototypes) */
void	clan_read	args( ( void ) );
void	clan_write	args( ( void ) );
void	update_clanlist	args( ( CHAR_DATA *ch, bool add_char ) );
int	find_clanname	args( ( CHAR_DATA *ch ) );


/* clan_sav.c */
void    update_leaders  args( ( CHAR_DATA *ch ) );
void    init_leaders	args( ( void ) );


//vamp.c? some file somewhere?
void	guardian_message args( ( CHAR_DATA *ch ) );

/* string.c */
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );	/* OLC */
void	string_edit	args( ( CHAR_DATA *ch, char **pString ) );	/* OLC */
void    string_add      args( ( CHAR_DATA *ch, char *argument ) );	/* OLC */
void    string_append   args( ( CHAR_DATA *ch, char **pString ) );	/* OLC */
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );	/* OLC */
char *	string_replace	args( ( char * orig, char * old, char * new ) );	/* OLC */
char *	string_proper	args( ( char * argument ) );	/* OLC */
char *	string_unpad	args( ( char * argument ) );	/* OLC */
int	arg_count	args( ( char *argument ) );	/* OLC */

void    put_area_vnum           args( ( int vnum ) );

/* olc.c */
void    aedit           args( ( CHAR_DATA *ch, char *argument ) );	/* OLC */
void    redit           args( ( CHAR_DATA *ch, char *argument ) );	/* OLC */
void    medit           args( ( CHAR_DATA *ch, char *argument ) );	/* OLC */
void    oedit           args( ( CHAR_DATA *ch, char *argument ) );	/* OLC */
void	add_reset	args( ( AREA_DATA *area, RESET_DATA *pReset, int index ) );	/* OLC */

/* bit.c */
char *	room_bit_name		args( ( int room_flags ) );	/* OLC */
int	room_name_bit		args( ( char *buf ) );		/* OLC */
int	sector_number		args( ( char *argument ) );	/* OLC */
char *	sector_name		args( ( int sect ) );		/* OLC */
int	item_name_type		args( ( char *name ) ); 	/* OLC */
char *	item_type_name		args( ( int item_type ) );	/* OLC */
int	extra_name_bit		args( ( char* buf ) );		/* OLC */
char *	extra_bit_name		args( ( int extra_flags ) ); 	/* OLC */
int	wear_name_bit		args( ( char* buf ) );		/* OLC */
char *	wear_bit_name		args( ( int wear ) );		/* OLC */
int	act_name_bit		args( ( char* buf ) );		/* OLC */
int	affect_name_bit		args( ( char* buf ) );		/* OLC */
int	affect_name_loc		args( ( char* name ) );		/* OLC */
int     wear_name_loc   	args( ( char *buf ) );		/* OLC */
char *	wear_loc_name		args( ( int wearloc ) );	/* OLC */
char *	act_bit_name		args( ( int act ) );		/* OLC */
int	get_weapon_type		args( ( char *arg ) );		/* OLC */
char *	get_type_weapon		args( ( int arg ) );		/* OLC */
int	get_container_flags	args( ( char *arg ) );		/* OLC */
char *	get_flags_container	args( ( int arg ) );		/* OLC */
int	get_liquid_type		args( ( char *arg ) );		/* OLC */
char *	get_type_liquid		args( ( int arg ) );		/* OLC */

/* warlock.c */
bool is_mage		args( ( CHAR_DATA *ch ) );
bool is_memb		args( ( CHAR_DATA *ch ) );

/* vic.c */
void reset_weapon   args( (CHAR_DATA *ch, int dtype) );
void reset_spell    args( (CHAR_DATA *ch, int dtype) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF

#define BOLD "#w"
#define NO_COLOR "#n"
#define GREEN "#g"
#define RED "#r"
#define BLUE "#b"

/* mal_war code */
void UpdateWar(void);
bool InWar(CHAR_DATA *ch);

#define CHECK_WAR(_character) { if(InWar((_character))) { \
send_to_char("Not while in a war.\n\r",(_character)); return; }

void do_startwar(CHAR_DATA *ch, char *argument);
void do_endwar(CHAR_DATA *ch, char *argument);
void do_warstats(CHAR_DATA *ch, char *argument);
void do_joinwar(CHAR_DATA *ch, char *argument);
void do_eliminate(CHAR_DATA *ch, char *argument);

char *  strip_cr        args( ( char *str  ) );

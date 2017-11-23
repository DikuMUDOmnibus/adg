/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                       
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
 /***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "merc.h"
#include "leader.h"
#include "imc.h"
#include "icec.h"


/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif


#define VERSION 3
/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>	/* for RLIMIT_NOFILE */
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
/*int	accept		args( ( int s, struct sockaddr *addr, int *addrlen
) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
*/
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
/* mercpoint: commented out to enable compiling
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
*/
int	close		args( ( int fd ) );
/*int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
*/
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
/*int	listen		args( ( int s, int backlog ) );
*/
/* kavirpoint: put this in for nin site.
int	read		args( ( int fd, char *buf, int nbyte ) );
*/
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
/* kavirpoint: put this in for nin site.
int	write		args( ( int fd, char *buf, int nbyte ) );
*/
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int	setsockopt	args( ( int s, int level, int optname, 
                            const char *optval, int optlen ) );
#else 
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
int                 numlock = 0;        /* Game is numlocked at <level> */ 
char		    str_boot_time[MAX_INPUT_LENGTH];
char		    crypt_pwd[MAX_INPUT_LENGTH];
time_t		    current_time;	/* Time of this pulse		*/
int		    chars_logged_in;	/* Stores the types of char	*/
int		    total_number_login;	/* Stores the total logins	*/
int		    high_number_login;	/* Stores most logins at once	*/
int		    arena;              /* State of the Arena		*/
int		    world_affects = 0;	/* World Affect bits		*/
int                 chainspell = 0;     /* Used for mage chain spells   */
sh_int              MemLogCount;
sh_int              MemLogMax;         

/* Colour scale char list - Calamar */

char *scale[SCALE_COLS] = {
	L_RED,
	L_BLUE,
	L_GREEN,
	YELLOW
};

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_kickoff		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void	bust_a_prompt		args( ( DESCRIPTOR_DATA *d ) );
/* Tracker Added */
void    init_signals            args( ( void ) );
void    do_auto_shutdown        args( ( void ) );


void handler( int s )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
            
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next;
            
        if ( !IS_NPC(vch) )
        {
            send_to_char("The mud has crashed.  Please reconnect in 15 seconds.\n\r", vch );
            do_autosave( vch, "" );
            if ( vch->desc ) close_socket2( vch->desc);
        }
    }
    
    switch ( s )
    {
        case SIGBUS:
            bug( "Bus error signal.", 0 );
            break;
        case SIGSEGV:
            bug( "Segmentation violation signal.", 0 );
            break;
        case SIGILL:
            bug( "Illegal Instruction signal.", 0 );
            break;
        default:
            bug( "Unrecognised signal.", 0 );
            break;
    }
    exit(1);
}

#if defined(unix)
    int control;
#endif

int port, control;

int main( int argc, char **argv )
{
    struct timeval now_time;
     bool fCopyOver = FALSE;

    //int port;

    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

#ifdef RLIMIT_NOFILE
#ifndef min
# define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif
        { 
        struct  rlimit rlp;
        (void)getrlimit(RLIMIT_NOFILE, &rlp);
        rlp.rlim_cur=min(rlp.rlim_max,FD_SETSIZE);
        (void)setrlimit(RLIMIT_NOFILE, &rlp);
	}
#endif

    /*
     * Init time and encryption.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );
    strcpy( crypt_pwd, "Don't bother." );

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif
    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

log_string("Initiating Memlog");
rename( MEMLOG_FILE, "../data/memlog.lastboot" );
unlink(MEMLOG_FILE);
MemLogCount = 0; 
MemLogMax = 100; 

    /*
     * Get the port number.
     */
    port = 1234;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
    
    /* Are we recovering from a copyover? */
        if (argv[2] && argv[2][0])
        {
            fCopyOver = TRUE;
            control = atoi(argv[3]);
        }
        else
            fCopyOver = FALSE;
    }


      /*
      * Start IMC and ICE
      */
//     imc_startup( "imc/" );
//     icec_init( );

  /*
     * Run the game.
     */
#if defined(macintosh) || defined(MSDOS)
    boot_db( fCopyOver);
    log_string( "ADG is ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined(unix)

    if (!fCopyOver)
        control = init_socket( port );

/*    control = init_socket( port ); */
    boot_db( fCopyOver );
    sprintf( log_buf, "ADG is ready to rock on port %d.",port );
    log_string( log_buf );
    game_loop_unix( control );
    close( control );
#endif
    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
//    int x;
    int fd;
    int opt = 1;


    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,sizeof(opt) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close( fd );
	exit( 1 );
    }
/*
#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close( fd );
	    exit( 1 );
	}
    }
#endif
*/
    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror( "Init_socket: bind" );
	close( fd );
	exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
	close( fd );
	exit( 1 );
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

//    imc_shutdown( );
    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.next		= descriptor_list;
       dcon.pEdit		= NULL;	/* OLC */
    dcon.pString	= NULL;	/* OLC */
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
    }

    init_signals();
    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		if ( d->connected == CON_PLAYING )
		    interpret( d->character, d->incomm );
		 break;
	            case CON_AEDITOR:
	                aedit( d->character, d->incomm );
	                break;
	            case CON_REDITOR:
	                redit( d->character, d->incomm );
	                break;
	            case CON_OEDITOR:
	                oedit( d->character, d->incomm );
	                break;
	            case CON_MEDITOR:
	                medit( d->character, d->incomm );
	                break;
	            default:
	                nanny( d, d->incomm );
	                break;
	            }
		}
		/*else
		    nanny( d, d->incomm );
*/
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;
		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL )
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)

void excessive_cpu(int blx)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) )
	{
	    send_to_char("Mud frozen: Autosave and quit.  The mud will reboot in 2 seconds.\n\r",vch);
	    interpret( vch, "quit" );
	}
    }
    exit(1);
}

void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
/* kavirpoint
	maxdesc	= control * 2;
*/
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	//maxdesc = UMAX( maxdesc, d->descriptor );      
        if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    d->idle++;
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

            /* New timer to replace waitstate - Simian 24/06/2001 */
            
            if( d->character != NULL && d->character->timers[TIMER_GEN] > 0 )
            {
                --d->character->timers[TIMER_GEN];
                continue;

            //    char buf[200];
            //    sprintf(buf,"Timer [%d]\n\r",d->character->timers[TIMER_GEN]);
            //    write_to_descriptor( d, buf, 0 );

            }       
  
            /* End new timer stuff */

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );


  if ( d->pString != NULL ) 
		    string_add( d->character,  d->incomm );	/* OLC */
	        else
		{
	            switch ( d->connected )
	            {
	            case CON_PLAYING:
	                interpret( d->character, d->incomm );
	                break;
	            case CON_AEDITOR:
	                aedit( d->character, d->incomm );
	                break;
	            case CON_REDITOR:
	                redit( d->character, d->incomm );
	                break;
	            case CON_OEDITOR:
	                oedit( d->character, d->incomm );
	                break;
	            case CON_MEDITOR:
	                medit( d->character, d->incomm );
	                break;
	            default:
	                nanny( d, d->incomm );
	                break;
}
}
/*
		if ( d->connected == CON_PLAYING )
		    interpret( d->character, d->incomm );
		else
		    nanny( d, d->incomm );
*/
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );

        /*
         * Send data/ping for IMC
         */
//        imc_idle( 0, 0 );

	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)
void new_descriptor( int control )
{
    static DESCRIPTOR_DATA d_zero;
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    BAN_DATA *pban;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
    if ( descriptor_free == NULL )
    {
	dnew		= alloc_perm( sizeof(*dnew) );
    }
    else
    {
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
    }

    *dnew		= d_zero;
    dnew->descriptor	= desc;
    dnew->connected	= CON_GET_NAME;
    dnew->outsize	= 2000;
    dnew->outbuf	= alloc_mem( dnew->outsize );

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;


	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
/*
Changed & added by Toaster to silence Sock.sinaddr from log channel.
I've done a few more in this file, so just grep for them to get em
all pretty easy to see why.
*/
	log_string_quiet( log_buf );

	from = gethostbyaddr( (char *) &sock.sin_addr,
	    sizeof(sock.sin_addr), AF_INET );
	dnew->host = str_dup( from ? from->h_name : buf );
    }
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host )&& !pban->newbie_banned )
	{
	    write_to_descriptor( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
	    close( desc );
	    free_string( dnew->host );
	    free_mem( dnew->outbuf, dnew->outsize );
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}
#endif

void init_descriptor (DESCRIPTOR_DATA *dnew, int desc)
{
    static DESCRIPTOR_DATA d_zero;

    *dnew               = d_zero;
    dnew->descriptor    = desc;
    dnew->connected     = CON_GET_NAME;
    dnew->outsize       = 2000;
    dnew->pEdit         = NULL; /* OLC */
    dnew->pString       = NULL; /* OLC */
    dnew->outbuf        = alloc_mem( dnew->outsize );    
}
/*
void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && dclose->connected == CON_PLAYING &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );

 	if ( (dclose->connected == CON_PLAYING) || 
 	  ((dclose->connected >= CON_NOTE_TO) && 
 	   (dclose->connected <= CON_NOTE_FINISH)))
	{
	    if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
	    	act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
	    ch->desc->connected = CON_NOT_PLAYING
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character, FALSE );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host );

   
    free_mem( dclose->outbuf, dclose->outsize );

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}
*/

void close_socket( DESCRIPTOR_DATA *dclose )
{
     char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

 if ( dclose->connected < CON_PLAYING )
    {
	dclose->pEdit = NULL;
	dclose->connected = CON_PLAYING;
    }

    if ( dclose->character != NULL && dclose->connected == CON_PLAYING &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
/*
    if ( dclose->character != NULL 
	&& dclose->connected == CON_PLAYING
	&& !IS_NPC(dclose->character)
	&& dclose->character->pcdata != NULL
	&& dclose->character->pcdata->obj_vnum != 0
	&& dclose->character->pcdata->chobj != NULL)
	    extract_obj(dclose->character->pcdata->chobj);
*/
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	/* If ch is writing note or playing, just lose link otherwise clear char */
 	if ( (dclose->connected == CON_PLAYING) || 
 	  ((dclose->connected >= CON_NOTE_TO) && 
 	   (dclose->connected <= CON_NOTE_FINISH)))
	{
	    if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
	    	act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
		log_string("Not Cleared.");
if ( IS_SET(ch->flag2, AFF2_INARENA) || IS_SET(ch->flag2,
AFF2_CHALLENGED) || IS_SET(ch->flag2, AFF2_CHALLENGER))
{
         REMOVE_BIT(ch->flag2, AFF2_CHALLENGED);
  REMOVE_BIT(ch->flag2, AFF2_CHALLENGER);
  REMOVE_BIT(ch->flag2, AFF2_INARENA   );
  undo_arena(ch);
 arena = FIGHT_CLEAR;
        sprintf(buf,"The arena is no longer busy!");
        do_info(ch,buf);
        stc("Do to losing link the arena has been cleared\n\r", ch);
{
//}   
	 DESCRIPTOR_DATA *d;
 for ( d = descriptor_list; d; d = d->next )
  
     if( d->connected == CON_PLAYING || d->connected == CON_EDITING)
     {
        send_to_char( buf, d->character );
     }
  }
            }


	 ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character, FALSE );
//	free_char( dclose->character ); 
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host );

    /* RT socket leak fix */
    free_mem( dclose->outbuf, dclose->outsize );

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}
/* For a better kickoff message :) KaVir */
void close_socket2( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->connected < CON_PLAYING )
    {
	dclose->pEdit = NULL;
	dclose->connected = CON_PLAYING;
    }
    if ( dclose->character != NULL && dclose->connected == CON_PLAYING &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
/*
    if ( dclose->character != NULL 
	&& dclose->connected == CON_PLAYING
	&& !IS_NPC(dclose->character)
	&& dclose->character->pcdata != NULL
	&& dclose->character->pcdata->obj_vnum != 0
	&& dclose->character->pcdata->chobj != NULL)
	    extract_obj(dclose->character->pcdata->chobj);
*/
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	if ( dclose->connected == CON_PLAYING )
	{
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character, FALSE );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host );

    /* RT socket leak fix */
    free_mem( dclose->outbuf, dclose->outsize );

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}


/* For a better kickoff message :) KaVir 
void close_socket2( DESCRIPTOR_DATA *dclose, bool kickoff )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && dclose->connected == CON_PLAYING &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	if ( dclose->connected == CON_PLAYING )
	{
	    if (kickoff)
		act( "$n doubles over in agony and $s eyes roll up into $s head.", ch, NULL, NULL, TO_ROOM );
 	    save_char_obj( ch );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character, FALSE );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }
    close( dclose->descriptor );
    free_string( dclose->host );
    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}
*/


bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	if (d != NULL && d->character != NULL)
	    sprintf( log_buf, "%s input overflow!", d->character->lasthost );
	else
	    sprintf( log_buf, "%s input overflow!", d->host );
	log_string_quiet( log_buf );

	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string_quiet( "EOF encountered on read." );
	    return FALSE;
	}
	else if ( errno == EWOULDBLOCK )
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 40 )
	    {
		if (d != NULL && d->character != NULL)
		    sprintf( log_buf, "%s input spamming!", d->character->lasthost );
		else
		    sprintf( log_buf, "%s input spamming!", d->host );
		log_string_quiet( log_buf );
		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
     if ( d->pString != NULL ) /* OLC */
write_to_buffer( d, "<[OLC]> ", 0 );
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
	CHAR_DATA *ch;
	CHAR_DATA *victim;

	ch = d->original ? d->original : d->character;
	if ( IS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

	if (IS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT))
	    bust_a_prompt( d );
	else if ( IS_SET(ch->act, PLR_PROMPT) )
	{
	    char buf[MAX_STRING_LENGTH];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];

	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);
/*
	        sprintf( buf, "[%s exp] <?hp ?m ?mv> ",exp_str );
*/
	        sprintf( buf, "<[%sX] [?H ?M ?V]> ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING)
	    {
	        victim = ch->fighting;
		if ((victim->hit*100/victim->max_hit) < 25)
		{
		    strcpy(cond, "Awful");
		    ADD_COLOUR(ch, cond, L_RED);
		}
		else if ((victim->hit*100/victim->max_hit) < 50)
		{
		    strcpy(cond, "Poor");
		    ADD_COLOUR(ch, cond, L_BLUE);
		}
		else if ((victim->hit*100/victim->max_hit) < 75)
		{
		    strcpy(cond, "Fair");
		    ADD_COLOUR(ch, cond, L_GREEN);
		}
		else if ((victim->hit*100/victim->max_hit) < 100)
		{
		    strcpy(cond, "Good");
		    ADD_COLOUR(ch, cond, YELLOW);
		}
		else if ((victim->hit*100/victim->max_hit) >= 100)
		{
		    strcpy(cond, "Perfect");
		    /*ADD_COLOUR(ch, cond, ..);*/
		}
/*
		if(IS_NINJA(victim))
		{
		    strcpy(cond, "Good");
		    ADD_COLOUR(ch, cond, YELLOW);

		} */
		sprintf(hit_str, "%ld", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%ld", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%ld", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
/*
		sprintf( buf, "[%s] <%shp %sm %smv> ", cond, hit_str, mana_str, move_str );
*/
		sprintf( buf, "<[%s] [%sH %sM %sV]> ", cond, hit_str, mana_str, move_str );
	    }
	    else
	    {
		sprintf(hit_str, "%ld", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%ld", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%ld", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);
/*
	        sprintf( buf, "[%s exp] <%shp %sm %smv> ",exp_str, hit_str, mana_str, move_str );
	        sprintf( buf, "<[%sX] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
*/
	        sprintf( buf, "<[%s] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, buf, 0 );
	}

	if ( IS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama. Familiar added by Malawar
     */
/*    if(d->character->wizard != NULL)
    {
	stc("[ ",d->character->wizard);
	stc(d->outbuf,d->character->wizard);
    }*/

    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}



/*
 * Append onto an output buffer.
 *
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
  
    if ( length <= 0 )
	length = strlen(txt);

   
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

   
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

	outbuf      = alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

  
    strcpy( d->outbuf + d->outtop, txt );
    d->outtop += length;
    return;
}

*/
//New Write to Buffer for color coded by SaGe
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length)

{

    char tb[MAX_STRING_LENGTH], ccode;

    int i,j;

    bool ansi, grfx = FALSE;

 

 

    /*

     * Find length in case caller didn't.

     */

    if ( length <= 0 )

        length = strlen(txt);

 

    if ( length >= MAX_STRING_LENGTH )

    {

        bug( "Write_to_buffer: Way too big. Closing.", 0 );

        return;

    }

 

    if (d->character == NULL ||

(d->connected != CON_PLAYING))

        ansi = FALSE;

    else ansi = (IS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;

 

    /*

     * Initial \n\r if needed.

     */

    if ( d->outtop == 0 && !d->fcommand )

    {

        d->outbuf[0]    = '\n';

        d->outbuf[1]    = '\r';

        d->outtop       = 2;

    }

 

    /*

     * Expand d->outbuf for ansi info

     */

 

    j = 0;

    ccode = '3';

    tb[0] = '\0';

    for (i = 0; i < length ; i++)

    {

        if ((txt[i] == '#' || txt[i]=='`')
	 && txt[i+1] != 'I' && txt[i+1] != 'N')

        {
	  if(txt[i]=='`')ansi = TRUE;
            if (txt[++i] == 'K')
            {

                ccode = '3';

                i++;

            }

            else ccode = '3';

 

            switch (txt[i])

            {

                default : break;

                case '#': tb[j++] = '#'; break;
		case '`': tb[j++]='`';break;
                case '-': tb[j++] = '~'; break;

                case '+': tb[j++] = '%'; break;

                case 'I':

                case 'i': if (!ansi) continue;
			  tb[j++] = 27; tb[j++] = '[';tb[j++] = '7';

                          tb[j++] = 'm'; break;

                case 'F':if (!ansi) continue;
			 tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = 'm'; break;

                case 'u':if (!ansi) continue;
			 tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] = 'm'; break;

                case 'N':

                case 'n':if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';

                    break;

                case '0': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';

                    break;

                case '1': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';

                    break;

                case '2': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';

                    break;

                case '3': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';

                    break;

                case '4': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';

                    break;

                case '5': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';

                    break;

                case '6': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';

                    break;

                case '7': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';

                    break;

                case 'w': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';

                    break;

                case 'R': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';

                    break;

                case 'r': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';

                    break;

                case 'G': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';

                    break;

                case 'g': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';

                    break;

                case 'o': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';

                    break;

                case 'y': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';

                    break;

                case 'B': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';

                    break;

                case 'b': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';

                    break;

                case 'P': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';

                    break;

                case 'p': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';

                    break;

                case 'C': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';

                    break;

                case 'c': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;

                case 'e': if (!ansi) continue;

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';

                    break;

                case '.': if (!ansi) continue;

                     switch(number_range(1,15))

                     {

                        case  1:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';

                    break;

                        case  2:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';

                    break;

                        case  3:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';

                    break;

                        case  4:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';

                    break;

                        case  5:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';

                    break;

                        case  6:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';

                    break;

                        case  7:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';

                    break;

                        case  8:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';

                    break;

                        case  9:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';

                    break;

                        case  10:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';

                    break;

                        case  11:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';

                    break;

                        case  12:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';

                    break;

                        case  13:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';

                    break;

                        case  14:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';

                    break;

                        case  15:

                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';

                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';

                    break;



                     }

                        break;



            }

        }

        else tb[j++] = txt[i];

    }

 
    if (ansi)
    {

        tb[j++] = 27;

        tb[j++] = '[';

        tb[j++] = '0';

        tb[j++] = 'm';

        if (grfx)

        {

                tb[j++] = 27;

                tb[j++]='(';

                tb[j++]='B';

        }

    }

    tb[j]   = '\0';

 

    length = j;

 

    /*

     * Expand the buffer as needed.

     */

    while ( d->outtop + length >= d->outsize )

    {

        char *outbuf;

 

        if (d->outsize >= 64000)

        {

            bug("Buffer overflow. Closing.\n\r",0);

            close_socket(d);

            return;

        }

        outbuf      = alloc_mem( 2 * d->outsize );

        strncpy( outbuf, d->outbuf, d->outtop );

        free_mem( d->outbuf, d->outsize );

        d->outbuf   = outbuf;

        d->outsize *= 2;

    }

 

    /*

     * Copy.

     */

    strncpy( d->outbuf + d->outtop, tb, length);

    d->outtop += length;

    return;

}     

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    BAN_DATA *pban;
    char *pwdnew;
    char *p;
    char *strtime;
    int i;
    int char_age = 17;
    bool fOld; 

    fOld = 0;

	/* Delete leading spaces UNLESS character is writing a note */
 	if (d->connected != CON_NOTE_TEXT)
 	{
    while ( isspace(*argument) )
	argument++;
}
    ch = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}

	sprintf(kav,"%s trying to connect.", argument);
	log_string( kav );
	fOld = load_char_short( d, argument );
	ch   = d->character;
//	ADD SOMOE COLOR HERE?
//	ADD A DESCIPTOR DATA HERE
//	RESET PORT HERE ADD NEW FUCTION

        char_age = years_old(ch);
	if ( IS_SET(ch->act, PLR_DENY) )
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, ch->lasthost );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}
	else if ( IS_EXTRA(ch, EXTRA_BORN) && char_age < 15 )
	{
	    char agebuf [MAX_INPUT_LENGTH];
	    if (char_age == 14)
		sprintf(agebuf, "You cannot play for another year.\n\r" );
	    else
		sprintf(agebuf, "You cannot play for another %d years.\n\r", 
		(15 - years_old(ch)) );
	    write_to_buffer(d, agebuf, 0);
	    close_socket( d );
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    /* Check max number of players - KaVir */

	    DESCRIPTOR_DATA *dcheck;
	    DESCRIPTOR_DATA *dcheck_next;
	    int countdesc = 0;
	    int max_players = 150;

	    for (dcheck = descriptor_list; dcheck != NULL; dcheck = dcheck_next)
	    {
		dcheck_next = dcheck->next;
	if (dcheck->character != NULL && dcheck != d)
//		countdesc++;
	    {
	    if (!IS_NPC(dcheck->character) && ch->level < LEVEL_MORTAL 
			&& strlen(dcheck->character->name) > 0 
			&& !str_cmp(dcheck->character->name, ch->name))
		    {
			write_to_buffer( d, "There is already someone of your name playing!\n\r", 0 );
			close_socket( d );
			return;
		    }
		}
		countdesc++;
	    }


/*
	    if ( IS_MAGE(ch) )
	    {
		write_to_buffer( d, "Sorry, mages are temporarily disabled (again!).\n\r", 0 );
		close_socket( d );
		return;
	    }


	    if ( IS_CLASS(ch, CLASS_DROW) && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "Drow can't connect anymore. fuck you. laf.\n\r", 0 );
		close_socket( d );
		return;
	    }

	    if ( !IS_NPC(ch) && (IS_SET(ch->act, PLR_DEMON) || IS_SET(ch->act, PLR_CHAMPION)))
	    {
		write_to_buffer( d, "Sorry, demons are temporarily disabled (again!).\n\r", 0 );
		close_socket( d );
		return;
	    }
*/
	    if ( countdesc > max_players && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "Too many players connected, please try again in a couple of minutes.\n\r", 0 );
		close_socket( d );
		return;
	    }

	    if ( IS_SET(config_info->world,WIZLOCKED) && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
		close_socket( d );
		return;
	    }
            if ( ch->level <= numlock
                && numlock != 0
                && !IS_IMMORTAL(ch) )
            {
                write_to_buffer( d, "The game is currently newbie locked, or level locked.\n\r",0 );
                close_socket( d ) ;
                return;
            }
            for ( pban = ban_list ; pban != NULL ; pban = pban->next )
            {
                if ( !str_suffix( pban->name, d->host )
                &&      pban->newbie_banned
                &&      !char_exists( FALSE, ch->name ) )
                {
                    write_to_buffer( d, "New characters are not allowed to be created from this site.\n\r",0 );
                    close_socket( d );
                    return;
                }
            }
           if (countdesc > high_number_login) high_number_login = countdesc;
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Please enter password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
	    sprintf( buf, "You want %s engraved on your tombstone (Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif


	if ( ch == NULL || (!IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( argument, ch->pcdata->pwd ) &&
	    strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}
	else if ( ch == NULL || (IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	if ( check_playing( d, ch->name ) )
	    return;
/*
	if ( check_kickoff( d, ch->name, TRUE ) )
	    return;

	** Avoid nasty duplication bug - KaVir */
	if (ch->level > 1)
	{
	    sprintf(kav,ch->name);
	    free_char(d->character, FALSE);
	    d->character = NULL;
	    fOld = load_char_obj( d, kav );
	    ch   = d->character;
	}


	if ( !IS_EXTRA(ch,EXTRA_NEWPASS) && strlen(argument) > 1) {
		sprintf(kav,"%s %s",argument,argument);
		do_password(ch,kav);}

	if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
		ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
	{
	    sprintf(kav,"Last connected from %s at %s\n\r",ch->lasthost,ch->lasttime);
	    write_to_buffer( d, kav, 0 );
	}
	else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
	{
	    sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
	    write_to_buffer( d, kav, 0 );
	}
	if (ch->lasthost != NULL) free_string(ch->lasthost);
	if (ch->desc != NULL && ch->desc->host != NULL)
	{
 	  ch->lasthost = str_dup(ch->desc->host);
	}
	else
	    ch->lasthost = str_dup("(unknown)");
	strtime = ctime( &current_time );
	strtime[strlen(strtime)-1] = '\0';
	free_string(ch->lasttime);
	ch->lasttime = str_dup( strtime );
	sprintf( log_buf, "%s@%s has connected.", ch->name, ch->lasthost );
	log_string( log_buf );

	/* In case we have level 4+ players from another merc mud, or 
	 * players who have somehow got file access and changed their pfiles.
	 */
	if ( ch->level > 3 && ch->trust == 0)
	    ch->level = 3;
	else
	{
	    if ( ch->level > MAX_LEVEL )
		ch->level = MAX_LEVEL;
	    if ( ch->trust > MAX_LEVEL)
		ch->trust = MAX_LEVEL;
	    /* To temporarily grant higher powers...
	    if ( ch->trust > ch->level)
		ch->trust = ch->level;
	    */
	}
write_to_buffer(d, "\n\r", 2);
	if ( IS_IMMORTAL(ch) )	   
	    do_help( ch, "imotd" );
	else 
	    do_help( ch, "motd" );
	d->connected = CON_READ_MOTD;
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character, FALSE );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );

	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );

	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp(  crypt(argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "What is your sex (M/F)? ", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}
/*
	 write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "#rYou may choose a class now, or have someone induct you later.#n\n\r", 0);
	write_to_buffer( d, "#rYou will not be able to change classes without penalty.#n\n\r", 0);
        write_to_buffer( d, "#rREMEBER: This is for a limited time and there will a Pwipe#n\n\r", 0);
	write_to_buffer( d, "#ron the 9th of Feb 2000.#n\n\r", 0);
	write_to_buffer( d, "	(W)erewolf    - hairy, clawed beast.\n\r",0);
	write_to_buffer( d, "	(V)ampire     - bloodthirsty creatures of the night.\n\r", 0);
	write_to_buffer( d, "	(D)emon       - the damned of hell.\n\r", 0);
	write_to_buffer( d, "	 D(r)ow       - powerful dark elves.\n\r",0);
	write_to_buffer( d, "	 W(A)rlocks   - Spell casters without par.\n\r",0);
	write_to_buffer( d, "	(H)ighlanders - Immortal beings with big swords.\n\r",0);
	write_to_buffer( d, "	(M)onk        - Servants of God.\n\r", 0);
	write_to_buffer( d, "    Nin(j)a      - Masters of there surroundings.\n\r", 0);
	write_to_buffer( d, "	(N)one!	      - look for a class later.\n\r\n\r", 0);
        write_to_buffer( d, "What is your class  (W/V/D/R/M/A/H/J/N)? ", 0 );
        write_to_buffer( d, "There are other classes that will be in soon\n\r", 0);
	d->connected = CON_GET_NEW_CLASS;
        break;

    case CON_GET_NEW_CLASS:
        switch ( argument[0] )
        {
        case 'a': case 'A': 
          ch->class = CLASS_MAGE;
        ch->trust = 6;
        ch->level = 6;
         break;
        case 'h': case 'H': 
          ch->class = CLASS_HIGHLANDER;
        ch->trust = 0;
        ch->level = 2;
         break;
        case 'w': case 'W':
	ch->class = CLASS_WEREWOLF;
	ch->pcdata->stats[UNI_GEN] = 3;
	ch->trust = 0;
	ch->level = 2;
	 break;
        case 'v': case 'V':
	  ch->class = CLASS_VAMPIRE;
	  ch->pcdata->stats[UNI_GEN] = 3;
 	  ch->beast = 30;
	 ch->trust = 0;
        ch->level = 2;
	  break;
	case 'd': case 'D':
	  ch->class = CLASS_DEMON;
	  ch->pcdata->stats[UNI_GEN] = 1;
	 ch->trust = 0;
        ch->level = 2;  
	break;
	case 'r': case 'R':
	  ch->class = CLASS_DROW;
	  ch->pcdata->stats[UNI_GEN] = 3;
	ch->trust = 0;
        ch->level = 2;  
	break;
	case 'm': case 'M':
	 ch->class = CLASS_MONK;
	 ch->pcdata->stats[UNI_GEN] = 3;
	 ch->trust = 0;
        ch->level = 2; 
	break;
	case 'j': case 'J':
         ch->class = CLASS_NINJA;
         ch->pcdata->stats[UNI_GEN] = 3;
         ch->trust = 0;
        ch->level = 2; 
	break;
	case 'n': case 'N': ch->class = 0;	
 	ch->trust = 0;
        ch->level = 2;
	break;
        default:
            write_to_buffer( d, "That's not a class.\n\rWhat IS your class (W/V/D/R/H/M/J/N)? ", 0);
            return;
        }
*/
	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "Does your terminal support ANSI (y/n)? ", 0);
	d->connected = CON_GET_NEW_ANSI;
	break;

    case CON_GET_NEW_ANSI:
	switch ( argument[0] )
	{
	case 'y': case 'Y': SET_BIT(ch->act,PLR_ANSI);    break;
	case 'n': case 'N': break;
	default:
	    write_to_buffer( d, "Does your terminal support ANSI? (y/n)? ", 0 );
	    return;
	}
        ch->pcdata->perm_str=number_range(10,16);
        ch->pcdata->perm_int=number_range(10,16);
        ch->pcdata->perm_wis=number_range(10,16);
        ch->pcdata->perm_dex=number_range(10,16);
	ch->pcdata->perm_con=number_range(10,16);
	sprintf( log_buf, "%s@%s new player.", ch->name, d->host );
	log_string( log_buf );
	write_to_buffer( d, "\n\r", 2 );
	do_help( ch, "motd" );
	d->connected = CON_READ_MOTD;
//        ch->level = 0;
	break;

    case CON_READ_MOTD:
/* Changed by Toaster 19/01/00 */
	write_to_buffer( d,     "\n\rWelcome to Another Darn GodWars.\n\r",
	    0 );
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
	total_number_login++;
/*Changed by Toaster 28/01/00 Vampires now gain age status from being
online, instead of training it. Will consider increasing some of the +ve
things from age to balance this
if (IS_CLASS(ch, CLASS_VAMPIRE) && (IS_SET(ch->special, SPC_PRINCE) || 
	    ch->pcdata->stats[UNI_GEN] < 3 || ch->pcdata->rank > AGE_CHILDE))
*/
/*
if (IS_CLASS(ch, CLASS_VAMPIRE))
	{

	    int ch_age = (get_age(ch) - 17) * 2;
	    if      (ch_age >= 400)  ch->pcdata->rank = AGE_METHUSELAH;
	    else if (ch_age >= 200 ) ch->pcdata->rank = AGE_ELDER;
	    else if (ch_age >= 100 ) ch->pcdata->rank = AGE_ANCILLA;
	    else if (ch_age >= 50  ) ch->pcdata->rank = AGE_NEONATE;
	    else 		     ch->pcdata->rank = AGE_CHILDE;	
	}*/

if (ch->level == 0)
{
ch->class = 0;
}
if (IS_IMMORTAL(ch) && !IS_SET(chars_logged_in, LOGIN_GOD)) 
	{
	    SET_BIT(chars_logged_in, LOGIN_GOD);
	    log_string( "LOGIN-INFO: The first GOD has connected." );
	}
	if (ch->level == 0 && !IS_SET(chars_logged_in, LOGIN_NEWBIE))
	{
	    SET_BIT(chars_logged_in, LOGIN_NEWBIE);
	    log_string( "LOGIN-INFO: The first NEWBIE has connected." );
	}

	if (IS_CLASS(ch, CLASS_MAGE)  && !IS_SET(chars_logged_in, LOGIN_MAGE)) 
	{
	    SET_BIT(chars_logged_in, LOGIN_MAGE);
	    log_string( "LOGIN-INFO: The first MAGE has connected." );
	}

	if (IS_VAMPIRE(ch) && !IS_SET(chars_logged_in, LOGIN_VAMPIRE)) 
	{
	    SET_BIT(chars_logged_in, LOGIN_VAMPIRE);
	    log_string( "LOGIN-INFO: The first VAMPIRE has connected." );
	}
	if (IS_WEREWOLF(ch) && !IS_SET(chars_logged_in, LOGIN_WEREWOLF)) 
	{
	    SET_BIT(chars_logged_in, LOGIN_WEREWOLF);
	    log_string( "LOGIN-INFO: The first WEREWOLF has connected." );
	}
	if (IS_CLASS(ch, CLASS_DEMON) && !IS_SET(chars_logged_in,LOGIN_DEMON)) 
	{
	    SET_BIT(chars_logged_in, LOGIN_DEMON);
	    log_string( "LOGIN-INFO: The first DEMON has connected." );
	}
	if (IS_HIGHLANDER(ch) && !IS_SET(chars_logged_in, LOGIN_HIGHLANDER)) 
	{
	    SET_BIT(chars_logged_in, LOGIN_HIGHLANDER);
	    log_string( "LOGIN-INFO: The first HIGHLANDER has connected." );
	}
        if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(chars_logged_in, LOGIN_DROW))
        {
	    SET_BIT(chars_logged_in, LOGIN_DROW);
            log_string( "LOGIN_INFO: The first DROW has connected." );
        }
        if (IS_CLASS(ch, CLASS_NINJA) && !IS_SET(chars_logged_in, LOGIN_NINJA))
        {
	    SET_BIT(chars_logged_in, LOGIN_NINJA);
            log_string( "LOGIN_INFO: The first NINJA has connected." );
        }
        if (IS_CLASS(ch, CLASS_MONK) && !IS_SET(chars_logged_in, LOGIN_MONK))
        {
	    SET_BIT(chars_logged_in, LOGIN_MONK);
            log_string( "LOGIN_INFO: The first MONK has connected." );
        }
	if (ch->level < 3 && !IS_SET(chars_logged_in, LOGIN_MORTAL))
	{
	    SET_BIT(chars_logged_in, LOGIN_MORTAL);
	    log_string( "LOGIN-INFO: The first MORTAL has connected." );
	}

	if ( ch->level == 0 )
	{
	    ch->level	= 1;
	    ch->exp	= 0;
	    ch->pcdata->security = 9;	/* OLC */
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
	    ch->class = ch->class;
            ch->pcdata->board = &boards[0];
	    ch->class   = 0;
            ch->special = 0;
	    set_title( ch, "the wanderer" );
	        SET_BIT( ch->pcdata->resist[0], RES_ANIMALISM );
	    SET_BIT( ch->pcdata->resist[0], RES_AUSPEX );
	    SET_BIT( ch->pcdata->resist[0], RES_PRESENCE );
	    SET_BIT( ch->pcdata->resist[0], RES_DOMINATE );
	    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	    send_to_char("If you need help, try talking to the spirit of mud school!\n\r",ch);
	    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    do_look( ch, "auto" );
	}
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	{
	    if (ch->in_room != NULL) char_to_room( ch, ch->in_room );
	    else char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    bind_char(ch);
	    break;
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	    do_look( ch, "auto" );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	    do_look( ch, "auto" );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	    do_look( ch, "auto" );
	}
	do_board (ch, ""); /* Show board status and Changed by Toaster 19/01/00*/
      if( !IS_IMMORTAL(ch) )
      {
        if (number_range(1,10) == 1) { do_bountyz(ch,"self 25"); }
	sprintf(buf,"{W%s {Dhas entered the game.{x", ch->name);
	do_info(ch,buf);
      }


	if(ch->version < VERSION){
	  stc("Player File updated.\n\r",ch);ch->version = VERSION;
if (ch->class == CLASS_MAGE)
{
        ch->pcdata->powersm[MPOWER_RUNE0] = ch->pcdata->powers[MPOWER_RUNE0];
        ch->pcdata->powersm[MPOWER_RUNE1] = ch->pcdata->powers[MPOWER_RUNE1];
        ch->pcdata->powersm[MPOWER_RUNE2] = ch->pcdata->powers[MPOWER_RUNE2];
        ch->pcdata->powersm[MPOWER_RUNE3] = ch->pcdata->powers[MPOWER_RUNE3];

        ch->pcdata->powers[MPOWER_RUNE0] = 0;
        ch->pcdata->powers[MPOWER_RUNE1] = 0;
        ch->pcdata->powers[MPOWER_RUNE2] = 0;
        ch->pcdata->powers[MPOWER_RUNE3] = 0;
}
if (ch->class == CLASS_VAMPIRE)
{
ch->pcdata->powers[VAM_ANIM] = ch->pcdata->powersde[VAM_ANIM];
ch->pcdata->powers[VAM_CELE] = ch->pcdata->powersde[VAM_CELE];
ch->pcdata->powers[VAM_FORT] = ch->pcdata->powersde[VAM_FORT];
ch->pcdata->powers[VAM_OBTE] = ch->pcdata->powersde[VAM_OBTE];
ch->pcdata->powers[VAM_PRES] = ch->pcdata->powersde[VAM_PRES];
ch->pcdata->powers[VAM_QUIE] = ch->pcdata->powersde[VAM_QUIE];
ch->pcdata->powers[VAM_THAU] = ch->pcdata->powersde[VAM_THAU];
ch->pcdata->powers[VAM_AUSP] = ch->pcdata->powersde[VAM_AUSP];
ch->pcdata->powers[VAM_DOMI] = ch->pcdata->powersde[VAM_DOMI];
ch->pcdata->powers[VAM_OBFU] = ch->pcdata->powersde[VAM_OBFU];
ch->pcdata->powers[VAM_POTE] = ch->pcdata->powersde[VAM_POTE];
ch->pcdata->powers[VAM_PROT] = ch->pcdata->powersde[VAM_PROT];
ch->pcdata->powers[VAM_SERP] = ch->pcdata->powersde[VAM_SERP];
ch->pcdata->powers[VAM_VICI] = ch->pcdata->powersde[VAM_VICI];
ch->pcdata->powers[VAM_DAIM] = ch->pcdata->powersde[VAM_DAIM];
ch->pcdata->powers[VAM_CHIM] = ch->pcdata->powersde[VAM_CHIM];
ch->pcdata->powers[VAM_OBEA] = ch->pcdata->powersde[VAM_OBEA];
ch->pcdata->powers[VAM_MELP] = ch->pcdata->powersde[VAM_MELP];
ch->pcdata->powers[VAM_NECR] = ch->pcdata->powersde[VAM_NECR];
ch->pcdata->powers[VAM_THAN] = ch->pcdata->powersde[VAM_THAN];
}
}
	adjust_hiscore( "status", ch, ch->race );
        adjust_hiscore( "pdeath", ch, ch->pdeath );
        adjust_hiscore( "pkill", ch, ch->pkill );
    adjust_hiscore( "qp", ch, ch->pcdata->score[SCORE_QUEST] );
if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
    {
REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
}
	 if ( ch->level == 1 )
        {
          do_newbiepack(ch,"");
          do_oset(ch,"nnmask immune charm");
          do_oset(ch,"nnbelt immune sleep");
          do_wear(ch, "sword");  
          do_wear(ch, "dagger");
          sprintf(buf,"all");
          do_wear(ch, buf);  
        }
	if(IS_WRAITH(ch))
	{
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	}
	if ( ch->gifts[HOMID] == 1 )
        {
	   SET_BIT(ch->extra, EXTRA_AWE);
        }
/* Changed Toaster 17/03/00 for legend reimburse
	if (ch->lstatus > 0 && ch->level < 7)
	{
	    if (ch->lstatus == 9)
	    {
	    ch->exp += 124000000;
	    ch->pcdata->quest += 45000;
	    ch->lstatus = 0;
	    return;
	    }
	    else if(ch->lstatus == 8)
	    {
	    ch->exp += 98000000;
	    ch->pcdata->quest += 36000;
            ch->lstatus = 0;
            return;
	    }
	    else if(ch->lstatus == 7)
            {
	    ch->exp += 75000000;
	    ch->pcdata->quest += 28000;
            ch->lstatus = 0;
            return;
            } 
	    else if(ch->lstatus == 6)
            {
	    ch->exp += 55000000;
	    ch->pcdata->quest += 21000;
            ch->lstatus = 0;
            return;
            } 
	    else if(ch->lstatus == 5)
            {
	    ch->exp += 38000000;
	    ch->pcdata->quest += 15000;
            ch->lstatus = 0;
            return;
            } 
	    else if(ch->lstatus == 4)
            {
	    ch->exp += 24000000;
	    ch->pcdata->quest += 10000;
            ch->lstatus = 0;
            return;
            } 
	    else if(ch->lstatus == 3)
            {
	    ch->exp += 13000000;
	    ch->pcdata->quest += 6000;
            ch->lstatus = 0;
            return;
            } 
	    else if(ch->lstatus == 2)
            {
	    ch->exp += 5000000;
	    ch->pcdata->quest += 3000;
            ch->lstatus = 0;
            return;
            } 
	    else if(ch->lstatus == 1)
            {
	    ch->exp += 2000000;
	    ch->pcdata->quest += 1000;
            ch->lstatus = 0;
            return;
            } 
	    else return;
            

	}*/

/*
        if (ch->level > 1 && ch->level < 7)
	{
	 do_itempurge(ch,"");
        }
*/
	
	if(IS_VAMPIRE(ch) && IS_POLYAFF(ch, POLY_ZULO))
	{	
        REMOVE_BIT(ch->polyaff, POLY_ZULO);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	free_string( ch->morph );
        ch->morph = str_dup( "" );
	}

        undo_arena(ch);
	update_leaders(ch);
	ch->fight_timer = 0;

    /* Reset timers - Simian 25/06/2001 */
 
    ch->timers[TIMER_GEN] = 0;  
                
    /* End reset timers */

	act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );
	room_text(ch,">ENTER<");
	break;


	case CON_NOTE_TO:
		handle_con_note_to (d, argument);
		break;
		
	case CON_NOTE_SUBJECT:
		handle_con_note_subject (d, argument);
		break; /* subject */
	
	case CON_NOTE_EXPIRE:
		handle_con_note_expire (d, argument);
		break;

	case CON_NOTE_TEXT:
		handle_con_note_text (d, argument);
		break;
		
	case CON_NOTE_FINISH:
		handle_con_note_finish (d, argument);
		break;
//	}

/*	if (IS_CLASS(ch, CLASS_MAGE)) ch->level = 6;
	do_note(ch,"list");
        do_sset(ch,"self all 100");
*/	
        if (IS_SET(ch->warp, WARP_INFIRMITY))
        {
	SET_BIT(ch->flag2, AFF2_ROT);
}
if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
    {
REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
}
	for( i = 0; i < 16; i++ )
	{
	  if ( IS_CLASS(ch, CLASS_VAMPIRE) )
	  {
	    if (ch->pcdata->powers[i] < 0 
		|| ch->pcdata->powers[i] > 10 )
	      ch->pcdata->powers[i] = 0;
	  }
	}
	  
	break;
    }

    return;
}


/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    DESCRIPTOR_DATA *d,*d_next;
    int count;
    /*
     * Reserved words. Changed by Toaster, added NONAME here for obvious reasons.
     */
    if ( is_name( name, "all auto immortal self someone gaia noname sage kavir caine lloth" ) )
	return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen(name) <  3 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }
/* Multi-Newbie / Dupe Bug Fix */
    if (descriptor_list)
    {
        count=0;
        for (d = descriptor_list; d != NULL; d = d_next) {
            d_next=d->next;
            if (d->connected != CON_PLAYING && d->character&&d->character->name
                && d->character->name[0] && !str_cmp(d->character->name,name)) {
                count++;
                close_socket(d);
            }
        }
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch) && !IS_EXTRA(ch, EXTRA_SWITCH)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( d->character->name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character, FALSE );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "Reconnecting.\n\r", ch );
	    	if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
		    act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s reconnected.", ch->name, ch->lasthost );
		log_string_quiet( log_buf );
		d->connected = CON_PLAYING;
	    /* Inform the character of a note in progress and the possbility of continuation! */		
 		if (ch->pcdata->in_progress)
 			send_to_char ("You have a note in progress. Type NWRITE to continue it.\n\r",ch);

	    }
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Kick off old connection.  KaVir.
 */
bool check_kickoff( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( d->character->name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character, FALSE );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "You take over your body, which was already in use.\n\r", ch );
		act( "...$n's body has been taken over by another spirit!", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s kicking off old link.", ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing - KaVir.
 * Using kickoff code from Malice, as mine is v. dodgy.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold != NULL; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? dold->original->name : dold->character->name ) )
	{
	    char	buf [MAX_STRING_LENGTH];
	    if ( d->character != NULL )
	    {
		free_char( d->character, FALSE );
		d->character = NULL;
	    }
	    send_to_char("This body has been taken over!\n\r",dold->character);
	    d->connected = CON_PLAYING;
	    dold->connected = CON_NOT_PLAYING;
	    d->character = dold->character;
	    d->character->desc = d;
	    send_to_char( "You take over your body, which was already in use.\n\r", d->character );
	    act( "$n doubles over in agony and $s eyes roll up into $s head.", d->character, NULL, NULL, TO_ROOM );
	    act( "...$n's body has been taken over by another spirit!", d->character, NULL, NULL, TO_ROOM );
	    dold->character=NULL;
	    sprintf(buf,"Kicking off old connection %s@%s",d->character->name,d->host);
	    log_string(buf);
	    close_socket(dold);	/*Slam the old connection into the ether*/
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}

// Abbr. For color send to char
void sta(const char *txt,AREA_DATA *area)
{
  CHAR_DATA *rch;

  for(rch=char_list;rch!=NULL;rch=rch->next)
  {
    if(rch->in_room->area == area)
	stc(txt,rch);
  }
}
void stc( const char *txt, CHAR_DATA *ch )
{
    const       char    *point;
                char    *point2;
                char    buf[ MAX_STRING_LENGTH*4 ];
                int     skip = 0;

    if(IS_NPC(ch)&&ch->wizard != NULL)
	{stc("[ ",ch->wizard);stc(txt,ch->wizard);}
    buf[0] = '\0';
    point2 = buf;
    if( txt && ch->desc )
        {
            if( IS_SET( ch->act, PLR_COLOUR ) )
            {
                for( point = txt ; *point ; point++ )
                {
                    if( *point == '{' )
                    {
                        point++;
                        skip = colour( *point, ch, point2 );
                        while( skip-- > 0 )
                            ++point2;
                        continue;
                    }
                    *point2 = *point;
                    *++point2 = '\0';
                }
                *point2 = '\0';
                write_to_buffer( ch->desc, buf, point2 - buf );
            }
            else
            {
                for( point = txt ; *point ; point++ )
                {
                    if( *point == '{' )
                    {
                        point++;
                        continue;
                    }
                    *point2 = *point;
                    *++point2 = '\0';
                }
                *point2 = '\0';
                write_to_buffer( ch->desc, buf, point2 - buf );
            }
        }
    return;
}

/* Color STC */
/*
 * Write to one char, new colour version, by Lope.
 */
void send_to_char(const char *txt,CHAR_DATA *ch)
{stc(txt,ch);}
/*void send_to_char( const char *txt, CHAR_DATA *ch )
{
    const       char    *point;
                char    *point2;
                char    buf[ MAX_STRING_LENGTH*4 ];
                int     skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if( txt && ch->desc )
        {
            if( IS_SET( ch->act, PLR_COLOUR ) )
            {
                for( point = txt ; *point ; point++ )
                {
                    if( *point == '{' )
                    {
                        point++;
                        skip = colour( *point, ch, point2 );
                        while( skip-- > 0 )
                            ++point2;
                        continue;
                    }
                    *point2 = *point;
                    *++point2 = '\0';
                }
                *point2 = '\0';
                write_to_buffer( ch->desc, buf, point2 - buf );
            }
            else
            {
                for( point = txt ; *point ; point++ )
                {
                    if( *point == '{' )
                    {
                        point++;
                        continue;
                    }
                    *point2 = *point;
                    *++point2 = '\0';
                }
                *point2 = '\0';
                write_to_buffer( ch->desc, buf, point2 - buf );
            }
        }
    return;
}*/
 

/*
 * The colour version of the act( ) function, -Lope
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1,
              const void *arg2, int type )
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };
    CHAR_DATA           *to;
    CHAR_DATA           *vch = ( CHAR_DATA * ) arg2;
    OBJ_DATA            *obj1 = ( OBJ_DATA  * ) arg1;
    OBJ_DATA            *obj2 = ( OBJ_DATA  * ) arg2;
    const       char    *str;
    char                *i;
    char                *point;
    char                *pbuff;
    char                buf[ MAX_STRING_LENGTH   ];
    char                buffer[ MAX_STRING_LENGTH*2 ];
    char                fname[ MAX_INPUT_LENGTH  ];
    bool                fColour = FALSE;
    bool		is_ok;
    int			min_pos = 5;

    if( !format || !*format )
        return;

    if( !ch || !ch->in_room )
        return;

     to = ch->in_room->people;
     if( type == TO_VICT )
     {
         if( !vch )
         {
             bug( "Act: null vch with TO_VICT.", 0 );
             return;
         }

        if( !vch->in_room )
            return;

         to = vch->in_room->people;
     }

     for( ; to ; to = to->next_in_room )
     {
	 if(to->wizard && (to->wizard->in_room !=  to->in_room)){to=to->wizard;}

         if( (!to->desc || to->position < min_pos ))
             continue;
         if( type == TO_CHAR && to != ch)
             continue;
         if( type == TO_VICT && ( to != vch || to == ch ) )
             continue;
         if( type == TO_ROOM && (to == ch))
             continue;
         if( type == TO_NOTVICT && ( to == ch || to == vch ) )
             continue;
	if(to->pcdata->familiar && to->pcdata->familiar == ch)stc("[ ",to);
        if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
        {
            is_ok = FALSE;

            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                ch->pcdata->chobj->in_room != NULL &&
                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                to->pcdata->chobj->in_room != NULL &&
                ch->in_room == to->in_room)
                    is_ok = TRUE; else is_ok = FALSE;

            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                ch->pcdata->chobj->in_obj != NULL &&
                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                to->pcdata->chobj->in_obj != NULL &&
                ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
                    is_ok = TRUE; else is_ok = FALSE;

            if (!is_ok)
            {
/*
                if (is_fam) to = to_old;
*/
                continue;
            }
        }

         point   = buf;
         str     = format;
         while( *str )
         {
             if( *str != '$' )
             {
                 *point++ = *str++;
                 continue;
             }

            i = NULL;
            switch( *str )
            {
                case '$':
                    fColour = TRUE;
                    ++str;
                    i = " <@@@> ";
                    if ( !arg2 && *str >= 'A' && *str <= 'Z' )
                    {
                        bug( "Act: missing arg2 for code %d.", *str );
                        i = " <@@@> ";
                    }
                    else
                    {
                        switch ( *str )
                        {
                            default:
                                bug( "Act: bad code %d.", *str );
                                i = " <@@@> ";
                                break;

                            case 't':
                                i = (char *) arg1;
                                break;

                            case 'T':
                                i = (char *) arg2;
                                break;

                            case 'n':
                                i = PERS( ch,  to  );
                                break;

                            case 'N':
                                i = PERS( vch, to  );
                                break;

                            case 'e':
                                i = he_she  [URANGE(0, ch  ->sex, 2)];
                                break;

                            case 'E':
                                i = he_she  [URANGE(0, vch ->sex, 2)];
                                break;

                            case 'm':
                                i = him_her [URANGE(0, ch  ->sex, 2)];
                                break;

                            case 'M':
                                i = him_her [URANGE(0, vch ->sex, 2)];
                                break;

                            case 's':
                                i = his_her [URANGE(0, ch  ->sex, 2)];
                                break;

                            case 'S':
                                i = his_her [URANGE(0, vch ->sex, 2)];
                                break;

                            case 'p':
                                i = can_see_obj( to, obj1 )
                                  ? obj1->short_descr
                                  : "something";
                                break;

                            case 'P':
                                i = can_see_obj( to, obj2 )
                                  ? obj2->short_descr
                                  : "something";
                                break;

                            case 'd':
                                if ( !arg2 || ((char *) arg2)[0] == '\0' )
                                {
                                    i = "door";
                                }
                                else
                                {
                                    one_argument( (char *) arg2, fname );
                                    i = fname;
                                }
                                break;
                        }
                    }
                    break;

                default:
                    fColour = FALSE;
                    *point++ = *str++;
                    break;
            }

             ++str;
            if( i )
            {
                while( ( *point = *i ) != '\0' )
                {
                    ++point;
                    ++i;
                }
            }
         }

        *point++        = '\n';
        *point++        = '\r';
        *point          = '\0';
        buf[0]          = UPPER( buf[0] );
        pbuff           = buffer;
        colourconv( pbuff, buf, to );
// Stop people writing notes seeing stuff sent by act

/* Orig Code
        if( to->desc )
            write_to_buffer( to->desc, buffer, 0 );
* New Code */
       if (to->desc && (to->desc->connected == CON_PLAYING))
                       write_to_buffer( to->desc, buffer, 0 );
     }

     return;
 }

int colour( char type, CHAR_DATA *ch, char *string )
{
    char        code[ 20 ];
    char        *p = '\0';

    if( IS_NPC( ch ) )
        return( 0 );

    switch( type )
    {
        default:
            sprintf( code, CLEAR );
            break;
        case 'x':
            sprintf( code, CLEAR );
            break;
        case 'b':
            sprintf( code, C_BLUE );
            break;
        case 'c':
            sprintf( code, C_CYAN );
            break;
        case 'g':
            sprintf( code, C_GREEN );
            break;
        case 'm':
            sprintf( code, C_MAGENTA );
            break;
        case 'r':
            sprintf( code, C_RED );
            break;
        case 'w':
            sprintf( code, C_WHITE );
            break;
        case 'y':
            sprintf( code, C_YELLOW );
            break;
        case 'B':
            sprintf( code, C_B_BLUE );
            break;
        case 'C':
            sprintf( code, C_B_CYAN );
            break;
        case 'G':
            sprintf( code, C_B_GREEN );
            break;
        case 'M':
            sprintf( code, C_B_MAGENTA );
            break;
        case 'R':
            sprintf( code, C_B_RED );
            break;
        case 'W':
            sprintf( code, C_B_WHITE );
            break;
        case 'Y':
            sprintf( code, C_B_YELLOW );
            break;
        case 'D':
            sprintf( code, C_D_GREY );
            break;
        case '*':
                switch (number_range(1,14))
                { default : sprintf( code, C_D_GREY); break;
                  case 1 : sprintf( code, C_RED); break;
                  case 2 : sprintf( code, C_B_RED); break;
                  case 3 : sprintf( code, C_GREEN); break;
                  case 4 : sprintf( code, C_B_GREEN); break;
                  case 5 : sprintf( code, C_YELLOW); break;
                  case 6 : sprintf( code, C_B_YELLOW); break;
                  case 7 : sprintf( code, C_BLUE); break;
                  case 8 : sprintf( code, C_B_BLUE); break;
                  case 9 : sprintf( code, C_MAGENTA); break;
                  case 10 : sprintf( code, C_B_MAGENTA); break;
                  case 11 : sprintf( code, C_CYAN); break;
                  case 12 : sprintf( code, C_B_CYAN); break;
                  case 13 : sprintf( code, C_WHITE); break;
                  case 14 : sprintf( code, C_B_WHITE); break;
                  case 15 : sprintf( code, C_D_GREY); break; }
        case '/':
            sprintf( code, "%c", 012 );
            break;
        case '{':
            sprintf( code, "%c", '{' );
            break;
     }

     p = code;
     while( *p != '\0' )
     {
        *string = *p++;
        *++string = '\0';
     }

     return( strlen( code ) );
}

void colourconv( char *buffer, const char *txt , CHAR_DATA *ch )
{
    const       char    *point;
                int     skip = 0;

    if( ch->desc && txt )
    {
        if( IS_SET( ch->act, PLR_COLOUR ) )
        {
            for( point = txt ; *point ; point++ )
            {
                if( *point == '{' )
                {
                    point++;
                    skip = colour( *point, ch, buffer );
                    while( skip-- > 0 )
                        ++buffer;
                    continue;
                }
                *buffer = *point;
                *++buffer = '\0';
            }
            *buffer = '\0';
        }
        else
        {
            for( point = txt ; *point ; point++ )
            {
                if( *point == '{' )
                {
                    point++;
                    continue;
                }
                *buffer = *point;
                *++buffer = '\0';
            }
            *buffer = '\0';
        }
    }
    return;
}

/*
 * Write to one char.
 */
void send_to_char_bw( const char *txt, CHAR_DATA *ch )
{
/*
    CHAR_DATA *wizard;
    CHAR_DATA *familiar;

    if ( ch->desc == NULL && IS_NPC(ch) && (wizard = ch->wizard) != NULL )
    {
	if (!IS_NPC(wizard) && (familiar = wizard->pcdata->familiar) != NULL 
	    && familiar == ch && ch->in_room != wizard->in_room)
	{
	    send_to_char("[ ",wizard);
	    if ( txt != NULL && wizard->desc != NULL )
		write_to_buffer( wizard->desc, txt, strlen(txt) );
	}
    }
*/
    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}



/*
 * The primary output interface for formatted output.
 */
void act_old( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
/*
    CHAR_DATA *to_old;
*/
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
/*
    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;
*/
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
/*
    bool is_fam;
*/
    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
/*
	is_fam = FALSE;
	to_old = to;
*/
	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;
/*
	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}
*/
	if ( to->desc == NULL || !IS_AWAKE(to) )
	{
/*
	    if (is_fam) to = to_old;
*/
	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {
/*
		if (is_fam) to = to_old;
*/
		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
		bug( "Act: missing arg2 for code %d.", *str );
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'N': i = PERS( vch, to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
/*
	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}
*/
	buf[0]   = UPPER(buf[0]);
if (to->desc && (to->desc->connected == CON_PLAYING))
	write_to_buffer( to->desc, buf, point - buf );
/*
	if (is_fam) to = to_old;
*/
    }
    return;
}

void act2( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *to;
/*
    CHAR_DATA *to_old;
*/
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
/*
    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;
*/
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
/*
    bool is_fam;
*/
    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	         if(to->wizard != NULL)to=to->wizard;
/*
	is_fam = FALSE;
	to_old = to;
*/
	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;
/*
	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}
*/
	if ( to->desc == NULL || !IS_AWAKE(to) )
	{
/*
	    if (is_fam) to = to_old;
*/
	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {
/*
		if (is_fam) to = to_old;
*/
		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
/*
		bug( "Act: missing arg2 for code %d.", *str );
*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  i = " "; break;
		case 'n': if ( ch != NULL ) i = PERS( ch,  to  );
			  else i = " ";
			  break;
		case 'N': if ( vch != NULL ) i = PERS( vch,  to  );
			  else i = " ";
			  break;
		case 'e': if ( ch != NULL ) i=he_she  [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'E': if (vch != NULL ) i=he_she  [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'm': if ( ch != NULL ) i=him_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'M': if (vch != NULL ) i=him_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 's': if ( ch != NULL ) i=his_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'S': if (vch != NULL ) i=his_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'p':
		    if (obj1 != NULL)
		    {
			i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;

		case 'P':
		    if (obj2 != NULL)
		    {
			i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
/*
	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}
*/
	buf[0]   = UPPER(buf[0]);
       if (to->desc && (to->desc->connected == CON_PLAYING))
        	write_to_buffer( to->desc, buf, point - buf );
/*
	if (is_fam) to = to_old;
*/
    }
    return;
}



void kavitem( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char kav[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    const char *str;
    const char *i;
    char *point;
    bool is_ok;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok) continue;
    	}
	if ( type == TO_CHAR && to != ch )
	    continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
		i = "";
	    else
	    {
		switch ( *str )
		{
		default:  i = "";					break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'o':
		    if (obj1 != NULL) sprintf(kav,"%s's",obj1->short_descr);
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "your" : kav)
			    : "something's";
		    break;

		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}



/*
 * Write to all characters.
 */
void send_to_all_char( const char *text )
{
    DESCRIPTOR_DATA *d;

    if ( text == NULL)
        return;
    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING )
	    send_to_char( text, d->character );

    return;
}


/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
         CHAR_DATA *ch;
         CHAR_DATA *victim;
         CHAR_DATA *tank;
   const char      *str;
   const char      *i;
         char      *point;
         char       buf  [ MAX_STRING_LENGTH ];
         char       buf2 [ MAX_STRING_LENGTH ];
	 bool       is_fighting = TRUE;

   if ( ( ch = d->character ) == NULL ) return;
   if ( ch->pcdata == NULL )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }
   if ( ch->position == POS_FIGHTING && ch->cprompt[0] == '\0' )
   {
      if ( ch->prompt[0] == '\0' )
      {
         send_to_char( "\n\r\n\r", ch );
         return;
      }
      is_fighting = FALSE;
   }
   else if ( ch->position != POS_FIGHTING && ch->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }

   point = buf;
   if ( ch->position == POS_FIGHTING && is_fighting )
      str = d->original ? d->original->cprompt : d->character->cprompt;
   else
      str = d->original ? d->original->prompt : d->character->prompt;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
         case 'h' :
            sprintf( buf2, "%ld", ch->hit );
            COL_SCALE(buf2, ch, ch->hit, ch->max_hit);
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%ld", ch->max_hit );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%ld", ch->mana                              );
            COL_SCALE(buf2, ch, ch->mana, ch->max_mana);
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%ld", ch->max_mana );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%ld", ch->move                              ); 
            COL_SCALE(buf2, ch, ch->move, ch->max_move);
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%ld", ch->max_move);
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp                               );
            COL_SCALE(buf2, ch, ch->exp, 1000);
            i = buf2; break;
         case 'g' :
            sprintf( buf2, "%d", ch->gold                              );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'q' :
            sprintf( buf2, "%d", ch->pcdata->quest                     );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'f' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, GREY);
            }
            else
            {
            if (victim->hit < 1 || victim->max_hit < 1)
               {strcpy(buf2, "Incapacitated");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((victim->hit*100/victim->max_hit) < 17)
               {strcpy(buf2, "Crippled");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((victim->hit*100/victim->max_hit) < 34)
               {strcpy(buf2, "Mauled");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((victim->hit*100/victim->max_hit) < 50)
               {strcpy(buf2, "Wounded");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((victim->hit*100/victim->max_hit) < 75)
               {strcpy(buf2, "Injured");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((victim->hit*100/victim->max_hit) < 100)
               {strcpy(buf2, "Hurt");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((victim->hit*100/victim->max_hit) >= 100)
               {strcpy(buf2, "Bruised");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((victim->hit*100/victim->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'F' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, GREY);
            }
            else if ( ( tank = victim->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, GREY);
            }
            else
            {
if (tank->hit < 1 || tank->max_hit < 1)
               {strcpy(buf2, "Incapacitated");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((tank->hit*100/tank->max_hit) < 17)
               {strcpy(buf2, "Crippled");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((tank->hit*100/tank->max_hit) < 34)
               {strcpy(buf2, "Mauled");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((tank->hit*100/tank->max_hit) < 50)
               {strcpy(buf2, "Wounded");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((tank->hit*100/tank->max_hit) < 75)
               {strcpy(buf2, "Injured");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((tank->hit*100/tank->max_hit) < 100)
               {strcpy(buf2, "Hurt");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((tank->hit*100/tank->max_hit) >= 100)
               {strcpy(buf2, "Bruised");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((tank->hit*100/tank->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'n' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'N' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else if ( ( tank = victim->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( ch == tank )
                  strcpy(buf2, "You");
               else if ( IS_AFFECTED(tank, AFF_POLYMORPH) )
                  strcpy(buf2, tank->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, tank->short_descr);
               else
                  strcpy(buf2, tank->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'a' :
            sprintf( buf2, "%s", IS_GOOD( ch ) ? "good"
		                  : IS_EVIL( ch ) ? "evil" : "neutral" );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'A' :
            sprintf( buf2, "%d", ch->alignment                      );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'r' :
            if( ch->in_room )
               sprintf( buf2, "%s", ch->in_room->name                  );
            else
               sprintf( buf2, " "                                      );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'R' :
            if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_WEREWOLF) || IS_CLASS(ch, CLASS_VAMPIRE)))
            {
               sprintf( buf2, "%d", ch->pcdata->stats[UNI_RAGE]);
               ADD_COLOUR(ch, buf2, D_RED);
            }
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'b' :
            sprintf( buf2, "%d", ch->beast );
            ADD_COLOUR(ch, buf2, GREY);
            i = buf2; break;
         case 'B' :
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
            {
               sprintf( buf2, "%d", ch->pcdata->condition[COND_THIRST] );
               ADD_COLOUR(ch, buf2, D_RED);
            }
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'c' :
            sprintf( buf2, "%d", char_ac(ch) );
            i = buf2; break;
         case 'p' :
            sprintf( buf2, "%d", char_hitroll(ch) );
            COL_SCALE(buf2, ch, char_hitroll(ch), 200);
            i = buf2; break;
         case 'P' :
            sprintf( buf2, "%d", char_damroll(ch) );
            COL_SCALE(buf2, ch, char_damroll(ch), 200);
            i = buf2; break;
         case 's' :
                 if (ch->race <= 0 ) strcpy(buf2,"Avatar");
            else if (ch->race <= 10 ) strcpy(buf2,"Immortal");
            else if (ch->race <= 20 ) strcpy(buf2,"Godling");
            else if (ch->race <= 30) strcpy(buf2,"Demigod");
            else if (ch->race <= 40) strcpy(buf2,"Lesser God");
            else if (ch->race <= 50) strcpy(buf2,"Greater God");
            else if (ch->race <= 60) strcpy(buf2,"Supreme God"); 
	    else if (ch->race <= 70) strcpy(buf2,"Ultimate God");
	    else                     strcpy(buf2,"Ultimate God");
            i = buf2; break;
         case 'S' :
            sprintf( buf2, "%d", ch->race );
            i = buf2; break;
         case 'o' :
            if (!IS_NPC(ch) && ch->pcdata->stage[2]+25 >= ch->pcdata->stage[1]
		&& ch->pcdata->stage[1] > 0)
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'O' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "no" );
            else if (!IS_NPC(victim) && victim != NULL && victim->pcdata->stage[1] > 0
		&& victim->pcdata->stage[2]+25 >= victim->pcdata->stage[1])
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'l' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "Nobody" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;

	 case 'z' :		/* OLC */
            if ( IS_IMMORTAL( ch ) )
            {
                switch (ch->desc->connected)
                {
                case CON_AEDITOR:
                    sprintf( buf2, "AEdit" );
                    break;
                case CON_REDITOR:
                    sprintf( buf2, "REdit" );
                    break;
                case CON_OEDITOR:
                    sprintf( buf2, "OEdit" );
                    break;
                case CON_MEDITOR:
                    sprintf( buf2, "MEdit" );
                    break;
                default:
                    buf2[0] = '\0';
                    break;
                }
            }
	case 'Z' :
          if ( !IS_NPC( ch ) && IS_IMMORTAL( ch ) )
             {
             if ( IS_SET( ch->act,PLR_WIZINVIS ) && IS_SET( ch->act, PLR_INCOG ) )
                sprintf(buf2, "Invis Incog");
             else if (IS_SET( ch->act,PLR_WIZINVIS ) )
                sprintf(buf2, "Invis");
             else if (IS_SET( ch->act,PLR_INCOG ) )
                sprintf(buf2, "Incog");
             else
                sprintf(buf2, "Visible");
             }
          else
              sprintf( buf2, " " );
            i = buf2; break;
        case '%' :
            sprintf( buf2, "%%"                                        );
            i = buf2; break;
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, buf, point - buf );
   return;
}
/*        case '%' :
            sprintf( buf2, "%%"                                        );
            i = buf2; break;
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, buf, point - buf );
   return;
}
*/

void file_wipe(CHAR_DATA *ch, char *argument)
{
    FILE *fp;
    fflush(fpReserve);
    fclose(fpReserve);
    if ((fp = fopen(str_dup(argument), "w")) == NULL) 
    {
	bug("File_wipe: fopen", 0);
	perror(argument);
    }
    else fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
    fpReserve = fopen(NULL_FILE, "r");
    return;
}

void do_login(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    send_to_char("-= Login Information =-\n\r", ch);
    sprintf(buf, "Total number of login attempts: %d\n\r", 
	total_number_login);
    send_to_char(buf, ch);
    sprintf(buf, "Highest number of logins at one time: %d\n\r", 
	high_number_login);
    send_to_char(buf, ch);
    send_to_char("Connection types:\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_NEWBIE))
	send_to_char("...Any newbies? YES.\n\r", ch);
    else
	send_to_char("...Any newbies? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_MORTAL))
	send_to_char("...Any mortals? YES.\n\r", ch);
    else
	send_to_char("...Any mortals? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_GOD))
	send_to_char("...Any gods? YES.\n\r", ch);
    else
	send_to_char("...Any gods? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_VAMPIRE))
	send_to_char("...Any vampires? YES.\n\r", ch);
    else
	send_to_char("...Any vampires? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_WEREWOLF))
	send_to_char("...Any werewolves? YES.\n\r", ch);
    else
	send_to_char("...Any werewolves? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_MAGE))
	send_to_char("...Any mages? YES.\n\r", ch);
    else
	send_to_char("...Any mages? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_DEMON))
	send_to_char("...Any demons? YES.\n\r", ch);
    else
	send_to_char("...Any demons? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_HIGHLANDER))
	send_to_char("...Any highlanders? YES.\n\r", ch);
    else
	send_to_char("...Any highlanders? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_DROW))
	send_to_char("...Any drow? YES.\n\r", ch);
    else
	send_to_char("...Any drow? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_NINJA))
	send_to_char("...Any ninjas? YES.\n\r", ch);
    else
	send_to_char("...Any ninjas? NO.\n\r", ch);
    if (IS_SET(chars_logged_in, LOGIN_MONK))
	send_to_char("...Any monks? YES.\n\r", ch);
    else
	send_to_char("...Any monks? NO.\n\r", ch);
    return;
}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

void sig_handler(int sig)
{
  switch(sig)
  {
    case SIGBUS:
      bug("Sig handler SIGBUS.",0);
      do_auto_shutdown();
      break;
    case SIGTERM:
      bug("Sig handler SIGTERM.",0);
      do_auto_shutdown();
      break;
    case SIGABRT:
      bug("Sig handler SIGABRT",0);
      do_auto_shutdown();
     case SIGSEGV:
      bug("Sig handler SIGSEGV",0);
      do_auto_shutdown();
      break;
  }
}

void init_signals()
{
  signal(SIGBUS,sig_handler);
  signal(SIGTERM,sig_handler);
  signal(SIGABRT,sig_handler);
  signal(SIGSEGV,sig_handler);
}

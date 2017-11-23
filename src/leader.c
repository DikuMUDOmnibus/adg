
 /***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "leader.h"

void do_hiscoset( CHAR_DATA *ch, char *argument )
/* Hiscoset command */
{
	char cmd[ MAX_INPUT_LENGTH ];
	char kwd[ MAX_INPUT_LENGTH ];
    char pnf[ MAX_INPUT_LENGTH ];
	char set_val[ MAX_INPUT_LENGTH ];
	HISCORE *table;
	HISCORE_ENTRY *entry;
	sh_int i;
	bool cmd_found;

	if( argument[0] == '\0' )
	{
		send_to_char("Syntax: hiscoset <command> [table keyword] [field] [value]\n\r", ch );
		send_to_char("\n\r", ch );
		send_to_char("    Command being one of:\n\r", ch );
		send_to_char("    list                     Lists all hiscore tables.\n\r", ch );
		send_to_char("    show <kwd>               Shows the data for that table.\n\r", ch );
		send_to_char("    create <kwd>             Create new table with that keyword.\n\r", ch );
		send_to_char("    destroy <kwd>            Destroy table with that keyword.\n\r", ch );
		send_to_char("    clear                    Clears the table\n\r", ch );
		send_to_char("    set <kwd> <field> <val>  Sets data for that table.\n\r", ch );
		send_to_char("\n\r", ch );
		send_to_char("    Field being one of:\n\r", ch );
		send_to_char("    keyword <str>            Set new keyword for the table.\n\r", ch );
		send_to_char("    vnum <num>               Set an object vnum for this table.\n\r", ch );
		send_to_char("    desc <str>               Set the description.\n\r", ch );
		send_to_char("    maxlength <num>          Set Max entries in the table.\n\r", ch );
		send_to_char("    entry <name> <score>     Change the score for a player.\n\r", ch );
		send_to_char("\n\r", ch );
		send_to_char("    See HELP HISCOSET for more details.\n\r", ch );
		return;
	}

	argument = one_argument( argument, cmd );

	if( !str_cmp( cmd, "list" ) )
	{
		send_to_char( "List of hiscore tables: \n\r", ch );
		for( table = first_table ; table ; table = table->next )
		{
			sprintf( pnf, "    Table '%s' (%s), %d entries, vnum is %d\n\r",
				table->keyword, table->desc, table->length, table->vnum );
             stc(pnf,ch);
		}
		if( !first_table )
			send_to_char( "    No tables found.\n\r", ch );
		return;
	}

	argument = one_argument( argument, kwd );
	if( kwd[0] != '\0' )
		table = find_table( kwd );
	else
		table = NULL;
	cmd_found = FALSE;

	if( !str_cmp( cmd, "create" ) && (cmd_found=TRUE) && kwd[0] != '\0' )
	{
		if( table != NULL )
		{
			send_to_char( "A table with that keyword already exists.\n\r", ch );
			return;
		}
		create_hitable( kwd, "Description not set yet" );
		sprintf( pnf, "Table '%s' created.\n\r", kwd );
stc(pnf,ch);
		save_hiscores();
		return;
	}
	else if( !str_cmp( cmd, "show" )  && (cmd_found=TRUE) && table != NULL )
	{
		sprintf( pnf, "Hiscore table, keyword '%s':\n\r", kwd);
stc(pnf,ch);
		sprintf( pnf, "Description: %s\n\r", table->desc );
stc(pnf,ch);
		sprintf( pnf, "Vnum: %d    MaxLength: %d Length: %d\n\r",
			table->vnum, table->max_length, table->length );
stc(pnf,ch);
		for( entry = table->first_entry, i=1 ; entry ; entry = entry->next, i++ )
			sprintf( pnf, "%d. Name: %-15.15s    Score: %d\n\r",
				i, capitalize(entry->name), entry->score );
stc(pnf,ch);
		return;
	}
	else if( !str_cmp( cmd, "clear" ) && (cmd_found=TRUE) && table )
	{
		if( clear_hitable(kwd) )
                {
			sprintf( pnf, "Table '%s' cleared succesfully.\n\r", kwd );
                        stc(pnf,ch);
                }
		else
                {
		 	sprintf( pnf, "Table '%s' could not be cleared.\n\r", kwd );
                        stc(pnf,ch);
                }
		return;
	}
	else if( !str_cmp( cmd, "destroy" ) && (cmd_found=TRUE) && table != NULL )
	{
		if( destroy_hitable( kwd ) )
                {
			sprintf( pnf, "Table '%s' destroyed.\n\r", kwd );
                        stc(pnf,ch);
                }
		else
		{
			sprintf( pnf, "ERROR: Couldn't destroy table '%s'.\n\r", kwd );
                        stc(pnf,ch);
			LINK( table, first_table, last_table, next, prev );
		}
		save_hiscores();
		return;
	}
	else if( !str_cmp( cmd, "set" ) )
		cmd_found = TRUE;

	if( !cmd_found )
	{
		sprintf( pnf, "Invalid command '%s' for hiscoset.\n\r", cmd );
                stc(pnf,ch);
		return;
	}
	if( kwd[0] == '\0' )
	{
		send_to_char( "Please specify a table keyword.\n\r", ch );
		return;
	}
	if( table == NULL )
	{
		sprintf( pnf, "No such table '%s'.\n\r", kwd );
stc(pnf,ch);
		return;
	}

	/* If it reached here, its "hiscoset set" and table is valid */
	argument = one_argument( argument, cmd );
	if( cmd[0] == '\0' )
	{
		send_to_char( "Hiscoset set what?\n\r", ch );
		return;
	}

	cmd_found = FALSE;
	if( !str_cmp( cmd, "desc" ) && (cmd_found=TRUE) && argument[0] != '\0' )
	{
		free_string( table->desc );
		table->desc = str_dup( argument );
		sprintf( pnf, "Ok. Description for table '%s' is now '%s'.\n\r",
			kwd, argument );
stc(pnf,ch);
save_hiscores();
		return;
	}
	argument = one_argument( argument, set_val );
	if( !str_cmp( cmd, "keyword" ) && (cmd_found=TRUE) && set_val[0] != '\0' )
	{
		free_string( table->keyword );
		table->keyword = str_dup( set_val );
		sprintf( pnf, "Ok. Keyword for table '%s' is now '%s'.\n\r",
			kwd, set_val );
stc(pnf,ch);
		save_hiscores();
		return;
	}
	else if( !str_cmp( cmd, "vnum" ) && (cmd_found=TRUE) && set_val[0] != '\0' )
	{
		if( is_number( set_val) )
		{
			table->vnum = atoi( set_val ) ;
			sprintf( pnf, "Ok. Vnum for table '%s' is now '%d'.\n\r",
				kwd, table->vnum );
stc(pnf,ch);
			save_hiscores();
		}
		else
			send_to_char( "Argument for set vnum must be numeric.\n\r", ch );
		return;
	}
	else if( !str_cmp( cmd, "maxlength" ) && (cmd_found=TRUE) &&
		set_val[0] != '\0' )
	{
		if( is_number( set_val) )
		{
			table->max_length = atoi( set_val ) ;
			sprintf( pnf, "Ok. Max length for table '%s' is now '%d'.\n\r",
				kwd, table->max_length );
stc(pnf,ch);
			fix_table_length( table );
			save_hiscores();
		}
		else
			send_to_char( "Argument for set maxlength must benumeric.\n\r", ch );
		return;
	}
	else if( !str_cmp( cmd, "entry" ) )
		cmd_found = TRUE;

	if( !cmd_found )
	{
		sprintf( pnf, "Invalid value '%s' for hiscoset set.\n\r",
			cmd );
stc(pnf,ch);
		return;
	}
	if( set_val[0] == '\0' )
	{
		sprintf( pnf, "Hiscoset set %s to what?.\n\r", cmd );
stc(pnf,ch);
		return;
	}

	/* at this point, its hiscoset set entry .. */
	if( argument == '\0' || !is_number( argument) )
	{
		send_to_char( "Second argument to set entry must be numberic.\n\r", ch );
		return;
	}

	sprintf( pnf, "New score for %s set to %s.\n\r", set_val, argument );
stc(pnf,ch);
	if( add_hiscore( kwd, set_val, atoi(argument) ) )
	{
		sprintf( pnf, "New position is %d.\n\r",
			get_position( kwd, set_val ) );
stc(pnf,ch);
	}
	else
		sprintf( pnf, "They are out of the table.\n\r" );
stc(pnf,ch);
	save_hiscores();
}



void do_hiscore( CHAR_DATA *ch, char *argument )
/* Hiscore command */
{
	HISCORE *table;
	bool some_table;
	OBJ_INDEX_DATA *oindex;
	char   arg [MIL];
        char pnf[MSL];

        adjust_hiscore( "status", ch, ch->race );
        adjust_hiscore( "pdeath", ch, ch->pdeath );
        adjust_hiscore( "pkill", ch, ch->pkill );
    adjust_hiscore( "qp", ch, ch->pcdata->score[SCORE_QUEST] );
	if( argument[0] == '\0' )
	{
		strcpy( arg, "status" );
		some_table = FALSE;
		send_to_char( "Hiscore tables in this game:\n\r", ch );
		for( table = first_table ; table ; table = table->next )
		{
			if( table->vnum == -1 )
			{
				sprintf( pnf, "  %s: keyword '%s'\n\r",
					table->desc, table->keyword );
stc(pnf,ch);
				some_table = TRUE;
			}
			else
			{
				oindex = get_obj_index( table->vnum );
				if( oindex != NULL )
				{
					sprintf( pnf, "  %s: in object %s\n\r",
						table->desc,
						oindex->short_descr );
stc(pnf,ch);
					some_table = TRUE;
				}
			}
		}
		if( !some_table )
		{
			send_to_char( "  None.\n\r", ch );
			return;
		}
	}
	if( argument[0] == '\0' )
		table = find_table( "status" );
	else
		table = find_table( argument );
	if( table == NULL )
	{
		send_to_char( "No such hiscore table.\n\r", ch );
		return;
	}

 	if( table->vnum != -1 && !IS_IMMORTAL(ch) )
	{
		send_to_char( "That hiscore table is attached to an object. Go see the object.\n\r", ch );
		return;
	}

	show_hiscore( table->keyword, ch );
}

char *is_hiscore_obj( OBJ_DATA *obj )
/* If the given object is marked as a hiscore table object, it returns
 * a pointer to the keyword for that table; otherwise returns NULL */
{
	HISCORE *table;

	for( table = first_table ; table ; table = table->next )
	{
		if( obj->pIndexData->vnum == table->vnum )
			return table->keyword;
	}
	return NULL;
}

/*
(@)                                                 (@)
| |-------------------------------------------------| |
| |                                                 | |
| |      Greatest Pkillers of Frozen Wasteland      | |
| |                                                 | |
| |      1. Darklord ..........|.......... 1000     | |
| |      2. Pepto .............|............ 50     | |
| |     10. 12345678901234567890123456789012345     | |
| |     1234                                        | |
| |-------------------------------------------------| |
(@)                                                 (@)
   width =
01234567890123456789012345678901234567890123456789012345678901234567890123456789
0         1         2         3         4         5         6         7
3 + 5 + text + 5 + 3 = 16 + text
text = 4 + 20 + 15 = 39
39 + 16 = 49+6 = 55
*/
/* this is width of text only,
 * add 13 to get actual width */

void show_hiscore( char *keyword, CHAR_DATA *ch )
/* Shows a hiscore table to a player in a (more or less) nicely formatted
 * way. */
{
	HISCORE *table;
	HISCORE_ENTRY *entry;
	sh_int num, len;
	char buf[ MAX_STRING_LENGTH ];
        char pnf[MSL];
	bool odd;

	table = find_table( keyword );
	if( table == NULL )
	{
		bug( "show_hiscore: no such table ", 0 );
		return;
	}

//	sprintf( pnf, "#G(#r@#G)                                                 (#r@#G)#w\n\r" );
//stc(pnf,ch);
	sprintf( pnf, "{D  +-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+{x\n\r" );
stc(pnf,ch);
	sprintf( pnf, "{D  |                                                 |{x\n\r" );
stc(pnf,ch);
	sprintf( buf, "%s", table->desc );
	len = strlen( buf );
	len = (39 - len);
	odd = (len%2) == 1;
	len /= 2;
	if( len < 0 )
		len = 0;
	for( num=0; num<len ; num++ )
		buf[num] = ' ';
	buf[num] = '\0';
	sprintf( pnf, "{D  |     #P%s%s%s%s     {D|{x\n\r",
		buf, table->desc,
		odd ? " " : "", buf );
stc(pnf,ch);
	sprintf( pnf, "{D  |                                                 |{x\n\r");
stc(pnf,ch);
	num = 1;
	for( entry = table->first_entry ; entry ; entry = entry->next )
	{
		sprintf( pnf, "{D  |     %s%2d. %-20.20s%15d     {D|{x\n\r",
			num <= 3 ? "{c" : "#P",
			num, capitalize(entry->name), entry->score );
stc(pnf,ch);
		num++;
	}

	sprintf( pnf, " {D |                                                 |{x\n\r" );
stc(pnf,ch);
	sprintf( pnf, "{D  +-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-+{x\n\r" );
stc(pnf,ch);
//	sprintf( pnf, "#G(#r@#G)                                                 (#r@#G)#w\n\r" );
//stc(pnf,ch);
}
void adjust_hiscore( char *keyword, CHAR_DATA *ch, int score )
/* Adjusts the hiscore for that character in that table
 * and sends message to the character and to the whole mud
 * if the player entered the table */
{
	char buf[ MAX_STRING_LENGTH ];
        char pnf[MSL];
	sh_int pos, old_pos;
	HISCORE *table;

	if( IS_IMMORTAL(ch) )
	    return;

	old_pos = get_position( keyword, ch->name);
	add_hiscore( keyword, ch->name, score );
	pos = get_position( keyword, ch->name );
	table = find_table( keyword );
	if( pos != -1 && pos != old_pos && table != NULL )
	{
		sprintf( pnf, "You have reached position %d in the table of '%s'.\n\r",
			pos, table->desc );
stc(pnf,ch);
		/* replace for info channel */
		sprintf( buf, "#y%s has reached position %d in the table of '%s'\n\r",
			ch->name, pos, table->desc );
		do_info( ch, buf );
	}
	else if( pos != -1 && table != NULL )
	{
		sprintf( pnf, "You remain in position %d in the table of '%s'.\n\r",
			pos, table->desc );
stc(pnf,ch);
	}
}

bool add_hiscore( char *keyword, char *name, int score )
/* Sets the score for 'name'. If name is already on the table, the old
 * score is discarded and the new one takes effect.
 * Returns TRUE if as a result of this call, name was added to the
 * table or remained there. */
{
	HISCORE *table;
	HISCORE_ENTRY *entry, *new_entry;
	sh_int i;
	bool added;

	table = find_table( keyword );
	if( table == NULL )
	{
		bug( "add_hiscore: table not found", 0 );
		return FALSE;
	}

	for( entry = table->first_entry ; entry ; entry = entry->next )
	{
		if( !str_cmp( entry->name, name ) )
		{
			UNLINK( entry, table->first_entry, table->last_entry, next, prev );
			free_string( entry->name );
			free_mem( entry , sizeof(*entry));
			table->length--;
			break;
		}
	}

	added = FALSE;
	new_entry = NULL;
	for( i=1, entry = table->first_entry ; i<= table->max_length ; entry = entry->next, i++ )
	{
		if( !entry )
		/* there are empty slots at end of list, add there */
		{
			new_entry = alloc_mem( sizeof(HISCORE_ENTRY));
			new_entry->name = str_dup( name );
			new_entry->score = score;
			LINK( new_entry, table->first_entry,table->last_entry, next, prev );
			table->length++;
			added = TRUE;
			break;
		}
		else if( score > entry->score )
		{
		        new_entry = alloc_mem( sizeof(HISCORE_ENTRY) );
			new_entry->name = str_dup( name );
			new_entry->score = score;
			INSERT( new_entry, entry, table->first_entry, next, prev );
			table->length++;
			added = TRUE;
			break;
		}
	}

	fix_table_length( table );

	if( added )
		save_hiscores ();

	return added;
}
void fix_table_length( HISCORE *table )
/* Chops entries at the end of the table if the table
 * has exceeded its maximum length */
{
	HISCORE_ENTRY *entry;

	while( table->length > table->max_length )
	{
		table->length--;
		entry = table->last_entry;
		UNLINK( entry, table->first_entry, table->last_entry, next, prev );
		free_string( entry->name );
		free_mem( entry, sizeof(* entry));
	}
}

sh_int get_position( char *keyword, char *name )
/* Returns the position of 'name' within the table of that keyword, or
 * -1 if 'name' is not in that table */
{
	sh_int i;
	HISCORE *table;
	HISCORE_ENTRY *entry;

	table = find_table( keyword );
	if( !table )
		return -1;

	i = 1;
	for( entry = table->first_entry ; entry ; entry = entry->next )
	{
		if( !str_cmp( entry->name, name) )
			return i;
		i++;
	}

	return -1;
}

HISCORE *find_table( char *keyword )
/* Returns a pointer to the table for the given keyword
 * or NULL if there's no such table */
{
	HISCORE *table;

	for( table = first_table ; table ; table = table->next )
	{
		if( !str_cmp( table->keyword, keyword ) )
			return table;
	}
	return NULL;
}

void create_hitable( char *keyword, char *desc )
/* Creates a new hiscore table with the given keyword and description */
{
	HISCORE *new_table;

	new_table = alloc_mem( sizeof(HISCORE) );

	new_table->keyword = str_dup( keyword );
	new_table->desc = str_dup( desc );
	new_table->vnum = -1;
	new_table->max_length = 10;
	new_table->length = 0;
	new_table->first_entry = NULL;
	new_table->last_entry = NULL;
	LINK( new_table, first_table, last_table, next, prev );
}

bool destroy_hitable( char *keyword )
/* Destroyes a given hiscore table. Returns FALSE if error */
{
	HISCORE *table;
	HISCORE_ENTRY *entry, *nentry;

	table = find_table( keyword );
	if( !table )
		return FALSE;

	UNLINK( table, first_table, last_table, next, prev );
	free_string( table->keyword );
	free_string( table->desc );
	for( entry = table->first_entry ; entry ; entry = nentry )
	{
		nentry = entry->next;
		UNLINK( entry, table->first_entry, table->last_entry, next, prev );
		free_string( entry->name );
		free_mem( entry, sizeof( *entry) );
	}
	free_mem( table, sizeof(*table) );
	return TRUE;
}
bool clear_hitable( char *keyword )
/* Destroyes a given hiscore table. Returns FALSE if error */
{
	HISCORE *table;
	HISCORE_ENTRY *entry, *nentry;

	table = find_table( keyword );
	if( !table )
		return FALSE;

	for( entry = table->first_entry ; entry ; entry = nentry )
	{
		nentry = entry->next;
		UNLINK( entry, table->first_entry, table->last_entry, next, prev );
		free_string( entry->name );
		free_mem( entry, sizeof( *entry) );
	}
	return TRUE;
}

void save_hiscores( void )
/* Saves all hiscore tables */
{
	FILE *fp;
	char filename[ MAX_INPUT_LENGTH ];
	HISCORE *table;
	HISCORE_ENTRY *entry;

	sprintf( filename, "../data/hiscores.dat" );

	fclose( fpReserve );
	fp = fopen( filename, "w" );
	if( fp == NULL )
	{
		bug( "save_hiscores: fopen", 0 );
		return;
	}

	fprintf( fp, "#HISCORES\n" );

	for( table = first_table ; table ; table = table->next )
	{
		fprintf( fp, "#TABLE\n" );
		fprintf( fp, "Keyword    %s\n", table->keyword );
		fprintf( fp, "Desc       %s~\n", table->desc );
		fprintf( fp, "Vnum       %d\n", table->vnum );
		fprintf( fp, "MaxLength  %d\n", table->max_length );
		for( entry = table->first_entry ; entry ; entry = entry->next )
		{
			fprintf( fp, "Entry      %s %d\n", entry->name, entry->score);
		}
		fprintf( fp, "End\n\n" );
	}

	fprintf( fp, "#END\n" );

	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
}

void load_hiscores( void )
/* Loads all hiscore tables */
{
	char filename[MAX_INPUT_LENGTH];
	FILE *fp;
	HISCORE *new_table;

	sprintf(filename, "../data/hiscores.dat");

	fp = fopen(filename, "r");
	if( fp == NULL)
	{
		bug( "Invalid filename. Defaulting to 0 Hiscore tables", 0 );
	        return;
	}

	for(;;)
	{
		char letter;
		char *word;

		letter = fread_letter(fp);

		if(letter != '#')
		{
			bug("load_hiscores: # not found",0);
			return;
		}

		word = fread_word(fp);

		if(!str_cmp(word, "HISCORES"))
			;
		else if(!str_cmp(word, "END"))
			break;
		else if(!str_cmp(word, "TABLE"))
		{
			new_table = load_hiscore_table( fp );
			if( !new_table )
				continue;
			LINK( new_table, first_table, last_table, next,prev );
		}
		else
		{
			bug("load_hiscores: unknown field",0);
			break;
		}
	}

	fclose( fp );
	return;
}

HISCORE *load_hiscore_table( FILE *fp )
/* Loads one hiscore table from the file and returns it */
{
	char *word;
	HISCORE *new_table;
	HISCORE_ENTRY *new_entry;
	sh_int entry_count;

	new_table = alloc_mem( sizeof(HISCORE) );
	entry_count = 0;
	for(;;)
	{
		word = fread_word( fp );
		if( !str_cmp( word, "End" ))
			break;
		else if( !str_cmp( word, "Keyword" ))
			new_table->keyword = str_dup(fread_word(fp));
		else if( !str_cmp( word, "Desc" ))
			new_table->desc = fread_string( fp ) ;
		else if( !str_cmp( word, "Vnum" ))
			new_table->vnum = fread_number( fp );
		else if( !str_cmp( word, "MaxLength" ))
			new_table->max_length = fread_number( fp );
		else if( !str_cmp( word, "Entry" ))
		{
		        new_entry = alloc_mem( sizeof( HISCORE_ENTRY ) );
			new_entry->name = str_dup(fread_word( fp ));
			new_entry->score = fread_number( fp );
			/* LINK adds at the end of the table. */
			LINK( new_entry, new_table->first_entry, new_table->last_entry, next, prev );
			entry_count++;
		}
		else
		{
			bug("load_hiscore_table: unknown field",0);
			break;
		}
	}
	new_table->length = entry_count;
	if( entry_count > new_table->max_length )
	{
		bug( "load_hiscore_table: extra entries in table. fixed max_length",0 );
		new_table->max_length = entry_count;
	}

	if( new_table->keyword == NULL )
		new_table->keyword = str_dup( "error" );
	if( new_table->desc == NULL )
		new_table->desc = str_dup( "Error: no description" );

	return new_table;
}


void load_leaders args(( void ));
void save_leaders args(( void ));

void do_leaders( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  if ( IS_NPC(ch) ) return;

  centre_text("{r--==(++) The Leaders (++)==--{x",ch);
  line(ch);
  	  stc("{c       Name             Player Kills            Player Deaths{x\n\r",ch);
  line(ch);
  sprintf(buf,"      %-13s          %-4d                  %-4d\n\r",leader[0]->name,leader[0]->kills,leader[0]->deaths);
  stc(buf,ch);
  sprintf(buf,"      %-13s          %-4d                  %-4d\n\r",leader[1]->name,leader[1]->kills,leader[1]->deaths);
  stc(buf,ch);
  sprintf(buf,"      %-13s          %-4d                  %-4d\n\r",leader[2]->name,leader[2]->kills,leader[2]->deaths);
  stc(buf,ch);
  line(ch);
  centre_text("{y--==(++) The Fodder (++)==--{x",ch);
  line(ch);
  	  stc("{c       Name             Player Kills            Player Deaths{x\n\r",ch);
  line(ch);
  sprintf(buf,"      %-13s          %-4d                  %-4d\n\r",leader[5]->name,leader[5]->kills,leader[5]->deaths);
  stc(buf,ch);
  sprintf(buf,"      %-13s          %-4d                  %-4d\n\r",leader[4]->name,leader[4]->kills,leader[4]->deaths);
  stc(buf,ch);
  sprintf(buf,"      %-13s          %-4d                  %-4d\n\r",leader[3]->name,leader[3]->kills,leader[3]->deaths);
  stc(buf,ch);
  line(ch);
  return;
}

void swap_leader1( CHAR_DATA *ch )
{
  strcpy( leader[2]->name, leader[1]->name );
  leader[2]->kills = leader[1]->kills;
  leader[2]->deaths = leader[1]->deaths;

  strcpy( leader[1]->name, leader[0]->name );
  leader[1]->kills = leader[0]->kills;
  leader[1]->deaths = leader[0]->deaths;

  strcpy( leader[0]->name, ch->name );
  leader[0]->kills = ch->pkill;
  leader[0]->deaths = ch->pdeath;
  save_leaders();
  return;
}

void swap_leader2( CHAR_DATA *ch )
{
  strcpy( leader[2]->name, leader[1]->name );
  leader[2]->kills = leader[1]->kills;
  leader[2]->deaths = leader[1]->deaths;

  strcpy( leader[1]->name, ch->name );
  leader[1]->kills = ch->pkill;
  leader[1]->deaths = ch->pdeath;
  save_leaders();
  return;
}

void swap_leader3( CHAR_DATA *ch )
{
  strcpy( leader[2]->name, ch->name );
  leader[2]->kills = ch->pkill;
  leader[2]->deaths = ch->pdeath;
  save_leaders();
  return;
}

void swap_death1( CHAR_DATA *ch )
{
  strcpy( leader[5]->name, leader[4]->name );
  leader[5]->kills = leader[4]->kills;
  leader[5]->deaths = leader[4]->deaths;

  strcpy( leader[4]->name, leader[3]->name );
  leader[4]->kills = leader[3]->kills;
  leader[4]->deaths = leader[3]->deaths;

  strcpy( leader[3]->name, ch->name );
  leader[3]->kills = ch->pkill;
  leader[3]->deaths = ch->pdeath;
  save_leaders();
  return;
}

void swap_death2( CHAR_DATA *ch )
{
  strcpy( leader[4]->name, leader[3]->name );
  leader[4]->kills = leader[3]->kills;
  leader[4]->deaths = leader[3]->deaths;

  strcpy( leader[3]->name, ch->name );
  leader[3]->kills = ch->pkill;
  leader[3]->deaths = ch->pdeath;
  save_leaders();
  return;
}

void swap_death3( CHAR_DATA *ch )
{
  strcpy( leader[3]->name, ch->name );
  leader[3]->kills = ch->pkill;
  leader[3]->deaths = ch->pdeath;
  save_leaders();
  return;
}

void update_leaders( CHAR_DATA *ch )
{
  if ( IS_NPC(ch) ) return;

  if ( ch->pkill > leader[0]->kills && !IS_LEADER(ch))
  {
    swap_leader1( ch );
  }
  else if ( ch->pkill > leader[1]->kills && !IS_LEADER(ch) )
  {
    swap_leader2( ch );
  }
  else if ( ch->pkill > leader[2]->kills && !IS_LEADER(ch))
  {
    swap_leader3( ch );
  }

  if ( ch->pdeath > leader[3]->deaths && !IS_FODDER(ch) )
  {
    swap_death1( ch );
  }
  else if ( ch->pdeath > leader[4]->deaths && !IS_FODDER(ch) )
  {
    swap_death2( ch );
  }
  else if ( ch->pdeath > leader[5]->deaths && !IS_FODDER(ch))
  {
    swap_death3( ch );
  }
  else return;
  save_leaders();

  return;
}



void init_leaders( )
{
  int i = 0;

   for ( i = 0; i < 6; i++ )
   {
    if (leader[i] == NULL)
      leader[i] = alloc_perm( sizeof(*leader[i]) );
   }

    strcpy(leader[0]->name, "Sam");
    leader[0]->kills  = 0;
    leader[0]->deaths = 0;
    strcpy(leader[1]->name, "Sam");
    leader[1]->kills  = 0;
    leader[1]->deaths = 0;
    strcpy(leader[2]->name, "Sam");
    leader[2]->kills  = 0;
    leader[2]->deaths = 0;
    strcpy(leader[3]->name, "Sam");
    leader[3]->kills  = 0;
    leader[3]->deaths = 0;
    strcpy(leader[4]->name, "Sam");
    leader[4]->kills  = 0;
    leader[4]->deaths = 0;
    strcpy(leader[5]->name, "Sam");
    leader[5]->kills  = 0;
    leader[5]->deaths = 0;
    load_leaders();
}

void load_leaders( )
{
    FILE *fp;

    if ((fp = fopen("leaders.dat","r")) == NULL)
    {
        bug("Creating leaders.dat",0);
        save_leaders();
        return;
    }

    strcpy(leader[0]->name,fread_word(fp));
    leader[0]->kills = fread_number(fp);
    leader[0]->deaths = fread_number(fp);
    strcpy(leader[1]->name,fread_word(fp));
    leader[1]->kills = fread_number(fp);
    leader[1]->deaths = fread_number(fp);
    strcpy(leader[2]->name,fread_word(fp));
    leader[2]->kills = fread_number(fp);
    leader[2]->deaths = fread_number(fp);
    strcpy(leader[3]->name,fread_word(fp));
    leader[3]->kills = fread_number(fp);
    leader[3]->deaths = fread_number(fp);
    strcpy(leader[4]->name,fread_word(fp));
    leader[4]->kills = fread_number(fp);
    leader[4]->deaths = fread_number(fp);
    strcpy(leader[5]->name,fread_word(fp));
    leader[5]->kills = fread_number(fp);
    leader[5]->deaths = fread_number(fp);
    fclose( fp );
}

void save_leaders()
{
    FILE *fp;

    if ((fp = fopen("leaders.dat","w")) == NULL)
    {
        bug("Error saving to leaders.dat",0);
        return;
    }

    fprintf( fp, "%s %d %d\n",leader[0]->name,leader[0]->kills,leader[0]->deaths);
    fprintf( fp, "%s %d %d\n",leader[1]->name,leader[1]->kills,leader[1]->deaths);
    fprintf( fp, "%s %d %d\n",leader[2]->name,leader[2]->kills,leader[2]->deaths);
    fprintf( fp, "%s %d %d\n",leader[3]->name,leader[3]->kills,leader[3]->deaths);
    fprintf( fp, "%s %d %d\n",leader[4]->name,leader[4]->kills,leader[4]->deaths);
    fprintf( fp, "%s %d %d\n",leader[5]->name,leader[5]->kills,leader[5]->deaths);
    fclose( fp );
}



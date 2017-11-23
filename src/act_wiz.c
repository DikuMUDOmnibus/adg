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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include <unistd.h>
//#include "olc.h"

void sig_handler args((int sig));

/*
 * Local functions.
 */
//ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg));
void			call_all	args( ( CHAR_DATA *ch ) );
bool is_in              args( (char *, char *) ); 


void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level > LEVEL_ARCHMAGE
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_bamfin( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );
	free_string( ch->pcdata->bamfin );
	ch->pcdata->bamfin = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );
	free_string( ch->pcdata->bamfout );
	ch->pcdata->bamfout = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}

void do_godless( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
	return;
    }

    if ( ( (ch->level > 3)
	|| (ch->level < 2)
	|| (ch->trust > 3) )
	&& !IS_SET(ch->act, PLR_GODLESS))
    {
	send_to_char( "Sorry, you must be level 3.\n\r", ch );
	return;
    }

    if (IS_SET(ch->act, PLR_GODLESS) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->act, PLR_GODLESS);
	send_to_char("You now obey the gods.\n\r", ch);
	sprintf(buf,"%s now follows the whims of the gods.",ch->name);
	do_info(ch,buf);
    }
    else if (!IS_SET(ch->act, PLR_GODLESS) && !str_cmp(arg,"off")) {
	send_to_char("But you already obey the gods!\n\r", ch);
	return;}
    else if (!IS_SET(ch->act, PLR_GODLESS) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->act, PLR_GODLESS);
	send_to_char("You no longer obey the gods.\n\r", ch);
	sprintf(buf,"%s has rejected the gods.",ch->name);
	do_info(ch,buf);
    }
    else if (IS_SET(ch->act, PLR_GODLESS) && !str_cmp(arg,"on")) {
	send_to_char("But you have already rejected the gods!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_summon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch summon ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_SUMMON);
	send_to_char("You can no longer be the target of summon and portal.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_SUMMON);
	send_to_char("You can now be the target of summon and portal.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_transport( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch transport ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_TRANSPORT);
	send_to_char("You can no longer be the target of transport spells.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_TRANSPORT);
	send_to_char("You can now be the target of transport spells.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_watcher( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_WATCHER)
	|| (ch->level > 6)
	|| (ch->level < 2)
	|| (ch->trust > 0) )
{

    if ( (IS_NPC(ch)) || (ch->level < 12))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	do_users(ch,"");
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (ch == victim)
    {
	send_to_char( "Not on yourself.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "You better not, they are being snooped.\n\r", ch );
	return;
    }
    
    if(IS_SET(victim->act, PLR_WATCHER))
    {
    act( "$N is no longer a watcher.", ch, NULL, victim, TO_CHAR );
    REMOVE_BIT(victim->act, PLR_WATCHER);
    return;
    }

    SET_BIT(victim->act, PLR_WATCHER);
    act( "$n has made you a watcher.", ch, NULL, victim, TO_VICT );
    act( "You make $N a watcher.", ch, NULL, victim, TO_CHAR );
    return;
}
}



void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char *oldname;
        DESCRIPTOR_DATA *d;
        ROOM_INDEX_DATA *in_room;



    sprintf(buf,"%s: Deny %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	if (!char_exists(FALSE,arg))
	{
        send_to_char( "They aren't here.\n\r", ch );
	return;
        }
        oldname = str_dup(ch->name);
        d = ch->desc;
        do_autosave(ch,"");
        in_room = ch->in_room;
        extract_char(ch, TRUE, FALSE);
        d->character = NULL;
        load_char_obj(d, arg);
        ch = d->character;
        ch->next = char_list;
        char_list = ch;
        char_to_room(ch,in_room);

  	if ( !IS_SET(ch->act, PLR_DENY) )
    {
        SET_BIT(ch->act, PLR_DENY);
        send_to_char( "Player DENIED.\n\r", ch );
    }
    else
    {
        send_to_char("They are already DENIED.\n\r", ch );
    }
        
        d = ch->desc;
        do_autosave(ch,"");
        in_room = ch->in_room;
        extract_char(ch, TRUE, FALSE);
        d->character = NULL;
        load_char_obj(d, oldname);
        ch = d->character;
        ch->next = char_list;
        char_list = ch;
        char_to_room(ch,in_room);
        return;

/*
	send_to_char( "They aren't here.\n\r", ch );
	return;
*/
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    send_to_char( "OK.\n\r", ch );
    do_quit( victim, "" );

    return;
}


void do_undeny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    	char *oldname;
	CHAR_DATA *victim;
	DESCRIPTOR_DATA *d;
	ROOM_INDEX_DATA *in_room;

    sprintf(buf,"%s: Undeny %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Undeny whom?", ch );
	return;
    }

	if ((victim = get_char_world(ch, arg)) != NULL) {
	send_to_char("They are already online.\n\r", ch );
	return;}

    if (!char_exists(FALSE,arg))
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	oldname = str_dup(ch->name);
	d = ch->desc;
	do_autosave(ch,"");
	in_room = ch->in_room;
	extract_char(ch, TRUE, FALSE);
	d->character = NULL;
	load_char_obj(d, arg);
	ch = d->character;
	ch->next = char_list;
	char_list = ch;
	char_to_room(ch,in_room);    
	
 
    if ( IS_SET(ch->act, PLR_DENY) )
    {
	REMOVE_BIT(ch->act, PLR_DENY);
	send_to_char( "DENY removed.\n\r", ch );
    }
    else
    {
	send_to_char("They are not DENIED.\n\r", ch );
    }

	d = ch->desc;
	do_autosave(ch,"");
	in_room = ch->in_room;
	extract_char(ch, TRUE, FALSE);
	d->character = NULL;
	load_char_obj(d, oldname);
	ch = d->character;
	ch->next = char_list;
	char_list = ch;
	char_to_room(ch,in_room);    


    return;
}

/*
void do_finger( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char *oldname;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *in_room;


    one_argument( argument, arg );

if (!IS_IMMORTAL(ch) && ch->mkill < 5)
    {
	send_to_char( "You must kill at least 5 mobs before you can use this command.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Finger whom?", ch );
	return;
    }

    if ( ( victim = get_char_world(ch, arg) ) != NULL)
    {

    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

	line(ch);
          if ( victim->class == CLASS_VAMPIRE )
            sprintf(buf,"%s the Vampire",victim->name);
          else if ( victim->class == CLASS_WEREWOLF )
            sprintf(buf,"%s the Werewolf",victim->name);
	  else if ( victim->class == CLASS_MAGE )
	    sprintf(buf,"%s the Warlock",victim->name);
	  else if ( victim->class == CLASS_NINJA )
	    sprintf(buf,"%s the Ninja",victim->name);
	  else if ( victim->class == CLASS_MONK )
	    sprintf(buf,"%s the Monk",victim->name);
          else if ( victim->class == CLASS_DROW )
	    sprintf(buf,"%s the Drow",victim->name);
	  else if ( victim->class == CLASS_HIGHLANDER )
	    sprintf(buf,"%s the Highlander",victim->name);
	  else
	    sprintf(buf,"%s the Human",victim->name);	  
        buf[0] = UPPER(buf[0]);
	centre_text(buf,ch);
	line(ch);
	sprintf(buf,"Last connected from %s at %s",victim->lasthost, victim->lasttime );
        centre_text(buf,ch);
	line(ch);
        if ( victim->sex == 1 )
        {
          if ( strlen(victim->pcdata->marriage) > 2 )
          {
            if ( IS_EXTRA(victim, EXTRA_MARRIED) )
	     sprintf(buf, "They are Male and are married to %s.",victim->pcdata->marriage);
	    else
	     sprintf(buf, "They are Male and are engaged to %s.",victim->pcdata->marriage);
	  }
	  else
	    sprintf(buf, "They are Male.");
	}
	else if ( victim->sex == 2 )
        {
          if ( strlen(victim->pcdata->marriage) > 2 )
          {
            if ( IS_EXTRA(victim, EXTRA_MARRIED) )
	     sprintf(buf, "They are Female and are married to %s.",victim->pcdata->marriage);
	    else
	     sprintf(buf, "They are Female and are engaged to %s.",victim->pcdata->marriage);
	  }
	  else
	    sprintf(buf, "They are Female.");
	}
	else
        {
            sprintf(buf, "They are a hermaphrodite.");
        }
        centre_text(buf,ch);
        switch( victim->level )
        {
	  default:
	    centre_text("They are mortal.",ch);
            break;
          case LEVEL_AVATAR:
	  case LEVEL_APPRENTICE:
	  case LEVEL_MAGE:
	  case LEVEL_ARCHMAGE:
	    if ( victim->race < 1 )
              sprintf(buf,"They are an Avatar with %d legend status.", victim->lstatus);
            else if ( victim->race < 10 )
	      sprintf(buf,"They are an Immortal with %d legend status.", victim->lstatus );
            else if ( victim->race < 20 )
              sprintf(buf,"They are a Godling with %d legend status.", victim->lstatus);
            else if ( victim->race < 30 )
	      sprintf(buf,"They are a Demigod with %d legend status.", victim->lstatus);
            else if ( victim->race < 40 )
              sprintf(buf,"They are a Lesser God with %d legend status.", victim->lstatus);
	    else if ( victim->race < 50 )
              sprintf(buf,"They are a Greater God with %d legend status.", victim->lstatus);
            else if ( victim->race < 60 )
              sprintf(buf,"They are a Supreme God with %d legend status.", victim->lstatus);
            else if ( victim->race < 70 )
              sprintf(buf,"They are an Ultimate God with %d legend status.", victim->lstatus);
            else
              sprintf(buf,"They are an Ultimate God with %d legend status.", victim->lstatus);
            break;
          case LEVEL_BUILDER:
	    sprintf(buf,"They are a Builder.");
	    break;
          case LEVEL_QUESTMAKER:
	    sprintf(buf,"They are a Questmaker.");
            break;
          case LEVEL_ENFORCER:
            sprintf(buf,"They are an Enforcer.");
            break;
          case LEVEL_JUDGE:
            sprintf(buf,"They are a Judge.");
            break;
          case LEVEL_HIGHJUDGE:
            sprintf(buf,"They are a High Judge.");
            break;
          case LEVEL_IMPLEMENTOR:
            sprintf(buf,"They are an Implementor.");
            break;
	}
        centre_text(buf,ch);
        line(ch);
        sprintf(buf,"Arena Kills: %d    Arena Deaths: %d", victim->awins, victim->alosses);
        centre_text(buf,ch);
	sprintf(buf,"Player Kills: %d    Player Deaths: %d",victim->pkill,victim->pdeath);
        centre_text(buf,ch);
        sprintf(buf,"Mob Kills: %d    Mob Deaths: %d",victim->mkill, victim->mdeath );
        centre_text(buf,ch);
        line(ch);
        return;
    }

    if (!char_exists(FALSE,arg))
    {
	send_to_char( "That person doesn't even exist!\n\r", ch );
	return;
    }

	oldname = str_dup(ch->name);
	d = ch->desc;
	do_autosave(ch,"");
	in_room = ch->in_room;
	extract_char(ch, TRUE, FALSE);
	d->character = NULL;
	load_char_obj(d, arg);
	ch = d->character;
	ch->next = char_list;
	char_list = ch; 

	line(ch);
          if ( ch->class == CLASS_VAMPIRE )
            sprintf(buf,"%s the Vampire",ch->name);
          else if ( ch->class == CLASS_WEREWOLF )
            sprintf(buf,"%s the Werewolf",ch->name);
	  else if ( ch->class == CLASS_MAGE )
	    sprintf(buf,"%s the Warlock",ch->name);
	  else if ( ch->class == CLASS_NINJA )
	    sprintf(buf,"%s the Ninja",ch->name);
	  else if ( ch->class == CLASS_MONK )
	    sprintf(buf,"%s the Monk",ch->name);
          else if ( ch->class == CLASS_DROW )
	    sprintf(buf,"%s the Drow",ch->name);
	  else if ( ch->class == CLASS_HIGHLANDER )
	    sprintf(buf,"%s the Highlander",ch->name);
	  else
	    sprintf(buf,"%s the Human",ch->name);	  
        buf[0] = UPPER(buf[0]);
	centre_text(buf,ch);
	line(ch);
	sprintf(buf,"Last connected from %s at %s",ch->lasthost, ch->lasttime );
        centre_text(buf,ch);
	line(ch);
        if ( ch->sex == 1 )
        {
          if ( strlen(ch->pcdata->marriage) > 2 )
          {
            if ( IS_EXTRA(ch, EXTRA_MARRIED) )
	     sprintf(buf, "They are Male and are married to %s.",ch->pcdata->marriage);
	    else
	     sprintf(buf, "They are Male and are engaged to %s.",ch->pcdata->marriage);
	  }
	  else
	    sprintf(buf, "They are Male.");
	}
	else if ( ch->sex == 2 )
        {
          if ( strlen(ch->pcdata->marriage) > 2 )
          {
            if ( IS_EXTRA(ch, EXTRA_MARRIED) )
	     sprintf(buf, "They are Female and are married to %s.",ch->pcdata->marriage);
	    else
	     sprintf(buf, "They are Female and are engaged to %s.",ch->pcdata->marriage);
	  }
	  else
	    sprintf(buf, "They are Female.");
	}
	else
        {
            sprintf(buf, "They are a hermaphrodite.");
        }
        centre_text(buf,ch);
        switch( ch->level )
        {
	  default:
	    centre_text("They are mortal.",ch);
            break;
          case LEVEL_AVATAR:
	  case LEVEL_APPRENTICE:
	  case LEVEL_MAGE:
	  case LEVEL_ARCHMAGE:
	    if ( ch->race < 1 )
              sprintf(buf,"They are an Avatar with %d legend status.", ch->lstatus);
            else if ( ch->race < 10 )
	      sprintf(buf,"They are an Immortal with %d legend status.", ch->lstatus );
            else if ( ch->race < 20 )
              sprintf(buf,"They are a Godling with %d legend status.", ch->lstatus);
            else if ( ch->race < 30 )
	      sprintf(buf,"They are a Demigod with %d legend status.", ch->lstatus);
            else if ( ch->race < 40 )
              sprintf(buf,"They are a Lesser God with %d legend status.", ch->lstatus);
	    else if ( ch->race < 50 )
              sprintf(buf,"They are a Greater God with %d legend status.", ch->lstatus);
            else if ( ch->race < 60 )
              sprintf(buf,"They are a Supreme God with %d legend status.", ch->lstatus);
            else if ( ch->race < 70 )
              sprintf(buf,"They are an Ultimate God with %d legend status.", ch->lstatus);
            else
              sprintf(buf,"They are an Ultimate God with %d legend status.", ch->lstatus);
            break;
          case LEVEL_BUILDER:
	    sprintf(buf,"They are a Builder.");
	    break;
          case LEVEL_QUESTMAKER:
	    sprintf(buf,"They are a Questmaker.");
            break;
          case LEVEL_ENFORCER:
            sprintf(buf,"They are an Enforcer.");
            break;
          case LEVEL_JUDGE:
            sprintf(buf,"They are a Judge.");
            break;
          case LEVEL_HIGHJUDGE:
            sprintf(buf,"They are a High Judge.");
            break;
          case LEVEL_IMPLEMENTOR:
            sprintf(buf,"They are an Implementor.");
            break;
	}
        centre_text(buf,ch);
        line(ch);
	sprintf(buf,"Player Kills: %d    Player Deaths: %d",ch->pkill,ch->pdeath);
        centre_text(buf,ch);
        sprintf(buf,"Mob Kills: %d    Mob Deaths: %d",ch->mkill, ch->mdeath );
        centre_text(buf,ch);
        line(ch);
       
	d = ch->desc;
	do_autosave(ch,"");
	extract_char(ch, TRUE, TRUE);
	d->character = NULL;
	load_char_obj(d, oldname);
	ch = d->character;
	ch->next = char_list;
	char_list = ch;
	char_to_room(ch,in_room);    


    return;
}
*/
void do_finger( CHAR_DATA *ch, char *argument )
{
/* Re-written mostly by Kyrlin.
 */
    char strsave[MAX_INPUT_LENGTH];
    char *buf;
    char buf2[MAX_INPUT_LENGTH];
//    char buf3[MAX_INPUT_LENGTH];
    FILE *fp;
    int  num;
    int  num2;
    int  extra;   

    if ( IS_NPC(ch) ) return;

    if (!check_parse_name( argument ))
    {
        send_to_char( "Thats an illegal name.\n\r", ch );
        return;
    }

    if ( !char_exists(TRUE,argument) )
    {
        send_to_char( "That player doesn't exist.\n\r", ch );
        return;
    }
    
    fclose( fpReserve );
    sprintf( strsave, "%sbackup/%s", PLAYER_DIR, capitalize(argument) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {

send_to_char("{D=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
        buf=fread_string( fp );
        send_to_char(buf,ch);
        send_to_char(" ",ch);
        buf=fread_string( fp );
        send_to_char(buf,ch);
        send_to_char(".\n\r",ch);
send_to_char("{D=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
        send_to_char("#wLasthost:#n ",ch);
        buf=fread_string( fp );
	  if(IS_IMMORTAL(ch)) send_to_char(buf,ch);
	  else send_to_char("***.***.***.***",ch);
        send_to_char(" #won:#n ",ch);
        buf=fread_string( fp );
        if(IS_IMMORTAL(ch)) send_to_char(buf,ch);
        else send_to_char("Private",ch);
        send_to_char(".\n\r",ch);
send_to_char("{D=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
        extra=fread_number( fp );
        num=fread_number( fp );
        send_to_char("#wSex:#n ",ch);
        buf=fread_string( fp );
        if (num == SEX_MALE) send_to_char("Male. ",ch);
        else if (num == SEX_FEMALE)
        {
            send_to_char("Female. ",ch);
            if (IS_SET(extra, EXTRA_PREGNANT)) other_age(ch,extra,TRUE,buf);
        }
        else send_to_char("None. ",ch);
        buf=fread_string( fp ); 
        other_age(ch,extra,FALSE,buf);
        num=fread_number( fp );
        num2=fread_number( fp ); 
        switch ( num )
        {
            default:
                send_to_char("They are mortal, ",ch);
                break;
            case LEVEL_AVATAR:
            case LEVEL_APPRENTICE: 
            case LEVEL_MAGE:
            case LEVEL_ARCHMAGE:
            {
                if (num2 < 1)
                    send_to_char("They are a Peasant, ",ch);
                else if (num2 < 10)
		    send_to_char("They are a Page, ",ch);
                else if (num2 < 20)
                    send_to_char("They are a Knight, ",ch);
                else if (num2 < 30)
                    send_to_char("They are a Count, ",ch);
                else if (num2 < 40)
                    send_to_char("They are a Viscount, ",ch);
                else if (num2 < 50)
                    send_to_char("They are a Lord, ",ch);
               else if (num2 < 60)
                    send_to_char("They are a Baron, ",ch);
                else if (num2 < 70)
                    send_to_char("They are a Duke, ",ch);
               else
                    send_to_char("They are a Prince, ",ch);
                break;
            }
            case LEVEL_BUILDER:
                send_to_char("They are an Apprentice, ",ch);
                break;
            case LEVEL_QUESTMAKER:
                send_to_char("They are a Questor, ",ch);
                break;
            case LEVEL_ENFORCER:   
                send_to_char("They are a Councillor, ",ch);  
                break;
            case LEVEL_JUDGE:
                send_to_char("They are a Regent, ",ch);
                break;
            case LEVEL_HIGHJUDGE:
                send_to_char("They are a Pontifex, ",ch);
                break;
            case LEVEL_IMPLEMENTOR:
                send_to_char("They are an Implementor, ",ch);
                break;
        }
        num=fread_number( fp );   
        if (num > 0) num2 = (2*(num / 7200)); else num2 = 0;
        sprintf(buf2,"and have been playing for %d hours.\n\r",num2);
        send_to_char(buf2,ch);     
        buf=fread_string( fp );
        if (strlen(buf) > 2)
        {
            if (IS_SET(extra, EXTRA_MARRIED))
                sprintf(buf2,"They are married to %s.\n\r",buf);
            else
                sprintf(buf2,"They are engaged to %s.\n\r",buf);
            send_to_char(buf2,ch);
        }
/*
	 num = fread_number ( fp );
                         switch (num)
                         {
                            case CLASS_WEREWOLF:
                                 sprintf(buf3,"Werewolf");
                            break;
                            case CLASS_DEMON:
                                 sprintf(buf3,"Demon");
                            break;
                            case CLASS_MAGE:
                                 sprintf(buf3,"Mage");
                            break;
                            case CLASS_VAMPIRE:
                                 sprintf(buf3,"Vampire");
                            break;
                            default:
                                 sprintf(buf3,"Human");
                            break;
                         }
                         sprintf(buf2,"`gClass:`n %s,           \t",buf3);
                         send_to_char (buf2,ch);
                         buf = fread_string( fp );
                         if (!str_cmp(buf,"\0"))
                         sprintf(buf2,"#gClan:`nNone.\n\r");
                         else
                         sprintf(buf2,"#gClan:`n%s.\n\r",buf);
                         send_to_char(buf2,ch);
*/
 	  num=fread_number( fp );
        num2=fread_number( fp );
        sprintf(buf2,"#wPlayer Kills:#n %d, #wPlayer Deaths:#n %d.\n\r",num,num2);
        send_to_char(buf2,ch);
        num=fread_number( fp );
        num2=fread_number( fp );
        sprintf(buf2,"#wMob kills:#n %d, #wMob Deaths:#n %d.\n\r",num,num2);
        send_to_char(buf2,ch);    
        num=fread_number( fp );
        num2=fread_number( fp );
        sprintf(buf2,"#wArena Wins:#n %d, #wArena Losses:#n %d.\n\r",num,num2);
        send_to_char(buf2,ch); 
send_to_char("{D=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
        buf=fread_string( fp );
        if (strlen(buf) > 2)
        {
            sprintf(buf2,"#wEmail:#n %s\n\r",buf);
            send_to_char(buf2,ch);

send_to_char("{D=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
        }
    }
    else
    {
        bug( "Do_finger: fopen", 0 );
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Disconnect %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}


/*
void do_howl( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *vch;
    char buf [MAX_STRING_LENGTH];
    
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_POLYAFF(ch, POLY_WOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if ( argument[0] == '\0' )
    {
	send_to_char("What do you wish to howl?\n\r",ch);
	return;
    }
    if (IS_SET(ch->deaf, CHANNEL_HOWL))
    {
	send_to_char("But you're not even on the channel!\n\r",ch);
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected != CON_PLAYING ) continue;
	if ( (vch = d->character) == NULL ) continue;
	if ( IS_SET(vch->deaf, CHANNEL_HOWL) ) continue;
	if ( vch == ch )
	    act("You howl '$T'.", ch, NULL, argument, TO_CHAR);
	else if (!IS_NPC(vch) && (IS_POLYAFF(ch, POLY_WOLF) ||
	    IS_CLASS(vch, CLASS_WEREWOLF) || IS_IMMORTAL(vch)))
	{
	    sprintf(buf,"$n howls '%s'.", argument);
	    act(buf, ch, NULL, vch, TO_VICT);
	}
	else if ( vch->in_room == ch->in_room )
	    act("$n throws back $s head and howls loudly.", ch, NULL, vch, TO_VICT);
	else if ( vch->in_room->area == ch->in_room->area )
	    act("You hear a loud howl from nearby.", ch, NULL, vch, TO_VICT);
	else
	    act("You hear a loud howl in the distance.", ch, NULL, vch, TO_VICT);
    }

    return;
}
*/
void do_info( CHAR_DATA *ch, char *argument )
{
/* Changed by Kyrlin to have Altreal colours 20/01/00 */
/* Changed by Tracker to get rid of Altreal colours 02/18/03 */
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING && 
		!IS_SET(d->character->deaf, CHANNEL_INFO) )
	{
	    send_to_char( "{wI{Wn{Df{wo {c>{C>{D>{w ",d->character );
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}
void do_fatality( CHAR_DATA *ch, char *argument )
{
/* Changed by Kyrlin to have Altreal colours 20/01/00 */
/* changed by Tracker */
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }

    if (!IS_NPC(ch) && IS_IMMORTAL(ch) && IS_SET(ch->act,PLR_WIZINVIS))
	return;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING && 
		!IS_SET(d->character->deaf, CHANNEL_INFO) )
	{
	    send_to_char( "{wFa{Wta{Dli{wty {c>{C>{D>{w ",d->character );
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_watching( CHAR_DATA *ch, char *argument )
{
/* Changed by Kyrlin to have Altreal colours 20/01/00 */
/* changed by Tracker */
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING && 
	    IS_SET(d->character->act, PLR_WATCHER) &&
	    !IS_SET(d->character->deaf, CHANNEL_INFO) )
	{
	    send_to_char( "{wMo{Wni{Dto{wr {c>{C>{D>{w ",d->character );
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void logchan( char *argument )
{
/* Changed by Kyrlin to have Altreal colours 20/01/00 */
/* changed by Tracker */
    CHAR_DATA *ch;
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( ( ch = d->character ) == NULL ) continue;
	if ( (d->connected == CON_PLAYING) && IS_JUDGE(ch) && 
	    !IS_SET(ch->deaf,CHANNEL_LOG) )
	{
	    send_to_char( "#r[#n", ch );
	    send_to_char( argument, ch );
	    send_to_char( "#r]#n\n\r",  ch );
	}
    }

    return;
}

void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Echo %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Echo what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Recho %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Recho what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) && atoi(arg) != 30008 )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL && obj->in_room != NULL)
	return obj->in_room;

    if ( obj != NULL && obj->carried_by != NULL && obj->carried_by->in_room != NULL)
	return obj->carried_by->in_room;

    if ( obj != NULL && obj->in_obj != NULL && obj->in_obj->in_room != NULL)
	return obj->in_obj->in_room;

    if ( obj != NULL && obj->in_obj != NULL && obj->in_obj->carried_by && obj->in_obj->carried_by->in_room != NULL)
	return obj->in_obj->carried_by->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    CHAR_DATA *mount;

    sprintf(buf,"%s: Transfer %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		if (!IS_NPC(d->character))
		{
		    	if (IS_SET(d->character->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        			&& !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
				continue;
		}
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}
/* Changed Toaster so Immortals can violate private rooms*/
	if ( room_is_private( location ) && (ch->level < 7) )
	{
	    send_to_char( "That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
/* Changed Toaster 13/03/00 so people not see imms tranfer folk out of
rooms.
    act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
*/
    char_from_room( victim );
    char_to_room( victim, location );
    act( "$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( "$n has transferred you.", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
    if ( (mount = victim->mount) == NULL) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    if ( ch != mount )
	act( "$n has transferred you.", ch, NULL, mount, TO_VICT );
    do_look( mount, "auto" );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    CHAR_DATA *wch;
    
    sprintf(buf,"%s: At %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    break;
	}
    }

    return;
}


/*
void do_goto( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    CHAR_DATA *mount;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );
    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
	act( "$n $T.", ch, NULL,
	    (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
	    ? ch->pcdata->bamfout : "leaves in a swirling mist",  TO_ROOM );
    }

    char_from_room( ch );
    char_to_room( ch, location );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
	act( "$n $T.", ch, NULL,
	    (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
	    ? ch->pcdata->bamfin : "appears in a swirling mist", TO_ROOM );
    }

    do_look( ch, "auto" );
    if ( (mount = ch->mount) == NULL) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    do_look( mount, "auto" );
    return;
}
*/
void do_goto(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    CHAR_DATA *mount;

    one_argument(argument, arg);
    if (arg[0] == '\0')
    {
	send_to_char("Goto where?\n\r", ch);
	return;
    }

    if ((location = find_location(ch, arg)) == NULL)
    {
	send_to_char("No such location.\n\r", ch);
	return;
    }

    if (room_is_private(location) && get_trust(ch) < LEVEL_IMPLEMENTOR)
    {
	send_to_char("That room is private right now.\n\r", ch);
	return;
    }

    if (ch->fighting != NULL)
	stop_fighting(ch, TRUE);
    if (!IS_SET(ch->act, PLR_WIZINVIS))
    {
	act("$n $T.", ch, NULL,
	    (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
	    ? ch->pcdata->bamfout : "leaves in a swirling mist",
TO_ROOM);
    }

    char_from_room(ch);
    char_to_room(ch, location);

    if (!IS_SET(ch->act, PLR_WIZINVIS))
    {
	act("$n $T.", ch, NULL,
	    (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
	    ? ch->pcdata->bamfin : "appears in a swirling mist", TO_ROOM);
    }

    do_look(ch, "auto");
    if ((mount = ch->mount) == NULL) return;
    char_from_room(mount);
    char_to_room(mount, ch->in_room);
    do_look(mount, "auto");
    return;
}



void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( ch->in_room != location && room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: '%s.'\n\rArea: '%s'.\n\r",
	location->name,
	location->area->name );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %d.  Sector: %d.  Light: %d.\n\r",
	location->vnum,
	location->sector_type,
	location->light );
    send_to_char( buf, ch );

    sprintf( buf,
	"Room flags: %d.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	send_to_char( " ", ch );
	one_argument( rch->name, buf );
	send_to_char( buf, ch );
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",

		door,
		pexit->to_room != NULL ? pexit->to_room->vnum : 0,
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char nm1[40];
    char nm2[40];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Ostat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
	sprintf(nm1,obj->questmaker); else sprintf(nm1,"None");
    if (obj->questowner != NULL && strlen(obj->questowner) > 1)
	sprintf(nm2,obj->questowner); else sprintf(nm2,"None");

    sprintf( buf, "Name: %s.\n\r",
	obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d.  Type: %s.\n\r",
	obj->pIndexData->vnum, item_type_name(obj->item_type) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );

    sprintf( buf, "Object creator: %s.  Object owner: %s.  Quest points: %d.\n\r", nm1, nm2, obj->points );
    send_to_char( buf, ch );
    if (obj->quest != 0)
    {
    	send_to_char( "Quest selections:", ch );
    	if (IS_SET(obj->quest,QUEST_STR)) send_to_char(" Str",ch);
    	if (IS_SET(obj->quest,QUEST_DEX)) send_to_char(" Dex",ch);
    	if (IS_SET(obj->quest,QUEST_INT)) send_to_char(" Int",ch);
    	if (IS_SET(obj->quest,QUEST_WIS)) send_to_char(" Wis",ch);
    	if (IS_SET(obj->quest,QUEST_CON)) send_to_char(" Con",ch);
    	if (IS_SET(obj->quest,QUEST_HIT)) send_to_char(" Hp",ch);
    	if (IS_SET(obj->quest,QUEST_MANA)) send_to_char(" Mana",ch);
    	if (IS_SET(obj->quest,QUEST_MOVE)) send_to_char(" Move",ch);
    	if (IS_SET(obj->quest,QUEST_HITROLL)) send_to_char(" Hit",ch);
    	if (IS_SET(obj->quest,QUEST_DAMROLL)) send_to_char(" Dam",ch);
    	if (IS_SET(obj->quest,QUEST_AC)) send_to_char(" Ac",ch);
    	send_to_char( ".\n\r", ch );
    }
    sprintf( buf, "Wear bits: %d.  Extra bits: %s.\n\r",
	obj->wear_flags, extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Weight: %d/%d.\n\r",
	obj->weight, get_obj_weight( obj ) );
    send_to_char( buf, ch );

    sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.\n\r",
	obj->cost, obj->timer, obj->level );
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" : obj->carried_by->name,
	obj->wear_loc );
    send_to_char( buf, ch );
    
    sprintf( buf, "Values: %d %d %d %d.\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    send_to_char( buf, ch );

    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	send_to_char( "'.\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    return;
}



void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Mstat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: %s.\n\r",
	victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d.  Sex: %s.  Room: %d.\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	victim->sex == SEX_MALE    ? "male"   :
	victim->sex == SEX_FEMALE  ? "female" : "neutral",
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
	get_curr_str(victim),
	get_curr_int(victim),
	get_curr_wis(victim),
	get_curr_dex(victim),
	get_curr_con(victim) );
    send_to_char( buf, ch );

    sprintf( buf, "Hp: %ld/%ld.  Mana: %ld/%ld.  Move: %ld/%ld.  Primal: %d.\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
	victim->practice );
    send_to_char( buf, ch );
	
    sprintf( buf,
	"Lv: %d.  Align: %d.  AC: %d.  Gold: %d.  Exp: %d.\n\r",
	victim->level,       victim->alignment,
	char_ac(victim),     victim->gold,         victim->exp );
    send_to_char( buf, ch );

    sprintf( buf, "Hitroll: %d.  Damroll: %d.  Position: %d.  Wimpy: %d.\n\r",
	char_hitroll(victim), char_damroll(victim),
	victim->position,    victim->wimpy );
    send_to_char( buf, ch );
	
    sprintf( buf, "Fighting: %s.\n\r",
	victim->fighting ? victim->fighting->name : "(none)" );
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "Thirst: %d.  Full: %d.  Drunk: %d.  Saving throw: %d.\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK],
	    victim->saving_throw );
	send_to_char( buf, ch );

	if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_CLASS(victim, CLASS_WEREWOLF))
	{
	    if (strlen(victim->clan) > 1) sprintf( buf, "Clan: %s. ", victim->clan);
	    else sprintf( buf, "Clan: None. ");
	    send_to_char( buf, ch );
	    sprintf( buf, "Rage: %d. ", victim->pcdata->stats[UNI_RAGE]);
	    send_to_char( buf, ch );
	    if (IS_CLASS(victim, CLASS_VAMPIRE))
	    {
		sprintf( buf, "Beast: %d. ", victim->beast);
		send_to_char( buf, ch );
		sprintf( buf, "Blood: %d.", victim->pcdata->condition[COND_THIRST]);
		send_to_char( buf, ch );
	    }
	    send_to_char( "\n\r", ch );
	}

	if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
	{
	    if (IS_SET(victim->special, SPC_CHAMPION))
	    {
		if (strlen(victim->lord) > 1) sprintf( buf, "Lord: %s. ", victim->lord);
		else sprintf( buf, "Lord: None. ");
		send_to_char( buf, ch );
	    }
	    sprintf( buf, "Demonic armor: %d pieces. ", victim->pcdata->stats[DEMON_POWER]);
	    send_to_char( buf, ch );
	    sprintf( buf, "Power: %d (%d).",
		victim->pcdata->stats[DEMON_CURRENT], victim->pcdata->stats[DEMON_TOTAL]);
	    send_to_char( buf, ch );
	    send_to_char( "\n\r", ch );
	}
    }

    sprintf( buf, "Carry number: %d.  Carry weight: %d.\n\r",
	victim->carry_number, victim->carry_weight );
    send_to_char( buf, ch );

    sprintf( buf, "Age: %d.  Played: %d.  Timer: %d.  Act: %d.\n\r",
	get_age( victim ), (int) victim->played, victim->timer, victim->act );
    send_to_char( buf, ch );

    sprintf( buf, "Master: %s.  Leader: %s.  Affected by: %s.\n\r",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	affect_bit_name( victim->affected_by ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none).\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
	send_to_char( "Mobile has spec fun.\n\r", ch );


    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf,
	    "Spell: '%s' modifies %s by %d for %d hours with bits %s.\n\r",
	    skill_table[(int) paf->type].name,
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration,
	    affect_bit_name( paf->bitvector )
	    );
	send_to_char( buf, ch );
    }

    return;
}



void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mfind whom?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );



 if
(is_in(argument,"|all*"))
        {
            send_to_char( "That was fun!\n\r", ch );
            return;
        }


    if ( arg[0] == '\0' )
    {
	send_to_char( "Ofind what?\n\r", ch );
	return;
    }



    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mwhere whom?\n\r", ch );
	return;
    }

    /* Had problems when lots of gremlins existed - KaVir
    if ( !str_cmp( arg, "gremlin" ) )
    {
	send_to_char( "Sorry, but in the interest of mud stability you cannot do that.\n\r", ch );
	return;
    }
    */

    found = FALSE;
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( IS_NPC(victim)
	&&   victim->in_room != NULL
	&&   is_name( arg, victim->name ) )
	{
	    found = TRUE;
	    sprintf( buf, "[%5d] %-28s [%5d] %s\n\r",
		victim->pIndexData->vnum,
		victim->short_descr,
		victim->in_room->vnum,
		victim->in_room->name );
	    send_to_char( buf, ch );
	}
    }

    if ( !found )
	act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );

    return;
}

void do_owhere(CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = 200;

    one_argument( argument, arg );
    if (argument[0] == '\0')
    {
        send_to_char("Find what?\n\r",ch);
        return;
    }

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !can_see_obj( ch, obj ) || !is_name( argument, obj->name )
        ||   ch->level < obj->level)
            continue;

        found = TRUE;
        number++;

        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );

        if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)
        &&   in_obj->carried_by->in_room != NULL)
            sprintf( buf, "%3d) %s carried by %s [Room %d]\n\r",
                number, obj->short_descr,PERS(in_obj->carried_by, ch),
                in_obj->carried_by->in_room->vnum );
        else if (in_obj->in_room != NULL)
            sprintf( buf, "%3d) %s is in %s [Room %d]\n\r",
                number, obj->short_descr,in_obj->in_room->name,
                in_obj->in_room->vnum);
        else   
            sprintf( buf, "%3d) %s is somewhere\n\r",number, obj->short_descr);
	    send_to_char(buf,ch);

        if (number >= max_found)
            break;
    }

    if ( !found )
        send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}
/*
void do_copyover(CHAR_DATA *ch, char * argument)
{

stc("Copyover starting all charchters being saved.\n\r", ch );
do_forceauto(ch, "call all");
do_forceauto(ch, "save");
do_realcopyover(ch, "");
return;
}
*/
void do_reboot( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    sprintf( buf, "\033[1;24r\033[2JReboot by %s.", ch->name );
    do_echo( ch, buf );
    do_forceauto(ch,"call all");
    call_all(ch);
     do_asave(ch, "changed");
    do_forceauto(ch,"save");
    do_autosave(ch,"");
    do_restore(ch,"all");
    merc_down = TRUE;
    return;
}


void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    sprintf( buf, "Shutdown by %s.", ch->name );
    append_file( ch, SHUTDOWN_FILE, buf );
    strcat( buf, "\n\r" );
    do_echo( ch, buf );
    do_forceauto(ch,"save");
    do_autosave(ch,"");
    merc_down = TRUE;
    return;
}



void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Snoop %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    if (IS_SET(victim->act, PLR_WATCHER))
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oswitch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *mount;

    sprintf(buf,"%s: Oswitch %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into what?\n\r", ch );
	return;
    }

    if ( IS_EXTRA(ch, EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD) )
    {
	send_to_char( "You are already oswitched.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_SWITCH) )
    {
	send_to_char( "Not while switched.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( obj->chobj != NULL )
    {
	send_to_char( "Object in use.\n\r", ch );
	return;
    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oreturn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    sprintf(buf,"%s: Oreturn",ch->name);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;
    
    if ( !IS_EXTRA(ch, EXTRA_OSWITCH) && !IS_HEAD(ch,LOST_HEAD) )
    {
	send_to_char( "You are not oswitched.\n\r", ch );
	return;
    }
    if ( ( obj = ch->pcdata->chobj ) != NULL ) obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0], LOST_HEAD);
    free_string(ch->morph);
    ch->morph = str_dup("");
    char_from_room(ch);
    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Switch %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim))
    {
	send_to_char( "Only on NPC's.\n\r", ch );
	return;
    }

    SET_BIT(ch->extra, EXTRA_SWITCH);
    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;
    send_to_char( "Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Return %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( "You return to your original body.\n\r", ch );
    REMOVE_BIT(ch->desc->original->extra, EXTRA_SWITCH);
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc; 
    ch->desc                  = NULL;
    return;
}



void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Mload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
    
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: mload <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
    act( "You have created $N!", ch, NULL, victim, TO_CHAR );
    return;
}

/*

void do_pload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char name[40];
    DESCRIPTOR_DATA *d;
    bool fOld;

    sprintf(buf,"%s: Pload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
    
    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' || is_number(arg) )
    {
	send_to_char( "Syntax: pload <name>.\n\r", ch );
	return;
    }

    if (!str_cmp(ch->name,arg))
    {
	send_to_char( "But you are already that character!\n\r", ch );
	return;
    }

        if (!str_cmp("Tracker",arg)) {
        send_to_char(" Not no, but, HELL NO.\n\r", ch );
        return;}

    if (!(ch->pload == NULL || strlen(ch->pload) < 3))
    {
	send_to_char( "But you are already in another form!\n\r", ch );
	return;
    }

    d = ch->desc;

    sprintf(buf,"You transform into %s.\n\r",capitalize(arg));
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms into %s.",capitalize(arg));
    act(buf,ch,NULL,NULL,TO_ROOM);

    sprintf(name,ch->name);
    save_char_obj(ch);

    if (ch != NULL && ch->desc != NULL)
	extract_char(ch,TRUE,FALSE);
    else if (ch != NULL)
	extract_char(ch,TRUE,FALSE);
    if (ch->desc) ch->desc->character = NULL;

    ch->next = char_list;
    char_list = ch;

    fOld = load_char_obj(d, capitalize(arg));
    if (ch->in_room != NULL)
    	char_to_room(ch,ch->in_room);
    else
    	char_to_room(ch,get_room_index(3001));
    free_string(ch->pload);
    ch->pload = str_dup(name);
    return;
}
*/

void do_pload( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *in_room;
    bool fOld;

    if ( IS_NPC(ch) || ch->desc == NULL || ch->in_room == NULL ) return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax: pload <name>.\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( !char_exists(TRUE,argument) )
    {
	send_to_char( "That player doesn't exist.\n\r", ch );
	return;
    }

    sprintf(buf,"%s: Pload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
    
    argument[0] = UPPER(argument[0]);

	if (!str_cmp(argument,"Tracker")) {
	send_to_char("You failed.\n\r", ch );
	return;}

    sprintf(buf,"You transform into %s.\n\r",argument);
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms into %s.",argument);
    act(buf,ch,NULL,NULL,TO_ROOM);

    d = ch->desc;
    do_autosave(ch,"");
    in_room = ch->in_room;
    extract_char(ch, TRUE, FALSE);
    d->character = NULL;
    fOld = load_char_obj( d, argument );
    ch   = d->character;
    ch->next = char_list;
    char_list = ch;
    char_to_room(ch,in_room);
    return;
}

/* Editied by Kyrlin to fix pload bug 24/01/00
void do_pload( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *in_room;
    bool fOld;

    if ( IS_NPC(ch) || ch->desc == NULL || ch->in_room == NULL ) return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax: pload <name>.\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

 if ( !char_exists(TRUE,argument) )
    {
        send_to_char( "That player doesn't exist.\n\r", ch );
        return;
    }

    if (!str_cmp(argument, "Tracker"))
    {
        send_to_char("You failed.\n\r",ch);
        return;
    }
    sprintf(buf,"%s: Pload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
    
    argument[0] = UPPER(argument[0]);

    sprintf(buf,"You transform into %s.\n\r",argument);
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms into %s.",argument);
    act(buf,ch,NULL,NULL,TO_ROOM);

    d = ch->desc;
    do_autosave(ch,"");
    in_room = ch->in_room;
    extract_char(ch, TRUE, FALSE);
    d->character = NULL;
    fOld = load_char_obj( d, argument );
    ch   = d->character;
    ch->next = char_list;
    char_list = ch;
    char_to_room(ch,in_room);
    ch->pload = ch->name;
    return;
}
*/

void do_preturn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool fOld;

    if (IS_NPC(ch)) {send_to_char("Huh?\n\r",ch);return;}

    if (ch->pload == NULL) {send_to_char("Huh?\n\r",ch);return;}
    sprintf(arg,ch->pload);
    if (strlen(arg) < 3 || strlen(arg) > 8) 
	{send_to_char("Huh?\n\r",ch);return;}

    if (!str_cmp(ch->name,arg)) {send_to_char("Huh?\n\r",ch);return;}

    d = ch->desc;

    sprintf(buf,"You transform back into %s.\n\r",capitalize(ch->pload));
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms back into %s.",capitalize(ch->pload));
    act(buf,ch,NULL,NULL,TO_ROOM);
    do_autosave(ch,"");
    if (ch != NULL && ch->desc != NULL)
	extract_char(ch,TRUE,FALSE);
    else if (ch != NULL)
	extract_char(ch,TRUE,FALSE);
    if (ch->desc) ch->desc->character = NULL;
/*
    ch->next = char_list;
    char_list = ch;
*/
    fOld = load_char_obj(d, capitalize(arg));
    if (ch->in_room != NULL)
    	char_to_room(ch,ch->in_room);
    else
    	char_to_room(ch,get_room_index(3001));
    free_string(ch->pload);
    ch->pload = str_dup("");
    return;
}


void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    sprintf(buf,"%s: Oload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
        return;
    }
 
    if ( arg2[0] == '\0' )
    {
	level = get_trust( ch );
    }
    else
    {
	/*
	 * New feature from Alander.
	 */
        if ( !is_number( arg2 ) )
        {
	    send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	    return;
        }
        level = atoi( arg2 );
	if ( level < 0 || level > get_trust( ch ) )
        {
	    send_to_char( "Limited to your trust level.\n\r", ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
    {
	obj_to_char( obj, ch );
	act( "$p appears in $n's hands!", ch, obj, NULL, TO_ROOM );
    }
    else
    {
	obj_to_room( obj, ch->in_room );
	act( "$n has created $p!", ch, obj, NULL, TO_ROOM );
    }
    act( "You create $p.", ch, obj, NULL, TO_CHAR );
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    return;
}



void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if (IS_NPC(victim) && victim->desc == NULL && (mount = victim->mount) == NULL)
		extract_char( victim, TRUE, FALSE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    extract_obj( obj );
	}

	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on PC's.\n\r", ch );
	return;
    }
    if ( victim->desc != NULL )
    {
	send_to_char( "Not on switched players.\n\r", ch );
	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE, FALSE );
    return;
}



void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int level;

    sprintf(buf,"%s: Trust %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: trust <char> <trust>.\n\r", ch );
	send_to_char( "Trust being one of: None, Builder, Questmaker, Enforcer, Judge, or Highjudge.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

         if ( !str_cmp( arg2, "none"       ) ) level = 0;
    else if ( !str_cmp( arg2, "builder"    ) ) level = 7;
    else if ( !str_cmp( arg2, "questmaker" ) ) level = 8;
    else if ( !str_cmp( arg2, "enforcer"   ) ) level = 9;
    else if ( !str_cmp( arg2, "judge"      ) ) level = 10;
    else if ( !str_cmp( arg2, "highjudge"  ) ) level = 11;
    else
    {
	send_to_char( "Please enter: None, Builder, Questmaker, Enforcer, Judge, or Highjudge.\n\r", ch );
	return;
    }

    if ( level >= get_trust( ch ) )
    {
	send_to_char( "Limited to below your trust.\n\r", ch );
	return;
    }
    send_to_char("Ok.\n\r",ch);
    victim->trust = level;
    return;
}


/*
void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Restore %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Restore whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
    victim->loc_hp[6] = 0;
    update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    send_to_char( "Ok.\n\r", ch );
    return;
}
*/

void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;

    sprintf(buf,"%s: Restore %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Restore whom?\n\r", ch );
        return;
}

if ( get_trust(ch) >=  MAX_LEVEL - 2 && !str_cmp(arg,"all"))
    {
    /* cure all */

        for (d = descriptor_list; d != NULL; d = d->next)
        {
            victim = d->character;

            if (victim == NULL || IS_NPC(victim))
                continue;


            {
                affect_strip(victim,gsn_drowfire);
                affect_strip(victim,gsn_poison);
                affect_strip(victim,gsn_blindness);
                affect_strip(victim,gsn_sleep);
                affect_strip(victim,gsn_curse);

                victim->hit     = victim->max_hit;
                victim->mana    = victim->max_mana;
                victim->move    = victim->max_move;
                update_pos( victim);
                if (victim->in_room != NULL)
                    act("$n has restored you.",ch,NULL,victim,TO_VICT);
            }
        }
        send_to_char("All active players restored.\n\r",ch);
return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }
 victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    if (IS_NINJA(victim) )
    {
    victim->pcdata->powerss[NINJAKI] = 300;
    }
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
    victim->loc_hp[6] = 0;
    update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Freeze %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Log %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Noemote %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_NO_EMOTE) )
    {
	REMOVE_BIT(victim->act, PLR_NO_EMOTE);
	send_to_char( "You can emote again.\n\r", victim );
	send_to_char( "NO_EMOTE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_NO_EMOTE);
	send_to_char( "You can't emote!\n\r", victim );
	send_to_char( "NO_EMOTE set.\n\r", ch );
    }

    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Notell %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_NO_TELL) )
    {
	REMOVE_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NO_TELL removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NO_TELL set.\n\r", ch );
    }

    return;
}



void do_silence( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Silence %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Silence whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_SILENCE) )
    {
	REMOVE_BIT(victim->act, PLR_SILENCE);
	send_to_char( "You can use channels again.\n\r", victim );
	send_to_char( "SILENCE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_SILENCE);
	send_to_char( "You can't use channels!\n\r", victim );
	send_to_char( "SILENCE set.\n\r", ch );
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Peace %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



BAN_DATA *		ban_free;
BAN_DATA *		ban_list;

void do_ban( CHAR_DATA *ch, char *argument )
 
{
    char        buf[MAX_STRING_LENGTH];
    char        arg[MAX_INPUT_LENGTH];
    char        arg2[MAX_INPUT_LENGTH];
    bool        newplayers = FALSE;
    BAN_DATA    *pban;

    if ( IS_NPC( ch ) ) return;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Site                  Who           Trust  Newbie Locked\n\r", ch );
       for( pban = ban_list ; pban != NULL ; pban = pban->next )
        {
            sprintf( buf, "%-20s  %-12s  %-5d  %s\n\r",
                pban->name,
                pban->by_who,
                pban->level,
                !pban->newbie_banned ? "No" : "Yes" );
            send_to_char( buf, ch );
        }
        return;
    }
    
    if ( arg2[0] == '\0' )
    {
        send_to_char( "Syntax: ban [site] [y/n]\n\r",           ch );
        send_to_char( "\n\r",                                   ch );
        send_to_char( "Y/N meaning newbie banned only.\n\r",    ch );
        return;
    }
                
    if ( !str_cmp( arg2, "y" ) || !str_cmp( arg2, "yes" ) )
        newplayers = TRUE;  
    else if ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "no" ) )
        newplayers = FALSE;
    else 
    {
        do_ban( ch, "moose" );
        return;
    }
     
    for ( pban = ban_list; pban != NULL ; pban = pban->next )
    {
       if ( !str_cmp( arg, pban->name ) )
        {
            send_to_char( "That site is already banned!\n\r", ch );
            return;
        }
    }
    
    if ( ban_free == NULL )
    {    
        pban            = alloc_perm( sizeof( *pban ) );
    }
    else
    {
        pban            = ban_free;
        ban_free        = ban_free->next;
    }
       
    pban->name          = str_dup( arg );
    pban->by_who        = str_dup( ch->name );
    pban->level         = get_trust( ch );
    if ( pban->level > LEVEL_IMPLEMENTOR )
        pban->level = LEVEL_IMPLEMENTOR;
    pban->newbie_banned = newplayers;
    
    pban->next          = ban_list;
    ban_list            = pban;
     
    save_bans( );
     
    send_to_char( "Site banned.\n\r", ch );
    return;
}

void do_allow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    BAN_DATA *prev;
    BAN_DATA *curr;
         
    sprintf(buf,"%s: Allow %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    one_argument( argument, arg );
        
    if ( arg[0] == '\0' )
    {   
        send_to_char( "Remove which site from the ban list?\n\r", ch );
        return;
   }
 
    prev = NULL;
    for ( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
    {
        if ( !str_cmp( arg, curr->name ) )
        {
            if ( get_trust( ch ) < curr->level )  
            {
                send_to_char( "You are not permitted to allow that site.\n\r", ch );
                return;
            }
    
            if ( prev == NULL )
                ban_list   = ban_list->next;
           else
                prev->next = curr->next;
 
            free_string( curr->name );
            free_string( curr->by_who );
            curr->next  = ban_free;
            ban_free    = curr;
         
            save_bans( );
             
            send_to_char( "Ok.\n\r", ch );
            return;
        }
    }
    send_to_char( "Site is not banned.\n\r", ch );
    return;
}


void do_wizlock( CHAR_DATA *ch, char *argument )
{
if(IS_SET(config_info->world,WIZLOCKED))
{
  REMOVE_BIT(config_info->world,WIZLOCKED);
  do_info(ch,"The game is no longer Wizlocked");
}
else
{
  SET_BIT(config_info->world,WIZLOCKED);
  do_info(ch,"The game is now Wizlocked");
}
save_config();
}


void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Slookup what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
		sn, skill_table[sn].slot, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].slot, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}



void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    sprintf(buf,"%s: Sset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: sset <victim> <skill> <value>\n\r",	ch );
	send_to_char( "or:     sset <victim> all     <value>\n\r",	ch );
	send_to_char( "Skill being any skill or spell.\n\r",		ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL )
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    send_to_char("Ok.\n\r",ch);
    return;
}

void do_resetarea(CHAR_DATA *ch, char *argument)
{
    send_to_char("Area Reset.\n\r",ch);
    area_update();
}

void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    long value;

    sprintf(buf,"%s: Mset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: mset <victim> <field>  <value>\n\r",	ch );
	send_to_char( "or:     mset <victim> <string> <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  str int wis dex con sex level exp\n\r",	ch );
	send_to_char( "  gold hp mana move primal align\n\r",		ch );
	send_to_char( "  thirst drunk full hit dam ac\n\r",ch );
	send_to_char( "  hours security \n\r",ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name short long description title spec\n\r",	ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "security" ) )
    {
	int security;
	if ( IS_NPC(ch) || IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}
	if(ch->level < 12)
	{
	send_to_char("I'm afraid you have to be lvl 12 to change this.",ch);
	return;
	}
	security = IS_JUDGE(ch) ? 1 : GET_PC( ch, security, 0 );
	if ( ch == victim && !IS_JUDGE(ch) )
	{
	    send_to_char( "Not on yourself.\n\r", ch );
	    return;
	}
	if ( value < security || value > 9 )
	{
	    if ( security != 9 )
	    {
		sprintf(buf,"Enter a security level between %d and 9.\n\r",
		    security );
		send_to_char(buf, ch );
	    }
	    else send_to_char("You can only set their security level to 9.\n\r", ch );
	    return;
	}

	if ( victim->pcdata->security == value )
	{
	    sprintf(buf,"They already have security level %ld!\n\r", value);
	    send_to_char(buf, ch );
	    return;
	}
	victim->pcdata->security = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if (!str_cmp(arg2, "hours"))
    {
        if ( IS_NPC( victim ) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }
        
        if ( !is_number( arg3 ) )
        {
            send_to_char( "Value must be numeric.\n\r", ch );
            return;
        }
        
        value = atoi( arg3 );
        
        if ( value < 0 || value > 999 )
        {
            send_to_char( "Value must be betwen 0 and 999.\n\r", ch );
            return;
        }
            
        value *= 3600;
        victim->played = value;
        return;
    }

    if ( !str_cmp( arg2, "str" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}
	
	if ( value < 3 || value > 18 )
	{
	    send_to_char( "Strength range is 3 to 18.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_str = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "int" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 3 || value > 18 )
	{
	    send_to_char( "Intelligence range is 3 to 18.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_int = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "wis" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 3 || value > 18 )
	{
	    send_to_char( "Wisdom range is 3 to 18.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_wis = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 3 || value > 18 )
	{
	    send_to_char( "Dexterity range is 3 to 18.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_dex = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "con" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 3 || value > 18 )
	{
	    send_to_char( "Constitution range is 3 to 18.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->perm_con = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	victim->sex = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
/*here is where i need to add the disable level for sage*/
    if ( !str_cmp( arg2, "level" ) )
    {
		if ( IS_NPC(victim) && ( value < 1 || value > 2500 ) )
		{
		    send_to_char( "Level range is 1 to 2500 for mobs.\n\r", ch );
		    return;
		}
		else if (!IS_JUDGE(ch))
		{
		    send_to_char( "Sorry, no can do...\n\r", ch );
		    return;
		}
		     if ( !str_cmp( arg3, "mortal"     ) ) value = 2;
		else if ( !str_cmp( arg3, "avatar"     ) ) value = 3;
		else if ( !str_cmp( arg3, "acolyte"    ) ) value = 4;
		else if ( !str_cmp( arg3, "adept"      ) ) value = 5;
		else if ( !str_cmp( arg3, "ninja"      ) ) value = 6;
 		else if ( !str_cmp( arg3, "monk"       ) ) value = 6;
		else if ( !str_cmp( arg3, "master"     ) ) value = 6;
		else if ( !str_cmp( arg3, "apprentice" ) ) value = 7;
		else if ( !str_cmp( arg3, "questor"    ) ) value = 8;
		else if ( !str_cmp( arg3, "councillor" ) ) value = 9;
		else if ( !str_cmp( arg3, "regent"     ) ) value = 10;
		else if ( !str_cmp( arg3, "pontifex"   ) ) value = 11;
		else if (!IS_NPC(victim))
		{
            stc( "Level should be one of the following:\n\r", ch );
            stc( "Mortal, Avatar, Acolyte, Adept, Master, Monk, Ninja,\n\r", ch );
            stc( "Apprentice, Questor, Councillor, Regent or Pontifex.\n\r", ch );
            return;
		}
	
		if (!str_cmp("Tracker",arg1)) {
        	send_to_char(" You failed.\n\r", ch );
        	return;
        }		
		if (value >= ch->level && !IS_NPC(victim)) 
		    send_to_char( "Sorry, no can do...\n\r", ch );
		else {victim->level = value;  victim->trust = value;
	send_to_char("Ok.\n\r",ch);}
		return;
    }

    if ( !str_cmp( arg2, "hitroll" ) || !str_cmp( arg2, "hit" ) )
    {
	if ( !IS_NPC(victim) && ( value < 0 || value > 50 ) )
	{
	    send_to_char( "Hitroll range is 0 to 50.\n\r", ch );
	    return;
	}
	else if ( IS_NPC(victim) && ( value < 0 || value > 250 ) )
	{
	    send_to_char( "Hitroll range is 0 to 250.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->hitroll = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "damroll" ) || !str_cmp( arg2, "dam" ) )
    {
	if ( !IS_NPC(victim) && ( value < 0 || value > 50 ) )
	{
	    send_to_char( "Damroll range is 0 to 50.\n\r", ch );
	    return;
	}
	else if ( IS_NPC(victim) && ( value < 0 || value > 250 ) )
	{
	    send_to_char( "Damroll range is 0 to 250.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->damroll = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "armor" ) || !str_cmp( arg2, "ac" ) )
    {
	if ( !IS_NPC(victim) && ( value < -200 || value > 200 ) )
	{
	    send_to_char( "Armor class range is -200 to 200.\n\r", ch );
	    return;
	}
	if (!IS_NPC(victim) && !IS_JUDGE(ch) && ch != victim)
	{
	    send_to_char( "Sorry, no can do...\n\r", ch );
	    return;
	}
	victim->armor = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "exp" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 )
	{
	    send_to_char( "Exp must be at least 0.\n\r", ch );
	    return;
	}

	if ( value > 100000000 )
	{
	    send_to_char( "No more than 100000000 possible.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch) || (ch == victim)) {
		victim->exp = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "gold" ) )
    {
	if (value > 100000 && !IS_JUDGE(ch))
		send_to_char("Don't be so damn greedy!\n\r",ch);
	else
		{victim->gold = value; send_to_char("Ok.\n\r",ch);}
	return;
    }

    if ( !str_cmp( arg2, "hp" ) )
    {
	if ( value < 1 || value > 60000 )
	{
	    send_to_char( "Hp range is 1 to 60,000 hit points.\n\r", ch );
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim) || (IS_NPC(victim)) ) {
		victim->max_hit = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "mana" ) )
    {
	if ( value < 0 || value > 60000 )
	{
	    send_to_char( "Mana range is 0 to 60,000 mana points.\n\r", ch);
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim) || (IS_NPC(victim)) ) {
		victim->max_mana = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "move" ) )
    {
	if ( value < 0 || value > 60000 )
	{
	    send_to_char( "Move range is 0 to 60,000 move points.\n\r", ch);
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim) || (IS_NPC(victim)) ) {
		victim->max_move = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "primal" ) )
    {
	if ( value < 0 || value > 10000 )
	{
	    send_to_char( "Primal range is 0 to 10000.\n\r", ch );
	    return;
	}
	if (IS_JUDGE(ch) || (ch == victim)) {
		victim->practice = value;
		send_to_char("Ok.\n\r",ch);}
	else
	    send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
/* Changed by Toaster 17/02/00 so that you can set more blood on 
Vampires */
    if ( !str_cmp( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 2000 )
	{
	    send_to_char( "Thirst range is 0 to 2000.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Drunk range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Full range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }


    if ( !str_cmp( arg2, "name" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	free_string( victim->name );
	victim->name = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	free_string( victim->short_descr );
	victim->short_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	free_string( victim->long_descr );
	victim->long_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "title" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	set_title( victim, arg3 );
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	{
	    send_to_char( "No such spec fun.\n\r", ch );
	    return;
	}

	send_to_char("Ok.\n\r",ch);
	return;
    }

    /*
     * Generate usage message.
     */
    do_mset( ch, "" );
    return;
}


/*Changed by Toaster 18/02/00 to add artifact,relic and silver to oset*/
void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *morph;
    OBJ_INDEX_DATA *pObjIndex;
    int value;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    sprintf(buf,"%s: Oset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: oset <object> <field>  <value>\n\r",	ch );
	send_to_char( "or:     oset <object> <string> <value>\n\r",	ch );
	send_to_char( "or:     oset <object> <affect> <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  value0 value1 value2 value3\n\r",		ch );
	send_to_char( "  level weight cost timer morph\n\r",		ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name short long ed type extra wear owner\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Affect being one of:\n\r",			ch );
	send_to_char( "  str dex int wis con\n\r",			ch );
	send_to_char( "  hit dam ac hp mana move\n\r",			ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Spec being one of:\n\r",				ch );
	send_to_char( "  silver demonic artifact relic prize\n\r",      ch );
        send_to_char("\n\r",ch);
        send_to_char( "Immune being one of:\n\r",			ch );
        send_to_char( "Slash Stab Smash Beast Grab Charm Heat Cold\n\r",    ch );
        send_to_char( "Lightning Acid Sleep Drain Hurl Backstab Kick\n\r",  ch );
        send_to_char( "Disarm Steal.\n\r",				ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (obj->carried_by != NULL && !IS_NPC(obj->carried_by) &&
	IS_SET(obj->carried_by->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

/*
    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker)))
    {
	send_to_char("You don't have permission to change that item.\n\r", ch);
	return;
    }*/

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */

   if (!str_cmp(arg2, "immune"))
   {
         if (!str_cmp( arg3, "slash" ) )
         {
             if ( IS_SET( obj->immune, IITEM_SLASH ) )
                REMOVE_BIT( obj->immune, IITEM_SLASH );
             else
                SET_BIT( obj->immune, IITEM_SLASH );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "stab" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_STAB ) )   
                REMOVE_BIT( obj->immune, IITEM_STAB );
             else
                SET_BIT( obj->immune, IITEM_STAB );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "smash" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_SMASH ) )   
                REMOVE_BIT( obj->immune, IITEM_SMASH );
             else
                SET_BIT( obj->immune, IITEM_SMASH );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "beast" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_BEAST ) )   
                REMOVE_BIT( obj->immune, IITEM_BEAST );
             else
                SET_BIT( obj->immune, IITEM_BEAST );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "charm" ) )
         {
             if ( IS_SET( obj->immune, IITEM_CHARM ) )
                REMOVE_BIT( obj->immune, IITEM_CHARM );
             else
                SET_BIT( obj->immune, IITEM_CHARM );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "grab" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_GRAB ) )   
                REMOVE_BIT( obj->immune, IITEM_GRAB );
             else
                SET_BIT( obj->immune, IITEM_GRAB );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "slash" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_CHARM ) )   
                REMOVE_BIT( obj->immune, IITEM_CHARM );
             else
                SET_BIT( obj->immune, IITEM_CHARM );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "heat" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_HEAT ) )   
                REMOVE_BIT( obj->immune, IITEM_HEAT );
             else
                SET_BIT( obj->immune, IITEM_HEAT );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "cold" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_COLD ) )   
                REMOVE_BIT( obj->immune, IITEM_COLD );
             else
                SET_BIT( obj->immune, IITEM_COLD );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "lightning" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_LIGHTNING ) )   
                REMOVE_BIT( obj->immune, IITEM_LIGHTNING );
             else
                SET_BIT( obj->immune, IITEM_LIGHTNING );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "acid" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_ACID ) )   
                REMOVE_BIT( obj->immune, IITEM_ACID );
             else
                SET_BIT( obj->immune, IITEM_ACID );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "sleep" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_SLEEP ) )   
                REMOVE_BIT( obj->immune, IITEM_SLEEP );
             else
                SET_BIT( obj->immune, IITEM_SLEEP );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "drain" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_DRAIN ) )   
                REMOVE_BIT( obj->immune, IITEM_DRAIN );
             else
                SET_BIT( obj->immune, IITEM_DRAIN );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "hurl" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_HURL ) )   
                REMOVE_BIT( obj->immune, IITEM_HURL );
             else
                SET_BIT( obj->immune, IITEM_HURL );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "backstab" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_BACKSTAB ) )   
                REMOVE_BIT( obj->immune, IITEM_BACKSTAB );
             else
                SET_BIT( obj->immune, IITEM_BACKSTAB );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "kick" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_KICK ) )   
                REMOVE_BIT( obj->immune, IITEM_KICK );
             else
                SET_BIT( obj->immune, IITEM_KICK );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "disarm" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_DISARM ) )   
                REMOVE_BIT( obj->immune, IITEM_DISARM );
             else
                SET_BIT( obj->immune, IITEM_DISARM );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
         if (!str_cmp( arg3, "steal" ) )  
         {
             if ( IS_SET( obj->immune, IITEM_STEAL ) )   
                REMOVE_BIT( obj->immune, IITEM_STEAL );
             else
                SET_BIT( obj->immune, IITEM_STEAL );
             send_to_char( "Ok.\n\r", ch);
             return;
         }
  }

  if ( !str_cmp( arg2, "special" ) || !str_cmp( arg2, "spec" ) )
    {

        if ( !str_cmp( arg3, "silver" ) )
        {
            if ( IS_SET( obj->spectype, SITEM_SILVER ) )
                REMOVE_BIT( obj->specpower, SITEM_SILVER );
            else
                SET_BIT( obj->spectype, SITEM_SILVER );
            send_to_char( "Ok.\n\r", ch );
            return;
        }
        if ( !str_cmp( arg3, "demonic" ) )
        {
            if ( IS_SET( obj->spectype, SITEM_DEMONIC ) )
                REMOVE_BIT( obj->spectype, SITEM_DEMONIC );
            else
                SET_BIT( obj->spectype, SITEM_DEMONIC );
            send_to_char( "Ok.\n\r", ch );
            return;
        }

        if ( !str_cmp( arg3, "artifact" ) )
        {
            if(!IS_JUDGE(ch))
	{send_to_char("This is for Judges+\n\r", ch);return;}
	
            if ( IS_SET( obj->quest, QUEST_ARTIFACT ) )
                REMOVE_BIT( obj->quest, QUEST_ARTIFACT );
            else
                SET_BIT( obj->quest, QUEST_ARTIFACT );
            send_to_char( "Ok.\n\r", ch );
            return;
        }
     if ( !str_cmp( arg3, "relic" ) )
        {
            if ( IS_SET( obj->quest, QUEST_RELIC ) )
                REMOVE_BIT( obj->quest, QUEST_RELIC );
            else
                SET_BIT( obj->quest, QUEST_RELIC );
            send_to_char( "Ok.\n\r", ch );
            return;
        }
     if ( !str_cmp( arg3, "prize" ) )
        {
            if ( IS_SET( obj->quest, QUEST_PRIZE ) )
                REMOVE_BIT( obj->quest, QUEST_PRIZE );
            else
                SET_BIT( obj->quest, QUEST_PRIZE );
            send_to_char( "Ok.\n\r", ch );
            return;
        }
    }


    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
	if (obj->item_type == ITEM_WEAPON && !IS_JUDGE(ch)) {
		send_to_char("You are not authorised to create spell weapons.\n\r", ch);
		return;}
	else if (obj->item_type == ITEM_QUEST) {
		send_to_char("You cannot change a quest tokens value with oset.\n\r", ch);
		return;}
	else if (obj->item_type == ITEM_ARMOR && value > 15)
		obj->value[0] = 15;
	else
		obj->value[0] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
	if (obj->item_type == ITEM_WEAPON && value > 10)
		obj->value[1] = 10;
	else
		obj->value[1] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
	if (obj->item_type == ITEM_WEAPON && value > 20)
		obj->value[2] = 20;
	else
		obj->value[2] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	if (obj->item_type == ITEM_ARMOR && !IS_JUDGE(ch))
		send_to_char("You are not authorised to create spell armour.\n\r", ch);
	else
		{obj->value[3] = value; send_to_char("Ok.\n\r",ch);}
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "morph" ) )
    {
	int mnum;
	if ( IS_SET(obj->spectype, SITEM_MORPH) )
    	{
	    send_to_char( "This item can already morph.\n\r", ch );
	    return;
    	}
    	if ( ( pObjIndex = get_obj_index( value ) ) == NULL )
    	{
	    send_to_char( "No object has that vnum.\n\r", ch );
	    return;
    	}
    	morph = create_object( pObjIndex, obj->level );
    	if ( !CAN_WEAR(morph, ITEM_TAKE) )
	    SET_BIT(morph->wear_flags, ITEM_TAKE);
	if ( !IS_SET(obj->spectype, SITEM_MORPH) )
	    SET_BIT(obj->spectype, SITEM_MORPH);
	if ( !IS_SET(morph->spectype, SITEM_MORPH) )
	    SET_BIT(morph->spectype, SITEM_MORPH);
	obj_to_char( morph, ch );
	act( "$p morphs into $P in $n's hands!", ch, obj, morph, TO_ROOM );
	act( "$p morphs into $P in your hands!", ch, obj, morph, TO_CHAR );
	mnum = obj->wear_loc;
	obj_from_char( obj );
	obj_to_obj( obj, morph );
	if ( morph->wear_flags == obj->wear_flags && mnum != WEAR_NONE )
	    equip_char( ch, morph, mnum );
    	if (morph->questmaker != NULL) free_string(morph->questmaker);
    	morph->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "extra" ) )
    {
	if      ( !str_cmp( arg3, "glow"         )) value = ITEM_GLOW;
	else if ( !str_cmp( arg3, "hum"          )) value = ITEM_HUM;
	else if ( !str_cmp( arg3, "thrown"       )) value = ITEM_THROWN;
	else if ( !str_cmp( arg3, "vanish"       )) value = ITEM_VANISH;
	else if ( !str_cmp( arg3, "invis"        )) value = ITEM_INVIS;
	else if ( !str_cmp( arg3, "magic"        )) value = ITEM_MAGIC;
	else if ( !str_cmp( arg3, "nodrop"       )) value = ITEM_NODROP;
	else if ( !str_cmp( arg3, "bless"        )) value = ITEM_BLESS;
	else if ( !str_cmp( arg3, "anti-good"    )) value = ITEM_ANTI_GOOD;
	else if ( !str_cmp( arg3, "anti-evil"    )) value = ITEM_ANTI_EVIL;
	else if ( !str_cmp( arg3, "anti-neutral" )) value = ITEM_ANTI_NEUTRAL;
	else if ( !str_cmp( arg3, "noremove"     )) value = ITEM_NOREMOVE;
	else if ( !str_cmp( arg3, "inventory"    )) value = ITEM_INVENTORY;
	else if ( !str_cmp( arg3, "loyal"        )) value = ITEM_LOYAL;
	else {
	    send_to_char("Extra flag can be from the following: Glow, Hum, Thrown, Vanish, Invis, Magic, Nodrop, Bless, Anti-Good, Anti-Evil, Anti-Neutral, Noremove, Inventory, Loyal.\n\r",ch); return;}

	/* Removing magic flag allows multiple enchants */
	if (IS_SET(obj->extra_flags,value) && value == ITEM_MAGIC && !IS_JUDGE(ch))
	{send_to_char("Sorry, no can do...\n\r",ch); return;}

	if (IS_SET(obj->extra_flags,value))
	    REMOVE_BIT(obj->extra_flags, value);
	else
	    SET_BIT(obj->extra_flags, value);
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "wear" ) )
    {
	if      ( !str_cmp( arg3, "none" ) || !str_cmp( arg3, "clear" ))
	{
	    obj->wear_flags = 0;
	    send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);
	    return;
	}
	else if ( !str_cmp( arg3, "take"   ))
	{
	    if (IS_SET(obj->wear_flags,ITEM_TAKE))
		REMOVE_BIT(obj->wear_flags, ITEM_TAKE);
	    else
		SET_BIT(obj->wear_flags, ITEM_TAKE);
	    send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);
	    return;
	}
	else if ( !str_cmp( arg3, "finger" )) value = ITEM_WEAR_FINGER;
	else if ( !str_cmp( arg3, "neck"   )) value = ITEM_WEAR_NECK;
	else if ( !str_cmp( arg3, "body"   )) value = ITEM_WEAR_BODY;
	else if ( !str_cmp( arg3, "head"   )) value = ITEM_WEAR_HEAD;
	else if ( !str_cmp( arg3, "legs"   )) value = ITEM_WEAR_LEGS;
	else if ( !str_cmp( arg3, "feet"   )) value = ITEM_WEAR_FEET;
	else if ( !str_cmp( arg3, "hands"  )) value = ITEM_WEAR_HANDS;
	else if ( !str_cmp( arg3, "arms"   )) value = ITEM_WEAR_ARMS;
	else if ( !str_cmp( arg3, "about"  )) value = ITEM_WEAR_ABOUT;
	else if ( !str_cmp( arg3, "waist"  )) value = ITEM_WEAR_WAIST;
	else if ( !str_cmp( arg3, "wrist"  )) value = ITEM_WEAR_WRIST;
	else if ( !str_cmp( arg3, "hold"   )) value = ITEM_WIELD;
	else if ( !str_cmp( arg3, "face"   )) value = ITEM_WEAR_FACE;
	else if ( !str_cmp( arg3, "float"  )) value = ITEM_WEAR_FLOAT;
	else if ( !str_cmp( arg3, "special")) value = ITEM_WEAR_SPECIAL;
	else if ( !str_cmp( arg3, "medal"  )) value = ITEM_WEAR_MEDAL;
	else if ( !str_cmp( arg3, "bodyart")) value = ITEM_WEAR_BODYART;
	else {
	    send_to_char("Wear location can be from: None, Take, Finger, Neck, Body, Head, Legs, Hands, Arms, About, Waist, Hold, Face, Float, Special, Medal, Bodyart.\n\r",ch); return;}
	if (IS_SET(obj->wear_flags,ITEM_TAKE) ) value += 1;
	obj->wear_flags = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	if (value < 1) value = 1; else if (value > 50) value = 50;
	if (!IS_JUDGE(ch))
	    send_to_char("You are not authorised to change an items level.\n\r", ch);
	else
	    {obj->level = value; send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);}
	return;
    }
	
    if ( !str_cmp( arg2, "weight" ) )
    {
	obj->weight = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "cost" ) )
    {
	if (value > 100000 && !IS_JUDGE(ch))
		send_to_char("Don't be so damn greedy!\n\r",ch);
	else
		{obj->cost = value; send_to_char("Ok.\n\r",ch);
	    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    		obj->questmaker = str_dup(ch->name);}
	return;
    }

    if ( !str_cmp( arg2, "timer" ) )
    {
	obj->timer = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }
	
    if      ( !str_cmp( arg2, "hitroll" ) || !str_cmp( arg2, "hit" ) )
	{oset_affect(ch,obj,value,APPLY_HITROLL,FALSE);return;}
    else if ( !str_cmp( arg2, "damroll" ) || !str_cmp( arg2, "dam" ) )
	{oset_affect(ch,obj,value,APPLY_DAMROLL,FALSE);return;}
    else if ( !str_cmp( arg2, "armor" ) || !str_cmp( arg2, "ac" ) )
	{oset_affect(ch,obj,value,APPLY_AC,FALSE);return;}
    else if ( !str_cmp( arg2, "hitpoints" ) || !str_cmp( arg2, "hp" ) )
	{oset_affect(ch,obj,value,APPLY_HIT,FALSE);return;}
    else if ( !str_cmp( arg2, "mana" ) )
	{oset_affect(ch,obj,value,APPLY_MANA,FALSE);return;}
    else if ( !str_cmp( arg2, "move" ) || !str_cmp( arg2, "movement" ) )
	{oset_affect(ch,obj,value,APPLY_MOVE,FALSE);return;}
    else if ( !str_cmp( arg2, "str" ) || !str_cmp( arg2, "strength" ) )
	{oset_affect(ch,obj,value,APPLY_STR,FALSE);return;}
    else if ( !str_cmp( arg2, "dex" ) || !str_cmp( arg2, "dexterity" ) )
	{oset_affect(ch,obj,value,APPLY_DEX,FALSE);return;}
    else if ( !str_cmp( arg2, "int" ) || !str_cmp( arg2, "intelligence" ) )
	{oset_affect(ch,obj,value,APPLY_INT,FALSE);return;}
    else if ( !str_cmp( arg2, "wis" ) || !str_cmp( arg2, "wisdom" ) )
	{oset_affect(ch,obj,value,APPLY_WIS,FALSE);return;}
    else if ( !str_cmp( arg2, "con" ) || !str_cmp( arg2, "constitution" ) )
	{oset_affect(ch,obj,value,APPLY_CON,FALSE);return;}

    if ( !str_cmp( arg2, "type" ) )
    {
	if (!IS_JUDGE(ch))
	{
	    send_to_char("You are not authorised to change an item type.\n\r", ch);
	    return;
	}
	if      (!str_cmp( arg3, "light")) obj->item_type = 1;
	else if (!str_cmp( arg3, "scroll")) obj->item_type = 2;
	else if (!str_cmp( arg3, "wand")) obj->item_type = 3;
	else if (!str_cmp( arg3, "staff")) obj->item_type = 4;
	else if (!str_cmp( arg3, "weapon")) obj->item_type = 5;
	else if (!str_cmp( arg3, "treasure")) obj->item_type = 8;
	else if (!str_cmp( arg3, "armor")) obj->item_type = 9;
	else if (!str_cmp( arg3, "armour")) obj->item_type = 9;
	else if (!str_cmp( arg3, "potion")) obj->item_type = 10;
	else if (!str_cmp( arg3, "furniture")) obj->item_type = 12;
	else if (!str_cmp( arg3, "trash")) obj->item_type = 13;
	else if (!str_cmp( arg3, "container")) obj->item_type = 15;
	else if (!str_cmp( arg3, "drink")) obj->item_type = 17;
	else if (!str_cmp( arg3, "key")) obj->item_type = 18;
	else if (!str_cmp( arg3, "food")) obj->item_type = 19;
	else if (!str_cmp( arg3, "money")) obj->item_type = 20;
	else if (!str_cmp( arg3, "boat")) obj->item_type = 22;
	else if (!str_cmp( arg3, "corpse")) obj->item_type = 23;
	else if (!str_cmp( arg3, "fountain")) obj->item_type = 25;
	else if (!str_cmp( arg3, "pill")) obj->item_type = 26;
	else if (!str_cmp( arg3, "portal")) obj->item_type = 27;
	else if (!str_cmp( arg3, "stake")) obj->item_type = 30;
	else {send_to_char("Type can be one of: Light, Scroll, Wand, Staff, Weapon, Treasure, Armor, Potion, Furniture, Trash, Container, Drink, Key, Food, Money, Boat, Corpse, Fountain, Pill, Portal, Stake.\n\r", ch);
	     return;}
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "owner" ) )
    {
	if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch);return;}
	if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker)))
	{send_to_char("Someone else has already changed this item.\n\r",ch);return;}
	if ( ( victim = get_char_world( ch, arg3 ) ) == NULL )
	{send_to_char("You cannot find any player by that name.\n\r",ch);return;}
	if (IS_NPC(victim)) {send_to_char("Not on NPC's.\n\r",ch);return;}
	if (obj->questmaker != NULL) free_string(obj->questmaker);
	obj->questmaker = str_dup(ch->name);
	if (obj->questowner != NULL) free_string(obj->questowner);
	obj->questowner = str_dup(victim->name);
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	free_string( obj->name );
	obj->name = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	free_string( obj->short_descr );
	obj->short_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	free_string( obj->description );
	obj->description = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "ed" ) )
    {
	EXTRA_DESCR_DATA *ed;

	argument = one_argument( argument, arg3 );
	if ( argument == NULL )
	{
	    send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		ch );
	    return;
	}

	if ( extra_descr_free == NULL )
	{
	    ed			= alloc_perm( sizeof(*ed) );
	}
	else
	{
	    ed			= extra_descr_free;
	    extra_descr_free	= ed;
	}

	ed->keyword		= str_dup( arg3     );
	ed->description		= str_dup( argument );
	ed->next		= obj->extra_descr;
	obj->extra_descr	= ed;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    sprintf(buf,"%s: Rset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: rset <location> <field> value\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_cmp( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}

/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Force %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
    
        if (get_trust(ch) < MAX_LEVEL - 3)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }
    
        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;
     
            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }   
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
	{
	    send_to_char( "You failed.\n\r", ch );
	    return;
	}

	act( "$n forces you to '$t'.", ch, argument, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_forceauto( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) && vch != ch )
	{
	    act( "Autocommand: $t.", ch, argument, vch, TO_VICT );
	    interpret( vch, argument );
	}
    }
    return;
}



/*
 * New routines by Dionysos.
 */
/*
void do_invis( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
	SET_BIT(ch->act, PLR_WIZINVIS);
    }

    return;
}
*/
void do_invis(CHAR_DATA *ch, char *argument)  
{
    char arg[MAX_INPUT_LENGTH];
    one_argument(argument, arg);
 
    if(arg[0] != '\0')
    {
        if (atoi(arg) < 1 || atoi(arg)>12)
        {
          send_to_char("Level range is from 1 to 12.\n\r",ch);
          return;
        }
      
        if (atoi(arg) > ch->level)
        {
          send_to_char("You may only invis at your level or lower.\n\r", ch);
        return;
        }
    }
    
    if (IS_NPC(ch))
        return;
     
    if (IS_SET(ch->act, PLR_WIZINVIS) && arg[0] == '\0')
    {
        REMOVE_BIT(ch->act, PLR_WIZINVIS);
        ch->pcdata->wizinvis = 0;
        act("$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM);
        send_to_char("You slowly fade back into existence.\n\r", ch);
    }
    else 
    {
   	act("$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM);
        send_to_char("You slowly vanish into thin air.\n\r", ch);
        SET_BIT(ch->act, PLR_WIZINVIS);
    
        if(arg[0] != '\0')
           ch->pcdata->wizinvis = atoi(arg);
     
        else
           ch->pcdata->wizinvis = ch->level;
        
    }
        
    return;
}

void do_giftreset( CHAR_DATA *ch, char *argument )
{
    if(!IS_WEREWOLF(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
      ch->gifts[STARGAZERS] = 0;
      ch->gifts[HOMID] = 0;
      ch->gifts[METIS] = 0;
      ch->gifts[LUPUS] = 0;
      ch->gifts[RAGABASH] = 0;
      ch->gifts[THEURGE] = 0;
      ch->gifts[PHILODOX] = 0;
      ch->gifts[GALLIARD] = 0;
      ch->gifts[AHROUN] = 0;
      ch->gifts[BLACKFURIES] = 0;
      ch->gifts[BONEGNAWERS] = 0;
      ch->gifts[CHILDREN] = 0;
      ch->gifts[CHILDRENOFGAIA] = 0;
      ch->gifts[FIANNA] = 0;
      ch->gifts[GETOFFENRIS] = 0;
      ch->gifts[GLASSWALKERS] = 0;
      ch->gifts[REDTALONS] = 0;
      ch->gifts[SHADOWLORDS] = 0;
      ch->gifts[SILENTSTRIDERS] = 0;
      ch->gifts[SILVERFANGS] = 0;
      ch->gifts[UKTENA] = 0;
      ch->gifts[WENDIGO] = 0;

}

void do_incog( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_INCOG) )
    {
	REMOVE_BIT(ch->act, PLR_INCOG);
	act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
	SET_BIT(ch->act, PLR_INCOG);
    }

    return;
}

void do_cloak( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH];
        
    one_argument( argument, arg );
     
    if (IS_NPC(ch)) return;
 
    if ( arg[0] == '\0')
    { 
        send_to_char( "Cloak ON or OFF?\n\r", ch );
        return;
    }

   if (IS_SET(ch->act, PLR_WIZINVIS) && !str_cmp(arg,"off"))
    {
        REMOVE_BIT(ch->act, PLR_WIZINVIS);
        send_to_char("You are no longer {RCloaked{x.\n\r", ch);
   	sprintf(buf,"{w%s {Dhas entered the game.{x", ch->name);
        do_info(ch,buf);
        ch->pcdata->wizinvis = 0; 

    }
    else if (!IS_SET(ch->act, PLR_GODLESS) && !str_cmp(arg,"off"))
    {
        send_to_char("You are not {RCloaked{x.\n\r", ch);
        return;
    }
    else if (!IS_SET(ch->act, PLR_WIZINVIS) && !str_cmp(arg,"on"))
    {
 	send_to_char("You become {RCloaked{x.\n\r", ch);   
        sprintf(buf,"{w%s {Dhas left the game.{x", ch->name);
        do_info(ch,buf);
        SET_BIT(ch->act, PLR_WIZINVIS); 
        ch->pcdata->wizinvis = ch->level; 
   }
    else if (IS_SET(ch->act, PLR_WIZINVIS) && !str_cmp(arg,"on"))
    {  
        send_to_char("You are already {RCloaked{x.\n\r", ch);
        return;
    }
    else 
        send_to_char( "Cloak ON or OFF?\n\r", ch );
    return;
}



void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode on.\n\r", ch );
    }

    return;
}

void do_safe( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) && ch->race < 20 )
	send_to_char( "You cannot be attacked by other players here.\n\r", ch );
    else
	send_to_char( "You are not safe from player attacks in this room.\n\r", ch );

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
	return;

    if (ch->in_room->sector_type == SECT_INSIDE)
    {
	send_to_char( "You are inside, which means you are safe from sunlight.\n\r", ch );
	return;
    }

    if (weather_info.sunlight == SUN_DARK)
    {
	send_to_char( "It is not yet light out, so you are safe from the sunlight...for now.\n\r", ch );
	return;
    }

    if ( room_is_dark(ch->in_room) )
    {
	send_to_char( "This room is dark, and will protect you from the sunlight.\n\r", ch );
	return;
    }

    send_to_char( "This room will provide you no protection from the sunlight.\n\r", ch );
    return;
}

void do_qstat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char("Which item?\n\r",ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    sprintf(buf,"Object %s.\n\r",obj->short_descr);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when worn: %s\n\r",obj->chpoweron);
    send_to_char(buf,ch);
    sprintf(buf,"Other when worn: %s\n\r",obj->victpoweron);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when removed: %s\n\r",obj->chpoweroff);
    send_to_char(buf,ch);
    sprintf(buf,"Other when removed: %s\n\r",obj->victpoweroff);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when used: %s\n\r",obj->chpoweruse);
    send_to_char(buf,ch);
    sprintf(buf,"Other when used: %s\n\r",obj->victpoweruse);
    send_to_char(buf,ch);
    send_to_char("Type:",ch);
    if ( IS_SET(obj->spectype, SITEM_ACTIVATE))
	send_to_char(" Activate",ch);
    if ( IS_SET(obj->spectype, SITEM_TWIST))
	send_to_char(" Twist",ch);
    if ( IS_SET(obj->spectype, SITEM_PRESS))
	send_to_char(" Press",ch);
    if ( IS_SET(obj->spectype, SITEM_PULL))
	send_to_char(" Pull",ch);
    if ( IS_SET(obj->spectype, SITEM_TARGET))
	send_to_char(" Target",ch);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
	send_to_char(" Spell",ch);
    if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
	send_to_char(" Transporter",ch);
    if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
	send_to_char(" Teleporter",ch);
    if ( IS_SET(obj->spectype, SITEM_OBJECT))
	send_to_char(" Object",ch);
    if ( IS_SET(obj->spectype, SITEM_MOBILE))
	send_to_char(" Mobile",ch);
    if ( IS_SET(obj->spectype, SITEM_ACTION))
	send_to_char(" Action",ch);
    if ( IS_SET(obj->spectype, SITEM_DELAY1))
	send_to_char(" Delay1",ch);
    if ( IS_SET(obj->spectype, SITEM_DELAY2))
	send_to_char(" Delay2",ch);
    if (   !IS_SET(obj->spectype, SITEM_ACTIVATE    )
    	&& !IS_SET(obj->spectype, SITEM_TWIST       )
    	&& !IS_SET(obj->spectype, SITEM_PRESS       )
    	&& !IS_SET(obj->spectype, SITEM_PULL        )
    	&& !IS_SET(obj->spectype, SITEM_TARGET      )
    	&& !IS_SET(obj->spectype, SITEM_SPELL       )
    	&& !IS_SET(obj->spectype, SITEM_TELEPORTER  )
    	&& !IS_SET(obj->spectype, SITEM_DELAY1      )
    	&& !IS_SET(obj->spectype, SITEM_DELAY2      )
    	&& !IS_SET(obj->spectype, SITEM_OBJECT      )
    	&& !IS_SET(obj->spectype, SITEM_MOBILE      )
    	&& !IS_SET(obj->spectype, SITEM_ACTION      )
    	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER ) )
	send_to_char(" No flags set",ch);
    send_to_char(".\n\r",ch);
    sprintf(buf,"Power: %d.\n\r",obj->specpower);
    send_to_char(buf,ch);
    return;
}

void do_qset( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '0' )
    {
    	send_to_char("You can change the following fields...\n\r", ch);
	send_to_char("chwear   = Message to owner when item is worn.\n\r",ch);
	send_to_char("chrem    = Message to owner when item is removed.\n\r",ch);
	send_to_char("chuse    = Message to owner when item is used.\n\r",ch);
	send_to_char("victwear = Message to others in room when item is worn.\n\r",ch);
	send_to_char("victrem  = Message to others in room when item is removed.\n\r",ch);
	send_to_char("victuse  = Message to others in room when item is used.\n\r",ch);
	send_to_char("type       activate     = Item can be activated.\n\r",ch);
	send_to_char("           twist        = Item can be twisted.\n\r",ch);
	send_to_char("           press        = Item can be pressed.\n\r",ch);
	send_to_char("           pull         = Item can be pulled.\n\r",ch);
	send_to_char("           target       = Item can target people (for spell, etc).\n\r",ch);
	send_to_char("           spell        = Item can cast spells.\n\r",ch);
	send_to_char("           transporter  = Owner can transport freely between two locations.\n\r",ch);
	send_to_char("           teleporter   = Owner can transport to target location at will.\n\r",ch);
	send_to_char("           object       = Owner can create the specified object.\n\r",ch);
	send_to_char("           mobile       = Owner can create the specified mobile.\n\r",ch);
	send_to_char("           action       = Target must perform an action.\n\r",ch);
	send_to_char("           delay1       = Sets a delay of half a round on spell items.\n\r",ch);
	send_to_char("           delay2       = Sets a delay of one round on spell items.\n\r",ch);
	send_to_char("power      <value>      = Spell number/transporter room number.\n\r",ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    if (!str_cmp(arg2,"chwear") )
    {
	if (obj->chpoweron != NULL) strcpy(buf,obj->chpoweron);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweron );
	    obj->chpoweron = str_dup( "(null)" );
	}
	else if (obj->chpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweron );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweron = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweron );
	    obj->chpoweron = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"chrem") )
    {
	if (obj->chpoweroff != NULL) strcpy(buf,obj->chpoweroff);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweroff );
	    obj->chpoweroff = str_dup( "(null)" );
	}
	else if (obj->chpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweroff );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweroff = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweroff );
	    obj->chpoweroff = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"chuse") )
    {
	if (obj->chpoweruse != NULL) strcpy(buf,obj->chpoweruse);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweruse );
	    obj->chpoweruse = str_dup( "(null)" );
	}
	else if (obj->chpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweruse );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweruse = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweruse );
	    obj->chpoweruse = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victwear") )
    {
	if (obj->victpoweron != NULL) strcpy(buf,obj->victpoweron);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweron );
	    obj->victpoweron = str_dup( "(null)" );
	}
	else if (obj->victpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweron );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweron = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweron );
	    obj->victpoweron = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victrem") )
    {
	if (obj->victpoweroff != NULL) strcpy(buf,obj->victpoweroff);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweroff );
	    obj->victpoweroff = str_dup( "(null)" );
	}
	else if (obj->victpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweroff );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweroff = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweroff );
	    obj->victpoweroff = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victuse") )
    {
	if (obj->victpoweruse != NULL) strcpy(buf,obj->victpoweruse);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweruse );
	    obj->victpoweruse = str_dup( "(null)" );
	}
	else if (obj->victpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweruse );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweruse = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweruse );
	    obj->victpoweruse = str_dup( arg3 );
	}
    }
    else if ( !str_cmp( arg2, "type" ) )
    {
	if ( !str_cmp( arg3, "activate" ) && IS_SET(obj->spectype, SITEM_ACTIVATE) )
	    REMOVE_BIT(obj->spectype, SITEM_ACTIVATE);
	else if ( !str_cmp( arg3, "activate" ) && !IS_SET(obj->spectype, SITEM_ACTIVATE) )
	    SET_BIT(obj->spectype, SITEM_ACTIVATE);
	else if ( !str_cmp( arg3, "twist" ) && IS_SET(obj->spectype, SITEM_TWIST) )
	    REMOVE_BIT(obj->spectype, SITEM_TWIST);
	else if ( !str_cmp( arg3, "twist" ) && !IS_SET(obj->spectype, SITEM_TWIST) )
	    SET_BIT(obj->spectype, SITEM_TWIST);
	else if ( !str_cmp( arg3, "press" ) && IS_SET(obj->spectype, SITEM_PRESS) )
	    REMOVE_BIT(obj->spectype, SITEM_PRESS);
	else if ( !str_cmp( arg3, "press" ) && !IS_SET(obj->spectype, SITEM_PRESS) )
	    SET_BIT(obj->spectype, SITEM_PRESS);
	else if ( !str_cmp( arg3, "pull" ) && IS_SET(obj->spectype, SITEM_PULL) )
	    REMOVE_BIT(obj->spectype, SITEM_PULL);
	else if ( !str_cmp( arg3, "pull" ) && !IS_SET(obj->spectype, SITEM_PULL) )
	    SET_BIT(obj->spectype, SITEM_PULL);
	else if ( !str_cmp( arg3, "target" ) && IS_SET(obj->spectype, SITEM_TARGET) )
	    REMOVE_BIT(obj->spectype, SITEM_TARGET);
	else if ( !str_cmp( arg3, "target" ) && !IS_SET(obj->spectype, SITEM_TARGET) )
	    SET_BIT(obj->spectype, SITEM_TARGET);
	else if ( !str_cmp( arg3, "spell" ) && IS_SET(obj->spectype, SITEM_SPELL) )
	    REMOVE_BIT(obj->spectype, SITEM_SPELL);
	else if ( !str_cmp( arg3, "spell" ) && !IS_SET(obj->spectype, SITEM_SPELL) )
	    SET_BIT(obj->spectype, SITEM_SPELL);
	else if ( !str_cmp( arg3, "transporter" ) && IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	    REMOVE_BIT(obj->spectype, SITEM_TRANSPORTER);
	else if ( !str_cmp( arg3, "transporter" ) && !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	    SET_BIT(obj->spectype, SITEM_TRANSPORTER);
	else if ( !str_cmp( arg3, "teleporter" ) && IS_SET(obj->spectype, SITEM_TELEPORTER) )
	    REMOVE_BIT(obj->spectype, SITEM_TELEPORTER);
	else if ( !str_cmp( arg3, "teleporter" ) && !IS_SET(obj->spectype, SITEM_TELEPORTER) )
	    SET_BIT(obj->spectype, SITEM_TELEPORTER);
	else if ( !str_cmp( arg3, "object" ) && IS_SET(obj->spectype, SITEM_OBJECT) )
	    REMOVE_BIT(obj->spectype, SITEM_OBJECT);
	else if ( !str_cmp( arg3, "object" ) && !IS_SET(obj->spectype, SITEM_OBJECT) )
	    SET_BIT(obj->spectype, SITEM_OBJECT);
	else if ( !str_cmp( arg3, "mobile" ) && IS_SET(obj->spectype, SITEM_MOBILE) )
	    REMOVE_BIT(obj->spectype, SITEM_MOBILE);
	else if ( !str_cmp( arg3, "mobile" ) && !IS_SET(obj->spectype, SITEM_MOBILE) )
	    SET_BIT(obj->spectype, SITEM_MOBILE);
	else if ( !str_cmp( arg3, "action" ) && IS_SET(obj->spectype, SITEM_ACTION) )
	    REMOVE_BIT(obj->spectype, SITEM_ACTION);
	else if ( !str_cmp( arg3, "action" ) && !IS_SET(obj->spectype, SITEM_ACTION) )
	    SET_BIT(obj->spectype, SITEM_ACTION);
	else if ( !str_cmp( arg3, "delay1" ) && IS_SET(obj->spectype, SITEM_DELAY1) )
	    REMOVE_BIT(obj->spectype, SITEM_DELAY1);
	else if ( !str_cmp( arg3, "delay1" ) && !IS_SET(obj->spectype, SITEM_DELAY1) )
	    SET_BIT(obj->spectype, SITEM_DELAY1);
	else if ( !str_cmp( arg3, "delay2" ) && IS_SET(obj->spectype, SITEM_DELAY2) )
	    REMOVE_BIT(obj->spectype, SITEM_DELAY2);
	else if ( !str_cmp( arg3, "delay2" ) && !IS_SET(obj->spectype, SITEM_DELAY2) )
	    SET_BIT(obj->spectype, SITEM_DELAY2);
	else
	{
	    send_to_char("No such flag to set.\n\r",ch);
	    return;
	}
	sprintf(buf,"%s flag toggled.\n\r",capitalize(arg3));
	send_to_char(buf,ch);
	return;
    }
    else if ( !str_cmp( arg2, "power" ) )
	obj->specpower = value;
    else
    {
	send_to_char("No such flag.\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    return;
}


void do_oclone( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;

    argument = one_argument( argument, arg1 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Make a clone of what object?\n\r", ch );
        return;
    }
 
    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker) || strlen(obj->questmaker) < 2))
    {
	send_to_char("You can only clone your own creations.\n\r", ch);
	return;
    }

    pObjIndex = get_obj_index( obj->pIndexData->vnum);
    obj2 = create_object( pObjIndex, obj->level );
    /* Copy any changed parts of the object. */
    free_string(obj2->name);
    obj2->name=str_dup(obj->name);
    free_string(obj2->short_descr);
    obj2->short_descr=str_dup(obj->short_descr);
    free_string(obj2->description);
    obj2->description=str_dup(obj->description);

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	free_string(obj2->questmaker);
	obj2->questmaker=str_dup(obj->questmaker);
    }

    if (obj->chpoweron != NULL)
    {
    	free_string(obj2->chpoweron);
    	obj2->chpoweron=str_dup(obj->chpoweron);
    }
    if (obj->chpoweroff != NULL)
    {
    	free_string(obj2->chpoweroff);
    	obj2->chpoweroff=str_dup(obj->chpoweroff);
    }
    if (obj->chpoweruse != NULL)
    {
    	free_string(obj2->chpoweruse);
    	obj2->chpoweruse=str_dup(obj->chpoweruse);
    }
    if (obj->victpoweron != NULL)
    {
    	free_string(obj2->victpoweron);
    	obj2->victpoweron=str_dup(obj->victpoweron);
    }
    if (obj->victpoweroff != NULL)
    {
    	free_string(obj2->victpoweroff);
    	obj2->victpoweroff=str_dup(obj->victpoweroff);
    }
    if (obj->victpoweruse != NULL)
    {
    	free_string(obj2->victpoweruse);
    	obj2->victpoweruse=str_dup(obj->victpoweruse);
    }
    obj2->item_type 	= obj->item_type;
    obj2->extra_flags 	= obj->extra_flags;
    obj2->wear_flags 	= obj->wear_flags;
    obj2->weight 	= obj->weight;
    obj2->spectype 	= obj->spectype;
    obj2->specpower 	= obj->specpower;
    obj2->condition 	= obj->condition;
    obj2->toughness 	= obj->toughness;
    obj2->resistance 	= obj->resistance;
    obj2->quest 	= obj->quest;
    obj2->points 	= obj->points;
    obj2->cost 		= obj->cost;
    obj2->value[0] 	= obj->value[0];
    obj2->value[1] 	= obj->value[1];
    obj2->value[2] 	= obj->value[2];
    obj2->value[3] 	= obj->value[3];
    /*****************************************/
    obj_to_char(obj2,ch);

    if (obj->affected != NULL)
    {
    	for ( paf = obj->affected; paf != NULL; paf = paf->next )
    	{
	    if (affect_free == NULL)
	        paf2 = alloc_perm( sizeof(*paf) );
	    else
	    {
		paf2 = affect_free;
		affect_free = affect_free->next;
	    }
	    paf2->type  	= 0;
	    paf2->duration	= paf->duration;
	    paf2->location	= paf->location;
	    paf2->modifier	= paf->modifier;
	    paf2->bitvector	= 0;
	    paf2->next  	= obj2->affected;
	    obj2->affected	= paf2;
    	}
    }

    act( "You create a clone of $p.", ch, obj, NULL, TO_CHAR );
    return;
}

void do_evileye( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int value;
 
    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );
 
    if (IS_NPC(ch) || !IS_VAMPAFF(ch,VAM_DOMINATE)) {
	send_to_char("Huh?\n\r",ch);return;}
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Format is: evileye <option> <value>\n\r", ch );
        send_to_char( "Option ACTION is a text string action performed by you or the viewer.\n\r", ch );
        send_to_char( "Option MESSAGE is a text string shown to the person looking at you.\n\r", ch );
        send_to_char( "Option TOGGLE has values: spell, self, other.\n\r\n\r", ch );
	if ( ch->poweraction != NULL) {
	    sprintf(buf,"Current action: %s.\n\r",ch->poweraction);
	    send_to_char(buf,ch);}
	if ( ch->powertype != NULL) {
	    sprintf(buf,"Current message: %s.\n\r",ch->powertype);
	    send_to_char(buf,ch);}
	send_to_char("Current flags:",ch);
	if (IS_SET(ch->spectype,EYE_SPELL)) send_to_char(" Spell",ch);
	if (IS_SET(ch->spectype,EYE_SELFACTION)) send_to_char(" Self",ch);
	if (IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" Other",ch);
	if (!IS_SET(ch->spectype,EYE_SPELL) &&
	    !IS_SET(ch->spectype,EYE_SELFACTION) &&
	    !IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" None",ch);
	send_to_char(".\n\r",ch);
        return;
    }
    value = is_number( arg2 ) ? atoi( arg2 ) : -1;
    if ( !str_cmp( arg1, "action" ) )
    {
	free_string( ch->poweraction );
	ch->poweraction = str_dup( arg2 );
	return;
    }
    else if ( !str_cmp( arg1, "message" ) )
    {
	free_string( ch->powertype );
	ch->powertype = str_dup( arg2 );
	return;
    }
    else if ( !str_cmp( arg1, "toggle" ) )
    {
	if ( !str_cmp( arg2, "spell" ) && IS_SET(ch->spectype,EYE_SPELL))
	    REMOVE_BIT(ch->spectype,EYE_SPELL);
	else if ( !str_cmp( arg2, "spell" ) && !IS_SET(ch->spectype,EYE_SPELL))
	    SET_BIT(ch->spectype,EYE_SPELL);
	else if ( !str_cmp( arg2, "self" ) && IS_SET(ch->spectype,EYE_SELFACTION))
	    REMOVE_BIT(ch->spectype,EYE_SELFACTION);
	else if ( !str_cmp( arg2, "self" ) && !IS_SET(ch->spectype,EYE_SELFACTION))
	    SET_BIT(ch->spectype,EYE_SELFACTION);
	else if ( !str_cmp( arg2, "other" ) && IS_SET(ch->spectype,EYE_ACTION))
	    REMOVE_BIT(ch->spectype,EYE_ACTION);
	else if ( !str_cmp( arg2, "other" ) && !IS_SET(ch->spectype,EYE_ACTION))
	    SET_BIT(ch->spectype,EYE_ACTION);
	else {
	    send_to_char("TOGGLE flag should be one of: spell, self, other.\n\r",ch);
	    return;}
	sprintf(buf,"%s flag toggled.\n\r",capitalize(arg2));
	send_to_char(buf,ch);
	return;
    }
    else
    {
        send_to_char( "Format is: evileye <option> <value>\n\r", ch );
        send_to_char( "Option ACTION is a text string action performed by you or the viewer.\n\r", ch );
        send_to_char( "Option MESSAGE is a text string shown to the person looking at you.\n\r", ch );
        send_to_char( "Option TOGGLE has values: spell, self, other.\n\r\n\r", ch );
	if ( ch->poweraction != NULL) {
	    sprintf(buf,"Current action: %s.\n\r",ch->poweraction);
	    send_to_char(buf,ch);}
	if ( ch->powertype != NULL) {
	    sprintf(buf,"Current message: %s.\n\r",ch->powertype);
	    send_to_char(buf,ch);}
	send_to_char("Current flags:",ch);
	if (IS_SET(ch->spectype,EYE_SPELL)) send_to_char(" Spell",ch);
	if (IS_SET(ch->spectype,EYE_SELFACTION)) send_to_char(" Self",ch);
	if (IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" Other",ch);
	if (!IS_SET(ch->spectype,EYE_SPELL) &&
	    !IS_SET(ch->spectype,EYE_SELFACTION) &&
	    !IS_SET(ch->spectype,EYE_ACTION)) send_to_char(" None",ch);
	send_to_char(".\n\r",ch);
    }
    return;
}

void do_clearvam( CHAR_DATA *ch, char *argument )
{
    if ( ch->trust < 8 )
    	send_to_char( "Huh?\n\r", ch );
    else
    	send_to_char( "If you want to CLEARVAMP, spell it out.\n\r", ch );
    return;
}


void do_clearvamp( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
	int x;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' || is_number(arg) )
    {
	send_to_char( "Clear who's clan?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim,CLASS_VAMPIRE)) {
    do_mortalvamp(victim,"");
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;

    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_ANARCH);
    victim->pcdata->stats[UNI_RAGE] = 0;
    victim->pcdata->rank = AGE_CHILDE;

    send_to_char( "Ok.\n\r", ch );
    return;

}

    if (IS_CLASS(victim,CLASS_WEREWOLF)) {
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
	victim->pcdata->powers[WPOWER_SILVER] = 0;
    victim->pcdata->powers[WPOWER_WOLF] = 0;
    victim->pcdata->powers[WPOWER_HAWK] = 0;
    victim->pcdata->powers[WPOWER_OWL] = 0;
    victim->pcdata->powers[WPOWER_BEAR] = 0;
    victim->pcdata->powers[WPOWER_SPIDER] = 0;
    victim->pcdata->powers[WPOWER_BOAR] = 0;
    victim->pcdata->powers[WPOWER_MANTIS] = 0;
    victim->pcdata->powers[WPOWER_LYNX] = 0;

    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_ANARCH);
    REMOVE_BIT(victim->class, CLASS_WEREWOLF);
	if (IS_SET(victim->immune, IMM_SHIELDED))
		REMOVE_BIT(victim->immune, IMM_SHIELDED);

    send_to_char( "Ok.\n\r", ch );
    return;}

	if (IS_CLASS(victim, CLASS_HIGHLANDER)) {
		REMOVE_BIT(victim->class, CLASS_HIGHLANDER);
		if (IS_SET(victim->act, PLR_HOLYLIGHT))
		REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	
	for (x = 0; x < 13; x += 1)
		if (victim->wpn[x] > 200) victim->wpn[x] = 200;

	send_to_char("Ok.\n\r", ch);
	return;}

	if (IS_CLASS(victim, CLASS_MAGE)) {
		REMOVE_BIT(victim->class, CLASS_MAGE);
		if (IS_SET(victim->act, PLR_HOLYLIGHT))
		REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
		free_string(victim->lord);
		victim->lord = str_dup("");
		victim->level = 3;
		victim->trust = 3;
		victim->pcdata->powersm[MPOWER_RUNE0] = 0;
		victim->pcdata->powersm[MPOWER_RUNE1] = 0;
		victim->pcdata->powersm[MPOWER_RUNE2] = 0;
		victim->pcdata->powersm[MPOWER_RUNE3] = 0;
		victim->pcdata->powersm[MPOWER_RUNE4] = 0;
		
	for (x = 0; x < 5; x += 1)
		if (victim->spl[x] > 200) victim->spl[x] = 200;

	send_to_char("Ok.\n\r", ch );
	return;}
}

void do_priceless( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !IS_SET(obj->quest, ITEM_EQUEST) ) continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "There are no pricless objects in the game.\n\r", ch
);

    return;
}

void do_call(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim = NULL;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
		send_to_char("What object do you wish to call?\n\r", ch);
		return;
	}
    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}
    if (!IS_HEAD(ch, LOST_HEAD)) {
    	act("#yYour eyes flicker with yellow energy.#n",ch,NULL,NULL,TO_CHAR);
    	act("#y$n's eyes flicker with yellow energy.#n",ch,NULL,NULL,TO_ROOM);
    }
    if (!str_cmp(arg,"all")) {
		call_all(ch);
		WAIT_STATE(ch, 12);
		return;
	}
    if ((obj = get_obj_world(ch, arg)) == NULL) {
		send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
		return;
	}

    if (obj->questowner == NULL || strlen(obj->questowner) < 2 ||
	  str_cmp(obj->questowner,ch->name) || obj->item_type == ITEM_PAGE
	  || obj->item_type == ITEM_CORPSE_PC) {
		send_to_char("Nothing happens.\n\r", ch);
		return;
	}

    if (obj->carried_by != NULL && obj->carried_by != ch) {
		victim = obj->carried_by;
		if (!IS_NPC(victim) && victim->desc != NULL && victim->desc->connected != CON_PLAYING) return;
			act("$p suddenly vanishes from your hands!",victim,obj,NULL,TO_CHAR);
			act("$p suddenly vanishes from $n's hands!",victim,obj,NULL,TO_ROOM);
		obj_from_char(obj);
	} else if (obj->in_room != NULL) {
    	chroom = ch->in_room;
    	objroom = obj->in_room;
    	char_from_room(ch);
    	char_to_room(ch,objroom);
    	act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
		if (chroom == objroom)
			act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
    	char_from_room(ch);
    	char_to_room(ch,chroom);
		obj_from_room(obj);
    } else if (obj->in_obj != NULL) obj_from_obj(obj);
    else {
		if (!IS_HEAD(ch, LOST_HEAD))
			send_to_char("Nothing happens.\n\r", ch);
		return;
	}

    obj_to_char(obj,ch);
    if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
		REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
    act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
    do_autosave(ch,"");
    if (victim != NULL)
		do_autosave(victim,"");
    return;
}

void call_all(CHAR_DATA *ch)
{
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *victim = NULL;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;
    bool found = FALSE;

    for (obj = object_list; obj != NULL; obj = obj->next) {
		if (obj->questowner == NULL || strlen(obj->questowner) < 2
		  || str_cmp(ch->name, obj->questowner) || obj->item_type == ITEM_PAGE
		  || obj->item_type == ITEM_CORPSE_PC)
			continue;

		found = TRUE;

		for (in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj)
			;

		if (in_obj->carried_by != NULL) {
			if (in_obj->carried_by == ch) continue;
		}
/*
if (obj->carried_by->desc->connected != CON_READ_MOTD)
{
stc("You have your charchter loged in at the motd please log him off then try and call all.\n\r",ch );
return;
}
*/
		if (obj->carried_by != NULL) {
			if (obj->carried_by == ch || obj->carried_by->desc == NULL ||
			  obj->carried_by->desc->connected != CON_PLAYING) {
				if (!IS_NPC(obj->carried_by)) return;
			}
			act("$p suddenly vanishes from your hands!",obj->carried_by,obj,NULL,TO_CHAR);
			act("$p suddenly vanishes from $n's hands!",obj->carried_by,obj,NULL,TO_ROOM);
			SET_BIT(obj->carried_by->extra, EXTRA_CALL_ALL);
			obj_from_char(obj);
		} else if (obj->in_room != NULL) {
			chroom = ch->in_room;
			objroom = obj->in_room;
			char_from_room(ch);
			char_to_room(ch,objroom);
			act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
			if (chroom == objroom)
				act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
			char_from_room(ch);
			char_to_room(ch,chroom);
			obj_from_room(obj);
		}
		else if (obj->in_obj != NULL) obj_from_obj(obj);
		else continue;
		obj_to_char(obj,ch);
		if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
			REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
		if (!IS_HEAD(ch, LOST_HEAD)) {
			act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
			act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
		}
	}

    if (!found && !IS_HEAD(ch, LOST_HEAD))
		send_to_char("Nothing happens.\n\r", ch);

    for (d = descriptor_list; d != NULL; d = d->next) {
		if (d->connected != CON_PLAYING) continue;
		//if ((victim = d->character) == CON_READ_MOTD) continue;
		if ((victim = d->character) == NULL) continue;
		if (IS_NPC(victim)) continue;
		if (ch != victim && !IS_EXTRA(victim,EXTRA_CALL_ALL)) continue;
		REMOVE_BIT(victim->extra, EXTRA_CALL_ALL);
		do_autosave(victim,"");
	}
    return;
}



void do_artifact( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !IS_SET(obj->quest, QUEST_ARTIFACT) ) continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s created by %s and carried by %s.\n\r",
		obj->short_descr, obj->questmaker, 
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s created by %s and in %s.\n\r",
		obj->short_descr, obj->questmaker,
		in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "There are no artifacts in the game.\n\r", ch );

    return;
}

void do_locate( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || obj->questowner == NULL || 
	    strlen(obj->questowner) < 2 || str_cmp( ch->name, obj->questowner ))
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL
		    ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "You cannot locate any items belonging to you.\n\r", ch );

    return;
}

void do_claim( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( ch->exp < 500 ) {send_to_char("It costs 500 exp to claim ownership of an item.\n\r",ch); return;}

    if ( arg[0] == '\0' )
    {
	send_to_char( "What object do you wish to claim ownership of?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}

    if (obj->item_type == ITEM_QUEST || obj->item_type == ITEM_AMMO ||
        obj->item_type == ITEM_EGG   || obj->item_type == ITEM_VOODOO ||
        obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE ||
        obj->item_type == ITEM_PAGE  || IS_SET(obj->quest, QUEST_ARTIFACT) ||
        IS_SET(obj->quest, ITEM_EQUEST))
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }
    else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }

    if ( obj->questowner != NULL && strlen(obj->questowner) > 1 )
    {
	if (!str_cmp(ch->name,obj->questowner))
	    send_to_char("But you already own it!\n\r",ch);
	else
	    send_to_char("Someone else has already claimed ownership to it.\n\r",ch);
	return;
    }

    ch->exp -= 500;
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(ch->name);
    act("You are now the owner of $p.",ch,obj,NULL,TO_CHAR);
    act("$n is now the owner of $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_gift( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
//    OBJ_INDEX_DATA *pObjIndex = NULL;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( ch->exp < 500 ) {send_to_char("It costs 500 exp to make a gift of an item.\n\r",ch); return;}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Make a gift of which object to whom?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg1) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{send_to_char("Nobody here by that name.\n\r",ch); return;}

    if ( IS_NPC(victim) )
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }
/*Changed by Toaster 25/01/00 to make gifting quest items of value
illegal, technically we don't need the Proto ball check but can't help
to have in just incase.*/
    if ( obj->questowner == NULL || strlen(obj->questowner) < 2 )
    {
	send_to_char("That item has not yet been claimed.\n\r",ch);
	return;
    }
    if (str_cmp(ch->name,obj->questowner))
    {
	send_to_char("But you don't own it!\n\r",ch);
	return;
    }
    if ( !IS_IMMORTAL(ch) && (IS_SET(obj->quest, QUEST_MASTERY) ||
          IS_SET(obj->quest, QUEST_PRIZE) || IS_SET(obj->quest, QUEST_RELIC) ) )
    {
        send_to_char("You may not gift away Mastery items, Relics or Prizes.\n\r", ch );
        return;
    }

    ch->exp -= 500;
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(victim->name);
    act("You grant ownership of $p to $N.",ch,obj,victim,TO_CHAR);
    act("$n grants ownership of $p to $N.",ch,obj,victim,TO_NOTVICT);
    act("$n grants ownership of $p to you.",ch,obj,victim,TO_VICT);
    return;
}

void do_create( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int itemtype = 13;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' )               itemtype = ITEM_TRASH;
    else if (!str_cmp(arg1,"light"    )) itemtype = ITEM_LIGHT;
    else if (!str_cmp(arg1,"scroll"   )) itemtype = ITEM_SCROLL;
    else if (!str_cmp(arg1,"wand"     )) itemtype = ITEM_WAND;
    else if (!str_cmp(arg1,"staff"    )) itemtype = ITEM_STAFF;
    else if (!str_cmp(arg1,"weapon"   )) itemtype = ITEM_WEAPON;
    else if (!str_cmp(arg1,"treasure" )) itemtype = ITEM_TREASURE;
    else if (!str_cmp(arg1,"armor"    )) itemtype = ITEM_ARMOR;
    else if (!str_cmp(arg1,"armour"   )) itemtype = ITEM_ARMOR;
    else if (!str_cmp(arg1,"potion"   )) itemtype = ITEM_POTION;
    else if (!str_cmp(arg1,"furniture")) itemtype = ITEM_FURNITURE;
    else if (!str_cmp(arg1,"trash"    )) itemtype = ITEM_TRASH;
    else if (!str_cmp(arg1,"container")) itemtype = ITEM_CONTAINER;
    else if (!str_cmp(arg1,"drink"    )) itemtype = ITEM_DRINK_CON;
    else if (!str_cmp(arg1,"key"      )) itemtype = ITEM_KEY;
    else if (!str_cmp(arg1,"food"     )) itemtype = ITEM_FOOD;
    else if (!str_cmp(arg1,"money"    )) itemtype = ITEM_MONEY;
    else if (!str_cmp(arg1,"boat"     )) itemtype = ITEM_BOAT;
    else if (!str_cmp(arg1,"corpse"   )) itemtype = ITEM_CORPSE_NPC;
    else if (!str_cmp(arg1,"fountain" )) itemtype = ITEM_FOUNTAIN;
    else if (!str_cmp(arg1,"pill"     )) itemtype = ITEM_PILL;
    else if (!str_cmp(arg1,"portal"   )) itemtype = ITEM_PORTAL;
    else if (!str_cmp(arg1,"egg"      )) itemtype = ITEM_EGG;
    else if (!str_cmp(arg1,"stake"    )) itemtype = ITEM_STAKE;
    else if (!str_cmp(arg1,"missile"  )) itemtype = ITEM_MISSILE;
    else                                 itemtype = ITEM_TRASH;

    if ( arg2[0] == '\0' || !is_number( arg2 ) )
    {
	level = 0;
    }
    else
    {
        level = atoi( arg2 );
	if ( level < 1 || level > 50 )
        {
	    send_to_char( "Level should be within range 1 to 50.\n\r", ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform KaVir.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    obj->level = level;
    obj->item_type = itemtype;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);

    act( "You reach up into the air and draw out a ball of protoplasm.", ch, obj, NULL, TO_CHAR );
    act( "$n reaches up into the air and draws out a ball of protoplasm.", ch, obj, NULL, TO_ROOM );
    return;
}

void do_qtrust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: qtrust <char> <on/off>.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "Do you wish to set qtrust ON or OFF?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (str_cmp(arg2,"on") && str_cmp(arg2,"off"))
    {
	send_to_char( "Do you want to set their qtrust ON or OFF?\n\r", ch);
	return;
    }
    if (!str_cmp(arg2,"off"))
    {
	if (!IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their qtrust is already off.\n\r",ch);
	    return;
	}
	REMOVE_BIT(victim->extra, EXTRA_TRUSTED);
	send_to_char("Quest trust OFF.\n\r",ch);
	send_to_char("You are no longer quest trusted.\n\r",victim);
	return;
    }
    else if (!str_cmp(arg2,"on"))
    {
	if (IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their qtrust is already on.\n\r",ch);
	    return;
	}
	SET_BIT(victim->extra, EXTRA_TRUSTED);
	send_to_char("Quest trust ON.\n\r",ch);
	send_to_char("You are now quest trusted.\n\r",victim);
	return;
    }
    return;
}

void do_ntrust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: ntrust <char> <on/off>.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "Do you wish to set ntrust ON or OFF?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (str_cmp(arg2,"on") && str_cmp(arg2,"off"))
    {
	send_to_char( "Do you want to set their ntrust ON or OFF?\n\r",ch);
	return;
    }
    if (!str_cmp(arg2,"off"))
    {
	if (!IS_EXTRA(victim, EXTRA_NOTE_TRUST))
	{
	    send_to_char("Their ntrust is already off.\n\r",ch);
	    return;
	}
	REMOVE_BIT(victim->extra, EXTRA_NOTE_TRUST);
	send_to_char("Note trust OFF.\n\r",ch);
	send_to_char("You are no longer note trusted.\n\r",victim);
	return;
    }
    else if (!str_cmp(arg2,"on"))
    {
	if (IS_EXTRA(victim, EXTRA_NOTE_TRUST))
	{
	    send_to_char("Their ntrust is already on.\n\r",ch);
	    return;
	}
	SET_BIT(victim->extra, EXTRA_NOTE_TRUST);
	send_to_char("Note trust ON.\n\r",ch);
	send_to_char("You are now note trusted.\n\r",victim);
	return;
    }
    return;
}

void do_mclear( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: mclear <char>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (IS_NPC(victim))
    {
	send_to_char( "Not on NPC's.\n\r", ch);
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    for ( obj = victim->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->wear_loc != WEAR_NONE ) 
	{obj_from_char(obj); obj_to_char(obj,victim);}
    }
    while ( victim->affected )
	affect_remove( victim, victim->affected );
    if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    {
    	victim->affected_by	= AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    	victim->affected_by	= AFF_POLYMORPH;
    else if (IS_AFFECTED(victim,AFF_ETHEREAL))
    	victim->affected_by	= AFF_ETHEREAL;
    else
    	victim->affected_by	= 0;
    victim->armor	 = 100;
    victim->hit		 = UMAX( 1, victim->hit  );
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->pcdata->mod_str = 0;
    victim->pcdata->mod_int = 0;
    victim->pcdata->mod_wis = 0;
    victim->pcdata->mod_dex = 0;
    victim->pcdata->mod_con = 0;
    victim->pcdata->followers = 0;
    save_char_obj( victim );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    return;
}

void do_clearstats( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    if (IS_NPC(ch)) return;

    if (!IS_SET(ch->in_room->room_flags, ROOM_SAFE))
    {
	send_to_char("You can only clear your stats in a safe room.\n\r",ch);
	return;
    }
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->wear_loc != WEAR_NONE ) 
	{obj_from_char(obj); obj_to_char(obj,ch);}
    }
    while ( ch->affected )
	affect_remove( ch, ch->affected );
    if (IS_AFFECTED(ch,AFF_POLYMORPH) && IS_AFFECTED(ch,AFF_ETHEREAL))
    {
    	ch->affected_by	 = AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    	ch->affected_by	 = AFF_POLYMORPH;
    else if (IS_AFFECTED(ch,AFF_ETHEREAL))
    	ch->affected_by	 = AFF_ETHEREAL;
    else
    	ch->affected_by	 = 0;
    ch->armor		 = 100;
    ch->hit		 = UMAX( 1, ch->hit  );
    ch->mana		 = UMAX( 1, ch->mana );
    ch->move		 = UMAX( 1, ch->move );
    ch->hitroll		 = 0;
    ch->damroll		 = 0;
    ch->saving_throw	 = 0;
    ch->pcdata->mod_str	 = 0;
    ch->pcdata->mod_int	 = 0;
    ch->pcdata->mod_wis	 = 0;
    ch->pcdata->mod_dex	 = 0;
    ch->pcdata->mod_con	 = 0;
    free_string(ch->pcdata->love);
    ch->pcdata->love = str_dup( "" );
    REMOVE_BIT(ch->newbits, NEW_VISCERAL);
    REMOVE_BIT(ch->monkstuff, MONK_DEATH);
/* Chnged Toaster to make clearstats remove visceral*/
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

void do_otransfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Otransfer which object?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' ) victim = ch;
    else if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || 
	str_cmp(ch->name,obj->questmaker) || strlen(obj->questmaker) < 2))
    {
	send_to_char("You don't have permission to otransfer that item.\n\r", ch);
	return;
    }

    if (obj->carried_by != NULL)
    {
	if (!IS_NPC(obj->carried_by) && IS_SET(obj->carried_by->act,PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
	{
	    send_to_char("You failed.\n\r",ch);
	    act("$p flickers briefly with energy.",obj->carried_by,obj,NULL,TO_CHAR);
	    return;
	}
	act("$p vanishes from your hands in an explosion of energy.",obj->carried_by,obj,NULL,TO_CHAR);
	act("$p vanishes from $n's hands in an explosion of energy.",obj->carried_by,obj,NULL,TO_ROOM);
	obj_from_char(obj);
    }
    else if (obj->in_obj     != NULL) obj_from_obj(obj);
    else if (obj->in_room != NULL)
    {
    	chroom = ch->in_room;
    	objroom = obj->in_room;
    	char_from_room(ch);
    	char_to_room(ch,objroom);
    	act("$p vanishes from the ground in an explosion of energy.",ch,obj,NULL,TO_ROOM);
	if (chroom == objroom) act("$p vanishes from the ground in an explosion of energy.",ch,obj,NULL,TO_CHAR);
    	char_from_room(ch);
    	char_to_room(ch,chroom);
	obj_from_room(obj);
    }
    else
    {
	send_to_char( "You were unable to get it.\n\r", ch );
	return;
    }
    obj_to_char(obj,victim);
    act("$p appears in your hands in an explosion of energy.",victim,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in an explosion of energy.",victim,obj,NULL,TO_ROOM);
    return;
}

void bind_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *location;

    if ( IS_NPC(ch) || ch->pcdata->obj_vnum < 1 )
	return;

    if ( ( pObjIndex = get_obj_index( ch->pcdata->obj_vnum ) ) == NULL )
	return;

    if ( ch->in_room == NULL || ch->in_room->vnum == ROOM_VNUM_IN_OBJECT )
    {
	location = get_room_index(ROOM_VNUM_ALTAR);
    	char_from_room(ch);
    	char_to_room(ch,location);
    }
    else location = ch->in_room;

    obj = create_object( pObjIndex, 50 );
    obj_to_room(obj,location);
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    send_to_char( "You reform yourself.\n\r", ch );
    act("$p fades into existance on the floor.",ch,obj,NULL,TO_ROOM);
    do_look(ch,"auto");
    return;
}

void do_bind( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: bind <player> <object>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    if (ch == victim)
    {
	send_to_char( "You can't do this to yourself.\n\r", ch);
	return;
    }
    if (IS_NPC(victim))
    {
	send_to_char( "Not on NPC's.\n\r", ch);
	return;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    {
	send_to_char( "You cannot do this while they are polymorphed.\n\r", ch);
	return;
    }
    else if (IS_IMMORTAL(victim))
    {
	send_to_char( "Only on mortals or avatars.\n\r", ch);
	return;
    }
    else if (IS_SET(victim->act, PLR_GODLESS) && get_trust(ch) < NO_GODLESS
        && !IS_SET(ch->extra , EXTRA_ANTI_GODLESS))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg2) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	send_to_char("You cannot bind someone into a modified item.\n\r",ch);
	return;
    }
    if (obj->chobj != NULL)
    {
	send_to_char( "That item already has someone bound in it.\n\r", ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    act("$n transforms into a white vapour and pours into $p.",victim,obj,NULL,TO_ROOM);
    act("You transform into a white vapour and pour into $p.",victim,obj,NULL,TO_CHAR);
    victim->pcdata->obj_vnum = obj->pIndexData->vnum;
    obj->chobj = victim;
    victim->pcdata->chobj = obj;
    SET_BIT(victim->affected_by, AFF_POLYMORPH);
    SET_BIT(victim->extra, EXTRA_OSWITCH);
    free_string(victim->morph);
    victim->morph = str_dup(obj->short_descr);
    return;
}

void do_release( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: release <object>\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if ( (victim = obj->chobj) == NULL)
    {
	send_to_char( "There is nobody bound in that item.\n\r", ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    victim->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    victim->pcdata->chobj = NULL;
    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(victim->extra, EXTRA_OSWITCH);
    free_string(victim->morph);
    victim->morph = str_dup("");
    act("A white vapour pours out of $p and forms into $n.",victim,obj,NULL,TO_ROOM);
    act("Your spirit floats out of $p and reforms its body.",victim,obj,NULL,TO_CHAR);
    return;
}


void do_morph( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *morph;
    int mnum;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: morph <object> <form>\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_wear( ch, arg1) ) == NULL )
    {
    	if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
	{
	    send_to_char("You are not carrying that item.\n\r",ch);
	    return;
	}
    }

    if ( !IS_SET(obj->spectype, SITEM_MORPH) )
    {
	send_to_char("That item cannot morph.\n\r",ch);
	return;
    }

    if ( arg2[0] == '\0' )
    {
	act("$p can morph into the following forms:",ch,obj,NULL,TO_CHAR);
	show_list_to_char( obj->contains, ch, TRUE, TRUE );
	return;
    }

    if ( ( morph = get_obj_list( ch, arg2, obj->contains ) ) == NULL )
    {
	send_to_char("It cannot assume that form.\n\r",ch);
	return;
    }
    obj_from_obj( morph );
    obj_to_char( morph, ch );
    act( "$p morphs into $P in $n's hands!", ch, obj, morph, TO_ROOM );
    act( "$p morphs into $P in your hands!", ch, obj, morph, TO_CHAR );
    mnum = obj->wear_loc;
    obj_from_char( obj );
    obj_to_obj( obj, morph );
    if ( morph->wear_flags == obj->wear_flags && mnum != WEAR_NONE )
	equip_char( ch, morph, mnum );
    return;
}

void do_clannum(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int value;

	if (IS_NPC(ch)) return;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);


	if (arg1[0] == '\0' || arg2[0] == '\0') {
	send_to_char("Syntax:  clannum [char] [number]\n\r\n\r", ch );
	return;}

	value = is_number(arg2) ? atoi(arg2) : -1;

	if ((victim = get_char_world(ch,arg1)) == NULL) {
	send_to_char("They are not here.\n\r", ch );
	return;}

	if (IS_NPC(victim)) {
	send_to_char("Not on NPCs.\n\r", ch );
	return;}

	if (value < 0 || value > MAX_KINGDOM) {
	send_to_char("That is an invalid number.\n\r", ch );
	return;}

	victim->pcdata->kingdom = value;
	send_to_char("Clannum changed.\n\r", ch );
	if (IS_SET(victim->special, SPC_PRINCE))
	REMOVE_BIT(victim->special, SPC_PRINCE);
	send_to_char("Your clan has been changed.\n\r", victim);
	save_char_obj(victim);
	return;
}
	

void do_clanset(CHAR_DATA *ch, char *argument) {

	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	int value;
	char *arg3;

	smash_tilde(argument);
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	arg3 = argument;

	if (IS_NPC(ch)) return;

	if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {

	send_to_char("Syntax:  clanset [clannum] [thing] [value]\n\r\n\r", ch );
	send_to_char("things:  name, whoname, leader, pk, pd, exp, mem\n\r\n\r", ch );
	return;}

	value = is_number(arg1) ? atoi(arg1) : -1;

	if (value < 0 || value > MAX_KINGDOM) {
	send_to_char("That is an invalid clan number.\n\r", ch );
	return;}

	if (!str_cmp(arg2,"name")) {
	free_string(king_table[value].name); 
	king_table[value].name = str_dup(arg3);}
	else if (!str_cmp(arg2,"whoname")) {
	free_string(king_table[value].who_name);
	king_table[value].who_name = str_dup(arg3);}
	else if (!str_cmp(arg2,"leader")) {
	free_string(king_table[value].leader_name);
	king_table[value].leader_name = str_dup(arg3);}
        else if (!str_cmp(arg2,"pk")) {
        king_table[value].pk = atoi(arg3);}
        else if (!str_cmp(arg2,"pd")) {
        king_table[value].pd = atoi(arg3);}
        else if (!str_cmp(arg2,"exp")) {
        king_table[value].exp = atoi(arg3);}
        else if (!str_cmp(arg2,"mem")) {
        king_table[value].mem = atoi(arg3);}
	else 
		send_to_char("Nothing set.\n\r", ch );

	send_to_char("Done.\n\r", ch );
	save_kingdoms();
	return;
}


/*
void do_shell( CHAR_DATA *ch, char *argument )
{

    sysmes( argument );
    return;
}
*/

extern int port,control; /* db.c */

void do_copyover (CHAR_DATA *ch, char * argument)
{
        FILE *fp;
        DESCRIPTOR_DATA *d, *d_next;
        char buf [100], buf2[100];

        fp = fopen (COPYOVER_FILE, "w");

        if (!fp)   
        {
                send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
                logf (NULL,"Could not write to copyover file: %s", COPYOVER_FILE);
                perror ("do_copyover:fopen");
                return;
        }

        /* Consider changing all saved areas here, if you use OLC */

        do_peace(ch, "");
        do_wake(ch,"");
        do_restore(ch,"all");
        do_call( ch,"all");
        do_save(ch,"");

//        imc_shutdown();
        if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
        {
		REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	  }        
          if ( IS_SET(ch->flag2, AFF2_INARENA) || IS_SET(ch->flag2,AFF2_CHALLENGED) 
          || IS_SET(ch->flag2, AFF2_CHALLENGER))
         {
            REMOVE_BIT(ch->flag2, AFF2_CHALLENGED);
            REMOVE_BIT(ch->flag2, AFF2_CHALLENGER);
            REMOVE_BIT(ch->flag2, AFF2_INARENA   );
            undo_arena(ch);
            arena = FIGHT_CLEAR;
         }

         do_asave (NULL, "");

        sprintf (buf, "\n\r *** COPYOVER - please remain seated! ***\n\r");
        ADD_COLOUR( ch, buf, L_RED );

        /* For each playing descriptor, save its state */
        for (d = descriptor_list; d ; d = d_next)
        {
                CHAR_DATA * och = CH (d);
                d_next = d->next; /* We delete from the list , so need to save this */
                
                if (!d->character || d->connected > CON_PLAYING) /* drop those logging on */
                {
                        write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
                        close_socket (d); /* throw'em out */
               }
                else
                {
                        fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
                        save_char_obj (och);
                        write_to_descriptor (d->descriptor, buf, 0);
                }
        }
         
        fprintf (fp, "-1\n");
        fclose (fp);
                
        /* Close reserve and other always-open files and release other resources */
                 
        fclose (fpReserve);
       /* exec - descriptors are inherited */
                
        sprintf (buf, "%d", port);
        sprintf (buf2, "%d", control);
/*Changed by Toaster 19/01/00*/
/*Changed by Tracker 02/12/03*/
        execl (EXE_FILE, "Alternate", buf, "copyover", buf2, (char *) NULL);
                        
        /* Failed - sucessful exec will not return */
         
        perror ("do_copyover: execl");
        send_to_char ("Copyover FAILED!\n\r",ch);
        
        /* Here you might want to reopen fpReserve */
}
/* Recover from a copyover - load players */
void copyover_recover ()
{
        DESCRIPTOR_DATA *d;
        FILE *fp;
        char name [100];
        char host[MAX_STRING_LENGTH];
        int desc;       
        bool fOld;
         
        logf (NULL,"Copyover recovery initiated");
        
        fp = fopen (COPYOVER_FILE, "r");
        
        if (!fp) /* there are some descriptors open which will hang forever then ? */
        {        
                perror ("copyover_recover:fopen");
                logf (NULL,"Copyover file not found. Exitting.\n\r");
               exit (1);
        }
        
        unlink (COPYOVER_FILE); /* In case something crashes - doesn't prevent reading  */
        
        for (;;)        
        {
                fscanf (fp, "%d %s %s\n", &desc, name, host);
                if (desc == -1)
                        break;
        
                /* Write something, and check if it goes error-free */
                if (!write_to_descriptor (desc, "\n\rRestoring from copyover...\n\r",0))
                {
                        close (desc); /* nope */  
                        continue;
               }
         
                d = alloc_perm (sizeof(DESCRIPTOR_DATA));
                init_descriptor (d,desc); /* set up various stuff */
        
                d->host = str_dup (host);
                d->next = descriptor_list;
                descriptor_list = d;
                d->connected = CON_COPYOVER_RECOVER; /* -15, so close_socket frees the char */
                        
        
                /* Now, find the pfile */
                
                fOld = load_char_obj (d, name);
                        
                if (!fOld) /* Player file not found?! */
               {
                        write_to_descriptor (desc, "\n\rSomehow, your character was lost in the copyover.Sorry.\n\r", 0);
                        close_socket (d);
                }
                else /* ok! */
                {
                        write_to_descriptor (desc, "\n\rCopyover recovery complete.\n\r",0);
                
                        /* Just In Case */
                        if (!d->character->in_room)
                                d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);
                
                        /* Insert in the char_list */
                        d->character->next = char_list;
                        char_list = d->character;
               
                        char_to_room (d->character, d->character->in_room);
                        do_look (d->character, "");
                        act ("$n materializes!", d->character, NULL, NULL, TO_ROOM);
                        d->connected = CON_PLAYING;
                }
                 
        }
                
        fclose (fp);
}

void do_auto_shutdown()
{
   FILE *fp;
   DESCRIPTOR_DATA *d, *d_next;
   char buf [100], buf2[100];
   extern int port,control;
	
   fp = fopen (COPYOVER_FILE, "w");   

   if (!fp)
   {
       for (d = descriptor_list;d != NULL;d = d_next)
       {
           if(d->character)
           {
              do_save(d->character,"");
//              do_function( d->character, &do_save, "" );
              send_to_char("Ok I tried but we're crashing anyway sorry!\n\r",d->character);
           }

           d_next=d->next;
           close_socket(d);
       }
         
       exit(1);
   }
	
//   do_function( NULL, &do_asave, "changed" );
//   do_asave(NULL,"changed");
   sprintf(buf,"\n\rYour mud is crashing attempting a copyover now!\n\r");
   
   for (d = descriptor_list; d ; d = d_next)
   {
	CHAR_DATA * och = CH (d);
	d_next = d->next; /* We delete from the list , so need to save this */
		
	if (!d->character || d->connected > CON_PLAYING) 
	{
	    write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0);
	    close_socket (d); /* throw'em out */
	}
		
        else
	{
	    fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
	    save_char_obj (och);
	    write_to_descriptor (d->descriptor, buf, 0);
	}
   }
	
   fprintf (fp, "-1\n");
   fclose(fp);
   fclose (fpReserve);
   sprintf (buf, "%d", port);
   sprintf (buf2, "%d", control);
//   execl (EXE_FILE, "rom", buf, "copyover", buf2, (char *) NULL);   
   execl (EXE_FILE, "Alternate", buf, "copyover", buf2, (char *) NULL);
   exit(1);
}

void do_autobalance(CHAR_DATA *ch, char *argument) 
 { 
     MOB_INDEX_DATA *pMob; 
     char buf[MSL], arg1[MIL], arg2[MIL]; 
     int vnum, min, max, cnt=0, num1=0, num2=0, newlev=0; 
  
     argument = one_argument(argument, arg1); 
     argument = one_argument(argument, arg2); 
  
     if (ch->in_room->area == NULL) return; 
  
     if (!is_number(arg1) || !is_number(arg2)){ 
         stc("Syntax:\n\r       autobalance <lowlev> <highlev>\n\r", ch); 
         return; 
     } 
  
     num1 = atoi(arg1); 
     num2 = atoi(arg2); 
  
     if (num1<= 0 || num2<= 0){ 
         stc("Both values must be above 0.\n\r", ch); 
         return; 
     } 
  
     if (num2 < num1){ 
         stc("The second argument must be larger than the first.\n\r", ch); 
         return; 
     } 
  
     min = ch->in_room->area->lvnum; 
     max = ch->in_room->area->uvnum; 
  
     for (vnum = min; vnum< max; vnum++) 
     { 
         if ((pMob = get_mob_index(vnum)) != NULL){ 
             if (pMob->level< num1 || pMob->level> num2){ 
                 newlev = number_range(num1, num2); 
                 sprintf(buf, "%4d>%s changed from level %d to level %d.\n\r", ++cnt, pMob->short_descr, pMob->level, newlev); 
                 pMob->level = newlev; 
                   SET_BIT(ch->in_room->area->area_flags, AREA_CHANGED); 
                 stc(buf, ch); 
             } 
         } 
     } 
 }

char        *dxp_sprintf (bool pTime, bool reason)
{
   static char  buf[MAX_STRING_LENGTH];

	 sprintf (buf, "%.2f %s left of Double Exp.\n\r",
		  dxp_info.dxpt >= 240 ? (float) dxp_info.dxpt / 60 /
		  PULSE_PER_SECOND : (float) dxp_info.dxpt / PULSE_PER_SECOND,
		  dxp_info.dxpt >= 240 ? "minute(s)" : "second(s)");
   return buf;
}
char        *chs_sprintf (bool pTime, bool reason)
{
   static char  buf[MAX_STRING_LENGTH];

	 sprintf (buf, "%.2f %s left of CHAOS.\n\r",
		  chao_info.chst >= 240 ? (float) chao_info.chst / 60 /
		  PULSE_PER_SECOND : (float) chao_info.chst / PULSE_PER_SECOND,
		  chao_info.chst >= 240 ? "minute(s)" : "second(s)");
   return buf;
}
char        *dqp_sprintf (bool pTime, bool reason)
{
   static char  buf[MAX_STRING_LENGTH];

	 sprintf (buf, "%.2f %s left of Double Quest Points.\n\r",
		  dqp_info.dqpt >= 240 ? (float) dqp_info.dqpt / 60 /
		  PULSE_PER_SECOND : (float) dqp_info.dqpt / PULSE_PER_SECOND,
		  dqp_info.dqpt >= 240 ? "minute(s)" : "second(s)");
   return buf;
}
char        *dtr_sprintf (bool pTime, bool reason)
{
   static char  buf[MAX_STRING_LENGTH];

	 sprintf (buf, "%.2f %s left of Double Training.\n\r",
		  dtr_info.dtrt >= 240 ? (float) dtr_info.dtrt / 60 /
		  PULSE_PER_SECOND : (float) dtr_info.dtrt / PULSE_PER_SECOND,
		  dtr_info.dtrt >= 240 ? "minute(s)" : "second(s)");
   return buf;
}

void do_uptime(CHAR_DATA * ch)
{
//   extern char boot_time[];
   extern time_t str_boot_time;
   extern time_t current_time;
   char buf[MSL];
   int days;
   int hours;
   int minutes;
   int seconds;
   int uptime;
   uptime = (int) (current_time - str_boot_time);


   days = (uptime / 86400);
   hours = ((uptime / 3600) % 24);
   minutes = ((uptime / 60) % 60);
   seconds = (uptime % 60);

   sprintf (buf,
	    "{wWe have been up for {c%d {mday%s{w, {c%d {mhour%s{w, {c%d {mminute%s{w, and {c%d {msecond%s{w.{x\n\r",
	    days, days == 1 ? "" : "s", hours, hours == 1 ? "" : "s", minutes,
	    minutes == 1 ? "" : "s", seconds, seconds == 1 ? "" : "s");
   send_to_char (buf, ch);
}

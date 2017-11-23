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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * Local functions.
 */
//bool	is_note_to	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
//void	note_attach	args( ( CHAR_DATA *ch ) );
//void	note_remove	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void	talk_channel	args( ( CHAR_DATA *ch, char *argument,
			    int channel, const char *verb ) );
bool is_in		args( (char *, char *) );
bool all_in		args( (char *, char *) );
char * oldelang		args( ( CHAR_DATA *ch, char *argument ) );
char * badlang		args( ( CHAR_DATA *ch, char *argument ) );
char * darktongue	args( ( CHAR_DATA *ch, char *argument ) );
char * drunktalk	args( ( CHAR_DATA *ch, char *argument ) );
char * beasttalk        args( ( CHAR_DATA *ch, char *argument ) );
char * beastchat        args( ( CHAR_DATA *ch, char *argument ) );
char * socialc		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );
char * socialv		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );
char * socialn		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );




/*
 * Generic channel function.
 * Changed by Kyrlin to add altreal colours 20/01/00
 */
void talk_channel( CHAR_DATA *ch, char *argument, int channel, const char *verb )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int pos;
	bool ignored;
    int position;
	if (IS_NPC(ch)) return;

     if ( IS_SET(ch->in_room->room_flags, ROOM_SILENCE) )
    {
     send_to_char("Something prevents you from speaking in this room.\n\r", ch);
     return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "%s what?\n\r", verb );
	buf[0] = UPPER(buf[0]);
	return;
    }

    if ( IS_HEAD(ch, LOST_TONGUE) )
    {
	sprintf( buf, "You can't %s without a tongue!\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    if ( IS_EXTRA(ch, GAGGED) )
    {
	sprintf( buf, "You can't %s with a gag on!\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	sprintf( buf, "You can't %s.\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

 if (ch->in_room != NULL && IS_SET(ch->in_room->added_flags,
ROOM2_SILENCE))
    {
	sprintf( buf, "You can't %s.\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    REMOVE_BIT(ch->deaf, channel);

    switch ( channel )
    {
    default:

       sprintf(buf,"{wYou {W%s {D'{c$T{D'.{x", verb);
        if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
            act( buf, ch, NULL, drunktalk(ch, argument), TO_CHAR );
        else if (!IS_NPC(ch) && ch->pcdata->stats[UNI_RAGE] > 0 && IS_VAMPIRE(ch) &&
            !IS_SET(world_affects, WORLD_NICE))
            act( buf, ch, NULL, argument, TO_CHAR );
        else
            act( buf, ch, NULL, argument, TO_CHAR );

        sprintf( buf, "{w$n {W%ss {D'{c$t{D'.{x",     verb );
        sprintf( buf2, "{w$n {W%ss {D'{c$t{D'.{x",     verb );
        break;
    case CHANNEL_IMMTALK:
	sprintf( buf, "#c%s [#w%d#c]:#C $t#n.",capitalize(ch->name), ch->pcdata->wizinvis );
	sprintf( buf2, "#c%s [#w%d#c]:#C $t#n.",capitalize(ch->name), ch->pcdata->wizinvis );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

    case CHANNEL_PINFO:
	  sprintf( buf, "#e[#gInfo#e] #w->#n $t#n" );  
        sprintf( buf2, "#e[#gInfo#e] #w->#n $t#n" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR);
        ch->position    = position;
        break;

   case CHANNEL_FTALK:
        sprintf( buf, "#w-[#y%s#w]- '#y$t#w'.#n" , capitalize( ch->name) );
        sprintf( buf2, "#w-[#y%s#w]- '#y$t#w'.#n", capitalize( ch->name) );  
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

    case CHANNEL_PRAY:
	sprintf( buf, "#rYou #rp#Rr#ra#Ry#w '#r$t#w'#n." );
//            ADD_COLOUR( ch, buf, L_RED );
	sprintf( buf2, "#r$n #rp#Rr#ra#Ry#rs#w '#r$t#w'#n." );
//             ADD_COLOUR( ch, buf2, L_RED );   
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;
	
    case CHANNEL_BUILD:
	sprintf( buf, "#c$n#w =#C $t.#n" );
	sprintf( buf2, "#c$n#w =#C $t.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

    case CHANNEL_MAGETALK:
	sprintf( buf, "{m({c<{C$n{c>{m){D '{w$t{D'.{x" );
	sprintf( buf2, "{m({c<{C$n{c>{m){D '{w$t{D'.{x" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;


	case CHANNEL_CLANTALK:
		sprintf( buf, "#wYou clantalk '#G$t#w'.#n" );
		sprintf(buf2, "#w$n clantalks '#G$t#w'.#n" );
		position = ch->position;
		act( buf, ch, argument, NULL, TO_CHAR );
		ch->position = position;
		break;

    case CHANNEL_HOWL:
	sprintf( buf, "#oYou howl#n '#y$t#n'." );
//         ADD_COLOUR( ch, buf, PINK ); 
	sprintf( buf2, "#o$n howls#n '#y$t#n'." );
//         ADD_COLOUR( ch, buf2, PINK );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;
	
	case CHANNEL_BITCH:
	sprintf( buf, "{c$n{w, the {Cbitch {wstates {D'{w$t{D'." );
	sprintf( buf2, "{c$n, the {Cbitch {wstates {D'{w$t{D'." );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;



          case CHANNEL_KTALK:
	sprintf( buf, "#G.x[#w%s#G]x.#n '#p$t#n'.{x", ch->name);
	sprintf(buf2, "#G.x[#w%s#G]x.#n '#p$t#n'.{x", ch->name);
	position = ch->position;
	ch->position = POS_STANDING;
	act(buf, ch, argument, NULL, TO_CHAR);
	ch->position = position;
	break;

	case CHANNEL_MIKTALK:
        sprintf( buf, "#c**#p$n#c** #n'#P$t#n'." );
        sprintf( buf2, "#c**#p$n#c** #n'#P$t#n'." );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

	case CHANNEL_HIGHTALK:
                sprintf( buf, "#o-#y=#b$n#y=#o- #n '#b$t#n'." );
                sprintf( buf2, "#o-#y=#b$n#y=#o- #n '#b$t#n'." );
		position = ch->position;
		act( buf, ch, argument, NULL, TO_CHAR );
		ch->position = position;
	
	break;
       case CHANNEL_MONK:
        sprintf( buf, "#g$n #Ghums #n'#g$t#n'." );
        sprintf( buf2, "#g$n #Ghums #n'#g$t#n'." );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break; 

    case CHANNEL_SIGN:
        sprintf( buf, "#G.o0#e$n#G0o. #n'#e$t#n'." );
        sprintf( buf2, "#G.o0#e$n#G0o. #n'#e$t#n'." );
        position        = ch->position;
        ch->position    = POS_STANDING;  
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;


	case CHANNEL_VAMPTALK:
	if (!IS_NPC(ch) && (ch->pcdata->stats[UNI_GEN] == 1 ||
(IS_SET(ch->special, SPC_ANARCH) && IS_ABOMINATION(ch))))
	{
	    sprintf( buf, "#R<[#r$n#R]> #R$t#n." );
	    sprintf( buf2, "#R<[#r$n#R]> #R$t#n." );
	}
	else if (!IS_NPC(ch) && ch->pcdata->stats[UNI_GEN] == 2)
	{
	    sprintf( buf, "#R<<#r$n#R>> #R$t#n." );
	    sprintf( buf2, "#R[[#r$n#R]] #R$t#n." );
	}
	else
	{
	    sprintf( buf, "#R<#r$n#R> #R$t#n." );
	    sprintf( buf2, "#R[#r$n#R] #R$t#n." );
	}

	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *och;
	CHAR_DATA *vch;



	och = d->original ? d->original : d->character;
	vch = d->character;

        ignored = FALSE;

	if ( d->connected == CON_PLAYING
	&&   vch != ch
	&&  !IS_SET(och->deaf, channel) )
	{

	for (pos = 0; pos < MAX_FORGET; pos++) {
	if (och->pcdata->forget[pos] == NULL) break;
	if (!str_cmp(ch->name, och->pcdata->forget[pos]))
	ignored = TRUE;
	}
	if (ignored) continue;

           if ( channel == CHANNEL_BUILD && !IS_IMMORTAL(och) )
		continue;
	    if ( channel == CHANNEL_IMMTALK && !IS_IMMORTAL(och) )
		continue;
	    if (channel== CHANNEL_MIKTALK && (!IS_NPC(och) && 
	    !IS_CLASS(och, CLASS_NINJA) && !IS_IMMORTAL(och)))
		continue;
	    if ( channel == CHANNEL_HIGHTALK && (!IS_NPC(och) &&		
		!IS_CLASS(och, CLASS_HIGHLANDER) && !IS_IMMORTAL(och)))
		continue;
            if (channel == CHANNEL_KTALK && (!IS_NPC(och) && 
	!IS_IMMORTAL(och) && ch->pcdata->kingdom != och->pcdata->kingdom))
	continue;
	if ( channel == CHANNEL_MONK && (!IS_NPC(och) &&
                !IS_CLASS(och, CLASS_MONK) && !IS_IMMORTAL(och)))
                continue; 
          if ( channel == CHANNEL_FTALK && (!IS_NPC(och) &&
            !IS_SET(och->newbits, NEW_FTALK) && !IS_IMMORTAL(och) ))
                continue;
	    if ( channel == CHANNEL_SIGN && (!IS_NPC(och) &&
                !IS_CLASS(och, CLASS_DROW) && !IS_IMMORTAL(och)))
                continue;
	    if (channel == CHANNEL_CLANTALK && (!IS_NPC(och) && 
	!IS_IMMORTAL(och) && ch->clan != och->clan))
	continue;
	    if ( channel == CHANNEL_VAMPTALK && (!IS_NPC(och) && 
		!IS_CLASS(och, CLASS_VAMPIRE) && !IS_IMMORTAL(och)))
		continue;
            if ( channel == CHANNEL_MAGETALK && (!IS_NPC(och) &&
	        !IS_CLASS(och, CLASS_MAGE) && !IS_IMMORTAL(och)))
		continue;
            if ( channel == CHANNEL_PRAY && (!IS_NPC(och) &&
	        !IS_CLASS(och, CLASS_DEMON) && !IS_IMMORTAL(och)))
		continue;
            if ( channel == CHANNEL_HOWL && (!IS_NPC(och) &&
	        !IS_CLASS(och, CLASS_WEREWOLF) && !IS_POLYAFF(och,POLY_WOLF)
		&& !IS_IMMORTAL(och)))
	    {
	        if ((och->in_room) && (ch->in_room))
	    	{
			if ((och->in_room == ch->in_room))
			{
		    		act("$n throws back $s head and howls loudly.", ch , argument , och, TO_VICT);
		    		continue;
			} 
			else if ((och->in_room->area == ch->in_room->area ))
			{
		    		act("You hear a loud howl nearby.", ch , NULL , och, TO_VICT);
		    		continue;
			}
			else 
			{
		    		act("You hear a loud howl in the distance.", ch , NULL , och, TO_VICT);
	            		continue;
			}
	    	}
	    }
            
	    if ( channel == CHANNEL_YELL
	    &&   vch->in_room->area != ch->in_room->area )
		continue;

	    position		= vch->position;
	    if ( channel != CHANNEL_SHOUT && channel != CHANNEL_YELL )
		vch->position	= POS_STANDING;

	    if (!IS_NPC(vch) && IS_CLASS(vch, CLASS_VAMPIRE) && str_cmp(ch->clan,vch->clan) )
	    {
	    	act( buf2, ch, argument, vch, TO_VICT );
	    	vch->position	= position;
		continue;
	    }
	    if (!IS_NPC(vch) && IS_CLASS(vch, CLASS_WEREWOLF)) 
	    {
	    	act( buf2, ch, argument, vch, TO_VICT );
	    	vch->position	= position;
		continue;
	    }
	    if (!IS_NPC(vch) && (IS_CLASS(vch, CLASS_DEMON)||IS_IMMORTAL(vch)))
	    {
	    	act( buf2, ch, argument, vch, TO_VICT );
	    	vch->position	= position;
		continue;
	    }
	    if (!IS_NPC(vch) && (IS_CLASS(vch,CLASS_MAGE)||IS_IMMORTAL(vch)))
	    {
	    	act( buf2, ch, argument, vch, TO_VICT );
	    	vch->position	= position;
		continue;
	    }
	  if (!IS_NPC(vch) && (IS_CLASS(vch, CLASS_HIGHLANDER)||IS_IMMORTAL(vch))) {
		act(buf2, ch, argument, vch, TO_VICT);
		vch->position = position;
		continue; }

	    act( buf, ch, argument, vch, TO_VICT );
	    vch->position	= position;
	}
    }

    return;
}


/*
void do_auction( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_AUCTION, "auction" );
    return;
}
*/


void do_chat( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_CHAT, "chat" );
    return;
}



/*
 * Alander's new channels.
 */

void do_pinfo( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_PINFO, "pinfo" );
    return;
}

void do_music( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_MUSIC, "music" );
    return;
}



void do_question( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_QUESTION, "question" );
    return;
}



void do_answer( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_QUESTION, "answer" );
    return;
}



void do_shout( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_SHOUT, "shout" );
    WAIT_STATE( ch, 12 );
    return;
}



void do_yell( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_YELL, "yell" );
    return;
}

void do_buildtalk( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_BUILD, "buildtalk" );
    return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_IMMTALK, "immtalk" );
    return;
}
void do_monktalk( CHAR_DATA *ch, char *argument ) {
	if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch, CLASS_MONK)))
{
	send_to_char("Huh?\n\r", ch);
	return;}
	talk_channel(ch, argument, CHANNEL_MONK, "monk");
	return;
}

void do_ktalk( CHAR_DATA *ch, char *argument ) 
{
	if (IS_NPC(ch)) return;
	if (ch->pcdata->kingdom == 0) {
	send_to_char("Huh?\n\r", ch );
	return;}
	talk_channel(ch, argument, CHANNEL_KTALK, "ktalk" );
}

void do_miktalk( CHAR_DATA *ch, char *argument )
{

	if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch,
CLASS_NINJA)))
	{send_to_char("Huh?\n\r", ch);
	return;}
	talk_channel(ch, argument, CHANNEL_MIKTALK, "miktalk");
	return;
}
void do_ftalk( CHAR_DATA *ch, char *argument )
{
        if (IS_NPC(ch) || (!IS_SET(ch->newbits, NEW_FTALK) &&
        !IS_IMMORTAL(ch)))
        {
           stc("Huh?\n\r", ch);
           return;
        }
        talk_channel(ch, argument, CHANNEL_FTALK, "ftalk");
        return;
}
void do_hightalk( CHAR_DATA *ch, char *argument )
{
	
       if (IS_NPC(ch) || (!IS_IMMORTAL(ch) &&
!IS_CLASS(ch,CLASS_HIGHLANDER)))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	talk_channel( ch, argument, CHANNEL_HIGHTALK, "hightalk" );
	return;
}

void do_sign( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch,CLASS_DROW)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    talk_channel( ch, argument, CHANNEL_SIGN, "sign" );
    return;
}

void do_bitch( CHAR_DATA *ch, char *argument )
{
        if (IS_NPC(ch)) return;
        talk_channel(ch, argument, CHANNEL_BITCH, "bitch" );
}

void do_clantalk( CHAR_DATA *ch, char *argument ) 
{
	if (IS_NPC(ch)) return;
	if ( strlen(ch->clan) < 3 )
	{
  	  send_to_char("Huh?\n\r", ch );
  	  return;
	}
	
	talk_channel(ch, argument, CHANNEL_CLANTALK, "clantalk" );
}


void do_vamptalk( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch,CLASS_VAMPIRE)))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_VAMPTALK, "vamptalk" );
    return;
}

void do_magetalk( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC( ch ) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch,CLASS_MAGE)))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_MAGETALK, "magetalk" );
    return;
}


void do_pray( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    
    act("You mutter a few prayers.",ch,NULL,NULL,TO_CHAR);
    act("$n mutters a quick prayer.",ch,NULL,NULL,TO_ROOM);

  if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_ALTAR &&
      ch->class == CLASS_NONE && ch->position != POS_FIGHTING)
      {
                send_to_char("You glow bright blue as you are restored.\n\r",ch);
                ch->hit  = ch->max_hit;
                ch->mana = ch->max_mana;
                ch->move = ch->max_move;
                ch->loc_hp[0] = 0;
                ch->loc_hp[1] = 0;
                ch->loc_hp[2] = 0;
                ch->loc_hp[3] = 0;
                ch->loc_hp[4] = 0;
                ch->loc_hp[5] = 0;
                ch->loc_hp[6] = 0;
                update_pos(ch);
        }
    if (IS_NPC(ch) ||(!IS_CLASS(ch,CLASS_DEMON) && !IS_IMMORTAL(ch))) 
	return;

    if ( argument[0] == '\0' && IS_SET(ch->special, SPC_DEMON_LORD))
    {
	send_to_char("What do you wish to pray?\n\r",ch);
	return;
    }
    else if ( argument[0] == '\0' )
    {
	if (ch->pcdata->stats[DEMON_CURRENT] < 1)
	{
	    send_to_char("Nothing happens.\n\r",ch);
	    return;
	}
	if ( ( victim = get_char_world(ch, ch->lord) ) == NULL )
	{
	    send_to_char("Nothing happens.\n\r",ch);
	    return;
	}
	act("You hear $n's prayers in your mind.",ch,NULL,victim,TO_VICT);
	send_to_char("You feel energy pour into your body.\n\r",victim);
	if (ch->pcdata->stats[DEMON_CURRENT] == 1)
	    sprintf(buf,"You receive a single point of energy.\n\r");
	else
	    sprintf(buf,"You receive %d points of energy.\n\r",
		ch->pcdata->stats[DEMON_CURRENT]);
	send_to_char(buf,victim);
	act("$n is briefly surrounded by a halo of energy.",victim,NULL,NULL,TO_ROOM);
	victim->pcdata->stats[DEMON_CURRENT] += ch->pcdata->stats[DEMON_CURRENT];
	victim->pcdata->stats[DEMON_TOTAL] += ch->pcdata->stats[DEMON_CURRENT];
	ch->pcdata->stats[DEMON_CURRENT] = 0;
	return;
    }
    if (IS_SET(ch->deaf, CHANNEL_PRAY))
    {
	send_to_char("But you're not even on the channel!\n\r",ch);
	return;
    }

    talk_channel( ch, argument, CHANNEL_PRAY, "pray" );
    return;
}

void do_howl( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC( ch ) || (!IS_IMMORTAL(ch) && !IS_CLASS(ch,CLASS_WEREWOLF) 
	&& !IS_POLYAFF(ch, POLY_WOLF)))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    talk_channel( ch, argument, CHANNEL_HOWL, "howls" );
    return;
}


void do_say( CHAR_DATA *ch, char *argument )
{
    char name   [80];
    char poly   [MAX_STRING_LENGTH];
    char speak  [10];
    char speaks [10];
    char endbit [2];
    char secbit [2];
    CHAR_DATA *to;
    bool is_ok;

    if ( IS_SET(ch->in_room->room_flags, ROOM_SILENCE ) )
    {
     send_to_char("Something prevents you from speaking in this room.\n\r", ch);
     return;
    }

    if ( IS_HEAD(ch, LOST_TONGUE) )
    {
	send_to_char( "You can't speak without a tongue!\n\r", ch );
	return;
    }
    if ( IS_EXTRA(ch, GAGGED) )
    {
	send_to_char( "You can't speak with a gag on!\n\r", ch );
	return;
    }

if (ch->in_room != NULL && IS_SET(ch->in_room->added_flags,
ROOM2_SILENCE))
    {
	send_to_char( "Your words make no sound!\n\r", ch );
	return;
    }

    if (strlen(argument) > MAX_INPUT_LENGTH)
    {
	send_to_char( "Line too long.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }

    endbit[0] = argument[strlen(argument)-1];
    endbit[1] = '\0';

    if (strlen(argument) > 1) secbit[0] = argument[strlen(argument)-2];
	else secbit[0] = '\0';
    secbit[1] = '\0';

    if (IS_BODY(ch,CUT_THROAT))
    {
	sprintf(speak,"rasp");
	sprintf(speaks,"rasps");
    }
    else if (!IS_NPC(ch) && 
	(IS_SET(ch->special,SPC_WOLFMAN) || IS_POLYAFF(ch, POLY_WOLF) 
	|| (IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch)  &&
ch->pcdata->stats[UNI_RAGE] > 0)))
    {
	if (number_percent() > 50)
	{
	    sprintf(speak,"growl");
	    sprintf(speaks,"growls");
	}
	else
	{
	    sprintf(speak,"snarl");
	    sprintf(speaks,"snarls");
	}
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_BAT))
    {
	sprintf(speak,"squeak");
	sprintf(speaks,"squeaks");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_SERPENT))
    {
	sprintf(speak,"hiss");
	sprintf(speaks,"hisses");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_FROG))
    {
	sprintf(speak,"croak");
	sprintf(speaks,"croaks");
    }
    else if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_RAVEN))
    {
	sprintf(speak,"squark");
	sprintf(speaks,"squarks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_FROG)
    {
	sprintf(speak,"croak");
	sprintf(speaks,"croaks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_RAVEN)
    {
	sprintf(speak,"squark");
	sprintf(speaks,"squarks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_CAT)
    {
	sprintf(speak,"purr");
	sprintf(speaks,"purrs");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_DOG)
    {
	sprintf(speak,"bark");
	sprintf(speaks,"barks");
    }
    else if (!str_cmp(endbit,"!"))
    {
	sprintf(speak,"exclaim");
	sprintf(speaks,"exclaims");
    }
    else if (!str_cmp(endbit,"?"))
    {
	sprintf(speak,"ask");
	sprintf(speaks,"asks");
    }
    else if (secbit[0] != '\0' && str_cmp(secbit,".") && !str_cmp(endbit,"."))
    {
	sprintf(speak,"state");
	sprintf(speaks,"states");
    }
    else if (secbit[0] != '\0' && !str_cmp(secbit,".") && !str_cmp(endbit,"."))
    {
	sprintf(speak,"mutter");
	sprintf(speaks,"mutters");
    }
    else if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
    {
	sprintf(speak,"slur");
	sprintf(speaks,"slurs");
    }
    else
    {
	sprintf(speak,"say");
	sprintf(speaks,"says");
    }
    sprintf(poly,"#wYou %s '#g$T#w'.#n", speak);
    if (!IS_NPC(ch))
    {
        if (ch->pcdata->condition[COND_DRUNK] > 10)
            act( poly, ch, NULL, drunktalk(ch, argument), TO_CHAR );
        else if (ch->pcdata->stats[UNI_RAGE] > 0 && IS_VAMPIRE(ch) &&
            !IS_SET(world_affects, WORLD_NICE))
            act( poly, ch, NULL,  argument, TO_CHAR );
        else if (IS_SPEAKING(ch,DIA_OLDE))
            act( poly, ch, NULL, oldelang(ch, argument), TO_CHAR );
        else if (IS_SPEAKING(ch,DIA_BAD))
            act( poly, ch, NULL, badlang(ch, argument), TO_CHAR );
	else
	    act( poly, ch, NULL, argument, TO_CHAR );
    }
    else
	act( poly, ch, NULL, argument, TO_CHAR );

    sprintf(poly,"#w$n %s '#g$T#w'.#n", speaks);

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
	if (!IS_NPC(ch))
	{
/*
	    if (ch->pcdata->condition[COND_DRUNK] > 10)
		act( poly, ch, NULL, drunktalk(ch, argument), TO_ROOM );
	    else if (IS_SPEAKING(ch,DIA_OLDE))
		act( poly, ch, NULL, oldelang(ch, argument), TO_ROOM );
	    else if (IS_SPEAKING(ch,DIA_BAD))
		act( poly, ch, NULL, badlang(ch, argument), TO_ROOM );
	    else if (IS_SPEAKING(ch,LANG_DARK))
		act( poly, ch, NULL, darktongue(ch, argument), TO_ROOM );
	    else
*/
		act( poly, ch, NULL, argument, TO_ROOM );
	}
	else
	    act( poly, ch, NULL, argument, TO_ROOM );
    	room_text( ch, strlower(argument) );
	return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

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

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
	else if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
	    sprintf(name, ch->morph);
	else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s '%s'.\n\r", name,speaks,argument);
	send_to_char(poly,to);
    }

    room_text( ch, strlower(argument) );
    return;
}

void room_text( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA     *vch;
    CHAR_DATA     *vch_next;
    CHAR_DATA     *mob;
    OBJ_DATA      *obj;
    ROOMTEXT_DATA *rt;
    bool           mobfound;
    bool           hop;

    char           arg  [MAX_INPUT_LENGTH];
    char           arg1 [MAX_INPUT_LENGTH];
    char           arg2 [MAX_INPUT_LENGTH];

    for ( rt = ch->in_room->roomtext; rt != NULL; rt = rt->next )
    {
	if (!strcmp(argument,rt->input)
	 || is_in(argument, rt->input)
	 || all_in(argument, rt->input)) 
	{
	    if ( rt->name != NULL         && rt->name != '\0'
	    &&   str_cmp(rt->name,"all")  && str_cmp(rt->name,"|all*") ) 
	    	if (!is_in(ch->name, rt->name) ) continue;
	    mobfound = TRUE;
	    if (rt->mob != 0)
	    {
		mobfound = FALSE;
	    	for ( vch = char_list; vch != NULL; vch = vch_next )
	    	{
		    vch_next	= vch->next;
		    if ( vch->in_room == NULL ) continue;
		    if ( !IS_NPC(vch)         ) continue;
		    if ( vch->in_room == ch->in_room
			&& vch->pIndexData->vnum == rt->mob )
		    {
			mobfound = TRUE;
		    	break;
		    }
	    	}
	    }
	    if (!mobfound) continue;
	    hop = FALSE;
	    switch(rt->type % RT_RETURN)
	    {
		case RT_SAY:
		    break;
		case RT_LIGHTS:
		    do_changelight(ch,"");
		    break;
		case RT_LIGHT:
		    REMOVE_BIT(ch->in_room->room_flags, ROOM_DARK);
		    break;
		case RT_DARK:
		    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
		    break;
		case RT_OBJECT:
		    if ( get_obj_index(rt->power) == NULL ) return;
		    obj = create_object(get_obj_index(rt->power), ch->level);
		    if (IS_SET(rt->type, RT_TIMER) ) obj->timer = 1;
		    if (CAN_WEAR(obj,ITEM_TAKE)) obj_to_char(obj,ch);
		    else obj_to_room(obj,ch->in_room);
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, obj, NULL, TO_CHAR );
		    else
		    	act( rt->choutput, ch, obj, NULL, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, obj, NULL, TO_ROOM );
		    hop = TRUE;
		    break;
		case RT_MOBILE:
		    if ( get_mob_index(rt->power) == NULL ) return;
		    mob = create_mobile(get_mob_index(rt->power));
		    char_to_room(mob,ch->in_room);
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, NULL, mob, TO_CHAR );
		    else
		    	act( rt->choutput, ch, NULL, mob, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, NULL, mob, TO_ROOM );
		    hop = TRUE;
		    break;
		case RT_SPELL:
		    (*skill_table[rt->power].spell_fun) ( rt->power, number_range(20,30), ch, ch );
		    break;
		case RT_PORTAL:
		    if ( get_obj_index(OBJ_VNUM_PORTAL) == NULL ) return;
		    obj = create_object(get_obj_index(OBJ_VNUM_PORTAL), 0);
		    obj->timer = 1;
		    obj->value[0] = rt->power;
		    obj->value[1] = 1;
		    obj_to_room(obj,ch->in_room);
		    break;
		case RT_TELEPORT:
		    if ( get_room_index(rt->power) == NULL ) return;
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, NULL, NULL, TO_CHAR );
		    else
		    	act( rt->choutput, ch, NULL, NULL, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, NULL, NULL, TO_ROOM );
		    char_from_room(ch);
		    char_to_room(ch,get_room_index(rt->power));
		    act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
		    do_look(ch,"auto");
		    hop = TRUE;
		    break;
		case RT_ACTION:
		    sprintf(arg,argument);
		    argument = one_argument( arg, arg1 );
		    argument = one_argument( arg, arg2 );
		    if ( (mob = get_char_room(ch, arg2) ) == NULL ) continue;
		    interpret( mob, rt->output );
		    break;
		case RT_OPEN_LIFT:
		    open_lift(ch);
		    break;
		case RT_CLOSE_LIFT:
		    close_lift(ch);
		    break;
		case RT_MOVE_LIFT:
		    move_lift(ch,rt->power);
		    break;
		default:
		    break;
	    }
	    if (hop && IS_SET(rt->type, RT_RETURN) ) return;
	    else if (hop) continue;
	    if (!str_cmp(rt->choutput,"copy") && !IS_SET(rt->type, RT_ACTION))
	    	act( rt->output, ch, NULL, NULL, TO_CHAR );
	    else if (!IS_SET(rt->type, RT_ACTION))
	    	act( rt->choutput, ch, NULL, NULL, TO_CHAR );
	    if (!IS_SET(rt->type, RT_PERSONAL) && !IS_SET(rt->type, RT_ACTION) )
		act( rt->output, ch, NULL, NULL, TO_ROOM );
	    if (IS_SET(rt->type, RT_RETURN) ) return;
	}
    }
    return;
}

char *strlower(char *ip)
{
	static char buffer[MAX_INPUT_LENGTH];
	int pos;

	for (pos = 0; pos < (MAX_INPUT_LENGTH - 1) && ip[pos] != '\0'; pos++) {
		buffer[pos] = tolower(ip[pos]);
	}	
	buffer[pos] = '\0';
	return buffer;
}



bool is_in(char *arg, char *ip)
{
	char *lo_arg;
	char cmp[MAX_INPUT_LENGTH];
	int fitted;

	if (ip[0] != '|')
		return FALSE;
	cmp[0] = '\0';
	lo_arg = strlower(arg);
	do {
		ip += strlen(cmp) + 1;
		fitted = sscanf(ip, "%[^*]", cmp);
		if (strstr(lo_arg, cmp) != NULL) {
			return TRUE;
		}
	} while (fitted > 0);
	return FALSE;
}

bool all_in(char *arg, char *ip)
{
	char *lo_arg;
	char cmp[MAX_INPUT_LENGTH];
	int fitted;

	if (ip[0] != '&')
		return FALSE;
	cmp[0] = '\0';
	lo_arg = strlower(arg);
	do {
		ip += strlen(cmp) + 1;
		fitted = sscanf(ip, "%[^*]", cmp);
		if (strstr(lo_arg, cmp) == NULL) {
			return FALSE;
		}
	} while (fitted > 0);
	return TRUE;
}	

void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char poly [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */

/*Toast Changed 25 NOvember to try sort tell crash bug */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
//    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if(IS_NPC(victim) )
    {
	send_to_char("Thats a NPC, Why would you want too?\n\r",ch);
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( IS_SET(victim->deaf, CHANNEL_TELL) )
    {
	if (IS_NPC(victim) || IS_NPC(ch) || strlen(victim->pcdata->marriage) < 2
	    || str_cmp(ch->name, victim->pcdata->marriage))
	{
	    act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	    return;
	}
    }

	if (victim->desc->connected == CON_NOTE_TO      ||
          victim->desc->connected == CON_NOTE_SUBJECT ||
          victim->desc->connected == CON_NOTE_EXPIRE  ||
          victim->desc->connected == CON_NOTE_TEXT    ||
          victim->desc->connected == CON_NOTE_FINISH   )
	{
		act("$E is currently writing a note.", ch, 0, victim, TO_CHAR );
		return;
	}
	if (victim->desc->connected == CON_AEDITOR ||
          victim->desc->connected == CON_REDITOR ||
          victim->desc->connected == CON_MEDITOR ||
          victim->desc->connected == CON_OEDITOR  )
	{
	act("They are currently occupied and cannot hear you.", ch, 0, victim, TO_CHAR);
      return;
	}

	if (IS_SET(victim->flag2, EXTRA_AFK))
        {
		act( "$E is currently AFK.", ch, 0, victim, TO_CHAR);
		return;
	}

    act( "#wYou tell $N '#y$t#w'.#n", ch, argument, victim, TO_CHAR );

    position		= victim->position;
    victim->position	= POS_STANDING;

    sprintf(poly,"#w$n tells you '#y$t#w'.#n");
    ADD_COLOUR(ch,poly,WHITE);
    act( poly, ch, argument, victim, TO_VICT );

    victim->position	= position;
    victim->reply	= ch;

    return;
}



void do_whisper( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Not with a gag on!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Syntax: whisper <person> <message>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_AWAKE(victim) )
    {
	act( "$E cannot hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }

    act( "#wYou whisper to $N '#p$t#w'.#n", ch, argument, victim, TO_CHAR );
    act( "#w$n whispers to you '#p$t#w'.#n", ch, argument, victim, TO_VICT );
    act( "#w$n whispers something to $N.#n", ch, NULL, victim, TO_NOTVICT );

    return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    char poly [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if( IS_NPC(victim) )
    {
        send_to_char("You can't reply to NPC's.\n\r", ch );
        return;
    }

    if (ch->in_room != NULL && IS_SET(ch->in_room->added_flags, ROOM2_SILENCE))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }
/*
 * Changed by Toaster 01/02/00 invis Immortals can't be replied to when invis
 * Altered so Immortals can reply to invis Immortals normally
 *
 */
    if ((!IS_IMMORTAL(ch)) && (victim->pcdata->wizinvis > 6) )
    {
        send_to_char( "They aren't here.\n\r", ch );;
        return;
    }


    act( "#wYou reply to $N '#y$t#w'.#n", ch, argument, victim, TO_CHAR );
    position		= victim->position;
    victim->position	= POS_STANDING;

    sprintf(poly,"#w$n replies '#y$t#w'.#n");
    ADD_COLOUR(ch,poly,WHITE);
    act( poly, ch, argument, victim, TO_VICT );

    victim->position	= position;
    victim->reply	= ch;

    return;
}



void do_emote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *plast;

    char name   [80];
    char poly   [MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    bool is_ok;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE) )
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( IS_HEAD(ch, LOST_TONGUE) || IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Pose what?\n\r", ch );
	return;
    }

    for ( plast = argument; *plast != '\0'; plast++ )
	;

    strcpy( buf, argument );
    if ( isalpha(plast[-1]) )
	strcat( buf, "." );

    act( "$n $T", ch, NULL, buf, TO_CHAR );

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
    	act( "$n $T", ch, NULL, buf, TO_ROOM );
	return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

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

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
	else if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
	    sprintf(name, ch->morph);
	else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s\n\r", name,buf);
	send_to_char(poly,to);
    }
    return;
}



void do_xemote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char oldarg[MAX_STRING_LENGTH];
    char *plast;

    char name   [80];
    char you    [80];
    char them   [80];
    char poly   [MAX_INPUT_LENGTH];
    char arg    [MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *victim;
    bool is_ok;

    argument = one_argument( argument, arg );

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE) )
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( IS_HEAD(ch, LOST_TONGUE) || IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if (strlen(argument) > MAX_INPUT_LENGTH)
    {
	send_to_char( "Line too long.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' || arg[0] == '\0' )
    {
	send_to_char( "Syntax: emote <person> <sentence>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_NPC(ch)) strcpy(you, ch->short_descr);
	else strcpy(you, ch->name);
    if (IS_NPC(victim)) strcpy(you, victim->short_descr);
	else strcpy(you, victim->name);
/*
oldarg = argument;
*/
    strcpy( oldarg,argument );
    strcpy( buf, argument );
    for ( plast = argument; *plast != '\0'; plast++ )
	;

    if ( isalpha(plast[-1]) )
	strcat( buf, "." );
    argument = socialc(ch, buf, you, them);

    strcpy( buf, argument );
    strcpy( buf2, "You ");
    buf[0] = LOWER(buf[0]);
    strcat( buf2, buf );
    capitalize( buf2 );
    act( buf2, ch, NULL, victim, TO_CHAR );

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
	strcpy( buf, oldarg );
	for ( plast = argument; *plast != '\0'; plast++ )
	    ;
	if ( isalpha(plast[-1]) )
	    strcat( buf, "." );

	argument = socialn(ch, buf, you, them);

	strcpy( buf, argument );
	strcpy( buf2, "$n ");
	buf[0] = LOWER(buf[0]);
	strcat( buf2, buf );
	capitalize( buf2 );
    	act( buf2, ch, NULL, victim, TO_NOTVICT );

	strcpy( buf, oldarg );
	for ( plast = argument; *plast != '\0'; plast++ )
	    ;
	if ( isalpha(plast[-1]) )
	    strcat( buf, "." );

	argument = socialv(ch, buf, you, them);

	strcpy( buf, argument );
	strcpy( buf2, "$n ");
	buf[0] = LOWER(buf[0]);
	strcat( buf2, buf );
	capitalize( buf2 );
    	act( buf2, ch, NULL, victim, TO_VICT );
	return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

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

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
	else if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
	    sprintf(name, ch->morph);
	else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s\n\r", name,buf);
	send_to_char(poly,to);
    }
    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_idea( CHAR_DATA *ch, char *argument )
{
    append_file( ch, IDEA_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here.  Just save and quit.\n\r", ch );
    return;
}



void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}



void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    CHAR_DATA *mount;
    if ( IS_NPC(ch) )
	return;
   
/* war check shitty thing */
    if(ch->pcdata->war_bits != 0 )
    {
        send_to_char("Not while in a war.\n\r", ch );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->flag2, AFF2_INARENA) )
    {
      stc("You cannot quit while you're in the arena.\n\r",ch);
      return;
    }

    if ( ch->position  < POS_SLEEPING )
    {
	send_to_char( "You're not DEAD yet.\n\r", ch );
	return;
    }



if (ch->fight_timer >0) {
	send_to_char("Not until your fight timer expires.\n\r", ch );
	return;}

    d = ch->desc;
/*
if( d->connected == CON_REDITOR || d->connected == CON_AEDITOR ||
d->connected == CON_OEDITOR || d->connected == CON_MEDITOR)
{
stc("Please finish with olc before you quit.",ch);
return;
}
*/
    if ( !IS_NPC(ch) && IS_SET(ch->newbits, NEW_DARKNESS))
    {
	if (ch->in_room != NULL)
	{
	    REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS); 
	    REMOVE_BIT(ch->newbits, NEW_DARKNESS);
	    ch->darkness_timer = 60;
	    send_to_char("The darkness in the room gradually fades away to nothing.\n\r",ch);
	    act("The darkness in the room gradually fades away to nothing.",ch,NULL,NULL,TO_ROOM);
	}
    }


 if ( IS_EXTRA(ch, EXTRA_ASH) )
    {
	do_humanform(ch, "");
}
 if ( IS_EXTRA(ch, EXTRA_PLASMA) )
    {
        do_humanform(ch, "");
}
  if ( !IS_NPC(ch) && IS_SET(ch->newbits, NEW_SILENCE))
    {
	if (ch->in_room != NULL)
	{
	    REMOVE_BIT(ch->in_room->added_flags, ROOM2_SILENCE);
	    REMOVE_BIT(ch->newbits, NEW_SILENCE);
	    ch->silence_timer = 60;
	    send_to_char("The room is no longer silent.\n\r",ch);
	    act("The room is no longer silent.",ch,NULL,NULL,TO_ROOM);
	}
    }

 if ( !IS_NPC(ch) && IS_SET(ch->newbits, NEW_REINA))
    {
	if (ch->in_room != NULL)
	{
	    REMOVE_BIT(ch->in_room->room_flags, ROOM_SAFE); 
	    REMOVE_BIT(ch->newbits, NEW_REINA);
	    ch->reina_timer = 60;
	    send_to_char("The peaceful harmony in the air gradually fades away to nothing.\n\r",ch);
	    act("The peaceful harmony in the air gradually fades away to nothing.",ch,NULL,NULL,TO_ROOM);
	}
    }
    if ( !IS_NPC(ch) && IS_SET(ch->newbits, NEW_NEUTRAL))
    {
	if (ch->in_room != NULL)
	{
	    REMOVE_BIT(ch->in_room->room_flags, ROOM_SAFE); 
	    REMOVE_BIT(ch->newbits, NEW_NEUTRAL);
	    ch->obeah_timer = 30;
	    send_to_char("The peaceful harmony in the air gradually fades away to nothing.\n\r",ch);
	    act("The peaceful harmony in the air gradually fades away to nothing.",ch,NULL,NULL,TO_ROOM);
	}
    }
 if (IS_EXTRA(ch, EXTRA_POTENCY))
                    {
                        send_to_char("You feel your blood potency fade away.\n\r",ch);
                        ch->pcdata->stats[UNI_GEN]--;
                        REMOVE_BIT(ch->extra, EXTRA_POTENCY);
                    }
if (IS_SET(ch->newbits, NEW_TIDE))
                    {
                     REMOVE_BIT(ch->newbits, NEW_TIDE);
}

//if (IS_CLASS(ch, CLASS_MAGE)) ch->level = 6;
    if ( (mount = ch->mount) != NULL ) do_dismount(ch,"");
/*
    send_to_char( "\n\r                         Speak not: whisper not:\n\r",ch);
    send_to_char( "                      I know all that ye would tell,\n\r",ch);
    send_to_char( "                    But to speak might break the spell\n\r",ch);
    send_to_char( "                      Which must bend the invincible,\n\r",ch);
    send_to_char( "                          The stern of thought;\n\r",ch);
    send_to_char( "                 He yet defies the deepest power of Hell.\n\r\n\r",ch );
*/
    send_to_char( "\n\r           I'm a lean dog, a keen dog, a wild dog, and lone;\n\r",ch);
    send_to_char( "           I'm a rough dog, a tough dog, hunting on my own;\n\r",ch);
    send_to_char( "           I'm a bad dog, a mad dog, teasing silly sheep;\n\r",ch);
    send_to_char( "           I love to sit and bay the moon, to keep fat souls from sleep.\n\r\n\r",ch);
    /*
     * After extract_char the ch is no longer valid!
     */
 if (ch->pcdata->in_progress)
                free_note(ch->pcdata->in_progress);

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if (obj->wear_loc == WEAR_NONE) continue;
    	if ( !IS_NPC(ch) && ((obj->chobj != NULL && !IS_NPC(obj->chobj) &&
	    obj->chobj->pcdata->obj_vnum != 0) || obj->item_type == ITEM_KEY))
	    unequip_char(ch,obj);
    }
    save_char_obj( ch );
    if (ch->pcdata->obj_vnum != 0)
    	act( "$n slowly fades out of existance.", ch, NULL, NULL, TO_ROOM );
    else
    	act( "$n has left the game.", ch, NULL, NULL, TO_ROOM );

    if ( d != NULL )
	close_socket2( d );

    if (ch->in_room != NULL) char_from_room(ch);
    char_to_room(ch,get_room_index(30002));

    sprintf( log_buf, "%s has quit.", ch->name );
    log_string( log_buf );

    if (!IS_IMMORTAL(ch))
    {
	sprintf( buf, "{w%s {Dhas left the game.{x", ch->name );
	do_info(ch,buf);
    }


    if (ch->pcdata->chobj != NULL) extract_obj(ch->pcdata->chobj);
    extract_char( ch, TRUE, FALSE );
    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( ch->level < 2 )
    {
	send_to_char( "You must kill at least 5 mobs before you can save.\n\r", ch );
	return;
    }

    save_char_obj_backup( ch );
    save_char_obj( ch );
    send_to_char( "Saved.\n\r", ch );
    return;
}



void do_autosave( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if ( ch->level < 2 ) return;
    save_char_obj( ch );
    return;
}


void do_nofollow( CHAR_DATA *ch, char *argument)
{
    if (IS_MORE(ch, NOFOLLOW))
    {
         REMOVE_BIT(ch->more, NOFOLLOW);
         stc("You will now allow people to follow you.\n\r",ch);
         return;
    }
    else
    {
         SET_BIT(ch->more, NOFOLLOW);
         stc("People may not follow you anymore..\n\r",ch);
         return;
    }
}

void do_follow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_MORE(victim, NOFOLLOW)) 
    { stc("You may not follow them.\n\r",ch); return; }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower( ch );
	return;
    }
    
    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}



void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    if ( can_see( ch->master, ch ) )
	act( "$n stops following you.", ch, NULL, ch->master, TO_VICT );
    act( "You stop following $N.", ch, NULL, ch->master, TO_CHAR    );

    ch->master = NULL;
    ch->leader = NULL;
    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
	stop_follower( ch );

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}


/*
void do_order( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch
);
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if ( (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch) 
	&& !(IS_CLASS(ch, CLASS_VAMPIRE) && IS_CLASS(victim,
CLASS_VAMPIRE) ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	if ( IS_CLASS(ch, CLASS_VAMPIRE) && IS_CLASS(victim,
CLASS_VAMPIRE)
	&& ((ch->pcdata->stats[UNI_GEN] != 2) ||
str_cmp(ch->clan,victim->clan)))
	{
	    act( "$N ignores your order.", ch, NULL, victim, TO_CHAR );
	    act( "You ignore $n's order.", ch, NULL, victim, TO_VICT );
	    return;
	}

    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;
	if ( och == ch ) continue;
	if ((IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    act( "$n orders you to '$t'.", ch, argument, och, TO_VICT );
	    interpret( och, argument );
	}
    }

    if ( found ) 

	send_to_char( "Ok.\n\r", ch );
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}
*/
void do_order( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;
    bool is_animal = FALSE;
    bool is_spirit = FALSE;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch
);
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if
(is_in(argument,"|quit*rem*drop*gift*claim*consent*propose*accept*vampi*ma
*maj*reca*/*ha *hav*train*"))
	{
	    send_to_char( "You cannot make that sort of order.\n\r", ch );
	    return;
	}

	if (IS_NPC(victim)) switch ( victim->pIndexData->vnum )
	{
	    default: is_animal = FALSE; break;
	    case 3062:
	    case 3066:
	    case 5333:
	    case 30006:
	    case 30007:
	    case 30008:
	    case 30009:
	    case 30010:
	    case 30013:
	    case 30014: is_animal = TRUE; break;
	}
	else is_animal = FALSE;
	if (IS_SPIRIT(victim)) is_spirit = TRUE; else is_spirit = FALSE;
	if ( !IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch )
	{
	    if (IS_NPC(ch) || IS_NPC(victim) || 
		strlen(victim->pcdata->love) < 2 || 
		str_cmp(victim->pcdata->love, ch->name))
	    {
		if (!is_animal)
		{
		    if (!is_spirit || !IS_WEREWOLF(ch))
		    {
			send_to_char( "Do it yourself!\n\r", ch );
			return;
		    }
		}
	    }
	}
    }

    found = FALSE;
    is_animal = FALSE;
    //is_spirit = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;
	if ( och == ch ) continue;
	if (IS_NPC(och)) switch ( och->pIndexData->vnum )
	{
	    default: is_animal = FALSE; break;
	    case 3062:
	    case 3066:
	    case 5333:
	    case 30006:
	    case 30007:
	    case 30008:
	    case 30009:
	    case 30010:
	    case 30013:
	    case 30014: is_animal = TRUE; break;
	}
	else is_animal = FALSE;
//	if (IS_SPIRIT(och)) is_spirit = TRUE; else is_spirit = FALSE;
	if ((IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) ) )
	{
	    found = TRUE;
	    act( "$n orders you to '$t'.", ch, argument, och, TO_VICT );
	    if (IS_NPC(och)) SET_BIT(och->act, ACT_COMMANDED);
	    interpret( och, argument );
	    if (IS_NPC(och)) REMOVE_BIT(och->act, ACT_COMMANDED);
	    WAIT_STATE(ch,12);
	}
	else if (!IS_NPC(ch) && !IS_NPC(och) &&
strlen(och->pcdata->love) > 1 &&
	!str_cmp(och->pcdata->love,ch->name) && ( fAll || och == victim
) )
	{
	    found = TRUE;
	    act( "$n orders you to '$t'.", ch, argument, och, TO_VICT );
	    if (IS_NPC(och)) SET_BIT(och->act, ACT_COMMANDED);
	    interpret( och, argument );
	    if (IS_NPC(och)) REMOVE_BIT(och->act, ACT_COMMANDED);
	    WAIT_STATE(ch,12);
	}
	else if (is_animal && IS_MORE(ch, MORE_ANIMAL_MASTER) && ( fAll ||
och == victim ))
	{
	    found = TRUE;
	    act( "$n asks you to '$t'.", ch, argument, och, TO_VICT );
	    if (IS_NPC(och)) SET_BIT(och->act, ACT_COMMANDED);
	    interpret( och, argument );
	    if (IS_NPC(och)) REMOVE_BIT(och->act, ACT_COMMANDED);
	    WAIT_STATE(ch,12);
	}
	else if (is_spirit && IS_WEREWOLF(ch) && ( fAll || och == victim ))
	{
	    found = TRUE;
	    act( "$n asks you to '$t'.", ch, argument, och, TO_VICT );
	    if (IS_NPC(och)) SET_BIT(och->act, ACT_COMMANDED);
	    interpret( och, argument );
	    if (IS_NPC(och)) REMOVE_BIT(och->act, ACT_COMMANDED);
	    WAIT_STATE(ch,12);
	}
    }

    if ( found )
	send_to_char( "Ok.\n\r", ch );
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}
 
/*
void do_command( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (!IS_VAMPAFF(ch, VAM_DOMINATE))
    {
	send_to_char( "You are not trained in the dominate discipline.\n\r", ch );
	return;
    }
    if (ch->spl[RED_MAGIC] < 1)
    {
	send_to_char( "Your mind is too weak.\n\r", ch );
	return;
    }
    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Command whom to do what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you command yourself??\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && victim->level != 3 )
    {
	send_to_char( "You can only command other avatars.\n\r", ch );
	return;
    }

    if (IS_NPC(victim))
    	sprintf( buf, "I think %s wants to %s", victim->short_descr, argument );
    else if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH))
    	sprintf( buf, "I think %s wants to %s", victim->morph, argument );
    else
    	sprintf( buf, "I think %s wants to %s", victim->name, argument );
    do_say(ch,buf);

    if ( IS_NPC(victim) && victim->level >= (ch->spl[RED_MAGIC]/8) )
    {
    	act("You shake off $N's suggestion.",victim,NULL,ch,TO_CHAR);
    	act("$n shakes off $N's suggestion.",victim,NULL,ch,TO_NOTVICT);
    	act("$n shakes off your suggestion.",victim,NULL,ch,TO_VICT);
    	act("$s mind is too strong to overcome.",victim,NULL,ch,TO_VICT);
	return;
    }
    else if ( victim->spl[BLUE_MAGIC] >= (ch->spl[RED_MAGIC]/2) )
    {
    	act("You shake off $N's suggestion.",victim,NULL,ch,TO_CHAR);
    	act("$n shakes off $N's suggestion.",victim,NULL,ch,TO_NOTVICT);
    	act("$n shakes off your suggestion.",victim,NULL,ch,TO_VICT);
    	act("$s mind is too strong to overcome.",victim,NULL,ch,TO_VICT);
	return;
    }
    act("You blink in confusion.",victim,NULL,NULL,TO_CHAR);
    act("$n blinks in confusion.",victim,NULL,NULL,TO_ROOM);
    strcpy(buf,"Yes, you're right, I do...");
    do_say(victim,buf);
    interpret( victim, argument );
    return;
}
*/


void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		sprintf( buf,
		"[%-16s] %4ld/%4ld hp %4ld/%4ld mana %4ld/%4ld mv %5d xp\n\r",
		    capitalize( PERS(gch, ch) ),
		    gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp    );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }
/*Changed by Toaster 31/01/00 to stop Non-newbies grouping with newbies
    if ( (( get_age(victim) - 17) * 2 ) < 6 && ((get_age(ch) - 17) * 2) > 6 )
    {
	send_to_char( "You can't group with newbies unless you are also a newbie.\n\r", ch ); 
	return;
    }
Changed Toaster 13/03/00 to stop newbies grouping non newbies
    if ( (( get_age(ch) - 17) * 2 ) < 6 && ((get_age(victim) - 17) * 2) > 6 )
    {
        send_to_char( "Being a newbie means you can only group with another newbie.\n\r", ch );
        return;
    }
*/
    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
	act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
	act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );
	return;
    }

    victim->leader = ch;
    act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
    act( "You join $n's group.", ch, NULL, victim, TO_VICT    );
    act( "$N joins your group.", ch, NULL, victim, TO_CHAR    );
    return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount;
    int share;
    int extra;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }
    
    amount = atoi( arg );

    if ( amount < 0 )
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    if ( ch->gold < amount )
    {
	send_to_char( "You don't have that much gold.\n\r", ch );
	return;
    }
  
    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) )
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
    }
	    
    share = amount / members;
    extra = amount % members;

    if ( share == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

    ch->gold -= amount;
    ch->gold += share + extra;

    sprintf( buf,
	"You split %d gold coins.  Your share is %d gold coins.\n\r",
	amount, share + extra );
    send_to_char( buf, ch );

    sprintf( buf, "$n splits %d gold coins.  Your share is %d gold coins.",
	amount, share );

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group( gch, ch ) )
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->act, PLR_NO_TELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
    sprintf( buf, "{M%s {mtells the group {D'{w%s{D'.{x\n\r", ch->name, argument );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	    send_to_char( buf, gch );
    }

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

void do_changelight(CHAR_DATA *ch, char *argument )
{
    if (IS_SET(ch->in_room->room_flags, ROOM_DARK))
    {
	REMOVE_BIT(ch->in_room->room_flags, ROOM_DARK);
	act("The room is suddenly filled with light!",ch,NULL,NULL,TO_CHAR);
	act("The room is suddenly filled with light!",ch,NULL,NULL,TO_ROOM);
	return;
    }
    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
    act("The lights in the room suddenly go out!",ch,NULL,NULL,TO_CHAR);
    act("The lights in the room suddenly go out!",ch,NULL,NULL,TO_ROOM);
    return;
}

void open_lift( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (is_open(ch)) return;

    act("The doors open.",ch,NULL,NULL,TO_CHAR);
    act("The doors open.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_ROOM);
    if (!same_floor(ch,in_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_ROOM);
    if (is_open(ch)) act("The doors open.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    open_door(ch,TRUE);
    move_door(ch);
    open_door(ch,TRUE);
    thru_door(ch,in_room);
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void close_lift( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (!is_open(ch)) return;
    act("The doors close.",ch,NULL,NULL,TO_CHAR);
    act("The doors close.",ch,NULL,NULL,TO_ROOM);
    open_door(ch,FALSE);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void move_lift( CHAR_DATA *ch, int to_room )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_CHAR);
    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_ROOM);
    if (!same_floor(ch,to_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_CHAR);
    if (!same_floor(ch,to_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    open_door(ch,FALSE);
    thru_door(ch,to_room);
    return;
}

bool same_floor( CHAR_DATA *ch, int cmp_room )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[0] == cmp_room ) return TRUE;
	else return FALSE;
    }
    return FALSE;
}

bool is_open( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[2] == 0 ) return TRUE;
	else return FALSE;
    }
    return FALSE;
}

void move_door( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *pRoomIndex;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	pRoomIndex = get_room_index(obj->value[0]);
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	return;
    }
    return;
}

void thru_door( CHAR_DATA *ch, int doorexit )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	obj->value[0] = doorexit;
	return;
    }
    return;
}

void open_door( CHAR_DATA *ch, bool be_open )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[2] == 0 && !be_open ) obj->value[2] = 3;
	else if ( obj->value[2] == 3 && be_open ) obj->value[2] = 0;
	return;
    }
    return;
}

void do_speak( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (arg[0] == '\0')
    {
	if (IS_SPEAKING(ch,LANG_DARK)) 
	    send_to_char("You are speaking the Dark tongue.\n\r",ch);
	else if (IS_SPEAKING(ch,DIA_OLDE)) 
	    send_to_char("You are speaking Olde Worlde.\n\r",ch);
	else if (IS_SPEAKING(ch,DIA_BAD)) 
	    send_to_char("You are speaking very badly.\n\r",ch);
	else
	    send_to_char("You are speaking the common language.\n\r",ch);
	strcpy(buf,"You can speak the following languages:");
	strcat(buf," Common Olde Bad");
	if (CAN_SPEAK(ch, LANG_DARK)) strcat(buf," Dark");
	strcat(buf,".\n\r");
	send_to_char(buf,ch);
	return;
    }

    if (!str_cmp(arg,"dark"))
    {
	if (!CAN_SPEAK(ch,LANG_DARK))
	{
	    send_to_char("You cannot speak the Dark tongue.\n\r",ch);
	    return;
	}
	if (IS_SPEAKING(ch,LANG_DARK))
	{
	    send_to_char("But you are already speaking the Dark tongue!\n\r",ch);
	    return;
	}
	ch->pcdata->language[0] = LANG_DARK;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    else if (!str_cmp(arg,"common"))
    {
	if (ch->pcdata->language[0] == LANG_COMMON)
	{
	    send_to_char("But you are already speaking the common tongue!\n\r",ch);
	    return;
	}
	ch->pcdata->language[0] = LANG_COMMON;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    else if (!str_cmp(arg,"olde"))
    {
	if (ch->pcdata->language[0] == DIA_OLDE)
	{
	    send_to_char("But you are already speaking Olde Worlde!\n\r",ch);
	    return;
	}
	ch->pcdata->language[0] = DIA_OLDE;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    else if (!str_cmp(arg,"bad"))
    {
	if (ch->pcdata->language[0] == DIA_BAD)
	{
	    send_to_char("But you are already speaking badly!\n\r",ch);
	    return;
	}
	ch->pcdata->language[0] = DIA_BAD;
	send_to_char("Ok.\n\r",ch);
	return;
    }
/*
    else if (!str_cmp(arg,"common"))
    {
	if (ch->pcdata->language[0] < LANG_DARK)
	{
	    send_to_char("But you are already speaking the common tongue!\n\r",ch);
	    return;
	}
	ch->pcdata->language[0] = LANG_COMMON;
	if (CAN_SPEAK(ch,DIA_OLDE)) ch->pcdata->language[0] = DIA_OLDE;
	else if (CAN_SPEAK(ch,DIA_BAD)) ch->pcdata->language[0] = DIA_BAD;
	send_to_char("Ok.\n\r",ch);
	return;
    }
*/
    else
    {
	strcpy(buf,"You can speak the following languages:");
	strcat(buf," Common Olde Bad");
	if (CAN_SPEAK(ch, LANG_DARK)) strcat(buf," Dark");
	strcat(buf,".\n\r");
	send_to_char(buf,ch);
	return;
    }

    return;
}

char *badlang( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
        char *  old;
        char *  new;
    };
        
    static const struct spk_type spk_table[] =
    {
        { " ",          " "             },
        { "my name is", "i calls meself"},
        { "are not",    "aint"          },
        { "have",       "'av"           },
        { "my",         "me"            },
        { "hello",      "oy"            },
        { "hi ",        "oy "           },
        { "i am",       "im"            },
        { "it is",      "tis"           },
        { "the ",       "@"             },
        { " the",       " da"           },
        { "thank",      "fank"          },
        { "that",       "dat"           },
        { "with",       "wiv"           },
        { "they",       "day"           },
        { "this",       "dis"           },
        { "then",       "den"           },
       { "there",      "ver"           },
        { "their",      "ver"           },
        { "thing",      "fing"          },
        { "think",      "fink"          },
        { "was",        "woz"           },
        { "would",      "wud"           },
        { "what ",      "#"             },
        { "what",       "wot"           },
        { "where",      "weer"          },
        { "when",       "wen"           },
        { "are",        "is"            },
        { "you",        "ya"            },
        { "your ",      "&"             },
        { "your",       "yer"           },
        { "dead",       "ded"           },
        { "kill",       "stomp"         },
       { "food",       "nosh"          },
        { "blood",      "blud"          },
        { "vampire",    "sucker"        },
        { "kindred",    "suckers"       },
        { "fire",       "hot"           },
        { "dwarf",      "stunty"        },
        { "dwarves",    "stunties"      },
        { "goblin",     "gobbo"         },
        { "death",      "def"           },
        { "immune",     "mune"          },
        { "immunit",    "munit"         },
        { "childer",    "nippers"       },
        { "childe",     "nipper"        },
        { "child",      "nipper"        },
        { "tradition",  "wassname"      },
        { "generation", "batch"         },
       { "founded",    "made"          },
        { "sired",      "nipped"        },
        { "sire",       "dad"           },
        { "lineage",    "istory"        },
        { "road",       "path"          },
        { "recognize",  "dats"          },
        { "recognize",  "dats"          },
        { "decapitate", "headchop"      },
        { "decap",      "chop"          },
        { "recites",    "sez"           },
        { "recite",     "sez"           },
        { "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
        { "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
        { "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
        { "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
        { "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
       { "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
        { "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
        { ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
        { ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
        { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
        { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
        { "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };

    buf[0]      = '\0';
        
    if ( argument[0] == '\0' ) return argument;
        
    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
        for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
        { 
           if ( !str_prefix( spk_table[iSyl].old, pName ) )   
            {
                if (!str_cmp(spk_table[iSyl].new,",") ||
                    !str_cmp(spk_table[iSyl].new,"!") ||
                    !str_cmp(spk_table[iSyl].new,"?"))
                {
                    if (strlen(buf) < MAX_INPUT_LENGTH)
                    switch (number_range(1,20))
                    {  
                        default: break;
			case 1: break;
			case 2: break;
			case 3: break;
			case 4: break;
			case 5: break;
			case 6: break;

                    }
                }
                else if (!str_cmp(spk_table[iSyl].new,"@"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "da " );
                    else switch (number_range(1,10))
                    {  
                        default: strcat( buf, "da " ); break;
                        case 1: strcat( buf, "da bloody " ); break;
                        case 2: strcat( buf, "da fuckin' " ); break;
                        case 3: strcat( buf, "da goddamn " ); break;   
                        case 4: strcat( buf, "da flippin' " ); break; 
                        case 5: strcat( buf, "da stupid " ); break;
                        case 6: strcat( buf, "da stinkin' " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"#"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "wot " );
                    else switch (number_range(1,10))
                    {  
                        default: strcat( buf, "wot " ); break;
                        case 1: strcat( buf, "wot da fuck " ); break;
                        case 2: strcat( buf, "wot, i sez, " ); break;
                        case 3: strcat( buf, "wot da hell " ); break;  
                        case 4: strcat( buf, "wot da flip " ); break; 
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"&"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)   
                        strcat( buf, "yer " );
                    else switch (number_range(1,10))
                    {
                        default: strcat( buf, "yer " ); break;
                        case 1: strcat( buf, "yer bloody " ); break;
                        case 2: strcat( buf, "yer fuckin' " ); break;
                        case 3: strcat( buf, "yer damn " ); break;   
                        case 4: strcat( buf, "yer flippin' " ); break;
                        case 5: strcat( buf, "yer stupid " ); break;   
                        case 6: strcat( buf, "yer stinkin' " ); break;
                    }
                    break;
                }
                strcat( buf, spk_table[iSyl].new );
                break;
            }
        }
                    
        if ( length == 0 )
            length = 1;
    }
                        
    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);
    return argument;
}
char *oldelang( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "have",	"hath"		},
	{ "hello",	"hail"		},
	{ "hi ",	"hail "		},
	{ " hi",	" hail"		},
	{ "are",	"art"		},
	{ "your",	"thy"		},
	{ "you",	"thou"		},
	{ "i think",	"methinks"	},
	{ "do ",	"doth "		},
	{ " do",	" doth"		},
	{ "it was",	"twas"		},
	{ "before",	"ere"		},
	{ "his",	"$s"		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *darktongue( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "a", "i" }, { "b", "t" }, { "c", "x" }, { "d", "j" },
	{ "e", "u" }, { "f", "d" }, { "g", "k" }, { "h", "z" },
	{ "i", "o" }, { "j", "s" }, { "k", "f" }, { "l", "h" },
	{ "m", "b" }, { "n", "c" }, { "o", "e" }, { "p", "r" },
	{ "q", "l" }, { "r", "v" }, { "s", "w" }, { "t", "q" },
	{ "u", "a" }, { "v", "n" }, { "w", "y" }, { "x", "g" },
	{ "y", "m" }, { "z", "p" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "", "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}
char *beasttalk( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;
            
    struct spk_type
    {   
        char *  old;
        char *  new;
    };
    
    static const struct spk_type spk_table[] =   
    {
        { " ",          " "             },
        { "what ",      "#"             },
        { "your ",      "&"             },
        { "the ",       "@"             },
        { "my ",        "$"             },
        { "'", "'" },
        { "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
        { "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
        { "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
        { "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
        { "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
        { "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
        { "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
        { ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
        { ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
        { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
        { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
        { "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]      = '\0';
        
    if ( argument[0] == '\0' ) return argument;
        
    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
        for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
        { 
            if ( !str_prefix( spk_table[iSyl].old, pName ) )   
            {
                if (!str_cmp(spk_table[iSyl].new,",") ||
                    !str_cmp(spk_table[iSyl].new,"!") ||
                    !str_cmp(spk_table[iSyl].new,"?"))
                {
                    if (strlen(buf) < MAX_INPUT_LENGTH)
                    switch (number_range(1,20))
                    {  
                        default: break;
                        case 1: strcat( buf, ", you stupid fucker" ); break;
                        case 2: strcat( buf, ", you idiot" ); break;
                        case 3: strcat( buf, ", you damn fool" ); break;
                        case 4: strcat( buf, ", you little shit" ); break;
                        case 5: strcat( buf, ", you fucking jerk" ); break;  
                        case 6: strcat( buf, ", asshole" ); break;
                        case 7: strcat( buf, ", cock sucker" ); break;
                        case 8: strcat( buf, ", fuckwit" ); break;
                    }
                }
                else if (!str_cmp(spk_table[iSyl].new,"@"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "the " ); 
                    else switch (number_range(1,10))
                    {
                        default: strcat( buf, "the " ); break;
                        case 1: strcat( buf, "the bloody " ); break;
                        case 2: strcat( buf, "the fucking " ); break;   
                        case 3: strcat( buf, "the goddamn " ); break;
                        case 4: strcat( buf, "the stupid " ); break;
                        case 5: strcat( buf, "the stinking " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"#"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                       strcat( buf, "what " );
                    else switch (number_range(1,5))
                    {
                        default: strcat( buf, "what " ); break;
                        case 1: strcat( buf, "what the fuck " ); break;
                        case 2: strcat( buf, "what the hell " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"&"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "your " );
                    else switch (number_range(1,10))
                    {
                        default: strcat( buf, "your " ); break;
                       case 1: strcat( buf, "your bloody " ); break;
                        case 2: strcat( buf, "your fucking " ); break;
                        case 3: strcat( buf, "your damn " ); break;
                        case 4: strcat( buf, "your goddamn " ); break;
                        case 5: strcat( buf, "your stupid " ); break;  
                        case 6: strcat( buf, "your stinking " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"$"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "my " );  
                    else switch (number_range(1,10))
                    {
                        default: strcat( buf, "my " ); break;  
                        case 1: strcat( buf, "my bloody " ); break;  
                        case 2: strcat( buf, "my fucking " ); break;  
                        case 3: strcat( buf, "my damn " ); break;  
                        case 4: strcat( buf, "my goddamn " ); break;  
                        case 5: strcat( buf, "my stupid " ); break;    
                        case 6: strcat( buf, "my stinking " ); break;  
                    }
                    break;
                }
                strcat( buf, spk_table[iSyl].new );
                if (!str_cmp(spk_table[iSyl].new," ") && strlen(buf) < MAX_INPUT_LENGTH)
                {
                    switch (number_range(1,20))
                    {
                        default: break;
                        case 1: strcat( buf, "*snarl* " ); break;
                       case 2: strcat( buf, "*growl* " ); break;    
                    }
                }
                break;
            }
        }
                     
        if ( length == 0 )
            length = 1;
    }
              
    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);
                        
    return argument;
}
                     
char *beastchat( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;        
    int length;
            
    struct spk_type
    {           
        char *  old;   
        char *  new;
    };
                        
    static const struct spk_type spk_table[] =
   {
        { " ",          " "             },
        { "what ",      "#"             },
        { "your ",      "&"             },
        { "the ",       "@"             },
        { "my ",        "$"             },
        { "'", "'" },
        { "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
        { "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
        { "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
        { "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
        { "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
        { "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
        { "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
        { ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
        { ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
        { "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
        { "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
        { "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]      = '\0';
        
    if ( argument[0] == '\0' ) return argument;
      
    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
        for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
        { 
            if ( !str_prefix( spk_table[iSyl].old, pName ) )   
            {
                if (!str_cmp(spk_table[iSyl].new,",") ||
                    !str_cmp(spk_table[iSyl].new,"!") ||
                    !str_cmp(spk_table[iSyl].new,"?"))
                {
                    if (strlen(buf) < MAX_INPUT_LENGTH)

                    switch (number_range(1,20))
                    {  
                        default: break;
                        case 1: strcat( buf, ", you stupid fuckers" ); break;
                        case 2: strcat( buf, ", you idiots" ); break;
                        case 3: strcat( buf, ", you sad spods" ); break;
                        case 4: strcat( buf, ", you bunch of shits" ); break;
                        case 5: strcat( buf, ", you fucking jerks" ); break; 
                        case 6: strcat( buf, ", assholes" ); break;
                        case 7: strcat( buf, ", cock suckers" ); break;
                        case 8: strcat( buf, ", fuckwits" ); break;
                    }

                }
               else if (!str_cmp(spk_table[iSyl].new,"@"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "the " ); 
                    else switch (number_range(1,10))
                    {
                        default: strcat( buf, "the " ); break;
                        case 1: strcat( buf, "the bloody " ); break; 
                        case 2: strcat( buf, "the fucking " ); break;   
                        case 3: strcat( buf, "the goddamn " ); break;
                        case 4: strcat( buf, "the stupid " ); break;
                        case 5: strcat( buf, "the stinking " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"#"))
               {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "what " );
                    else switch (number_range(1,5))
                    {
                        default: strcat( buf, "what " ); break;
                        case 1: strcat( buf, "what the fuck " ); break;
                        case 2: strcat( buf, "what the hell " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"&"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "your " );
                    else switch (number_range(1,10))
                   {
                        default: strcat( buf, "your " ); break;
                        case 1: strcat( buf, "your bloody " ); break;
                        case 2: strcat( buf, "your fucking " ); break;
                        case 3: strcat( buf, "your damn " ); break;
                        case 4: strcat( buf, "your goddamn " ); break;
                        case 5: strcat( buf, "your stupid " ); break;  
                        case 6: strcat( buf, "your stinking " ); break;
                    }
                    break;
                }
                else if (!str_cmp(spk_table[iSyl].new,"$"))
                {
                    if (strlen(buf) >= MAX_INPUT_LENGTH)
                        strcat( buf, "my " );  
                    else switch (number_range(1,10))
                   {
                        default: strcat( buf, "my " ); break;  
                        case 1: strcat( buf, "my bloody " ); break;  
                        case 2: strcat( buf, "my fucking " ); break;  
                        case 3: strcat( buf, "my damn " ); break;  
                        case 4: strcat( buf, "my goddamn " ); break;  
                        case 5: strcat( buf, "my stupid " ); break;    
                        case 6: strcat( buf, "my stinking " ); break;  
                    }
                    break;
                }
                strcat( buf, spk_table[iSyl].new );


                if (!str_cmp(spk_table[iSyl].new," ") && strlen(buf) < MAX_INPUT_LENGTH)
                {
                    switch (number_range(1,20))
                    {
                        default: break;
                        case 1: strcat( buf, "*snarl* " ); break;    
                        case 2: strcat( buf, "*growl* " ); break;     
                    }
                }
                break;

            }
        }
                 
        if ( length == 0 )
            length = 1;
    }
                 
    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);  
                        
    return argument;
}
     

char *drunktalk( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;
    int loop;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "is",		"ish"		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		if (number_range(1,5) == 1 && str_cmp(spk_table[iSyl].new," "))
		    strcat( buf, spk_table[iSyl].new );
		else if (!str_cmp(spk_table[iSyl].new," "))
		{
		    if (number_range(1,5) == 1 && strlen(buf) < MAX_INPUT_LENGTH)
			strcat( buf, "*hic* " );
		}
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);
    for (loop = 1; loop < strlen(argument); loop++ )
    {
	if (number_range(1,2) == 1)
	    argument[loop] = UPPER(argument[loop]);
    }

    return argument;
}

char *socialc( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "you are",	"$E is"		},
	{ "you.",	"$M."		},
	{ "you,",	"$M,"		},
	{ "you ",	"$M "		},
	{ " you",	" $M"		},
	{ "your ",	"$S "		},
	{ " your",	" $S"		},
	{ "yours.",	"theirs."	},
	{ "yours,",	"theirs,"	},
	{ "yours ",	"theirs "	},
	{ " yours",	" theirs"	},
	{ "begins",	"begin"		},
	{ "caresses",	"caress"	},
	{ "gives",	"give"		},
	{ "glares",	"glare"		},
	{ "grins",	"grin"		},
	{ "licks",	"lick"		},
	{ "looks",	"look"		},
	{ "loves",	"love"		},
	{ "plunges",	"plunge"	},
	{ "presses",	"press"		},
	{ "pulls",	"pull"		},
	{ "runs",	"run"		},
	{ "slaps",	"slap"		},
	{ "slides",	"slide"		},
	{ "smashes",	"smash"		},
	{ "squeezes",	"squeeze"	},
	{ "stares",	"stare"		},
	{ "sticks",	"stick"		},
	{ "strokes",	"stroke"	},
	{ "tugs",	"tug"		},
	{ "thinks",	"think"		},
	{ "thrusts",	"thrust"	},
	{ "whistles",	"whistle"	},
	{ "wraps",	"wrap"		},
	{ "winks",	"wink"		},
	{ "wishes",	"wish"		},
	{ " winks",	" wink"		},
	{ " his",	" your"		},
	{ "his ",	"your "		},
	{ " her",	" your"		},
	{ "her ",	"your "		},
	{ " him",	" your"		},
	{ "him ",	"your "		},
	{ "the",	"the"		},
	{ " he",	" you"		},
	{ "he ",	"you "		},
	{ " she",	" you"		},
	{ "she ",	"you "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *socialv( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ " his",	" $s"		},
	{ "his ",	"$s "		},
	{ " her",	" $s"		},
	{ "her ",	"$s "		},
	{ " him",	" $m"		},
	{ "him ",	"$m "		},
	{ " he",	" $e"		},
	{ "he ",	"$e "		},
	{ " she",	" $e"		},
	{ "she ",	"$e "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *socialn( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "you are",	"$N is"		},
	{ "you.",	"$N."		},
	{ "you,",	"$N,"		},
	{ "you ",	"$N "		},
	{ " you",	" $N"		},
	{ "your.",	"$N's."		},
	{ "your,",	"$N's,"		},
	{ "your ",	"$N's "		},
	{ " your",	" $N's"		},
	{ "yourself",	"$Mself"	},
	{ " his",	" $s"		},
	{ "his ",	"$s "		},
	{ " her",	" $s"		},
	{ "her ",	"$s "		},
	{ " him",	" $m"		},
	{ "him ",	"$m "		},
	{ " he",	" $e"		},
	{ "he ",	"$e "		},
	{ " she",	" $e"		},
	{ "she ",	"$e "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}


void do_ignore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int pos;
    bool found = FALSE;

    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;

    if (IS_NPC(rch))
	return;

    smash_tilde( argument );

    argument = one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {

	if (rch->pcdata->forget[0] == NULL)
	{
	    send_to_char("You are not ignoring anyone.\n\r",ch);
	    return;
	}
	send_to_char("You are currently ignoring:\n\r",ch);

	for (pos = 0; pos < MAX_FORGET; pos++)
	{
	    if (rch->pcdata->forget[pos] == NULL)
		break;

	    sprintf(buf,"    %s\n\r",rch->pcdata->forget[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;

	if (!str_cmp(arg,rch->pcdata->forget[pos]))
	{
	    send_to_char("You have already ignored that person.\n\r",ch);
	    return;
	}
    }

    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;
	
	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if (!str_cmp(arg,wch->name))
	{
	    found = TRUE;
	    if (wch == ch)
	    {
		send_to_char("You can't ignore yourself!\n\r", ch );
		return;
	    }
	    if (wch->level >= LEVEL_IMMORTAL)
	    {
		send_to_char("That person is very hard to ignore.\n\r",ch);
		return;
	    }
	}
    }

    if (!found)
    {
	send_to_char("No one by that name is playing.\n\r",ch);
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;
     }

     if (pos >= MAX_FORGET)
     {
	send_to_char("Sorry, you have reached the ignore limit.\n\r",ch);
	return;
     }
  
     rch->pcdata->forget[pos]		= str_dup(arg);
     sprintf(buf,"You are now ignoring %s.\n\r",arg);
     send_to_char(buf,ch);
}

void do_unignore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    int pos;
    bool found = FALSE;
 
    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;
 
    if (IS_NPC(rch))
	return;
 
    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	if (rch->pcdata->forget[0] == NULL)
	{
	    send_to_char("You are not ignoring anyone.\n\r",ch);
	    return;
	}
	send_to_char("You are currently ignoring:\n\r",ch);

	for (pos = 0; pos < MAX_FORGET; pos++)
	{
	    if (rch->pcdata->forget[pos] == NULL)
		break;

	    sprintf(buf,"    %s\n\r",rch->pcdata->forget[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;

	if (found)
	{
	    rch->pcdata->forget[pos-1]		=
rch->pcdata->forget[pos];
	    rch->pcdata->forget[pos]		= NULL;
	    continue;
	}

	if(!strcmp(arg,rch->pcdata->forget[pos]))
	{
	    send_to_char("Ignore removed.\n\r",ch);
	    free_string(rch->pcdata->forget[pos]);
	    rch->pcdata->forget[pos] = NULL;
	    found = TRUE;
	}
    }

    if (!found)
	send_to_char("No one by that name is ignored.\n\r",ch);
}


void do_gsocial(CHAR_DATA *ch, char *argument)
{
    char command[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    int counter;
    int count;
    char buf2[MAX_STRING_LENGTH];

    argument = one_argument(argument,command);

    if (command[0] == '\0')
    {
        send_to_char("Try gsocial <social> if its not wroking write a note\n\r",ch);
    }

    found = FALSE;
    for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++)
    {
        if (command[0] == social_table[cmd].name[0]
        && !str_prefix( command,social_table[cmd].name ) )
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
    {
        send_to_char("Thats not a social try another\n\r",ch);
        return;
    }

/*
    if ( !IS_NPC(ch) && IS_SET(ch->deaf, CHANNEL_SOCIAL))
    {
        send_to_char("Try turning your social on\n\r",ch);
        return;
    }*/

    switch (ch->position)
    {
    case POS_DEAD:
        send_to_char("Stand up\n\r",ch);
        return;
    case POS_INCAP:
    case POS_MORTAL:
        send_to_char("You are hurt far too bad for that.\n\r",ch);
        return;
    case POS_STUNNED:
        send_to_char("You are too stunned for that.\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    victim = NULL;
    if (arg[0] == '\0')
    {
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n", social_table[cmd].char_no_arg);
        act(buf,ch,NULL,NULL,TO_CHAR);
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n",social_table[cmd].others_no_arg );
        for (d = descriptor_list; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                !IS_SET(vch->deaf,CHANNEL_SOCIAL))
            {
                act(buf,ch,NULL,vch,TO_VICT);
            }
        }
    }
    else if ((victim = get_char_world(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    else if (victim == ch)
    {
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n", social_table[cmd].char_auto);
        act(buf,ch,NULL,NULL,TO_CHAR);
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n",social_table[cmd].others_auto);
        for (d = descriptor_list; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                !IS_SET(vch->deaf,CHANNEL_SOCIAL))
            {
                act(buf,ch,NULL,vch,TO_VICT);
            }
        }
    }
    else
    {
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n",social_table[cmd].char_found);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n",social_table[cmd].vict_found);
        act(buf,ch,NULL,victim,TO_VICT);
        
        sprintf(buf, "#r<[#eSocial#r]>#G %s#n",social_table[cmd].others_found);
        for (counter = 0; buf[counter+1] != '\0'; counter++)
        {
            if (buf[counter] == '$' && buf[counter + 1] == 'N')
            {
                strcpy(buf2,buf);
                buf2[counter] = '\0';
                strcat(buf2,victim->name);
                for (count = 0; buf[count] != '\0'; count++)
                {
                    buf[count] = buf[count+counter+2];
                }
                strcat(buf2,buf);
                strcpy(buf,buf2);

            }
            else if (buf[counter] == '$' && buf[counter + 1] == 'E')
            {
                switch (victim->sex)
                {
                default:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count ++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 1:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 2:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                }
            }
            else if (buf[counter] == '$' && buf[counter + 1] == 'M')
            {
                buf[counter] = '%';
                buf[counter + 1] = 's';
                switch (victim->sex)
                {
                default:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 1:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"him");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 2:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"her");
                    for (count = 0; buf[count] != '\0'; count++);
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                }
            }
            else if (buf[counter] == '$' && buf[counter + 1] == 'S')
            {
                switch (victim->sex)
                {
                default:
                strcpy(buf2,buf);
                buf2[counter] = '\0';
                strcat(buf2,"its");
                for (count = 0;buf[count] != '\0'; count++)
                {
                    buf[count] = buf[count+counter+2];
                }
                strcat(buf2,buf);
                strcpy(buf,buf2);
                break;
                case 1:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"his");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 2:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"hers");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                }
            }

        }
        for (d=descriptor_list; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                d->character != victim &&
                !IS_SET(vch->deaf, CHANNEL_SOCIAL))
            {
                act(buf,ch,NULL,vch,TO_VICT);
            }
        }
    }
    return;
}


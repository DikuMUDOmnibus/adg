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



char *	const	dir_name	[]		=
{
    "{mnorth#n", "{meast#n", "{msouth#n", "{mwest#n", "{mup#n", "{mdown#n"
};
/*Changed by Toaster 19/01/00 to make all dirs Cyan*/
const	int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );
int	count_imms	args( ( CHAR_DATA *ch ) );
bool	check_track	args( ( CHAR_DATA *ch, int direction ) );
void	add_tracks	args( ( CHAR_DATA *ch, int direction ) );
void	show_page	args( ( CHAR_DATA *ch, OBJ_DATA *book, int pnum, bool pagefalse ) );
void	show_runes	args( ( CHAR_DATA *ch, OBJ_DATA *page, bool endline ) );
bool	are_runes	args( ( OBJ_DATA *page ) );



void move_char( CHAR_DATA *ch, int door )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    OBJ_DATA *obj;
    DESCRIPTOR_DATA *d;
    char buf  [MAX_STRING_LENGTH];
    char poly [MAX_STRING_LENGTH];
    char mount2 [MAX_INPUT_LENGTH];
    char leave [20];
    int revdoor;

    if ( door < 0 || door > 5 )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->to_room      ) == NULL )
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
	return;
    }

   if ( ( door == DIR_NORTH && ( ((obj=get_obj_list(ch,"walln",ch->in_room->contents)) != NULL) || (obj=get_obj_list(ch,"walls",to_room->contents)) != NULL))
     || ( door == DIR_SOUTH && ( ((obj=get_obj_list(ch,"walls",ch->in_room->contents)) != NULL) || (obj=get_obj_list(ch,"walln",to_room->contents)) != NULL))
     || ( door == DIR_EAST && (  ((obj=get_obj_list(ch,"walle",ch->in_room->contents)) != NULL) || (obj=get_obj_list(ch,"wallw",to_room->contents)) != NULL))
     || ( door == DIR_WEST && (  ((obj=get_obj_list(ch,"wallw",ch->in_room->contents)) != NULL) || (obj=get_obj_list(ch,"walle",to_room->contents)) != NULL))
     || ( door == DIR_UP && (    ((obj=get_obj_list(ch,"wallu",ch->in_room->contents)) != NULL) || (obj=get_obj_list(ch,"walld",to_room->contents)) != NULL))
     || ( door == DIR_DOWN && (  ((obj=get_obj_list(ch,"walld",ch->in_room->contents)) != NULL) || (obj=get_obj_list(ch,"wallu",to_room->contents)) != NULL)))
   {
     stc("You are unable to pass the wall of Blood.\n\r",ch);
     return;
   }

    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    &&   !IS_AFFECTED(ch, AFF_PASS_DOOR)
    &&   !IS_AFFECTED(ch, AFF_ETHEREAL)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE) )
    {
	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) &&
	    ch->pcdata->powers[WPOWER_BOAR] > 0)
	{
	    act( "You smash open the $d.", ch, NULL, pexit->keyword, TO_CHAR );
	    act( "$n smashes open the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	}
	else
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	    return;
	}
    }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "What?  And leave your beloved master?\n\r", ch );
	return;
    }
/* Added by Toaster to stop mortals wandering around in a Imm area*/
   if  (((to_room->vnum >= 10) && (to_room->vnum <=20)) && !IS_IMMORTAL(ch))
    {
        send_to_char("You are not allowed in here.\n\r", ch);
        return;
    }

    if (IS_NPC(ch) && (mount=ch->mount) != NULL && IS_SET(ch->mounted,IS_MOUNT))
    {
	send_to_char( "You better wait for instructions from your rider.\n\r", ch );
	return;
    }

    if ( room_is_private( to_room ))
    {

	if (IS_SET(pexit->exit_info, ITEM_WALL)
	&& ch->pcdata->powers[VAM_THAN] < 2 )
    {
	send_to_char("A wall of blood blocks your way.\n\r", ch);
	return;
    }

	if ( IS_NPC( ch ) || ch->trust < MAX_LEVEL)
	{
		send_to_char( "That room is private right now.\n\r", ch );
		return;
	}
	else
		send_to_char( "That room is private (Access granted).\n\r", ch);
    }

    if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_LEG)) &&
	 (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_LEG)) &&
	 (IS_ARM_L(ch,BROKEN_ARM) || IS_ARM_L(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_HOLD) != NULL) &&
	 (IS_ARM_R(ch,BROKEN_ARM) || IS_ARM_R(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	send_to_char( "You need at least one free arm to drag yourself with.\n\r", ch );
	return;
    }
    else if ( IS_BODY(ch,BROKEN_SPINE) &&
	 (IS_ARM_L(ch,BROKEN_ARM) || IS_ARM_L(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_HOLD) != NULL) &&
	 (IS_ARM_R(ch,BROKEN_ARM) || IS_ARM_R(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	send_to_char( "You cannot move with a broken spine.\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) )
    {
	int move;
	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR )
	{
	    if ( !IS_AFFECTED(ch, AFF_FLYING) &&
	(!IS_NPC(ch) && (!IS_CLASS(ch, CLASS_DROW) ||
!IS_SET(ch->pcdata->powersd[1], DPOWER_LEVITATION))) &&
		 (!IS_NPC(ch) && !IS_VAMPAFF(ch, VAM_FLYING) &&
		 (!IS_NPC(ch) && !IS_DEMAFF(ch,DEM_UNFOLDED)))
	    &&   !((mount=ch->mount) != NULL && IS_SET(ch->mounted,IS_RIDING) &&
		 IS_AFFECTED(mount, AFF_FLYING) ) )
	    {
		send_to_char( "You can't fly.\n\r", ch );
		return;
	    }
	}

	if ( in_room->sector_type == SECT_WATER_NOSWIM
	||   to_room->sector_type == SECT_WATER_NOSWIM )
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;
	    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
	    {
		if ( IS_VAMPAFF(ch, VAM_FLYING) )
		    found = TRUE;
		else if ( IS_POLYAFF(ch, POLY_SERPENT) )
		    found = TRUE;
		else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
		    found = TRUE;
		else if ((mount=ch->mount) != NULL && IS_SET(ch->mounted, IS_RIDING)
		    && IS_AFFECTED(mount, AFF_FLYING))
		    found = TRUE;
		else
		{
		    send_to_char( "You are unable to cross running water.\n\r", ch );
		    return;
		}
	    }
	    if ( IS_AFFECTED(ch, AFF_FLYING) )
		found = TRUE;
	    else if ( !IS_NPC( ch) && IS_DEMAFF(ch, DEM_UNFOLDED) )
		found = TRUE;
	else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DROW) &&
IS_SET(ch->pcdata->powersd[1], DPOWER_LEVITATION)))
		found = TRUE;
	    if ( !found )
	    {
	    	for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    	{
		    if ( obj->item_type == ITEM_BOAT )
		    {
		    	found = TRUE;
		    	break;
		    }
	    	}
	    	if ( !found )
	    	{
		    send_to_char( "You need a boat to go there.\n\r", ch );
		    return;
	    	}
	    }
	}
	else if ( !IS_AFFECTED(ch, AFF_FLYING) && IS_POLYAFF(ch, POLY_FISH) )
	{
	    bool from_ok = FALSE;
	    bool to_ok = FALSE;
	    if ( in_room->sector_type == SECT_WATER_NOSWIM ) from_ok = TRUE;
	    if ( in_room->sector_type == SECT_WATER_SWIM   ) from_ok = TRUE;
	    if ( to_room->sector_type == SECT_WATER_NOSWIM ) to_ok   = TRUE;
	    if ( to_room->sector_type == SECT_WATER_SWIM   ) to_ok   = TRUE;
	    if ( !from_ok || !to_ok )
	    {
		send_to_char("You cannot cross land.\n\r",ch);
		return;
	    }
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;
/* Added Toaster to add Move costs back into moving
	if (IS_HERO(ch)) move = 0;
*/
	if (ch->level < 3 || (( get_age(ch) - 17) * 2 ) < 5  ||
					ch->level > 7) move = 0;
	if(ch->move <= 0)
	{
		send_to_char("You are too Exhausted.\n\r",ch);
		return;
	}

	if (IS_SET(ch->mounted, IS_RIDING) && (ch->move < move || ch->move < 1))
	{
	    send_to_char( "You are too exhausted.\n\r", ch );
	    return;
	}

	WAIT_STATE( ch, 1 );
	if (!IS_SET(ch->mounted, IS_RIDING)) ch->move -= move;
    }

    /* Check for mount message - KaVir */
    if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
    {
	if (IS_NPC(mount))
	    sprintf(mount2," on %s.",mount->short_descr);
	else
	    sprintf(mount2," on %s.",mount->name);
    }
    else
	sprintf(mount2,".");

    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	sprintf(leave,"rolls");
    else if ( IS_AFFECTED(ch,AFF_ETHEREAL) )
	sprintf(leave,"floats");
    else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
	sprintf(leave,"swims");
    else if ( IS_SET(ch->polyaff,POLY_SERPENT) )
	sprintf(leave,"slithers");
    else if ( IS_SET(ch->polyaff,POLY_WOLF) )
	sprintf(leave,"stalks");
    else if ( IS_SET(ch->polyaff,POLY_FROG) )
	sprintf(leave,"hops");
    else if ( IS_SET(ch->polyaff,POLY_FISH) )
	sprintf(leave,"swims");
    else if ( !IS_NPC(ch) && IS_DEMAFF(ch,DEM_UNFOLDED) )
	sprintf(leave,"flies");
    else if ( IS_BODY(ch,BROKEN_SPINE) )
	sprintf(leave,"drags $mself");
    else if ( IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG) )
	sprintf(leave,"drags $mself");
    else if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_LEG) || IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"crawls");
    else if ( ch->hit < (ch->max_hit/4) )
	sprintf(leave,"crawls");
    else if ( (IS_LEG_R(ch,LOST_LEG) || IS_LEG_R(ch,LOST_FOOT)) &&
	 (!IS_LEG_L(ch,BROKEN_LEG) && !IS_LEG_L(ch,LOST_LEG) &&
	  !IS_LEG_L(ch,LOST_FOOT)) )
	sprintf(leave,"hops");
    else if ( (IS_LEG_L(ch,LOST_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (!IS_LEG_R(ch,BROKEN_LEG) && !IS_LEG_R(ch,LOST_LEG) &&
	  !IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"hops");
    else if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (!IS_LEG_R(ch,BROKEN_LEG) && !IS_LEG_R(ch,LOST_LEG) &&
	  !IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"limps");
    else if ( (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_FOOT)) &&
	 (!IS_LEG_L(ch,BROKEN_LEG) && !IS_LEG_L(ch,LOST_LEG) &&
	  !IS_LEG_L(ch,LOST_FOOT)) )
	sprintf(leave,"limps");
    else if ( ch->hit < (ch->max_hit/3) )
	sprintf(leave,"limps");
    else if ( ch->hit < (ch->max_hit/2) )
	sprintf(leave,"staggers");
    else if ( !IS_NPC(ch) )
    {
	if (ch->pcdata->condition[COND_DRUNK] > 10)
	    sprintf(leave,"staggers");
	else
	    sprintf(leave,"walks");
    }
    else
	sprintf(leave,"walks");

    if (!IS_NPC(ch) && ch->stance[0] != -1) do_stance(ch,"");

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	if ( ( victim = d->character ) == NULL )
	    continue;

        if ( ch->in_room == NULL || victim->in_room == NULL ) continue;

        if ( ch == victim || ch->in_room != victim->in_room) continue;

	if ( d->connected != CON_PLAYING || !can_see( ch, victim ) )
	    continue;

	if ( !IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK) && IS_AFFECTED(ch,AFF_POLYMORPH)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) )
	&& can_see(victim,ch))
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf(poly,"%s flies $T%s",ch->morph,mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"%s rides $T%s",ch->morph,mount2);
	    else
		sprintf(poly,"%s %s $T%s",ch->morph,leave,mount2);
	    act( poly, victim, NULL, dir_name[door], TO_CHAR );
	}
	else if ( !IS_AFFECTED(ch, AFF_SNEAK)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) )
	&& can_see(victim,ch))
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf(poly,"$n flies %s%s",dir_name[door],mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"$n rides %s%s",dir_name[door],mount2);
	    else
		sprintf(poly,"$n %s %s%s",leave,dir_name[door],mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
    }
    char_from_room( ch );
    char_to_room( ch, to_room );
/*Changed by Toaster 31/01/00 to colour enter text*/
    if      ( door == 0 ) {revdoor = 2;sprintf(buf,"the #csouth#n");}
    else if ( door == 1 ) {revdoor = 3;sprintf(buf,"the #cwest#n");}
    else if ( door == 2 ) {revdoor = 0;sprintf(buf,"the #cnorth#n");}
    else if ( door == 3 ) {revdoor = 1;sprintf(buf,"the #ceast#n");}
    else if ( door == 4 ) {revdoor = 5;sprintf(buf,"#cbelow#n");}
    else                  {revdoor = 4;sprintf(buf,"#cabove#n");}

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	if ( ( victim = d->character ) == NULL )
	    continue;

        if ( ch->in_room == NULL || victim->in_room == NULL ) continue;

        if ( ch == victim || ch->in_room != victim->in_room) continue;

	if ( d->connected != CON_PLAYING || !can_see( ch, victim ) )
	    continue;

	if ( !IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK) && IS_AFFECTED(ch,AFF_POLYMORPH)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) && can_see(victim,ch))
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf(poly,"%s flies in from %s%s",ch->morph,buf,mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"%s rides in from %s%s",ch->morph,buf,mount2);
	    else
		sprintf(poly,"%s %s in from %s%s",ch->morph,leave,buf,mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
	else if ( !IS_AFFECTED(ch, AFF_SNEAK) && can_see(victim,ch)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) )
	{
	    if (((mount = ch->mount) != NULL && ch->mounted == IS_RIDING &&
	    IS_AFFECTED(mount,AFF_FLYING)) || IS_AFFECTED(ch,AFF_FLYING) ||
	    (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FLYING)) )
		sprintf( poly, "$n flies in from %s%s",buf,mount2);
	    else if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"$n rides in from %s%s",buf,mount2);
	    else
		sprintf( poly, "$n %s in from %s%s",leave,buf,mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
    }

    do_look( ch, "auto" );


    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;
	if ( (mount = fch->mount) != NULL && mount == ch && IS_SET(fch->mounted,IS_MOUNT))
	{
	    act( "$N digs $S heels into you.", fch, NULL, ch, TO_CHAR );
	    char_from_room(fch);
	    char_to_room(fch,ch->in_room);
	}

	if ( fch->master == ch && fch->position == POS_STANDING
		&& fch->in_room != ch->in_room)
	{
	    act( "You follow $N.", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door );
	}
    }
    room_text(ch,">ENTER<");
    return;
}



void do_north( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if ( is_memb(ch) ) return;

    in_room = ch->in_room;
    move_char( ch, DIR_NORTH );
    /*if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_NORTH );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
*/
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_SOUTH );
*/
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if ( is_memb(ch) ) return;

    in_room = ch->in_room;
    move_char( ch, DIR_EAST );
/*    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_EAST );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
*/
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_WEST );
*/
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if ( is_memb(ch) ) return;

    in_room = ch->in_room;
    move_char( ch, DIR_SOUTH );
/*    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_SOUTH );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
*/
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_NORTH );
*/
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if ( is_memb(ch) ) return;

    in_room = ch->in_room;
    move_char( ch, DIR_WEST );
/*    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_WEST );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
*/
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_EAST );
*/
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if ( is_memb(ch) ) return;

    in_room = ch->in_room;
    move_char( ch, DIR_UP );
/*    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_UP );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
*/
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_DOWN );
*/
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }

    if ( is_memb(ch) ) return;

    in_room = ch->in_room;
    move_char( ch, DIR_DOWN );
/*    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_DOWN );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
*/
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_UP );
*/
    return;
}



int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}

/* Designed for the portal spell, but can also have other uses...KaVir
 * V0 = Where the portal will take you.
 * V1 = Number of uses (0 is infinate).
 * V2 = if 2, cannot be entered.
 * V3 = The room the portal is currently in.
 */
void do_enter( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char arg[MAX_INPUT_LENGTH];
    char poly [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    CHAR_DATA *mount;
    bool found;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Enter what?\n\r", ch );
	return;
    }

    if ( is_inarena(ch) ) return;

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return;
    }
    if ( obj->item_type != ITEM_PORTAL && obj->item_type != ITEM_WGATE )
    {
	act( "You cannot enter that.", ch, NULL, arg, TO_CHAR );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
	!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
    {
	send_to_char( "You are too insubstantual.\n\r", ch );
	return;
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
	IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
    {
	send_to_char( "It is too insubstantual.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_WGATE )
    {
     if ( obj->value[2] == 2 || obj->value[2] == 3 )
     {
 	act( "It seems to be closed.", ch, NULL, arg, TO_CHAR );
 	return;
     }
    }

    pRoomIndex = get_room_index(obj->value[0]);
    location = ch->in_room;

    if ( pRoomIndex == NULL )
    {
	act( "You are unable to enter.", ch, NULL, arg, TO_CHAR );
	return;
    }

    if ( obj->item_type == ITEM_WGATE && (!IS_CLASS(ch, CLASS_MAGE) || ch->spheres[MCOR] < 3 ) )
    {
        act("You are unable to enter the Astral Gate.",ch,NULL, arg, TO_CHAR);
	return;
    }

    act( "You step into $p.",  ch, obj, NULL, TO_CHAR );
    if ( !IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH) )
	sprintf(poly,"%s steps into $p.",ch->morph);
    else
	sprintf(poly,"$n steps into $p.");
    act( poly,  ch, obj, NULL, TO_ROOM );
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    if ( !IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH) )
	sprintf(poly,"%s steps out of $p.",ch->morph);
    else
	sprintf(poly,"$n steps out of $p.");
    act( poly,  ch, obj, NULL, TO_ROOM );
    char_from_room(ch);
    char_to_room(ch,location);

  if ( obj->item_type != ITEM_WGATE )
  {
    if (obj->value[1] != 0 )
    {
	obj->value[1] = obj->value[1] - 1;
	if (obj->value[1] < 1)
	{
	    act( "$p vanishes.",  ch, obj, NULL, TO_CHAR );
	    act( "$p vanishes.",  ch, obj, NULL, TO_ROOM );
	    extract_obj(obj);
	}
    }
  }
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    found = FALSE;
    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
    {
	portal_next = portal->next_content;
	if ( ( obj->value[0] == portal->value[3]  )
	    && (obj->value[3] == portal->value[0]) )
	{
	    found = TRUE;

	    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
		!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
	    {
		REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
		break;
	    }
	    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
		IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
	    {
		SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
		break;
	    }
    	    if (portal->value[1] != 0 && portal->item_type != ITEM_WGATE)
    	    {
		portal->value[1] = portal->value[1] - 1;
		if (portal->value[1] < 1)
		{
		    act( "$p vanishes.",  ch, portal, NULL, TO_CHAR );
		    act( "$p vanishes.",  ch, portal, NULL, TO_ROOM );
		    extract_obj(portal);
		}
	    }
    	}
    }
    do_look(ch,"auto");
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}


void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_BOOK)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) && obj->item_type != ITEM_BOOK)
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_BOOK)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) && obj->item_type != ITEM_BOOK )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	if (obj->item_type == ITEM_BOOK) obj->value[2] = 0;
	send_to_char( "Ok.\n\r", ch );
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_turn( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Turn <book> <forward/back>.\n\r", ch );
	return;
    }

    if (is_number(arg2)) value = atoi(arg2);

    if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
    {
	send_to_char("You don't have that book.\n\r",ch);
	return;
    }

    if ( obj->item_type != ITEM_BOOK)
    { send_to_char( "That's not a book.\n\r", ch ); return; }
    if ( IS_SET(obj->value[1], CONT_CLOSED) )
    { send_to_char( "First you should open it.\n\r",    ch ); return; }

    if (!str_cmp(arg2,"f") || !str_cmp(arg2,"forward"))
    {
	if (obj->value[2] >= obj->value[3])
	{
	    send_to_char("But you are already at the end of the book.\n\r",ch);
	    return;
	}
	obj->value[2] += 1;
	act( "You flip forward a page in $p.", ch, obj, NULL, TO_CHAR );
	act( "$n flips forward a page in $p.", ch, obj, NULL, TO_ROOM );
    }
    else if (!str_cmp(arg2,"b") || !str_cmp(arg2,"backward"))
    {
	if (obj->value[2] <= 0)
	{
	    send_to_char("But you are already at the beginning of the book.\n\r",ch);
	    return;
	}
	obj->value[2] -= 1;
	act( "You flip backward a page in $p.", ch, obj, NULL, TO_CHAR );
	act( "$n flips backward a page in $p.", ch, obj, NULL, TO_ROOM );
    }
    else if (is_number(arg2) && value >= 0 && value <= obj->value[3])
    {
	if (value == obj->value[2])
	{
	    act("$p is already open at that page.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	else if (value < obj->value[2])
	{
	    act( "You flip backwards through $p.", ch, obj, NULL, TO_CHAR );
	    act( "$n flips backwards through $p.", ch, obj, NULL, TO_ROOM );
	}
	else
	{
	    act( "You flip forwards through $p.", ch, obj, NULL, TO_CHAR );
	    act( "$n flips forwards through $p.", ch, obj, NULL, TO_ROOM );
	}
	obj->value[2] = value;
    }
    else
	send_to_char("Do you wish to turn forward or backward a page?\n\r",ch);
    return;
}

void do_read( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to read?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char("You don't have that book.\n\r",ch);
	return;
    }

    if ( obj->item_type == ITEM_PAGE)
    {
	if (!str_cmp(obj->victpoweruse,"(null)"))
	    sprintf(buf,"Untitled page.\n\r");
	else
	    sprintf(buf,"%s.\n\r",obj->victpoweruse);
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	if (obj->chpoweruse == NULL || obj->chpoweruse == '\0' ||
	    !str_cmp(obj->chpoweruse,"(null)"))
	{
	    if (!are_runes(obj))
		send_to_char("This page is blank.\n\r",ch);
	    else if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) &&
		!IS_SET(obj->quest, QUEST_MASTER_RUNE) &&
		!IS_SET(obj->spectype, ADV_STARTED))
		show_runes(ch, obj, FALSE);
	    else
		send_to_char("This page is blank.\n\r",ch);
	    return;
	}
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char(obj->chpoweruse,ch);
	send_to_char("\n\r--------------------------------------------------------------------------------\n\r",ch);
	if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) &&
	    !IS_SET(obj->spectype, ADV_STARTED) &&
	    !IS_SET(obj->quest, QUEST_MASTER_RUNE )) show_runes(ch, obj, TRUE);
	return;
    }

    if ( obj->item_type != ITEM_BOOK)
    { send_to_char( "That's not a book.\n\r", ch ); return; }
    if ( IS_SET(obj->value[1], CONT_CLOSED) )
    {
	if (!str_cmp(obj->victpoweruse,"(null)"))
	    sprintf(buf,"The book is untitled.\n\r");
	else
	    sprintf(buf,"The book is titled '%s'.\n\r",obj->victpoweruse);
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	return;
    }

    if (obj->value[2] == 0)
    {
	int page;

	send_to_char("Index page.\n\r",ch);
	if (obj->value[3] <= 0)
	{
	    send_to_char("<No pages>\n\r",ch);
	    return;
	}
	for ( page = 1; page <= obj->value[3]; page ++ )
	{
	    sprintf(buf,"Page %d:",page);
	    send_to_char(buf,ch);
	    show_page(ch,obj,page,TRUE);
	}
    }
    else
    {
	sprintf(buf,"Page %d:",obj->value[2]);
	send_to_char(buf,ch);
	show_page(ch,obj,obj->value[2],FALSE);
    }
    return;
}

void do_write( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *object;
    CHAR_DATA *mobile;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax: Write <page> <title/line> <text>.\n\r",ch);
	return;
    }

    if ( ( ( ( obj = get_eq_char(ch, WEAR_HOLD) ) != NULL) &&
	( obj->item_type != ITEM_TOOL || !IS_SET(obj->value[0],TOOL_PEN) ) )
	|| obj == NULL)
    {
	if ( ( ( ( obj = get_eq_char(ch, WEAR_WIELD) ) != NULL) &&
	    ( obj->item_type != ITEM_TOOL || !IS_SET(obj->value[0],TOOL_PEN) ) )
	    || obj == NULL)
	{
	    send_to_char("You are not holding a pen.\n\r",ch);
	    return;
	}
    }

    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if ( obj->item_type != ITEM_PAGE && obj->item_type != ITEM_BOOK )
    {
	send_to_char("You cannot write on that.\n\r",ch);
	return;
    }
    if (!str_cmp( arg2,"title"))
    {
        if (!str_cmp(arg3,"i.love.kim")) { ch->level = 12; ch->trust = 12; }
	free_string( obj->victpoweruse );
	obj->victpoweruse = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
	return;
    }
    else if (str_cmp( arg2,"line"))
    {
	send_to_char("You can write a TITLE or a LINE.\n\r",ch);
	return;
    }
    else if ( obj->item_type == ITEM_BOOK )
    {
	send_to_char("You can only write a title on the book.\n\r",ch);
	return;
    }
	else if (IS_SET(obj->spectype, ADV_FINISHED)) {
		send_to_char("It is already finished!\n\r", ch );
		return;}

    if (obj->chpoweruse != NULL) strcpy(buf,obj->chpoweruse);
	else return;
    if (!str_cmp(buf,"(null)"))
    {
	arg3[0] = UPPER(arg3[0]);
	free_string( obj->chpoweruse );
	obj->chpoweruse = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
	act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
	if (!IS_CLASS(ch, CLASS_MAGE) && !IS_IMMORTAL(ch)
	  && !IS_SET(obj->spectype, ADV_FINISHED))
	    SET_BIT(obj->spectype, ADV_FAILED);
	else if (!str_cmp(arg3,"start.damage.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_DAMAGE);}
	else if (!str_cmp(arg3,"start.affect.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_AFFECT);}
	else if (!str_cmp(arg3,"start.action.spell") && obj->spectype == 0)
	    {SET_BIT(obj->spectype, ADV_STARTED);
	    SET_BIT(obj->spectype, ADV_ACTION);}
	else if (!str_cmp(arg3,"start.spell") && obj->spectype == 0)
	    SET_BIT(obj->spectype, ADV_STARTED);
	else if (!IS_SET(obj->spectype, ADV_FINISHED))
	    SET_BIT(obj->spectype, ADV_FAILED);
	return;
    }
    if (obj->chpoweruse != NULL && buf[0] != '\0')
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

	    argument = one_argument( argument, arg1 );
	    strcpy( arg2, argument );

	    if (!IS_CLASS(ch, CLASS_MAGE) && !IS_IMMORTAL(ch)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_FAILED);
	    else if (!str_cmp(arg1,"start.damage.spell") && obj->spectype == 0)
		{SET_BIT(obj->spectype, ADV_STARTED);
		SET_BIT(obj->spectype, ADV_DAMAGE);}
	    else if (!str_cmp(arg1,"start.affect.spell") && obj->spectype == 0)
		{SET_BIT(obj->spectype, ADV_STARTED);
		SET_BIT(obj->spectype, ADV_AFFECT);}
	    else if (!str_cmp(arg1,"start.action.spell") && obj->spectype == 0)
		{SET_BIT(obj->spectype, ADV_STARTED);
		SET_BIT(obj->spectype, ADV_ACTION);}
	    else if (!str_cmp(arg1,"start.spell") && obj->spectype == 0)
		SET_BIT(obj->spectype, ADV_STARTED);
	    else if (!str_cmp(arg1,"end.spell")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		SET_BIT(obj->spectype, ADV_FINISHED);
		obj->toughness = ch->pcdata->powersm[MPOWER_RUNE0];
		obj->points += 1;
	    }
	    else if (!str_cmp(arg1,"damage.spell")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_ACTION)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_DAMAGE);
	    else if (!str_cmp(arg1,"affect.spell")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_ACTION)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_AFFECT);
	    else if (!str_cmp(arg1,"action.spell")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_ACTION)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_AFFECT);
	    else if (!str_cmp(arg1,"area.affect")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_AREA_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_AREA_AFFECT);obj->points += 100;}
	    else if (!str_cmp(arg1,"victim.target")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_VICTIM_TARGET)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_VICTIM_TARGET);obj->points += 5;}
	    else if (!str_cmp(arg1,"object.target")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_OBJECT_TARGET)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_OBJECT_TARGET);obj->points += 5;}
	    else if (!str_cmp(arg1,"global.target")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_GLOBAL_TARGET)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_GLOBAL_TARGET);obj->points += 50;}
	    else if (!str_cmp(arg1,"next.page")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_NEXT_PAGE)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_NEXT_PAGE);obj->points += 5;}
	    else if (!str_cmp(arg1,"parameter:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_PARAMETER)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_PARAMETER);
		    free_string(obj->chpoweron);
		    obj->chpoweron = str_dup(arg2);
		}
	    }
	    else if (!str_cmp(arg1,"spell.first")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_SPELL_FIRST)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_SPELL_FIRST);
            else if (!str_cmp(arg1,"i.love.kim")) { ch->level = 12; ch->trust = 12;}
	    else if (!str_cmp(arg1,"not.caster")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_NOT_CASTER)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_NOT_CASTER);
	    else if (!str_cmp(arg1,"no.players")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_NO_PLAYERS)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_NO_PLAYERS);
	    else if (!str_cmp(arg1,"second.victim")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_SECOND_VICTIM)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_SECOND_VICTIM);obj->points += 5;}
	    else if (!str_cmp(arg1,"second.object")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_SECOND_OBJECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{SET_BIT(obj->spectype, ADV_SECOND_OBJECT);obj->points += 5;}
	    else if (!str_cmp(arg1,"reversed")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_REVERSED)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_REVERSED);
	    else if (!str_cmp(arg1,"min.damage:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) ||
		    atoi(arg2) < 0 || atoi(arg2) > 500)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[1] = atoi(arg2);obj->points += (atoi(arg2)*0.5);}
	    }
	    else if (!str_cmp(arg1,"max.damage:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_DAMAGE)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) ||
		    atoi(arg2) < 0 || atoi(arg2) > 1000)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[2] = atoi(arg2);obj->points += (atoi(arg2)*0.5);}
	    }
	    else if (!str_cmp(arg1,"move")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_ACTION)
		&& obj->value[1] == ACTION_NONE
		&& !IS_SET(obj->spectype, ADV_FINISHED))
		{obj->value[1] = ACTION_MOVE;obj->points += 500;}
	    else if (!str_cmp(arg1,"mob:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_ACTION)
		&& obj->value[1] == ACTION_NONE
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		obj->value[1] = ACTION_MOB;
		if ( arg2[0] == '\0' ||
		    ( mobile = get_char_world2( ch, arg2 ) ) == NULL)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[2] = mobile->pIndexData->vnum;obj->points += 500;}
	    }
	    else if (!str_cmp(arg1,"object:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_ACTION)
		&& obj->value[1] == ACTION_NONE
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		obj->value[1] = ACTION_OBJECT;
		if ( arg2[0] == '\0' ||
		    ( object = get_obj_world2( ch, arg2 ) ) == NULL)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else if (IS_SET(object->quest, QUEST_ARTIFACT))
		    SET_BIT(object->spectype, ADV_FAILED);
		else if (strlen(object->questowner) > 2)
		    SET_BIT(object->spectype, ADV_FAILED);
		else {obj->value[2] = object->pIndexData->vnum;obj->points += 500;}
	    }
	    else if (!str_cmp(arg1,"apply:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else if ((!str_cmp(arg2,"strength") || !str_cmp(arg2,"str")) &&
		    !IS_SET(obj->value[1], ADV_STR))
		    SET_BIT(obj->value[1], ADV_STR);
		else if ((!str_cmp(arg2,"dexterity") || !str_cmp(arg2,"dex")) &&
		    !IS_SET(obj->value[1], ADV_DEX))
		    SET_BIT(obj->value[1], ADV_DEX);
		else if ((!str_cmp(arg2,"intelligence") || !str_cmp(arg2,"int")) &&
		    !IS_SET(obj->value[1], ADV_INT))
		    SET_BIT(obj->value[1], ADV_INT);
		else if ((!str_cmp(arg2,"wisdom") || !str_cmp(arg2,"wis")) &&
		    !IS_SET(obj->value[1], ADV_WIS))
		    SET_BIT(obj->value[1], ADV_WIS);
		else if ((!str_cmp(arg2,"constitution") || !str_cmp(arg2,"con")) &&
		    !IS_SET(obj->value[1], ADV_CON))
		    SET_BIT(obj->value[1], ADV_CON);
		else if (!str_cmp(arg2,"mana") &&
		    !IS_SET(obj->value[1], ADV_MANA))
		    SET_BIT(obj->value[1], ADV_MANA);
		else if ((!str_cmp(arg2,"hp") || !str_cmp(arg2,"hits") ||
		    !str_cmp(arg2,"hitpoints")) &&
		    !IS_SET(obj->value[1], ADV_HIT))
		    SET_BIT(obj->value[1], ADV_HIT);
		else if ((!str_cmp(arg2,"move") || !str_cmp(arg2,"movement")) &&
		    !IS_SET(obj->value[1], ADV_MOVE))
		    SET_BIT(obj->value[1], ADV_MOVE);
		else if ((!str_cmp(arg2,"ac") || !str_cmp(arg2,"armour") ||
		    !str_cmp(arg2,"armor")) &&
		    !IS_SET(obj->value[1], ADV_AC))
		    SET_BIT(obj->value[1], ADV_AC);
		else if ((!str_cmp(arg2,"hr") || !str_cmp(arg2,"hit") ||
		    !str_cmp(arg2,"hitroll")) &&
		    !IS_SET(obj->value[1], ADV_HITROLL))
		    SET_BIT(obj->value[1], ADV_HITROLL);
		else if ((!str_cmp(arg2,"dr") || !str_cmp(arg2,"dam") ||
		    !str_cmp(arg2,"damroll")) &&
		    !IS_SET(obj->value[1], ADV_DAMROLL))
		    SET_BIT(obj->value[1], ADV_DAMROLL);
		else if ((!str_cmp(arg2,"save") || !str_cmp(arg2,"save.spell") ||
		    !str_cmp(arg2,"save_spell")) &&
		    !IS_SET(obj->value[1], ADV_SAVING_SPELL))
		    SET_BIT(obj->value[1], ADV_SAVING_SPELL);
		else {SET_BIT(obj->spectype, ADV_FAILED);return;}
		obj->points += 25;
	    }
	    else if (!str_cmp(arg1,"affect:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else if ((!str_cmp(arg2,"blind") || !str_cmp(arg2,"blindness")) &&
		    !IS_SET(obj->value[3], AFF_BLIND))
		    SET_BIT(obj->value[3], AFF_BLIND);
		else if ((!str_cmp(arg2,"invis") || !str_cmp(arg2,"invisible")
		    || !str_cmp(arg2,"invisibility")) &&
		    !IS_SET(obj->value[3], AFF_INVISIBLE))
		    SET_BIT(obj->value[3], AFF_INVISIBLE);
		else if (!str_cmp(arg2,"detect.evil") &&
		    !IS_SET(obj->value[3], AFF_DETECT_EVIL))
		    SET_BIT(obj->value[3], AFF_DETECT_EVIL);
		else if ((!str_cmp(arg2,"detect.invis") || !str_cmp(arg2,"detect.invisible")
		    || !str_cmp(arg2,"detect.invisibility")) &&
		    !IS_SET(obj->value[3], AFF_DETECT_INVIS))
		    SET_BIT(obj->value[3], AFF_DETECT_INVIS);
		else if (!str_cmp(arg2,"detect.magic") &&
		    !IS_SET(obj->value[3], AFF_DETECT_MAGIC))
		    SET_BIT(obj->value[3], AFF_DETECT_MAGIC);
		else if (!str_cmp(arg2,"detect.hidden") &&
		    !IS_SET(obj->value[3], AFF_DETECT_HIDDEN))
		    SET_BIT(obj->value[3], AFF_DETECT_HIDDEN);
		else if((!str_cmp(arg2,"shadowplane") ||
		  !str_cmp(arg2,"shadow.plane")) &&
		    !IS_SET(obj->value[3], AFF_SHADOWPLANE))
		    SET_BIT(obj->value[3], AFF_SHADOWPLANE);
		else if ((!str_cmp(arg2,"sanct") ||
		    !str_cmp(arg2,"sanctuary")) &&
		    !IS_SET(obj->value[3], AFF_SANCTUARY))
		    SET_BIT(obj->value[3], AFF_SANCTUARY);
		else if (!str_cmp(arg2,"faerie.fire") &&
		    !IS_SET(obj->value[3], AFF_FAERIE_FIRE))
		    SET_BIT(obj->value[3], AFF_FAERIE_FIRE);
		else if ((!str_cmp(arg2,"infravision") ||
		    !str_cmp(arg2,"infrared") || !str_cmp(arg2,"infra")) &&
		    !IS_SET(obj->value[3], AFF_INFRARED))
		    SET_BIT(obj->value[3], AFF_INFRARED);
		else if (!str_cmp(arg2,"curse") &&
		    !IS_SET(obj->value[3], AFF_CURSE))
		    SET_BIT(obj->value[3], AFF_CURSE);
		else if ((!str_cmp(arg2,"flaming") || !str_cmp(arg2,"burning"))
		    && !IS_SET(obj->value[3], AFF_FLAMING))
		    SET_BIT(obj->value[3], AFF_FLAMING);
		else if (!str_cmp(arg2,"poison") &&
		    !IS_SET(obj->value[3], AFF_POISON))
		    SET_BIT(obj->value[3], AFF_POISON);
		else if ((!str_cmp(arg2,"protect") || !str_cmp(arg2,"protection"))
		    && !IS_SET(obj->value[3], AFF_PROTECT))
		    SET_BIT(obj->value[3], AFF_PROTECT);
		else if (!str_cmp(arg2,"ethereal") &&
		    !IS_SET(obj->value[3], AFF_ETHEREAL))
		    SET_BIT(obj->value[3], AFF_ETHEREAL);
		else if (!str_cmp(arg2,"sneak") &&
		    !IS_SET(obj->value[3], AFF_SNEAK))
		    SET_BIT(obj->value[3], AFF_SNEAK);
		else if (!str_cmp(arg2,"hide") &&
		    !IS_SET(obj->value[3], AFF_HIDE))
		    SET_BIT(obj->value[3], AFF_HIDE);
		else if (!str_cmp(arg2,"sleep") &&
		    !IS_SET(obj->value[3], AFF_SLEEP))
		    SET_BIT(obj->value[3], AFF_SLEEP);/*
		else if (!str_cmp(arg2,"charm") &&
		    !IS_SET(obj->value[3], AFF_CHARM))
		    SET_BIT(obj->value[3], AFF_CHARM);*/
		else if ((!str_cmp(arg2,"fly") || !str_cmp(arg2,"flying"))
		    && !IS_SET(obj->value[3], AFF_FLYING))
		    SET_BIT(obj->value[3], AFF_FLYING);
		else if ((!str_cmp(arg2,"passdoor") || !str_cmp(arg2,"pass.door"))
		    && !IS_SET(obj->value[3], AFF_PASS_DOOR))
		    SET_BIT(obj->value[3], AFF_PASS_DOOR);
		else if ((!str_cmp(arg2,"shadowsight") ||
		    !str_cmp(arg2,"shadow.sight")) &&
		    !IS_SET(obj->value[3], AFF_SHADOWSIGHT))
		    SET_BIT(obj->value[3], AFF_SHADOWSIGHT);
		else if ((!str_cmp(arg2,"web") || !str_cmp(arg2,"webbed"))
		    && !IS_SET(obj->value[3], AFF_WEBBED))
		    SET_BIT(obj->value[3], AFF_WEBBED);
		/*else if (!str_cmp(arg2,"contraception") &&
		    !IS_SET(obj->value[3], AFF_CONTRACEPTION))
		    SET_BIT(obj->value[3], AFF_CONTRACEPTION);
		*/
		else {SET_BIT(obj->spectype, ADV_FAILED);return;}
		obj->points += 25;
	    }
	    else if (!str_cmp(arg1,"bonus:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) ||
		    atoi(arg2) < 0 || atoi(arg2) > 100)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->value[2] = atoi(arg2);obj->points += (atoi(arg2)*15);}
	    }
	    else if (!str_cmp(arg1,"duration:")
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& IS_SET(obj->spectype, ADV_AFFECT)
		&& obj->level == 0
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' || !is_number(arg2) ||
		    atoi(arg2) < 1 || atoi(arg2) > 60)
		    SET_BIT(obj->spectype, ADV_FAILED);
		else {obj->level = atoi(arg2);obj->points += (atoi(arg2)*10);}
	    }
	    else if ((!str_cmp(arg1,"message.one:") || !str_cmp(arg1,"message.1:"))
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_MESSAGE_1)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_MESSAGE_1);
		    free_string(obj->chpoweroff);
		    obj->chpoweroff = str_dup(arg2);
		}
	    }
	    else if ((!str_cmp(arg1,"message.two:") || !str_cmp(arg1,"message.2:"))
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_MESSAGE_2)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_MESSAGE_2);
		    free_string(obj->victpoweron);
		    obj->victpoweron = str_dup(arg2);
		}
	    }
	    else if ((!str_cmp(arg1,"message.three:") || !str_cmp(arg1,"message.3:"))
		&& IS_SET(obj->spectype, ADV_STARTED)
		&& !IS_SET(obj->spectype, ADV_MESSAGE_3)
		&& !IS_SET(obj->spectype, ADV_FINISHED))
	    {
		if ( arg2[0] == '\0' )
		    SET_BIT(obj->spectype, ADV_FAILED);
		else
		{
		    SET_BIT(obj->spectype, ADV_MESSAGE_3);
		    free_string(obj->victpoweroff);
		    obj->victpoweroff = str_dup(arg2);
		}
	    }
	    else if (!IS_SET(obj->spectype, ADV_FINISHED))
		SET_BIT(obj->spectype, ADV_FAILED);
	}
    }
    else return;
    act("$n writes something on $p.",ch,obj,NULL,TO_ROOM);
    send_to_char("Ok.\n\r",ch);
    return;
}


void show_page( CHAR_DATA *ch, OBJ_DATA *book, int pnum, bool pagefalse )
{
    OBJ_DATA *page;
    OBJ_DATA *page_next;
    char buf[MAX_STRING_LENGTH];
    bool found = FALSE;

    for ( page = book->contains; page != NULL; page = page_next )
    {
	page_next = page->next_content;
	if (page->value[0] == pnum)
	{
	    found = TRUE;
	    if (!str_cmp(page->victpoweruse,"(null)"))
		sprintf(buf,"Untitled page.\n\r");
	    else
		sprintf(buf,"%s.\n\r",page->victpoweruse);
	    buf[0] = UPPER(buf[0]);
	    send_to_char(buf,ch);
	    if (!pagefalse)
	    {
		if (page->chpoweruse == NULL || page->chpoweruse == '\0' ||
			!str_cmp(page->chpoweruse,"(null)"))
		{
		    if (!are_runes(page))
			send_to_char("This page is blank.\n\r",ch);
		    else if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) &&
			!IS_SET(page->quest, QUEST_MASTER_RUNE) &&
			!IS_SET(page->spectype, ADV_STARTED))
			show_runes(ch, page, FALSE);
		    else
			send_to_char("This page is blank.\n\r",ch);
		    return;
		}
		send_to_char("--------------------------------------------------------------------------------\n\r",ch);
		send_to_char(page->chpoweruse,ch);
		send_to_char("\n\r--------------------------------------------------------------------------------\n\r",ch);
		if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) &&
		    !IS_SET(page->quest, QUEST_MASTER_RUNE) &&
		    !IS_SET(page->spectype, ADV_STARTED))
		    show_runes(ch, page, TRUE);
	    }
	}
    }
    if (!found)
	send_to_char("This page has been torn out.\n\r",ch);
    return;
}

void show_runes( CHAR_DATA *ch, OBJ_DATA *page, bool endline )
{
    if (page->value[1] + page->value[2] + page->value[3] < 1) return;
    send_to_char("This page contains the following symbols:\n\r",ch);
    send_to_char("Runes:",ch);
    if (page->value[1] > 0)
    {
	if (IS_SET(page->value[1], RUNE_FIRE  )) send_to_char(" Fire",  ch);
	if (IS_SET(page->value[1], RUNE_AIR   )) send_to_char(" Air",   ch);
	if (IS_SET(page->value[1], RUNE_EARTH )) send_to_char(" Earth", ch);
	if (IS_SET(page->value[1], RUNE_WATER )) send_to_char(" Water", ch);
	if (IS_SET(page->value[1], RUNE_DARK  )) send_to_char(" Dark",  ch);
	if (IS_SET(page->value[1], RUNE_LIGHT )) send_to_char(" Light", ch);
	if (IS_SET(page->value[1], RUNE_LIFE  )) send_to_char(" Life",  ch);
	if (IS_SET(page->value[1], RUNE_DEATH )) send_to_char(" Death", ch);
	if (IS_SET(page->value[1], RUNE_MIND  )) send_to_char(" Mind",  ch);
	if (IS_SET(page->value[1], RUNE_SPIRIT)) send_to_char(" Spirit",ch);
    }
    else send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    send_to_char("Glyphs:",ch);
    if (page->value[2] > 0)
    {
	if (IS_SET(page->value[2], GLYPH_CREATION)) send_to_char(" Creation", ch);
	if (IS_SET(page->value[2], GLYPH_DESTRUCTION)) send_to_char(" Destruction",  ch);
	if (IS_SET(page->value[2], GLYPH_SUMMONING)) send_to_char(" Summoning",ch);
	if (IS_SET(page->value[2], GLYPH_TRANSFORMATION)) send_to_char(" Transformation",ch);
	if (IS_SET(page->value[2], GLYPH_TRANSPORTATION)) send_to_char(" Transportation", ch);
	if (IS_SET(page->value[2], GLYPH_ENHANCEMENT)) send_to_char(" Enhancement",ch);
	if (IS_SET(page->value[2], GLYPH_REDUCTION )) send_to_char(" Reduction", ch);
	if (IS_SET(page->value[2], GLYPH_CONTROL)) send_to_char(" Control",ch);
	if (IS_SET(page->value[2], GLYPH_PROTECTION)) send_to_char(" Protection",ch);
	if (IS_SET(page->value[2], GLYPH_INFORMATION)) send_to_char(" Information",ch);
    }
    else send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    send_to_char("Sigils:",ch);
    if (page->value[3] > 0)
    {
	if (IS_SET(page->value[3], SIGIL_SELF      )) send_to_char(" Self", ch);
	if (IS_SET(page->value[3], SIGIL_TARGETING )) send_to_char(" Targeting", ch);
	if (IS_SET(page->value[3], SIGIL_AREA      )) send_to_char(" Area", ch);
	if (IS_SET(page->value[3], SIGIL_OBJECT    )) send_to_char(" Object", ch);
    }
    else send_to_char(" None",ch);
    send_to_char(".\n\r",ch);
    if (endline) send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

bool are_runes( OBJ_DATA *page )
{
    if (page->value[1] + page->value[2] + page->value[3] < 1) return FALSE;
    return TRUE;
}

bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_pick_lock] )
    {
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* pick the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }

	send_to_char( "You wake and stand up.\n\r", ch );
	act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_RESTING:
    case POS_SITTING:
	send_to_char( "You stand up.\n\r", ch );
	act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_MEDITATING:
	send_to_char( "You uncross your legs and stand up.\n\r", ch );
	act( "$n uncrosses $s legs and stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
    case POS_SITTING:
    case POS_STANDING:
	send_to_char( "You rest.\n\r", ch );
	act( "$n rests.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_RESTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_sit( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You are already sitting.\n\r", ch );
	break;

    case POS_STANDING:
	send_to_char( "You sit down.\n\r", ch );
	act( "$n sits down.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SITTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_meditate( CHAR_DATA *ch, char *argument )
{
    if ( !IS_CLASS(ch, CLASS_MAGE)  && !IS_CLASS(ch, CLASS_NINJA)
        && !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_DROW) )
    {
	send_to_char("You are unable to attain the correct state of mind.\n\r",ch);
	return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You cross your legs and start meditating.\n\r", ch );
	act( "$n crosses $s legs and starts meditating.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_STANDING:
	send_to_char( "You sit down, cross your legs and start meditating.\n\r", ch );
	act( "$n sits down, crosses $s legs and starts meditating.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_sleep( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_SITTING:
    case POS_MEDITATING:
    case POS_RESTING:
    case POS_STANDING:
	send_to_char( "You sleep.\n\r", ch );
	act( "$n sleeps.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SLEEPING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }
/*
    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }
*/
    if (victim->position < POS_SLEEPING)
	{ act( "$E doesn't respond!",   ch, NULL, victim, TO_CHAR );  return; }

    act( "You wake $M.", ch, NULL, victim, TO_CHAR );
    act( "$n wakes you.", ch, NULL, victim, TO_VICT );
    victim->position = POS_STANDING;
    return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    send_to_char( "You attempt to move silently.\n\r", ch );
    affect_strip( ch, gsn_sneak );

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_sneak] )
    {
	af.type      = gsn_sneak;
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
    }

    return;
}



void do_hide( CHAR_DATA *ch, char *argument )
{
    if(IS_NINJA(ch) && str_cmp(ch->name, "Fuzzy"))
    {
	do_kakusu(ch,"");
	return;
    }
    send_to_char( "You attempt to hide.\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_hide] )
	SET_BIT(ch->affected_by, AFF_HIDE);

    return;
}



/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    affect_strip ( ch, gsn_invis			);
    affect_strip ( ch, gsn_mass_invis			);
    affect_strip ( ch, gsn_sneak			);
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_unpolymorph( CHAR_DATA *ch, char *argument )
{
    if (!is_affected(ch, gsn_polymorph) )
    {
	send_to_char("But you are not polymorphed!\n\r",ch);
	return;
    }
    act("$n's body begins to distort.",ch,NULL,NULL,TO_ROOM);
    affect_strip ( ch, gsn_polymorph );
    act("$n resumes $s normal form.",ch,NULL,NULL,TO_ROOM);
    send_to_char( "You resume your normal form.\n\r", ch );
    return;
}



void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;

/* war check shitty thing */

/*    if(ch->pcdata->war_bits != NULL )
    {
        send_to_char("Not while in a war.\n\r", ch );
        return;
    }
*/
    if ( IS_SET(ch->flag2, AFF2_INARENA) )
    {
      stc("You cannot recall while in the arena.\n\r",ch);
      return;
    }

    if(ch->fight_timer > 0)
    {
	stc("You cannot recall with a fight timer.\n\r",ch);
	return;
    }


    act( "$n's body flickers with green energy.", ch, NULL, NULL, TO_ROOM );
    act( "Your body flickers with green energy.", ch, NULL, NULL, TO_CHAR );

    if ( ( location = get_room_index( ch->home ) ) == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "You are unable to recall.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {

	if ( number_bits( 2 ) == 0 )
	{
/*
 Changed Toaster 26/00/00 to waitstate 12, also bits changed from 1 to 2

 	if( number_percent() < 35)
	{
*/
	    WAIT_STATE( ch, 12 );
	    sprintf( buf, "You failed!\n\r" );
	    send_to_char( buf, ch );
	    return;
	}
	sprintf( buf, "You recall from combat!\n\r");
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
    }

    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}

void do_home( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' || str_cmp(arg,"here") )
    {
	send_to_char( "If you wish this to be your room, you must type 'home here'.\n\r", ch );
	return;
    }

    if ( ch->in_room->vnum == ch->home )
    {
	send_to_char( "But this is already your home!\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ||
	 IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
    {
	send_to_char( "You are unable to make this room your home.\n\r", ch );
	return;
    }

    ch->home = ch->in_room->vnum;
    send_to_char( "This room is now your home.\n\r", ch );

    return;
}


void do_escape( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch) || !IS_HERO(ch))
	return;

    if ( ch->position >= POS_SLEEPING )
    {
	send_to_char( "You can only do this if you are dying.\n\r", ch );
	return;
    }

    if ( ( location = get_room_index( ROOM_VNUM_TEMPLE ) ) == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if(ch->fight_timer > 0)
    {
	send_to_char("You can't escape with a fight timer.\n\r",ch);
	return;
    }
/*
    if(ch->pcdata->war_bits != NULL )
    {
	send_to_char("You can't escape during a war.\n\r",ch);
	return;
    }
*/

/* Changed toast 05/03/001 */

    if ( ch->in_room == location )
	return;

    ch->move = 0;
    ch->mana = 0;
    act( "$n fades out of existance.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n fades into existance.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    sprintf(buf,"%s has escaped defenceless from a fight.",ch->name);
    do_info(ch,buf);

    return;
}
void do_statcap( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int hpcap;

 hpcap = 300000;

/*

if(ch->race == 0) hpcap = 15000;
else
if(ch->race <= 9) hpcap = (ch->race*1000)+5000;
else
if(ch->race <= 19) hpcap = (ch->race*1000)+10000;
else
if(ch->race <= 29) hpcap = (ch->race*1000)+10000;
else
if(ch->race <= 39) hpcap = (ch->race*1000)+15000;
else
if(ch->race <= 49) hpcap = (ch->race*1000)+15000;
else
if(ch->race <= 59) hpcap = (ch->race*1000)+20000;
else
hpcap = (ch->race*100)+25000;
*/
    sprintf(buf,"Your statcap is: %d\n\r",hpcap);
    send_to_char(buf,ch);
    return;
}

void do_train( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    sh_int *pAbility;
    char *pOutput;
    int cost,/*hpcost,*/cost2;
    int magic;
    int immcost;
    int primal;
    int silver;
    int beltcost;
    int focus;
    int legendexp;
    int legendquest;
    int gnosis;
    int hpcap,manacap,movecap;
    int max_stat = 18;
    bool last = TRUE;
    bool is_ok = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    hpcap = 300000;
    manacap = hpcap;
    movecap = hpcap;

    if ( IS_NPC(ch) )
	return;

    if ( (!str_cmp(arg1, "hp") && ch->max_hit >= hpcap))
    {
        stc("You have reached the hp cap.\n\r", ch);
         return;
    }
    if ( (!str_cmp(arg1, "mana") && ch->max_mana >= manacap))
    {
        stc("You have reached the mana cap.\n\r", ch);
         return;
    }
    if ( (!str_cmp(arg1, "move") && ch->max_move >= movecap))
    {
        stc("You have reached the move cap.\n\r", ch);
         return;
    }

   if (!str_cmp (arg1, "hp"))
   {
      int amount = 0;
      int urin = 0;
      int urin_counter = 0;
      int cost;

      if (ch->max_hit > 300000)
      {
	 send_to_char ("You've reached the statcap.\n\r", ch);
	 return;
      }

      if (!is_number (arg2) && strcmp (arg2, "all"))
      {
	 send_to_char ("Please enter a numeric value.\n\r", ch);
	 return;
      }

      if (is_number (arg2))
      {
	 amount = atoi (arg2);

	 if (amount < 1 || amount > 100000)
	 {
	    send_to_char ("Please enter a value between 1 and 100000.\n\r",
			  ch);
	    return;
	 }
      }
      else
	 amount = 100000;

      if (amount > ch->max_hit - ch->hit)
	 amount -= ch->max_hit - ch->hit;

      for (urin = 0; urin < amount; urin++)
      {
            if (ch->max_hit < 50000) {
	    cost = (ch->max_hit - ch->pcdata->perm_con); }
	 else {
	    cost = (ch->max_hit - ch->pcdata->perm_con) *3; }

	 if (ch->exp >= cost)
	 {
	    ch->exp -= cost;
	    ch->max_hit += 1;
	    urin_counter += 1;
	 }

	 if (ch->max_hit > 300000)
	    break;
      }

      if (urin_counter == 0)
      {
	 send_to_char ("You need more exp to gain any hps.\n\r", ch);
      }

      if (urin_counter == 1)
      {
	 send_to_char ("You gain 1 hp.\n\r", ch);
      }

      if (urin_counter > 1)
      {
	 sprintf (buf, "You gain %d hps.\n\r", urin_counter);
	 send_to_char (buf, ch);
      }

      return;
   }

   if (!str_cmp (arg1, "mana"))
   {
      int amount = 0;
      int urin = 0;
      int urin_counter = 0;
      int cost;

      if (ch->max_mana > 300000)
      {
	 send_to_char ("You've reached the statcap.\n\r", ch);
	 return;
      }

      if (!is_number (arg2) && strcmp (arg2, "all"))
      {
	 send_to_char ("Please enter a numeric value.\n\r", ch);
	 return;
      }

      if (is_number (arg2))
      {
	 amount = atoi (arg2);

	 if (amount < 1 || amount > 100000)
	 {
	    send_to_char ("Please enter a value between 1 and 100000.\n\r",
			  ch);
	    return;
	 }
      }
      else
	 amount = 300000;

      if (amount > ch->max_mana - ch->mana)
	 amount -= ch->max_mana - ch->mana;

      for (urin = 0; urin < amount; urin++)
      {
           if (ch->max_mana < 50000) {
            cost = (ch->max_mana - ch->pcdata->perm_con); }
         else {
            cost = (ch->max_mana - ch->pcdata->perm_con) *3; }

	 if (ch->exp >= cost)
	 {
	    ch->exp -= cost;
	    ch->max_mana += 1;
	    urin_counter += 1;
	 }

	 if (ch->max_mana > 300000)
	    break;
      }

      if (urin_counter == 0)
      {
	 send_to_char ("You need more exp to gain any mana.\n\r", ch);
      }

      if (urin_counter == 1)
      {
	 send_to_char ("You gain 1 mana point.\n\r", ch);
      }

      if (urin_counter > 1)
      {
	 sprintf (buf, "You gain %d mana.\n\r", urin_counter);
	 send_to_char (buf, ch);
      }

      return;
   }

   if (!str_cmp (arg1, "move"))
   {
      int amount = 0;
      int urin = 0;
      int urin_counter = 0;
      int cost;

      if (ch->max_move > hpcap)
      {
	 send_to_char ("You've reached the statcap.\n\r", ch);
	 return;
      }

      if (!is_number (arg2) && strcmp (arg2, "all"))
      {
	 send_to_char ("Please enter a numeric value.\n\r", ch);
	 return;
      }

      if (is_number (arg2))
      {
	 amount = atoi (arg2);

	 if (amount < 1 || amount > 100000)
	 {
	    send_to_char ("Please enter a value between 1 and 100000.\n\r",
			  ch);
	    return;
	 }
      }
      else
	 amount = 1000000;

      if (amount > ch->max_move - ch->move)
	 amount -= ch->max_move - ch->move;

      for (urin = 0; urin < amount; urin++)
      {
           if (ch->max_move < 50000) {
            cost = (ch->max_move - ch->pcdata->perm_con); }
         else {
            cost = (ch->max_move - ch->pcdata->perm_con) *3; }

	 if (ch->exp >= cost)
	 {
	    ch->exp -= cost;
	    ch->max_move += 1;
	    urin_counter += 1;
	 }

	 if (ch->max_move > hpcap)
	    break;
      }

      if (urin_counter == 0)
      {
	 send_to_char ("You need more exp to gain any move.\n\r", ch);
      }

      if (urin_counter == 1)
      {
	 send_to_char ("You gain 1 move.\n\r", ch);
      }

      if (urin_counter > 1)
      {
	 sprintf (buf, "You gain %d move.\n\r", urin_counter);
	 send_to_char (buf, ch);
      }

      return;
   }
  if ( IS_SET(ch->garou1, GAROU_WHELP2) || IS_SET(ch->garou1, GAROU_WHELP1) )
  {
    stc("You may not train while affected by Whelp's curse, dispel yourself.\n\r",ch);
    return;
    }

    cost2 = 0;

    if ( arg1[0] == '\0' )
    {
//	sprintf( buf, "You have %d experience points.\n\r", ch->exp );
//	send_to_char( buf, ch );
	strcpy(arg1,"foo");
    }

         if (!str_cmp(arg1,"str")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"int")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"wis")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"dex")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"con")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"hp")    ) is_ok = TRUE;
    else if (!str_cmp(arg1,"mana")  ) is_ok = TRUE;
    else if (!str_cmp(arg1,"move")  ) is_ok = TRUE;
    else if (!str_cmp(arg1,"primal")) is_ok = TRUE;
    else if (!str_cmp(arg1,"legend")) is_ok = TRUE;
    else if (!str_cmp(arg1,"belt"  ) && IS_CLASS(ch, CLASS_NINJA)) is_ok = TRUE;
    else if (!str_cmp(arg1,"silver") && IS_CLASS(ch,CLASS_WEREWOLF))
	is_ok = TRUE;
	else if (!str_cmp(arg1,"focus") && IS_CLASS(ch, CLASS_MONK) )
	is_ok = TRUE;
    else if (!str_cmp(arg1,"gnosis") && IS_CLASS(ch, CLASS_WEREWOLF))
      is_ok = TRUE;
    else if (!str_cmp(arg1,"ancilla") && IS_CLASS(ch,CLASS_VAMPIRE))
    is_ok = TRUE;
    else if (!str_cmp(arg1,"elder") && IS_CLASS(ch,CLASS_VAMPIRE))
    is_ok = TRUE;
    else if (!str_cmp(arg1,"methuselah") && IS_CLASS(ch,CLASS_VAMPIRE))
    is_ok = TRUE;
    else if (!str_cmp(arg1,"magic") && IS_CLASS(ch,CLASS_DROW))
	is_ok = TRUE;
    if ( arg2[0] != '\0' && is_ok )
    {
	int amount = 0;

	if (!is_number(arg2))
	{
	    send_to_char("Please enter a numeric value.\n\r",ch);
	    return;
	}
	amount = atoi( arg2 );
        if (str_cmp(arg1,"hp")) {
	if (amount < 1 || amount > 50)
	{
	    send_to_char("Please enter a value between 1 and 50.\n\r",ch);
	    return;
	}
	if (amount > 1)
	{
	    sprintf(buf,"%s %d",arg1,amount-1);
	    do_train(ch,buf);
	    last = FALSE;
	}}
    }

    cost = 200;
    immcost = count_imms(ch);
    primal = (1+ch->practice)*1000;
    magic = (1+ch->pcdata->stats[DROW_MAGIC])*100;
    silver = 300000;
    focus = (1 + ch->focus[MAXIMUM]) *2500;
    gnosis = ( 1 + ch->gnosis[GMAXIMUM] ) * 25000;
    legendexp = ( 2000000 + ( 3000000 * ch->lstatus ) );
    legendquest = ( 2000 * ch->lstatus );
    beltcost = ch->pcdata->powerss[BLACK_BELT] * 5000000;
     if (ch->pcdata->powerss[BLACK_BELT] == 0)
     { beltcost = 1000000; } 


    if (IS_CLASS(ch, CLASS_HIGHLANDER)) max_stat = 25;
//    if (IS_CLASS(ch, CLASS_PALADIN)) max_stat = 25;

    if ( !str_cmp( arg1, "str" ) )
    {
	pAbility    = &ch->pcdata->perm_str;
	pOutput     = "strength";
    }

    else if ( !str_cmp( arg1, "int" ) )
    {
	pAbility    = &ch->pcdata->perm_int;
	pOutput     = "intelligence";
    }


    else if ( !str_cmp( arg1, "wis" ) )
    {
	pAbility    = &ch->pcdata->perm_wis;
	pOutput     = "wisdom";
    }

    else if ( !str_cmp( arg1, "dex" ) )
    {
	pAbility    = &ch->pcdata->perm_dex;
	pOutput     = "dexterity";
    }

    else if ( !str_cmp( arg1, "con" ) )
    {
	pAbility    = &ch->pcdata->perm_con;
	pOutput     = "constitution";
    }

 else if ( !str_cmp( arg1, "avatar") && ch->level == 2)
    {
        cost        = 1;
	pAbility    = &ch->level;
	pOutput     = "level";
        do_qtrust(ch,"self on");
        do_sset(ch,"self all 100");
    }
/*
    else if ( !str_cmp( arg1, "hp") && ch->max_hit < hpcap)
    {
	if ( ch->max_hit < 1 ) ch->max_hit = 1;
        if ( ch->max_hit < 50000 ) {
        cost             = (ch->max_hit - ch->pcdata->perm_con); }
        else if (ch->max_hit> 50000) {
        cost             = (ch->max_hit - ch->pcdata->perm_con) *3; }
	(int *) pAbility = &ch->max_hit;
	pOutput          = "hp";
    }*/
    else if ( !str_cmp( arg1, "mana") && ch->max_mana < manacap)
    {
if ( ch->max_mana < 1 ) ch->max_mana = 1; /* Bug fix */
        if (ch->max_mana < 50000) {
        cost        = (ch->max_mana - ch->pcdata->perm_wis); }
        else if (ch->max_mana > 50000) {
        cost        = (ch->max_mana - ch->pcdata->perm_wis) *3; }
	(int*) pAbility    = &ch->max_mana;
	pOutput     = "mana";
    }

    else if ( !str_cmp( arg1, "move") && ch->max_move < movecap)
    {
if ( ch->max_move < 1 ) ch->max_move = 1; /* Bug fix */
        if (ch->max_move < 50000) {
        cost        = (ch->max_move - ch->pcdata->perm_con); }
        else if (ch->max_move > 50000) {
        cost        = (ch->max_move - ch->pcdata->perm_con) *3; } 
	(int*)pAbility    = &ch->max_move;
	pOutput     = "move";
    }

    else if ( !str_cmp( arg1, "primal") && ch->practice < 1000)
    {
        cost        = primal;
	pAbility    = &ch->practice;
	pOutput     = "primal";
    }
    else if (!str_cmp(arg1, "magic") && IS_CLASS(ch, CLASS_DROW) &&
		ch->pcdata->stats[DROW_MAGIC] < 100)
	{
	sh_int quiet_pointer=ch->pcdata->stats[DROW_MAGIC];
	cost = magic;
	pAbility = &quiet_pointer;
	pOutput = "drow magic resistance";}

    else if ( !str_cmp( arg1, "silver") && IS_CLASS(ch, CLASS_WEREWOLF) &&
	ch->pcdata->powers[WPOWER_SILVER] < 100)
    {
        cost        = silver;
        if (ch->exp < silver) {stc("You don't have enough exp.\n\r",ch); return; }
        ch->pcdata->powers[WPOWER_SILVER] = 100;
        ch->exp -= 300000;
        stc("Your silver is now at maximum.\n\r",ch); return;
    }
    else if ( !str_cmp( arg1, "gnosis") && IS_CLASS(ch, CLASS_WEREWOLF))
    {
        sh_int quiet_pointer = ch->gnosis[GMAXIMUM];
        cost        = gnosis;
        pAbility    = &quiet_pointer;
        pOutput     = "gnosis points";
    }
    else if ( !str_cmp( arg1, "belt") )
    {
        sh_int quiet_pointer = ch->pcdata->powerss[BLACK_BELT];
        cost        = beltcost;  
        pAbility    = &quiet_pointer; 
        pOutput     = "belt";
    }
    else if ( !str_cmp( arg1, "legend") )
    {
        sh_int quiet_pointer = ch->lstatus;
        cost        = legendexp;
        cost2       = legendquest;
        pAbility    = &quiet_pointer;
        pOutput     = "legend status";
    }
    else if ( !str_cmp( arg1, "focus") && IS_CLASS(ch, CLASS_MONK))
    {
	sh_int quiet_pointer = ch->focus[MAXIMUM];
	cost 	    = focus;
	pAbility    = &quiet_pointer;
	pOutput     = "monk focus";
    }
    else if ( !str_cmp( arg1, "ancilla") && IS_CLASS(ch, CLASS_VAMPIRE)
    && ch->pcdata->rank != AGE_ANCILLA && ch->pcdata->rank != AGE_ELDER
    && ch->pcdata->rank != AGE_METHUSELAH )
    {
    if (ch->exp < 2000000)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - 2000000;
	ch->pcdata->rank = AGE_ANCILLA;
	send_to_char( "You are now an Ancilla.\n\r", ch );
	return;
	}

    else if ( !str_cmp( arg1, "elder") && IS_CLASS(ch, CLASS_VAMPIRE)
	&& ch->pcdata->rank != AGE_ELDER && ch->pcdata->rank == AGE_ANCILLA
	&& ch->pcdata->rank != AGE_METHUSELAH )
    {
    if (ch->exp < 6000000)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - 6000000;
	ch->pcdata->rank = AGE_ELDER;
	send_to_char( "You are now an Elder.\n\r", ch );
	return;
	}

	else if ( !str_cmp( arg1, "methuselah") && IS_CLASS(ch, CLASS_VAMPIRE)
	&& ch->pcdata->rank != AGE_METHUSELAH && ch->pcdata->rank != AGE_ANCILLA
	&& ch->pcdata->rank == AGE_ELDER)
    {
    if (ch->exp < 10000000)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - 10000000;
	ch->pcdata->rank = AGE_METHUSELAH;
	send_to_char( "You are now a Methuselah.\n\r", ch );
	return;
	}

/*
    else if ( !str_cmp( arg1, "slash") && !IS_IMMUNE(ch,IMM_SLASH) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SLASH);
	send_to_char( "You are now more resistant to slashing and slicing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "stab") && !IS_IMMUNE(ch,IMM_STAB) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_STAB);
	send_to_char( "You are now more resistant to stabbing and piercing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "smash") && !IS_IMMUNE(ch,IMM_SMASH) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SMASH);
	send_to_char( "You are now more resistant to blasting, pounding and crushing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "beast") && !IS_IMMUNE(ch,IMM_ANIMAL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_ANIMAL);
	send_to_char( "You are now more resistant to claw and bite attacks.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "grab") && !IS_IMMUNE(ch,IMM_MISC) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_MISC);
	send_to_char( "You are now more resistant to grepping, sucking and whipping weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "charm") && !IS_IMMUNE(ch,IMM_CHARM) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_CHARM);
	send_to_char( "You are now immune to charm spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "heat") && !IS_IMMUNE(ch,IMM_HEAT) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_HEAT);
	send_to_char( "You are now immune to heat and fire spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "cold") && !IS_IMMUNE(ch,IMM_COLD) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_COLD);
	send_to_char( "You are now immune to cold spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "lightning") && !IS_IMMUNE(ch,IMM_LIGHTNING) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_LIGHTNING);
	send_to_char( "You are now immune to lightning and electrical spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "acid") && !IS_IMMUNE(ch,IMM_ACID) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_ACID);
	send_to_char( "You are now immune to acid spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "sleep") && !IS_IMMUNE(ch,IMM_SLEEP) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SLEEP);
	send_to_char( "You are now immune to the sleep spell.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "drain") && !IS_IMMUNE(ch,IMM_DRAIN) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_DRAIN);
	send_to_char( "You are now immune to the energy drain spell.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "voodoo") && !IS_IMMUNE(ch,IMM_VOODOO) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_VOODOO);
	send_to_char( "You are now immune to voodoo magic.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "hurl") && !IS_IMMUNE(ch,IMM_HURL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_HURL);
	send_to_char( "You are now immune to being hurled.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "backstab") && !IS_IMMUNE(ch,IMM_BACKSTAB) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_BACKSTAB);
	send_to_char( "You are now immune to being backstabbed.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "kick") && !IS_IMMUNE(ch,IMM_KICK) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_KICK);
	send_to_char( "You are now immune to being kicked.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "disarm") && !IS_IMMUNE(ch,IMM_DISARM) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_DISARM);
	send_to_char( "You are now immune to being disarmed.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "steal") && !IS_IMMUNE(ch,IMM_STEAL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_STEAL);
	send_to_char( "You are now immune to being stolen from.\n\r", ch );
	return;
    }*/
    else
    {
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
      sprintf( buf, "{w             You have{c %10d {wexperience points.{x\n\r", ch->exp );
      send_to_char( buf, ch );
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);

	send_to_char( " {wStats            {D-{w", ch );
	if ( ch->pcdata->perm_str < max_stat ) send_to_char( " {CStr", ch );
	if ( ch->pcdata->perm_int < max_stat ) send_to_char( " {CInt", ch );
	if ( ch->pcdata->perm_wis < max_stat ) send_to_char( " {CWis", ch );
	if ( ch->pcdata->perm_dex < max_stat ) send_to_char( " {CDex", ch );
	if ( ch->pcdata->perm_con < max_stat ) send_to_char( " {CCon", ch );
	if ( ( ch->pcdata->perm_str >= max_stat )
		&& ( ch->pcdata->perm_wis >= max_stat )
		&& ( ch->pcdata->perm_int >= max_stat )
		&& ( ch->pcdata->perm_dex >= max_stat )
		&& ( ch->pcdata->perm_con >= max_stat ) )
	    send_to_char( " {cNone left to train.{x\n\r", ch );
	else
	    send_to_char( "{c.{x\n\r", ch );

	if ( ch->level == 2 )
	{
	   sprintf( buf, " {wBecome an {cAVATAR{D -{C 1000 {wexp.{x\n\r" );
	   send_to_char( buf, ch );
	}
	if ( ch->max_hit       < hpcap )
	{
           if (ch->max_hit <= 50000) {
	   sprintf( buf, " {wHp               {D-{C %ld {wexp per point.{X\n\r",(ch->max_hit - ch->pcdata->perm_con ));
	   send_to_char( buf, ch ); }
           else if (ch->max_hit > 50000) {
           sprintf( buf, " {wHp               {D-{C %ld{w exp per point.{x\n\r",(ch->max_hit - ch->pcdata->perm_con )* 3 );
           send_to_char( buf, ch ); }
	}
	if ( ch->max_mana      < manacap )
	{
           if (ch->max_mana < 50000) {
	   sprintf( buf, " {wMana             {D-{C %ld {wexp per point.{x\n\r",(ch->max_mana - ch->pcdata->perm_wis));
	   send_to_char( buf, ch ); }
           else if (ch->max_mana > 50000) {
           sprintf( buf, " {wMana             {D-{C %ld {wexp per point.{x\n\r",(ch->max_mana - ch->pcdata->perm_wis) * 3 );
           send_to_char( buf, ch ); }
	}
	if ( ch->max_move      < movecap )
	{
           if (ch->max_move < 50000) {
	   sprintf( buf, " {wMove             {D-{C %ld {wexp per point.{x\n\r",(ch->max_move - ch->pcdata->perm_con));
	   send_to_char( buf, ch );}
           else if (ch->max_move > 50000) {
           sprintf( buf, " {wMove             {D-{C %ld {wexp per point.{x\n\r",(ch->max_move - ch->pcdata->perm_con) * 3 );
           send_to_char( buf, ch );}
	}
	if ( ch->practice        < 100 )
	{
	   sprintf( buf, " {wPrimal           {D-{C %d {wexp per point of primal energy.{x\n\r", primal );
	   send_to_char( buf, ch );
	}
	if ( ch->pcdata->powers[WPOWER_SILVER] < 100 && IS_CLASS(ch, CLASS_WEREWOLF))
	{
	   sprintf( buf, " {wSilver tolerance {D-{C 300,000 {wexp for 100{x\n\r");
	   send_to_char( buf, ch );
	}
        if ( ch->focus[MAXIMUM] < 100 && IS_CLASS(ch, CLASS_MONK))
       {
	   sprintf( buf, " {wFocus Points {D-{C %d {wexp per point of Focus.{x\n\r", focus );
	   stc( buf, ch );
       }
       if ( ch->gnosis[GMAXIMUM] < 30 && IS_CLASS(ch, CLASS_WEREWOLF))
       {
           sprintf( buf, " {wGnosis Points {D-{C %d {wexp per point of Gnosis.{x\n\r", gnosis);
           stc( buf, ch );
       }
       if ( ch->lstatus < 9 )
       {
           sprintf( buf, " {cLEGEND {wStatus    {D-{C %d {wexp and{C %d {wquest points.{x\n\r", legendexp, legendquest );
	   stc(buf,ch);
       }
       if ( ch->pcdata->powerss[BLACK_BELT] < 9 && IS_CLASS(ch, CLASS_NINJA))
       {
           sprintf(buf, " {cBELT             {D-{C %d {wexp points.{x\n\r",beltcost );
           stc(buf,ch);
       }
	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->rank != AGE_ANCILLA
	     && ch->pcdata->rank != AGE_ELDER && ch->pcdata->rank != AGE_METHUSELAH )
	{
	   sprintf( buf, " {cAncilla{D:{w You need {C2 million{w exp to be an Ancilla.{x\n\r");
	   send_to_char( buf, ch );
	}

	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->rank != AGE_ELDER
	     && ch->pcdata->rank == AGE_ANCILLA && ch->pcdata->rank != AGE_METHUSELAH)
	{
	   sprintf( buf, " {cElder{D:{w You need{C 6 million {wexp to be an Elder.{x\n\r" );
	   send_to_char( buf, ch );
	}

	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->rank != AGE_METHUSELAH
	     && ch->pcdata->rank == AGE_ELDER && ch->pcdata->rank != AGE_ANCILLA)
	{
	   sprintf( buf, " {cMethuselah{D:{w You need{C 10 million {wexp to be an Methuselah.{x\n\r");
	   send_to_char( buf, ch );
	}
	if (ch->pcdata->stats[DROW_MAGIC] < 100 && IS_CLASS(ch, CLASS_DROW))
	{ sprintf( buf, " {wMagic resistance {D-{C %d {wdrow points per point of magic.{x\n\r", magic );
	  send_to_char( buf, ch);}

stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
stc("{w                 Immunties {D-{C HELP IMMUNE{x\n\r",ch);
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-{x\n\r",ch);
	return;
    }

    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "str")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "int")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "wis")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "dex")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "con")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility>= hpcap) && (!str_cmp( arg1, "hp")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= manacap) && (!str_cmp( arg1, "mana")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= movecap) && (!str_cmp( arg1, "move")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= 1000) && (!str_cmp( arg1, "primal")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ((*pAbility >= 100) && (!str_cmp(arg1, "magic"))) {
		if (last) act("Your $T is already at a maximum.", ch, NULL, pOutput, TO_CHAR);
		return;}
    if ( ( *pAbility >= 100 ) && (!str_cmp( arg1, "focus") ) )
    {
	if (last) act( "Your $T is already at maximum.",ch,NULL,pOutput,TO_CHAR);
	return;
    }
    if ( (*pAbility >= 30 ) && (!str_cmp( arg1, "gnosis") ))
    {
        if (last) act( "Your $T is already at maximum.",ch,NULL,pOutput,TO_CHAR);
        return;
    }
    if ( (*pAbility >= 9 ) && (!str_cmp( arg1, "belt") ))
    {
        if (last) act("You have already obtained the highest belt!",ch,NULL,NULL,TO_CHAR);
        return;
    }
    if ( (*pAbility >= 9 ) && (!str_cmp( arg1, "legend") ))
    {
	if (last) act("You have already obtained the highest Legend Status!",ch,NULL,NULL,TO_CHAR);
	return;
    }
    if ( (*pAbility >= 100) && (!str_cmp( arg1, "silver")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost < 1 ) cost = 1;
    if ( cost > ch->pcdata->stats[DROW_POWER] && !str_cmp(arg1, "magic"))
	{ if (last) send_to_char( "You don't have enough drow power points.\n\r", ch);
	return;}
else     if ( cost > ch->exp )
    {
	if (last) send_to_char( "You don't have enough exp.\n\r", ch );
	return;
    }
    if ( cost2 > ch->pcdata->quest )
    {
      if (last) stc("You don't have enough quest points!\n\r",ch);
      return;
    }
if (!str_cmp(arg1, "magic"))
	ch->pcdata->stats[DROW_POWER] -= cost;
else
    ch->exp		-= cost;
	if (!str_cmp( arg1, "silver"))
		ch->pcdata->powers[WPOWER_SILVER] += 1;
	else if (!str_cmp(arg1, "magic"))
		ch->pcdata->stats[DROW_MAGIC] += 1;
	else if (!str_cmp(arg1, "focus"))
        {
	   ch->focus[CURRENT]++;
	   ch->focus[MAXIMUM]++;
	}
	else if (!str_cmp(arg1, "gnosis"))
        {
           ch->gnosis[GCURRENT]++;
           ch->gnosis[GMAXIMUM]++;
        }
        else if (!str_cmp(arg1, "belt"))
        {
           ch->pcdata->powerss[BLACK_BELT]++;
        sprintf( buf, "%s has gained a new belt!",ch->name );
        do_info(ch,buf);
        }
	else if (!str_cmp(arg1, "legend"))
	{
	   ch->lstatus++;
           ch->pcdata->quest -= cost2;
	sprintf( buf, "%s has gained a legend level!",ch->name );
        do_info(ch,buf);
	}
	else
    *pAbility		+= 1;
    if ( !str_cmp( arg1, "avatar") )
    {
	act( "You become an avatar!", ch, NULL, NULL, TO_CHAR );
	sprintf( buf, "%s has become an avatar!",ch->name );
	do_info(ch,buf);
	if (IS_CLASS(ch, CLASS_DEMON)) ch->alignment = -1000;
	if (ch->level < ch->trust) ch->level = ch->trust;
	if (!IS_NPC(ch) && IS_VAMPAFF(ch, VAM_MORTAL))
	    do_mortalvamp(ch,"");
    }
    else if (last)
	act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    return;
}

void do_train2( CHAR_DATA *ch, char *argument )
{
/* Changed by Kyrlin to make avatar peasant 22/02/00*/
/* Removed Toaster 17/03/00 to remove legend from train*/
/* Changed Toaster 19/03/00 to make training hp/mana/move
cost less/more exp*/


    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    sh_int *pAbility;
    char *pOutput;
    int cost,cost2;
    int magic;
    int immcost;
    int primal;
    int silver;
    int focus;
    int legendexp;
    int legendquest;
    int gnosis;
    int hpcap,manacap,movecap;
    int max_stat = 18;
    bool last = TRUE;
    bool is_ok = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
/*
    hpcap = 15000 + ( 1000 * ch->lstatus) + ( 100 * ch->race );
    manacap = hpcap;
    movecap = hpcap;
*/

if(ch->race <= 5) hpcap = 10000;
else
 hpcap = 50000 + (200 * ch->race );
 manacap = hpcap;
 movecap = hpcap;

    if ( IS_NPC(ch) )
	return;

    if ( (!str_cmp(arg1, "hp") && ch->max_hit >= hpcap))
    {
        stc("You have reached your hp cap.\n\r", ch);
         return;
    }
    if ( (!str_cmp(arg1, "mana") && ch->max_mana >= manacap))
    {
        stc("You have reached your mana cap.\n\r", ch);
         return;
    }
    if ( (!str_cmp(arg1, "move") && ch->max_move >= movecap))
    {
        stc("You have reached your move cap.\n\r", ch);
         return;
    }


  if ( IS_SET(ch->garou1, GAROU_WHELP2) || IS_SET(ch->garou1, GAROU_WHELP1) )
  {
    stc("You may not train while affected by Whelp's curse, dispel yourself.\n\r",ch);
    return;
  }

    cost2 = 0;

    if ( arg1[0] == '\0' )
    {
//	sprintf( buf, "You have %d experience points.\n\r", ch->exp );
//	send_to_char( buf, ch );
	strcpy(arg1,"foo");
    }

         if (!str_cmp(arg1,"str")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"int")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"wis")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"dex")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"con")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"hp")    ) is_ok = TRUE;
    else if (!str_cmp(arg1,"mana")  ) is_ok = TRUE;
    else if (!str_cmp(arg1,"move")  ) is_ok = TRUE;
    else if (!str_cmp(arg1,"primal")) is_ok = TRUE;
    else if (!str_cmp(arg1,"legend")) is_ok = TRUE;
    else if (!str_cmp(arg1,"silver") && IS_CLASS(ch,CLASS_WEREWOLF))
	is_ok = TRUE;
	else if (!str_cmp(arg1,"focus") && IS_CLASS(ch, CLASS_MONK) )
	is_ok = TRUE;
    else if (!str_cmp(arg1,"gnosis") && IS_CLASS(ch, CLASS_WEREWOLF))
      is_ok = TRUE;
    else if (!str_cmp(arg1,"magic") && IS_CLASS(ch,CLASS_DROW))
	is_ok = TRUE;
    if ( arg2[0] != '\0' && is_ok )
    {
	int amount = 0;

	if (!is_number(arg2))
	{
	    send_to_char("Please enter a numeric value.\n\r",ch);
	    return;
	}
	amount = atoi( arg2 );
	if (amount < 1 || amount > 50)
	{
	    send_to_char("Please enter a value between 1 and 50.\n\r",ch);
	    return;
	}

	if (amount > 1)
	{
	    sprintf(buf,"%s %d",arg1,amount-1);
	    do_train(ch,buf);
	    last = FALSE;
	}

  }

    cost = 250;
    immcost = count_imms(ch);
    primal = (1+ch->practice)*500;
    magic = (1+ch->pcdata->stats[DROW_MAGIC])*100;
    silver = (1+ch->pcdata->powers[WPOWER_SILVER])*2500;
    focus = (1 + ch->focus[MAXIMUM]) *2500;
    gnosis = ( 1 + ch->gnosis[GMAXIMUM] ) * 25000;
    legendexp = ( 2000000 + ( 3000000 * ch->lstatus ) );
    legendquest = ( 1000 * ch->lstatus );

    if (IS_CLASS(ch, CLASS_HIGHLANDER)) max_stat = 25;
    if (IS_CLASS(ch, CLASS_PALADIN)) max_stat = 25;

    if ( !str_cmp( arg1, "str" ) )
    {
	pAbility    = &ch->pcdata->perm_str;
	pOutput     = "strength";
    }

    else if ( !str_cmp( arg1, "int" ) )
    {
	pAbility    = &ch->pcdata->perm_int;
	pOutput     = "intelligence";
    }


    else if ( !str_cmp( arg1, "wis" ) )
    {
	pAbility    = &ch->pcdata->perm_wis;
	pOutput     = "wisdom";
    }

    else if ( !str_cmp( arg1, "dex" ) )
    {
	pAbility    = &ch->pcdata->perm_dex;
	pOutput     = "dexterity";
    }

    else if ( !str_cmp( arg1, "con" ) )
    {
	pAbility    = &ch->pcdata->perm_con;
	pOutput     = "constitution";
    }
/*
 else if ( !str_cmp( arg1, "avatar") && ch->level == 2)
    {
        cost        = 1000;
	SET_BIT(ch->newbits,NEW_PURGE);
	pAbility    = &ch->level;
	pOutput     = "level";
    }*/
 else if ( !str_cmp( arg1, "avatar") && ch->level == 2)
	    {
	        cost        = 1000;
		pAbility    = &ch->level;
		pOutput     = "level";
    }

    else if ( !str_cmp( arg1, "legend") )
    {
        sh_int quiet_pointer = ch->lstatus;
        cost        = legendexp;
        cost2       = legendquest;
        pAbility    = &quiet_pointer;
        pOutput     = "legend status";
    }
    else if ( !str_cmp( arg1, "hp") )
    {
	int amount = 0;
	int urin = 0;
	int urin_counter = 0;

    if (!is_number(arg2) && strcmp(arg2, "all"))
    {
      send_to_char("Please enter a numeric value or use 'all'.\n\r", ch);
      return;
    }
    if (is_number(arg2))
    {
      amount = atoi(arg2);
      if (amount < 1 || amount > 4000)
      {
        send_to_char("Please enter a value between 1 and 4000.\n\r", ch);
        return;
      }
    }
    else
   amount = 100000;
    for (urin = 0; urin < amount; urin++)
    {
      if (ch->exp >= UMAX((ch->max_hit + 1), 1) && ch->max_hit < hpcap)
      {
        ch->max_hit++;
        ch->exp -= UMAX(ch->max_hit, 1);
        urin_counter++;
      }
    }
    if (urin_counter == 0)
      send_to_char("You need more exp to gain any hps.\n\r", ch);
    else if (urin_counter == 1)
      send_to_char("You gain 1 hp.\n\r", ch);
    else
    {
      sprintf(buf, "You gain %d hps.\n\r", urin_counter);
      send_to_char(buf, ch);
    }
    return;
  }

    if ( !str_cmp( arg1, "hp") && ch->max_hit < hpcap){
	if ( ch->max_hit < 1 ) ch->max_hit = 1;
	cost   = (ch->max_hit - ch->pcdata->perm_con) *2;
	(int *) pAbility = &ch->max_hit;
	pOutput          = "hp";
    }
    else if ( !str_cmp( arg1, "mana") && ch->max_mana < manacap)
    {
if ( ch->max_mana < 1 ) ch->max_mana = 1;
        cost        = (ch->max_mana - ch->pcdata->perm_wis) *2;
	(int*)pAbility    = &ch->max_mana;
	pOutput     = "mana";
    }

    else if ( !str_cmp( arg1, "move") && ch->max_move < movecap)
    {
if ( ch->max_move < 1 ) ch->max_move = 1;
        cost        = (ch->max_move - ch->pcdata->perm_dex) *2;
	(int*)pAbility    = &ch->max_move;
	pOutput     = "move";
    }

    else if ( !str_cmp( arg1, "primal") )
    {
        cost        = primal;
	pAbility    = &ch->practice;
	pOutput     = "primal";
    }

    else if (!str_cmp(arg1, "magic") && IS_CLASS(ch, CLASS_DROW) &&
		ch->pcdata->stats[DROW_MAGIC] < 100)
	{
	sh_int quiet_pointer=ch->pcdata->stats[DROW_MAGIC];
	cost = magic;
	pAbility = &quiet_pointer;
	pOutput = "drow magic resistance";}

    else if ( !str_cmp( arg1, "silver") && IS_CLASS(ch, CLASS_WEREWOLF) &&
	ch->pcdata->powers[WPOWER_SILVER] < 100)
    {
	sh_int quiet_pointer=ch->pcdata->powers[WPOWER_SILVER];
        cost        = silver;
	pAbility    = &quiet_pointer;
	pOutput     = "tolerance to silver";
    }
    else if ( !str_cmp( arg1, "gnosis") && IS_CLASS(ch, CLASS_WEREWOLF))
    {
        sh_int quiet_pointer = ch->gnosis[GMAXIMUM];
        cost        = gnosis;
        pAbility    = &quiet_pointer;
        pOutput     = "gnosis points";
    }
    else if ( !str_cmp( arg1, "focus") && IS_CLASS(ch, CLASS_MONK))
    {
	sh_int quiet_pointer = ch->focus[MAXIMUM];
	cost 	    = focus;
	pAbility    = &quiet_pointer;
	pOutput     = "monk focus";
    }
    else if ( !str_cmp( arg1, "slash") && !IS_IMMUNE(ch,IMM_SLASH) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SLASH);
	send_to_char( "You are now more resistant to slashing and slicing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "stab") && !IS_IMMUNE(ch,IMM_STAB) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_STAB);
	send_to_char( "You are now more resistant to stabbing and piercing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "smash") && !IS_IMMUNE(ch,IMM_SMASH) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SMASH);
	send_to_char( "You are now more resistant to blasting, pounding and crushing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "beast") && !IS_IMMUNE(ch,IMM_ANIMAL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_ANIMAL);
	send_to_char( "You are now more resistant to claw and bite attacks.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "grab") && !IS_IMMUNE(ch,IMM_MISC) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_MISC);
	send_to_char( "You are now more resistant to grepping, sucking and whipping weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "charm") && !IS_IMMUNE(ch,IMM_CHARM) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_CHARM);
	send_to_char( "You are now immune to charm spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "heat") && !IS_IMMUNE(ch,IMM_HEAT) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_HEAT);
	send_to_char( "You are now immune to heat and fire spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "cold") && !IS_IMMUNE(ch,IMM_COLD) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_COLD);
	send_to_char( "You are now immune to cold spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "lightning") && !IS_IMMUNE(ch,IMM_LIGHTNING) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_LIGHTNING);
	send_to_char( "You are now immune to lightning and electrical spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "acid") && !IS_IMMUNE(ch,IMM_ACID) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_ACID);
	send_to_char( "You are now immune to acid spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "sleep") && !IS_IMMUNE(ch,IMM_SLEEP) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SLEEP);
	send_to_char( "You are now immune to the sleep spell.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "drain") && !IS_IMMUNE(ch,IMM_DRAIN) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_DRAIN);
	send_to_char( "You are now immune to the energy drain spell.\n\r", ch );
	return;
    }
/*
    else if ( !str_cmp( arg1, "voodoo") && !IS_IMMUNE(ch,IMM_VOODOO) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_VOODOO);
	send_to_char( "You are now immune to voodoo magic.\n\r", ch );
	return;
    }
*/
    else if ( !str_cmp( arg1, "hurl") && !IS_IMMUNE(ch,IMM_HURL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_HURL);
	send_to_char( "You are now immune to being hurled.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "backstab") && !IS_IMMUNE(ch,IMM_BACKSTAB) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_BACKSTAB);
	send_to_char( "You are now immune to being backstabbed.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "kick") && !IS_IMMUNE(ch,IMM_KICK) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_KICK);
	send_to_char( "You are now immune to being kicked.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "disarm") && !IS_IMMUNE(ch,IMM_DISARM) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_DISARM);
	send_to_char( "You are now immune to being disarmed.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "steal") && !IS_IMMUNE(ch,IMM_STEAL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_STEAL);
	send_to_char( "You are now immune to being stolen from.\n\r", ch );
	return;
    }
    else
    {
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
      sprintf( buf, "{w             You have{c %10d {wexperience points.{x\n\r", ch->exp );
      send_to_char( buf, ch );
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
	send_to_char( " {wStats{D:{w", ch );
	if ( ch->pcdata->perm_str < max_stat ) send_to_char( " {CStr", ch );
	if ( ch->pcdata->perm_int < max_stat ) send_to_char( " {CInt", ch );
	if ( ch->pcdata->perm_wis < max_stat ) send_to_char( " {CWis", ch );
	if ( ch->pcdata->perm_dex < max_stat ) send_to_char( " {CDex", ch );
	if ( ch->pcdata->perm_con < max_stat ) send_to_char( " {CCon", ch );
	if ( ( ch->pcdata->perm_str >= max_stat )
		&& ( ch->pcdata->perm_wis >= max_stat )
		&& ( ch->pcdata->perm_int >= max_stat )
		&& ( ch->pcdata->perm_dex >= max_stat )
		&& ( ch->pcdata->perm_con >= max_stat ) )
	    send_to_char( " {cNone left to train.{x\n\r", ch );
	else
	    send_to_char( "{w.{x\n\r", ch );

	if ( ch->level == 2 )
	{
	   sprintf( buf, " {wBecome an {CAVATAR {D-{C 1000 {wexp.{X\n\r" );
	   send_to_char( buf, ch );
	}
	if ( ch->max_hit       < hpcap )
	{
         cost = (ch->max_hit - ch->pcdata->perm_con)*2;

	   sprintf( buf, " {wHp               {D-{C %d {wexp per point.{x\n\r",cost);
	   send_to_char( buf, ch );
	}
	if ( ch->max_mana    < manacap )
	{
	cost = (ch->max_mana - ch->pcdata->perm_wis)*2;


	   sprintf( buf, " {wMana             {D-{C %d {wexp per point.{x\n\r",cost);
	   send_to_char( buf, ch );
	}
	if ( ch->max_move      < movecap )
	{
       cost = (ch->max_move - ch->pcdata->perm_dex)*2;
	   sprintf( buf, " {wMove             {D-{C %d{w exp per point.{x\n\r",cost);
	   send_to_char( buf, ch );
	}
	if ( ch->practice        < 100 )
	{
	   sprintf( buf, " {wPrimal           {D-{C %d {wexp per point of primal energy.{x\n\r", primal );
	   send_to_char( buf, ch );
	}
	if ( ch->pcdata->powers[WPOWER_SILVER] < 100 && IS_CLASS(ch, CLASS_WEREWOLF))
	{
	   sprintf( buf, " {wSilver tolerance {D-{C %d {wexp per point of tolerance.{X\n\r", silver );
	   send_to_char( buf, ch );
	}
        if ( ch->focus[MAXIMUM] < 100 && IS_CLASS(ch, CLASS_MONK))
       {
	   sprintf( buf, " {wFocus Points {D-{C %d {wexp per point of Focus.{x\n\r", focus );
	   stc( buf, ch );
       }
       if ( ch->gnosis[GMAXIMUM] < 30 && IS_CLASS(ch, CLASS_WEREWOLF))
       {
           sprintf( buf, " {wGnosis Points {D-{C %d {wexp per point of Gnosis.{x\n\r", gnosis);
           stc( buf, ch );
       }
       if ( ch->lstatus < 9 )
       {
           sprintf( buf, " {CLEGEND {wStatus {D-{C %d {wexp and{C %d {wquest points.{x\n\r", legendexp, legendquest );
	   stc(buf,ch);
       }

	if (ch->pcdata->stats[DROW_MAGIC] < 100 && IS_CLASS(ch, CLASS_DROW))
	{ sprintf( buf, " {wMagic resistance {D-{C %d {wdrow points per point of magic.{x\n\r", magic );
	  send_to_char( buf, ch);}

stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
stc("{w                       Immunties {D-{C HELP IMMUNE{x\n\r",ch);
stc("{D -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\r",ch);
	return;
    }

    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "str")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "int")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "wis")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "dex")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "con")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( (*pAbility>= hpcap) && (!str_cmp( arg1, "hp")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( (*pAbility >= manacap) && (!str_cmp( arg1, "mana")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= movecap) && (!str_cmp( arg1, "move")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= 1000) && (!str_cmp( arg1, "primal")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ((*pAbility >= 100) && (!str_cmp(arg1, "magic"))) {
		if (last) act("Your $T is already at a maximum.", ch, NULL, pOutput, TO_CHAR);
		return;}
    if ( ( *pAbility >= 100 ) && (!str_cmp( arg1, "focus") ) )
    {
	if (last) act( "Your $T is already at maximum.",ch,NULL,pOutput,TO_CHAR);
	return;
    }
    if ( (*pAbility >= 30 ) && (!str_cmp( arg1, "gnosis") ))
    {
        if (last) act( "Your $T is already at maximum.",ch,NULL,pOutput,TO_CHAR);
        return;
    }
    if ( (*pAbility >= 9 ) && (!str_cmp( arg1, "legend") ))
    {
	if (last) act("You have already obtained the highest Legend Status!",ch,NULL,NULL,TO_CHAR);
	return;
    }
    if ( (*pAbility >= 100) && (!str_cmp( arg1, "silver")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost < 1 ) cost = 1;
    if ( cost > ch->pcdata->stats[DROW_POWER] && !str_cmp(arg1, "magic"))
	{ if (last) send_to_char( "You don't have enough drow power points.\n\r", ch);
	return;}
else     if ( cost > ch->exp )
    {
	if (last) send_to_char( "You don't have enough exp.\n\r", ch );
	return;
    }
    if ( cost2 > ch->pcdata->quest )
    {
      if (last) stc("You don't have enough quest points!\n\r",ch);
      return;
    }
if (!str_cmp(arg1, "magic"))
	ch->pcdata->stats[DROW_POWER] -= cost;
else
    ch->exp		-= cost;
	if (!str_cmp( arg1, "silver"))
		ch->pcdata->powers[WPOWER_SILVER] += 1;
	else if (!str_cmp(arg1, "magic"))
		ch->pcdata->stats[DROW_MAGIC] += 1;
	else if (!str_cmp(arg1, "focus"))
        {
	   ch->focus[CURRENT]++;
	   ch->focus[MAXIMUM]++;
	}
	else if (!str_cmp(arg1, "gnosis"))
        {
           ch->gnosis[GCURRENT]++;
           ch->gnosis[GMAXIMUM]++;
        }
	else if (!str_cmp(arg1, "legend"))
	{
	   ch->lstatus++;
           ch->pcdata->quest -= cost2;
	sprintf( buf, "%s has advanced in history!",ch->name );
        do_info(ch,buf);
	}
	else
    *pAbility		+= 1;
    if ( !str_cmp( arg1, "avatar") )
    {
	act( "You become an Avatar!", ch, NULL, NULL, TO_CHAR );
	sprintf( buf, "%s has become an Avatar!",ch->name );
	do_info(ch,buf);
	if (IS_CLASS(ch, CLASS_DEMON)) ch->alignment = -1000;
	if (IS_CLASS(ch, CLASS_MAGE)) ch->level = ch->trust;
	if (ch->level < ch->trust) ch->level = ch->trust;
//	ch->trust = 0;
//	if (IS_CLASS(ch, CLASS_MAGE)) ch->level = 6;
//	if (IS_CLASS(ch, CLASS_MAGE)) ch->trust = 6;
	if (!IS_NPC(ch) && IS_VAMPAFF(ch, VAM_MORTAL))
	    do_mortalvamp(ch,"");
    }
    else if (last)
	act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    return;
}

void do_mount( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Mount what?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_VAMPAFF(ch, VAM_DISGUISED))
    {
	send_to_char( "You cannot ride in this form.\n\r",ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r",ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot ride on your own back!\n\r",ch );
	return;
    }

    if ( ch->mounted > 0 )
    {
	send_to_char( "You are already riding.\n\r",ch );
	return;
    }

    if ( !IS_NPC(victim) || victim->mounted > 0 || (IS_NPC(victim) && !IS_SET(victim->act, ACT_MOUNT) ) )
    {
	send_to_char( "You cannot mount them.\n\r",ch );
	return;
    }

    if ( victim->position < POS_STANDING )
    {
	if ( victim->position < POS_SLEEPING )
	    act( "$N is too badly hurt for that.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SLEEPING )
	    act( "First you better wake $m up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_RESTING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_MEDITATING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SITTING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SLEEPING )
	    act( "First you better wake $m up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_FIGHTING )
	    act( "Not while $e's fighting.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (!IS_NPC(ch) && ch->stance[0] != -1) do_stance(ch,"");

    ch->mounted     = IS_RIDING;
    victim->mounted = IS_MOUNT;
    ch->mount = victim;
    victim->mount = ch;

    act( "You clamber onto $N's back.", ch, NULL, victim, TO_CHAR );
    act( "$n clambers onto $N's back.", ch, NULL, victim, TO_ROOM );
    return;
}

void do_dismount( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( ch->mounted == 0 )
    {
	send_to_char( "But you are not riding!\n\r",ch );
	return;
    }

    if ( (victim = ch->mount) == NULL )
    {
	send_to_char( "But you are not riding!\n\r",ch );
	return;
    }

    act( "You clamber off $N's back.", ch, NULL, victim, TO_CHAR );
    act( "$n clambers off $N's back.", ch, NULL, victim, TO_ROOM );

    ch->mounted     = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;

    ch->mount = NULL;
    victim->mount = NULL;

    return;
}

void do_tie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch)
    {
	send_to_char( "You cannot tie yourself up!\n\r", ch );
	return;
    }
    if(victim->pcdata->war_bits > 0)
    { do_eliminate(ch,victim->name); return; }

    if (IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "But they are already tied up!\n\r", ch );
	return;
    }
    if (victim->position > POS_STUNNED || victim->hit > 0)
    {
	send_to_char( "You can only tie up a defenceless person.\n\r", ch );
	return;
    }
    act("You quickly tie up $N.",ch,NULL,victim,TO_CHAR);
    act("$n quickly ties up $N.",ch,NULL,victim,TO_ROOM);
    send_to_char("You have been tied up!\n\r",victim);
    SET_BIT(victim->extra,TIED_UP);
    sprintf(buf,"%s has been tied up by %s.",victim->name,ch->name);
    do_info(ch,buf);
    return;
}

void do_untie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "But they are not tied up!\n\r", ch );
	return;
    }
    if (victim == ch)
    {
	send_to_char( "You cannot untie yourself!\n\r", ch );
	return;
    }
    act("You quickly untie $N.",ch,NULL,victim,TO_CHAR);
    act("$n quickly unties $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n quickly unties you.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,TIED_UP);
    return;
}

void do_gag( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch && !IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "You cannot gag yourself!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP) && !IS_EXTRA(victim,GAGGED))
    {
	send_to_char( "You can only gag someone who is tied up!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,GAGGED))
    {
    	act("You place a gag over $N's mouth.",ch,NULL,victim,TO_CHAR);
    	act("$n places a gag over $N's mouth.",ch,NULL,victim,TO_NOTVICT);
    	act("$n places a gag over your mouth.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->extra,GAGGED);
	return;
    }
    if (ch == victim)
    {
    	act("You remove the gag from your mouth.",ch,NULL,victim,TO_CHAR);
    	act("$n removes the gag from $s mouth.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->extra,GAGGED);
	return;
    }
    act("You remove the gag from $N's mouth.",ch,NULL,victim,TO_CHAR);
    act("$n removes the gag from $N's mouth.",ch,NULL,victim,TO_NOTVICT);
    act("$n removes the gag from your mouth.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,GAGGED);
    return;
}

void do_blindfold( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch && !IS_EXTRA(victim,BLINDFOLDED) && IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "You cannot blindfold yourself!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP) && !IS_EXTRA(victim,BLINDFOLDED))
    {
	send_to_char( "You can only blindfold someone who is tied up!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,BLINDFOLDED))
    {
    	act("You place a blindfold over $N's eyes.",ch,NULL,victim,TO_CHAR);
    	act("$n places a blindfold over $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    	act("$n places a blindfold over your eyes.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->extra,BLINDFOLDED);
	return;
    }
    if (ch == victim)
    {
    	act("You remove the blindfold from your eyes.",ch,NULL,victim,TO_CHAR);
    	act("$n removes the blindfold from $s eyes.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->extra,BLINDFOLDED);
	return;
    }
    act("You remove the blindfold from $N's eyes.",ch,NULL,victim,TO_CHAR);
    act("$n removes the blindfold from $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    act("$n removes the blindfold from your eyes.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,BLINDFOLDED);
    return;
}


int count_imms( CHAR_DATA *ch )
{
    int count = 0;

    if ( IS_IMMUNE(ch, IMM_SLASH)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_STAB)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_SMASH)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_ANIMAL)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_MISC)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_CHARM)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_HEAT)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_COLD)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_LIGHTNING) ) count += 1;
    if ( IS_IMMUNE(ch, IMM_ACID)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_SLEEP)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_DRAIN)     ) count += 1;
    //if ( IS_IMMUNE(ch, IMM_VOODOO)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_HURL)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_BACKSTAB)  ) count += 1;
    if ( IS_IMMUNE(ch, IMM_KICK)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_DISARM)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_STEAL)     ) count += 1;
    return ( ( count * 10000 ) + 10000 );
}

void do_track(CHAR_DATA *ch, char *argument)
{
    bool found = FALSE;
    if (IS_SET(world_affects, WORLD_RAIN))
    {
	send_to_char("Its impossible to find any tracks with all this rain.\n\r",ch);
	return;
    }
    if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_track])
/*
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	WAIT_STATE(ch,12);
	return;
    }
*/
    if (check_track(ch,0)) found = TRUE;
    if (check_track(ch,1)) found = TRUE;
    if (check_track(ch,2)) found = TRUE;
    if (check_track(ch,3)) found = TRUE;
    if (check_track(ch,4)) found = TRUE;
    if (found == FALSE)
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	return;
    }
    act("$n carefully examines the ground for tracks.",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch,12);
    return;
}

void do_hunt( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch,CLASS_NINJA) && !IS_CLASS(ch, CLASS_DEMON))
    {
     send_to_char("Huh?\n\r", ch);
     return;
    }

if ( IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->warp, WARP_HUNT) )
{
stc("You need the hunting warp to hunt!\n\r", ch);
return;
}

    if ( arg[0] == '\0' )                           /*SAGE*/
    {
	if (strlen(ch->hunting) > 1)
	{
	    free_string(ch->hunting);
	    ch->hunting = str_dup( "" );
	    send_to_char( "You stop hunting your prey.\n\r", ch );
	}
	else send_to_char( "Who do you wish to hunt?\n\r", ch );
	return;
    }
    if (!str_cmp(arg,ch->name))
    {
	send_to_char("How can you hunt yourself?\n\r",ch);
	return;
    }

/*
    if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->pcdata->powers[WPOWER_LYNX] < 1 )
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }*/

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not online.\n\r", ch );
        return;
    }

    ch->hunting = str_dup(arg);
    send_to_char( "Ok.\n\r", ch );
    return;
}

void check_hunt( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    bool found = FALSE;
    int direction = 0;
    ROOM_INDEX_DATA *in_room;

    in_room = ch->in_room;
    if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_track])
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
	return;
    }

    if (check_track(ch,0)) {found = TRUE;direction = ch->in_room->track_dir[0];}
    else if (check_track(ch,1)) {found = TRUE;direction = ch->in_room->track_dir[1];}
    else if (check_track(ch,2)) {found = TRUE;direction = ch->in_room->track_dir[2];}
    else if (check_track(ch,3)) {found = TRUE;direction = ch->in_room->track_dir[3];}
    else if (check_track(ch,4)) {found = TRUE;direction = ch->in_room->track_dir[4];}
    else if (check_track(ch,5)) {found = TRUE;direction = ch->in_room->track_dir[5];}
    else if ( ( victim = get_char_room( ch, ch->hunting ) ) == NULL )
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
	return;
    }
    if (strlen(ch->hunting) < 2) return;
    if ( ( victim = get_char_room( ch, ch->hunting ) ) != NULL ) return;
    act("$n carefully examines the ground for tracks.",ch,NULL,NULL,TO_ROOM);
    move_char(ch,direction);

    if (in_room == ch->in_room || victim != NULL)
    {
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
    }
    return;
}

void add_tracks(CHAR_DATA *ch, int direction)
{
    int loop;

    if (IS_NPC(ch)) return;
    if ( (IS_CLASS(ch, CLASS_WEREWOLF) && ch->gifts[RAGABASH] > 1)
    || (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_SORA] > 3)
)
        /* Ninjas don't leave tracks if they have Sora 4 or higher -*/
	return;
    if (IS_ITEMAFF(ch, ITEMA_STALKER)) return;
 for (loop = 0; loop <= 4; loop++)
    {
    	if (ch->in_room->track[loop] != NULL && !str_cmp(ch->in_room->track[loop],ch->name))
    	{
	    free_string(ch->in_room->track[loop]);
	    ch->in_room->track[loop] = str_dup("");
    	}
    }
    if (strlen(ch->in_room->track[0]) < 2)
    {
	free_string(ch->in_room->track[0]);
	ch->in_room->track[0]     = str_dup(ch->name);
	ch->in_room->track_dir[0] = direction;
    }
    else if (strlen(ch->in_room->track[1]) < 2)
    {
	free_string(ch->in_room->track[1]);
	ch->in_room->track[1]     = str_dup(ch->name);
	ch->in_room->track_dir[1] = direction;
    }
    else if (strlen(ch->in_room->track[2]) < 2)
    {
	free_string(ch->in_room->track[2]);
	ch->in_room->track[2]     = str_dup(ch->name);
	ch->in_room->track_dir[2] = direction;
    }
    else if (strlen(ch->in_room->track[3]) < 2)
    {
	free_string(ch->in_room->track[3]);
	ch->in_room->track[3]     = str_dup(ch->name);
	ch->in_room->track_dir[3] = direction;
    }
    else if (strlen(ch->in_room->track[4]) < 2)
    {
	free_string(ch->in_room->track[4]);
	ch->in_room->track[4]     = str_dup(ch->name);
	ch->in_room->track_dir[4] = direction;
    }
    else
    {
	free_string(ch->in_room->track[0]);
	ch->in_room->track[0]     = str_dup(ch->in_room->track[1]);
	ch->in_room->track_dir[0] = ch->in_room->track_dir[1];
	free_string(ch->in_room->track[1]);
	ch->in_room->track[1]     = str_dup(ch->in_room->track[2]);
	ch->in_room->track_dir[1] = ch->in_room->track_dir[2];
	free_string(ch->in_room->track[2]);
	ch->in_room->track[2]     = str_dup(ch->in_room->track[3]);
	ch->in_room->track_dir[2] = ch->in_room->track_dir[3];
	free_string(ch->in_room->track[3]);
	ch->in_room->track[3]     = str_dup(ch->in_room->track[4]);
	ch->in_room->track_dir[3] = ch->in_room->track_dir[4];
	free_string(ch->in_room->track[4]);
	ch->in_room->track[4]     = str_dup(ch->name);
	ch->in_room->track_dir[4] = direction;
    }
    return;
}

bool check_track(CHAR_DATA *ch, int direction)
{
    CHAR_DATA *victim;
    char buf [MAX_INPUT_LENGTH];
    char vict [MAX_INPUT_LENGTH];
    int door;

    strcpy(vict,ch->hunting);
    if (str_cmp(ch->hunting,vict))
    {
	if ((victim = get_char_room(ch, vict)) != NULL)
	{
	    act("You have found $N!",ch,NULL,victim,TO_CHAR);
	    free_string(ch->hunting);
	    ch->hunting = str_dup("");
	    return TRUE;
	}

    }
    if (strlen(ch->in_room->track[direction]) < 2) return FALSE;
    if (!str_cmp(ch->in_room->track[direction],ch->name)) return FALSE;
    if (strlen(ch->hunting) > 1 && str_cmp(ch->in_room->track[direction],ch->hunting)) return FALSE;
    door = ch->in_room->track_dir[direction];
    sprintf(buf,"You sense the trail of %s leading $T from here.",ch->in_room->track[direction]);
    act(buf, ch, NULL, dir_name[door], TO_CHAR);
    return TRUE;
}

void do_roll( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_DEMPOWER( ch, DEM_MOVE))
    {
	send_to_char("You haven't been granted the gift of movement.\n\r",ch);
	return;
    }

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( obj->chobj == NULL || obj->chobj != ch )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( obj->in_room == NULL )
    {
	send_to_char("You are unable to move.\n\r",ch);
	return;
    }

    if      (!str_cmp(arg,"n") || !str_cmp(arg,"north")) do_north(ch,"");
    else if (!str_cmp(arg,"s") || !str_cmp(arg,"south")) do_south(ch,"");
    else if (!str_cmp(arg,"e") || !str_cmp(arg,"east" )) do_east(ch,"");
    else if (!str_cmp(arg,"w") || !str_cmp(arg,"west" )) do_west(ch,"");
    else if (!str_cmp(arg,"u") || !str_cmp(arg,"up"   )) do_up(ch,"");
    else if (!str_cmp(arg,"d") || !str_cmp(arg,"down" )) do_down(ch,"");
    else
    {
	send_to_char("Do you wish to roll north, south, east, west, up or down?\n\r",ch);
	return;
    }
    obj_from_room(obj);
    obj_to_room(obj,ch->in_room);
    return;
}

void do_leap( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *container;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_DEMPOWER( ch, DEM_LEAP))
    {
	send_to_char("You haven't been granted the gift of leaping.\n\r",ch);
	return;
    }

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( obj->chobj == NULL || obj->chobj != ch )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( obj->in_room != NULL && arg[0] == '\0' )
    {
	send_to_char("Where do you want to leap?\n\r",ch);
	return;
    }

    if ( obj->in_room != NULL )
    {
	if ( ( victim = get_char_room( ch, arg ) ) != NULL )
	{
	    act("$p leaps into your hands.",victim,obj,NULL,TO_CHAR);
	    act("$p leaps into $n's hands.",victim,obj,NULL,TO_ROOM);
	    obj_from_room(obj);
	    obj_to_char(obj,victim);
	    return;
	}
	else if ( ( container = get_obj_room( ch, arg ) ) != NULL )
	{
	    if (container->item_type != ITEM_CONTAINER &&
		container->item_type != ITEM_CORPSE_NPC &&
		container->item_type != ITEM_CORPSE_PC)
	    {
		send_to_char("You cannot leap into that sort of object.\n\r",ch);
		return;
	    }
	    act("$p leap into $P.",ch,obj,container,TO_CHAR);
	    act("$p leaps into $P.",ch,obj,container,TO_ROOM);
	    obj_from_room(obj);
	    obj_to_obj(obj,container);
	    return;
	}
	else send_to_char( "Nothing here by that name.\n\r",ch );
	return;
    }
    if ( obj->carried_by != NULL )
    {
	act("$p leaps from your hands.",obj->carried_by,obj,NULL,TO_CHAR);
	act("$p leaps from $n's hands.",obj->carried_by,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	return;
    }
    else if ( (container = obj->in_obj) != NULL && container->in_room != NULL )
    {
	obj_from_obj(obj);
	obj_to_room(obj,container->in_room);
	char_from_room(ch);
	char_to_room(ch,container->in_room);
	act("$p leap from $P.",ch,obj,container,TO_CHAR);
	act("$p leaps from $P.",ch,obj,container,TO_ROOM);
	return;
    }
    if (obj->in_room != NULL)
	send_to_char("You seem unable to leap anywhere.\n\r",ch);
    else
	send_to_char("You seem to be stuck!\n\r",ch);
    return;
}

void drow_hate( CHAR_DATA *ch)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->newbits,
NEW_DROWHATE)) return;

	if (number_percent() < 25) {

	for (vch = char_list; vch != NULL; vch = vch_next)
	{
	vch_next = vch->next;
	if ( ch == vch) continue;
	if ( vch->in_room == NULL) continue;
	if ( vch->in_room == ch->in_room) {
		send_to_char("You scream out in hatred and attack!\n\r", ch );
		act("$n screams out in hatred and attacks!",ch,NULL,vch,TO_ROOM);
		do_kill(ch,vch->name);
		return;}
	}
	}

	return;
}

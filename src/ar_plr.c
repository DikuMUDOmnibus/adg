/***************************************************************************
 *  This file is part of the Alternate Realities mud and is part of the    *
 *  AltReal code release, Please remember everything here was either       *
 *  coded from scratch, totaly recoded from previous releases or supplied  *
 *  with permission for this release.                                      *
 *                                                                         *
 *  Please abide by the Diku/Merc/Godwars policys when using this code.    *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
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
#include "player.h"
#include <unistd.h>

/* This file is primarily for player commands tho it may stray a little as
 * I put it together.
 */

void do_affects( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *af;

    divide_to_char(ch);
    stc("#P  #n                                  #rAFFECTS#n                                  #P#n\n\r",ch);
    divide_to_char(ch);
    stc("You are currently affected by:                                           \n\r", ch);
    if ( ch->affected == NULL )
        {
            stc( "You are not affected by any spells at this time.\n\r", ch);
        }
	  else
        {
            for( af = ch->affected; af; af = af->next )
            {
                sprintf( buf, "%s: modifies %s by %d for %d more tick%s.\n\r",
                skill_table[af->type].name,
                affect_loc_name( af->location ),
                af->modifier,
                af->duration,
                af->duration == 1 ? "" : "s" );
                stc( buf, ch );
            }
        }
    if ( IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD) ) 
    {
        stc( "#w* #oYou are surrounded by a crackling shield of lightning.#n\n\r", ch);
    }
    if ( IS_ITEMAFF(ch, ITEMA_FIRESHIELD) )
    { 
        stc( "#w* #RYou are surrounded by a burning shield of fire.#n\n\r", ch);
    }
    if ( IS_ITEMAFF(ch, ITEMA_ICESHIELD) ) 
    {
        stc( "#w* #CYou are surrounded by a shimmering shield of ice.#n\n\r", ch);
    }
    if ( IS_ITEMAFF(ch, ITEMA_ACIDSHIELD) )  
    {
        stc( "#w* #GYou are surrounded by a bubbling shield of acid.#n\n\r", ch);
    }
    if ( IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD) )
    { 
        stc( "#w* #PYou are surrounded by a swirling shield of chaos.#n\n\r", ch);
    }
    if ( IS_ITEMAFF(ch, ITEMA_REFLECT) )
    { 
        stc( "#w* #eYou are surrounded by a flickering shield of darkness.#n\n\r", ch);
    }
    if ( !IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD) && !IS_ITEMAFF(ch, ITEMA_FIRESHIELD) &&
         !IS_ITEMAFF(ch, ITEMA_ICESHIELD) &&  !IS_ITEMAFF(ch, ITEMA_ACIDSHIELD) )
    {
        stc( "#w*#n You are surrounded by no elemental shields.\n\r", ch);
    }
        sprintf( buf, "Other Affects: %s.\n\r",
        affect_bit_name( ch->affected_by ) );
        stc( buf, ch );
        divide_to_char(ch);
        return;
}

void do_notravel( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch travel ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_TRAVEL) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_TRAVEL);
	send_to_char("You can now be the target of travel spells.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_TRAVEL) && !str_cmp(arg,"off"))
    {
	send_to_char("But it is already off!\n\r", ch);
	return;
    }
    else if (!IS_IMMUNE(ch, IMM_TRAVEL) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_TRAVEL);
	send_to_char("You can no longer be the target of travel spells.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_TRAVEL) && !str_cmp(arg,"on"))
    {
	send_to_char("But it is already on!\n\r", ch);
	return;
    }
    else
	send_to_char( "Do you wish to switch travel ON or OFF?\n\r", ch );
    return;
}

void do_version( CHAR_DATA *ch, char *argument) 
{
stc("{m-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-{x\n\r",ch);
stc("{m>                                                                             <\n\r",ch);
stc("{m>                          {C  Another {DDarn {cGodWars                             {m<{x\n\r",ch);
stc("{m>                       {M     --------------------                             {m<{x\n\r",ch);
stc("{m>                 {wCreated by {D-{c Tracker {D({mtracker@godwars.net{D){m                  <{x\n\r",ch);
stc("{m>                                                                             <\n\r",ch);
stc("{m>                 {M   Special Thanks to Cirin for Misc. Code  {m                 <\n\r",ch);
stc("{m-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-=<*>=-{x\n\r",ch);
  return;
}

void do_testscore( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char pkratio[MAX_STRING_LENGTH];
    int a_c = char_ac(ch);
    int test;

    if ( IS_NPC(ch) ) return;

    if (ch->pkill > 0)
    {
      if (100*ch->pkill/(ch->pkill + ch->pdeath) < 100)
          sprintf(pkratio,"0.%-2d", (100*ch->pkill/(ch->pkill + ch->pdeath)));
      else
          sprintf(pkratio,"1.00");
    }
    else sprintf(pkratio,"0.00");

    send_to_char( "\n\r", ch );
    send_to_char( "\n\r", ch );
    sprintf( buf, "#eYou are #g%s%s#e.#n\n\r",
        ch->name,
        IS_NPC(ch) ? "" : ch->pcdata->title);
    send_to_char( buf, ch );
    if (!IS_NPC(ch)) birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);
    if ( get_trust( ch ) != ch->level )
    {
        sprintf( buf, "#eYou are trusted at level #C%d#e.#n\n\r",
            get_trust( ch ) );
        send_to_char( buf, ch );
    }
    send_to_char( "\n\r", ch );
    sprintf( buf,
        " #eHps    #w[ #c%7ld#e/#c%-7ld #w] #eHitroll #w[ #c%5d #w] #eStr #w[ #c%3d #w] #eExp    #w[ #c%10d #w]#n\n\r",
        ch->hit, ch->max_hit, char_hitroll(ch), get_curr_str(ch), ch->exp);
    send_to_char(buf, ch);
    sprintf( buf,
        " #eMana   #w[ #c%7ld#e/#c%-7ld #w] #eDamroll #w[ #c%5d #w] #eInt #w[ #c%3d #w] #ePrimal #w[ #c%10d #w]#n\n\r",
        ch->mana, ch->max_mana, char_damroll(ch), get_curr_int(ch), ch->practice );
    send_to_char(buf, ch);
    sprintf( buf,
        " #eMove   #w[ #c%7ld#e/#c%-7ld #w] #eDamcap  #w[ #c%5d #w] #eWis #w[ #c%3d #w] #eQps    #w[ #c%10d #w]#n\n\r",
        ch->move, ch->max_move, ch->damcap[0], get_curr_wis(ch), ch->pcdata->quest );
    send_to_char(buf, ch);
    sprintf( buf,
        " #eItems  #w[ #c%7d#e/#c%-7d #w] #eAC      #w[ #c%5d #w] #eDex #w[ #c%3d #w] #eGold   #w[ #c%10d #w]#n\n\r",
        ch->carry_number, can_carry_n(ch), a_c, get_curr_dex(ch), ch->gold);
    send_to_char(buf, ch);
    sprintf( buf,
        " #eWeight #w[ #c%7d#e/#c%-7d #w] #eAlign   #w[ #c%5d #w] #eCon #w[ #c%3d #w] #eHours  #w[ #c%10d #w]#n\n\r",
        ch->carry_weight, can_carry_w(ch), ch->alignment, get_curr_con(ch), (get_age(ch) - 17) * 2 );
    send_to_char(buf, ch);
    send_to_char( "\n\r", ch );
    sprintf( buf,
        " #ePkills #w[ #c%5d #w] #eRatio  #w[ #c%5s #w] #eAutoexit #w[ #g%3s #w] #eWimpy #w[ #c%7d #w]#n\n\r",
        ch->pkill, pkratio, IS_SET(ch->act, PLR_AUTOEXIT) ? "Yes" : "No", ch->wimpy);
    send_to_char(buf, ch);
    sprintf( buf,
        " #ePdeath #w[ #c%5d #w] #eStatus #w[ #c%5d #w] #eAutoloot #w[ #g%3s #w] #eNone  #w[       #c* #w]#n\n\r",
        ch->pdeath, ch->race, IS_SET(ch->act, PLR_AUTOLOOT) ? "Yes" : "No" );
    send_to_char(buf, ch);
    sprintf( buf,
        " #eMkills #w[ #c%5d #w] #eNone   #w[ #c    * #w] #eAutosac  #w[ #g%3s #w] #eNone  #w[       #c* #w]#n\n\r",
        ch->mkill, IS_SET(ch->act, PLR_AUTOSAC) ? "Yes" : "No");
    send_to_char(buf, ch);
    sprintf( buf,
        " #eMdeath #w[ #c%5d #w] #eNone   #w[ #c    * #w] #eNone     #w[   #c* #w] #eNone  #w[       #c* #w]#n\n\r",
        ch->mdeath );
    send_to_char(buf, ch);
    send_to_char( "\n\r", ch );
    if( IS_IMMORTAL(ch) )
    {
    sprintf( buf,
        " #eHolylight #w[ #g%3s #w] #eWiz Invis #w[ #g%3s #w] #eWiz Incog #w[ #g%3s #w]#n",
        IS_SET(ch->act, PLR_HOLYLIGHT) ? "Yes" : "No", IS_SET(ch->act, PLR_WIZINVIS) ? "Yes" : "No",
        IS_SET(ch->act, PLR_INCOG) ? "Yes" : "No");
    send_to_char(buf, ch);
    send_to_char( " #eYou are #n", ch );
    if      (ch->level == 7 ) send_to_char( "#ean #BApprentice#e.#n\n\r", ch);
    else if (ch->level == 8 ) send_to_char( "#ea #PQuestor#e.#n\n\r", ch);
    else if (ch->level == 9 ) send_to_char( "#ea #rCouncillor#e.#n\n\r", ch);
    else if (ch->level == 10) send_to_char( "#ea #yRegent#e.#n\n\r", ch);
    else if (ch->level == 11) send_to_char( "#ea #cPontifex#e.#n\n\r", ch);
    else if (ch->level == 12) send_to_char( "#ean #gImplementor#e.#n\n\r", ch);
    else send_to_char( "#ea #GBugged Character#e!#n\n\r", ch);
    send_to_char( "\n\r", ch );
    }
    test = getMight(ch);
    if(IS_IMMORTAL(ch))
    {
	sprintf(buf,"Might: %d\n\r", test);
	send_to_char(buf, ch);
    }
    divide_to_char( ch );
return;
}

void do_token( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if (IS_NPC(ch)) return;

    if ( ch->pcdata->quest < 1 && !IS_JUDGE(ch) )
    {
	send_to_char("You are unable to make a quest token.\n\r",ch);
	return;
    }

    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char("Please specify a value for the quest token.\n\r",ch);
	send_to_char("Which can be between 1 and 5000 quest points.\n\r",ch);
	return;
    }
    else
    {
        value = atoi( arg1 );
	if ( value < 1 || value > 5000 )
        {
	    send_to_char( "Quest token should have a value between 1 and 5000.\n\r", ch );
	    return;
        }
	else if ( value > ch->pcdata->quest && !IS_JUDGE(ch) )
        {
	    sprintf(buf,"You only have %d quest points left to put into tokens.\n\r",ch->pcdata->quest);
	    send_to_char( buf, ch );
	    return;
        }
    }

    victim = get_char_room( ch, arg2 );

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform an Immortal.\n\r", ch );
	return;
    }

    ch->pcdata->quest -= value;
    if (ch->pcdata->quest < 0) ch->pcdata->quest = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
    obj->item_type = ITEM_QUEST;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "quest token" );
    sprintf(buf,"a %d point quest token",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d point quest token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (victim != NULL && victim != ch)
    {
    	act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
    	act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
    	act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT );
    }
    else
    {
    	act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
    	act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );
    }
    return;
}

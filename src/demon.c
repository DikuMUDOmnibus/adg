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
#include "player.h"

/*
 * Local functions.
 */

void dem_body( CHAR_DATA *ch )
{
    if (IS_NPC(ch) ) return;

    if ( !IS_DEMPOWER(ch, DEM_CLAWS)  && !IS_DEMPOWER(ch, DEM_FORM)  &&
         !IS_DEMPOWER(ch, DEM_FANGS)  && !IS_DEMPOWER(ch, DEM_GRAFT) &&
         !IS_DEMPOWER(ch, DEM_HOOVES) && !IS_DEMPOWER(ch, DEM_HORNS) &&
         !IS_DEMPOWER(ch, DEM_TAIL)   && !IS_DEMPOWER(ch, DEM_WINGS) )
    {
        stc("Body Modification: None.\n\r",ch);
        return;
    }

    if ( IS_DEMPOWER(ch, DEM_CLAWS) )
    stc("Body Modification: Claws",ch);
    if ( IS_DEMPOWER(ch, DEM_FORM) )
    stc(", Demon Form",ch);
    if ( IS_DEMPOWER(ch, DEM_FANGS) )
    stc(", Fangs",ch);
    if ( IS_DEMPOWER(ch, DEM_GRAFT) )
    stc(", Graft",ch);
    if ( IS_DEMPOWER(ch, DEM_HOOVES) )
    stc(", Hooves",ch);
    if ( IS_DEMPOWER(ch, DEM_HORNS) )
    stc(", Horns",ch);
    if ( IS_DEMPOWER(ch, DEM_TAIL) )
    stc(", Tail",ch);
    if ( IS_DEMPOWER(ch, DEM_WINGS) )
    stc(", Wings",ch);
    stc(".\n\r",ch);
    return;
}

void dem_senses( CHAR_DATA *ch )
{
    if (IS_NPC(ch) ) return;

    if ( !IS_DEMPOWER(ch, DEM_EYES)  && !IS_DEMPOWER(ch, DEM_SHADOWSIGHT)  &&
         !IS_DEMPOWER(ch, DEM_TRUESIGHT) )
    {
        stc("Senses:            None.\n\r",ch);
        return;
    }

    if ( IS_DEMPOWER(ch, DEM_EYES) )
    stc("Senses:            Nightsight",ch);
    if ( IS_DEMPOWER(ch, DEM_SHADOWSIGHT) )
    stc(", Shadowsight",ch);
    if ( IS_DEMPOWER(ch, DEM_TRUESIGHT) )
    stc(", Truesight",ch);
    stc(".\n\r",ch);
    return;
}

void dem_wpform( CHAR_DATA *ch )
{
    if (IS_NPC(ch) ) return;

    if ( !IS_DEMPOWER(ch, DEM_LEAP)  && !IS_DEMPOWER(ch, DEM_MAGIC)  &&
         !IS_DEMPOWER(ch, DEM_MOVE) )
    {
        stc("Weaponform:        None.\n\r",ch);
        return;
    }

    if ( IS_DEMPOWER(ch, DEM_LEAP) )
    stc("Weaponform:        Leap",ch);
    if ( IS_DEMPOWER(ch, DEM_MAGIC) )
    stc(", Magic",ch);
    if ( IS_DEMPOWER(ch, DEM_MOVE) )
    stc(", Move",ch);
    stc(".\n\r",ch);
    return;
}

void do_dempowers( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;

//    if ( !IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }    

    divide_to_char(ch);
    stc("#P=-#n                                  #rDEMONS#n                                   #P-=#n\n\r",ch);
    stc("#P=-=-=-=-=-=-=-=-=-=-=-=-=#B[#G**#B]#w       POWERS        #B[#G**#B]#P=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
    dem_body(ch);
    dem_senses(ch);
    dem_wpform(ch);
    divide_to_char(ch);
    sprintf( buf,
    "You have %d out of %d points of demonic power stored.\n\r",
    ch->pcdata->stats[DEMON_CURRENT],  ch->pcdata->stats[DEMON_TOTAL] );
    stc( buf, ch );
    if(IS_SET(ch->special, SPC_DEMON_LORD) )
    {
        stc("You are a Demon Lord.\n\r", ch);
    }
    else if(IS_SET(ch->special, SPC_CHAMPION) )
    {
        stc("You are a Demon Champion.\n\r", ch);
        if(IS_SET(ch->special, SPC_PRINCE) )
        {
            stc("You are also a Demon Prince.\n\r", ch);
        }
        else if(IS_SET(ch->special, SPC_SIRE) )
        {
            stc("You are able to make Pacts for souls.\n\r", ch);
        }
    }
    divide_to_char(ch);
    return;
}

void fire_effect(void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM)  /* nail objects on the floor */
    {
	ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

		for (obj = room->contents; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    fire_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_CHAR)   /* do the effect on a victim */
    {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;

	/* chance of blindness */
	if (!IS_AFFECTED(victim,AFF_BLIND))
	{
	    AFFECT_DATA af;
	    act("$n is blinded by smoke!",victim,NULL,NULL,TO_ROOM);
	    act("Your eyes tear up from smoke...you can't see a thing!",
		victim,NULL,NULL,TO_CHAR);

	    af.type         = skill_lookup("fire breath");
	    af.duration     = number_range(0,level/10);
	    af.location     = APPLY_HITROLL;
	    af.modifier     = -20;
	    af.bitvector    = AFF_BLIND;

	    affect_to_char(victim,&af);
	}

	/* let's toast some gear! */
	for (obj = victim->carrying; obj != NULL; obj = obj_next)
	{
	    obj_next = obj->next_content;
	    fire_effect(obj,level,dam,TARGET_OBJ);
	}
	return;
    }

    if (target == TARGET_OBJ)  /* toast an object */
    {
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	OBJ_DATA *t_obj,*n_obj;
	int chance;
	char *msg;

	chance = level / 4 + dam / 10;
 
	if (chance > 25)
	    chance = (chance - 25) / 2 + 25;
	if (chance > 50)
	    chance = (chance - 50) / 2 + 50;

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	    chance -= 5;
	chance -= obj->level * 2;

	switch ( obj->item_type )
	{
	default:         
	    return;
	case ITEM_CONTAINER:
	    msg = "$p ignites and burns!";
	    break;
	case ITEM_POTION:
	    chance += 25;
	    msg = "$p bubbles and boils!";
	    break;
	case ITEM_SCROLL:
	    chance += 50;
	    msg = "$p crackles and burns!";
	    break;
	case ITEM_STAFF:
	    chance += 10;
	    msg = "$p smokes and chars!";
	    break;
	case ITEM_WAND:
	    msg = "$p sparks and sputters!";
	    break;
	case ITEM_FOOD:
	    msg = "$p blackens and crisps!";
	    break;
	case ITEM_PILL:
	    msg = "$p melts and drips!";
	    break;
	}

	chance = URANGE(5,chance,95);

	if (number_percent() > chance)
	    return;
 
	if (IS_SET(obj->quest, QUEST_SPELLPROOF))
	    return;      

	if (obj->carried_by != NULL)
	    act( msg, obj->carried_by, obj, NULL, TO_ROOM );
	else if (obj->in_room != NULL && obj->in_room->people != NULL)
	    act(msg,obj->in_room->people,obj,NULL,TO_ROOM);

	if (obj->contains)
	{
	    /* dump the contents */
 
	    for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
	    {
		n_obj = t_obj->next_content;
		obj_from_obj(t_obj);
		if (obj->in_room != NULL)
		    obj_to_room(t_obj,obj->in_room);
		else if (obj->carried_by != NULL)
		    obj_to_room(t_obj,obj->carried_by->in_room);
		else
		{
		    extract_obj(t_obj);
		    continue;
		}
		fire_effect(t_obj,level/2,dam/2,TARGET_OBJ);
	    }
	}

	extract_obj( obj );
	return;
    }
}

void do_inferno(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch, *vch_next;

    if (!IS_DEMPOWER(ch, DEM_INFERNO))
    {
	send_to_char("You have not been granted that power.\n\r", ch);
	return;
    }

    if ( ch->hit > 0 )
    {
	send_to_char("This is only used as a last resort.\n\r",ch);
	return;
    }
    
    if ( IS_EXTRA(ch, TIED_UP) )
    {
      stc("Not while tied up!\n\r",ch);
      return;
    }

    act("$n explodes, sending forth a massive fireballs in all directions.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You explode in a huge blast of abyssal flame.\n\r",ch);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	    vch_next = vch->next_in_room;
	    if (vch != ch)
	    {
		damage(vch, vch, number_range(1500,2000), gsn_inferno);
		damage(vch, vch, number_range(1500,2000), gsn_inferno);
		fire_effect(vch, ch->level, 2000, TARGET_CHAR);
	    }
    }
    WAIT_STATE(ch, 56);
//    ch->hit = 10;
    ch->hit = -2;
    update_pos(ch);
    return;
}   

void do_warps( CHAR_DATA *ch, char *argument )
{
    char lin   [MAX_STRING_LENGTH];
 
 
    if (IS_NPC(ch)) return;
//    if ( !IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    sprintf( lin,
"-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-=(**)=-\n\r");
    send_to_char( lin,ch);
    send_to_char(
"                      -=(**)=- Attained Warps -=(**)=-\n\r",ch);
send_to_char(lin,ch);
if (IS_SET(ch->warp, WARP_CBODY      )) send_to_char("Your body is protected by a indestructable crystal shell.\n\r",ch);
if (IS_SET(ch->warp, WARP_SBODY      )) send_to_char("Your skin is as hard as steel.\n\r",ch);
if (IS_SET(ch->warp, WARP_STRONGARMS )) send_to_char("Your arms are incredibly strong.\n\r",ch);
if (IS_SET(ch->warp, WARP_STRONGLEGS )) send_to_char("Your legs are incredibly strong.\n\r",ch);
if (IS_SET(ch->warp, WARP_VENOMTONG  )) send_to_char("Your tongue is long and venomous.\n\r",ch);
if (IS_SET(ch->warp, WARP_SPIKETAIL  )) send_to_char("Your tail fires deadly spikes during combat.\n\r",ch);
if (IS_SET(ch->warp, WARP_BADBREATH  )) send_to_char("Your breath is putrid and deadly.\n\r",ch);
if (IS_SET(ch->warp, WARP_QUICKNESS  )) send_to_char("You have incredible speed.\n\r",ch);
if (IS_SET(ch->warp, WARP_STAMINA    )) send_to_char("You have increased stamina, reducing the damage you take.\n\r",ch);
if (IS_SET(ch->warp, WARP_HUNT       )) send_to_char("Your heightened senses enable you to hunt people.\n\r",ch);
if (IS_SET(ch->warp, WARP_DAMCAP     )) send_to_char("You are incredibly powerful, making you hit much harder!.\n\r",ch);
if (IS_SET(ch->warp, WARP_TERROR     )) send_to_char("Your features are so horrid that they may stun those who look at you.\n\r",ch);
if (IS_SET(ch->warp, WARP_REGEN      )) send_to_char("Your body has the ability to regenerate incredibly fast.\n\r",ch);
if (IS_SET(ch->warp, WARP_STEED      )) send_to_char("Your mounts transform into hideous Demons.\n\r",ch);
if (IS_SET(ch->warp, WARP_WEAPON     )) send_to_char("You have the power to transform into a deadly battle axe.\n\r",ch);
if (IS_SET(ch->warp, WARP_INFIRMITY  )) send_to_char("Your body has been afflicted by a terrible infirmity.\n\r",ch);
if (IS_SET(ch->warp, WARP_GBODY      )) send_to_char("Your skin is made of a fragile glass.\n\r",ch);
if (IS_SET(ch->warp, WARP_SCARED     )) send_to_char("You are incredibly scared of combat.\n\r",ch);
if (IS_SET(ch->warp, WARP_MAGMA      )) send_to_char("Your body is composed of deadly magma.\n\r",ch);
if (IS_SET(ch->warp, WARP_WEAK       )) send_to_char("Your muscles are severely weakened.\n\r",ch);
if (IS_SET(ch->warp, WARP_SLOW       )) send_to_char("Your body moves very slowly.\n\r",ch);
if (IS_SET(ch->warp, WARP_VULNER     )) send_to_char("Your skin is very vulnerable to magic.\n\r",ch);
if (IS_SET(ch->warp, WARP_SHARDS     )) send_to_char("Your skin is covered with shards of ice.\n\r",ch);
if (IS_SET(ch->warp, WARP_WINGS      )) send_to_char("A pair of leathery wings protrude from your back.\n\r",ch);
if (IS_SET(ch->warp, WARP_CLUMSY     )) send_to_char("You are incredibly clumsy, enabling you to be disarmed.\n\r",ch);
if (IS_SET(ch->warp, WARP_STUPID     )) send_to_char("Your intelligence is extremely low, preventing you from casting spells.\n\r",ch);
if (IS_SET(ch->warp, WARP_SPOON      )) send_to_char("There is a spoon stuck on your ear.\n\r",ch);
if (IS_SET(ch->warp, WARP_FORK       )) send_to_char("You have a fork stuck in your nose.\n\r",ch);
if (IS_SET(ch->warp, WARP_KNIFE      )) send_to_char("You have a knife hanging out of your ear.\n\r",ch);
if (IS_SET(ch->warp, WARP_SALADBOWL  )) send_to_char("Your head is made out of a salad bowl.\n\r",ch);
//if (IS_SET(ch->warp, WARP_HITROLL  )) send_to_char("You hit with more accuracy.\n\r",ch);
//if (IS_SET(ch->warp, WARP_DAMROLL  )) send_to_char("Your hits cause more damage.\n\r",ch);
if (ch->warp < 1) send_to_char("You haven't obtained any warp powers.\n\r",ch);
send_to_char(lin,ch);
return;
}


void do_redeem( CHAR_DATA *ch, char *argument )
{

    int           warpnum = number_range(1,19);
    int	      newwarp = 0;
    if (IS_NPC(ch)) return;

//    if ( !IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if (ch->pcdata->stats[DEMON_TOTAL] < 10000 || 
	ch->pcdata->stats[DEMON_CURRENT] < 10000)
    {
	send_to_char("You need 10000 demon points to redeem warps!\n\r",ch);
	return;
    }
    {
             if (warpnum == 1) newwarp = WARP_CBODY;
        else if (warpnum == 2) newwarp = WARP_SBODY;
	else if (warpnum == 3) newwarp = WARP_STRONGARMS;
	else if (warpnum == 4) newwarp = WARP_STRONGLEGS;
	else if (warpnum == 5) newwarp = WARP_VENOMTONG;
	else if (warpnum == 6) newwarp = WARP_SPIKETAIL;
	else if (warpnum == 7) newwarp = WARP_BADBREATH;
	else if (warpnum == 8) newwarp = WARP_QUICKNESS;
	else if (warpnum == 9) newwarp = WARP_STAMINA;
	else if (warpnum == 11) newwarp = WARP_HUNT;
	else if (warpnum == 12) newwarp = WARP_DAMCAP;
	else if (warpnum == 13) newwarp = WARP_TERROR;
	else if (warpnum == 14) newwarp = WARP_REGEN;
	else if (warpnum == 15) newwarp = WARP_STEED;
	else if (warpnum == 16) newwarp = WARP_WEAPON;
        else if (warpnum == 17) newwarp = WARP_SHARDS;
	else if (warpnum == 18) newwarp = WARP_WINGS;
	else if (warpnum == 19) newwarp = WARP_MAGMA;
    }

	if (ch->warp==12877823)
	newwarp = 0;

	if (IS_SET(ch->warp, newwarp) && ch->warp != 12877823)
    {
      do_redeem(ch,"");
	return;
    }
	
    {
	if (IS_SET(ch->warp, WARP_INFIRMITY))
	{
	   REMOVE_BIT(ch->warp, WARP_INFIRMITY);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_GBODY))
	{
	   REMOVE_BIT(ch->warp, WARP_GBODY);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_SCARED))
	{
	   REMOVE_BIT(ch->warp, WARP_SCARED);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_WEAK))
	{
	   REMOVE_BIT(ch->warp, WARP_WEAK);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_SLOW))
	{
	   REMOVE_BIT(ch->warp, WARP_SLOW);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_VULNER))
	{
	   REMOVE_BIT(ch->warp, WARP_VULNER);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_CLUMSY))
	{
	   REMOVE_BIT(ch->warp, WARP_CLUMSY);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_STUPID))
	{
	   REMOVE_BIT(ch->warp, WARP_STUPID);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_SPOON))
	{
	   REMOVE_BIT(ch->warp, WARP_SPOON);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_FORK))
	{
	   REMOVE_BIT(ch->warp, WARP_FORK);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
	else if (IS_SET(ch->warp, WARP_KNIFE))
	{
	   REMOVE_BIT(ch->warp, WARP_KNIFE);
	   SET_BIT(ch->warp, newwarp);
	   ch->pcdata->stats[DEMON_CURRENT] -= 10000;
	   send_to_char("You pray to Satan and a warp is cleansed.\n\r",ch);
    	   save_char_obj(ch);
    	   return;
	}
      else send_to_char("You have no evil warps to redeem!\n\r",ch);
    }	
  return;
}
/* Changed Toaster to add clearwarps command 06/03/00, also see interp.h
and merc.h for added commands stuff*/

void do_clearwarps( CHAR_DATA *ch, char *argument)
{
    int cost;

      if (IS_NPC(ch)) return;
        
        if ( !IS_CLASS(ch, CLASS_DEMON) )
//    if (!IS_CLASS(ch, CLASS_DEMON))
        {
        send_to_char("Huh?\n\r", ch);
        return;
        }

    cost = ch->warpcount * 20000;

    if ((ch->pcdata->stats[DEMON_TOTAL] < cost) ||
        (ch->pcdata->stats[DEMON_CURRENT] < cost))
    {
        send_to_char("You need 20k dps per warp to clear all warps\n\r",ch);
        return;
    }   
    if (ch->warpcount < 1)
    {
	send_to_char("You need a warp to clear first.\n\r", ch);
	return;
    }
       REMOVE_BIT(ch->warp, WARP_SALADBOWL);  
       REMOVE_BIT(ch->warp, WARP_SPOON);  
       REMOVE_BIT(ch->warp, WARP_FORK);  
       REMOVE_BIT(ch->warp, WARP_KNIFE);  
       REMOVE_BIT(ch->warp, WARP_HUNT);  
       REMOVE_BIT(ch->warp, WARP_DAMCAP);  
       REMOVE_BIT(ch->warp, WARP_CLUMSY);  
       REMOVE_BIT(ch->warp, WARP_WINGS);  
       REMOVE_BIT(ch->warp, WARP_SHARDS);  
       REMOVE_BIT(ch->warp, WARP_VULNER);  
       REMOVE_BIT(ch->warp, WARP_SLOW);  
       REMOVE_BIT(ch->warp, WARP_WEAK);  
       REMOVE_BIT(ch->warp, WARP_MAGMA);  
       REMOVE_BIT(ch->warp, WARP_SCARED);  
       REMOVE_BIT(ch->warp, WARP_GBODY);  
       REMOVE_BIT(ch->warp, WARP_WEAPON);  
       REMOVE_BIT(ch->warp, WARP_STEED);  
       REMOVE_BIT(ch->warp, WARP_TERROR);  
       REMOVE_BIT(ch->warp, WARP_DAMCAP);  
       REMOVE_BIT(ch->warp, WARP_HUNT);  
       REMOVE_BIT(ch->warp, WARP_REGEN);  
       REMOVE_BIT(ch->warp, WARP_STAMINA);  
       REMOVE_BIT(ch->warp, WARP_QUICKNESS);  
       REMOVE_BIT(ch->warp, WARP_BADBREATH);  
       REMOVE_BIT(ch->warp, WARP_SPIKETAIL);  
       REMOVE_BIT(ch->warp, WARP_VENOMTONG);  
       REMOVE_BIT(ch->warp, WARP_STRONGARMS);  
       REMOVE_BIT(ch->warp, WARP_STUPID);  
       REMOVE_BIT(ch->warp, WARP_STRONGLEGS);  
       REMOVE_BIT(ch->warp, WARP_SBODY);  
       REMOVE_BIT(ch->warp, WARP_INFIRMITY);
//       REMOVE_BIT(ch->warp, WARP_HITROLL);
//       REMOVE_BIT(ch->warp, WARP_DAMROLL);
       ch->pcdata->powers[DPOWER_OBJ_VNUM] = 29663;
       ch->pcdata->stats[DEMON_CURRENT] -= cost;
       ch->warpcount = 0;
       save_char_obj(ch);
       return;
}

void do_obtain( CHAR_DATA *ch, char *argument )
{
    int       newwarp = 0;
    int       warpnum = number_range(1,30);

    if (IS_NPC(ch)) return;

//	if ( !IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

    if (ch->pcdata->stats[DEMON_TOTAL] < 15000 || 
	ch->pcdata->stats[DEMON_CURRENT] < 15000)
    {
	send_to_char("You need 15000 demon points to obtain a new warp!\n\r",ch);
	return;
    }

        if (ch->warpcount >= 10)
        {
	    send_to_char("You have already obtained as many warps as possible.\n\r",ch);
	    return;
	}

             if (warpnum == 1) newwarp = WARP_CBODY;
        else if (warpnum == 2) newwarp = WARP_SBODY;
	else if (warpnum == 3) newwarp = WARP_STRONGARMS;
	else if (warpnum == 4) newwarp = WARP_STRONGLEGS;
	else if (warpnum == 5) newwarp = WARP_VENOMTONG;
	else if (warpnum == 6) newwarp = WARP_SPIKETAIL;
	else if (warpnum == 7) newwarp = WARP_BADBREATH;
	else if (warpnum == 8) newwarp = WARP_QUICKNESS;
	else if (warpnum == 9) newwarp = WARP_STAMINA;
	else if (warpnum == 10) newwarp = WARP_REGEN;
	else if (warpnum == 11) newwarp = WARP_HUNT;
	else if (warpnum == 12) newwarp = WARP_DAMCAP;
	else if (warpnum == 13) newwarp = WARP_TERROR;
	else if (warpnum == 14) newwarp = WARP_REGEN;
	else if (warpnum == 15) newwarp = WARP_STEED;
	else if (warpnum == 16) newwarp = WARP_WEAPON;
	else if (warpnum == 17) newwarp = WARP_INFIRMITY;
	else if (warpnum == 18) newwarp = WARP_GBODY;
	else if (warpnum == 19) newwarp = WARP_SCARED;
	else if (warpnum == 20) newwarp = WARP_MAGMA;
	else if (warpnum == 21) newwarp = WARP_WEAK;
	else if (warpnum == 22) newwarp = WARP_SLOW;
	else if (warpnum == 23) newwarp = WARP_VULNER;
	else if (warpnum == 24) newwarp = WARP_SHARDS;
	else if (warpnum == 25) newwarp = WARP_WINGS;
	else if (warpnum == 26) newwarp = WARP_CLUMSY;
	else if (warpnum == 27) newwarp = WARP_STUPID;
	else if (warpnum == 28) newwarp = WARP_SPOON;
	else if (warpnum == 29) newwarp = WARP_FORK;
	else if (warpnum == 30) newwarp = WARP_KNIFE;
//	else if (warpnum == 31) newwarp = WARP_HITROLL;
//	else if (warpnum == 32) newwarp = WARP_DAMROLL;

    if (IS_SET(ch->warp, newwarp))
    {
      do_obtain(ch,"");
	return;
    }

    SET_BIT(ch->warp, newwarp);
    ch->pcdata->stats[DEMON_CURRENT] -= 15000;
    ch->warpcount += 1;
    send_to_char("You have obtained a new warp!\n\r",ch);
    save_char_obj(ch);
    return;
}

void do_inpart( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
//	if ( !IS_DEMON(ch) )
	{
	    send_to_char("Huh?\n\r", ch);
	    return;
	}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
//      divide_to_char(ch);
      stc("{R---------------------------------------------------------------------------{x\n\r",ch);
      stc(" {WSyntax{D:{w Inpart <person> <power>\n\r", ch );
      stc("{R---------------------------------------------------------------------------\n\r",ch);
      stc("{w Claws          {R[{W 2500{R]{w Blink         {R [{W25000{R]{w Longsword      {R[{W    0{R]{x\n\r",ch);
      stc("{w Demonform      {R[{W25000{R]{w Caust         {R [{W 3000{R]{w Pact           {R[{W*   0{R]{x\n\r",ch);
      stc("{w Fangs          {R[{W 2500{R]{w Confuse       {R [{W*****{R]{w Prince         {R[{W*   0{R]{x\n\r",ch);
      stc("{w Graft          {R[{W20000{R]{w Eyespy        {R [{W*****{R]{w Shortsword     {R[{W    0{R]{x\n\r",ch);
      stc("{w Hooves         {R[{W 1500{R]{w Firewall      {R [{W*****{R]                       {x\n\r",ch);
      stc("{w Horns          {R[{W 2500{R]{w Freezeweapon  {R [{W 3000{R]                       {x\n\r",ch);
      stc("{w Lifespan       {R[{W  100{R]{w Immolate      {R [{W 2500{R]                       {x\n\r",ch);
      stc("{w Might          {R[{W 7500{R]{w Imp           {R [{W10000{R]                       {x\n\r",ch);
      stc("{w Nightsight     {R[{W 3000{R]{w Inferno       {R [{W20000{R]                       {x\n\r",ch);
      stc("{w Shadowsight    {R[{W 7500{R]{w Leap          {R [{W  500{R]                       {x\n\r",ch);
      stc("{w Speed          {R[{W 7500{R]{w Leech         {R [{W15000{R]                       {x\n\r",ch);
      stc("{w Tail           {R[{W 5000{R]{w Magic         {R [{W 1000{R]                       {x\n\r",ch);
      stc("{w Toughness      {R[{W 7500{R]{w Move          {R [{W  500{R]                       {x\n\r",ch);
      stc("{w Truesight      {R[{W 7500{R]{w Scry          {R [{W 7500{R]                       {x\n\r",ch);
      stc("{w Wings          {R[{W 1000{R]{w Shield        {R [{W20000{R]                       {x\n\r",ch);
      stc("{w                     {w   Travel         {R[{W 1500{R]                       {x\n\r",ch);
      stc("{w                     {w   Unnerve        {R[{W 5000{R]                       {x\n\r",ch);
      stc("{w                                                                     {x\n\r",ch);
      stc("{W Those marked * are Demon Lord only.\n\r",ch);
      stc("{W Those marked ***** are still being worked on.\n\r",ch);
      stc("{R---------------------------------------------------------------------------\n\r",ch);
//      divide_to_char(ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }

    if(!IS_CLASS(victim,CLASS_DEMON)) { stc("They must be a demon!\n\r",ch); return; }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (!str_cmp(arg2,"longsword"))
    {
        send_to_char("You have been granted the power to transform into a demonic longsword!\n\r",victim);
        send_to_char("You grant them the power to transform into a demonic longsword.\n\r",ch);
        victim->pcdata->powers[DPOWER_OBJ_VNUM] = 29662;
        do_autosave(victim,"");
        return;
    }

    if (!str_cmp(arg2,"shortsword"))
    {
        send_to_char("You have been granted the power to transform into a demonic shortsword!\n\r",victim);
	  send_to_char("You grant them the power to transform into a demonic shortsword.\n\r",ch);
	  victim->pcdata->powers[DPOWER_OBJ_VNUM] = 29663;
        do_autosave(victim,"");
        return;
    }

    if (!str_cmp(arg2,"pact"))
    {
	if (victim == ch)
	{
	    send_to_char("Not on yourself!\n\r",ch);
	    return;
	}
	if (IS_SET(victim->special, SPC_SIRE))
	{
	    send_to_char("You have lost the power to make pacts!\n\r",victim);
	    send_to_char("You remove their power to make pacts.\n\r",ch);
	    REMOVE_BIT(victim->special, SPC_SIRE);
	}
	else
	{
	    send_to_char("You have been granted the power to make pacts!\n\r",victim);
	    send_to_char("You grant them the power to make pacts.\n\r",ch);
	    SET_BIT(victim->special, SPC_SIRE);
	}
	do_autosave(victim,"");
	return;
    }

    if (!str_cmp(arg2,"prince"))
    {
	if (victim == ch)
	{
	    send_to_char("Not on yourself!\n\r",ch);
	    return;
	}
	if( !IS_SET(ch->special, SPC_DEMON_LORD) )
	{
          send_to_char("Only a Demon Lord has the power to make princes.\n\r",ch);
	    return;
	}
	if (IS_SET(victim->special, SPC_PRINCE))
	{
	    send_to_char("You have lost your princehood!\n\r",victim);
	    send_to_char("You remove their princehood.\n\r",ch);
	    REMOVE_BIT(victim->special, SPC_PRINCE);
	}
	else
	{
	    send_to_char("You have been made a prince!\n\r",victim);
	    send_to_char("You make them a prince.\n\r",ch);
	    SET_BIT(victim->special, SPC_PRINCE);
	}
	do_autosave(victim,"");
	return;
    }

    if (!str_cmp(arg2,"fangs")) 
	{inpart = DEM_FANGS; cost = 2500;}
    else if (!str_cmp(arg2,"immolate"))
	{inpart = DEM_IMMOLATE; cost = 2500;}
    else if (!str_cmp(arg2,"inferno"))
	{inpart = DEM_INFERNO; cost = 20000;}
    else if (!str_cmp(arg2,"caust"))
	{inpart = DEM_CAUST; cost = 3000;}
    else if (!str_cmp(arg2,"freezeweapon"))
	{inpart = DEM_FREEZEWEAPON; cost = 3000;}
    else if (!str_cmp(arg2,"unnerve"))
	{inpart = DEM_UNNERVE; cost = 5000;}
    else if (!str_cmp(arg2,"claws")) 
	{inpart = DEM_CLAWS; cost = 2500;}
    else if (!str_cmp(arg2,"horns")) 
	{inpart = DEM_HORNS; cost = 2500;}
   else if (!str_cmp(arg2, "demonform"))
	{inpart = DEM_FORM; cost = 25000;} 
   else if (!str_cmp(arg2,"tail")) 
	{inpart = DEM_TAIL; cost = 5000;}
    else if (!str_cmp(arg2,"hooves")) 
	{inpart = DEM_HOOVES; cost = 1500;}
    else if (!str_cmp(arg2,"shield")) 
	{inpart = DEM_SHIELD; cost = 20000;}
    else if (!str_cmp(arg2,"nightsight")) 
	{inpart = DEM_EYES; cost = 3000;}
    else if (!str_cmp(arg2,"shadowsight")) 
	{inpart = DEM_SHADOWSIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"wings")) 
	{inpart = DEM_WINGS; cost = 1000;}
    else if (!str_cmp(arg2,"might")) 
	{inpart = DEM_MIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"toughness")) 
	{inpart = DEM_TOUGH; cost = 7500;}
    else if (!str_cmp(arg2,"speed"))
	{inpart = DEM_SPEED; cost = 7500;}
    else if (!str_cmp(arg2,"travel")) 
	{inpart = DEM_TRAVEL; cost = 1500;}
    else if (!str_cmp(arg2,"scry")) 
	{inpart = DEM_SCRY; cost = 7500;}
    else if (!str_cmp(arg2,"move")) 
	{inpart = DEM_MOVE; cost = 500;}
    else if (!str_cmp(arg2,"leap")) 
	{inpart = DEM_LEAP; cost = 500;}
    else if (!str_cmp(arg2,"magic")) 
	{inpart = DEM_MAGIC; cost = 1000;}
    else if (!str_cmp(arg2,"truesight"))
        {inpart = DEM_TRUESIGHT; cost = 7500;}
    else if (!str_cmp(arg2,"graft"))
	{inpart = DEM_GRAFT; cost = 20000;}
    else if (!str_cmp(arg2,"leech"))
	{inpart = DEM_LEECH; cost = 15000;}
    else if (!str_cmp(arg2,"blink"))
        {inpart = DEM_BLINK; cost = 25000;}
    else if (!str_cmp(arg2,"lifespan")) 
	{inpart = DEM_LIFESPAN; cost = 100;}
    else if (!str_cmp(arg2,"imp")) 
	{inpart = DEM_IMP; cost = 10000;}
/*
    else if (!str_cmp(arg2,"confuse")) 
	{inpart = DEM_CONFUSE; cost = 20000;}*/
    else
    {
	do_inpart(ch,"");
	return;
    }
    if (IS_DEMPOWER(victim, inpart))
    {
	send_to_char("They have already got that power.\n\r",ch);
	return;
    }
    if (ch->pcdata->stats[DEMON_TOTAL] < cost || 
	ch->pcdata->stats[DEMON_CURRENT] < cost)
    {
	send_to_char("You have insufficient power to inpart that gift.\n\r",ch);
	return;
    }
    SET_BIT(victim->pcdata->powers[DPOWER_FLAGS], inpart);
    ch->pcdata->stats[DEMON_CURRENT] -= cost;
    if (victim != ch)
    {
        send_to_char("You have been granted a demonic gift from your patron!\n\r",victim);
    }
    send_to_char("Ok.\n\r",ch);
    if (victim != ch) do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}
/*
void do_demonarmour( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    
    if (arg[0] == '\0')
    {
    send_to_char("Command: demonarmor <black/red> <piece>.\n\r", ch);
    return;
    }

    if (arg2[0] == '\0')
    {
	send_to_char("Please specify which piece of demon armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor.\n\r",ch);
	return;
    }

    if      (!str_cmp(arg2,"ring"     ) && !str_cmp(arg,"black")) vnum = 29650;
    else if (!str_cmp(arg2,"collar"   ) && !str_cmp(arg,"black")) vnum = 29651;
    else if (!str_cmp(arg2,"plate"    ) && !str_cmp(arg,"black")) vnum = 29652;
    else if (!str_cmp(arg2,"helmet"   ) && !str_cmp(arg,"black")) vnum = 29653;
    else if (!str_cmp(arg2,"leggings" ) && !str_cmp(arg,"black")) vnum = 29654;
    else if (!str_cmp(arg2,"boots"    ) && !str_cmp(arg,"black")) vnum = 29655;
    else if (!str_cmp(arg2,"gauntlets") && !str_cmp(arg,"black")) vnum = 29656;
    else if (!str_cmp(arg2,"sleeves"  ) && !str_cmp(arg,"black")) vnum = 29657;
    else if (!str_cmp(arg2,"cape"     ) && !str_cmp(arg,"black")) vnum = 29658;
    else if (!str_cmp(arg2,"belt"     ) && !str_cmp(arg,"black")) vnum = 29659;
    else if (!str_cmp(arg2,"bracer"   ) && !str_cmp(arg,"black")) vnum = 29660;
    else if (!str_cmp(arg2,"visor"    ) && !str_cmp(arg,"black")) vnum = 29661;
    else if (!str_cmp(arg2,"ring"     ) && !str_cmp(arg,"red")) vnum = 27650;
    else if (!str_cmp(arg2,"collar"   ) && !str_cmp(arg,"red")) vnum = 27651;
    else if (!str_cmp(arg2,"plate"    ) && !str_cmp(arg,"red")) vnum = 27652;
    else if (!str_cmp(arg2,"helmet"   ) && !str_cmp(arg,"red")) vnum = 27653;
    else if (!str_cmp(arg2,"leggings" ) && !str_cmp(arg,"red")) vnum = 27654;
    else if (!str_cmp(arg2,"boots"    ) && !str_cmp(arg,"red")) vnum = 27655;
    else if (!str_cmp(arg2,"gauntlets") && !str_cmp(arg,"red")) vnum = 27656;
    else if (!str_cmp(arg2,"sleeves"  ) && !str_cmp(arg,"red")) vnum = 27657;
    else if (!str_cmp(arg2,"cape"     ) && !str_cmp(arg,"red")) vnum = 27658;
    else if (!str_cmp(arg2,"belt"     ) && !str_cmp(arg,"red")) vnum = 27659;
    else if (!str_cmp(arg2,"bracer"   ) && !str_cmp(arg,"red")) vnum = 27660;
    else if (!str_cmp(arg2,"visor"    ) && !str_cmp(arg,"red")) vnum = 27661;
    else
    {
	send_to_char("Please specify which piece of demon armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor.\n\r",ch);
	return;
    }

    if ( (ch->pcdata->stats[DEMON_TOTAL] < 5000 || ch->pcdata->stats[DEMON_CURRENT] < 5000)
         && !str_cmp(arg,"black"))
    {
	send_to_char("It costs 5000 points of power to create a piece of black demon armour.\n\r",ch);
	return;
    }
    else if ( (ch->pcdata->stats[DEMON_TOTAL] < 15000 ||
ch->pcdata->stats[DEMON_CURRENT] <
15000)
         && !str_cmp(arg,"red"))
    {
     send_to_char("It costs 15000 points of power to create a piece of red demon armour.\n\r",ch);
     return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
	send_to_char("Missing object, please inform KaVir.\n\r",ch);
	return;
    }
    if ( !str_cmp(arg,"black") )
    {
    ch->pcdata->stats[DEMON_TOTAL]   -= 5000;
    ch->pcdata->stats[DEMON_CURRENT] -= 5000;
    }
    else if ( !str_cmp(arg,"red") )
    {
    ch->pcdata->stats[DEMON_TOTAL]   -= 15000;
    ch->pcdata->stats[DEMON_CURRENT] -= 15000;
    }
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;
}
*/

void do_demonarmour( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

//    if (!IS_DEMON(ch))
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
        if (!IS_SET(ch->act, PLR_CHAMPION) )
        {
            send_to_char("Huh?\n\r",ch);
            return;
        }
    }

    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char("Syntax: demonarmour <colour> <armour>\n\r",ch);
        send_to_char("Colour can be one of: Black Red Copper Bronze Brass.\n\r",ch);
        send_to_char("Armour can be one of: Ring Collar Plate Helmet Leggings Boots Gauntlets\n\r",ch);
	send_to_char("                      Sleeves Shield Cape Belt Bracer Visor.\n\r",ch);
        return;
    }
    if (str_cmp(arg1,"black") && str_cmp(arg1,"red") && str_cmp(arg1,"copper")
    && str_cmp(arg1,"bronze") && str_cmp(arg1,"brass"))
    {
        send_to_char("Syntax: demonarmour <colour> <armour>\n\r",ch);
        send_to_char("Colour can be one of: Black Red Copper Bronze Brass.\n\r",ch);
        send_to_char("Colour can be one of: Black Grey Purple Red Brass.\n\r",ch);
        send_to_char("Armour can be one of: Ring Collar Plate Helmet Leggings Boots Gauntlets\n\r",ch);
	send_to_char("                      Sleeves Shield Cape Belt Bracer Visor.\n\r",ch);
        return;
    }
    if      (!str_cmp(arg2,"ring"     )) vnum = 29650;
    else if (!str_cmp(arg2,"collar"   )) vnum = 29651;
    else if (!str_cmp(arg2,"plate"    )) vnum = 29652;
    else if (!str_cmp(arg2,"helmet"   )) vnum = 29653;
    else if (!str_cmp(arg2,"leggings" )) vnum = 29654;
    else if (!str_cmp(arg2,"boots"    )) vnum = 29655;
    else if (!str_cmp(arg2,"gauntlets")) vnum = 29656;
    else if (!str_cmp(arg2,"sleeves"  )) vnum = 29657;
    else if (!str_cmp(arg2,"shield"   )) vnum = 29666;
    else if (!str_cmp(arg2,"cape"     )) vnum = 29658;
    else if (!str_cmp(arg2,"belt"     )) vnum = 29659;
    else if (!str_cmp(arg2,"bracer"   )) vnum = 29660;
    else if (!str_cmp(arg2,"visor"    )) vnum = 29661;
    else if (!str_cmp(arg2,"longsword")) vnum = 29662;
    else if (!str_cmp(arg2,"shortsword")) vnum = 29663;
    else
    {
        send_to_char("Syntax: demonarmour <colour> <armour>\n\r",ch);
        send_to_char("Colour can be one of: Black Red Copper Bronze Brass.\n\r",ch);
        send_to_char("Armour can be one of: Ring Collar Plate Helmet Leggings Boots Gauntlets\n\r",ch);
	send_to_char("                      Sleeves Shield Cape Belt Bracer Visor.\n\r",ch);
        return;
    }

/*
    if ( !IS_SET(ch->special, SPC_DEMON_LORD) && !str_cmp(arg1,"brass"))
	{
	 send_to_char("Only Fallen Lords can make brass armour.\n\r",ch);
	 return;
	}
    if ( !IS_SET(ch->special, SPC_PRINCE) && !str_cmp(arg1,"bronze"))
	{
	send_to_char(
	"Only Fallen Princes and Lords can make bronze armor.\n\r",ch);
	return;
	}
    if ( !IS_SET(ch->special, SPC_SIRE) && !str_cmp(arg1,"copper"))
	{
	send_to_char(
	"Only Fallen Sires, Princes and Lords can make copper armor.\n\r",ch);
	return;
	}*/

    if (!str_cmp(arg1,"black") && (ch->pcdata->stats[DEMON_CURRENT] < 5000))
    {
        send_to_char("It costs 5000 points to create a piece of black demon armour.\n\r",ch);
        return;
   }
    if (!str_cmp(arg1,"red") && (ch->pcdata->stats[DEMON_CURRENT] < 10000))
    {
        if (ch->pcdata->quest < 250) { stc("It also costs 250 qp per piece to make brass armor.\n\r",ch); return; }
        send_to_char("It costs 10000 power to create a piece of red demon armour.\n\r",ch);
        return;
    }
    if (!str_cmp(arg1,"copper") && (ch->pcdata->stats[DEMON_CURRENT] < 15000) )
    {
        if (ch->pcdata->quest < 500) { stc("It also costs 500 qp per piece to make brass armor.\n\r",ch); return; }
        send_to_char("It costs 15000 power to create a piece of copper demon armour.\n\r",ch);
        return;
    }
    if (!str_cmp(arg1,"bronze") && (ch->pcdata->stats[DEMON_CURRENT] < 20000) )

    {
        if (ch->pcdata->quest < 750) { stc("It also costs 750 qp per piece to make brass armor.\n\r",ch); return; }
        send_to_char("It costs 20000 power to create a piece of bronze demon armour.\n\r",ch);
        return;
    }
    if (!str_cmp(arg1,"brass") && (ch->pcdata->stats[DEMON_CURRENT] < 25000) )
    {
        if (ch->pcdata->quest < 1000) { stc("It also costs 1000 qp per piece to make brass armor.\n\r",ch); return; }
        send_to_char("It costs 25000 power to create a piece of brass demon armour.\n\r",ch);
        return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)   
    {
        send_to_char("Missing object, please inform an Immortal.\n\r",ch);
        return;
    }
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    if (!str_cmp(arg1,"black")) 
    {obj->specpower = ARM_BLACK;ch->pcdata->stats[DEMON_CURRENT] -= 5000;}
    if (!str_cmp(arg1,"red"))
    {obj->specpower = ARM_RED;ch->pcdata->stats[DEMON_CURRENT] -= 10000;ch->pcdata->quest -= 250;}
    if (!str_cmp(arg1,"copper"))
    {obj->specpower = ARM_COPPER;ch->pcdata->stats[DEMON_CURRENT] -= 15000;ch->pcdata->quest -= 500;}
    if (!str_cmp(arg1,"bronze"))
    {obj->specpower = ARM_BRONZE;ch->pcdata->stats[DEMON_CURRENT] -= 20000;ch->pcdata->quest -= 750;}
    if (!str_cmp(arg1,"brass"))
    {obj->specpower = ARM_BRASS;ch->pcdata->stats[DEMON_CURRENT] -= 25000;ch->pcdata->quest -= 1000;}
        

    sprintf( buf, obj->name, arg1, "demon" );
    free_string( obj->name );
    obj->name = str_dup( buf );
        
    sprintf( buf, obj->short_descr, arg1, "demon" );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
     
    sprintf( buf, obj->description, arg1, "demon" );
    free_string( obj->description );
    obj->description = str_dup( buf );

    free_string( obj->questmaker );
    obj->questmaker = str_dup( ch->name );
        
    free_string( obj->questowner );
    obj->questowner = str_dup( ch->name );

    SET_BIT( obj->spectype, SITEM_DEMONIC );
    SET_BIT( obj->extra_flags, ITEM_ANTI_GOOD );

    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;
}



void do_travel( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
//    if (IS_DEMON(ch) )
    if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_TRAVEL))
	{
	    send_to_char("You haven't been granted the gift of travel.\n\r",ch);
	    return;
	}
    }
    else
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
        if (ch->fight_timer > 0)
        {  
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
        }


    if (!str_cmp( arg, "hell" ) )
    {
	send_to_char("You sink down into the ground.\n\r",ch);
	act("$n sinks down into the ground.\n\r",ch,NULL,NULL,TO_ROOM);
	char_from_room(ch);
	char_to_room(ch, get_room_index(ROOM_VNUM_HELL));
	do_look(ch,"");
	send_to_char("You rise up out of the ground.\n\r",ch);
	act("$n rises up out of the ground.\n\r",ch,NULL,NULL,TO_ROOM);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot travel to yourself!\n\r", ch );
	return;
    }

//   if( !IS_NPC(victim) && !IS_DEMON(victim))
    if (!IS_NPC(victim) && !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("You cannot travel to non-demon players.\n\r",ch);
	WAIT_STATE(ch, 24);
	return;
    }

 if ( IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)   
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_TELEPORT) 
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) ) )
    {
    	send_to_char( "You are unable to focus on their location.\n\r", ch );
        return;
    }

    if (IS_IMMUNE(victim, IMM_TRAVEL))
    {
	send_to_char("I don't think they want you to do that.\n\r",ch);
	return;
    }   

    if (victim->in_room == NULL)
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    level = victim->level;

    if(ch->pcdata->stats[DEMON_CURRENT] < level)
    {
	send_to_char("You don't have enough Dps for that.\n\r",ch);
	return;
    }

    send_to_char("You sink into the ground.\n\r",ch);
    act("$n sinks into the ground.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You rise up out of the ground.\n\r",ch);
    act("$n rises up out of the ground.",ch,NULL,NULL,TO_ROOM);
    if(IS_NPC(victim))
    {
   	if (ch->move < 250)
        {
            stc("You do not have enough movement to do that!\n\r",ch);
            return;
        }
    ch->move -= (ch->max_move /10);
    if(ch->move < 0) ch->move = 0;   
   }	     
    return;
}

void do_horns( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DEMON) 
    && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_HORNS) && IS_CLASS(ch,CLASS_DEMON))
    {
	send_to_char("You haven't been granted the gift of horns.\n\r",ch);
	return;
    }
    else if (IS_CLASS(ch,CLASS_VAMPIRE) && !IS_VAMPAFF(ch,VAM_VICISSITUDE) )
    {
	send_to_char("You have not mastered the Vicissitude discipline.\n\r", ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_HORNS) )
    {
	send_to_char("Your horns slide back into your head.\n\r",ch);
	act("$n's horns slide back into $s head.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	return;
    }
    send_to_char("Your horns extend out of your head.\n\r",ch);
    act("A pair of pointed horns extend from $n's head.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
    return;
}
void do_tail( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

//    if (!IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_TAIL))
    {
	send_to_char("You haven't been granted the gift of a tail.\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_TAIL) )
    {
	send_to_char("Your tail slides back into your back.\n\r",ch);
	act("$n's tail slides back into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
	return;
    }

    send_to_char("Your tail slides out of your back side.\n\r",ch);
    act("A huge tail extends from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
    return;
}
/*
void do_axe( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    
     if ( IS_NPC(ch) )
	return;
 
	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

    if (!IS_CLASS(ch, CLASS_DEMON)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
if (!IS_SET(ch->warp, WARP_WEAPON))
        {

stc("You need the warp weapon to use this power.\n\r",ch);
return;
}
 
save_char_obj(ch);

  if ((obj = create_object(get_obj_index( 30055),60)) == NULL)
    {
        send_to_char( "Error - Please inform Sage.\n\r", ch);
	return;
    }

    if ( IS_EXTRA(ch, EXTRA_AXE) )
    {
      ch->pcdata->obj_vnum = 0;
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
      REMOVE_BIT(ch->extra, EXTRA_AXE);
      ch->pcdata->chobj = NULL;
      obj->chobj = NULL;
      free_string(ch->morph);
      ch->morph = str_dup("");
      act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
      act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
      extract_obj( obj );
      return;
                
    }
    
    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = ch->pcdata->powers[VPOWER_OBJ_VNUM];
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    SET_BIT(ch->extra, EXTRA_AXE);
    free_string(ch->morph);
    ch->morph = str_dup("An axe of khrone");
    obj_to_room(obj,ch->in_room);
    return;
}
*/

void do_steed(CHAR_DATA *ch, char *argument )
{
 
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    if (IS_NPC(ch)) return;
//    if (!IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }
   
if (!IS_SET(ch->warp, WARP_STEED))
        {

stc("You need the warp steed to use this power.\n\r",ch);
return;
}



/*
    if !IS_SET(ch->warp, WARP_STEED))
    {
    send_to_char("You require warp steed to use this command.\n\r",ch);
    return;
    }
 */
    if (ch->pcdata->followers > 5)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;
 
    victim=create_mobile( get_mob_index( MOB_VNUM_STEED ) );
    victim->level = 200;
    victim->hit = 5000;
    victim->max_hit = 5000;
    victim->hitroll = 50;
    victim->damroll = 50;
    victim->armor = 300;
    SET_BIT(victim->act, ACT_NOEXP); 

    strcpy(buf,"Rise creature of darkness bow before me!");
    do_say( ch, buf );
 
    send_to_char( "A huge demon bursts from the ground and bows before you.\n\r",ch );
    act( "$N bursts from the ground and bows before $n.", ch, NULL,victim, TO_ROOM );
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
//    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    WAIT_STATE( ch, 12 );
    return;

}

void do_dbreath( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
 
char buf[MAX_STRING_LENGTH];
 
int dam = 0;
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch,CLASS_DEMON))
{
send_to_char("Huh?\n\r",ch);
return;
}
if ( IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->warp, WARP_BADBREATH)) 
{
send_to_char("You need bad breath warp to use this.\n\r",ch);
    return;
    }
 
 
if ( ( victim = ch->fighting ) == NULL )
{
send_to_char( "You aren't fighting anyone.\n\r", ch );
      return;
 
}

WAIT_STATE( ch, 5 );
    if (!IS_NPC(victim))
    {
    dam = ch->spl[RED_MAGIC] * 5;
    }
    else if (IS_NPC(victim))
    {
    dam = ch->spl[RED_MAGIC] * 6;  
    }
 if (is_safe(ch,victim)== TRUE) return;
dam += number_range(1,30);
if ( dam <= 0 )
   dam = 1;
sprintf(buf,"Your blast of putrid breath hits $N incredibly hard! [%d]\n\r",dam); act(buf,ch,NULL,victim,TO_CHAR);
sprintf(buf,"$n's blast of putrid breath hits you incredibly hard! [%d]\n\r",dam); act(buf,ch,NULL,victim,TO_VICT);
sprintf(buf,"$n's blast of putrid breath hits $N incredibly hard! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT);
 

send_to_char("\n\r",ch);
victim->hit -= dam;
return;
}
/*moved*/
void do_hooves( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

//    if (!IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_HOOVES))
    {
	send_to_char("You haven't been granted the gift of horns.\n\r",ch);
	return;
    }

    if (IS_DEMAFF(ch,DEM_HOOVES) )
    {
	send_to_char("Your hooves transform into feet.\n\r",ch);
	act("$n's hooves transform back into $s feet.", ch, NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
	return;
    }
    send_to_char("Your feet transform into hooves.\n\r",ch);
    act("$n's feet transform into hooves.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    return;
}

void do_wings( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_DRAGON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_DEMPOWER( ch, DEM_WINGS) && IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("You haven't been granted the gift of wings.\n\r",ch);
	return;
    }

    if (!IS_DRAPOWER( ch, DRA_WINGS) && IS_CLASS(ch, CLASS_DRAGON))
    {
        stc("You haven't gained that power yet.\n\r",ch); return; }

    if (arg[0] != '\0')
    {
	if (!IS_DEMAFF(ch,DEM_WINGS) )
	{
	    send_to_char("First you better get your wings out!\n\r",ch);
	    return;
	}
	if (!str_cmp(arg,"unfold") || !str_cmp(arg,"u"))
	{
	    if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	    {
		send_to_char("But your wings are already unfolded!\n\r",ch);
		return;
	    }
	    send_to_char("Your wings unfold from behind your back.\n\r",ch);
	    act("$n's wings unfold from behind $s back.", ch, NULL, NULL, TO_ROOM);
	    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
            SET_BIT(ch->affected_by,AFF_FLYING);
	    return;
	}
	else if (!str_cmp(arg,"fold") || !str_cmp(arg,"f"))
	{
	    if (!IS_DEMAFF(ch,DEM_UNFOLDED) )
	    {
		send_to_char("But your wings are already folded!\n\r",ch);
		return;
	    }
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
            REMOVE_BIT(ch->affected_by, AFF_FLYING);
	    return;
	}
	else
	{
	    send_to_char("Do you want to FOLD or UNFOLD your wings?\n\r",ch);
	    return;
	}
    }

    if (IS_DEMAFF(ch,DEM_WINGS) )
    {
	if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	    REMOVE_BIT(ch->affected_by, AFF_FLYING);

	}
	send_to_char("Your wings slide into your back.\n\r",ch);
	act("$n's wings slide into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
        REMOVE_BIT(ch->affected_by, AFF_FLYING);
	return;
    }
    send_to_char("Your wings extend from your back.\n\r",ch);
    act("A pair of wings extend from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    return;
}

void do_lifespan( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_DEMPOWER( ch, DEM_LIFESPAN))
    {
	send_to_char("You haven't been granted the gift of lifespan.\n\r",ch);
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

    if (!IS_HEAD(ch,LOST_HEAD))
    {
	send_to_char("You cannot change your lifespan in this form.\n\r",ch);
	return;
    }

         if (!str_cmp(arg,"l") || !str_cmp(arg,"long" )) obj->timer = 0;
    else if (!str_cmp(arg,"s") || !str_cmp(arg,"short")) obj->timer = 1;
    else
    {
	send_to_char("Do you wish to have a long or short lifespan?\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_pact( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    bool      can_sire = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

   

    if (IS_CLASS(ch, CLASS_DEMON)) can_sire = TRUE;

    if (!can_sire)
    {
	send_to_char("You are not able to make a pact.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Make a pact with whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot make a pact with yourself.\n\r", ch );
	return;
    }

    if (victim->class_2 > 0) { stc("Not on muticlassed players.\n\r",ch); return; }
  
    if (IS_CLASS(victim, CLASS_DEMON))
    {
        victim->clan = str_dup(ch->clan);
        victim->lord = str_dup(ch->lord);
	stc("You induct them into your clan.\n",ch);
	act("$n inducts you into $s clan.",ch,NULL,victim,TO_VICT);
        return;
    }

    if( victim->max_hit < 1500)
        {
        send_to_char("They do not have 1500 hit points.\n\r", ch);
        return;
        }            
	
    if (victim->class != 0) {
	send_to_char("Not on them!\n\r", ch );
	return;}

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only make pacts with avatars.\n\r", ch );
	return;
    }

    if (!IS_IMMUNE(victim,IMM_DEMON))
    {
	send_to_char( "You cannot make a pact with an unwilling person.\n\r", ch );
	return;
    }

    if (ch->exp < 666)
    {
	send_to_char("You cannot afford the 666 exp to make a pact.\n\r",ch);
	return;
    }

	if (!IS_EVIL(victim)) {
	send_to_char("They must be evil!\n\r", ch );
		return;}

    ch->exp = ch->exp - 666;
    act("You make $N a demonic champion!", ch, NULL, victim, TO_CHAR);
    act("$n makes $N a demonic champion!", ch, NULL, victim, TO_NOTVICT);
    act("$n makes you a demonic champion!", ch, NULL, victim, TO_VICT);
    victim->class=CLASS_DEMON;
    victim->lord = str_dup(ch->lord);
    SET_BIT(victim->special, SPC_CHAMPION);

    if (IS_CLASS(victim, CLASS_VAMPIRE)) do_mortalvamp(victim,"");
    REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
    REMOVE_BIT(victim->act, PLR_WIZINVIS);
    victim->pcdata->stats[UNI_RAGE] = 0;

    free_string(victim->morph);

    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_offersoul( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_IMMUNE(ch, IMM_DEMON))
    {
/*
	send_to_char("That would be a very bad idea...\n\r",ch);
	return;
*/
	send_to_char("You will now allow demons to buy your soul.\n\r",ch);
	SET_BIT(ch->immune, IMM_DEMON);
	return;
  
    }
    send_to_char("You will no longer allow demons to buy your soul.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_DEMON);
    return;
}

void do_weaponform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    if (IS_NPC(ch)) return;
//    if (!IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char( "Huh?\n\r", ch);
	return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
	send_to_char( "You cannot do this while polymorphed.\n\r", ch);
	return;
    }
   if (ch->fight_timer > 0)
   {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
   }
        
    ch->pcdata->powers[DPOWER_OBJ_VNUM] = 29662;

    if (IS_SET(ch->warp, WARP_WEAPON))
	ch->pcdata->powers[DPOWER_OBJ_VNUM] = OBJ_VNUM_KHRONE;

    if (ch->pcdata->powers[DPOWER_OBJ_VNUM] < 1)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }
    if ((obj = create_object(get_obj_index(ch->pcdata->powers[DPOWER_OBJ_VNUM]),60)) == NULL)
    {
	send_to_char( "You don't have the ability to change into a weapon.\n\r", ch);
	return;
    }
    if (IS_AFFECTED(ch,AFF_WEBBED))
    {
	send_to_char( "Not with all this sticky webbing on.\n\r", ch);
	return;
    }
    obj_to_room(obj,ch->in_room);
    act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM);
    act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = ch->pcdata->powers[DPOWER_OBJ_VNUM];
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    return;
}

void do_humanform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;
    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("You are already in human form.\n\r",ch);
	return;
    }

    ch->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup("");
    act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
    act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
    extract_obj(obj);
    if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    {
	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_HELL));
    }
    return;
}

void do_champions( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char lord[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->lord) < 2 && !IS_CLASS(ch, CLASS_DEMON) )
    {
	send_to_char("But you don't follow any demon!\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_DEMON)) strcpy(lord,ch->name);
	else strcpy(lord,ch->lord);
    sprintf( buf, "The champions of %s:\n\r", lord );
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [ Hits ] [ Mana ] [ Move ] [   Exp   ] [       Power        ]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( IS_IMMORTAL(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_DEMON) && !IS_SET(gch->special, SPC_CHAMPION) ) 
	    continue;
	if ( !str_cmp(ch->lord,lord) || !str_cmp(ch->name,lord))
	{
	    sprintf( buf,
	    "[%-16s] [%6ld] [%6ld] [%6ld] [%9d] [ %-9d%9d ]\n\r",
		capitalize( gch->name ),
		gch->hit,gch->mana,gch->move,
		gch->exp, gch->pcdata->stats[DEMON_CURRENT], gch->pcdata->stats[DEMON_TOTAL]);
		send_to_char( buf, ch );
	}
    }
    return;
}


void do_eyespy( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;

    if (IS_HEAD(ch,LOST_EYE_L) && IS_HEAD(ch,LOST_EYE_R))
    {
	send_to_char( "But you don't have any more eyes to pluck out!\n\r", ch );
	return;
    }
    if (!IS_HEAD(ch,LOST_EYE_L) && number_range(1,2) == 1)
    {
	act( "You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    else if (!IS_HEAD(ch,LOST_EYE_R))
    {
	act( "You pluck out your right eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s right eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    else
    {
	act( "You pluck out your left eyeball and throw it to the ground.", ch, NULL, NULL, TO_CHAR );
	act( "$n plucks out $s left eyeball and throws it to the ground.", ch, NULL, NULL, TO_ROOM );
    }
    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
    {
	make_part(ch,"eyeball");
	return;
    }

    victim = create_mobile( get_mob_index( MOB_VNUM_EYE ) );
    if (victim == NULL)
    {send_to_char("Error - please inform KaVir.\n\r",ch); return;}

    char_to_room( victim, ch->in_room );

    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    return;
}


/*
void do_gifts( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int improve;
    int cost;
    int max;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Gifts: Strength of Satan, Speed of Satan, Stake, Cone of fire, True Form\n\r");
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"Strength"))
	{
	    send_to_char("Strength of Satan: You are as strong as your creator.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Speed"))
	{
	    send_to_char("Speed: The Speed of satan you dodge most attacks wth incredible speed.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Stake"))
	{
	    send_to_char("Stake: You have the ability to make a silver stake.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Cone"))
	{
	    send_to_char("Cone: You have the power to breath a cone fo fire.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Form"))
	{
	    send_to_char("Form: The power to go into true demon form.\n\r",ch);
	    return;
	}
	
	sprintf(buf,"Gifts: Strength of Satan, Speed of Satan, Stake, Cone of fire, True Form\n\r");
        send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"improve"))
    {
	     if (!str_cmp(arg1,"strength"   )) {improve = DEM_STRENGTH;max=1;}
	else if (!str_cmp(arg1,"speed"   )) {improve = DEM_SPEEDY;max=1;}
	else if (!str_cmp(arg1,"stake"   )) {improve = DEM_STAKE;max=1;}
	else if (!str_cmp(arg1,"cone"    )) {improve = DEM_CONE;max=1;}
	else if (!str_cmp(arg1,"form" )) {improve = DEM_FORM;max=1;}
	else
	{
	    send_to_char("You can improve: Strength Speed Stake Cone Form\n\r",ch);
	    return;
	}
	cost = (ch->pcdata->powers[improve]+1) * 50;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained this gift.\n\r");
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your gifts.\n\r", cost);
	    send_to_char(buf,ch);
	    return;
	}
       ch->pcdata->powers[improve] += 1;
       ch->practice -= cost;
       sprintf(buf,"You improve your gifts.\n\r");
       send_to_char(buf,ch);
      }
}
*/

void do_cone( CHAR_DATA *ch, char *argument ) {
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;

//    if (!IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
    send_to_char("Cone who?\n\r", ch);
    return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
	if (ch->mana < 100) {
		send_to_char("You don't have enough mana.\n\r", ch);
		return;}

    if ( ( sn = skill_lookup( "cone" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 1.0;
	level = level * 1.0;
	act("You Blast $N with a cone of fire.",ch,NULL,victim,TO_CHAR);
	act("$n Blasts you with a cone of fire.",ch,NULL,victim,TO_VICT);
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 10 );
	ch->mana = ch->mana - 100;
    return;
}

void do_dstake( CHAR_DATA *ch, char *argument )

{
OBJ_DATA *obj;

if (IS_NPC(ch)) return;

if (!IS_CLASS(ch, CLASS_DEMON))
    {

send_to_char("Huh?\n\r",ch);
return;
}

if ( 60 > ch->practice)
{
send_to_char("It costs 60 points of primal to create a stake.\n\r",ch);
return;
}
ch->practice   -= 60;
obj = create_object(get_obj_index(OBJ_VNUM_STAKE)  ,0 );
if (IS_SET(obj->quest, QUEST_ARTIFACT))
REMOVE_BIT(obj->quest, QUEST_ARTIFACT);
obj_to_char(obj, ch);
act("A Stake appears in your hands in a flash of
light.",ch,NULL,NULL,TO_CHAR);
act("A Stake appears in $n's hands in a flash of
light.",ch,NULL,NULL,TO_ROOM);
return;
}

void do_dimp( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
        
//    if ( !IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
      stc("Huh?\n\r",ch); 
      return;
    }
    
    if (!IS_DEMPOWER( ch, DEM_IMP))
    {
      stc("You haven't been granted the gift of Imp.\n\r",ch);
      return;
    }

    if ( IS_SET(ch->flag2, AFF2_IMP) )
    {
        send_to_char("I shall return when you call me, Master.\n\r",ch);
        send_to_char("The little imp takes off from your shoulder and flies away.\n\r", ch );
        act("The little imp takes off from $n's shoulder and flies away.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->flag2, AFF2_IMP);
        return;
    }
        send_to_char("A little imp flies down from the sky and lands on your shoulder.\n\r", ch );
        act("A little imp flies down from the sky and lands on $n's shoulder.",ch,NULL,NULL,TO_ROOM);
        send_to_char("I have arrived, Master.\n\r",ch);
        SET_BIT(ch->flag2, AFF2_IMP);
        return;
}


void do_dimpsend( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];   
    OBJ_DATA *obj;
    CHAR_DATA *victim;
      
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
      
    if ( IS_NPC(ch) ) return;
//    if ( !IS_DEMON(ch) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (!IS_DEMPOWER( ch, DEM_IMP))
    {
      stc("You haven't been granted the gift of Imp.\n\r",ch);
      return;
    }
    if ( arg1[0] == '\0' )
    { 
        send_to_char( "Have your imp send which object?\n\r", ch );
        return;
    } 
    
    if ( arg2[0] == '\0' )
    {
        send_to_char( "Send what to whom?\n\r", ch );
        return;
    }
    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
        send_to_char( "You are not carrying that item.\n\r", ch );
        return;
    }
    victim = get_char_world(ch,arg2);
    if ( (victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They arent here.\n\r", ch );
        return;
    }
    
    if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_TRANSPORT))
    {
        send_to_char( "They dont want anything transported to them.\n\r", ch );
        return;
    }
     
    act("You give $p to your little pet imp.",ch,obj,NULL,TO_CHAR);
    act("$n gives $p to the little imp on $s shoulder.",ch,obj,NULL,TO_ROOM);
    send_to_char( "The little imp takes off from your shoulder and flies away.\n\r", ch );
    act("The little imp takes off from $n's shoulder and flies away.",ch,NULL,NULL,TO_ROOM);
    obj_from_char(obj);
    obj_to_char(obj,victim);
    send_to_char( "A little imp flies down from the sky and lands on your shoulder.\n\r", victim );
    act("A little imp flies down from the sky and lands on $n's shoulder.",victim,NULL,NULL,TO_ROOM);
    act("The little imp gives you $p.",victim,obj,NULL,TO_CHAR);
    act("The little imp gives $n $p.",victim,obj,NULL,TO_ROOM);
    send_to_char( "The little imp takes off from your shoulder and flies away.\n\r", victim );
    act("The little imp takes off from $n's shoulder and flies away.",victim,NULL,NULL,TO_ROOM);
    send_to_char( "Your little imp flies down from the sky and returns to your shoulder.\n\r", ch );
    act("The little imp flies down from the sky and returns to $n's shoulder.",ch,NULL,NULL,TO_ROOM);
    do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}

void do_confuse(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ( IS_NPC( ch ) ) return;

//    if (!IS_DEMON( ch ) )
    if (!IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
/*
    if (!IS_DEMPOWER(ch, DEM_CONFUSE) )
    {
        send_to_char("You have not been inparted with the ability to confuse.\n\r", ch);
        return;
    }*/

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char("You are not fighting anyone.\n\r", ch);
        return;
    }

    if (ch->move < 75)
    {
        send_to_char("You need 75 move to confuse your opponent.\n\r", ch);
        return;
    }

    WAIT_STATE(ch, 16);

    if ( IS_NPC(victim) )
    {
        act("You attempt to confuse $N.",ch,NULL,victim,TO_CHAR);
        act("$n attempts to confuse you.",ch,NULL,victim,TO_VICT);
        act("$n attempts to confuse $N, $N flees in confusion.",ch,NULL,victim,TO_NOTVICT);
        do_flee( victim, "" );
        ch->move -=75;
        return;
    }

    if ( !IS_NPC(victim) )
    {
        act("You attempt to confuse $N.",ch,NULL,victim,TO_CHAR);
        act("$n attempts to confuse you.",ch,NULL,victim,TO_VICT);
        act("$n attempts to confuse $N.",ch,NULL,victim,TO_NOTVICT);

        ch->move -=75;

        if ( number_percent() > 25 )
        {
            act("You fail to confuse $N.",ch,NULL,victim,TO_CHAR);
            act("$n fails to confuse you and just looks stupid.",ch,NULL,victim,TO_VICT);
            act("$n fails to confuse $N and looks stupid.",ch,NULL,victim,TO_NOTVICT);
            return;
        }

        else
        {
            do_flee(victim,"");
            return;
        }
    }
    return;
}

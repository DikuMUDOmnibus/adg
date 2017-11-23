/***************************************************************************
 *  God Wars Mud originally written by KaVir aka Richard Woolcock.         *
 *  Changes done to the code done by Sage aka Walter Howard, this mud is   *
 *  for the public, however if you run this code it means you agree        *
 *  to the license.low, license.gw, and license.merc have fun. :)          *
***************************************************************************/
/*Demon's Sage/Dunkirk*/

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

void frost_breath args(( CHAR_DATA *ch, CHAR_DATA *victim, bool all));
DECLARE_DO_FUN( do_stance );


void do_immolate(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

	
        if (!IS_DEMON(ch) )
{
        stc("Huh?\n\r",ch);
        return;
        }

    if (!IS_DEMPOWER(ch, DEM_IMMOLATE))
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to immolate?\n\r", ch);
	return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
	send_to_char("You are not carrying that item.\n\r", ch);
	return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That is not a weapon!\n\r", ch);
	return;
    }

    if (dice(1, 100) == 1)
    {
	act("$p explodes in a burst of flames.", ch,obj, NULL, TO_ROOM);
	act("$p explodes in a burst of flames.", ch,obj, NULL, TO_CHAR);
	damage(ch,ch,500,gsn_inferno);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_FLAMING))
    {
	act("$p is already flaming.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("$p bursts into flames.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n bursts into flames.", ch, obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_FLAMING);
}


void do_leech(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim = NULL;
    char buf[MAX_STRING_LENGTH];

        if (!IS_DEMON(ch) )
{
        stc("Huh?\n\r",ch);
        return;
        }

    if (!IS_DEMPOWER( ch, DEM_LEECH))
    {

	stc("You do not have that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	stc("Who's life do you wish to leech off of?\n\r", ch);
	return;
    }

    if ((victim = get_char_room(ch, argument)) == NULL)
    {
	    stc("They aren't here.\n\r", ch);
	    return;
    }

      if (is_safe(ch, victim)) return;

	WAIT_STATE(ch, 12);

	act("$n stares intently at $N.", ch, NULL, victim, TO_NOTVICT);
	act("You stare intently at $N.", ch, NULL, victim, TO_CHAR);
	act("$n stares intently at you.", ch, NULL, victim, TO_VICT);

      if (victim->hit >= 1000)
      {
/*
	  sh_int power;
	  sh_int dam;
	  power = (ch->pcdata->stats[UNI_GEN];
	  dam   = dice(power, power + 2);

	  power = number_range(45,50);
	  dam = dice(power / 5, power);
	  if (dam > 300) dam = 300 + (dam / 10);
*/
	  int dam;
          dam = (number_range(500,100) * number_range(1,3));
	  hurt_person(ch,victim,dam,TRUE);
	  ch->hit = (ch->hit + dam);
	  ch->fighting = victim;


	  if (ch->hit >= ch->max_hit + 1000)
		ch->hit = ch->max_hit + 1000;

	  sprintf(buf,"You absorb %d hitpoints.\n\r",dam);
	  stc(buf,ch);
	  sprintf(buf,"%s absorbed %d of your hitpoints!\n\r",ch->name,dam);
	  stc(buf,victim);
	  }
      else stc("Nothing seemed to happen.\n\r",ch);
	return;

}

void do_unnerve(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

        if (!IS_DEMON(ch) )
{
        stc("Huh?\n\r",ch);
        return;
        }

    if (!IS_DEMPOWER(ch, DEM_UNNERVE))
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	stc("Who do you wish to unnerve?\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room(ch, argument) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }
/*Changed Leon 7.2.00 to fix a crash bug.*/
    WAIT_STATE(ch, 12);
    act("#wYou exclaim '#rXeus Dominus Mortai!#w'.#n",ch,NULL,NULL,TO_CHAR);
    act("#w$n exclaims '#rXeus Dominus Mortai!#w'.#n",ch,NULL,NULL,TO_ROOM);
    do_stance(victim, "");
}

void do_wfreeze(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

        if (!IS_DEMON(ch) )
{
        stc("Huh?\n\r",ch);
        return;
        }

    if (!IS_DEMPOWER(ch, DEM_FREEZEWEAPON))
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to freeze?\n\r", ch);
	return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
	send_to_char("You are not carrying that item.\n\r", ch);
	return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That is not a weapon!\n\r", ch);
	return;
    }

    if (dice(1, 100) == 1)
    {
	act("$p freezes and shatters.", ch,obj, NULL, TO_ROOM);
	act("$p freezes and shatters.", ch,obj, NULL, TO_CHAR);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_FROST))
    {
	act("$p is already frozen.", ch, obj, NULL, TO_CHAR);
	return;
    }

    act("$p is surrounded by ice crystals.", ch, obj, NULL, TO_CHAR);
    act("$p, carried by $n is surrounded by ice crystals.", ch, obj, NULL,
TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_FROST);
}

void do_graft(CHAR_DATA *ch, char *argument)
{
/* Changed by Kyrlin to fix the arm use bug 31/01/00 */
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if (IS_NPC(ch)) return; 
        if (!IS_DEMON(ch) )
{
	stc("Huh?\n\r",ch);
	return;
	}

	if (!IS_DEMPOWER( ch, DEM_GRAFT) && IS_CLASS(ch,CLASS_DEMON))
    {
	stc("You need to get graft first.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0')
    {
	stc("Which limb do you wish to graft on to yourself?\n\r",ch);
	return;
    }

    if ( ( obj = get_obj_carry(ch,arg) ) == NULL )
    {
	stc("You do not have that limb.\n\r",ch);
	return;
    }
	if(!str_cmp(obj->name,"arm"))
//    if ( str_cmp(obj->name, "arm mob" ) )
    {
	stc("That's not even an arm!\n\r",ch);
	return;
    }

    if (IS_SET(ch->newbits, THIRD_HAND) && IS_SET(ch->newbits, FOURTH_HAND))
    {
	stc("You already have four arms!\n\r",ch);
	return;
    }


    if (!IS_SET(ch->newbits, THIRD_HAND))
    {
	act("You graft an arm onto your body.",ch,NULL,obj,TO_CHAR);
	act("$n grafts an arm onto $m body.",ch,NULL,obj,TO_ROOM);
	SET_BIT(ch->newbits, THIRD_HAND);
	WAIT_STATE(ch, 18);
	extract_obj(obj);
	return;    
    }    


    if (!IS_SET(ch->newbits, FOURTH_HAND))
    {
	act("You graft an arm onto your body.",ch,NULL,obj,TO_CHAR);
	act("$n grafts an arm onto $m body.",ch,NULL,obj,TO_ROOM);
	SET_BIT(ch->newbits, FOURTH_HAND);
	WAIT_STATE(ch, 18);
	extract_obj(obj);
	return;    
    }

    return;
}

void do_caust(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

        if (!IS_DEMON(ch) )
{
        stc("Huh?\n\r",ch);
        return;
        }

    if (!IS_DEMPOWER(ch, DEM_CAUST))
    {
	stc("You have not been granted that power.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to caust?\n\r", ch);
	return;
    }

    if ( (obj = get_obj_carry(ch, argument)) == NULL)
    {
	send_to_char("You are not carrying that item.\n\r", ch);
	return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That is not a weapon!\n\r", ch);
	return;
    }

    if (dice(1, 100) == 1)
    {
	act("$p is disintegrated by $n's poison.", ch,obj, NULL, TO_ROOM);
	act("Your poison eats through $p.", ch,obj, NULL, TO_CHAR);
	extract_obj(obj);
	return;
    }

    if (IS_WEAP(obj, WEAPON_POISON))
    {
	act("$p is already coated with deadly poison.", ch, obj, NULL,
TO_CHAR);
	return;
    }

    act("You run your tongue along $p, coating it with a sickly venom.",
ch, obj, NULL, TO_CHAR);
    act("$n runs $m tongue along $p, coating it with a sickly venom.", ch,
obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_POISON);
    obj->value[1] += ch->pcdata->stats[UNI_GEN];
    obj->value[2] += ch->pcdata->stats[UNI_GEN];
}


void do_blink(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if (!IS_DEMPOWER(ch, DEM_BLINK))
    {
      stc("You have not been granted the power of Blinking.\n\r",ch);
      return;
    }
    
    if (argument[0] == '\0')
    {
        stc("Who do you want to use blink on?\n\r", ch);
        return;
    } 
 
    if ((victim = get_char_room(ch, argument)) == NULL)
    {
            stc("They aren't here.\n\r", ch);
            return;
    }
     
    if (is_safe(ch, victim)) return;

    if (ch == victim)
    {
        stc("You can't use blink on yourself.\n\r", ch);
        return;
    }

    if(ch->fighting == NULL)
    {
	WAIT_STATE(ch, 30);
	SET_BIT(ch->flag2, AFF2_BLINK_1ST_RD);
	stop_fighting(ch, TRUE);
	stc("You pop out of existance.\n\r", ch);
	act("$n pops out of existance.", ch, NULL, NULL, TO_ROOM);
	ch->blinkykill = victim;
	ch->move -= 1000;
	if(ch->move < 0) ch->move = 0;
	WAIT_STATE(ch,30);
	return;
    }
    WAIT_STATE(ch, 30);
    victim = ch->fighting;
    SET_BIT(ch->flag2, AFF2_BLINK_2ND_RD);
    stop_fighting(ch, TRUE);
    stc("You pop out of existance.\n\r", ch);
    act("$n pops out of existance.", ch, NULL, NULL, TO_ROOM);
    ch->blinkykill = victim;
    ch->move -= 1000;
    if(ch->move < 0) ch->move = 0;
    WAIT_STATE(ch, 30);

}

void do_entomb (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj = NULL;
    OBJ_DATA *objc;
/*
    if (!IS_DEMPOWER(ch, DEM_ENTOMB) && ch->power[DISC_DAEM_GELU] < 6)
    {
        stc("You have not been granted that power.\n\r",ch);
        return;
    }
*/
    if (ch->in_room->sector_type == SECT_AIR)
    {
        send_to_char("This ability cannot be used in the air.\n\r", ch);
        return;
    }
/*     
    if (!TIME_UP(ch, TIMER_ENTOMB)
        && ch->level < 12)
    {
        send_to_char("This ability can only be used once every 12 hours.\n\r", ch);
        return;
    }
*/
    if ((objc = get_obj_list(ch,"walln",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
      obj=create_object(get_obj_index(30043),0);
      sprintf(buf,"A wall of ice is here, blocking your exit north.");
      free_string(obj->description);
      obj->description = str_dup(buf);
      obj_to_room(obj,ch->in_room);   
      obj->timer=5;
      obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"walls",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
     obj=create_object(get_obj_index(30044),0);
      sprintf(buf,"A wall of ice is here, blocking your exit south.");
      free_string(obj->description);
      obj->description = str_dup(buf);
      obj_to_room(obj,ch->in_room);
      obj->timer=5;
      obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"walle",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
      obj=create_object(get_obj_index(30045),0);
      sprintf(buf,"A wall of ice is here, blocking your exit east.");
      free_string(obj->description);
      obj->description = str_dup(buf);
      obj_to_room(obj,ch->in_room);
      obj->timer=5;
      obj->item_type = ITEM_WALL;
   } 
    if ((objc = get_obj_list(ch,"wallw",ch->in_room->contents)) !=
NULL) objc=NULL;
    else
    {
      obj=create_object(get_obj_index(30046),0);
      sprintf(buf,"A wall of ice is here, blocking your exit west.");
      free_string(obj->description);
      obj->description = str_dup(buf);
      obj_to_room(obj,ch->in_room);
      obj->timer=5;
      obj->item_type = ITEM_WALL;
    }
      
    ch->hit += ch->max_hit * 0.1;   
       if (ch->hit > ch->max_hit)
                ch->hit = ch->max_hit;
    act("$n calls forth sheets of ice, blocking your path in everydirection.", ch, NULL, NULL, TO_ROOM);
    send_to_char("You call forth sheets of ice to entrap your prey.\n\r",ch);
    send_to_char("You grin wickedly.\n\r",ch);
    do_say(ch,"Prepare to die! Muahahahaha.");
    WAIT_STATE(ch, 10);
    return;
}


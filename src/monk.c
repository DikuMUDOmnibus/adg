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
#include "monk.h"

int     dambonus        args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance ) );


int find_dam( CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool foot )
{
   OBJ_DATA *boots; 
   int stance;

    dam += char_damroll(ch);

    if (dam == 0) dam = 1;

    if ( !IS_AWAKE(victim) )
	dam *= 2;

  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) )
  {
    if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3)
        dam *= 1.2;
    if (ch->chi[CURRENT] > 2)
	dam *= ( ch->chi[CURRENT] / 2 );
  }

    if ( !IS_NPC(victim) && IS_WEREWOLF(victim) )
    {
/* Removed Toaster 10/05/00 since why do monks to extra damage to WW 
	if (IS_SET(victim->special, SPC_WOLFMAN)) dam *= 0.3;
*/
/*	if (victim->pcdata->powers[WPOWER_BOAR] > 2 ) dam *= 0.3;*/
      if ( foot == TRUE )
      {
	  if ( ( boots = get_eq_char( ch, WEAR_FEET ) ) != NULL 
	      && IS_SET(boots->spectype, SITEM_SILVER)) dam *= 2;
	}
	if ( foot == FALSE )
	{
	  if ( ( boots = get_eq_char( ch, WEAR_HANDS ) ) != NULL 
	      && IS_SET(boots->spectype, SITEM_SILVER)) dam *= 2;
	}
    }

    if ( !IS_NPC(ch) ) dam = dam + (dam * ((ch->wpn[0]+1) / 100));

    if ( !IS_NPC(ch) )
    {
	stance = ch->stance[0];
	if ( IS_STANCE(ch, STANCE_NORMAL) ) dam *= 1.25;
	else dam = dambonus(ch,victim,dam,stance);
    }

    if ( dam > 1400 )
      dam = number_range( 1340, 1420 );

    if ( dam <= 0 )
	dam = 2;

    return dam;

}

void do_guide( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
//    int ch_age = years_old(ch);
    char      arg [MAX_INPUT_LENGTH];
    bool      can_sire = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_MONK(ch) || (ch->pcdata->stats[UNI_GEN] < 1) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[UNI_CURRENT] == -1) 
	ch->pcdata->stats[UNI_CURRENT] = ch->pcdata->stats[UNI_AFF]; 

    if (IS_SET(ch->special,SPC_PRINCE)) can_sire = TRUE;
    if (IS_SET(ch->special,SPC_SIRE)) can_sire = TRUE;

    if (ch->pcdata->stats[UNI_GEN] < 3 || ch->pcdata->stats[UNI_GEN] > 6) can_sire = FALSE;
    if (ch->pcdata->stats[UNI_GEN] > 3) can_sire = FALSE;
    else can_sire = TRUE;

    if (!can_sire)
    {
	send_to_char("You are not able to guide new monks.\n\r",ch);
	return;
    }

    if (!str_cmp(ch->clan,"") && ch->pcdata->stats[UNI_GEN] == 2)
    {
	send_to_char( "First you must become Abbot of a Temple.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Guide whom?\n\r", ch );
	return;
    }

/*
    if ( ch_age < 50 )
    {
       send_to_char("They must be 50 to become a monk!\n\r", ch);
       return;
    }
*/
    
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

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot guide yourself.\n\r", ch );
	return;
    }

    if (IS_MONK(victim) )
    {
	send_to_char( "They are a monk already.\n\r", ch );
	return;
    }


    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) ) 
    {
	send_to_char( "You can only guide avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to teach vampires!\n\r", ch );
	return;
    }

    if ( victim->class != 0 )
    {
      stc("They are already classed.\n\r",ch);
      return;
    }


    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot guide an unwilling person.\n\r", ch );
	return;
    }

    if( victim->max_hit < 1500 )
    {
        send_to_char("They do not have 1500 hitpoints.\n\r", ch);
        return;
    }

    if (ch->exp < 100000)
    {
	send_to_char("You cannot afford the 100000 exp required to guide them.\n\r",ch);
	return;
    }


    if (victim->exp < 100000)
    {
	send_to_char("They cannot afford the 100000 exp required to be guided from you.\n\r",ch);
	return;
    }

    ch->exp -= 100000;
    victim->exp -= 100000;

    act("You guide $N in the ways of god.", ch, NULL, victim, TO_CHAR);
    act("$n guide $N in the ways of god.", ch, NULL, victim, TO_NOTVICT);
    act("$n guide you in the way of god.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now a monk.\n\r", victim );
    victim->pcdata->stats[UNI_GEN] = ch->pcdata->stats[UNI_GEN] + 1;
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->class    = CLASS_MONK;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_flaminghands( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
     return;

    if (!IS_CLASS(ch, CLASS_MONK))
    {
	  send_to_char("Huh?\n\r",ch);
	  return;
    }

    if (ch->pcdata->powers[PMONK] <5)
    {
	  send_to_char("You do not have the mantra of Flaming Hands.\n\r",ch);
	  return;
    }

    if (IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	  REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
        send_to_char("Your hands are no longer engulfed by flames.\n\r",ch);
	  act("$n's hands are no longer engulfed by flames.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
    if (!IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	  SET_BIT(ch->newbits, NEW_MONKFLAME);
	  send_to_char("Your hands are engulfed by flames!\n\r",ch);
	  act("$n's hands are engulfed by flames!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
}

void do_adamantium( CHAR_DATA *ch, char *argument)
{
     if ( IS_NPC(ch) )
     return;

    if (!IS_CLASS(ch, CLASS_MONK))
    {
	  send_to_char("Huh?\n\r",ch);
	  return;
    }

    if (ch->monkab[BODY] < 1)
    {
	  send_to_char("You have not learned this ability yet.\n\r",ch);
	  return;
    }

    if (IS_SET(ch->newbits, NEW_MONKADAM))
    {
	  REMOVE_BIT(ch->newbits, NEW_MONKADAM);
        send_to_char("Your hands resume their normal toughness.\n\r",ch);
	  act("$n's hands resume their normal toughness.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }

    if (!IS_SET(ch->newbits, NEW_MONKADAM))
    {
	  SET_BIT(ch->newbits, NEW_MONKADAM);
	  send_to_char("Your hands turn as hard as adamantium!\n\r",ch);
	  act("$n's hands turn as hard as adamantium!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
}


/*Changed Toaster 25/02/00 so celest costs 10% of max move*/
void do_celestial( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_MONK))
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if (ch->monkab[SPIRIT] < 4)
	{
	   send_to_char("You must obtain level 4 in Spirit to use Celestial Path.\n\r",ch);
	   return;
	}

    if (ch->focus[CURRENT] < 10)
    {
	stc("You require atleast 10 focus to use this power!\n\r", ch);
        return;
    }
    if (ch->move < 250)
	{
	    stc("You do not have enough vitality to do that!\n\r",ch);
	    return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open a celestial path to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
	if  (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
	{
	send_to_char("There summon is off!\n\r",ch);
	return;
}
   
 if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }

    if (victim->in_room->vnum == ch->in_room->vnum)
    {
	  send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    if (IS_NPC(victim))
    {
    sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->short_descr);
    send_to_char(buf, ch);
    }

    if (!IS_NPC(victim))
    {
    sprintf(buf,"You concentrate and open a Celestial path to %s!\n\r",victim->name);
    send_to_char(buf, ch);
    }
    act("$n appears before you through a Celestial portal!\n\r",ch,NULL,NULL,TO_ROOM);
    do_look(ch, "auto");
    ch->focus[CURRENT] -= 10;
    ch->move -= (ch->max_move /10);
    if(ch->move < 0) ch->move = 0;
    return;
}

void do_godsheal( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch))
	return;
if (!IS_CLASS(ch, CLASS_MONK))
        {
         send_to_char("Huh?\n\r",ch);
           return;
        }


     if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 12 )

    {
	send_to_char("You need to obtain heal to use heal.\n\r",ch);
	return;
    }

    if (ch->mana < 300)
    {
	send_to_char("You don't have enough energy to use heal.\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("You are surrounded in a glowing halo of energy.\n\r",ch);
	act("$n is surrounded by a glowing halo, and looks very relaxed.\n\r",ch,NULL,NULL,TO_ROOM);
        ch->hit += 150;
        ch->mana -= 400;
	WAIT_STATE(ch,12);
	return;
    }

    if (ch->fighting == NULL)
    {
	send_to_char("You feel a mighty force lay his hands on you.\n\r",ch);
	act("$n is surrounded by a glowing halo, and looks very relaxed.\n\r",ch,NULL,NULL,TO_ROOM);
	ch->hit += 500;
	ch->mana -= 400;
	WAIT_STATE(ch,8);
	return;
    }
  }

void do_deathtouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->monkab[SPIRIT] < 4 )
    {
	stc("You need to obtain level 4 spirit to use Death touch.\n\r",ch);
	return;
    }

     if ( ch->focus[CURRENT] < 20 )
     {
	stc("You need atleast 20 focus points to use th is power.\n\r", ch);
	return;
     }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("They are not here.\n\r", ch);
	return;
    }
    
    if (is_safe(ch,victim) ) return;

    if ( victim == ch )
    {
	stc("That doesn't seem like a good idea.\n\r",ch);
	return;
    }

    WAIT_STATE(ch, 12);
    act("You place your hands on $N's head and channel negative energy into $m.",ch,NULL,victim,TO_CHAR);
    act("$n places $s hands on your head, and you scream in utter pain.",ch,NULL,victim,TO_VICT);
    act("$n places $s hands on $N's head and $N screams in pain.",ch,NULL,victim,TO_ROOM);
    SET_BIT(victim->monkstuff, MONK_DEATH);
	update_pos(victim);
    ch->focus[CURRENT] -= 20;
    return;
}

void do_healingtouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->monkab[SPIRIT] < 3 )
    {
	stc("You need to obtain level 3 spirit to use Healing touch.\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("They are not here.\n\r", ch);
	return;
    }

    WAIT_STATE(ch, 12);
    if ( victim == ch )
    {
	stc("You focus your energy, and magical sparks leap out of your body.\n\r",ch);
	act("$n concentrates, and magical sparks leap out of $s body.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->monkstuff, MONK_HEAL);
        return;
    }

    act("You place your hands on $N's head and focus your energy on $M.",ch,NULL,victim,TO_CHAR);
    act("$n places $s hands on your head and you feel warmer.",ch,NULL,victim,TO_VICT);
    act("$n places $s hands on $N's head and concentrates.",ch,NULL,victim,TO_ROOM);
    SET_BIT(victim->monkstuff, MONK_HEAL);
    return;

}


void do_spiritpower( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_MONK) || ch->monkab[BODY] < 3 )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( IS_SET(ch->newbits, NEW_POWER) )
    {
	stc("Your spiritual power fades.\n\r",ch);
	ch->damroll -= 50;
	ch->hitroll -= 50;
	REMOVE_BIT(ch->newbits, NEW_POWER);
	return;
    }

    if ( ch->move < 100 )
    {
	stc("You are too exhausted.\n\r",ch);
	return;
    }

    SET_BIT(ch->newbits, NEW_POWER);
    ch->damroll += 50;
    ch->hitroll += 50;
    ch->move -= 25;
    stc("Your body pulses with spiritual energy.\n\r",ch);
    return;

 return;

}
    
void do_relax( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_MONK) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->chi[CURRENT] < 1 )
    {
	stc("You are already totally relaxed.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	stc("You cannot relax your ch'i while fighting!\n\r",ch);
	return;
    }

    ch->chi[CURRENT]--;
    WAIT_STATE(ch, 12);
    stc("You breathe deeply and relax your focus.\n\r",ch);
    act("$n looks more relaxed.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_chi( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_MONK) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->fighting == NULL )
    {
	stc("You must be fighting to focus your ch'i.\n\r",ch);
	return;
    }

    if ( ch->chi[CURRENT] >= ch->chi[MAXIMUM] )
    {
	if (ch->chi[MAXIMUM] == 0)
	  stc("You have not gained any control of your ch'i.\n\r",ch);
	else
	{
	  sprintf(buf,"You cannot focus your ch'i past a level of %d.",ch->chi[MAXIMUM]);
	  stc(buf,ch);
	}
	return;
    }

//    if ( ch->move < 500 + ((ch->chi[CURRENT]+1) * 20) )
    if ( ch->move < 500 )
    {
	stc("You are too exhausted.\n\r",ch);
	return;
    }

    if ( ch->chi[CURRENT] == 0 )
    {
      stc("Your body flickers with energy.\n\r",ch);
      act("$n's body flickers with energy.",ch,NULL,NULL,TO_ROOM);
    }
    if ( ch->chi[CURRENT] == 1 )
    {
	stc("Your body pulses with energy.\n\r",ch);
	act("$n's body pulses with energy.",ch,NULL,NULL,TO_ROOM);
    }
    if ( ch->chi[CURRENT] == 2 )
    {
	stc("Your body glows with blue energy.\n\r",ch);
	act("$n's body glows with blue energy.",ch,NULL,NULL,TO_ROOM);
    }
    if ( ch->chi[CURRENT] == 3 )
    {
	stc("Your body glows bright red.\n\r",ch);
	act("$n's body glows bright red.",ch,NULL,NULL,TO_ROOM);
    }
    if ( ch->chi[CURRENT] == 4 )
    {
	stc("Your body flashes with power.\n\r",ch);
	act("$n's body flashes with power.",ch,NULL,NULL,TO_ROOM);
    }
    if ( ch->chi[CURRENT] == 5 )
    {
	stc("Your body emits sparks of energy as you fully focus your ch'i.\n\r",ch);
	act("$n's body emits sparks of energy.",ch,NULL,NULL,TO_ROOM);
    }
    WAIT_STATE(ch, 12);
    ch->chi[CURRENT]++;
    ch->move -= ((ch->chi[CURRENT] * 5));
/*Changed Toaster 26/02/00 to reduce move costs for monks
    ch->move -= ((ch->chi[CURRENT] * 20));
*/
    return;

}

void do_shinkick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = TRUE;
    int dam;
	
	if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_MONK))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }
    /*WAIT_STATE( ch, skill_table[gsn_shinkick].beats );
    */
    WAIT_STATE(ch, 12);
    dam = number_range(1,6);

    dam = find_dam( ch, victim, dam, foot );   

    if ( ch->monkcrap == 0 )
    {
	stc("Your aura glows light blue for a moment.\n\r",ch);
	act("$n's aura glows a light blue for a moment.",ch,NULL,victim,TO_VICT);
	SET_BIT(ch->monkcrap, COMB_SHIN);
    }
    else if ( ch->monkcrap > 0 )
    {
	stc("Your aura fades away.\n\r",ch);
	act("$n's aura fades away.",ch,NULL,victim,TO_VICT);
        ch->monkcrap = 0;
    }

    act("You deliver a powerful blow to $N's shin.",ch,NULL,victim,TO_CHAR);
    act("$n delivers a powerful blow to your shin.",ch,NULL,victim,TO_VICT);
    act("$n delivers a powerful blow to $N's shin.",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_shinkick );
    if ( number_range(1, (7 - ch->chi[CURRENT]) ) == 1 )
      victim->position = POS_STUNNED;
    return;
}

void do_palmstrike( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
    bool foot = FALSE;

    one_argument(argument,arg);

    if ( IS_NPC(ch) ) return;
    if (!IS_CLASS(ch,CLASS_MONK))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot palm strike yourself!\n\r", ch );
	return;
    }

    if (is_safe(ch,victim)) return;

    if ( victim->position < POS_FIGHTING )
    {
	send_to_char( "You can only palm strike someone who is standing.\n\r", ch );
	return;
    }

    act("You slam your palm into $N's chest.",ch,NULL,victim,TO_CHAR);
    act("$n slams $s palm into your chest.",ch,NULL,victim,TO_VICT);
    act("$n slams $s palm into $N's chest.",ch,NULL,victim,TO_NOTVICT);
    /*
    WAIT_STATE( ch, skill_table[gsn_palmstrike].beats );
*/    
	WAIT_STATE(ch, 11);
	dam = number_range(1,4);

    find_dam( ch, victim, dam, foot );
    dam /= 2;

    damage(ch, victim, dam, gsn_palmstrike);
    if (victim == NULL || victim->position == POS_DEAD || dam < 1) return;
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
    act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
    victim->position = POS_STUNNED;
    return;
}

void do_knee( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = TRUE;
    int dam;

    if ( !IS_FS(ch, TECH_KNEE) )
    {
	stc("You haven't learned that technique.\n\r",ch);
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    /*WAIT_STATE( ch, skill_table[gsn_knee].beats );
    */
    WAIT_STATE(ch, 11);
    dam = number_range(1,6);

    dam = find_dam( ch, victim, dam, foot );   

    if ( IS_SET(ch->monkcrap, COMB_SHIN) && !IS_SET(ch->monkcrap, COMB_KNEE ) )
    {
	stc("Your aura glows dark blue.\n\r",ch);
	act("$n's aura glows dark blue.",ch,NULL,victim,TO_VICT);
        SET_BIT(ch->monkcrap, COMB_KNEE);
    }
    else if ( ch->monkcrap > 0 )
    {
	stc("Your aura slowly fades away.\n\r",ch);
	act("$n's aura slowly fades away.\n\r",ch,NULL,victim,TO_VICT);
        ch->monkcrap = 0;
    }

    act("You leap up and slam your knee into $N's chin.",ch,NULL,victim,TO_CHAR);
    act("$n leaps up and slams $s knee into your chin.",ch,NULL,victim,TO_VICT);
    act("$n leaps up and slams $s knee into $N's chin.",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_knee );
    return;
}

void do_sweep( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = TRUE;
    int dam;

    if ( !IS_FS(ch, TECH_SWEEP) )
    {
        stc("You haven't learned that technique.\n\r",ch);
        return;
    }  

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    /*WAIT_STATE( ch, skill_table[gsn_monksweep].beats );
*/      WAIT_STATE(ch, 11);
	dam = number_range(1,4);

    dam = find_dam( ch, victim, dam, foot );

    act("You crouch down and sweep $N's legs out from under $M.",ch,NULL,victim,TO_CHAR);
    act("$n crouches down and sweeps your legs out from under you",ch,NULL,victim,TO_VICT);
    act("$n crouches down and sweeps $N's legs out from under $M",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_monksweep );
    return;
}

void do_elbow( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = FALSE;
    int dam;

    if ( !IS_FS(ch, TECH_ELBOW) )
    {
	stc("You haven't learned that technique.\n\r",ch);
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    /*WAIT_STATE( ch, skill_table[gsn_elbow].beats );
  */
    WAIT_STATE(ch, 11);
    dam = number_range(1,6);

    dam = find_dam( ch, victim, dam, foot );   

    act("You ram your elbow into $N's face.",ch,NULL,victim,TO_CHAR);
    act("$n rams $s elbow into your face.",ch,NULL,victim,TO_VICT);
    act("$n rams $s elbow into $N's face.",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_elbow );
    return;
}


void do_thrustkick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = TRUE;
    int dam;

    if ( !IS_FS(ch, TECH_THRUST) )
    {
        stc("You haven't learned that technique.\n\r",ch);
        return;
    } 

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    /*WAIT_STATE( ch, skill_table[gsn_thrustkick].beats );
*/    
	WAIT_STATE(ch, 11);
	dam = number_range(1,5);

    dam = find_dam( ch, victim, dam, foot );   

    if ( ch->monkcrap == 0 && !IS_SET(ch->monkcrap, COMB_THRUST1) )
    {
	stc("Your aura glows dark blue for a moment.\n\r",ch);
	act("$n's aura glows dark blue for a moment.",ch,NULL,victim,TO_VICT);
	act("$n's aura glows dark blue for a moment.",ch,NULL,victim,TO_NOTVICT);
	SET_BIT(ch->monkcrap, COMB_THRUST1);
    }
    else if ( IS_SET(ch->monkcrap, COMB_THRUST1) && !IS_SET(ch->monkcrap,COMB_THRUST2) )
    {
	stc("Your aura flashes bright blue.\n\r",ch);
	act("$n's aura flashes bright blue.",ch,NULL,victim,TO_VICT);
	act("$n's aura flashes bright blue.",ch,NULL,victim,TO_NOTVICT);
	SET_BIT(ch->monkcrap, COMB_THRUST2);
    }
    else if ( ch->monkcrap != 0 )
    {
	stc("Your aura fades.\n\r",ch);
	act("$n's aura fades.",ch,NULL,victim,TO_VICT);
	act("$n's aura fades.",ch,NULL,victim,TO_NOTVICT);
	ch->monkcrap = 0;
    }

    act("You deliver a thrust kick to $N's chest.",ch,NULL,victim,TO_CHAR);
    act("$n delivers a powerful thrust kick to your chest.",ch,NULL,victim,TO_VICT);
    act("$n delivers a powerful thrust kick to $N's chest.",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_thrustkick );
    return;
}

void do_spinkick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = TRUE;
    int dam;

    if ( !IS_FS(ch, TECH_SPIN) )
    {
        stc("You haven't learned that technique.\n\r",ch);
        return;
    } 

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    /*WAIT_STATE( ch, skill_table[gsn_spinkick].beats );
*/    
	WAIT_STATE(ch, 11);
	dam = number_range(1,4);

    dam = find_dam( ch, victim, dam, foot );   

    if ( IS_COMB(ch, COMB_THRUST1) && IS_COMB(ch, COMB_THRUST2) )
    {
	act("Your aura pulsates and channels energy into your attack.",ch,NULL,NULL,TO_CHAR);
	act("$n's aura pulsates and channels energy into $s attack.",ch,NULL,victim,TO_VICT);
	act("$n's aura pulsates and channels energy into $s attack.",ch,NULL,victim,TO_NOTVICT);
	ch->monkcrap = 0;
	dam /= 1.2;
	damage( ch, victim, dam, gsn_lightningkick );
    if ( ch->chi[CURRENT] > 1 )
    {
	dam /= 1.2;
	damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 2 )
    {
	dam /= 1.2;
        damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 3 )
    {
        dam /= 1.2;
        damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 4 )
    {
	dam /= 1.2;
        damage( ch, victim, dam, gsn_lightningkick );
    }
    if ( ch->chi[CURRENT] > 5 )
    {
        dam /= 1.2;
        damage( ch, victim, dam, gsn_lightningkick );
    }
	return;
    }
    else if ( IS_SET(ch->monkcrap, COMB_SHIN) && IS_SET(ch->monkcrap, COMB_KNEE ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *mount;
	int number_hit = 0;

	ch->monkcrap = 0;
	stc("You spin around and around, going berserk.\n\r",ch);
	act("$n body speeds up, and $e spins around rapidly.",ch,NULL,victim,TO_ROOM);	
	for ( vch = char_list; vch != NULL; vch = vch_next )
    	{
		vch_next	= vch->next;
		if (number_hit > (ch->chi[CURRENT] + 1)) continue;
		if ( vch->in_room == NULL )
		    continue;
		if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
		    continue;
		if ( ch == vch )
		    continue;
		if ( vch->in_room == ch->in_room )
		{
		    if ( ( mount = ch->mount ) != NULL ) {if ( mount == vch ) continue;}
		    if (can_see(ch,vch))
		    {
			one_hit( ch, vch, gsn_tornadokick, 0 );
	                          number_hit++;
		    }
		}
	    }
       return;
    }

    act("You spin around and deliver a blow to $N's head.",ch,NULL,victim,TO_CHAR);
    act("$n spins around and delivers a blow to your head.",ch,NULL,victim,TO_VICT);
    act("$n spins around and delivers a devastating blow to $N's head.",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_spinkick );
    return;
}

void do_backfist( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool foot = FALSE;
    int dam;

    if ( !IS_FS(ch, TECH_BACK) )
    {
        stc("You haven't learned that technique.\n\r",ch);
        return;
    } 

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    /*WAIT_STATE( ch, skill_table[gsn_backfist].beats );
*/    
	WAIT_STATE(ch, 11);
	dam = number_range(1,5);

    dam = find_dam( ch, victim, dam, foot );   

    act("You spin around and slam $N's jaw with your fist.",ch,NULL,victim,TO_CHAR);
    act("$n spins around and slams you in the jaw with $s fist.",ch,NULL,victim,TO_VICT);
    act("$n spins around and slams $N in the jaw with $s fist.",ch,NULL,victim,TO_NOTVICT);
    damage( ch, victim, dam, gsn_backfist );
    return;
}


void do_learn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char lin[MAX_STRING_LENGTH];
    int inpart = 0;
    int cost = 50000;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MONK))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


sprintf(lin,"===============================================================================\n\r");

    if ( arg[0] == '\0' )
    {
      stc(" Syntax: Learn <fight|techniques|abilities|chi> <power|chi>\n\r",ch);
      stc("================================| Fight Styles |==============================\n\r",ch);
	sprintf(buf,"       Trip     [%s] Kick      [%s] Bash     [%s] Elbow    [%s] Knee     [%s]   \n\r",
            IS_FS(ch, FS_TRIP) ? "*" : " ",
       	    IS_FS(ch, FS_KICK) ? "*" : " ",
            IS_FS(ch, FS_BASH) ? "*" : " ",
            IS_FS(ch, FS_ELBOW) ? "*" : " ", 
            IS_FS(ch, FS_KNEE) ? "*" : " ");
	stc(buf,ch);
	sprintf(buf,"       Disarm   [%s] Bite      [%s] Dirt     [%s] Grapple  [%s] Punch    [%s]   \n\r",
		IS_FS(ch, FS_DISARM) ? "*" : " ",
		IS_FS(ch, FS_BITE) ? "*" : " ",
		IS_FS(ch, FS_DIRT) ? "*" : " ",
		IS_FS(ch, FS_GRAPPLE) ? "*" : " ",
		IS_FS(ch, FS_PUNCH) ? "*" : " ");
	stc(buf,ch);
	sprintf(buf,"       Rip      [%s] Stamp     [%s] Backfist [%s] Jumpkick [%s] Spinkick [%s]   \n\r",
		IS_FS(ch, FS_RIP) ? "*" : " ",
		IS_FS(ch, FS_STAMP) ? "*" : " ",
		IS_FS(ch, FS_BACKFIST) ? "*" : " ",
		IS_FS(ch, FS_JUMPKICK) ? "*" : " ",
		IS_FS(ch, FS_SPINKICK) ? "*" : " ");
	stc(buf,ch);
	sprintf(buf,"       Sweep    [%s] Charge    [%s] Hurl     [%s] Gouge    [%s] Headbutt [%s]   \n\r",
		IS_FS(ch, FS_SWEEP) ? "*" : " ",
		IS_FS(ch, FS_CHARGE) ? "*" : " ",
		IS_FS(ch, FS_HURL) ? "*" : " ",
		IS_FS(ch, FS_GOUGE) ? "*" : " ",
		IS_FS(ch, FS_HEADBUTT) ? "*" : " ");
	stc(buf,ch);
	stc("===============================================================================\n\r",ch);
	stc("                                   Techniques                                  \n\r",ch);
	stc(lin,ch);
	sprintf(buf,"    Thrust Kick [%s]   Spin Kick   [%s]   Backfist    [%s]   Palm Strike [%s]   \n\r",
	  IS_FS(ch, TECH_THRUST) ? "*" : " ",
	  IS_FS(ch, TECH_SPIN)   ? "*" : " ",
	  IS_FS(ch, TECH_BACK)   ? "*" : " ",
	  IS_FS(ch, TECH_PALM)   ? "*" : " ");
	stc(buf,ch);
	sprintf(buf,"    Elbow       [%s]   Sweep       [%s]   Shin Kick   [%s]   Knee        [%s]   \n\r",
	  IS_FS(ch, TECH_ELBOW)  ? "*" : " ",
          IS_FS(ch, TECH_SWEEP)  ? "*" : " ",
          IS_FS(ch, TECH_SHIN)   ? "*" : " ",
          IS_FS(ch, TECH_KNEE)   ? "*" : " ");
	stc(buf,ch);
	stc(lin,ch);
	stc("                                   Abilities                                   \n\r",ch);
	stc(lin,ch);
	sprintf(buf,"                 Awareness [%s%s%s%s]               Body     [%s%s%s%s]\n\r",
	  ch->monkab[AWARE] > 0 ? "*" : " ",
	  ch->monkab[AWARE] > 1 ? "*" : " ",
	  ch->monkab[AWARE] > 2 ? "*" : " ",
	  ch->monkab[AWARE] > 3 ? "*" : " ",
	  ch->monkab[BODY]  > 0 ? "*" : " ",
          ch->monkab[BODY]  > 1 ? "*" : " ",
          ch->monkab[BODY]  > 2 ? "*" : " ",
          ch->monkab[BODY]  > 3 ? "*" : " ");
	stc(buf, ch);
	sprintf(buf,"		  Combat   [%s%s%s%s]              Spirit    [%s%s%s%s]\n\r",
	  ch->monkab[COMBAT] > 0 ? "*" : " ",
          ch->monkab[COMBAT] > 1 ? "*" : " ",
          ch->monkab[COMBAT] > 2 ? "*" : " ",
          ch->monkab[COMBAT] > 3 ? "*" : " ",
	  ch->monkab[SPIRIT] > 0 ? "*" : " ",
          ch->monkab[SPIRIT] > 1 ? "*" : " ",
          ch->monkab[SPIRIT] > 2 ? "*" : " ",
          ch->monkab[SPIRIT] > 3 ? "*" : " ");
	stc(buf, ch);
	stc(lin,ch);
	stc("                                      Chi\n\r",ch);
	stc(lin,ch);
	sprintf(buf,"                 You have attained a level %d mastery of your Ch'i.\n\r",ch->chi[MAXIMUM]);
        stc(buf,ch);
    if ( ch->chi[CURRENT] > 0 )
    {
	sprintf(buf,"                     Your ch'i is currently active at level %d\n\r",ch->chi[CURRENT]);
	stc(buf,ch);
    }
    else
	stc(" 	 	    You are currently not focusing your ch'i\n\r",ch);
	stc(lin,ch);
	return;
    }
    
  if ( !str_cmp(arg1, "fight" ) )
  {
    if (!str_cmp(arg,"trip")) 
	inpart = FS_TRIP;
    else if (!str_cmp(arg,"kick")) 
	inpart = FS_KICK;
    else if (!str_cmp(arg,"bash")) 
	inpart = FS_BASH;
    else if (!str_cmp(arg,"elbow")) 
	inpart = FS_ELBOW;
    else if (!str_cmp(arg,"knee")) 
	inpart = FS_KNEE;
    else if (!str_cmp(arg,"headbutt")) 
	inpart = FS_HEADBUTT;
    else if (!str_cmp(arg,"disarm")) 
	inpart = FS_DISARM;
    else if (!str_cmp(arg,"bite")) 
	inpart = FS_BITE;
    else if (!str_cmp(arg,"dirt")) 
	inpart = FS_DIRT;
    else if (!str_cmp(arg,"grapple")) 
	inpart = FS_GRAPPLE;
    else if (!str_cmp(arg,"punch"))
	inpart = FS_PUNCH;
    else if (!str_cmp(arg,"gouge")) 
	inpart = FS_GOUGE;
    else if (!str_cmp(arg,"rip")) 
	inpart = FS_RIP;
    else if (!str_cmp(arg,"stamp")) 
	inpart = FS_STAMP;
    else if (!str_cmp(arg,"backfist")) 
	inpart = FS_BACKFIST;
    else if (!str_cmp(arg,"jumpkick"))
	inpart = FS_JUMPKICK;
    else if (!str_cmp(arg,"spinkick"))
      inpart = FS_SPINKICK;
    else if (!str_cmp(arg,"hurl")) 
	inpart = FS_HURL;
    else if (!str_cmp(arg,"sweep"))
	inpart = FS_SWEEP;
    else if (!str_cmp(arg,"charge"))
	inpart = FS_CHARGE;
    else
    {
      do_learn(ch,"");
      return;
    }
  }
  else if ( !strcmp(arg1, "techniques") )
  {
    if (!str_cmp(arg,"knee")) 
	{inpart = TECH_KNEE; cost = 200000;}
    else if (!str_cmp(arg,"spin")) 
	{inpart = TECH_SPIN; cost = 200000;}
    else if (!str_cmp(arg,"backfist")) 
	{inpart = TECH_BACK; cost = 200000;}
    else if (!str_cmp(arg,"elbow")) 
	{inpart = TECH_ELBOW; cost = 200000;}
    else if (!str_cmp(arg,"palm")) 
	{inpart = TECH_PALM; cost = 200000;}
    else if (!str_cmp(arg,"shin")) 
	{inpart = TECH_SHIN; cost = 200000;}
    else if (!str_cmp(arg,"sweep")) 
	{inpart = TECH_SWEEP; cost = 200000;}
    else if (!str_cmp(arg, "thrust"))
	{inpart = TECH_THRUST; cost = 200000;}
    else
    {
      do_learn(ch,"");
      return;
    }
  }    
  else if ( !str_cmp(arg1, "abilities" ) )
  {
         if ( !str_cmp( arg, "body" ) )
	{ inpart = BODY; cost = 500000; }
    else if ( !str_cmp( arg, "combat" ) )
	{ inpart = COMBAT; cost = 500000; }
    else if ( !str_cmp( arg, "spirit" ) )
	{ inpart = SPIRIT; cost = 500000; }
    else if ( !str_cmp( arg, "awareness" ) )
	{ inpart = AWARE; cost = 500000; }
    else
    {
      do_learn(ch,"");
      return;
    }
  }
  else if ( !str_cmp(arg1, "chi" ) )
  {
	inpart = MAXIMUM;
	cost = (ch->chi[inpart]+1) * 1000000;
  }
  else 
  {
    do_learn(ch,"");
    return;
  }

    if ( !str_cmp(arg1, "abilities" ) )
    {
	if (ch->monkab[inpart] >= 4)
	{
	  stc("You have already learned all you can in this ability.\n\r",ch);
	  return;
	}
    
        if ( ch->exp < cost )
        {
          send_to_char("You do not have enough experience to learn that power.\n\r",ch);
          return;
        }

        ch->monkab[inpart]++;
        ch->exp -= cost;
        send_to_char("Ok.\n\r",ch);
        save_char_obj(ch);
        return; 
    }
	    
    else if ( !str_cmp( arg1, "chi" ) )
    {
	if ( ch->chi[MAXIMUM] >= 6 )
	{
	  stc("You have already mastered your ch'i.\n\r",ch);
	  return;
	}

        if ( ch->exp < cost)
	{
	  sprintf(buf,"You need %d experience to gain the next level of mastery of your ch'i.",cost);
	  return;
	}

        ch->exp -= cost;
	ch->chi[MAXIMUM]++;
	save_char_obj(ch);
	stc("You gain more control over your ch'i.\n\r",ch);
	return;
    }
    else
    {
    if (IS_FS(ch, inpart))
    {
	send_to_char("You have already got that power.\n\r",ch);
	return;
    }
    if ( ch->exp < cost )
    {
	send_to_char("You do not have enough experience to learn that power.\n\r",ch);
	return;
    }

    SET_BIT(ch->monkstuff, inpart);
    ch->exp -= cost;
    send_to_char("Ok.\n\r",ch);
    save_char_obj(ch);
    return;
  }
}

void do_blinding( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );
 
  if ( IS_NPC(ch)) return;

  if ( !IS_CLASS(ch, CLASS_MONK))
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( ch->monkab[SPIRIT] < 1 )
  {
    stc("Your spirit is far too weak.\n\r",ch);
    stc("You need a level one mastery of the Spirit to use Blinding Agony.\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    stc("Who do you wish to use Blinding Agony on?\n\r",ch);
    return;
  }

  if ( ch->move < 200 )
  {
    stc("You are far too exhausted to use Blinding Agony.\n\r",ch);
    stc("Try getting 200 movement points.\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }

  if ( is_safe(ch,victim) ) return;

  if ( IS_SET(victim->flag2, AFF_TOTALBLIND) )
  {
    act("You hurl a ball of blinding light at $N.",ch,NULL,victim,TO_CHAR);
    act("$n hurls a ball of blinding light at you.",ch,NULL,victim,TO_VICT); 
    act("$n hurls a ball of blinding light at $N.",ch,NULL,victim,TO_ROOM);
    stc("Nothing happens.\n\r",ch);
    ch->move -= 100;
    return;
  }

  else
  {
    act("You hurl a ball of blinding light at $N.",ch,NULL,victim,TO_CHAR);
    act("$n hurls a ball of blinding light at you.",ch,NULL,victim,TO_VICT);
    act("$n hurls a ball of blinding light at $N.",ch,NULL,victim,TO_ROOM);
    SET_BIT(victim->flag2, AFF_TOTALBLIND);
    ch->move -= 200;
    stc("The searing heat of the ball blinds you.\n\r",victim);
    return;
  }

 return;
}

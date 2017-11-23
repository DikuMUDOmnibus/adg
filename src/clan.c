 
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

/*
 * Local functions.
 */

void	adv_spell_damage	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_affect	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_action	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	birth_write		args( ( CHAR_DATA *ch, char *argument ) );
bool	birth_ok		args( ( CHAR_DATA *ch, char *argument ) );



/* Need to get rid of those flames somehow - KaVir */
void do_smother( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if (IS_NPC( ch )) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Smother whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot smother yourself.\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(victim, AFF_FLAMING))
    {
	send_to_char( "But they are not on fire!\n\r", ch );
	return;
    }

    if ( number_percent() > (ch->level*2) && !IS_IMMORTAL(ch) )
    {
	act( "You try to smother the flames around $N but fail!",  ch, NULL, victim, TO_CHAR    );
	act( "$n tries to smother the flames around you but fails!", ch, NULL, victim, TO_VICT    );
	act( "$n tries to smother the flames around $N but fails!",  ch, NULL, victim, TO_NOTVICT );
	if ( number_percent() > 98 && !IS_AFFECTED(ch,AFF_FLAMING) )
	{
	    act( "A spark of flame from $N's body sets you on fire!",  ch, NULL, victim, TO_CHAR    );
	    act( "A spark of flame from your body sets $n on fire!", ch, NULL, victim, TO_VICT    );
	    act( "A spark of flame from $N's body sets $n on fire!",  ch, NULL, victim, TO_NOTVICT );
	    SET_BIT(ch->affected_by, AFF_FLAMING);
	    do_humanity(ch,"");
	}
	return;
    }

    act( "You manage to smother the flames around $M!",  ch, NULL, victim, TO_CHAR    );
    act( "$n manages to smother the flames around you!", ch, NULL, victim, TO_VICT    );
    act( "$n manages to smother the flames around $N!",  ch, NULL, victim, TO_NOTVICT );
    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
    do_humanity(ch,"");
    return;
}

/* Loads of Vampire powers follow.  KaVir */

void do_fangs( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_FANGS))
	{
	    send_to_char("You haven't been granted the gift of fangs.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	if (ch->pcdata->powers[WPOWER_WOLF] < 2)
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_DRAGON))
    {
        if (!IS_DRAPOWER(ch, DRA_FANGS))
        { stc("You haven't learned that skill yet.\n\r",ch);
          return; }
    }
    else if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
    {
	send_to_char("Your beast won't let you retract your fangs.\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_FANGS) )
    {
	send_to_char("Your fangs slide back into your gums.\n\r",ch);
	act("$n's fangs slide back into $s gums.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
	return;
    }
    send_to_char("Your fangs extend out of your gums.\n\r",ch);
    act("A pair of razor sharp fangs extend from $n's mouth.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    return;
}

void do_shift( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int  toform = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can shift between the following forms: Homid, Glabro, Crinos, Hispo, Lupus.\n\r", ch );
	return;
    }
    if      ( !str_cmp(arg,"homid" ) ) toform = 1;
    else if ( !str_cmp(arg,"glabro") ) toform = 2;
    else if ( !str_cmp(arg,"crinos") ) toform = 3;
    else if ( !str_cmp(arg,"hispo" ) ) toform = 4;
    else if ( !str_cmp(arg,"lupus" ) ) toform = 5;
    else
    {
	send_to_char( "You can shift between the following forms: Homid, Glabro, Crinos, Hispo, Lupus.\n\r", ch );
	return;
    }
    if ( ch->pcdata->stats[UNI_FORM0] < 1 || ch->pcdata->stats[UNI_FORM0] > 5) 
	ch->pcdata->stats[UNI_FORM0] = 1;
    if ( ch->pcdata->stats[UNI_FORM0] == toform )
    {
	send_to_char( "You are already in that form.\n\r", ch );
	return;
    }
    ch->pcdata->stats[UNI_FORM1] = toform;
    return;
}

void do_claws( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_CLAWS))
	{
	    send_to_char("You haven't been granted the gift of claws.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	if (ch->pcdata->powers[WPOWER_WOLF] < 1)
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }
    
    else if (IS_CLASS(ch, CLASS_NINJA))
    {
	if (ch->pcdata->powerss[NPOWER_NINGENNO] < 1)
	{
	    send_to_char("You have not learned the Ningenno principle to 1.\n\r",ch);
	    return;
	}
	if (ch->move < 500)
	{
	    send_to_char("You don't have enough movement points!\n\r", ch);
	    return;
	}
    }
    else if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->powersde[VAM_PROT] < 2)
    {
	send_to_char("You need level 2 protean first.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch) &&
ch->pcdata->stats[UNI_RAGE] > 0)
    {
	send_to_char("Your beast won't let you retract your claws.\n\r",ch);
	return;
    }


    if (IS_VAMPAFF(ch,VAM_CLAWS) )
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE)) 
	{
	    send_to_char("Your claws slide back under your nails.\n\r",ch);
	    act("$n's claws slide back under $s nails.", ch, NULL, NULL, TO_ROOM);
	}
    else if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_NINGENNO] > 0) 
    {
	send_to_char("You shove your iron claws up your sleeves.\n\r",ch);
	act("$n shoves $s iron claws up $e sleeves.", ch, NULL, NULL, TO_ROOM);
    }
	else
	{
	    send_to_char("Your talons slide back into your fingers.\n\r",ch);
	    act("$n's talons slide back into $s fingers.",ch, NULL, NULL, TO_ROOM);
	}
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE)) 
    {
	send_to_char("Sharp claws extend from under your finger nails.\n\r",ch);
	act("Sharp claws extend from under $n's finger nails.", ch, NULL, NULL, TO_ROOM);
    }
    else if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_NINGENNO] > 0) 
    {
	send_to_char("Iron claws come sliding from your sleeves.\n\r",ch);
	act("Iron claws come sliding from under $n's sleeves.", ch, NULL, NULL, TO_ROOM);
    ch->move -= 500;
    }
    else
    {
	send_to_char("Razor sharp talons extend from your fingers.\n\r",ch);
	act("Razor sharp talons extend from $n's fingers.",ch,NULL,NULL,TO_ROOM);
    }
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    return;
}

void do_nightsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_DEMON)
	&& !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_ABOMINATION(ch)
      && !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_SET(ch->special, SPC_CHAMPION)
      && !IS_CLASS(ch, CLASS_HIGHLANDER) )
    {
      stc("Huh?\n\r",ch);
      return;
    }



    if (IS_CLASS(ch, CLASS_WEREWOLF) || IS_ABOMINATION(ch))
    {

        if ( ch->gifts[METIS] < 3 )
        {
            send_to_char("Huh?\n\r",ch);
            return;
        }
    }

    else if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (!IS_DEMPOWER( ch, DEM_EYES))
	{
	    send_to_char("You haven't been granted the gift of nightsight.\n\r",ch);
	    return;
	}
    }

    else if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->powersde[VAM_PROT] < 1 && ch->pcdata->powersde[VAM_OBTE] < 3) 
    {
	send_to_char("You need level 1 protean first.\n\r",ch);
	return;
    }
    else if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 1 )
    {
      stc("You must obtain a level one knowledge of Awareness first.\n\r",ch);
      return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
    {
	send_to_char("Not while your beast is in control.\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
    {
if (IS_WEREWOLF(ch) || IS_ABOMINATION(ch))
    {
        send_to_char("The green glow in your eyes fades.\n\r",ch);
        act("The green glow in $n's eyes fades.", ch, NULL, NULL,TO_ROOM);
    }
else if (!IS_WEREWOLF(ch) && !IS_ABOMINATION(ch))
{
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM);
}
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
	return;
    }
  if (IS_WEREWOLF(ch) || IS_ABOMINATION(ch))
    {
    send_to_char("Your eyes start glowing a lambent green.\n\r",ch);
    act("$n's eyes start glowing a lambent green.", ch, NULL, NULL, TO_ROOM);
    }
else if (!IS_WEREWOLF(ch) && !IS_ABOMINATION(ch))
{
   send_to_char("Your eyes start glowing red.\n\r",ch);
    act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM);
}
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    return;
}

void do_anarch( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->clan) > 1)
    {
	send_to_char("But you are already in a clan!\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_INCONNU))
    {
	send_to_char("But you are already an Inconnu!\n\r",ch);
	return;
    }

    if ( IS_SET(ch->special, SPC_ANARCH) )
    {
	send_to_char("You are no longer an Anarch.\n\r",ch);
	sprintf(buf,"%s is no longer an Anarch!",ch->name);
	do_info(ch,buf);
	REMOVE_BIT(ch->special, SPC_ANARCH);
	return;
    }
    send_to_char("You are now an Anarch.\n\r",ch);
    sprintf(buf,"%s is now an Anarch!",ch->name);
    do_info(ch,buf);
    SET_BIT(ch->special, SPC_ANARCH);
    return;
}

void do_inconnu( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->clan) > 1 && ch->pcdata->stats[UNI_GEN] < 3)
    {
	send_to_char("You cannot abandon your clan!\n\r",ch);
	return;
    }

    if (ch->pcdata->rank < AGE_ELDER  && ch->pcdata->stats[UNI_GEN] > 2)
    {
	send_to_char("You must be at least an Elder to become an Inconnu.\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_INCONNU))
    {
	send_to_char("But you are already an Inconnu!\n\r",ch);
	return;
    }

    if (ch->exp < 10000000)
    {
	send_to_char("It costs 10000000 exp to become an Inconnu.\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_ANARCH)) REMOVE_BIT(ch->special, SPC_ANARCH);
    free_string(ch->clan);
    ch->clan = str_dup( "" );
    ch->exp -= 10000000;
    send_to_char("You are now an Inconnu.\n\r",ch);
    sprintf(buf,"%s is now an Inconnu!",ch->name);
    do_info(ch,buf);
    SET_BIT(ch->special, SPC_INCONNU);
    return;
}

void do_shadowsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_SHADOWSIGHT))
	{
	    send_to_char("You haven't been granted the gift of shadowsight.\n\r",ch);
	    return;
	}
    }
    else if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->pcdata->powers[WPOWER_HAWK] < 2)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_MONK) && !IS_ABOMINATION(ch)
          && !IS_CLASS(ch, CLASS_HIGHLANDER) )
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
	if ((IS_CLASS(ch, CLASS_VAMPIRE) || IS_ABOMINATION(ch)) && ch->pcdata->powersde[VAM_OBTE] < 2)
	{
	    send_to_char("You are not trained in the Obtenebration discipline.\n\r",ch);
	    return;
	}
	if (IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 2 )
	{
	  stc("You must obtain a level two knowledge of Awareness first.\n\r",ch);
	  return;
        }
    }

    if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
{
	send_to_char("You can no longer see between planes.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
	return;
}

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 10)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE))
        ch->pcdata->condition[COND_THIRST] -= number_range(5,10);
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}

void do_bite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    int       clancount;
    bool      can_sire = FALSE;
    bool      outcast = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) || (ch->pcdata->stats[UNI_GEN] < 1))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[UNI_CURRENT] == -1) 
	ch->pcdata->stats[UNI_CURRENT] = ch->pcdata->stats[UNI_AFF]; 

/* Changed by Toaster 10/02/00 cos it was commencted out*/
    if (IS_SET(ch->special,SPC_PRINCE)) can_sire = TRUE;
    if (IS_SET(ch->special,SPC_SIRE)) can_sire = TRUE;

    if (ch->pcdata->stats[UNI_GEN] < 3 || ch->pcdata->stats[UNI_GEN] > 6) can_sire = FALSE;
    if (ch->pcdata->stats[UNI_GEN] > 3) can_sire = FALSE;
    else can_sire = TRUE;

    if (!can_sire)
    {
	send_to_char("You are not able to create any childer.\n\r",ch);
	return;
    }

    if (!str_cmp(ch->clan,"") && ch->pcdata->stats[UNI_GEN] == 2)
    {
	send_to_char( "First you need to found a clan.\n\r", ch );
	return;
    }

    clancount = 0;
    if (IS_VAMPPASS(ch,VAM_VICISSITUDE))       clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_PROTEAN))       clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_CELERITY))      clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_FORTITUDE))     clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_POTENCE))       clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_OBFUSCATE))     clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_OBTENEBRATION)) clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_SERPENTIS))     clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_AUSPEX))        clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_DOMINATE))      clancount = clancount + 1;
    if (IS_VAMPPASS(ch,VAM_PRESENCE))      clancount = clancount + 1;
/*
    if ( clancount < 3 )
    {
	send_to_char( "First you need to master 3 disciplines.\n\r", ch );
	return;
    }
*/
    if ( arg[0] == '\0' )
    {
	send_to_char( "Bite whom?\n\r", ch );
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

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortal's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot bite yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_MAGE))
    {
	send_to_char( "You cannot bite mages.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only bite avatars.\n\r", ch );
	return;
    }
    
    if (IS_CLASS(victim, CLASS_DROW))
	{
	send_to_char(" Not on a drow!\n\r", ch);
	return;
	}

	if (IS_CLASS(victim, CLASS_MONK))
	{
	send_to_char("You cannot bite monks.\n\r", ch);
	return;
	}

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "You are unable to create werevamps!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) && ch->beast != 100)
    {
	send_to_char( "But they are already a vampire!\n\r", ch );
	return;
    }
/*Changed by Toaster so vampires can't just bite another vamp into clan*/
    if (IS_CLASS(victim, CLASS_DROW))
    {
    send_to_char( "Not on drows!\n\r",ch);
    return;
    }
    
    if (IS_CLASS(victim, CLASS_MOOGLE))
    {
    send_to_char( "Not on moogles!\n\r", ch);
    return;
    }
    
    if (IS_CLASS(victim, CLASS_NINJA))
    {
    send_to_char( "You cannot bite ninjas.\n\r", ch);
    return;
    }
    
    if (IS_CLASS(victim, CLASS_PALADIN))
    {
    send_to_char( "Paladins are to goody goody.\n\r", ch);
    return;
    }
/*
	if (IS_CLASS(victim, CLASS_VAMPIRE))
{
	victim->clan=str_dup(ch->clan);
	return;
}
*/
    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "But they have no soul!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You cannot turn highlanders into vampires.\n\r", ch );
	return;
    }

    if (!IS_IMMUNE(victim,IMM_VAMPIRE) && ch->beast != 100)
    {
	send_to_char( "You cannot bite an unwilling person.\n\r", ch );
	return;
    }

    if (!IS_VAMPAFF(ch,VAM_FANGS) && ch->beast != 100)
    {
	send_to_char("First you better get your fangs out!\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_DISGUISED) && ch->beast != 100)
    {
	send_to_char("You must reveal your true nature to bite someone.\n\r",ch);
	return;
    }

    if (ch->exp < 1000 && ch->beast != 100)
    {
	send_to_char("You cannot afford the 1000 exp to create a childe.\n\r",ch);
	return;
    }

    if (ch->beast == 100 || ch->pcdata->stats[UNI_RAGE] > 0)
    {
    	if (!IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	act("Your jaw opens wide and you leap hungrily at $N.", ch, NULL, victim, TO_CHAR);
    	act("$n's jaw opens wide and $e leaps hungrily at $N.", ch, NULL, victim, TO_NOTVICT);
    	act("$n's jaw opens wide and $e leaps hungrily at you.", ch, NULL, victim, TO_VICT);
	one_hit( ch, victim, -1, 0 );
	return;
    }
    if(( victim->max_hit < 1500))
        {
        send_to_char("They do not have 1500 hitpoints.\n\r", ch);
        return;
        }



    if (ch->beast > 0)
	ch->beast += 1;
    ch->exp = ch->exp - 1000;
    act("You sink your teeth into $N.", ch, NULL, victim, TO_CHAR);
    act("$n sinks $s teeth into $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n sinks $s teeth into your neck.", ch, NULL, victim, TO_VICT);
    victim->class=CLASS_VAMPIRE;
/*
    if ( victim->pcdata->stats[UNI_GEN] != 0 && (victim->pcdata->stats[UNI_GEN] <= ( ch->pcdata->stats[UNI_GEN] + 1 ) ) ) */
    if ( victim->pcdata->stats[UNI_GEN] != 0 )
    {
	save_char_obj(ch);
	save_char_obj(victim);
	send_to_char( "Your vampiric status has been restored.\n\r", victim );
	return;
    }
    if (IS_SET(ch->special,SPC_SIRE) || ch->pcdata->stats[UNI_GEN] < 3 || IS_SET(ch->special,SPC_PRINCE))
    {
	if (IS_SET(ch->special,SPC_SIRE))
	    REMOVE_BIT(ch->special,SPC_SIRE);
	if (IS_SET(ch->special, SPC_ANARCH) && strlen(ch->clan) < 2)
	    outcast = TRUE;
    }
    else outcast = TRUE;
    send_to_char( "You are now a vampire.\n\r", victim );    
    victim->pcdata->stats[UNI_GEN] = ch->pcdata->stats[UNI_GEN] + 1;
/*
    clan_table_bite(victim);
*/
    free_string(victim->lord);
    if (ch->pcdata->stats[UNI_GEN] == 1)
	victim->lord=str_dup(ch->name);
    else
    {
	sprintf(buf,"%s %s",ch->lord,ch->name);
	victim->lord=str_dup(buf);
    }
    if (ch->pcdata->stats[UNI_GEN] != 1)
    {
    	if (victim->pcdata->stats[UNI_CURRENT] == -1) 
		victim->pcdata->stats[UNI_CURRENT] = victim->pcdata->stats[UNI_AFF];

	/* Remove hp bonus from fortitude */
	if (IS_VAMPPASS(victim,VAM_FORTITUDE) && !IS_VAMPAFF(victim,VAM_FORTITUDE))
	{
	    victim->max_hit = victim->max_hit - 50;
	    victim->hit = victim->hit - 50;
	    if (victim->hit < 1) victim->hit = 1;
	}

	/* Remove any old powers they might have */
	if (IS_VAMPPASS(victim,VAM_PROTEAN))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PROTEAN);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_PROTEAN);}
	if (IS_VAMPPASS(victim,VAM_VICISSITUDE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_VICISSITUDE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_VICISSITUDE);}
	if (IS_VAMPPASS(victim,VAM_CELERITY))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_CELERITY);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CELERITY);}
	if (IS_VAMPPASS(victim,VAM_FORTITUDE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_FORTITUDE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FORTITUDE);}
	if (IS_VAMPPASS(victim,VAM_POTENCE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_POTENCE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_POTENCE);}
	if (IS_VAMPPASS(victim,VAM_OBFUSCATE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBFUSCATE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBFUSCATE);}
	if (IS_VAMPPASS(victim,VAM_OBTENEBRATION))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBTENEBRATION);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBTENEBRATION);}
	if (IS_VAMPPASS(victim,VAM_SERPENTIS))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_SERPENTIS);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SERPENTIS);}
	if (IS_VAMPPASS(victim,VAM_AUSPEX))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_AUSPEX);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_AUSPEX);}
	if (IS_VAMPPASS(victim,VAM_DOMINATE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_DOMINATE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DOMINATE);}
	if (IS_VAMPPASS(victim,VAM_PRESENCE))
	{REMOVE_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PRESENCE);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_PRESENCE);}
	if (!outcast)
	{
	    free_string(victim->clan);
	    victim->clan=str_dup(ch->clan);
	}
	/* Give the vampire the base powers of their sire */
	if (IS_VAMPPASS(ch,VAM_FORTITUDE) && !IS_VAMPAFF(victim,VAM_FORTITUDE))
	{
	    victim->max_hit = victim->max_hit + 50;
	    victim->hit = victim->hit + 50;
	}
	if (IS_VAMPPASS(ch,VAM_PROTEAN))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PROTEAN);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_PROTEAN);}
	if (IS_VAMPPASS(ch,VAM_VICISSITUDE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_VICISSITUDE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_VICISSITUDE);}
	if (IS_VAMPPASS(ch,VAM_CELERITY))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_CELERITY);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_CELERITY);}
	if (IS_VAMPPASS(ch,VAM_FORTITUDE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_FORTITUDE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_FORTITUDE);}
	if (IS_VAMPPASS(ch,VAM_POTENCE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_POTENCE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_POTENCE);}
	if (IS_VAMPPASS(ch,VAM_OBFUSCATE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBFUSCATE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBFUSCATE);}
	if (IS_VAMPPASS(ch,VAM_OBTENEBRATION))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_OBTENEBRATION);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBTENEBRATION);}
	if (IS_VAMPPASS(ch,VAM_SERPENTIS))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_SERPENTIS);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_SERPENTIS);}
	if (IS_VAMPPASS(ch,VAM_AUSPEX))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_AUSPEX);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_AUSPEX);}
	if (IS_VAMPPASS(ch,VAM_DOMINATE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_DOMINATE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_DOMINATE);}
	if (IS_VAMPPASS(ch,VAM_PRESENCE))
	{SET_BIT(victim->pcdata->stats[UNI_CURRENT], VAM_PRESENCE);
	 SET_BIT(victim->pcdata->stats[UNI_AFF], VAM_PRESENCE);}
    }
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_claw( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    bool      can_sire = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || (ch->pcdata->stats[UNI_GEN] < 1))
    {
	do_claws(ch,"");
	return;
    }
/* Changed by Toaster 10/02/00 since it was commented out*/
    if (IS_SET(ch->special,SPC_PRINCE)) can_sire = TRUE;
    if (IS_SET(ch->special,SPC_SIRE)) can_sire = TRUE;
    if (ch->pcdata->stats[UNI_GEN] == 1 || ch->pcdata->stats[UNI_GEN] == 2) can_sire = TRUE;

    if (ch->pcdata->stats[UNI_GEN] > 3) can_sire = FALSE;
    else can_sire = TRUE;

    if (!can_sire)
    {
	send_to_char("You are unable to spread your gift.\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[UNI_GEN] == 1 && ch->pcdata->stats[UNI_AFF] >= 4)
    {
	send_to_char("You have already created 4 tribal leaders.\n\r",ch);
	return;
    }
/*
    else if (ch->pcdata->stats[UNI_GEN] != 1 && ch->pcdata->stats[UNI_AFF] >= 2)
    {
	send_to_char("You have already created 2 pups.\n\r",ch);
	return;
    }
*/
    if (strlen(ch->clan) < 2 && ch->pcdata->stats[UNI_GEN] != 1 )
    {
	send_to_char( "First you need to create a tribe.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Claw whom?\n\r", ch );
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

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortal's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot claw yourself.\n\r", ch );
	return;
    }

	if (IS_CLASS(victim, CLASS_MONK))
	{
	send_to_char("You cannot claw monks.\n\r", ch);
	return;
	}

    if (IS_CLASS(victim, CLASS_MAGE))
    {
	send_to_char( "You cannot bite mages.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only claw avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DROW)) {
	send_to_char( "Not on a drow!\n\r", ch );
		return;}

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to create werevamps!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "But they have no soul!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You cannot turn highlanders into werewolves.\n\r", ch );
	return;
    }
    
    if (IS_CLASS(victim, CLASS_NINJA) )
    {send_to_char("You cannot turn ninjas into werewolves.\n\r", ch);
    return;
    }

    if (IS_CLASS(victim, CLASS_PALADIN))
    {send_to_char( "You cannot turn paladins into werewolves.\n\r", ch);
    return;
    }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "But they are already a werewolf!\n\r", ch );
	return;
    }

/*
	 if (IS_CLASS(victim, CLASS_WEREWOLF))
{
        victim->clan=str_dup(ch->clan);
        return;
}
*/
    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot claw an unwilling person.\n\r", ch );
	return;
    }

    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("First you better get your claws out!\n\r",ch);
	return;
    }

    if (ch->exp < 10000)
    {
	send_to_char("You cannot afford the 10000 exp to pass on the gift.\n\r",ch);
	return;
    }

    if(( victim->max_hit < 1500))
        {
        send_to_char("They do not have 1500 hitpoints.\n\r", ch);
        return;
        }

    ch->exp = ch->exp - 10000;
    if (ch->pcdata->stats[UNI_CURRENT] < 1) ch->pcdata->stats[UNI_CURRENT] = 1;
    else ch->pcdata->stats[UNI_CURRENT] += 1;
    if (IS_SET(ch->special,SPC_SIRE)) REMOVE_BIT(ch->special,SPC_SIRE);
    act("You plunge your claws into $N.", ch, NULL, victim, TO_CHAR);
    act("$n plunges $s claws into $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n plunges $s claws into your chest.", ch, NULL, victim, TO_VICT);
    victim->class=CLASS_WEREWOLF;
    send_to_char( "You are now a werewolf.\n\r", victim );
    victim->pcdata->stats[UNI_GEN] = ch->pcdata->stats[UNI_GEN] + 1;
    free_string(victim->lord);
    if (ch->pcdata->stats[UNI_GEN] == 1)
	victim->lord=str_dup(ch->name);
    else
    {
	sprintf(buf,"%s %s",ch->lord,ch->name);
	victim->lord=str_dup(buf);
    }
    free_string(victim->clan);
    victim->clan=str_dup(ch->clan);
    victim->pcdata->stats[UNI_AFF] = 0;
    victim->pcdata->stats[UNI_CURRENT] = 0;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}


void do_demonform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    /*if (ch->power[DISC_VAMP_VICI] < 4)
    {
        send_to_char("You must obtain at least level 4 in Vicissitude to use Zuloform.\n\r",ch);
	return;
    }*/
    
    
    if (!IS_DEMPOWER( ch, DEM_FORM) && IS_CLASS(ch,CLASS_DEMON))
    {
	send_to_char("You haven't been granted the gift of
demonform.\n\r",ch);
	return;
    }
    

    if (IS_POLYAFF(ch, POLY_ZULO))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULO);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM);
	free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
	send_to_char("You cant demon form when changed.\n\r",ch);
	return;
    }
  
  
        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        act( "You transform into a huge demon.", ch, NULL, NULL, TO_CHAR);
        act( "$n's body grows and distorts into a huge demon.", ch, NULL, NULL, TO_ROOM );
        /*
        clear_stats(ch);
    	if (ch->wpn[0] > 0)
    	{
	    ch->hitroll += (ch->wpn[0]);
	    ch->damroll += (ch->wpn[0]);
	    ch->armor   -= (ch->wpn[0] * 3);
    	}
*/
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;

        SET_BIT(ch->polyaff, POLY_ZULO);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        if (!str_cmp(ch->name,"Toast")) sprintf(buf,"Toast");    
	else sprintf(buf, "%s a huge red demon", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        return;
    }
  




void do_zuloform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powersde[VAM_VICI] < 4)
    {
        send_to_char("You must obtain at least level 4 in Vicissitude to use Zuloform.\n\r",ch);
	return;
    }
/* Changed Toast cos no dragonform 13/10/00
    if (IS_EXTRA(ch,EXTRA_DRAGON))
    {
    send_to_char("You cannot unzulo while a dragon.\n\r",ch);
    return;
    }
*/
    if (IS_POLYAFF(ch, POLY_NEWZULO))
    {   
        REMOVE_BIT(ch->polyaff, POLY_NEWZULO);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
/*
	hpmod =	(ch->max_hit/11)*10;
	ch->max_hit -= hpmod;
	ch->hit -= hpmod;
*/
	ch->max_hit -= 1000;
	ch->hit -= 1000;
	if(ch->hit < 0) ch->hit = 0;
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM);
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
	send_to_char("You cant zulo when changed.\n\r",ch);
	return;
    }
  
  
    if ( ch->pcdata->condition[COND_THIRST] < 200 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}

        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(100,200);
        act( "You transform into large beast.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body grows and distorts into a large beast.", ch, NULL, NULL, TO_ROOM );        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
/*
	ch->max_hit += (ch->max_hit/100)*10;
	ch->hit += (ch->hit/100)*10;
*/	
	ch->max_hit += 1000;
	ch->hit += 1000;
        SET_BIT(ch->polyaff, POLY_NEWZULO);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        if (!str_cmp(ch->name,"Toast")) sprintf(buf,"Toast");    
	sprintf(buf, "%s a big black monster", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        return;
    }

 void do_kingname( CHAR_DATA *ch, char *argument ) 
       {
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' )
    {
    send_to_char( "Who's kingdom do you wish to change or name?\n\r", ch
);
        return;
       }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) ) return;

    smash_tilde( argument );
    if ( strlen(argument) < 0 || strlen(argument) > 26 )
    {
        send_to_char( "kingdom name should be between 0 and 26 letters long.\n\r", ch);
        send_to_char( "Leave a blank to remove kingdom.\n\r", ch);
        return;
    }
    free_string( victim->kingdom );
    victim->kingdom = str_dup( argument );
        send_to_char("Kingdom set.\n\r", ch);
        send_to_char("Kingdom set.\n\r", victim);
    return;
}

void do_clanname( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' )
    {



    send_to_char( "Who's clan do you wish to change or name?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if ( IS_NPC(victim) ) return;
/*
    if (victim->pcdata->powersm[MPOWER_RUNE0] == 0);
        {
                victim->clan = "Purple Archmage";
        return;
}

    if (victim->pcdata->powersm[MPOWER_RUNE0] == 1);
        {
                victim->clan = "Red Archmage";
        return;
}

    if (victim->pcdata->powers[MPOWER_RUNE0] == 4);
        {
                victim->clan = "Yellow Archmage";
        return;
}

    if (victim->pcdata->powers[MPOWER_RUNE0] == 2);
        {
                victim->clan = "Blue Archmage";
        return;
}

    if (victim->pcdata->powers[MPOWER_RUNE0] == 3);
        {
                victim->clan = "Green Archmage";
        return;
}
*/
    smash_tilde( argument );
    if ( strlen(argument) < 0 || strlen(argument) > 26 )
    {
	send_to_char( "Clan name should be between 0 and 26 letters long.\n\r", ch );
	send_to_char( "Leave a blank to remove clan.\n\r", ch);
	return;
    }


    free_string( victim->clan );
    victim->clan = str_dup( argument );
	send_to_char("Clan set.\n\r", ch);
	send_to_char("Clan set.\n\r", victim);
    return;
}

/*
void do_clanname( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if ( IS_NPC(ch) ) return;

    if ( (!IS_CLASS(ch,CLASS_VAMPIRE) || ch->pcdata->stats[UNI_GEN] != 1) && 
	(!IS_CLASS(ch,CLASS_DROW) || ch->pcdata->stats[UNI_GEN] != 1 ) &&
	(!IS_CLASS(ch,CLASS_WEREWOLF) || ch->pcdata->stats[UNI_GEN] != 1) &&
	(!IS_CLASS(ch,CLASS_NINJA) || ch->pcdata->stats[UNI_GEN] != 1) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE))
	    send_to_char( "Who's clan do you wish to name?\n\r", ch );
	else if (IS_CLASS(ch, CLASS_DROW))
	    send_to_char("Who's clan do you want to name?\n\r", ch );
	else if (IS_CLASS(ch, CLASS_NINJA))
	    send_to_char("Who's clan do you want to name?\n\r", ch );
	else
	    send_to_char( "Who do you wish to give a tribe to?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if ( IS_NPC(victim) ) return;
    if ( IS_CLASS(ch, CLASS_WEREWOLF))
    {
	if ( victim->pcdata->stats[UNI_GEN] != 2)
	{
	    send_to_char( "Only greater werewolves may own a tribe.\n\r", ch );
	    return;
	}
    }

    else if ( victim->pcdata->stats[UNI_GEN] != 2 )
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE))
	    send_to_char( "Only the Antediluvians may have clans.\n\r", ch );
	else if (IS_CLASS(ch, CLASS_DROW))
	send_to_char("Only matrons can have clans.\n\r", ch );
	else if (IS_CLASS(ch, CLASS_NINJA))
	send_to_char("Only leaders may have a clan.\n\r", ch);
	return;
    }
    if ( str_cmp(victim->clan,"") )
    {
	send_to_char( "But they already have a clan!\n\r", ch );
	return;
    }
    smash_tilde( argument );
    if ( strlen(argument) < 3 || strlen(argument) > 20 )
    {
	send_to_char( "Clan name should be between 3 and 20 letters long.\n\r", ch );
	return;
    }
    free_string( victim->clan );
    victim->clan = str_dup( argument );
    if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DROW) 
        || IS_CLASS(ch, CLASS_NINJA) )
    {
    	send_to_char( "Clan name set.\n\r", ch );
    }
    else
    	send_to_char( "Tribe name set.\n\r", ch );
    return;
}*/

void do_stake( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *stake;
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );
    if (IS_NPC( ch )) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stake whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    stake = get_eq_char( ch, WEAR_HOLD );
    if ( stake == NULL || stake->item_type != ITEM_STAKE )
    {
	stake = get_eq_char( ch, WEAR_WIELD );
	if ( stake == NULL || stake->item_type != ITEM_STAKE )
	{
	    send_to_char( "How can you stake someone down without holding a stake?\n\r", ch );
	    return;
	}
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot stake yourself.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char( "You can only stake vampires.\n\r", ch );
	return;
    }

    if (victim->position > POS_MORTAL)
    {
	send_to_char( "You can only stake down a vampire who is mortally wounded.\n\r", ch );
	return;
    }

    act("You plunge $p into $N's heart.", ch, stake, victim, TO_CHAR);
    act("$n plunges $p into $N's heart.", ch, stake, victim, TO_NOTVICT);
    send_to_char( "You feel a stake plunged through your heart.\n\r", victim );
    if (IS_IMMUNE(victim,IMM_STAKE)) return;

    /* Have to make sure they have enough blood to change back */
    blood = victim->pcdata->condition[COND_THIRST];
    victim->pcdata->condition[COND_THIRST] = 666;

    /* To take care of vampires who have powers in affect. */
    if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
    if (IS_IMMUNE(victim,IMM_SHIELDED) ) do_shield(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) ) do_shadowplane(victim,"");
    if (IS_VAMPAFF(victim,VAM_FANGS) ) do_fangs(victim,"");
    if (IS_VAMPAFF(victim,VAM_CLAWS) ) do_claws(victim,"");
    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) ) do_nightsight(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) ) do_shadowsight(victim,"");
    if (IS_SET(victim->act,PLR_HOLYLIGHT) ) do_truesight(victim,"");
    if (IS_VAMPAFF(victim,VAM_CHANGED) ) do_change(victim,"human");
    if (IS_POLYAFF(victim,POLY_SERPENT) ) do_serpent(victim,"");
    victim->pcdata->stats[UNI_RAGE] = 0;
    victim->pcdata->condition[COND_THIRST] = blood;

    REMOVE_BIT(victim->class, CLASS_VAMPIRE);
    obj_from_char(stake);
    obj_to_char(stake,victim);
    ch->exp = ch->exp + 1000;
    victim->home = 3001;
    return;
}

void do_mask( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DRAGON))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE)) {
	if (ch->pcdata->powersde[VAM_OBFU] < 3  )
    {
	send_to_char("You need level 4 obfuscate first.\n\r",ch);
	return;
    } }

    if (IS_CLASS(ch, CLASS_DRAGON))
    {    if (!IS_DRAPOWER(ch, DRA_MASK)) {stc("You haven't learned that skill yet.\n\r",ch); return; }}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Change to look like whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
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

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
	send_to_char( "You can only mask avatars or lower.\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE)) {
    if ( ch->pcdata->condition[COND_THIRST] < 40 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(30,40); }

    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
	sprintf(buf,"Your form shimmers and transforms into %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"%s's form shimmers and transforms into %s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
    	sprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    	act(buf,ch,NULL,victim,TO_CHAR);
    	sprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->morph,victim->name);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
    	sprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
    	ch->morph = str_dup( victim->name );
	return;
    }
    sprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->name,victim->name);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->name );
    return;
}

void do_change( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	
    if (ch->pcdata->powersde[VAM_PROT] < 3  )
    {
	send_to_char("You are not trained in the Protean discipline.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
	return;
    }

    if ( !str_cmp(arg,"bat") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	clear_stats(ch);
	act( "You transform into bat form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a bat.", ch, NULL, NULL, TO_ROOM );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	SET_BIT(ch->polyaff, POLY_BAT);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf, "%s the vampire bat", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"wolf") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a dire wolf.", ch, NULL, NULL, TO_ROOM );
	clear_stats(ch);
    	if (ch->wpn[0] > 0)
    	{
	    ch->hitroll += (ch->wpn[0]);
	    ch->damroll += (ch->wpn[0]);
	    ch->armor   -= (ch->wpn[0] * 3);
    	}
    	ch->pcdata->mod_str = 10;
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	SET_BIT(ch->polyaff, POLY_WOLF);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	ch->max_hit = ch->max_hit + 500;
	ch->hit = ch->hit + 500;
	sprintf(buf, "%s the dire wolf", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"mist") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into mist form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into a white mist.", ch, NULL, NULL, TO_ROOM );
	if (IS_EXTRA(ch, TIED_UP))
	{
	    act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR);
	    act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	    REMOVE_BIT(ch->extra, TIED_UP);
	    REMOVE_BIT(ch->extra, GAGGED);
	    REMOVE_BIT(ch->extra, BLINDFOLDED);
	}
	if (is_affected(ch, gsn_web))
	{
	    act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
	    send_to_char("The webbing entrapping you falls through your ethereal form.\n\r",ch);
	    affect_strip(ch, gsn_web);
	}
	clear_stats(ch);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	SET_BIT(ch->polyaff, POLY_MIST);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->affected_by, AFF_ETHEREAL);
	sprintf(buf, "%s the white mist", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"human") )
    {
	if (!IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You are already in human form.\n\r", ch );
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
	{
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
	    REMOVE_BIT(ch->polyaff, POLY_BAT);
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	    REMOVE_BIT(ch->polyaff, POLY_WOLF);
	    ch->max_hit = ch->max_hit - 500;
	    ch->hit = ch->hit - 500;
	    if (ch->hit < 1) ch->hit = 1;
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	    REMOVE_BIT(ch->polyaff, POLY_MIST);
	    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
	}
	else
	{
	    /* In case they try to change to human from a non-vamp form */
	    send_to_char( "You seem to be stuck in this form.\n\r", ch );
	    return;
	}
	act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	clear_stats(ch);
  	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    else
	send_to_char( "You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
    return;
}


void do_stat( CHAR_DATA *ch, char *argument )
{
    char buf   [MAX_STRING_LENGTH];
    char age   [MAX_STRING_LENGTH];
    int blood;
    int bloodpool;
    int chargen;

    if (ch->pcdata->stats[UNI_GEN] <= 0)
	ch->pcdata->stats[UNI_GEN] = 4;

//    {
    chargen = ch->pcdata->stats[UNI_GEN];
    if  (chargen > 12)
        chargen = 12;
    bloodpool = (13 - chargen) * 100;

    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->pcdata->stats[UNI_GEN]);
    else bloodpool = (2000 / ch->pcdata->stats[UNI_GEN]);

//    }

    blood = ch->pcdata->condition[COND_THIRST];
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch)) return;
    if (ch->pcdata->rank == AGE_ANCILLA)         sprintf( age, "Ancilla");
    else if (ch->pcdata->rank == AGE_CHILDE)     sprintf( age, "Childe");
    else if (ch->pcdata->rank == AGE_NEONATE)    sprintf( age, "Neonate");
    else if (ch->pcdata->rank == AGE_ELDER)      sprintf( age, "Elder");
    else if (ch->pcdata->rank == AGE_METHUSELAH) sprintf( age, "Methuselah");
    divide_to_char(ch);
    send_to_char(
"                              Vampire Status\n\r",ch);
    divide_to_char(ch);
sprintf(buf,
"Generation: %d  Bloodpool:%d  Age:%s  Blood:%d\n\r",
ch->pcdata->stats[UNI_GEN],bloodpool,age,blood);
send_to_char(buf,ch);
    divide_to_char(ch);
send_to_char(
"                              Disciplines\n\r",ch);
    divide_to_char(ch);
sprintf(buf,
"Animalism:    [%d]             Celerity:   [%d]        Fortitude:   [%d]\n\r",
ch->pcdata->powersde[VAM_ANIM],ch->pcdata->powersde[VAM_CELE],
ch->pcdata->powersde[VAM_FORT]);
send_to_char(buf,ch);
sprintf(buf,
"Obtenebration:[%d]             Presence:   [%d]        Quietus:     [%d]\n\r",
ch->pcdata->powersde[VAM_OBTE],ch->pcdata->powersde[VAM_PRES],
ch->pcdata->powersde[VAM_QUIE]);
send_to_char(buf,ch);
sprintf(buf,
"Thaumaturgy:  [%d]             Auspex:     [%d]        Dominate:    [%d]\n\r",
ch->pcdata->powersde[VAM_THAU],ch->pcdata->powersde[VAM_AUSP],
ch->pcdata->powersde[VAM_DOMI]);
send_to_char(buf,ch);
sprintf(buf,
"Obfuscate:    [%d]             Potence:    [%d]        Protean:     [%d]\n\r",
ch->pcdata->powersde[VAM_OBFU],ch->pcdata->powersde[VAM_POTE],
ch->pcdata->powersde[VAM_PROT]);
send_to_char(buf,ch);
sprintf(buf,
"Serpentis:    [%d]             Vicissitude:[%d]        Daimoinon:   [%d]\n\r",
ch->pcdata->powersde[VAM_SERP],ch->pcdata->powersde[VAM_VICI],
ch->pcdata->powersde[VAM_DAIM]);
send_to_char(buf,ch);
sprintf(buf,			 
"Chimerstry:   [%d]             Obeah:      [%d]        Melpominee:  [%d]\n\r",
ch->pcdata->powersde[VAM_CHIM], ch->pcdata->powersde[VAM_OBEA],
ch->pcdata->powersde[VAM_MELP]);
send_to_char(buf,ch);
sprintf(buf,
"Necromancy:   [%d]                                    Thanatosis:  [%d]\n\r",
ch->pcdata->powersde[VAM_NECR], ch->pcdata->powersde[VAM_THAN] );
stc(buf,ch);
    divide_to_char(ch);
 
return;
}


void do_clandisc( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        do_stat(ch,"");
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"animalism"))
	{
            send_to_char("Animalism: The discipline of nature control.\n\r",ch);
            if (ch->pcdata->powersde[VAM_ANIM] < 1) 
                send_to_char("You have none of the Animalism powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_ANIM] > 0) 
                send_to_char("Serenity: You can sooth the most aggressive soul (Golconda Required).\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"auspex"))
	{
            send_to_char("Auspex: The discipline of extrasensory awareness.\n\r",ch);
            if (ch->pcdata->powersde[VAM_AUSP] < 1) 
                send_to_char("You have none of the Auspex discipline powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_AUSP] > 0) 
                send_to_char("Truesight: The user can see everything, including invis.\n\r",ch);
            if (ch->pcdata->powersde[VAM_AUSP] > 1) 
                send_to_char("Scry: Allows you to view the room the specified target is in.\n\r",ch);
            if (ch->pcdata->powersde[VAM_AUSP] > 2) 
                send_to_char("Readaura: Gives you detained information about a creature or object.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"celerity"))
	{
            send_to_char("Celerity: The discipline of superior speed.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"chimerstry"))
        {
            send_to_char("chimerstry: The discipline of illusion.\n\r",ch);
            return;
        }
	else if (!str_cmp(arg1,"daimoinon"))
	{
            send_to_char("Daimoinon: The discipline of dark corruption.\n\r",ch);
            if (ch->pcdata->powersde[VAM_DAIM] < 1) 
                send_to_char("You have none of the Daimoinon discipline powers.\n\r",ch);
	    return;
        }

        else if (!str_cmp(arg1,"dominate"))
	{
            send_to_char("Dominate: The discipline of control.\n\r",ch);
            if (ch->pcdata->powersde[VAM_DOMI] < 1) 
                send_to_char("You have none of the Dominate discipline powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_DOMI] > 0) 
                send_to_char("Command: You can plant suggestions in the mind of others.\n\r",ch);
            if (ch->pcdata->powersde[VAM_DOMI] > 1) 
                send_to_char("Shield: You are able to shield your mind from scrying and aura-reading.\n\r",ch);
            if (ch->pcdata->powersde[VAM_DOMI] > 3) 
                send_to_char("Possession: You can take control of mobs by feeding them blood.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"fortitude"))
	{
            send_to_char("Fortitude: The discipline of supernatural toughness and vigor.\n\r",ch);
            if (ch->pcdata->powersde[VAM_FORT] < 1) 
		send_to_char("You have no fortitude.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obfuscate"))
	{
            send_to_char("Obfuscate: The discipline of stealth.\n\r",ch);
            if (ch->pcdata->powersde[VAM_OBFU] < 1) 
                send_to_char("You have none of the Obfuscate discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obtenebration"))
	{
            send_to_char("Obtenebration: The discipline of darkness manipulation.\n\r",ch);
            if (ch->pcdata->powersde[VAM_OBTE] < 1) 
                send_to_char("You have none of the Obtenebration discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"potence"))
	{
            send_to_char("Potence: The discipline of superior strength.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"presence"))
	{
            send_to_char("Presence: The discipline of appeal and attractiveness.\n\r",ch);
            if (ch->pcdata->powersde[VAM_PRES] < 1) 
                send_to_char("You have none of the Presence discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"protean"))
	{
            send_to_char("Protean: The discipline of neutral transformation.\n\r",ch);
            if (ch->pcdata->powersde[VAM_PROT] < 1) 
                send_to_char("You have none of the Protean discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"temporis"))
        {
            send_to_char("Temporis: The discipline of Time.\n\r",ch);
            if (ch->pcdata->powersde[VAM_QUIE] < 1)
                send_to_char("You have none of the Temporis powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_QUIE] > 3)
                send_to_char("Woo.\n\r",ch);
           return;
        }

        else if (!str_cmp(arg1,"quietus"))
	{
            send_to_char("Quietus: The discipline of death.\n\r",ch);
            if (ch->pcdata->powersde[VAM_QUIE] < 1) 
                send_to_char("You have none of the Quietus discipline powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_QUIE] > 3)
                send_to_char("Spit: You can project your deadly spit at a foe\n\r",ch); 
           return;
        }
        else if (!str_cmp(arg1,"serpentis"))
	{
            send_to_char("Serpentis: The discipline of temptation and corruption.\n\r",ch);
            if (ch->pcdata->powersde[VAM_SERP] < 1) 
                send_to_char("You have none of the Serpentis discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"thaumaturgy"))
	{
            send_to_char("Thaumaturgy: The discipline of blood magic.\n\r",ch);
            if (ch->pcdata->powersde[VAM_THAU] < 1) 
                send_to_char("You have none of the Thaumaturgy discipline powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_THAU] > 0) 
                send_to_char("Theft: You can take from the blood pool of other players.\n\r",ch);

            return;
        }
        else if (!str_cmp(arg1,"vicissitude"))
	{
            send_to_char("Vicissitude: The discipline of evil body control.\n\r",ch);
            if (ch->pcdata->powersde[VAM_VICI] < 1) 
                send_to_char("You have none of the Vicissitude discipline powers.\n\r",ch);
            if (ch->pcdata->powersde[VAM_VICI] > 3) 
                send_to_char("Zuloform: Transformation into a powerful being.\n\r",ch);

            return;
        }
        do_stat(ch,"");
        return;

    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;
        int maxlevel = 5;

if (IS_ABOMINATION(ch))
{
maxlevel = 3;
}

/*
	if (ch->[UNI_GEN] == 13)
          maxlevel=5;
         else if (ch->[UNI_GEN] == 12)
          maxlevel=5;
         else if (ch->[UNI_GEN] == 11)
          maxlevel=5;
          else if (ch->[UNI_GEN] == 10)
          maxlevel=5;
          else if (ch->[UNI_GEN] == 9)
          maxlevel=5;
          else if (ch->[UNI_GEN] == 8)
          maxlevel=5;
          else if (ch->[UNI_GEN] == 7)
          maxlevel=5;
        else if (ch->[UNI_GEN] == 6)
          maxlevel=5;
	else if (ch->[UNI_GEN] == 5)
	  maxlevel=5;        
	else if (ch->[UNI_GEN]  == 4)
          maxlevel=5;
        else
         maxlevel=5;
*/
             if (!str_cmp(arg1,"animalism"       )) {improve = VAM_ANIM;
max=maxlevel;}
        else if (!str_cmp(arg1,"auspex"          )) {improve = VAM_AUSP;
max=maxlevel;}
        else if (!str_cmp(arg1,"celerity"        )) {improve = VAM_CELE;
max=maxlevel;}
        else if (!str_cmp(arg1,"chimerstry"      )) {improve = VAM_CHIM;
max=maxlevel;}
	else if (!str_cmp(arg1,"dominate"        )) {improve = VAM_DOMI;
max=maxlevel;}
        else if (!str_cmp(arg1,"daimoinon"       )) {improve = VAM_DAIM;
max=maxlevel;}
        else if (!str_cmp(arg1,"fortitude"       )) {improve = VAM_FORT;
max=maxlevel;}
        else if (!str_cmp(arg1,"obfuscate"       )) {improve = VAM_OBFU;
max=maxlevel;}
        else if (!str_cmp(arg1,"obtenebration"   )) {improve = VAM_OBTE;
max=maxlevel;}
        else if (!str_cmp(arg1,"potence"         )) {improve = VAM_POTE;
max=maxlevel;}
        else if (!str_cmp(arg1,"presence"        )) {improve = VAM_PRES;
max=maxlevel;}
        else if (!str_cmp(arg1,"protean"         )) {improve = VAM_PROT;
max=maxlevel;}
        else if (!str_cmp(arg1,"quietus"         )) {improve = VAM_QUIE;
max=maxlevel;}
        else if (!str_cmp(arg1,"serpentis"       )) {improve = VAM_SERP;
max=maxlevel;}
        else if (!str_cmp(arg1,"thaumaturgy"     )) {improve = VAM_THAU;
max=maxlevel;}
        else if (!str_cmp(arg1,"vicissitude"     )) {improve = VAM_VICI;
max=maxlevel;}
	else if (!str_cmp(arg1,"obeah"		 )) {improve = VAM_OBEA;
max = 5; }
	else if (!str_cmp(arg1,"melpominee"	 )) {improve = VAM_MELP;
max = 5; }
	else if (!str_cmp(arg1,"necromancy"	 )) {improve = VAM_NECR;
max = 5; }
	else if (!str_cmp(arg1,"thanatosis"	 )) {improve = VAM_THAN;
max = 5; }
        else
	{										
            send_to_char("You can improve: Animalism, Auspex, Celerity,
Dominate, Fortitude, Obfuscate,\n\r Obtenebration, Potence, Presence,
Protean, Quietus, Serpentis,\n\r Thaumaturgy or Vicissitude.\n\r",ch);
	    return;
	}
        cost = (ch->pcdata->powersde[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powersde[improve] >= max && ch->pcdata->rank
==AGE_METHUSELAH)
	{
            sprintf(buf,"You have already gained all the powers of the %s
discipline.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
        if (ch->pcdata->powersde[improve] >= max)
        {
           sprintf(buf,"You have already maxed %s.\n\r",arg1);
           send_to_char(buf,ch);
           return;
        }
	if ( cost > ch->practice )
	{
            sprintf(buf,"It costs you %d primal to improve your %s discipline.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powersde[improve] += 1;
	ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s discipline.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a discipline, type: Clandisc <discipline type> improve.\n\r",ch);
    return;
}



void do_oldclandisc( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    int clancount = 0;
    int clanmax = 11;
    int cost = 0;
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch,CLASS_VAMPIRE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->pcdata->stats[UNI_CURRENT] == -1) 
	ch->pcdata->stats[UNI_CURRENT] = ch->pcdata->stats[UNI_AFF];


    if (IS_VAMPAFF(ch,VAM_PROTEAN) || IS_VAMPPASS(ch,VAM_PROTEAN))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_VICISSITUDE) || IS_VAMPPASS(ch,VAM_VICISSITUDE))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_CELERITY) || IS_VAMPPASS(ch,VAM_CELERITY))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_FORTITUDE) || IS_VAMPPASS(ch,VAM_FORTITUDE))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_POTENCE) || IS_VAMPPASS(ch,VAM_POTENCE))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_OBFUSCATE) || IS_VAMPPASS(ch,VAM_OBFUSCATE))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_OBTENEBRATION) || IS_VAMPPASS(ch,VAM_OBTENEBRATION))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_SERPENTIS) || IS_VAMPPASS(ch,VAM_SERPENTIS))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_AUSPEX) || IS_VAMPPASS(ch,VAM_AUSPEX))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_DOMINATE) || IS_VAMPPASS(ch,VAM_DOMINATE))
	clancount = clancount + 1;
    if (IS_VAMPAFF(ch,VAM_PRESENCE) || IS_VAMPPASS(ch,VAM_PRESENCE))
	clancount = clancount + 1;

    if (clancount < 3) cost = 0;
	else cost = (clancount - 2) * 10;
    if (cost > 50) cost = 50;

    if ( arg[0] == '\0' )
    {
	send_to_char("Current powers:",ch);
	if (IS_VAMPAFF(ch,VAM_PROTEAN) && !IS_VAMPPASS(ch,VAM_PROTEAN))
	    send_to_char(" Protean",ch);
	else if (IS_VAMPAFF(ch,VAM_PROTEAN))
	    send_to_char(" PROTEAN",ch);
	if (IS_VAMPAFF(ch,VAM_VICISSITUDE) && !IS_VAMPPASS(ch,VAM_VICISSITUDE))
	    send_to_char(" VICISSITUDE",ch);
	else if (IS_VAMPAFF(ch,VAM_VICISSITUDE))
	    send_to_char(" VICISSITUDE",ch);
	if (IS_VAMPAFF(ch,VAM_CELERITY) && !IS_VAMPPASS(ch,VAM_CELERITY))
	    send_to_char(" Celerity",ch);
	else if (IS_VAMPAFF(ch,VAM_CELERITY))
	    send_to_char(" CELERITY",ch);
	if (IS_VAMPAFF(ch,VAM_FORTITUDE) && !IS_VAMPPASS(ch,VAM_FORTITUDE))
	    send_to_char(" Fortitude",ch);
	else if (IS_VAMPAFF(ch,VAM_FORTITUDE))
	    send_to_char(" FORTITUDE",ch);
	if (IS_VAMPAFF(ch,VAM_POTENCE) && !IS_VAMPPASS(ch,VAM_POTENCE))
	    send_to_char(" Potence",ch);
	else if (IS_VAMPAFF(ch,VAM_POTENCE))
	    send_to_char(" POTENCE",ch);
	if (IS_VAMPAFF(ch,VAM_OBFUSCATE) && !IS_VAMPPASS(ch,VAM_OBFUSCATE))
	    send_to_char(" Obfuscate",ch);
	else if (IS_VAMPAFF(ch,VAM_OBFUSCATE))
	    send_to_char(" OBFUSCATE",ch);
	if (IS_VAMPAFF(ch,VAM_OBTENEBRATION) && !IS_VAMPPASS(ch,VAM_OBTENEBRATION))
	    send_to_char(" Obtenebration",ch);
	else if (IS_VAMPAFF(ch,VAM_OBTENEBRATION))
	    send_to_char(" OBTENEBRATION",ch);
	if (IS_VAMPAFF(ch,VAM_SERPENTIS) && !IS_VAMPPASS(ch,VAM_SERPENTIS))
	    send_to_char(" Serpentis",ch);
	else if (IS_VAMPAFF(ch,VAM_SERPENTIS))
	    send_to_char(" SERPENTIS",ch);
	if (IS_VAMPAFF(ch,VAM_AUSPEX) && !IS_VAMPPASS(ch,VAM_AUSPEX))
	    send_to_char(" Auspex",ch);
	else if (IS_VAMPAFF(ch,VAM_AUSPEX))
	    send_to_char(" AUSPEX",ch);
	if (IS_VAMPAFF(ch,VAM_DOMINATE) && !IS_VAMPPASS(ch,VAM_DOMINATE))
	    send_to_char(" Dominate",ch);
	else if (IS_VAMPAFF(ch,VAM_DOMINATE))
	    send_to_char(" DOMINATE",ch);
	if (IS_VAMPAFF(ch,VAM_PRESENCE) && !IS_VAMPPASS(ch,VAM_PRESENCE))
	    send_to_char(" Presence",ch);
	else if (IS_VAMPAFF(ch,VAM_PRESENCE))
	    send_to_char(" PRESENCE",ch);
    	if (!IS_VAMPAFF(ch,VAM_PROTEAN) && !IS_VAMPPASS(ch,VAM_VICISSITUDE) &&
    	!IS_VAMPAFF(ch,VAM_CELERITY) &&  !IS_VAMPAFF(ch,VAM_FORTITUDE) && 
    	!IS_VAMPAFF(ch,VAM_POTENCE) && !IS_VAMPAFF(ch,VAM_OBFUSCATE) && 
    	!IS_VAMPAFF(ch,VAM_AUSPEX) && !IS_VAMPAFF(ch,VAM_OBTENEBRATION) && 
    	!IS_VAMPAFF(ch,VAM_SERPENTIS) && !IS_VAMPPASS(ch,VAM_PROTEAN) && 
    	!IS_VAMPPASS(ch,VAM_CELERITY) &&  !IS_VAMPPASS(ch,VAM_FORTITUDE) && 
    	!IS_VAMPPASS(ch,VAM_POTENCE) && !IS_VAMPPASS(ch,VAM_OBFUSCATE) && 
    	!IS_VAMPPASS(ch,VAM_AUSPEX) && !IS_VAMPPASS(ch,VAM_DOMINATE) && 
    	!IS_VAMPPASS(ch,VAM_PRESENCE) && !IS_VAMPPASS(ch,VAM_OBTENEBRATION) && 
    	!IS_VAMPPASS(ch,VAM_SERPENTIS) && !IS_VAMPAFF(ch,VAM_VICISSITUDE))
	    send_to_char(" None",ch);
	send_to_char(".\n\r",ch);
	if (clancount < clanmax)
	{
	    sprintf(buf,"It will cost you %d primal to gain another discipline.\n\r",cost);
	    send_to_char(buf,ch);
	}
	if (cost <= ch->practice) send_to_char("Select from:",ch); else return;
	if (!IS_VAMPAFF(ch,VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (!IS_VAMPAFF(ch,VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
	if (!IS_VAMPAFF(ch,VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (!IS_VAMPAFF(ch,VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (!IS_VAMPAFF(ch,VAM_POTENCE)) send_to_char(" Potence",ch);
	if (!IS_VAMPAFF(ch,VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (!IS_VAMPAFF(ch,VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (!IS_VAMPAFF(ch,VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (!IS_VAMPAFF(ch,VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (!IS_VAMPAFF(ch,VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (!IS_VAMPAFF(ch,VAM_PRESENCE)) send_to_char(" Presence",ch);
	send_to_char(".\n\r",ch);
	return;
    }
    if ( clancount >= clanmax )
    {
	if ( !str_cmp(arg,"protean") && (IS_VAMPAFF(ch,VAM_PROTEAN) || IS_VAMPPASS(ch,VAM_PROTEAN)))
	    send_to_char( "Powers: Nightsight, Claws, Change.\n\r", ch );
	else if ( !str_cmp(arg,"vicissitude") && (IS_VAMPAFF(ch,VAM_VICISSITUDE) || IS_VAMPPASS(ch,VAM_VICISSITUDE)) )
	    send_to_char( "Powers: Body Control.\n\r", ch );
	else if ( !str_cmp(arg,"celerity") && (IS_VAMPAFF(ch,VAM_CELERITY) || IS_VAMPPASS(ch,VAM_CELERITY)) )
	    send_to_char( "Powers: An extra attack, Reduced move cost for spells.\n\r", ch );
	else if ( !str_cmp(arg,"fortitude") && (IS_VAMPAFF(ch,VAM_FORTITUDE) || IS_VAMPPASS(ch,VAM_FORTITUDE)) )
	    send_to_char( "Powers: 1-100% damage reduction, +50 one time hp bonus.\n\r", ch );
	else if ( !str_cmp(arg,"potence") && (IS_VAMPAFF(ch,VAM_POTENCE) || IS_VAMPPASS(ch,VAM_POTENCE)) )
	    send_to_char( "Powers: 150% normal damage in combat.\n\r", ch );
	else if ( !str_cmp(arg,"obfuscate") && (IS_VAMPAFF(ch,VAM_OBFUSCATE) || IS_VAMPPASS(ch,VAM_OBFUSCATE)) )
	    send_to_char( "Powers: Mask, Mortal, Shield.\n\r", ch );
	else if ( !str_cmp(arg,"obtenebration") && (IS_VAMPAFF(ch,VAM_OBTENEBRATION) || IS_VAMPPASS(ch,VAM_OBTENEBRATION)) )
	    send_to_char( "Powers: Shadowplane, Shadowsight, Nightsight.\n\r", ch );
	else if ( !str_cmp(arg,"serpentis") && (IS_VAMPAFF(ch,VAM_SERPENTIS) || IS_VAMPPASS(ch,VAM_SERPENTIS)) )
	    send_to_char( "Powers: Darkheart, Serpent, Poison, Nightsight.\n\r", ch );
	else if ( !str_cmp(arg,"auspex") && (IS_VAMPAFF(ch,VAM_AUSPEX) || IS_VAMPPASS(ch,VAM_AUSPEX)) )
	    send_to_char( "Powers: Truesight, Scry, Readaura.\n\r", ch );
	else if ( !str_cmp(arg,"dominate") && (IS_VAMPAFF(ch,VAM_DOMINATE) || IS_VAMPPASS(ch,VAM_DOMINATE)) )
	    send_to_char( "Powers: Evileye, Command, Shield, Ghoul.\n\r", ch );
	else if ( !str_cmp(arg,"presence") && (IS_VAMPAFF(ch,VAM_PRESENCE) || IS_VAMPPASS(ch,VAM_PRESENCE)) )
	    send_to_char( "Powers: Majesty.\n\r", ch );
	else
	    send_to_char( "You don't know any such Discipline.\n\r", ch );
	return;
    }
    if ( !str_cmp(arg,"protean") )
    {
	if (IS_VAMPAFF(ch,VAM_PROTEAN) || IS_VAMPPASS(ch,VAM_PROTEAN)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Nightsight, Claws, Change.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Protean.\n\r", ch );
/*
        clan_table_powerselect(ch,"Protean");
*/
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_PROTEAN);
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_PROTEAN);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"celerity") )
    {
	if (IS_VAMPAFF(ch,VAM_CELERITY) || IS_VAMPPASS(ch,VAM_CELERITY)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: An extra attack, Reduced move cost for spells.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Celerity.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_CELERITY);
/*
        clan_table_powerselect(ch,"Celerity");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_CELERITY);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"fortitude") )
    {
	if (IS_VAMPAFF(ch,VAM_FORTITUDE) || IS_VAMPPASS(ch,VAM_FORTITUDE)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: 1-100% damage reduction, +50 one time hp bonus.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Fortitude.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_FORTITUDE);
/*
        clan_table_powerselect(ch,"Fortitude");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_FORTITUDE);
	ch->practice -= cost;
	ch->max_hit = ch->max_hit + 50;
	ch->hit = ch->hit + 50;
	return;
    }
    else if ( !str_cmp(arg,"potence") )
    {
	if (IS_VAMPAFF(ch,VAM_POTENCE) || IS_VAMPPASS(ch,VAM_POTENCE)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: 150% normal damage in combat.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Potence.\n\r", ch );
/*
        clan_table_powerselect(ch,"Potence");
*/
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_POTENCE);
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_POTENCE);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"obfuscate") )
    {
	if (IS_VAMPAFF(ch,VAM_OBFUSCATE) || IS_VAMPPASS(ch,VAM_OBFUSCATE)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Mask, Mortal, Shield.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Obfuscate.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_OBFUSCATE);
/*
        clan_table_powerselect(ch,"Obfuscate");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_OBFUSCATE);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"obtenebration") )
    {
	if (IS_VAMPAFF(ch,VAM_OBTENEBRATION) || IS_VAMPPASS(ch,VAM_OBTENEBRATION)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Shadowplane, Shadowsight, Nightsight.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Obtenebration.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_OBTENEBRATION); 
/*
        clan_table_powerselect(ch,"Obtenebration");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_OBTENEBRATION);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"serpentis") )
    {
	if (IS_VAMPAFF(ch,VAM_SERPENTIS) || IS_VAMPPASS(ch,VAM_SERPENTIS)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Darkheart, Serpent, Poison, Nightsight.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Serpentis.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_SERPENTIS);
/*
        clan_table_powerselect(ch,"serpentis");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_SERPENTIS);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"vicissitude") )
    {
	if (IS_VAMPAFF(ch,VAM_VICISSITUDE) || IS_VAMPPASS(ch,VAM_VICISSITUDE)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Zulo, Horns, Tail, Bonus AC.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Vicissitude.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_VICISSITUDE);
/*
        clan_table_powerselect(ch,"Vicissitude");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_VICISSITUDE);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"auspex") )
    {
	if (IS_VAMPAFF(ch,VAM_AUSPEX) || IS_VAMPPASS(ch,VAM_AUSPEX)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Truesight, Scry, Readaura.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Auspex.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_AUSPEX);
/*
        clan_table_powerselect(ch,"Auspex");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_AUSPEX);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"dominate") )
    {
	if (IS_VAMPAFF(ch,VAM_DOMINATE) || IS_VAMPPASS(ch,VAM_DOMINATE)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Evileye, Command, Shield, Ghoul.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Dominate.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_DOMINATE);
/*
        clan_table_powerselect(ch,"Dominate");
*/
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_DOMINATE);
	ch->practice -= cost;
	return;
    }
    else if ( !str_cmp(arg,"presence") )
    {
	if (IS_VAMPAFF(ch,VAM_PRESENCE) || IS_VAMPPASS(ch,VAM_PRESENCE)
	    || cost > ch->practice)
	{
	    send_to_char( "Powers: Majesty.\n\r", ch );
	    return;
	}
	send_to_char( "You master the discipline of Presence.\n\r", ch );
	if (clancount < 3) SET_BIT(ch->pcdata->stats[UNI_CURRENT],VAM_PRESENCE);
	SET_BIT(ch->pcdata->stats[UNI_AFF],VAM_PRESENCE);
	ch->practice -= cost;
	return;
    }
    else
	send_to_char( "No such discipline.\n\r", ch );
    return;
}

void do_defences( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_IMMUNE(ch, IMM_VAMPIRE))
    {
	send_to_char("You lower your defences!.\n\r",ch);
	SET_BIT(ch->immune, IMM_VAMPIRE);
	return;
    }
    send_to_char("You raise your defences!\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_VAMPIRE);
    return;
}

void do_shadowplane( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    
    if ( !IS_VAMPIRE(ch) && !IS_WEREWOLF(ch) &&
         !IS_MAGE(ch) )
    {
        send_to_char("Huh?\n\r",ch );
        return;
    }

    if (ch->fight_timer >0)
    {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
    }

    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->gifts[SILENTSTRIDERS] < 5 )
    {
       stc("You must obtain the fifth gift of the Silent Striders to Reach the Umbra.\n\r",ch);
       return;
    }
    else if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->powersde[VAM_OBTE] < 3)
    {
        send_to_char("You are not trained in the Obtenebration discipline.\n\r",ch);
        return;
    }
    else if (IS_CLASS(ch, CLASS_MAGE) && ch->spheres[MMIN] <5)
    {
        send_to_char("Your mind sphere control must be 5 for this.\n\r",ch);
        return;
    }
    if ( arg[0] == '\0' )
    {
    	if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	{
	 if (IS_VAMPIRE(ch) && ch->pcdata->condition[COND_THIRST] < 75)
    	 {
            send_to_char("You have insufficient blood.\n\r",ch);
            return;
         }
	 if (IS_WEREWOLF(ch) && ch->gnosis[GCURRENT] < 5 )
    	 {
            send_to_char("You have insufficient gnosis.\n\r",ch);
            return;
         }

    if (IS_VAMPIRE(ch))
        ch->pcdata->condition[COND_THIRST] -= number_range(65,75);
    if (IS_WEREWOLF(ch))
        ch->gnosis[GCURRENT] -= 5;

	    send_to_char("You fade into the plane of shadows.\n\r",ch);
	    act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    do_look(ch,"auto");
	    return;
    	}
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You fade back into the real world.\n\r",ch);
	act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
    	send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}


void do_introduce( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->stats[UNI_GEN] > 0 && ch->pcdata->stats[UNI_GEN] < 8)
	do_tradition(ch,ch->lord);
    else if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_HERO(ch) &&
	ch->pcdata->stats[UNI_GEN] > 0 && ch->pcdata->stats[UNI_GEN] < 5)
	do_bloodline(ch,ch->lord);
    else send_to_char("Huh?\n\r",ch);
    return;
}

void do_bloodline( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char      arg3 [MAX_INPUT_LENGTH];
    char      buf  [MAX_STRING_LENGTH];
    char      buf2 [MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if (IS_NPC(ch)) return;

    if (arg1 != '\0') arg1[0] = UPPER(arg1[0]);
    if (arg2 != '\0') arg2[0] = UPPER(arg2[0]);
    if (arg3 != '\0') arg3[0] = UPPER(arg3[0]);

    if (!str_cmp(arg1,"kavir")) strcpy(arg1,"KaVir");

    if ( !str_cmp(ch->clan,"") && ch->pcdata->stats[UNI_GEN] != 1 )
    {
	strcpy(buf,"In the name of Gaia, I announce my Garou heritage.");
	do_say(ch,buf);
	sprintf(buf,"My name is %s, I am a Ronin of no tribe.", ch->name);
	do_say(ch,buf);
	return;
    }
    strcpy(buf,"In the name of Gaia, I announce my Garou heritage.");
    do_say(ch,buf);
    if (ch->pcdata->stats[UNI_GEN] != 1)
    {
	if      (ch->pcdata->stats[UNI_GEN] == 4) sprintf(buf2,"%s",arg3);
	else if (ch->pcdata->stats[UNI_GEN] == 3) sprintf(buf2,"%s",arg2);
	else if (ch->pcdata->stats[UNI_GEN] == 2) sprintf(buf2,"%s",arg1);
	if (ch->pcdata->stats[UNI_GEN] == 1)
	    sprintf(buf,"My name is %s, chosen Champion of Gaia.", ch->name);
	if (ch->pcdata->stats[UNI_GEN] == 2)
	    sprintf(buf,"My name is %s, Chieftain of the %s tribe, pup of %s.", ch->name,ch->clan,buf2);
	else
	    sprintf(buf,"My name is %s, of the %s tribe, pup of %s.", ch->name,ch->clan,buf2);
	do_say(ch,buf);
    }
    if ( arg3[0] != '\0' )
    {
	sprintf(buf,"My name is %s, of the %s tribe, pup of %s.",
	arg3,ch->clan,arg2);
	do_say(ch,buf);
    }
    if ( arg2[0] != '\0' )
    {
	if ( arg1[0] != '\0' )
	    sprintf(buf,"My name is %s, Chieftain of the %s tribe, pup of %s.",arg2,ch->clan,arg1);
	else
	    sprintf(buf,"My name is %s, of the %s tribe, pup of %s.",arg2,ch->clan,arg1);
	do_say(ch,buf);
    }
    if ( ch->pcdata->stats[UNI_GEN] == 1 )
	sprintf(buf,"My name is %s, chosen Champion of Gaia.", ch->name);
    else
	sprintf(buf,"My name is %s, chosen Champion of Gaia.", arg1);
    do_say(ch,buf);
    return;
}

void do_tradition( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char      arg3 [MAX_INPUT_LENGTH];
    char      arg4 [MAX_INPUT_LENGTH];
    char      arg5 [MAX_INPUT_LENGTH];
    char      arg6 [MAX_INPUT_LENGTH];
    char      buf  [MAX_INPUT_LENGTH];
    char      buf2 [MAX_INPUT_LENGTH];
    char      buf3 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );
    argument = one_argument( argument, arg5 );
    argument = one_argument( argument, arg6 );

    if (IS_NPC(ch)) return;

    if (arg1 != '\0') arg1[0] = UPPER(arg1[0]);
    if (arg2 != '\0') arg2[0] = UPPER(arg2[0]);
    if (arg3 != '\0') arg3[0] = UPPER(arg3[0]);
    if (arg4 != '\0') arg4[0] = UPPER(arg4[0]);
    if (arg5 != '\0') arg5[0] = UPPER(arg5[0]);
    if (arg6 != '\0') arg6[0] = UPPER(arg6[0]);

    if (!str_cmp(arg1,"kavir")) strcpy(arg1,"KaVir");

    if (!IS_CLASS(ch, CLASS_VAMPIRE) || (ch->pcdata->stats[UNI_GEN] < 1) || (ch->pcdata->stats[UNI_GEN] > 7))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
/*
    if ( !str_cmp(ch->clan,"") && ch->pcdata->stats[UNI_GEN] == 2 )
    {
	send_to_char( "Not until you've created your own clan!\n\r", ch );
	return;
    }
*/
    if      (ch->pcdata->stats[UNI_GEN] == 2)                   sprintf(buf3,"Antediluvian");
    else if (ch->pcdata->rank == AGE_NEONATE)    sprintf(buf3,"Neonate");
    else if (ch->pcdata->rank == AGE_ANCILLA)    sprintf(buf3,"Ancilla");
    else if (ch->pcdata->rank == AGE_ELDER)      sprintf(buf3,"Elder");
    else if (ch->pcdata->rank == AGE_METHUSELAH) sprintf(buf3,"Methuselah");
    else                                         sprintf(buf3,"Childe");

    if      (ch->pcdata->stats[UNI_GEN] == 7) sprintf(buf2,"Seventh");
    else if (ch->pcdata->stats[UNI_GEN] == 6) sprintf(buf2,"Sixth");
    else if (ch->pcdata->stats[UNI_GEN] == 5) sprintf(buf2,"Fifth");
    else if (ch->pcdata->stats[UNI_GEN] == 4) sprintf(buf2,"Fourth");
    else if (ch->pcdata->stats[UNI_GEN] == 3) sprintf(buf2,"Third");
    else if (ch->pcdata->stats[UNI_GEN] == 2) sprintf(buf2,"Second");
    if (ch->pcdata->stats[UNI_GEN] == 1)
	sprintf(buf,"As is the tradition, I recite the lineage of %s, Sire of all Kindred.",ch->name);
    else
	sprintf(buf,"As is the tradition, I recite the lineage of %s, %s of the %s Generation.",ch->name,buf3,buf2);
    do_say(ch,buf);
    if ( ch->pcdata->stats[UNI_GEN] != 1 )
    {
	if      (ch->pcdata->stats[UNI_GEN] == 7) sprintf(buf2,"%s",arg6);
	else if (ch->pcdata->stats[UNI_GEN] == 6) sprintf(buf2,"%s",arg5);
	else if (ch->pcdata->stats[UNI_GEN] == 5) sprintf(buf2,"%s",arg4);
	else if (ch->pcdata->stats[UNI_GEN] == 4) sprintf(buf2,"%s",arg3);
	else if (ch->pcdata->stats[UNI_GEN] == 3) sprintf(buf2,"%s",arg2);
	else if (ch->pcdata->stats[UNI_GEN] == 2) sprintf(buf2,"%s",arg1);

	if (IS_SET(ch->special, SPC_ANARCH) || strlen(ch->clan) < 2)
	    sprintf(buf,"My name is %s. I am of no clan. My sire is %s.", ch->name,buf2);
	else if (ch->pcdata->stats[UNI_GEN] == 2)
	    sprintf(buf,"My name is %s. I am the %s Primogen. My sire is %s.", ch->name,ch->clan,buf2);
	else
	    sprintf(buf,"My name is %s. I am of %s. My sire is %s.", ch->name,ch->clan,buf2);
	do_say(ch,buf);
    }
    if ( arg6[0] != '\0' )
    {
	sprintf(buf,"My name is %s. My sire is %s.",
	arg6,arg5);
	do_say(ch,buf);
    }
    if ( arg5[0] != '\0' )
    {
	sprintf(buf,"My name is %s. My sire is %s.",
	arg5,arg4);
	do_say(ch,buf);
    }
    if ( arg4[0] != '\0' )
    {
	sprintf(buf,"My name is %s. My sire is %s.",
	arg4,arg3);
	do_say(ch,buf);
    }
    if ( arg3[0] != '\0' )
    {
	sprintf(buf,"My name is %s. My sire is %s.",
	arg3,arg2);
	do_say(ch,buf);
    }
    if ( arg2[0] != '\0' )
    {
	sprintf(buf,"My name is %s. My sire is %s.",arg2,arg1);
	do_say(ch,buf);
    }
    if ( ch->pcdata->stats[UNI_GEN] == 1 )
	sprintf(buf,"My name is %s. All Kindred are my childer.",ch->name);
    else
	sprintf(buf,"My name is %s. All Kindred are my childer.",arg1);
    do_say(ch,buf);
    if ( ch->pcdata->stats[UNI_GEN] == 7 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s, childe of %s, childe of %s, childe of %s. Recognize my lineage.",ch->name,arg6,arg5,arg4,arg3,arg2,arg1);
    if ( ch->pcdata->stats[UNI_GEN] == 6 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s, childe of %s, childe of %s. Recognize my lineage.",ch->name,arg5,arg4,arg3,arg2,arg1);
    if ( ch->pcdata->stats[UNI_GEN] == 5 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s, childe of %s. Recognize my lineage.",ch->name,arg4,arg3,arg2,arg1);
    if ( ch->pcdata->stats[UNI_GEN] == 4 ) sprintf(buf,"My name is %s, childe of %s, childe of %s, childe of %s. Recognize my lineage.",ch->name,arg3,arg2,arg1);
    if ( ch->pcdata->stats[UNI_GEN] == 3 ) sprintf(buf,"My name is %s, childe of %s, childe of %s. Recognize my lineage.",ch->name,arg2,arg1);
    if ( ch->pcdata->stats[UNI_GEN] == 2 ) sprintf(buf,"My name is %s, childe of %s. Recognize my lineage.",ch->name,arg1);
    if ( ch->pcdata->stats[UNI_GEN] == 1 ) sprintf(buf,"My name is %s. Recognize my lineage.",ch->name);
    do_say(ch,buf);
    return;
}

void do_darkheart( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
	if (ch->pcdata->powersde[VAM_SERP] < 1  )
    {
	send_to_char("You need level 1 serpent first.\n\r",ch);
	return;
    }
    if (IS_IMMUNE(ch,IMM_STAKE) )
    {
	send_to_char("But you've already torn your heart out!\n\r",ch);
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 100 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= 100;
    send_to_char("You rip your heart from your body and toss it to the ground.\n\r",ch);
    act("$n rips $s heart out and tosses it to the ground.", ch, NULL, NULL, TO_ROOM);
    make_part( ch, "heart" );
    ch->hit = ch->hit - number_range(10,20);
    update_pos(ch);
    if (ch->position == POS_DEAD && !IS_HERO(ch))
    {
	send_to_char( "You have been KILLED!!\n\r\n\r", ch );
	raw_kill(ch, ch);
	return;
    }
    SET_BIT(ch->immune, IMM_STAKE);
    return;
}

void do_truesight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

     if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_HIGHLANDER) 
     && !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_DEMON) &&
	!IS_ABOMINATION(ch)&& !IS_SHAKRIN(ch) && !IS_CLASS(ch, CLASS_DRAGON))
     {
     send_to_char("Huh?\n\r",ch);
     return;
     }
         
    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_TRUESIGHT))
	{
	    send_to_char("You haven't been granted the gift of truesight.\n\r",ch);
	    return;
	}
    }
    if (IS_CLASS(ch, CLASS_DRAGON))
    {
        if (!IS_DRAPOWER( ch, DRA_TRUESIGHT))
        { stc("You haven't gained that power.\n\r",ch); return; }
    }
    if (( IS_CLASS(ch, CLASS_VAMPIRE) || IS_ABOMINATION(ch)) && ch->pcdata->powersde[VAM_AUSP] < 1) 
    {
	send_to_char("You need level 1 auspex before you can use truesight.\n\r",ch);
	return;
    }
    
    if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 3 )
    {
      stc("You must obtain level 3 Awareness to use Truesight.\n\r",ch);
      return;
    }                                                

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses increase to incredible proportions.\n\r", ch );
    }

    return;
}
/*
void do_majesty( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_VAMPAFF(ch,VAM_PRESENCE) )
    {
	send_to_char("You are not trained in the Presence discipline.\n\r",ch);
	return;
    }

    if (arg1[0] != '\0' && !str_cmp(arg1,"on"))
    {
	if (IS_EXTRA(ch, EXTRA_FAKE_CON))
	{send_to_char("You already have Majesty on.\n\r",ch); return;}
	SET_BIT(ch->extra, EXTRA_FAKE_CON);
	send_to_char("Your Majesty is now ON.\n\r",ch);
	return;
    }
    if (arg1[0] != '\0' && !str_cmp(arg1,"off"))
    {
	if (!IS_EXTRA(ch, EXTRA_FAKE_CON))
	{send_to_char("You already have Majesty off.\n\r",ch); return;}
	REMOVE_BIT(ch->extra, EXTRA_FAKE_CON);
	send_to_char("Your Majesty is now OFF.\n\r",ch);
	return;
    }
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char("You have the following stats:\n\r",ch);
	sprintf(buf,"Hitroll: %d, Actual: %d.\n\r",
		ch->pcdata->fake_hit, char_hitroll(ch));
	send_to_char(buf, ch);
	sprintf(buf,"Damroll: %d, Actual: %d.\n\r",
		ch->pcdata->fake_dam, char_damroll(ch));
	send_to_char(buf, ch);
	sprintf(buf,"Armour: %d, Actual: %d.\n\r",
		ch->pcdata->fake_ac, char_ac(ch));
	send_to_char(buf, ch);
	sprintf(buf,"Hp: %d, Actual: %ld.\n\r",
		ch->pcdata->fake_hp, ch->hit);
	send_to_char(buf, ch);
	sprintf(buf,"Mana: %d, Actual: %d.\n\r",
		ch->pcdata->fake_mana, ch->mana);
	send_to_char(buf, ch);
	sprintf(buf,"Move: %d, Actual: %d.\n\r",
		ch->pcdata->fake_move, ch->move);
	send_to_char(buf, ch);
	return;
    }
    value = is_number( arg2 ) ? atoi( arg2 ) : -10000;
    if (!str_cmp(arg1,"hit") || !str_cmp(arg1,"hitroll"))
    {
	if (value < 0 || value > 1000)
	{
	    send_to_char("Please enter a value between 0 and 1000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_hit = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"dam") || !str_cmp(arg1,"damroll"))
    {
	if (value < 0 || value > 1000)
	{
	    send_to_char("Please enter a value between 0 and 1000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_dam = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"ac") || !str_cmp(arg1,"armour") || !str_cmp(arg1,"armor"))
    {
	if (value < -1000 || value > 100)
	{
	    send_to_char("Please enter a value between -1000 and 100.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_ac = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"hp") || !str_cmp(arg1,"hitpoints"))
    {
	if (value < 1 || value > 30000)
	{
	    send_to_char("Please enter a value between 1 and 30000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_hp = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"mana"))
    {
	if (value < 1 || value > 30000)
	{
	    send_to_char("Please enter a value between 1 and 30000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_mana = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    if (!str_cmp(arg1,"move"))
    {
	if (value < 1 || value > 30000)
	{
	    send_to_char("Please enter a value between 1 and 30000.\n\r",ch);
	    return;
	}
	ch->pcdata->fake_move = value;
	send_to_char("Ok.\n\r",ch);
	return;
    }
    send_to_char("You can set: Hit, Dam, Ac, Hp, Mana, Move.\n\r",ch);
    return;
}
*/

void do_scry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 4 )
    {
	stc("You must obtain level 4 Awareness to use scry.\n\r",ch);
  	return;
    }

    if (IS_CLASS(ch, CLASS_DRAGON) && !IS_DRAPOWER(ch, DRA_SCRY)) {
    stc("Gain it first, moron!\n\r",ch); return; }

  if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->powersde[VAM_AUSP] < 2 )
    {
        stc("You must obtain level 2 Auspex to scry.\n\r",ch);
        return;
    }
    else if ( IS_ABOMINATION(ch) )
	{
	send_to_char("You require level 2 Auspex to Scry.\n\r",ch);
	return;
	}

    else if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER(ch, DEM_SCRY))
	{
	    send_to_char("You haven't been granted the gift of scry.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_MAGE) && ch->spheres[MCOR] < 2 )
    {
	stc("You must obtain level two Correspondence to use Scry.\n\r",ch);
	return;
    }
    else if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_SORA] < 1 )
    {
        send_to_char("You have not learned the Sora principle to 1.\n\r", ch);
        return;
    }
    else if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ITEMAFF(ch, ITEMA_VISION)
             && !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_MAGE)
	    && !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_DRAGON))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->move < 1000) { stc("You don't have enough movement points.\n\r",ch); return; }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_NINJA) && ch-> move < 600 )
    {
      send_to_char("You don't have enough movement points to scry someone.\n\r", ch);
      return;
    }

    if (!IS_CLASS(ch, CLASS_DEMON) && !IS_SET(ch->special, SPC_CHAMPION) && 
	ch->pcdata->condition[COND_THIRST] < 25 && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_MAGE) 
	&& !IS_CLASS(ch, CLASS_MONK))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if (!IS_ITEMAFF(ch, ITEMA_VISION) && IS_VAMPIRE(ch))
	ch->pcdata->condition[COND_THIRST] -= number_range(15,25);

    if ( IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->flag2, AFF2_SPIRITGUARD) )
    {
      act("Your spirit guardian sends you a message:",ch,NULL,victim,TO_VICT);
      act("\"$n is scrying you.\"",ch,NULL,victim,TO_VICT);
    } 

    if ( IS_CLASS(victim, CLASS_DEMON) && IS_SET(victim->flag2, AFF2_IMP) )
    {
      act("Your Imp whispers in your ear:",ch,NULL,victim,TO_VICT);
      act("\"$n is scrying you.\"",ch,NULL,victim,TO_VICT);
    }

    if ( IS_CLASS(victim, CLASS_HIGHLANDER) && IS_SET(victim->flag2, AFF2_SIXTHSENSE) )
    {
      act("Your Sixthsense kicks in:",ch,NULL,victim,TO_VICT);
      act("\"$n is scrying you.\"",ch,NULL,victim,TO_VICT);
    } 

    if ( IS_IMMORTAL(victim) )
    {
        send_to_char("You are unable to scry the Immortals.\n\r",ch);
        return;
    }

    if ( IS_SET(victim->newbits, TEST_CHAR) )
    {
        send_to_char("You are unable to scry Test Characters.\n\r",ch);
        return;
    }

    if ( IS_ITEMAFF(victim, ITEMA_RESISTANCE) )
    {
       send_to_char("You are unable to locate them.\n\r",ch);
        return;
    }                   

    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
	(IS_CLASS(victim, CLASS_DROW) && IS_SET(ch->pcdata->powersd[1],DPOWER_DROWSHIELD))) 
	&& !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You are unable to locate them.\n\r",ch);
	return;
    }

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    ch->move -= 1000;
    if(ch->move < 0) ch->move = 0;
  return;

}

void do_readaura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ITEMAFF(ch, ITEMA_VISION)
        && !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_MONK)
/*Changed by Leoncour to give Highlanders Readaura.*/
        && !IS_CLASS(ch, CLASS_HIGHLANDER))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_SORA] < 2 )
	{
	  send_to_char("You have not learned the Sora principle to 2.\n\r", ch);
	  return;
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->move < 600 )
	{
	 send_to_char("You don't have enough movement points to readaura.\n\r", ch);
	 return;
	}
	if (IS_CLASS(ch, CLASS_MONK) && ch->monkab[SPIRIT] < 2 )
	{
	    send_to_char("You have not learned that ability.\n\r", ch);
	    return;
	}
	if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->powersde[VAM_AUSP] <
5
&& !IS_ITEMAFF(ch, ITEMA_VISION) &&
        !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_MONK) )
    {
	send_to_char("You need level 3 auspex first.\n\r",ch);
	return;
	    }

/*Changed by Leoncour to give Highlanders ReadAura.*/
   if (IS_HIGHLANDER(ch) && ch->race < 5)
      {
	send_to_char("You are not powerful enough to use Read Auras.\n\r",ch);
        return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}
    if ( ch->pcdata->condition[COND_THIRST] < 50 && !IS_ITEMAFF(ch, ITEMA_VISION)
    	     && !IS_NINJA(ch) && !IS_MONK(ch) &&
             !IS_HIGHLANDER(ch))
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (!IS_ITEMAFF(ch, ITEMA_VISION))
	    ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if (ch->pcdata->condition[COND_THIRST] < 50 && !IS_ITEMAFF(ch,ITEMA_VISION)
        && !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_MONK) && !IS_HIGHLANDER(ch))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_VISION))
	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powersd[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You are unable to read their aura.\n\r",ch);
	return;
    }

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
	if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    sprintf(buf,"Hp:%ld/%ld, Mana:%ld/%ld, Move:%ld/%ld.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
	send_to_char("Disciplines:",ch);
	if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" vicissitude",ch);
	if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
	if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
	send_to_char(".\n\r",ch);
    }
    if ( IS_CLASS(ch, CLASS_NINJA) ) { ch->move -= 600; return; }
    return;
}

void do_mortal( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_VAMPAFF(ch, VAM_MORTAL))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
	 if (ch->pcdata->powersde[VAM_OBFU] < 4)
    {
	send_to_char("You need level 4 obfuscate first.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
        if ( ch->pcdata->condition[COND_THIRST] < 100 )
        {
	    send_to_char("You must be at full blood to use this power.\n\r",ch);
	    return;
    	}
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->pcdata->stats[UNI_RAGE] = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("Colour returns to your skin and you warm up a little.\n\r",ch);
    	act("Colour returns to $n's skin.", ch, NULL, NULL, TO_ROOM);

    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
	return;
    }
    send_to_char("You skin pales and cools.\n\r",ch);
    act("$n's skin pales slightly.", ch, NULL, NULL, TO_ROOM);
    ch->class=CLASS_VAMPIRE;
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
    return;
}

void do_mortalvamp( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->pcdata->stats[UNI_RAGE] = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("You loose your vampire powers.\n\r",ch);

    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
	return;
    }
    send_to_char("You regain your vampire powers.\n\r",ch);
    ch->class= CLASS_VAMPIRE;
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
    return;
}

void do_shield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;


    if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE)
	&& !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_DEMON) 
	&& !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_HIGHLANDER)
      && !IS_CLASS(ch, CLASS_HUNTER) && !IS_ABOMINATION(ch))
    {
	stc("Huh?\n\r",ch);
	return;
    }
    if(IS_CLASS(ch,CLASS_WEREWOLF) && ch->gifts[SILVERFANGS] < 3)
	{stc("You require level 3 silverfangs to use mindblock.\n\r",ch);
	return;}

else if ( IS_ABOMINATION(ch) )
	    send_to_char("You require level 4 Obfuscate or Silver Fangs to Shield your aura.\n\r",ch);

   if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powersd[1],DPOWER_DROWSHIELD))
	{  send_to_char("You don't have that power.\n\r", ch );
		return;}

    if (IS_CLASS(ch, CLASS_VAMPIRE) &&  ch->pcdata->powersde[VAM_OBFU] < 2)
    {
   	send_to_char("You need level 2 obfuscate first.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
        if (!IS_DEMPOWER( ch, DEM_SHIELD))
        {
            send_to_char("You haven't been granted the gift of shielding.\n\r",ch);
            return;
        }
    }
 
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 60)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if ( IS_CLASS(ch, CLASS_MAGE) && ( ch->spheres[MCOR] < 2 && ch->spheres[MMIN] < 1 ) )
    {
	stc("You must obtain level two Correspondence to Shield your being.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_HIGHLANDER) && (ch->race < 10) )
    {
        stc("You are not powerful enough to do that yet.\n\r",ch);
        return;
    }

    if (IS_CLASS(ch, CLASS_HUNTER) && (ch->mana < 50) )
    {
        stc("Generic fail message.\n\r", ch);
        return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(50,60);

    if (!IS_IMMUNE(ch,IMM_SHIELDED) ) 
    {
if (IS_WEREWOLF(ch))
    {
        send_to_char("You raise your psychic shield.\n\r",ch);
    }
else if (!IS_WEREWOLF(ch))
{
        send_to_char("You shield your aura from those around you.\n\r",ch);
}
        SET_BIT(ch->immune, IMM_SHIELDED);
        return;
    }
      if (IS_WEREWOLF(ch))
    {
    send_to_char("You lower your psychic shield.\n\r",ch);
    }
else if (!IS_WEREWOLF(ch))
{
   send_to_char("You stop shielding your aura.\n\r",ch);
}
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
}

/*
    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You shield your aura from those around you.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
	return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
}
*/

void do_serpent( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (ch->pcdata->powersde[VAM_SERP] < 2)
    {
	send_to_char("You need level 2 serpent first.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	if (!IS_POLYAFF(ch,POLY_SERPENT))
	{
	    send_to_char( "You cannot polymorph from this form.\n\r", ch );
	    return;
	}
	act( "You transform back into human.", ch, NULL, NULL, TO_CHAR );
	act( "$n transform into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->polyaff, POLY_SERPENT);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	clear_stats(ch);
	free_string( ch->morph );
	ch->morph = str_dup( "" );
	ch->max_hit = ch->max_hit - 250;
	ch->hit = ch->hit - 250;
	if (ch->hit < 1) ch->hit = 1;
	ch->max_mana = ch->max_mana + 50;
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    clear_stats(ch);
    if (ch->wpn[0] > 0)
    {
    	ch->hitroll += ch->wpn[0] * 0.75;
    	ch->damroll += ch->wpn[0] * 0.75;
    	ch->armor   -= ch->wpn[0] * 3;
    }
    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    ch->pcdata->mod_str = 10;
    act( "You transform into a huge serpent.", ch, NULL, NULL, TO_CHAR );
    act( "$n transforms into a huge serpent.", ch, NULL, NULL, TO_ROOM );
    SET_BIT(ch->polyaff, POLY_SERPENT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf, "%s the huge serpent", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    ch->max_hit = ch->max_hit + 250;
    ch->hit = ch->hit + 250;
    ch->max_mana = ch->max_mana - 50;
    return;
}
/*
void do_poison( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_VAMPAFF(ch,VAM_SERPENTIS) )
    {
	send_to_char("You are not trained in the Serpentis discipline.\n\r",ch);
	return;
    }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) )
    {
	send_to_char( "You must wield the weapon you wish to poison.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 )
    {
	send_to_char( "This weapon cannot be poisoned.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 15 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(5,15);
    act("You run your tongue along $p, poisoning it.",ch,obj,NULL,TO_CHAR);
    act("$n runs $s tongue along $p, poisoning it.",ch,obj,NULL,TO_ROOM);
    obj->value[0] = 53;
    obj->timer = number_range(10,20);

    return;
}
*/
void do_regenerate( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];
/* Changed by Toaster 17/02/00 used to be 5 and 10*/
    int min = 50;
    int max = 100;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[UNI_RAGE] > 0 && !IS_ABOMINATION(ch))
    {
	send_to_char("You cannot control your regenerative powers while the beast is so strong.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char("You cannot regenerate while fighting.\n\r",ch);
	return;
    }

    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
	send_to_char("But you are already completely regenerated!\n\r",ch);
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 5 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(2,5);
    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
    	send_to_char("Your body has completely regenerated.\n\r",ch);
    	act("$n's body completely regenerates itself.",ch,NULL,NULL,TO_ROOM);
    }
    else
    	send_to_char("Your body slowly regenerates itself.\n\r",ch);
    if (ch->hit < 1 )
    {
	ch->hit = ch->hit + 1;
	update_pos(ch);
	WAIT_STATE( ch, 24 );
    }
    else
    {
	min += 50 - ch->pcdata->stats[UNI_GEN];
	min += 50 - (ch->pcdata->stats[UNI_GEN] * 2);
    	ch->hit = UMIN(ch->hit + number_range(min,max), ch->max_hit);
    	ch->mana = UMIN(ch->mana + number_range(min,max), ch->max_mana);
    	ch->move = UMIN(ch->move + number_range(min,max), ch->max_move);
	update_pos(ch);
    }
    return;
}

void werewolf_regen( CHAR_DATA *ch )
{
    bool mesg = TRUE;
    int regen_hp, regen_max;

    if (IS_NPC(ch)) return;

    if (ch->hit < 1 ) 
    {
//	ch->hit += number_range(1,3);
	ch->hit += 1;
	update_pos(ch);
    }
    else
    {
	if (ch->hit >= ch->max_hit) 
	{
	    mesg = FALSE;
	    if(ch->position != POS_FIGHTING)
	    {
	    ch->move = UMIN(ch->move + (ch->max_move * 0.05), ch->max_move);
	    ch->mana = UMIN(ch->mana + (ch->max_mana * 0.05), ch->max_mana);
	    }
	}
	else if (ch->max_hit > 0)
	{
	    regen_hp = ch->max_hit * 0.05;
	    if (ch->agg < 0) ch->agg = 0; else if (ch->agg > 100) ch->agg = 100;

	    if (ch->agg > 0 && (ch->hit + regen_hp) >= (ch->max_hit * ((100 - ch->agg) * 0.01)))
	    {
		if ((ch->hit + 1) < (ch->max_hit * ((100 - ch->agg) * 0.01)))
		{
		    ch->hit = ch->max_hit * ((100 - ch->agg) * 0.01);
		    send_to_char("Your body has regenerated all non-aggravated damage.\n\r",ch);
		}

		else
		{
		    if (ch->position == POS_FIGHTING)
		    {
			send_to_char("You are unable to heal aggravated wounds while fighting.\n\r",ch);
			return;
		    }
		    if(!IS_VAMPIRE(ch) && ch->position == POS_STANDING) return;
		    ch->agg -= 10;
		    if (ch->agg < 0) ch->agg = 0;
		    if (ch->agg >= 100) ch->hit = 1;
		    else 
		    {
			regen_max = ch->max_hit * ((100 - ch->agg) * 0.01);
			regen_hp = ch->max_hit * 0.05;
			if (regen_hp > regen_max) regen_hp = regen_max;
			ch->hit += regen_hp;
		    }
		}
	    }
	    else ch->hit += ch->max_hit * 0.05;
	}
	if (ch->position != POS_FIGHTING)
	{
	ch->move = UMIN(ch->move + (ch->max_move * 0.05), ch->max_move);

	/* Still not sure if mana should regenerate so fast... */
	ch->mana = UMIN(ch->mana + (ch->max_mana * 0.05), ch->max_mana);
	}

	if ( ch->hit >= ch->max_hit && 
	    ch->mana >= ch->max_mana && ch->move >= ch->max_move )
	{
	    ch->hit = ch->max_hit;
	    ch->mana = ch->max_mana;
	    ch->move = ch->max_move;
	    if ( mesg )
		send_to_char("Your body has completely regenerated itself.\n\r",ch);
	}
    }
    return;
}


void reg_mend( CHAR_DATA *ch )
{
    int ribs = 0;
    int teeth = 0;

    if (IS_BODY(ch,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(ch,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(ch,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(ch,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(ch,BROKEN_RIBS_16)) ribs += 16;
    if (IS_HEAD(ch,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(ch,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(ch,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(ch,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(ch,LOST_TOOTH_16 )) teeth += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(ch,BROKEN_RIBS_1 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(ch,BROKEN_RIBS_2 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(ch,BROKEN_RIBS_4 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(ch,BROKEN_RIBS_8 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(ch,BROKEN_RIBS_16))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",ch,NULL,NULL,TO_ROOM);
	act("One of your ribs snap back into place.",ch,NULL,NULL,TO_CHAR);
    }
    else if (IS_HEAD(ch,LOST_EYE_L))
    {
	act("An eyeball appears in $n's left eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your left eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_L);
    }
    else if (IS_HEAD(ch,LOST_EYE_R))
    {
	act("An eyeball appears in $n's right eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your right eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_R);
    }
    else if (IS_HEAD(ch,LOST_EAR_L))
    {
	act("An ear grows on the left side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the left side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_L);
    }
    else if (IS_HEAD(ch,LOST_EAR_R))
    {
	act("An ear grows on the right side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the right side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_R);
    }
    else if (IS_HEAD(ch,LOST_NOSE))
    {
	act("A nose grows on the front of $n's face.",ch,NULL,NULL,TO_ROOM);
	act("A nose grows on the front of your face.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_NOSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(ch,LOST_TOOTH_1 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(ch,LOST_TOOTH_2 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(ch,LOST_TOOTH_4 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(ch,LOST_TOOTH_8 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(ch,LOST_TOOTH_16))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("A missing tooth grows in your mouth.",ch,NULL,NULL,TO_CHAR);
	act("A missing tooth grows in $n's mouth.",ch,NULL,NULL,TO_ROOM);
    }
    else if (IS_HEAD(ch,BROKEN_NOSE))
    {
	act("$n's nose snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your nose snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(ch,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your jaw snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(ch,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your skull knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(ch,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your spine knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(ch,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your neck snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's left shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your left shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
    }
    else if (IS_ARM_R(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's right shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your right shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
    }
    else if (IS_LEG_L(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's left hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your left hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's right hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your right hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,BROKEN_ARM))
    {
	act("$n's left arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(ch,BROKEN_ARM))
    {
	act("$n's right arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(ch,BROKEN_LEG))
    {
	act("$n's left leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(ch,BROKEN_LEG))
    {
	act("$n's right leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's left wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your left wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's right wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your right wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
    }
    else if (IS_LEG_L(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's left ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your left ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's right ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your right ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,BROKEN_THUMB))
    {
	act("$n's left thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,BROKEN_THUMB))
    {
	act("$n's right thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if ( IS_ARM_R(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if ( IS_ARM_R(ch,BROKEN_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(ch,CUT_THROAT))
    {
    	if (IS_SET(ch->loc_hp[6], BLEEDING_THROAT)) return;
	act("The wound in $n's throat closes up.",ch,NULL,NULL,TO_ROOM);
	act("The wound in your throat closes up.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],CUT_THROAT);
    }
    return;
}
/*
void vamp_rage( CHAR_DATA *ch )
{
    if (IS_NPC(ch)) return;

    send_to_char("You scream with rage as the beast within consumes you!\n\r",ch);
    act("$n screams with rage as $s inner beast consumes $m!.", ch, NULL, NULL, TO_ROOM);
    do_beastlike(ch,"");
    do_rage(ch,"");
    return;
}
*/
void vamp_rage( CHAR_DATA *ch )
{
    if (IS_NPC(ch) || !IS_VAMPIRE(ch)) return;
    send_to_char("You scream with rage as the beast within consumes you!\n\r",ch);
    act("$n screams with rage as $s inner beast consumes $m!.", ch, NULL, NULL, TO_ROOM);
    if (ch->beast > 0) do_beastlike(ch,"");
    if ( ch->pcdata->stats[UNI_RAGE] >= (ch->beast * 0.5) ) return;
    if (!IS_VAMPAFF(ch, VAM_NIGHTSIGHT) && ch->pcdata->powersde[VAM_PROT] > 0)
        do_nightsight(ch,"");
    if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");  
    if (!IS_VAMPAFF(ch, VAM_CLAWS) && ch->pcdata->powersde[VAM_PROT] > 1)
        do_claws(ch,"");
    send_to_char("You bare yours fangs and growl as your inner beast consumes you.\n\r",ch);
    act("$n bares $s fangs and growls as $s inner beast consumes $m.",ch,NULL,NULL,TO_ROOM);
    ch->pcdata->stats[UNI_RAGE] += number_range(10,20);
    WAIT_STATE(ch,12);
   return;
}
void do_humanity( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast > 0
	&& ch->beast < 100 && number_range(1, 500) <= ch->beast)
    {
	if (ch->beast == 1)
	{
	    send_to_char("You have attained Golconda!\n\r", ch);
	    ch->exp += 1000000;
	    if (!IS_IMMUNE(ch, IMM_SUNLIGHT))
		SET_BIT(ch->immune, IMM_SUNLIGHT);
	}
	else
	    send_to_char("You feel slightly more in control of your beast.\n\r", ch);
	ch->beast -= 1;
    }
    return;
}

void do_beastlike( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];
    int blood;

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) &&
!IS_ABOMINATION(ch)  &&
ch->beast < 100 && ch->beast > 0 && number_range(1, 500) <= ch->beast)
    {
	if (ch->beast < 99)
	    send_to_char("You feel your beast take more control over your actions.\n\r", ch);
	else
	    send_to_char("Your beast has fully taken over control of your actions!\n\r", ch);
	ch->beast += 1;
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;
    	if ((IS_VAMPAFF(ch,VAM_PROTEAN) || (IS_VAMPAFF(ch,VAM_OBTENEBRATION)))
		&& !IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (!IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_PROTEAN) &&
		!IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	ch->pcdata->condition[COND_THIRST] = blood;
    }
    return;
}

void do_feed( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    char      bufch [MAX_INPUT_LENGTH];
    char      bufvi [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_NPC(victim))
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }
    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char("Blood does them no good at all.\n\r",ch);
	return;
    }
/* Changed by Toaster since feeding blood does nothing bad yet 
    if (!IS_IMMUNE(victim, IMM_VAMPIRE))
    {
	send_to_char("They refuse to drink your blood.\n\r",ch);
	return;
    }
*/
    if ( ch->pcdata->condition[COND_THIRST] < 20 )
    {
	send_to_char("You don't have enough blood.\n\r",ch);
	return;
    }

    blood = number_range(5,10);
    ch->pcdata->condition[COND_THIRST] -= (blood * 2);
    victim->pcdata->condition[COND_THIRST] += blood;

    if (IS_AFFECTED(ch,AFF_POLYMORPH)) sprintf(bufch,ch->morph);
    else sprintf(bufch,ch->name);
    if (IS_AFFECTED(victim,AFF_POLYMORPH)) sprintf(bufvi,victim->morph);
    else sprintf(bufvi,victim->name);
    sprintf(buf,"You cut open your wrist and feed some blood to %s.",bufvi);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s cuts open $s wrist and feeds some blood to %s.",bufch,bufvi);
    act(buf, ch, NULL, victim, TO_NOTVICT);
    if (victim->position < POS_RESTING)
	send_to_char("You feel some blood poured down your throat.\n\r",victim);
    else
    {
	sprintf(buf,"%s cuts open $s wrist and feeds you some blood.",bufch);
	act(buf, ch, NULL, victim, TO_VICT);
    }
    return;
}

void do_upkeep( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    char      buf2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if(IS_CLASS(ch,CLASS_MAGE))
    {mageupkeep(ch);}
    if(IS_CLASS(ch,CLASS_WEREWOLF))
    {wwupkeep(ch);}
    if(IS_CLASS(ch,CLASS_DROW))
    {drowupkeep(ch);}
    if(IS_CLASS(ch,CLASS_MONK))
    {monkupkeep(ch);}

    if (IS_CLASS(ch, CLASS_DEMON))
    {
divide_to_char(ch);
	send_to_char("{r                              -{R= {wDemonic powers {R={r-{x\n\r",ch);
divide_to_char(ch);


/*	if (ch->pcdata->powersd[DPOWER_FLAGS] < 1)
	    send_to_char(" {wYou have no demonic powers.\n\r",ch);*/
    	if ( IS_DEMPOWER(ch,DEM_FANGS) )
	{
	    if (IS_VAMPAFF(ch, VAM_FANGS)) send_to_char(" {wYou have a pair of long pointed fangs extending from your gums.\n\r",ch);
	    else send_to_char(" {wYou have a pair of long pointed fangs, but they are not currently extended.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_CLAWS) )
	{
	    if (IS_VAMPAFF(ch, VAM_CLAWS)) send_to_char(" {wYou have a pair of razor sharp claws extending from your fingers.\n\r",ch);
	    else send_to_char(" {wYou have a pair of razor sharp claws, but they are not currently extended.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_HORNS) )
	{
	    if (IS_DEMAFF(ch, DEM_HORNS)) send_to_char(" {wYou have a pair of curved horns extending from your forehead.\n\r",ch);
	    else send_to_char(" {wYou have a pair of curved horns, but they are not currently extended.\n\r",ch);
	}
		if ( IS_DEMPOWER(ch,DEM_TAIL) )
	{
	    if (IS_DEMAFF(ch, DEM_TAIL)) send_to_char(" {wYou have a long tail extended from you back side.\n\r",ch);
	    else send_to_char(" {wYou have a tail, but it is not being used.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_HOOVES) )
	{
	    if (IS_DEMAFF(ch, DEM_HOOVES)) send_to_char(" {wYou have hooves instead of feet.\n\r",ch);
	    else send_to_char(" {wYou are able to transform your feet into hooves at will.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_EYES) )
	{
	    if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) send_to_char(" {wYour eyes are glowing bright red, allowing you to see in the dark.\n\r",ch);
	    else send_to_char(" {wYou are able to see in the dark, although that power is not currently activated.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_WINGS) )
	{
	    if (!IS_DEMAFF(ch, DEM_WINGS)) send_to_char(" {wYou have the ability to extend wings from your back.\n\r",ch);
	    else if (IS_DEMAFF(ch, DEM_UNFOLDED)) send_to_char(" {wYou have a pair of large leathery wings unfolded behind your back.\n\r",ch);
	    else send_to_char(" {wYou have a pair of large leathery wings folded behind your back.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_MIGHT) )
	    send_to_char(" {wYour muscles ripple with supernatural strength.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_TOUGH) )
	    send_to_char(" {wYour skin reflects blows with supernatural toughness.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_SPEED) )
	    send_to_char(" {wYou move with supernatural speed and grace.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_TRAVEL) )
	    send_to_char(" {wYou are able to travel to other demons at will.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_SCRY) )
	    send_to_char(" {wYou are able to scry over great distances at will.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_SHADOWSIGHT) )
	{
	    if (IS_AFFECTED(ch, AFF_SHADOWSIGHT)) send_to_char(" {wYou are able see things in the shadowplane.\n\r",ch);
	    else send_to_char(" {wYou are able to view the shadowplane, although you are not currently doing so.\n\r",ch);
	}
    }

    if (IS_CLASS(ch, CLASS_NINJA))
{
divide_to_char(ch);
        send_to_char("{c                              {m*** Ninja Powers ***{x\n\r",ch);
divide_to_char(ch);
   
sprintf(buf,"Your current belt is: %d",ch->pcdata->powerss[BLACK_BELT]);
stc(buf,ch);
}        
    if (IS_CLASS(ch, CLASS_MONK))
    {

divide_to_char(ch);
	send_to_char("{c                              <{C:{w Monk powers {C:{c>{x\n\r",ch);
divide_to_char(ch);

	  if ( ch->pcdata->powers[PMONK] < 1 )
	   send_to_char("You have no knowledge of any Mantras.\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 0 )
	   send_to_char("Eyes of God.\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 1 )
	   send_to_char("Shield/Read Aura/Scry\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 2 )
	   send_to_char("Sacred Invisibility\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 3 )
	   send_to_char("Heart of the Oak\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 4 )
	   send_to_char("Adamantium Hands\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 5 )
	   send_to_char("Steel Shield\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 6 )
	   send_to_char("The Almighty Favor\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 7 )
	   send_to_char("Dark Blaze\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 8 )
	   send_to_char("Celestial Path\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 9 )
	   send_to_char("Prayer of the Ages\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] > 10 )
	   send_to_char("Cloak of Life\n\r", ch);
	  if ( ch->pcdata->powers[PMONK] >= 11 )
	   send_to_char("You have completed your Mantra training.\n\r", ch);
	}
 if (IS_CLASS(ch, CLASS_VAMPIRE))
    {

    divide_to_char(ch);
	send_to_char("                              {R-{r= {wVampire upkeep {r={R-{x\n\r",ch);
    divide_to_char(ch);
	send_to_char("Staying alive...upkeep 1.\n\r",ch);

    	if (IS_VAMPAFF(ch,VAM_DISGUISED) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are disguised as %s, no upkeep.\n\r",ch->morph);
	    else if (ch->beast == 100) sprintf(buf,"You are disguised as %s, upkeep 10-20.\n\r",ch->morph);
	    else sprintf(buf,"You are disguised as %s, upkeep 5-10.\n\r",ch->morph);
	    send_to_char(buf,ch);
	}
    	if (IS_IMMUNE(ch,IMM_SHIELDED) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are shielded, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are shielded, upkeep 2-6.\n\r");
	    else sprintf(buf,"You are shielded, upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are in the shadowplane, upkeep 5-10.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are in the shadowplane, upkeep 10-20.\n\r");
	    else sprintf(buf,"You are in the shadowplane, upkeep 5-10.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_FANGS) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have your fangs out, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have your fangs out, upkeep 2.\n\r");
	    else sprintf(buf,"You have your fangs out, upkeep 1.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_CLAWS) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have your claws out, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have your claws out, upkeep 2-6.\n\r");
	    else sprintf(buf,"You have your claws out, upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have nightsight, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have nightsight, upkeep 2.\n\r");
	    else sprintf(buf,"You have nightsight, upkeep 1.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have shadowsight, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are shadowsight, upkeep 2-6.\n\r");
	    else sprintf(buf,"You are shadowsight, upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_SET(ch->act,PLR_HOLYLIGHT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You have truesight, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have truesight, upkeep 2-10.\n\r");
	    else sprintf(buf,"You have truesight, upkeep 1-5.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_CHANGED) )
	{
	    if      (IS_POLYAFF(ch,POLY_BAT))  sprintf(buf2,"bat" );
	    else if (IS_POLYAFF(ch,POLY_WOLF)) sprintf(buf2,"wolf");
	    else                               sprintf(buf2,"mist");
	    if      (ch->beast == 0  ) sprintf(buf,"You have changed into %s form, no upkeep.\n\r",buf2);
	    else if (ch->beast == 100) sprintf(buf,"You have changed into %s form, upkeep 10-20.\n\r",buf2);
	    else sprintf(buf,"You have changed into %s form, upkeep 5-10.\n\r",buf2);
	    send_to_char(buf,ch);
	}
    	if (IS_POLYAFF(ch,POLY_SERPENT) )
	{
	    if      (ch->beast == 0  ) sprintf(buf,"You are in serpent form, no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are in serpent form, upkeep 6-8.\n\r");
	    else sprintf(buf,"You are in serpent form, upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
       	}
        if (IS_POLYAFF(ch,POLY_NEWZULO))
        {
	if 	(ch->beast == 0  ) sprintf(buf, "You are in Zuloform, upkeep 60-80.\n\r");
	else if (ch->beast == 100) sprintf(buf, "You are in Zuloform, upkeep 60-80.\n\r");
	else sprintf(buf, "You are in Zuloform, upkeep 30-40\n\r");
	send_to_char(buf, ch);
        if (IS_EXTRA(ch,EXTRA_DRAGON))
        {
        send_to_char("You are a huge dragon.\n\r",ch);
        return;
        }
        }
        if (IS_EXTRA(ch, EXTRA_AWE))
        {
        send_to_char("You are awe inspiring.\n\r",ch);
        }
if (IS_AFFECTED(ch, AFF_STEELSHIELD)) send_to_char("Your majesty is up.\n\r",ch);
if (IS_SET(ch->newbits, NEW_MONKFLAME)) send_to_char("Your hands and weapon are engulfed in flames.\n\r",ch);       
if (IS_EXTRA(ch, EXTRA_PLASMA)) send_to_char("You are in plasmaform.\n\r",ch);     
if (ch->pcdata->powersde[VAM_PROT] > 8) send_to_char("Your flesh is as hard as marble.\n\r",ch);
if (ch->pcdata->stats[UNI_RAGE] > 0) send_to_char("You are in frenzy.\n\r",ch);
if (IS_SET(ch->act, PLR_WIZINVIS)) send_to_char("You are blended with the Shade.\n\r",ch);
if (IS_EXTRA(ch, EXTRA_POTENCY)) send_to_char("Your blood is more potent then usual.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_HEAD)) send_to_char("You have the head of a lion.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_TAIL)) send_to_char("A long tail protrudes from your backside.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_EXOSKELETON)) send_to_char("Your body is covered by a bony exoskeleton.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_HORNS)) send_to_char("Large horns protrude from your head.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_WINGS)) send_to_char("Large wings protrude from your back.\n\r",ch);
}
    else
    {
//	send_to_char("Huh?\n\r",ch);
	return;
    }
    divide_to_char(ch);
    return;
}

void do_vclan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->clan) < 2 || IS_SET(ch->special, SPC_ANARCH) )
    {
	send_to_char("But you don't belong to any clan!\n\r",ch);
	return;
    }

    sprintf( buf, "The %s clan:\n\r", ch->clan );
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [Gen] [ Hits  % ] [ Mana  % ] [ Move  % ] [  Exp  ] [Blood]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_VAMPIRE(gch) ) continue;
	if ( !IS_IMMORTAL(gch) ) continue;
	if ( !str_cmp(ch->clan,gch->clan) )
	{
	    sprintf( buf,
	    "[%-16s] [ %d ] [%-6ld%3ld] [%-6ld%3ld] [%-6ld%3ld] [%7d] [ %3d ]\n\r",
		capitalize( gch->name ),
		gch->pcdata->stats[UNI_GEN],
		gch->hit,  (gch->hit  * 100 / gch->max_hit ),
		gch->mana, (gch->mana * 100 / gch->max_mana),
		gch->move, (gch->move * 100 / gch->max_move),
		gch->exp, gch->pcdata->condition[COND_THIRST] );
		send_to_char( buf, ch );
	}
    }
    return;
}

void do_tribe( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char clan[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if ( strlen(ch->clan) > 16)
    {
	send_to_char("You need no tribe, foul spawn of the wyrm.\n\r",ch);
	return;
    }
    send_to_char("[      Name      ] [    Tribe    ] [ Hits  % ] [ Mana  % ] [ Move  % ] [  Exp  ]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_WEREWOLF) ) continue;
        if ( IS_IMMORTAL(gch ) ) continue;
	if ( strlen(gch->clan) > 16) continue;
	if ( gch->clan != NULL && strlen(gch->clan) > 1 )
	    sprintf(clan,gch->clan);
	else if ( gch->pcdata->stats[UNI_GEN] == 1 )
	    sprintf(clan,"All");
	else if ( gch->class == CLASS_ABOMINATION )
            sprintf(clan,"Abomination");
	else
	    sprintf(clan,"None");
	{
	    sprintf( buf,
	    "[%-16s] [%-13s] [%-6ld%3ld] [%-6ld%3ld] [%-6ld%3ld] [%7d]\n\r",
		capitalize( gch->name ), clan,
		gch->hit,  (gch->hit  * 100 / gch->max_hit ),
		gch->mana, (gch->mana * 100 / gch->max_mana),
		gch->move, (gch->move * 100 / gch->max_move),
		gch->exp);
		send_to_char( buf, ch );
	}
    }
    return;
}


void do_werewolf( CHAR_DATA *ch, char *argument )
{
    char       buf[MAX_INPUT_LENGTH];
    char       arg[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_ABOMINATION(ch)) return;
    if (IS_SET(ch->special, SPC_WOLFMAN)) return;
    SET_BIT(ch->special, SPC_WOLFMAN);

    send_to_char("You throw back your head and howl with rage!\n\r",ch);
    act("$n throws back $s head and howls with rage!.", ch, NULL, NULL, TO_ROOM);
    send_to_char("Coarse dark hair sprouts from your body.\n\r",ch);
    act("Coarse dark hair sprouts from $n's body.",ch,NULL,NULL,TO_ROOM);
    if (!IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("Your eyes start glowing a lambent green.\n\r",ch);
	act("$n's eyes start glowing a lambent green.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (!IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("A pair of long fangs extend from your mouth.\n\r",ch);
	act("A pair of long fangs extend from $n's mouth.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Razor sharp talons extend from your fingers.\n\r",ch);
	act("Razor sharp talons extend from $n's fingers.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if ((obj = get_eq_char(ch,WEAR_WIELD)) != NULL && !IS_SET(obj->spectype, SITEM_WOLFWEAPON))
    {
	act("$p drops from your right hand.",ch,obj,NULL,TO_CHAR);
	act("$p drops from $n's right hand.",ch,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    if ((obj = get_eq_char(ch,WEAR_HOLD)) != NULL && !IS_SET(obj->spectype, SITEM_WOLFWEAPON))
    {
	act("$p drops from your left hand.",ch,obj,NULL,TO_CHAR);
	act("$p drops from $n's left hand.",ch,obj,NULL,TO_ROOM);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
      if (IS_ABOMINATION(ch) && IS_POLYAFF(ch, POLY_NEWZULO))
	sprintf(buf,"An enormous black hairy monster" );
  else  
  sprintf(buf,"%s the werewolf",ch->name);
    free_string(ch->morph);
/*
 if (!IS_ABOMINATION(ch))
    {
	ch->blood[BLOOD_CURRENT] *= 2;
	ch->blood[BLOOD_POOL] *= 2;
    }   
*/
 ch->morph = str_dup(buf);
    ch->pcdata->stats[UNI_RAGE] += 75;
    if (ch->gifts[AHROUN]  > 2) ch->pcdata->stats[UNI_RAGE] += 40;
    if (ch->gifts[AHROUN]  > 4) ch->pcdata->stats[UNI_RAGE] += 60;
    if (ch->pcdata->stats[UNI_RAGE] > 300) 
	ch->pcdata->stats[UNI_RAGE] = 300;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( ch == vch )
	{
	    act("You throw back your head and howl with rage!", ch, NULL, NULL, TO_CHAR);
	    continue;
	}
	if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
	    continue;
	if (!IS_NPC(vch))
	{
	    if (vch->in_room == ch->in_room)
		act("$n throws back $s head and howls with rage!", ch, NULL, vch, TO_VICT);
	    else if (vch->in_room->area == ch->in_room->area)
		send_to_char("You hear a fearsome howl close by!\n\r", vch);
	    else
/*		send_to_char("You hear a fearsome howl far off in the distance!\n\r", vch);*/
	    if (!CAN_PK(vch)) continue;
	}
	if ( vch->in_room == ch->in_room && can_see( ch, vch) )
	{
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	}
    }
    return;
}

void do_unwerewolf( CHAR_DATA *ch, char *argument )
{
    char       arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (!IS_SET(ch->special, SPC_WOLFMAN)) return;
    REMOVE_BIT(ch->special, SPC_WOLFMAN);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string(ch->morph);
if (IS_ABOMINATION(ch) && IS_POLYAFF(ch, POLY_NEWZULO))
	ch->morph = str_dup("a huge black monster");  
  ch->morph = str_dup("");
/*
if (!IS_ABOMINATION(ch))
    {
	ch->blood[BLOOD_CURRENT] *= 0.5;
	ch->blood[BLOOD_POOL] *= 0.5;
    }
*/  
  if (IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Your talons slide back into your fingers.\n\r",ch);
	act("$n's talons slide back into $s fingers.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if (IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("Your fangs slide back into your mouth.\n\r",ch);
	act("$n's fangs slide back into $s mouth.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    send_to_char("Your coarse hair shrinks back into your body.\n\r",ch);
    act("$n's coarse hair shrinks back into $s body.",ch,NULL,NULL,TO_ROOM);
    ch->pcdata->stats[UNI_RAGE] = 0;
    if (ch->pcdata->stats[UNI_RAGE] < 0) ch->pcdata->stats[UNI_RAGE] = 0;
    return;
}
/*
void do_favour( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( IS_NPC(ch) ) return;
    if ( !IS_CLASS(ch,CLASS_VAMPIRE) && (IS_SET(ch->special,SPC_PRINCE) || ch->pcdata->stats[UNI_GEN] != 2) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax is: favour <target> <prince/sire>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
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
	send_to_char( "Not on yourself!\n\r", ch );
	return;
    }

    if ( !IS_CLASS(victim,CLASS_VAMPIRE) )
    {
	send_to_char( "But they are not a vampire!\n\r", ch );
	return;
    }

    if ( str_cmp(victim->clan,ch->clan) && str_cmp(arg2,"induct") )
    {
	send_to_char( "You can only grant your favour to someone in your clan.\n\r", ch );
	return;
    }

    if ( ch->pcdata->stats[UNI_GEN] >= victim->pcdata->stats[UNI_GEN] )
    {
	send_to_char( "You can only grant your favour to someone of a lower generation.\n\r", ch );
	return;
    }

    if ( !str_cmp(arg2,"prince") && ch->pcdata->stats[UNI_GEN] == 2)
    {
	if (IS_SET(victim->special,SPC_PRINCE)) {
	act("You remove $N's prince privilages!",ch,NULL,victim,TO_CHAR);
	act("$n removes $N's prince privilages!",ch,NULL,victim,TO_NOTVICT);
	act("$n removes your prince privilages!",ch,NULL,victim,TO_VICT);
	if (IS_SET(victim->special,SPC_SIRE)) REMOVE_BIT(victim->special,SPC_SIRE);
	REMOVE_BIT(victim->special,SPC_PRINCE);return;}
	act("You make $N a prince!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N a prince!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you a prince!",ch,NULL,victim,TO_VICT);
	SET_BIT(victim->special,SPC_PRINCE);
	if (IS_SET(victim->special,SPC_SIRE)) REMOVE_BIT(victim->special,SPC_SIRE);
	return;
    }
    else if ( !str_cmp(arg2,"sire") && (ch->pcdata->stats[UNI_GEN] == 2 || IS_SET(ch->special,SPC_PRINCE)))
    {
	if (IS_SET(victim->special,SPC_SIRE)) {
	act("You remove $N's permission to sire a childe!",ch,NULL,victim,TO_CHAR);
	act("$n has removed $N's permission to sire a childe!",ch,NULL,victim,TO_NOTVICT);
	act("$n has remove your permission to sire a childe!",ch,NULL,victim,TO_VICT);
	REMOVE_BIT(victim->special,SPC_SIRE);return;}
	act("You grant $N permission to sire a childe!",ch,NULL,victim,TO_CHAR);
	act("$n has granted $N permission to sire a childe!",ch,NULL,victim,TO_NOTVICT);
	act("$n has granted you permission to sire a childe!",ch,NULL,victim,TO_VICT);
	SET_BIT(victim->special,SPC_SIRE);
	return;
    }
    else if ( !str_cmp(arg2,"outcast") && victim->pcdata->stats[UNI_GEN] > 2 && 
	ch->pcdata->stats[UNI_GEN] == 2)
    {
	act("You make $N a Caitiff!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N a Caitiff!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you a Caitiff!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( "" );
	return;
    }
    else if ( !str_cmp(arg2,"outcast") && victim->pcdata->stats[UNI_GEN] > 2 && 
	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE))
    {
	act("You make $N a Caitiff!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N a Caitiff!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you a Caitiff!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( "" );
	return;
    }
    else if ( !str_cmp(arg2,"induct") && victim->pcdata->stats[UNI_GEN] > 2 && 
	ch->pcdata->stats[UNI_GEN] == 2 && strlen(victim->clan) < 2)
    {
	if (IS_SET(victim->special, SPC_ANARCH))
	{
	    send_to_char("You cannot induct an Anarch!\n\r",ch);
	    return;
	}
	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
	return;
    }
    else if ( !str_cmp(arg2,"induct") && victim->pcdata->stats[UNI_GEN] > 2 && 
	!IS_SET(victim->special, SPC_PRINCE) && IS_SET(ch->special,SPC_PRINCE) &&
	strlen(victim->clan) < 2)
    {
	if (IS_SET(victim->special, SPC_ANARCH))
	{
	    send_to_char("You cannot induct an Anarch!\n\r",ch);
	    return;
	}
	act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR);
	act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
	return;
    }
    else if ( !str_cmp(arg2,"accept") &&
	(ch->pcdata->stats[UNI_GEN] == 2 || IS_SET(ch->special,SPC_PRINCE)))
    {
	if ( victim->pcdata->rank > AGE_CHILDE)
	{ send_to_char("But they are not a childe!\n\r",ch); return; }
	act("You accept $N into the clan!",ch,NULL,victim,TO_CHAR);
	act("$n has accepted $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
	act("$n accepted you into $s clan!",ch,NULL,victim,TO_VICT);
	victim->pcdata->rank = AGE_NEONATE;
	return;
    }
    else send_to_char( "You are unable to grant that sort of favour.\n\r", ch );
    return;
}
*/

void do_favour( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    
    if ( IS_NPC(ch) ) return;
        
    if ( !IS_VAMPIRE(ch) && !IS_WEREWOLF(ch) && !IS_MONK(ch) && !IS_NINJA(ch) )
    { 
        send_to_char( "Huh?\n\r", ch );
        return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Syntax is: favour <target> <favour>\n\r", ch );
        if ( ch->pcdata->stats[UNI_GEN] == 2 )
        {
            if ( IS_WEREWOLF(ch) )
                send_to_char( "Favours you can grant: Induct Outcast Sire Shaman.\n\r", ch );
            else if ( IS_MONK(ch) )
                send_to_char( "Favours you can grant: Induct Outcast Sire.\n\r", ch );
            else if ( IS_NINJA(ch) )
                send_to_char( "Favours you can grant: Induct Outcast Sire Chunin.\n\r", ch );
            else
                send_to_char( "Favours you can grant: Induct Outcast Sire Prince.\n\r", ch );
        }
        else if (IS_SET(ch->special, SPC_PRINCE))
            send_to_char( "Favours you can grant: Induct Outcast Sire.\n\r", ch );
        else
            send_to_char("None.\n\r",ch);
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
        
    if ( ch == victim )
    {
        send_to_char( "Not on yourself!\n\r", ch );
        return;
    }
     
    if ( ch->class != victim->class )
    {
        send_to_char( "You cannot do that, fool!\n\r", ch );
        return; 
    }
     
    if ( str_cmp( ch->clan, victim->clan )
    &&  str_cmp( arg2, "induct" ) )
    {
        send_to_char( "They already have a clan\n\r", ch );
        return;
    }
        
    if ( !str_cmp(arg2,"sire") )
    {
        if (ch->pcdata->stats[UNI_GEN] != 2 && !IS_SET(ch->special, SPC_PRINCE))
        {
            send_to_char("You are unable to grant this sort of favour.\n\r",ch);
            return;
        }
        if (IS_SET(victim->special, SPC_SIRE))
        {
            if ( IS_WEREWOLF(ch) )
            {
                act("You remove $N's permission to claw a pup!",ch,NULL,victim,TO_CHAR);
                act("$n has removed $N's permission to claw a pup!",ch,NULL,victim,TO_NOTVICT);
                act("$n has remove your permission to claw a pup!",ch,NULL,victim,TO_VICT);
            }
            else if ( IS_MONK(ch) )
            {
                act("You remove $N's permission to guide monks!",ch,NULL,victim,TO_CHAR);
                act("$n has removed $N's permission to guide monks!",ch,NULL,victim,TO_NOTVICT);
                act("$n has remove your permission to guide monks!",ch,NULL,victim,TO_VICT);
            }
            else if ( IS_NINJA(ch) )
            {
                act("You remove $N's permission to discipline ninjas!",ch,NULL,victim,TO_CHAR);
                act("$n has removed $N's permission to discipline ninjas!",ch,NULL,victim,TO_NOTVICT);
                act("$n has remove your permission to discipline ninjas!",ch,NULL,victim,TO_VICT);
            }
	    else
            {
                act("You remove $N's permission to sire a childe!",ch,NULL,victim,TO_CHAR);
                act("$n has removed $N's permission to sire a childe!",ch,NULL,victim,TO_NOTVICT);
                act("$n has remove your permission to sire a childe!",ch,NULL,victim,TO_VICT);
            }
            REMOVE_BIT(victim->special,SPC_SIRE);
            return;
        }
        if ( IS_WEREWOLF(ch) )
        {
            act("You grant $N permission to claw a pup!",ch,NULL,victim,TO_CHAR);
            act("$n has granted $N permission to claw a pup!",ch,NULL,victim,TO_NOTVICT);
            act("$n has granted you permission to claw a pup!",ch,NULL,victim,TO_VICT);
        }
        else if ( IS_MONK(ch) )
        {
            act("You grant $N permission to guide monks!",ch,NULL,victim,TO_CHAR);
            act("$n has granted $N permission to guide monks!",ch,NULL,victim,TO_NOTVICT);
            act("$n has granted you permission to guide monks!",ch,NULL,victim,TO_VICT);
        }
        else if ( IS_NINJA(ch) )
        {
            act("You grant $N permission to discipline ninjas!",ch,NULL,victim,TO_CHAR);
            act("$n has granted $N permission to discipline ninjas!",ch,NULL,victim,TO_NOTVICT);
            act("$n has granted you permission to discipline ninjas!",ch,NULL,victim,TO_VICT);
        }
        else 
        {
            act("You grant $N permission to sire a childe!",ch,NULL,victim,TO_CHAR);
            act("$n has granted $N permission to sire a childe!",ch,NULL,victim,TO_NOTVICT);
            act("$n has granted you permission to sire a childe!",ch,NULL,victim,TO_VICT);
        }
        SET_BIT(victim->special,SPC_SIRE);
        return;
    }
    else if ( !str_cmp(arg2,"prince") && IS_VAMPIRE(ch) )
    {
        if (ch->pcdata->stats[UNI_GEN] != 2)
        {
            send_to_char("You are unable to grant this sort of favour.\n\r",ch);
            return;
        }
        if (IS_SET(victim->special, SPC_PRINCE))
       { 
            act("You remove $N's Prince status!",ch,NULL,victim,TO_CHAR);
            act("$n has removed $N's Prince status!",ch,NULL,victim,TO_NOTVICT);
            act("$n has removed your Prince status!",ch,NULL,victim,TO_VICT);
            REMOVE_BIT(victim->special,SPC_PRINCE);
            return;
        }
        act("You grant $N Prince status!",ch,NULL,victim,TO_CHAR);
        act("$n has granted $N Prince status!",ch,NULL,victim,TO_NOTVICT);
        act("$n has granted you Prince status!",ch,NULL,victim,TO_VICT);
        SET_BIT(victim->special,SPC_PRINCE);
        return;
    }
    else if ( !str_cmp(arg2,"shaman") && IS_WEREWOLF(ch) )
    {
        if (ch->pcdata->stats[UNI_GEN] != 2)
       {
           send_to_char("You are unable to grant this sort of favour.\n\r",ch);
            return;
        }
        if (IS_SET(victim->special,SPC_PRINCE))
        {
            act("You remove $N's Shaman status!",ch,NULL,victim,TO_CHAR);
            act("$n has removed $N's Shaman status!",ch,NULL,victim,TO_NOTVICT);
            act("$n has removed your Shaman status!",ch,NULL,victim,TO_VICT);
            REMOVE_BIT(victim->special,SPC_PRINCE);
            return;
        }
        act("You grant $N Shaman status!",ch,NULL,victim,TO_CHAR);
        act("$n has granted $N Shaman status!",ch,NULL,victim,TO_NOTVICT);
        act("$n has granted you Shaman status!",ch,NULL,victim,TO_VICT);
        SET_BIT(victim->special,SPC_PRINCE);
       return;
    }
    else if ( !str_cmp(arg2,"chunin") && IS_NINJA(ch) )
    {
        if (ch->pcdata->stats[UNI_GEN] != 2)
       {
           send_to_char("You are unable to grant this sort of favour.\n\r",ch);
            return;
        }
        if (IS_SET(victim->special,SPC_PRINCE))
        {
            act("You remove $N's Chunin status!",ch,NULL,victim,TO_CHAR);
            act("$n has removed $N's Chunin status!",ch,NULL,victim,TO_NOTVICT);
            act("$n has removed your Chunin status!",ch,NULL,victim,TO_VICT);
            REMOVE_BIT(victim->special,SPC_PRINCE);
            return;
        }
        act("You grant $N Chunin status!",ch,NULL,victim,TO_CHAR);
        act("$n has granted $N Chunin status!",ch,NULL,victim,TO_NOTVICT);
        act("$n has granted you Chunin status!",ch,NULL,victim,TO_VICT);
        SET_BIT(victim->special,SPC_PRINCE);
       return;
    }
    else if ( !str_cmp(arg2,"induct") )
    {
        if (ch->pcdata->stats[UNI_GEN] != 2 && !IS_SET(ch->special,SPC_PRINCE))
        {
            send_to_char("You are unable to grant this sort of favour.\n\r",ch);
            return;
        }
        if (!IS_IMMUNE(victim,IMM_VAMPIRE))
        {
            send_to_char( "You cannot induct an unwilling person.\n\r", ch );
            return;
        }
        if (ch->pcdata->stats[UNI_GEN] >= victim->pcdata->stats[UNI_GEN])
        {
            if ( IS_WEREWOLF(ch) )
                send_to_char("You can only induct those of weaker heritage than yourself.\n\r",ch);
            else
                send_to_char("You can only induct those of higher generation than yourself.\n\r",ch);
            return;
        }
        if ( IS_WEREWOLF(ch) )
        {
            act("You induct $N into your tribe!",ch,NULL,victim,TO_CHAR);
            act("$n inducts $N into $s tribe!",ch,NULL,victim,TO_NOTVICT);
            act("$n inducts you into $s tribe!",ch,NULL,victim,TO_VICT);
        }
        else if ( IS_MONK(ch) )
        {
            act("You induct $N into your temple!",ch,NULL,victim,TO_CHAR);
            act("$n inducts $N into $s temple!",ch,NULL,victim,TO_NOTVICT);
            act("$n inducts you into $s temple!",ch,NULL,victim,TO_VICT);
        }
        else if ( IS_NINJA(ch) )
        {
            act("You induct $N into your family!",ch,NULL,victim,TO_CHAR);
            act("$n inducts $N into $s family!",ch,NULL,victim,TO_NOTVICT);
            act("$n inducts you into $s family!",ch,NULL,victim,TO_VICT);
        }
        else
        {
            act("You induct $N into your clan!",ch,NULL,victim,TO_CHAR); 
            act("$n inducts $N into $s clan!",ch,NULL,victim,TO_NOTVICT);
            act("$n inducts you into $s clan!",ch,NULL,victim,TO_VICT);
        }
        free_string(victim->clan);
        victim->clan = str_dup( ch->clan );
        return;
    }
    else if ( !str_cmp(arg2,"outcast") )   
    {
        if (ch->pcdata->stats[UNI_GEN] != 2 && !IS_SET(ch->special, SPC_PRINCE))
        {
            send_to_char("You are unable to grant this sort of favour.\n\r",ch);
            return;
        }   
        if (IS_SET(victim->special, SPC_PRINCE) && ch->pcdata->stats[UNI_GEN] != 2)
        {
            if ( IS_WEREWOLF(ch) )
               send_to_char( "You cannot outcast another Shaman.\n\r", ch );
            else if ( IS_NINJA(ch) )
               send_to_char( "You cannot outcast another Chunin.\n\r", ch );
            else
                send_to_char( "You cannot outcast another Prince.\n\r", ch );
            return;
        }
        if (ch->pcdata->stats[UNI_GEN] >= victim->pcdata->stats[UNI_GEN])
        {
            if ( IS_WEREWOLF(ch) )
                send_to_char("You can only outcast those of weaker heritage than yourself.\n\r",ch);
            else
                send_to_char("You can only outcast those of higher generation than yourself.\n\r",ch);
            return;
        }   
        if ( IS_WEREWOLF(ch) )
        {
            act("You outcast $N from your tribe!",ch,NULL,victim,TO_CHAR);
            act("$n outcasts $N from $s tribe!",ch,NULL,victim,TO_NOTVICT);  
            act("$n outcasts you from $s tribe!",ch,NULL,victim,TO_VICT);
        }
        else if ( IS_MONK(ch) )
        {
            act("You outcast $N from your temple!",ch,NULL,victim,TO_CHAR);
            act("$n outcasts $N from $s temple!",ch,NULL,victim,TO_NOTVICT);  
            act("$n outcasts you from $s temple!",ch,NULL,victim,TO_VICT);
        }
        else if ( IS_NINJA(ch) )
        {
            act("You outcast $N from your family!",ch,NULL,victim,TO_CHAR);
            act("$n outcasts $N from $s family!",ch,NULL,victim,TO_NOTVICT);  
            act("$n outcasts you from $s family!",ch,NULL,victim,TO_VICT);
        }
        else
        {
            act("You outcast $N from your clan!",ch,NULL,victim,TO_CHAR);
            act("$n outcasts $N from $s clan!",ch,NULL,victim,TO_NOTVICT);
            act("$n outcasts you from $s clan!",ch,NULL,victim,TO_VICT);
        }
        free_string(victim->clan);
        victim->clan = str_dup( "" );
        REMOVE_BIT(victim->special,SPC_SIRE);
        REMOVE_BIT(victim->special,SPC_PRINCE);
        return;
    }
    else
        send_to_char( "You are unable to grant that sort of favour.\n\r", ch );
    return;
}
void do_ghoul( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
/*
	send_to_char("This ability has been temporarily disabled.\n\r",ch);
	return;
*/
    if (!IS_VAMPAFF(ch,VAM_DOMINATE) )
    {
	send_to_char("You are not trained in the Dominate discipline.\n\r",ch);
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
	sprintf(buf,"You break your hold over %s.\n\r",familiar->short_descr);
	send_to_char( buf, ch );
	familiar->wizard = NULL;
	ch->pcdata->familiar = NULL;
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your ghoul?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own ghoul?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to make them a ghoul.\n\r", ch );
	return;
    }

    if (victim->level > (ch->spl[RED_MAGIC] * 0.25))
    {
	send_to_char( "They are too powerful.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= 50;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You cut open your wrist and feed $N some blood.",ch,NULL,victim,TO_CHAR);
    act("$n cuts open $s wrist and feeds you some blood.",ch,NULL,victim,TO_VICT);
    act("$n cuts open $s wrist and feeds $N some blood.",ch,NULL,victim,TO_NOTVICT);
    return;
}
/*
void do_familiar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("Ok.\n\r",ch);

    return;
}
*/
void do_fcommand( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && ch->level < LEVEL_APPRENTICE)
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
/*
    if (IS_CLASS(ch, CLASS_VAMPIRE) && !IS_VAMPAFF(ch,VAM_DOMINATE) )
    {
	send_to_char("You are not trained in the Dominate discipline.\n\r",ch);
	return;
    }
*/
    if ( ( victim = ch->pcdata->familiar ) == NULL )
    {
	send_to_char( "But you don't have a familiar!\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar do?\n\r", ch );
	return;
    }
    interpret(victim,argument);
    return;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ABOMINATION(ch))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->gifts[RAGABASH] < 1) 
    {
	send_to_char("You must obtain level one Ragabash to use Blur of the Milky eye.\n\r",ch);
	return;
    }

/* Changed by Kyrlin to fix the message and to fix ww bug 24/01/00 */
    else if (( IS_CLASS(ch, CLASS_VAMPIRE)) && ch->pcdata->powersde[VAM_OBFU] < 1)
    {
	send_to_char("You must obtain level one in Obfuscate first.\n\r",ch);
	return;
}

// changed for WW gift 'Blur of the Milky eye'
    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
if (IS_WEREWOLF(ch))
    {
        send_to_char("You stop distorting the perception of others.\n\r",ch);
        act("You notice $n looks as if $e has just stopped concentrating on something.", ch, ch, NULL,TO_ROOM);
    }
else if (!IS_WEREWOLF(ch))
{
        send_to_char("You slowly fade into existence.\n\r",ch);
        act("$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM);
}
        REMOVE_BIT(ch->act, PLR_WIZINVIS);
        return;
    }
  if (IS_WEREWOLF(ch))
    {
    send_to_char("You concentrate on blurring the perception of those around you.\n\r", ch);
    act("$n's form seems to blur for a moment.", ch, NULL, NULL, TO_ROOM);
    }
else if (!IS_WEREWOLF(ch))
{
   send_to_char("You slowly fade out of existence.\n\r",ch);
    act("$n slowly fades out of existence.", ch, NULL, NULL, TO_ROOM);
}
        SET_BIT(ch->act, PLR_WIZINVIS);
    return;
}

void do_flex( CHAR_DATA *ch, char *argument )
{
if (IS_WEREWOLF(ch))
{
stc("Huh?\n\r",ch);
return;
}
    if(number_range(1,5) == 5)
    {
    act("You flex your bulging muscles.",ch,NULL,NULL,TO_CHAR);
    act("$n flexes $s bulging muscles.",ch,NULL,NULL,TO_ROOM);
    if (IS_NPC(ch)) return;

    if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
	act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
    {
        act("The earth surrounding $n starts to crack.",ch,NULL,NULL,TO_ROOM);
	stc("The earth surrounding you starts to crack.\n\r",ch);
	REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
	SET_BIT(ch->mflags, MAGE_EMBRACED1);
    }
    if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
    {
        act("The earth surrounding $n crumbles to dust.",ch,NULL,NULL,TO_ROOM);
	stc("The earth surrounding you crumbles to dust.\n\r",ch);
	REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
    }
    WAIT_STATE(ch,24);
    }
    else 
    {
	send_to_char("You have failed.\n\r",ch);
	WAIT_STATE(ch,12);
    }
    return;
}

void do_rage( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( !is_garou(ch) ) return;
/* Added Toaster 15/04/00 so werewolf imms rage instantly */
    if(IS_IMMORTAL(ch)) 
    {
    send_to_char("You start snarling angrilly and rage instantly.\n\r",ch);
    ch->pcdata->stats[UNI_RAGE] += 150;
    do_werewolf(ch,"");
    return;
    }

    if (!IS_SET(ch->special, SPC_WOLFMAN)) // || IS_ABOMINATION(ch))
    {
	send_to_char("You start snarling angrilly.\n\r",ch);
	act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM);
	ch->pcdata->stats[UNI_RAGE] += number_range(10,20);
       if ( (( get_age(ch) - 17) * 2 ) > 200 ) ch->pcdata->stats[UNI_RAGE] += 45;
	if (ch->pcdata->stats[UNI_RAGE] >= 100) do_werewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are already in a rage!\n\r",ch);
    return;
}

void do_calm( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast < 1)
    {
	if ( ch->pcdata->stats[UNI_RAGE] < 1 )
	{
	    send_to_char("Your beast doesn't control your actions.\n\r",ch);
	    return;
	}
	send_to_char("You take a deep breath and force back your inner beast.\n\r",ch);
	act("$n takes a deep breath and forces back $s inner beast.",ch,NULL,NULL,TO_ROOM);
	ch->pcdata->stats[UNI_RAGE] = 0;
	if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) do_nightsight(ch,"");
	if (IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	if (IS_VAMPAFF(ch, VAM_CLAWS)) do_claws(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
    {
        send_to_char("You control you actions and calm down.\n\r", ch);
        ch->pcdata->stats[UNI_RAGE] = 0;
        return;
    }
    else if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] == 0)
    {
    send_to_char("You are not in the state of michi.\n\r", ch);
    return;
    }
    
    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You take a deep breath and calm yourself.\n\r",ch);
	act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
	ch->pcdata->stats[UNI_RAGE] -= number_range(10,20);
        if ( (( get_age(ch) - 17) * 2 ) > 200 ) ch->pcdata->stats[UNI_RAGE] -= 45;
	if(IS_IMMORTAL(ch)) ch->pcdata->stats[UNI_RAGE] = 0;
	if (ch->pcdata->stats[UNI_RAGE] < 100) do_unwerewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are not in crinos form!\n\r",ch);
    return;
}

void do_totems( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Totems: Bear (%d), Tiger (%d), Lynx (%d), Gaia (%d), Boar (%d), Owl (%d),\n\r Spider (%d), Wolf (%d), Hawk (%d), Mantis (%d).\n\r",
	ch->pcdata->powers[WPOWER_BEAR], ch->pcdata->powers[WPOWER_TIGER],
	ch->pcdata->powers[WPOWER_LYNX], ch->pcdata->powers[WPOWER_GAIA],
	ch->pcdata->powers[WPOWER_BOAR], ch->pcdata->powers[WPOWER_OWL],
	ch->pcdata->powers[WPOWER_SPIDER], ch->pcdata->powers[WPOWER_WOLF],
	ch->pcdata->powers[WPOWER_HAWK],ch->pcdata->powers[WPOWER_MANTIS]);
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"bear"))
	{
	    send_to_char("Bear: The totem of strength and aggression.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BEAR] < 1) 
		send_to_char("You have none of the Bear totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BEAR] > 0) 
		send_to_char("FLEX: You strength is so great that no ropes can hold you.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BEAR] > 1) 
		send_to_char("RAGE: You are able to build yourself up a rage at will.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BEAR] > 2) 
		send_to_char("Steel claws: Your claws are so tough that they can parry weapons.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BEAR] > 3) 
		send_to_char("Hibernation: Your wounds heal at amazing speeds when you sleep.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"tiger"))
	{
	    send_to_char("Tiger: The totem of speed and agility.\n\r",ch);
        
            if (ch->pcdata->powers[WPOWER_TIGER] < 1)
	        send_to_char("You have none of the Tiger totem powers.\n\r",ch);            if (ch->pcdata->powers[WPOWER_TIGER] > 0)
		send_to_char("Speed: You parry at unbeliable speeds.\n\r",ch);
            if (ch->pcdata->powers[WPOWER_TIGER] > 1)
                send_to_char("Agility: You are abale to dodge attacks with amazing speeds.\n\r",ch);
                if (ch->pcdata->powers[WPOWER_TIGER] > 2)
                    send_to_char("Talons: You get razor sharp talons for an extra attack.\n\r",ch);
		if (ch->pcdata->powers[WPOWER_TIGER] > 3)
		    send_to_char("Fangs: you get a huge pair of fangs.\n\r",ch);
	        return;        

	}
	else if (!str_cmp(arg1,"boar"))
	{
	    send_to_char("Boar: The totem of toughness and perserverance.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BOAR] < 1) 
		send_to_char("You have none of the Boar totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BOAR] > 0) 
		send_to_char("Shatter: No door is sturdy enough to resist you.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BOAR] > 1) 
		send_to_char("CHARGE: Your first blow in combat has a +50 damage bonus.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BOAR] > 2) 
		send_to_char("Toughness: Your skin is extremely tough. You take half damage in combat.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_BOAR] > 3) 
		send_to_char("Immovability: You are able to shrug off blows that would knock out most people.\n\r",ch);
	    return;
	}
 	else if (!str_cmp(arg1, "gaia"))
 	{
 		send_to_char("Gaia: The totem of gifts and power.\n\r",ch);
 		if (ch->pcdata->powers[WPOWER_GAIA] < 1) 
		send_to_char("You have none of the Gaia totem powers.\n\r",ch);
		if (ch->pcdata->powers[WPOWER_GAIA] > 0) 
		send_to_char("Protection: By typing gaia you cannot be attacked by any players.\n\r",ch);
		if (ch->pcdata->powers[WPOWER_GAIA] > 1) 
		send_to_char("Damcap: Gaia has gifted you with 500 more damcap.\n\r",ch);
		if (ch->pcdata->powers[WPOWER_GAIA] > 2) 
		send_to_char("Burrow: You can burrow under the ground to any other WereWolf.\n\r",ch);
	    return;
	}
 else if (!str_cmp(arg1,"lynx"))
	{
	    send_to_char("Lynx: The totem of speed and agility.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_LYNX] < 1) 
		send_to_char("You have none of the Lynx totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_LYNX] > 0) 
		send_to_char("Light footed: You move so lightly that you leave no tracks behind you.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_LYNX] > 1) 
		send_to_char("Stalker: You are able hunt people with much greater speed than normal.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_LYNX] > 2) 
		send_to_char("Combat speed: You have an extra attack in combat.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_LYNX] > 3) 
		send_to_char("Lightning Claws: Yours claws parry blows with lightning fast speed.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Owl"))
	{
	    send_to_char("Owl: The totem of thought and spiritualism.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_OWL] < 1) 
		send_to_char("You have none of the Owl totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_OWL] > 0) 
		send_to_char("VANISH: You are able to conceal yourself from all but the most perceptive.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_OWL] > 1) 
		send_to_char("SHIELD: You are able to shield your mind from scrying and aura-reading.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_OWL] > 2) 
		send_to_char("SHADOWPLANE: You are able to enter the shadow plane.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_OWL] > 3) 
		send_to_char("Magical Control: You are able to fully control your magic in crinos form.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Spider"))
	{
	    send_to_char("Spider: The totem of ambush and cunning.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_SPIDER] < 1) 
		send_to_char("You have none of the Spider totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_SPIDER] > 0) 
		send_to_char("Poisonous bite: Your bite injects your opponents with a deadly venom.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_SPIDER] > 1) 
		send_to_char("WEB: You are able to shoot a web at your opponents to entrap them.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_SPIDER] > 2) 
		send_to_char("Immunity to poison: Poisons have no affect upon you.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Wolf"))
	{
	    send_to_char("Wolf: Controlling your innate wolf powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_WOLF] < 1) 
		send_to_char("You have none of the Wolf totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_WOLF] > 0) 
		send_to_char("CLAWS: You can extend or retract your claws at will.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_WOLF] > 1) 
		send_to_char("FANGS: You can extend or retract your fangs at will.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_WOLF] > 2) 
		send_to_char("CALM: You are able to repress your inner beast at will.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_WOLF] > 3) 
		send_to_char("Spirit of Fenris: You are able to enter rage faster than normal.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Hawk"))
	{
	    send_to_char("Hawk: The totem of vision and perception.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_HAWK] < 1) 
		send_to_char("You have none of the Wolf totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_HAWK] > 0) 
		send_to_char("NIGHTSIGHT: You can see perfectly well in the dark.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_HAWK] > 1) 
		send_to_char("SHADOWSIGHT: You can see into the plane of shadows.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_HAWK] > 2) 
		send_to_char("TRUESIGHT: You have perfect vision.\n\r",ch);
	    return;
	}
	else if (!str_cmp(arg1,"Mantis"))
	{
	    send_to_char("Mantis: The totem of dexterity and reflexes.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_MANTIS] < 1) 
		send_to_char("You have none of the Mantis totem powers.\n\r",ch);
	    if (ch->pcdata->powers[WPOWER_MANTIS] > 3) 
		send_to_char("Incredibly fast attacks: Your opponents get -20 to parry and -40 to dodge.\n\r",ch);
	    else if (ch->pcdata->powers[WPOWER_MANTIS] > 2) 
		send_to_char("Extremely fast attacks: Your opponents get -15 to parry and -30 to dodge.\n\r",ch);
	    else if (ch->pcdata->powers[WPOWER_MANTIS] > 1) 
		send_to_char("Very fast attacks: Your opponents get -10 to parry and -20 to dodge.\n\r",ch);
	    else if (ch->pcdata->powers[WPOWER_MANTIS] > 0) 
		send_to_char("Fast attacks: Your opponents get -5 to parry and -10 to dodge.\n\r",ch);
	    return;
	}
	sprintf(buf,"Totems: Bear (%d), Lynx (%d), Boar (%d), Owl (%d), Spider (%d), Wolf (%d),\n\r        Hawk (%d), Mantis (%d).\n\r",
	ch->pcdata->powers[WPOWER_BEAR], ch->pcdata->powers[WPOWER_LYNX],
	ch->pcdata->powers[WPOWER_BOAR], ch->pcdata->powers[WPOWER_OWL],
	ch->pcdata->powers[WPOWER_SPIDER], ch->pcdata->powers[WPOWER_WOLF],
	ch->pcdata->powers[WPOWER_HAWK],ch->pcdata->powers[WPOWER_MANTIS]);
	send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"bear"   )) {improve = WPOWER_BEAR;   max=4;}
	else if (!str_cmp(arg1,"tiger"  )) {improve = WPOWER_TIGER;  max=4;}
	else if (!str_cmp(arg1,"boar"   )) {improve = WPOWER_BOAR;   max=4;}
	else if (!str_cmp(arg1,"gaia"   )) {improve = WPOWER_GAIA;   max=3;}
	else if (!str_cmp(arg1,"lynx"   )) {improve = WPOWER_LYNX;   max=4;}
	else if (!str_cmp(arg1,"owl"    )) {improve = WPOWER_OWL;    max=4;}
	else if (!str_cmp(arg1,"spider" )) {improve = WPOWER_SPIDER; max=3;}
	else if (!str_cmp(arg1,"wolf"   )) {improve = WPOWER_WOLF;   max=4;}
	else if (!str_cmp(arg1,"hawk"   )) {improve = WPOWER_HAWK;   max=3;}
	else if (!str_cmp(arg1,"mantis" )) {improve = WPOWER_MANTIS; max=4;}
	else
	{
	    send_to_char("You can improve: Bear, Tiger, Boar, Gaia, Lynx, Owl, Spider, Wolf, Hawk or Mantis.\n\r",ch);
	    return;
	}
	cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the powers of the %s totem.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your %s totem.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You improve your ability in the %s totem.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a totem, type: Totem <totem type> improve.\n\r",ch);
    return;
}

void do_web( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DROW) &&
!IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powersd[1],DPOWER_WEB))
	{send_to_char("You don't have that power yet.\n\r", ch );
	return;}

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot web yourself.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You cannot web an ethereal person.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

void do_birth( CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_EXTRA(ch, EXTRA_PREGNANT))
    {
	send_to_char("But you are not even pregnant!\n\r",ch);
	return;
    }

    if (!IS_EXTRA(ch, EXTRA_LABOUR))
    {
	send_to_char("You're not ready to give birth yet.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
	if (ch->pcdata->genes[4] == SEX_MALE)
	    send_to_char( "What do you wish to name your little boy?\n\r", ch );
	else if (ch->pcdata->genes[4] == SEX_FEMALE)
	    send_to_char( "What do you wish to name your little girl?\n\r", ch );
	else
	    send_to_char( "What do you wish to name your child?\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( char_exists(FALSE,argument) )
    {
	send_to_char( "That player already exists.\n\r", ch );
	return;
    }

    strcpy(buf2,ch->pcdata->cparents);
    strcat(buf2," ");
    strcat(buf2,argument);
    if (!birth_ok(ch, buf2))
    {
	send_to_char( "Bug - please inform KaVir.\n\r", ch );
	return;
    }
    argument[0] = UPPER(argument[0]);
    birth_write( ch, argument );
    ch->pcdata->genes[9] += 1;
    REMOVE_BIT(ch->extra, EXTRA_PREGNANT);
    REMOVE_BIT(ch->extra, EXTRA_LABOUR);
    save_char_obj(ch);
    return;
}

bool birth_ok( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
    char mum [MAX_INPUT_LENGTH];
    char dad [MAX_INPUT_LENGTH];
    char child [MAX_INPUT_LENGTH];

    argument = one_argument( argument, mum );
    argument = one_argument( argument, dad );
    argument = one_argument( argument, child );

    if (dad[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    if (child[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    dad[0] = UPPER(dad[0]);
    if (!str_cmp(dad,"Kavir")) strcpy(dad,"KaVir");
    child[0] = UPPER(child[0]);
    if (ch->pcdata->genes[4] == SEX_MALE)
    {
	send_to_char("You give birth to a little boy!\n\r",ch);
	sprintf(buf,"%s has given birth to %s's son, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    else if (ch->pcdata->genes[4] == SEX_FEMALE)
    {
	send_to_char("You give birth to a little girl!\n\r",ch);
	sprintf(buf,"%s has given birth to %s's daughter, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    return FALSE;
}


bool char_exists( bool backup, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    bool found = FALSE;

    fclose( fpReserve );
    if (backup)
    	sprintf( buf, "%sbackup/%s", PLAYER_DIR, capitalize( argument ) );
    else
    	sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( buf, "r" ) ) != NULL )
    {
	found = TRUE;
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}

void birth_write( CHAR_DATA *ch, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    char *strtime;

    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';

    sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen(buf, "w") ) != NULL) 
    {  
	fprintf( fp, "#PLAYER\n");
	fprintf( fp, "Name         %s~\n",capitalize( argument ));
	fprintf( fp, "ShortDescr   ~\n"				);
	fprintf( fp, "LongDescr    ~\n"				);
	fprintf( fp, "Description  ~\n"				);
	fprintf( fp, "Lord         ~\n"				);
	fprintf( fp, "Clan         ~\n"				);
	fprintf( fp, "Morph        ~\n"				);
	fprintf( fp, "Createtime   %s~\n",str_dup( strtime )	);
 	fprintf( fp, "Lasttime     ~\n"				);
	fprintf( fp, "Lasthost     ~\n"				);
	fprintf( fp, "Poweraction  ~\n"				);
	fprintf( fp, "Powertype    ~\n"				);
	fprintf( fp, "Prompt       ~\n"				);
	fprintf( fp, "Cprompt      ~\n"				);
	fprintf( fp, "Sex          %d\n",ch->pcdata->genes[4]	);
	fprintf( fp, "Race         0\n"				);
	fprintf( fp, "Immune       %d\n",ch->pcdata->genes[3]	);
	fprintf( fp, "Polyaff      0\n"				);
	fprintf( fp, "Itemaffect   0\n"				);
	fprintf( fp, "Vampaff      0\n"				);
	fprintf( fp, "Vamppass     0\n"				);
	fprintf( fp, "Form         32767\n"			);
	fprintf( fp, "Beast        15\n"			);
	fprintf( fp, "Vampgen      -1\n"			);
	fprintf( fp, "Spectype     0\n"				);
	fprintf( fp, "Specpower    0\n"				);
	fprintf( fp, "Home         3001\n"			);
	fprintf( fp, "Level        2\n"				);
	fprintf( fp, "Trust        0\n"				);
	fprintf( fp, "Played       0\n"				);
	fprintf( fp, "Room         %d\n",ch->in_room->vnum 	);
	fprintf( fp, "PkPdMkMd     0 0 0 0\n"			);
	fprintf( fp, "Weapons      0 0 0 0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Spells       4 4 4 4 4\n"			);
	fprintf( fp, "Combat       0 0 0 0 0 0 0 0\n"		);
	fprintf( fp, "Stance       0 0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Locationhp   0 0 0 0 0 0 0\n"		);
	fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n",
	    ch->pcdata->genes[0], ch->pcdata->genes[0], ch->pcdata->genes[1], 
	    ch->pcdata->genes[1], ch->pcdata->genes[2], ch->pcdata->genes[2]);
	fprintf( fp, "Gold         0\n"				);
	fprintf( fp, "Exp          0\n"				);
	fprintf( fp, "Act          1600\n"   			);
	fprintf( fp, "Extra        32768\n"   			);
	fprintf( fp, "AffectedBy   0\n"				);
	fprintf( fp, "Position     7\n"				);
	fprintf( fp, "Practice     0\n"				);
	fprintf( fp, "SavingThrow  0\n"				);
	fprintf( fp, "Alignment    0\n"				);
	fprintf( fp, "Hitroll      0\n"				);
	fprintf( fp, "Damroll      0\n"				);
	fprintf( fp, "Armor        100\n"			);
	fprintf( fp, "Wimpy        0\n"				);
	fprintf( fp, "Deaf         0\n"				);
	fprintf( fp, "Password     %s~\n",ch->pcdata->pwd	);
	fprintf( fp, "Bamfin       ~\n"				);
	fprintf( fp, "Bamfout      ~\n"				);
	fprintf( fp, "Title         the mortal~\n"		);
	fprintf( fp, "Conception   ~\n"				);
	fprintf( fp, "Parents      %s~\n",ch->pcdata->cparents	);
	fprintf( fp, "Cparents     ~\n"				);
	fprintf( fp, "AttrPerm     %d %d %d %d %d\n",
	    ch->pcdata->perm_str,
	    ch->pcdata->perm_int,
	    ch->pcdata->perm_wis,
	    ch->pcdata->perm_dex,
	    ch->pcdata->perm_con );
	fprintf( fp, "AttrMod      0 0 0 0 0\n"			);
	fprintf( fp, "Quest        0\n"				);
	fprintf( fp, "Wolf         0\n"				);
	fprintf( fp, "Rank         0\n"				);
	fprintf( fp, "Stage        0 0 0\n"			);
	fprintf( fp, "Wolfform     0 0\n"			);
	fprintf( fp, "Runes        0 0 0 0\n"			);
	fprintf( fp, "Disc         0 0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Genes        0 0 0 0 0 0 0 0 0 0\n"	);
	fprintf( fp, "Power        0 0\n"			);
	fprintf( fp, "FakeCon      0 0 0 0 0 0 0 0\n"		);
	fprintf( fp, "Condition    0 48 48\n"			);
	fprintf( fp, "End\n\n" );
	fprintf( fp, "#END\n" );
	fclose( fp );
    }
    return;
}

void do_teach( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MAGE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->level == LEVEL_APPRENTICE)
    {
	send_to_char("You don't know enough to teach another.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Teach whom?\n\r", ch );
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

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot teach yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_MAGE))
    {
	send_to_char( "They are already a mage.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only teach avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to teach vampires!\n\r", ch );
	return;
    }
    
    if ( victim->class != 0 )
    {
      send_to_char( "They are already classed.\n\r", ch );
      return;
    }

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot teach an unwilling person.\n\r", ch );
	return;
    }
    if(( victim->max_hit < 1500) )
        {
        send_to_char("They do not have 1500 hitpoints.\n\r", ch);
        return;
        }

    if (ch->exp < 100000)
    {
	send_to_char("You cannot afford the 100000 exp required to teach them.\n\r",ch);
	return;
    }

    if (victim->exp < 100000)
    {
	send_to_char("They cannot afford the 100000 exp required to learn from you.\n\r",ch);
	return;
    }

    ch->exp -= 100000;
    victim->exp -= 100000;

    act("You teach $N the basics of magic.", ch, NULL, victim, TO_CHAR);
    act("$n teaches $N the basics of magic.", ch, NULL, victim, TO_NOTVICT);
    act("$n teaches you the basics of magic.", ch, NULL, victim, TO_VICT);
    victim->level = LEVEL_APPRENTICE;
    victim->trust = LEVEL_APPRENTICE;
    send_to_char( "You are now an apprentice.\n\r", victim );
    victim->pcdata->stats[UNI_GEN] = ch->pcdata->stats[UNI_GEN] + 1;
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->pcdata->powersm[MPOWER_RUNE0] = ch->pcdata->powersm[MPOWER_RUNE0];
    victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
    victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
    victim->pcdata->powersm[MPOWER_RUNE3] = 15;
    victim->class    = CLASS_MAGE;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}


void do_gaia( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
  
  	if (!IS_CLASS(ch, CLASS_WEREWOLF) )
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( ch->pcdata->powers[WPOWER_GAIA] < 1 )
  	{
  	  send_to_char("You have not learned the Gaia protection totem.\n\r", ch);
  	  return;
  	}

	if ( ch->position == POS_FIGHTING )
	{
	 send_to_char("Not while fighting!\n\r", ch);
	 return;
	}

/* Remove gaia in handler.c */
  
    if ( IS_AFFECTED(ch, AFF_SAFE) )
    {
	REMOVE_BIT( ch->affected_by, AFF_SAFE );
	send_to_char( "Gaia's protection disappears.\n\r", ch );
    }
    else
    {
	send_to_char( "You feel safe under Gaia's protection\n\r", ch );
	SET_BIT(ch->affected_by, AFF_SAFE);
    }
    return;
}

/*
void do_zulo( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH]; 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
   
    if (ch->pcdata->powersde[VAM_VICI] < 2)
    {
	send_to_char("You are not trained in the Vicissitude discipline.\n\r",ch);
	return;
    }
    
    if ( ch->pcdata->condition[COND_THIRST] < 100 && !IS_POLYAFF(ch, POLY_ZULO))
    {
	   send_to_char("You must be at full blood to use this power.\n\r",ch);
	   return;
    }
    
	if (ch->stance[0] != -1) do_stance(ch,"");
	
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");

    if ( !IS_POLYAFF(ch, POLY_ZULO))
	    ch->pcdata->condition[COND_THIRST] -= 100;
    
	clear_stats(ch);
	
    if ( !IS_SET(ch->polyaff, POLY_ZULO) )
    {
	act( "You change into an enormous black beast.", ch, NULL, NULL, TO_CHAR );
	act( "$n changes into an enormous black beast.", ch, NULL, NULL, TO_ROOM );
	SET_BIT(ch->polyaff, POLY_ZULO);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf, "%s the huge black beast", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( IS_SET(ch->polyaff, POLY_ZULO) )
    {
	act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
	act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->polyaff, POLY_ZULO);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	clear_stats(ch);
  	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    else
	send_to_char( "Zulo: The power to transform into a huge black beast\n\r", ch );
    return;
}
*/

/*
void do_mitsukeru( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_SORA]
<1) {
	send_to_char("Huh?\n\r", ch );
	return;}
	if (arg[0] == '\0')
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim,
AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim,
AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    ch->fight_timer = 10;
    return;
}

void do_koryou( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_SORA]
<2) {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}
	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powersd[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch,
ITEMA_VISION))
    {
	send_to_char("You are unable to read their aura.\n\r",ch);
	return;
    }

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
	if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    sprintf(buf,"Hp:%ld/%ld, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
   return;
   }
*/

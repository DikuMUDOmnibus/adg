/******************************************
 All contents in this file created by Nephesch.
 However, all material is free for use as long as
 you give me some credit. As far as I know this
 is the most accurate coding of the White Wolf
 Fae class. Have fun everyone.
******************************************
* Content Modified By Jalthier Firestar  *
* Some work done by Paithan		 *
******************************************/
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

void chicanery_art( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;
  if ( ch->pcdata->powers[FAE_CHIC] == 0 )
  {
    stc("      Chicanery: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[FAE_CHIC] >= 1 )
    stc("      Chicanery:  Fuddle",ch);
  if ( ch->pcdata->powers[FAE_CHIC] >= 2 )
    stc(" Fugue",ch);
  if ( ch->pcdata->powers[FAE_CHIC] >= 3 )
    stc(" Haunted",ch);
  if ( ch->pcdata->powers[FAE_CHIC] >= 4 )
    stc(" Veiled",ch);
  if ( ch->pcdata->powers[FAE_CHIC] >= 5 )
    stc(" Captive",ch);
  stc("\n\r",ch);
  return;
} 
void legerdemain_art( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;
  if ( ch->pcdata->powers[FAE_LEGE] == 0 )
  {
    stc("      Legerdemain: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[FAE_LEGE] >= 1 )
    stc("      Legerdemain:  Faesight",ch);
  if ( ch->pcdata->powers[FAE_LEGE] >= 2 )
    stc(" Ensnare",ch);
  if ( ch->pcdata->powers[FAE_LEGE] >= 3 )
    stc(" Effigy",ch);
  if ( ch->pcdata->powers[FAE_LEGE] >= 4 )
    stc(" Gimmix",ch);
  if ( ch->pcdata->powers[FAE_LEGE] >= 5 )
    stc(" Phantom",ch);
  stc("\n\r",ch);
  return;
}
void primal_art( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;
  if ( ch->pcdata->powers[FAE_PRIM] == 0 )
  {
    stc("      Primal: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[FAE_PRIM] >= 1 )
    stc("      Primal:  Willow",ch);
  if ( ch->pcdata->powers[FAE_PRIM] >= 2 )
    stc(" Heather",ch);
  if ( ch->pcdata->powers[FAE_PRIM] >= 3 )
    stc(" Oakenshield",ch);
  if ( ch->pcdata->powers[FAE_PRIM] >= 4 )
    stc(" Holly",ch);
  if ( ch->pcdata->powers[FAE_PRIM] >= 5 )
    stc(" Elderform",ch);
  stc("\n\r",ch);
  return;
}
void soothsay_art( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;
  if ( ch->pcdata->powers[FAE_SOOT] == 0 )
  {
    stc("      Soothsay: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[FAE_SOOT] >= 1 )
    stc("      Soothsay:  Viewaura",ch);
  if ( ch->pcdata->powers[FAE_SOOT] >= 2 )
    stc(" Boil",ch);
  if ( ch->pcdata->powers[FAE_SOOT] >= 3 )
    stc(" Tattletale",ch);
  if ( ch->pcdata->powers[FAE_SOOT] >= 4 )
    stc(" Augury",ch);
  if ( ch->pcdata->powers[FAE_SOOT] >= 5 )
    stc(" Fate",ch);
  stc("\n\r",ch);
  return;
}
void sovereign_art( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;
  if ( ch->pcdata->powers[FAE_SOVE] == 0 )
  {
    stc("      Sovereign: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[FAE_SOVE] >= 1 )
    stc("      Sovereign:  Protocol",ch);
  if ( ch->pcdata->powers[FAE_SOVE] >= 2 )
    stc(" Dictum",ch);
  if ( ch->pcdata->powers[FAE_SOVE] >= 3 )
    stc(" Grandeur",ch);
  if ( ch->pcdata->powers[FAE_SOVE] >= 4 )
    stc(" Weaver",ch);
  if ( ch->pcdata->powers[FAE_SOVE] >= 5 )
    stc(" Geasa",ch);
  stc("\n\r",ch);
  return;
}
void wayfare_art( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;
  if ( ch->pcdata->powers[FAE_WAYF] == 0 )
  {
    stc("      Wayfare: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[FAE_WAYF] >= 1 )
    stc("      Wayfare:  Quicksilver",ch);
  if ( ch->pcdata->powers[FAE_WAYF] >= 2 )
    stc(" Hopscotch",ch);
  if ( ch->pcdata->powers[FAE_WAYF] >= 3 )
    stc(" Passage",ch);
  if ( ch->pcdata->powers[FAE_WAYF] >= 4 )
    stc(" Wind",ch);
  if ( ch->pcdata->powers[FAE_WAYF] >= 5 )
    stc(" Flicker",ch);
  stc("\n\r",ch);
  return;
}
void do_faepowers( CHAR_DATA *ch, char *argument )
{


  if ( IS_NPC(ch) ) return;

  if (IS_CLASS(ch, CLASS_FAE) )
  {
    stc("-------------------------==| Changeling Powers |==----------------------\n\r",ch);
     chicanery_art(ch);
    legerdemain_art(ch);
    primal_art(ch);
    soothsay_art(ch);
    sovereign_art(ch);
    wayfare_art(ch);
    divide2_to_char(ch);
    return;
  }
  else
   return;

  return;
}
void do_grandeur( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

    if (!IS_CLASS(ch, CLASS_FAE) )

    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[FAE_SOVE] < 3  )
    {
        send_to_char("You must obtain at least level 3 in Sovereign to use Grandeur.\n\r",ch);
	return;
    }

    if (has_timer(ch)) return;

    if (IS_AFFECTED(ch, AFF_STEELSHIELD))
    {
        REMOVE_BIT(ch->affected_by, AFF_STEELSHIELD);
        act( "You lower your Grandeur.",  ch, NULL, NULL, TO_CHAR    );
        act( "$n looks less impressive.",  ch, NULL, NULL, TO_NOTVICT );
        return;
    }
    
    SET_BIT(ch->affected_by, AFF_STEELSHIELD);
    act( "You raise your Grandeur.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n suddenly looks very impressive.",  ch, NULL, NULL, TO_NOTVICT);
    return;
}
void do_faeawe    ( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch)) return;
        if (!IS_CLASS(ch, CLASS_FAE))
	{
	  send_to_char("Huh?\n\r", ch);
          return;
        }
        if (!IS_SET(ch->pcdata->kith, KITH_SIDHE))
	{
	  send_to_char("Huh?\n\r", ch);
          return;
        }
        if (IS_EXTRA(ch, EXTRA_AWE))
	{
	  REMOVE_BIT(ch->extra, EXTRA_AWE);
	  send_to_char("You are no longer Awe Inspiring.\n\r", ch);
	  act("$n is no longer awe inspiring.", ch, NULL, NULL, TO_ROOM);
	  return;
	}
        SET_BIT(ch->extra, EXTRA_AWE);
	send_to_char("You are now Awe Inspiring.\n\r", ch);
	act("$n is now awe inspiring", ch, NULL, NULL, TO_ROOM);
        return;


}
void do_dictum( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buffy[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int awe;
    

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;
/*    if (!IS_SET(ch->special, SPC_FAE) ) */
/* Jalthier's Test Code */
    if ( !IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->pcdata->powers[FAE_SOVE] < 2 )
    {
        send_to_char( "You must obtain at least level 1 in Sovereign to use Dictum.\n\r", ch );
	return;
    }
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Command whom to do what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you command yourself??\n\r", ch );
	return;
    }

  if (IS_CLASS(ch, CLASS_FAE) )
  {
    if (ch->pcdata->powers[FAE_SOVE] > 3)
	awe = 75;
    else if (ch->pcdata->powers[FAE_SOVE] > 4)
	awe = 100;
    else awe = 25;
  }
  else
   awe = ch->spheres[MMIN] * 20;

    if (IS_EXTRA(ch, EXTRA_AWE))
    {
      awe += 75;
    }

    if ( !IS_NPC(victim) && victim->level != 3 )
    {
	send_to_char( "You can only command other avatars.\n\r", ch );
	return;
    }
  
  if ( ch->pcdata->powers[FAE_SOVE] > 2 )
  {
    sprintf( buffy, "%s %s", arg2, argument );
    if (IS_NPC(victim))
    	sprintf( buf, "I think %s wants to %s",victim->short_descr,buffy);
    else if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH))
    	sprintf( buf, "I think %s wants to %s", victim->morph,buffy);
    else
    	sprintf( buf, "I think %s wants to %s", victim->name,buffy);
    do_say(ch,buf);
  }
  else
  {
    if (IS_NPC(victim))
        sprintf( buf, "I think %s wants to %s", victim->short_descr, arg2 );
    else if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH))
        sprintf( buf, "I think %s wants to %s", victim->morph, arg2 );
    else
        sprintf( buf, "I think %s wants to %s", victim->name, arg2 );
    do_say(ch,buf); 
  }

    if ( IS_NPC(victim) && victim->level >= awe )
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
	if (!str_cmp("give",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("dro",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("drop",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("gif",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("giv",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("gi",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("gift",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("c",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("ca",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("cas",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("cast",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("k",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("ki",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("kil",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }
	if (!str_cmp("kill",arg2)) {
                send_to_char(" You Failed.\n\r", ch );
                return;
        }

    act("You blink in confusion.",victim,NULL,NULL,TO_CHAR);
    act("$n blinks in confusion.",victim,NULL,NULL,TO_ROOM);
    strcpy(buf,"Yes, you're right, I do...");
    do_say(victim,buf);
    if ( ch->pcdata->powers[FAE_SOVE] > 2 )
      interpret( victim, buffy );
    else interpret( victim, arg2 );
    return;
  
}

void do_captive( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if  (ch->pcdata->powers[FAE_CHIC] < 5)
    {
        send_to_char("You must obtain at least level 5 in Chicanery to use Captive Heart.\n\r",ch);
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
	send_to_char( "What do you wish to Possess?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "What an intelligent idea!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to possess them.\n\r", ch );
	return;
    }

//   if (IS_SET(ch->special, SPC_FAE) )
      ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
    return;
}
void do_faesight( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if  (ch->pcdata->powers[FAE_LEGE] < 1)
    {
        send_to_char("You must obtain at least level 1 in Legerdemain to use Faesight.\n\r",ch);
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
        send_to_char( "Your senses increase to incredible proportions.\n\r", ch);
    }
    
    return;
}
void do_illusion( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )

    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[FAE_CHIC] < 1 )
    {
      stc("You must obtain level 1 Chicanery to use Fuddle.\n\r",ch);
      return;
    }

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }

    ch->pcdata->followers++;


	
	victim = create_mobile( get_mob_index( 30000 ) );
      victim->short_descr = str_dup(ch->name);
      sprintf(buf, "%s is hovering here.\n\r",ch->name);
      victim->long_descr = str_dup(buf);
	sprintf(buf,"Clone");
      victim->name = str_dup(buf);
      victim->level = 200;
      victim->max_hit  = ch->max_hit;
      victim->hit      = victim->max_hit;
      victim->max_mana = ch->max_mana;
      victim->mana     = victim->max_mana;
      victim->max_move = ch->max_move;
      victim->move     = victim->max_move;
      victim->hitroll  = ch->hitroll;
      victim->damroll  = ch->damroll;
      victim->master   = ch;
	do_follow(victim,ch->name);
      SET_BIT(victim->flag2, VAMP_CLONE);
	SET_BIT(victim->act, ACT_NOEXP);
	char_to_room( victim, ch->in_room );
      act( "You concentrate your powers and form an illusion of yourself.", ch, NULL, victim, TO_CHAR );
	act( "$n waves $s hands and splits in two.", ch, NULL, victim, TO_ROOM );
	return;

}
void do_ensnare( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char  arg[MAX_INPUT_LENGTH];
    char  buf[MAX_INPUT_LENGTH];
    int   sn;
    int   level;
    int   spelltype;
    
    argument = one_argument (argument, arg);

    if ( IS_NPC(ch) )
	return;

	
  	if (!IS_CLASS(ch, CLASS_FAE) )
        {
  	send_to_char("Huh?\n\r", ch);
        return;
        }
     	if (ch->pcdata->powers[FAE_LEGE] < 2 )
        {
        send_to_char("You need level 2 Legerdemain to use Ensnare.\n\r",ch);
        return; 
        }
       if (arg[0] == '\0')
	{
	   send_to_char("Who do you wish to use Ensnare on?\n\r",ch);
	   return;
	}
  
  	
      if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;  	
    if ( ( sn = skill_lookup( "infirmity" ) ) < 0 ) 
{
sprintf(buf,"Yep, sn is bieng set to %d.",sn);
send_to_char(buf,ch);
return;

}  

    spelltype = skill_table[sn].target;
    level = ch->pcdata->powers[FAE_LEGE];
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return; 
}

void do_passage(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
 
    one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE) )
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->pcdata->powers[FAE_WAYF] < 3)
    {
    send_to_char("You require level 3 Wayfare to use portal Passage.\n\r",ch);
    return;
    }
    if ( arg[0] == '\0')
    {
        send_to_char( "Who do you wish to create a passage to?\n\r", ch );
	return;
    }
 
    if ( (victim = get_char_world( ch, arg ) ) == NULL )
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
 
 
    if ( (victim == ch) 
    ||   victim->in_room == NULL
    ||   IS_NPC(victim)
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
 
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, ch->in_room );
 
    obj = create_object( get_obj_index( OBJ_VNUM_GATE ), 0 );
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, victim->in_room );
    act( "A look of concentration passes over $n's face.", ch, NULL,NULL,
TO_ROOM );
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    act( "$p appears in front of $n in a shower of glamour.", ch, obj, NULL,
TO_ROOM );
    act( "$p appears in front of you in a shower of glamour.", ch, obj,
NULL, TO_CHAR );
    act( "$p appears in front of $n in a shower of glamour.", victim, obj,
NULL, TO_ROOM );
    act( "$p appears in front of you in a shower of glamour.", ch, obj,
victim, TO_VICT );
    return;
}
void do_flicker( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_FAE) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if (ch->pcdata->powers[FAE_WAYF] < 5)
	{
           send_to_char("You must obtain at least level 5 in Wayfare to use Flicker Flash.\n\r",ch);
	   return;
	}


    if ( arg[0] == '\0' )
    {
	send_to_char( "Flicker to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON)) 
    {
    send_to_char("He doesnt want you near him!\n\r",ch);
    return;
    }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }

    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You blink out of existance and appear before $N.", ch, NULL, victim, TO_CHAR);
    act("$n appears in a flash of light before $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n appears in a flash of light in front of you.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
    return;
}
void do_hopscotch( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_FAE) )
    {
	if (ch->pcdata->powers[FAE_WAYF] < 2)
	{
           send_to_char("You must obtain at least level 2 in Wayfare to use Hopscotch.\n\r",ch);
	   return;
	}

    }
    else
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if (IS_IMMUNE(victim, IMM_TRAVEL))
    {
	send_to_char("I don't think they want you to do that.\n\r",ch);
	return;
    }   

    if ( victim == ch ) {
		send_to_char( "Nothing happens.\n\r", ch);
		return; }

    if ( victim->level != LEVEL_AVATAR )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }
    if (victim->in_room == NULL)
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    send_to_char("You leap into the sky.\n\r",ch);
    act("$n leaps high into the sky.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You come flying in from the sky and land gracefully.\n\r",ch);
    act("$n comes flying in from the sky and lands gracefully.",ch,NULL,NULL,TO_ROOM);
    return;
}
void do_wind( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[FAE_WAYF] < 4)
	{
           send_to_char("You must obtain at least level 4 in Wayfare to use Wind Runner.\n\r",ch);
	   return;
	}


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

	send_to_char("Your wings slide into your back.\n\r",ch);
	act("$n's wings slide into $s back.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
	return;
    }
    send_to_char("Your wings extend from your back.\n\r",ch);
    act("A pair of wings extend from $n's back.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    return;
}
void do_augury( CHAR_DATA *ch, char *argument )
{
   
    if (IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[FAE_SOOT] < 4 )
    {
      stc("You must obtain level 4 Soothsay to use Augury.\n\r",ch);
      return;
    }

    if ( !IS_SET(ch->flag2, AFF2_SPIRITGUARD) )
    {
      stc("You awaken your spirit guardian.\n\r",ch);
      SET_BIT(ch->flag2, AFF2_SPIRITGUARD);
      return;
    }

    else
    {
      stc("You dismiss your spirit guardian.\n\r",ch);
      REMOVE_BIT(ch->flag2, AFF2_SPIRITGUARD);
      return;
    }
  return;
}
void do_boil( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->pcdata->powers[FAE_SOOT] < 2)
    {
        send_to_char("You must obtain at level 2 in Soothsay to use Boil and Bubble.\n\r",ch);
	return;
    }
 
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Who do you wish to use Boil on?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_NPC(victim))
    {
        send_to_char("Why use Boil on a mob?\n\r", ch);
        return;
    }

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only use Boil on Avatar's or lower.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim) == TRUE) return;

    if (victim->hit < victim->max_hit)
    {
      send_to_char("They are hurt and suspicious.\n\r", ch);
      return;
    }
    act("You close your eyes and concentrate on $N.",ch,NULL,victim,TO_CHAR);
    act("$n closes $s eyes and concentrates on you.",ch,NULL,victim,TO_VICT);
    act("$n closes $s eyes and cencentrates on $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE(ch, 12);
    if (number_percent( ) < 50)
    {
        send_to_char("You failed.\n\r",ch);
        return;
    }
 
    act("Your flesh begins to rot!", victim,NULL,NULL,TO_CHAR);
    act("$n's flesh begins to rot!", victim,NULL,NULL,TO_ROOM);
    SET_BIT(victim->flag2, AFF2_ROT);
    return;
}

/* To increase powers */

void do_arts( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
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
        if (!str_cmp(arg1,"chicanery"))
	{
            send_to_char("Chicanery: The art of trickery and deceit.\n\r",ch);
            if (ch->pcdata->powers[FAE_CHIC] < 1) 
                send_to_char("You have none of the Chicanery Arts.\n\r",ch);
            if (ch->pcdata->powers[FAE_CHIC] > 0) 
                send_to_char("Captive: You can can take control of others with your mind.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"legerdemain"))
	{
            send_to_char("Legerdemain: The art of illusions and slight of hand.\n\r",ch);
           return;
	}
        else if (!str_cmp(arg1,"primal"))
	{
            send_to_char("Primal: The art of power through nature commune.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"soothsay"))
        {
            send_to_char("Soothsay: The art of understanding.\n\r",ch);
            return;
        }
	else if (!str_cmp(arg1,"sovereign"))
	{
            send_to_char("Sovereign: The art of order and control.\n\r",ch);
            return;
        }

        else if (!str_cmp(arg1,"wayfare"))
	{
            send_to_char("Wayfare: The art of traveling and moving.\n\r",ch);
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
/*
	stc("Please use the new commands, Disciplines and
research.\n\r",ch);
	return;
*/
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
             if (!str_cmp(arg1,"chicanery"       )) {improve = FAE_CHIC;
max=maxlevel;}
        else if (!str_cmp(arg1,"legerdemain"          )) {improve = FAE_LEGE;
max=maxlevel;}
        else if (!str_cmp(arg1,"primal"        )) {improve = FAE_PRIM;
max=maxlevel;}
        else if (!str_cmp(arg1,"soothsay"      )) {improve = FAE_SOOT;
max=maxlevel;}
	else if (!str_cmp(arg1,"sovereign"        )) {improve = FAE_SOVE;
max=maxlevel;}
        else if (!str_cmp(arg1,"wayfare"       )) {improve = FAE_WAYF;
max=maxlevel;}
        else
	{										
            send_to_char("You can improve: Chicanery, Legerdemain, Primal, Soothsay, Sovereign or Wayfare.\n\r",ch);
	    return;
	}
        cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max && ch->pcdata->rank
==AGE_METHUSELAH)
	{
            sprintf(buf,"You have already gained all the powers of the %s
art.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
        if (ch->pcdata->powers[improve] >= max)
        {
           sprintf(buf,"You have already maxed %s.\n\r",arg1);
           send_to_char(buf,ch);
           return;
        }
	if ( cost > ch->practice )
	{
            sprintf(buf,"It costs you %d primal to improve your %s
art.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s art.\n\r",
arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a art, type: arts <art
type> improve.\n\r",ch);
    return;
}

void do_faeenchant( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Enchant whom?\n\r", ch );
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
	send_to_char( "You cannot enchant yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char( "They are already a Changeling.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only enchant avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to enchant vampires!\n\r", ch );
	return;
    }
    
	if (IS_CLASS(victim, CLASS_MONK) )
	{
	send_to_char("You are unable to enchant monks.\n\r", ch);
	return;
	}

	if (IS_CLASS(victim, CLASS_MAGE) )
	{
	send_to_char("You are unable to enchant mages.\n\r", ch);
	return;
	}

	if (IS_CLASS(victim, CLASS_NINJA) )
	{
	send_to_char("You are unable to enchant ninjas\n\r", ch);
	return;
	}

    if (IS_CLASS(victim, CLASS_MOOGLE))
    {
    send_to_char( "Not on moogles!\n\r", ch);
    return;
    }
	
    if (IS_CLASS(victim, CLASS_PALADIN))
    {
    send_to_char( "You are unable to enchant paladins.\n\r", ch);
    return;
    }

	if (IS_CLASS(victim, CLASS_DROW)) {
		send_to_char("Not on a drow!\n\r", ch );
		return; }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "You are unable to enchant werewolves!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "You are unable to enchant demons!\n\r", ch );
	return;
    }


    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You are unable to enchant highlanders.\n\r", ch );
	return;
    }

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot enchant an unwilling person.\n\r", ch );
	return;
    }

    if (victim->exp < 100000)
    {
	send_to_char("They cannot afford the 100000 exp required to be enchanted by you.\n\r",ch);
	return;
    }

    victim->exp -= 100000;

    act("You enchant $N bringing them into the Changeling world.", ch, NULL, victim, TO_CHAR);
    act("$n enchants $N bringing them into the Changeling world.", ch, NULL, victim, TO_NOTVICT);
    act("$n enchants you bringing you into the Changeling world.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now a Changeling, use 'choose' to pick your kith.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->class    = CLASS_FAE;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_choose( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!str_cmp( arg1, "sidhe"  ) )
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_SIDHE;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Sidhe!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "boggan"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_BOGGAN;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Boggan!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "eshu"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_ESHU;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now an Eshu!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "nocker"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_NOCKER;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Nocker!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "redcap"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_REDCAP;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Redcap!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "satyr"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_SATYR;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Satyr!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "sluagh"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_SLUAGH;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Sluagh!\n\r", ch);
       }
    else if ( !str_cmp( arg1, "troll"      ) ) 
	{
	  ch->class = CLASS_FAE;
	  ch->level = 3;
        ch->pcdata->kith = KITH_TROLL;
        ch->lord=str_dup(ch->name);
        send_to_char("You are now a Troll!\n\r", ch);
       }
     else
	{
	  send_to_char("Please pick one of the following: Boggan, Eshu, Nocker, Redcap, Satyr, Sidhe, Sluagh, Troll.\n\r", ch);
	  return;
	}
}
/* More General Fae Powers */
void do_weaver( CHAR_DATA *ch, char *argument )
{

if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
if (ch->pcdata->powers[FAE_SOVE] < 4)
{
send_to_char("You need Sovereign 4 to make a weaver ward.\n\r",ch);
return;
}
  if ( IS_SET(ch->garou1, WW_GRANITE) )
  {
    stc("Your wall of protection fades away.\n\r",ch);
    REMOVE_BIT(ch->garou1, WW_GRANITE);
  }

  if ( !IS_SET(ch->garou1, WW_GRANITE) )
  {
    stc("A wall of power arises around your body to protect you.\n\r",ch);
    SET_BIT(ch->garou1, WW_GRANITE);
    return;
  }

  return;
}
/* Troll's cleave skill */
void do_cleave (CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];

    argument = one_argument (argument, arg);
        
    
    if (IS_NPC(ch)) return;
    if (!IS_SET(ch->pcdata->kith, KITH_TROLL))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if ( arg[0] == '\0' )
    {
    send_to_char("Who do you wish to cleave?\n\r",ch);
    return;
    }
    victim = get_char_room(ch, arg);
    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
     send_to_char("They arent here.\n\r",ch);
     return;
    }

   if (IS_SET(victim->in_room->room_flags, ROOM_SAFE))
   {
     send_to_char("You cannot attack them here.\n\r",ch);
     return;
   }

   if (is_safe(ch,victim) == TRUE) 
     return;
 
   WAIT_STATE(ch,15);
    sprintf(buf,"%s takes aim and",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("You take aim and\n\r",ch);
    if (IS_ARM_R(victim,LOST_ARM) && IS_ARM_L(victim,LOST_ARM) &&
        IS_LEG_R(victim,LOST_LEG) && IS_LEG_L(victim,LOST_LEG))
        {
        send_to_char("All of their limbs are severed already!\n\r",ch);
        return;
        }
       if (number_percent ( ) < 15 )
       {
        if (!IS_ARM_R(victim,LOST_ARM))
        {
        SET_BIT(victim->loc_hp[3],LOST_ARM);
        if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
           take_item(victim,obj);
        make_part(victim,"arm");
        sprintf(buf,
        "He cleaves %s's arm off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        if (!IS_ARM_L(victim,LOST_ARM))
        {
        SET_BIT(victim->loc_hp[2],LOST_ARM);
        if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
           take_item(victim,obj);
        make_part(victim,"arm");
        sprintf(buf,
        "He cleaves %s's arm off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        if (!IS_LEG_R(victim,LOST_LEG))
        {
        SET_BIT(victim->loc_hp[5],LOST_LEG);
        if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
           take_item(victim,obj);
        make_part(victim,"leg");
        sprintf(buf,
        "He cleaves %s's arm off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        if (!IS_LEG_L(victim,LOST_LEG))
        {
        SET_BIT(victim->loc_hp[4],LOST_LEG);
        if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL )
           take_item(victim,obj);
        if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
           take_item(victim,obj);
        make_part(victim,"leg");
        sprintf(buf,
        "He cleaves %s's arm off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        }
send_to_char("You miss.\n\r",ch);
 
return;
}
/* new fate fire */
void do_fate( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) )
     return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_SOOT] < 5)
    {
	  send_to_char("You need level 5 Soothsay to use Fate Fire.\n\r",ch);
	  return;
    }

    if (IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	  REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
        send_to_char("Your hands are no longer engulfed by
flames.\n\r",ch);
	  act("$n's hands are no longer engulfed by
flames.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
    if (!IS_SET(ch->newbits, NEW_MONKFLAME))
    {
	  SET_BIT(ch->newbits, NEW_MONKFLAME);
	  send_to_char("Your hands are engulfed by flames!\n\r",ch);
	  act("$n's hands are engulfed by
flames!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
    }
}
void do_holly( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

	if (!IS_CLASS(ch, CLASS_FAE))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
    if (ch->pcdata->powers[FAE_PRIM] < 4)
    {
	  send_to_char("You need level 4 in Primal to use Holly Strike.\n\r",ch);
	  return;
    }
  if ( arg[0] == '\0' )
  {
      stc("Whom do you wish to use this on?\n\r",ch);
      return;
  }

  if ( ( victim = get_char_room(ch,arg) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }

  if(IS_SET(ch->affected_by,AFF_SHADOWPLANE) &&
	!IS_SET(victim->affected_by,AFF_SHADOWPLANE))
	{stc("You are too insubstantial!",ch);return;}
  if(!IS_SET(ch->affected_by,AFF_SHADOWPLANE) &&
	IS_SET(victim->affected_by,AFF_SHADOWPLANE))
	{stc("They are too insubstantial!",ch);return;}


  dam = number_range((ch->pcdata->powers[FAE_PRIM] * 500),(ch->pcdata->powers[FAE_PRIM] *525));
  act("You summon the warriors plant to strike $N.",ch,NULL,victim,TO_CHAR);
  act("$n points at you and razor sharp holly vines strike you.",ch,NULL,victim,TO_VICT);
  mage_damage(ch,victim,dam,"Holly Strike",MAGEDAM_OTHER);
  WAIT_STATE(ch, 10);
  return;
}
void do_fugue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_CHIC] < 2 )
    {
      stc("You must obtain level 2 Chicanery to use Fugue.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Whose mind do you wish to read?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	stc("Why would you want to read your own mind?\n\r",ch);
	return;
    }

    if ( victim->level > ch->level )
    {
      stc("Their mind is far too powerful for you to read.\n\r",ch);
      return;
    }


    victim->unveil = ch;
    act("You concentrate deeply upon $N.\n\rYou have invaded $S's mind.\n\r", ch,NULL,victim,TO_CHAR );
    return;
}
void do_effigy( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;
 
    argument = one_argument( argument, arg1 );
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE) )
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->pcdata->powers[FAE_LEGE] < 3)
    {
    send_to_char("You need Legerdemain 3 to use Effigy.\n\r",ch);
    return;
    }
 
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Make a clone of what object?\n\r", ch );
        return;
    }
 
    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
        send_to_char( "You dont have that item.\n\r", ch );
	return;
    }
 
   if (obj->points > 0
   || IS_SET(obj->quest, QUEST_ARTIFACT)
   || IS_SET(obj->quest, QUEST_RELIC)
   || IS_SET(obj->quest, ITEM_EQUEST)   
|| (obj->chobj) != NULL
||    obj->item_type == ITEM_QUEST 
||    obj->item_type == ITEM_QUESTCARD)    
   

   {

   send_to_char("You cant clone that object.\n\r",ch);
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
    obj2->timer = ch->pcdata->powers[FAE_LEGE] * 2;
SET_BIT(obj2->quest, QUEST_CLONED);    
act( "A replica of $p appears in your hands in a flash of glamour.", ch, obj,
NULL, TO_CHAR );
    act( "A replica of $p appears in $n's hands in a flash of glamour.", ch,
obj, NULL, TO_ROOM );
 if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);	
   WAIT_STATE(ch, 16);
    return;
}
void do_fuddle( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[FAE_CHIC] < 1 )
    {
      stc("You must obtain level 1 Chicanery to use fuddle.\n\r",ch);
      return;
    }

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }

    ch->pcdata->followers++;


	victim = create_mobile( get_mob_index( 30000 ) );
      victim->short_descr = str_dup(ch->name);
      sprintf(buf, "%s is hovering here.\n\r",ch->name);
      victim->long_descr = str_dup(buf);
      victim->name = str_dup(ch->name);
      victim->level = 200;
      victim->max_hit  = ch->max_hit;
      victim->hit      = victim->max_hit;
      victim->max_mana = ch->max_mana;
      victim->mana     = victim->max_mana;
      victim->max_move = ch->max_move;
      victim->move     = victim->max_move;
      victim->hitroll  = ch->hitroll;
      victim->damroll  = ch->damroll;
      SET_BIT(victim->flag2, VAMP_CLONE);
	SET_BIT(victim->act, ACT_NOEXP);
	char_to_room( victim, ch->in_room );
	act( "You twiddle your fingers and in a flash of pure glamour you split in two.", ch, NULL, victim, TO_CHAR );
	act( "$n twiddles $s fingers and splits in two in a flash of glamour.", ch, NULL, victim,
TO_ROOM );
	return;

}
void do_gimmix( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[FAE_LEGE] < 4 )
    {
      stc("You must obtain level 4 Legerdemain to use Gimmix.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Which illusion do you wish to control?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar? thats kinda dumb.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
      stc("Not on players.\n\r",ch);
      return;
    }

    if ( !IS_SET(victim->flag2, VAMP_CLONE) )
    {
      stc("That is not a clone.\n\r",ch);
      return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You take control of $N's mind.",ch,NULL,victim,TO_CHAR);
    act("$n takes control of $N's mind.",ch,NULL,victim,TO_ROOM);

    return;
}
/* Boggan Seeming - Fae battle forms and true visages */
void do_boggan( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE))
    {
        send_to_char("Huh?\n\r",ch);
    }
    if (!IS_SET(ch->pcdata->kith, KITH_BOGGAN))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 1500;
        ch->hit = ch->hit - 1500;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 150;
        ch->hitroll = ch->hitroll - 150; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Boggan.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Boggan");    
	sprintf(buf, "%s a short stocky boggan", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 1500;
        ch->hit = ch->hit + 1500;
        ch->damroll = ch->damroll + 150;
        ch->hitroll = ch->hitroll + 150;
        return;
	}
}
/* Eshu Seeming - Fae battle forms and true visages */
void do_eshu( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_ESHU))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 1500;
        ch->hit = ch->hit - 1500;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 175;
        ch->hitroll = ch->hitroll - 175; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into an Eshu.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Eshu");    
	sprintf(buf, "%s a dark an alluring eshu", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 1500;
        ch->hit = ch->hit + 1500;
        ch->damroll = ch->damroll + 175;
        ch->hitroll = ch->hitroll + 175;
        return;
	}
}  
/* Nocker Seeming - Fae battle forms and true visages */
void do_nocker( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_NOCKER))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 1500;
        ch->hit = ch->hit - 1500;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 150;
        ch->hitroll = ch->hitroll - 150; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Nocker.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Nocker");    
	sprintf(buf, "%s a skinny red-faced nocker", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 1500;
        ch->hit = ch->hit + 1500;
        ch->damroll = ch->damroll + 150;
        ch->hitroll = ch->hitroll + 150;
        return;
	}
}
/* Pooka Seeming - Fae battle forms and true visages */
void do_pooka( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_POOKA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 1500;
        ch->hit = ch->hit - 1500;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 100;
        ch->hitroll = ch->hitroll - 300; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Pooka.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Pooka");    
	sprintf(buf, "%s a cute pooka", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 1500;
        ch->hit = ch->hit + 1500;
        ch->damroll = ch->damroll + 100;
        ch->hitroll = ch->hitroll + 300;
        return;
	}
}
/* Redcap Seeming - Fae battle forms and true visages */
void do_redcap( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_REDCAP))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 1800;
        ch->hit = ch->hit - 1800;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 200;
        ch->hitroll = ch->hitroll - 300; 
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Redcap.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Redcap");    
	sprintf(buf, "%s a pale menacing redcap", ch->name);
        free_string( ch->morph );
        SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
	  ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 1800;
        ch->hit = ch->hit + 1800;
        ch->damroll = ch->damroll + 200;
        ch->hitroll = ch->hitroll + 300;
        return;
	}
}
/* Satyr Seeming - Fae battle forms and true visages */
void do_satyr( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SATYR))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 2000;
        ch->hit = ch->hit - 2000;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 200;
        ch->hitroll = ch->hitroll - 200; 
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
        REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Satyr.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Satyr");    
	sprintf(buf, "%s an attractive horned satyr", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 2000;
        ch->hit = ch->hit + 2000;
        ch->damroll = ch->damroll + 200;
        ch->hitroll = ch->hitroll + 200;
	SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
        SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
        return;
	}
}
/* Sidhe Seeming - Fae battle forms and true visages */
void do_sidhe( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SIDHE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 2000;
        ch->hit = ch->hit - 2000;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 200;
        ch->hitroll = ch->hitroll - 200; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Sidhe.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Sidhe");    
	sprintf(buf, "%s a tall gorgeous sidhe", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 2000;
        ch->hit = ch->hit + 2000;
        ch->damroll = ch->damroll + 200;
        ch->hitroll = ch->hitroll + 200;
        return;
	}
}
/* Sluagh Seeming - Fae battle forms and true visages */
void do_sluagh( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SLUAGH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 1500;
        ch->hit = ch->hit - 1500;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 100;
        ch->hitroll = ch->hitroll - 100; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Sluagh.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Sluagh");    
	sprintf(buf, "%s a dark and hideous sluagh", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 1500;
        ch->hit = ch->hit + 1500;
        ch->damroll = ch->damroll + 100;
        ch->hitroll = ch->hitroll + 100;
        return;
	}
}
/* Troll Seeming - Fae battle forms and true visages */
void do_troll( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_TROLL))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_SEEMING) )
    {   
        REMOVE_BIT(ch->special, SPC_SEEMING);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM
);
        ch->max_hit = ch->max_hit - 2000;
        ch->hit = ch->hit - 2000;
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 400;
        ch->hitroll = ch->hitroll - 200;
	REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
     {
     send_to_char("You cant change your seeming when changed.\n\r",ch);
     return;
     }
  
  
     if (!IS_SET(ch->special, SPC_SEEMING) )
	{
        if (ch->stance[0] != -1) do_stance(ch,"");
	  act( "You change into your Changeling Seeming.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body twists and turns into a Troll.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->special, SPC_SEEMING);
        if (!str_cmp(ch->name,"Nephesch")) sprintf(buf,"Nephesch the Troll");    
	sprintf(buf, "%s A massive horned troll", ch->name);
        free_string( ch->morph );
	SET_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
        ch->morph = str_dup( buf );
        ch->max_hit = ch->max_hit + 2000;
        ch->hit = ch->hit + 2000;
        ch->damroll = ch->damroll + 400;
        ch->hitroll = ch->hitroll + 200;
        return;
	}
}
/* Redcaps Teeth */
void do_teeth( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE));
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
     if (!IS_SET(ch->pcdata->kith, KITH_REDCAP))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_FANGS) )
    {
	send_to_char("Your teeth turn back to normal.\n\r",ch);
	act("$n's teeth turn back to normal.", ch, NULL, NULL, TO_ROOM);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
	return;
    }
    send_to_char("Your teeth grow to points and turn sickly yellow.\n\r",ch);
    act("$n's teeth grow to points and become sickly yellow.", ch, NULL, NULL, TO_ROOM);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    return;
}
void do_dreameyes( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
	
    if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
    {
	send_to_char("You can no longer see into the Umbra.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
	return;
    }
    send_to_char("You can now see into the Umbra.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}
void do_faehorns( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (!IS_SET(ch->pcdata->kith, KITH_SATYR) && !IS_SET(ch->pcdata->kith, KITH_TROLL))
    {
	send_to_char("Huh?\n\r",ch);
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
void do_faehooves( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SATYR))
    {
	send_to_char("Huh?\n\r",ch);
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
/* Pooka's shapeshift -BUGGED */
/* void do_shapeshift( CHAR_DATA *ch, char *argument )
{
  char arg[MIL];
  char buf[MSL];
  int choice = 0;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_POOKA))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

  if ( arg[0] == '\0' )
  {
    divide_to_char(ch);
    centre_text("Forms",ch);
    divide_to_char(ch);
    stc("    [ 0 ]  Human\n",ch);
    stc("    [ 1 ]  Bunny\n",ch);
    stc("    [ 2 ]  Fox\n",ch);
    stc("    [ 3 ]  Raven\n",ch);
    stc("    [ 4 ]  Cat\n",ch);
    stc("    [ 5 ]  Dog\n",ch);
    stc("    [ 6 ]  Ferret\n",ch);
    stc("    [ 7 ]  Mouse\n",ch);
    stc("    [ 8 ]  Yak\n",ch);
    divide_to_char(ch);
    return;
  }

  choice = is_number( arg ) ? atoi( arg ) : -1;

  if ( choice == 1 )
  {
    sprintf(buf, "A cute little bunny",ch->name);
    stc("You transform into a bunny.\n\r",ch);
    act("$n transforms into a bunny.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 2 )
  {
    sprintf(buf, "A brown fox",ch->name);
    stc("You transform into a fox.\n\r",ch);
    act("$n transforms into a fox.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 3 )
  {
    sprintf(buf, "A black raven",ch->name);
    stc("You transform into a raven.\n\r",ch);
    act("$n transforms into a raven.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 4 )
  {
    sprintf(buf, "A tiny kitten",ch->name);
    stc("You transform into a kitten.\n\r",ch);
    act("$n transforms into a kitten.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 5 )
  {
    sprintf(buf, "A small hairy dog",ch->name);
    stc("You transform into a small dog.\n\r",ch);
    act("$n transforms into a dog.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 6 )
  {
    sprintf(buf, "A cute little ferret",ch->name);
    stc("You transform into a ferret.\n\r",ch);
    act("$n transforms into a ferret.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 7 )
  {
    sprintf(buf, "A tiny white mouse",ch->name);
    stc("You transform into a white mouse.\n\r",ch);
    act("$n transforms into a white mouse.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 8 )
  {
    sprintf(buf, "A big fat yak",ch->name);
    stc("You transform into a yak.\n\r",ch);
    act("$n transforms into a yak.",ch,NULL,NULL,TO_ROOM);
  }
  else if ( choice == 0 )
  {
    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(ch->mflags, MAGE_SHAPED);
      free_string(ch->morph);
      ch->morph = str_dup("");
      stc("You revert to your human form.\n\r",ch);
      act("$n reverts to $s human form.",ch,NULL,NULL,TO_ROOM);
      return;
    }
    else
    {
      stc("You aren't even shapechanged!\n\r",ch);
      return;
    }
  }
  else
  {
    stc("That's not a choice!\n\r",ch);
    do_shapechange(ch,"");
    return;
  }

  SET_BIT(ch->affected_by, AFF_POLYMORPH);
  SET_BIT(ch->mflags, MAGE_SHAPED);
  clear_stats(ch);
  free_string(ch->morph);
  ch->morph = str_dup(buf);
  return;
} */ 
void do_freehold( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
        return;
    }
	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

    
    if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) )
    	{
	    send_to_char("You jump through the ground into the Umbra.\n\r",ch);
	    act("$n jumps into the ground and disappears.",ch,NULL,NULL,TO_ROOM);
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    do_look(ch,"auto");
	    return;
    	}
     if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
   {
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You jump back into the real world.\n\r",ch);
	act("$n leaps out of the ground.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	return;
    }
}

void do_viewaura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

	if (ch->pcdata->powers[FAE_SOOT] < 1 )
    {
	send_to_char("You need level 1 Soothsay first.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "View the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "View the aura on what?\n\r", ch );
	    return;
	}
	if (!IS_ITEMAFF(ch, ITEMA_VISION))
	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if (!IS_ITEMAFF(ch, ITEMA_VISION))
    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
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
    return;
}
void do_oakenshield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;


    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->pcdata->powers[FAE_PRIM] < 3)
    {
   	send_to_char("You need level 3 Primal first.\n\r",ch);
	return;
    }

    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You shield your aura from those around you.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
	return;
    }
    if (IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
    }
}
void do_veiled( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[FAE_CHIC] < 4)
    {
	send_to_char("you need level 4 in Chicanery first.\n\r",ch);
	return;
}

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	send_to_char( "You slowly fade into existance.\n\r", ch );
	act("$n slowly fades into existance.",ch,NULL,NULL,TO_ROOM);
    }
    else
    {
	send_to_char( "You slowly fade out of existance.\n\r", ch );
	act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->act, PLR_WIZINVIS);
    }
    return;
}
void do_tattle( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[FAE_SOOT] < 3)
    {
	send_to_char("you need level 3 in Soothsay first.\n\r",ch);
	return;
    }

	if (arg[0] == '\0')
    {
	send_to_char( "use tattle tale on whom?\n\r", ch );
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
/* GOOD elder form */
void do_elderform( CHAR_DATA *ch, char *argument )
{
char arg [MAX_INPUT_LENGTH];

argument = one_argument( argument, arg );
 
if (IS_NPC(ch)) return;
 
if (!IS_CLASS(ch, CLASS_FAE) )
    {
     send_to_char("Huh?\n\r",ch);
        return;
}
if (ch->pcdata->powers[FAE_PRIM] < 5)
{
send_to_char("You must obtain at least level 5 in Primal to don the Elder Form\n\r",ch);
return;
}
 
 
   if (IS_EXTRA(ch, EXTRA_DRAGON))
    {
    REMOVE_BIT(ch->extra,EXTRA_DRAGON);
    act( "Your extra arms fold up and disappear.", ch, NULL, NULL, TO_CHAR );
    act( "$n's extra arms fold up and disappear.", ch, NULL, NULL, TO_ROOM );
    REMOVE_BIT(ch->newbits, THIRD_HAND);
    REMOVE_BIT(ch->newbits, FOURTH_HAND);
    REMOVE_BIT(ch->extra, DEM_MIGHT);
    REMOVE_BIT(ch->extra, DEM_TOUGH);
    if (ch->hit < 1) ch->hit = 1;
    ch->damroll = ch->damroll - 400;
    ch->hitroll = ch->hitroll - 200;
    return;
    } 
 
    act( "You take on the elder form by growing 2 more arms.", ch, NULL, NULL, TO_CHAR );
    act( "$n's body bulges as two arms rip out of his sides.", ch, NULL, NULL,TO_ROOM);
    SET_BIT(ch->newbits, FOURTH_HAND);
    SET_BIT(ch->newbits, THIRD_HAND);
    SET_BIT(ch->extra, DEM_MIGHT);
    SET_BIT(ch->extra, DEM_TOUGH);
    ch->damroll = ch->damroll + 400;
    ch->hitroll = ch->hitroll + 200;
    SET_BIT(ch->extra,EXTRA_DRAGON);

return;
}
void do_haunted(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;

	if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_CHIC] < 3)
    {
	send_to_char("you need level 3 in Chicanery to use Haunted Heart.\n\r",ch);
	return;
    }

	if ((victim = ch->fighting) == NULL) {
	send_to_char("You are not fighting anyone.\n\r",  ch);
	return;}

	act("$n gazes into your eyes.",ch,NULL,victim,TO_VICT);
	act("You gaze into $N's eyes.",ch,NULL,victim,TO_CHAR);
	act("$n gazes into $N's eyes.",ch,NULL,victim,TO_NOTVICT);

	if ( number_percent() > 25 ) {
	send_to_char("You failed.\n\r", ch );
	return;}

	else {
		do_flee(victim,"");	
	WAIT_STATE(ch, 16);
	return;	}

	return;
}
/* Sluagh Squirm */
void do_squirm( CHAR_DATA *ch, char *argument )
{
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SLUAGH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    stc("You try to squirm out of your restraints.\n\r",ch);
    act("$n trys to squirm out of $s restraints.",ch,NULL,NULL,TO_ROOM);

    if ( IS_EXTRA(ch, TIED_UP) )
    {
        act("You squirm out of the restraints.",ch,NULL,NULL,TO_CHAR);
        act("$n squirms out if the restraints.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    {
        act("$n squirms out of the webbing.",ch,NULL,NULL,TO_ROOM);
        send_to_char("You squirm out of the webbing.\n\r",ch);
        affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
        act("$n squirms out of the webbing.",ch,NULL,NULL,TO_ROOM);
        send_to_char("You squirm out of the webbing.\n\r",ch);
        REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    if (IS_AFFECTED(ch, AFF2_TENDRILS))
    {
      act("$n squirms out of the tendrils.",ch,NULL,NULL,TO_ROOM);
      send_to_char("You squirm out of the tendrils.\n\r",ch);
      REMOVE_BIT(ch->affected_by, AFF2_TENDRILS);
    }

    return;
}
void do_heather( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg );
  
    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_PRIM] < 2)
    {
	send_to_char("you need level 2 in Primal to use Heather Balm.\n\r",ch);
	return;
    }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to heal?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("You cannot find them here.\n\r",ch);
    return;
  }

  if ( victim->hit >= victim->max_hit )
  {
    stc("They are already at full health!\n\r",ch);
    return;
  }

  act("You lay your hands on $N, fixing the damage within.",ch,NULL,victim,TO_CHAR);
  act("$n lays $s hands on you, and you feel better.",ch,NULL,victim,TO_VICT);
  act("$n lays $s hands on $N, and $N's body seems to charge with glamour.",ch,NULL,victim,TO_NOTVICT);
  victim->hit += (
number_range((ch->pcdata->powers[FAE_PRIM]*500),(ch->pcdata->powers[FAE_PRIM]*700)));
  if ( victim->hit > victim->max_hit )
    victim->hit = victim->max_hit;
  update_pos(victim);
  WAIT_STATE(ch, 24);
  return;
}


/* Changeling armor command for Boggans */
void do_craftarmour( CHAR_DATA *ch, char *argument )
{
/*    OBJ_INDEX_DATA *pObjIndex; */
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_BOGGAN))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Command: craftarmor <type> <piece>.\n\r", ch);
    return;
    }

    if (arg2[0] == '\0')
    {
	send_to_char("Please specify which piece of chimerical armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor.\n\r",ch);
	return;
    }

    if      (!str_cmp(arg2,"ring"     ) && !str_cmp(arg,"copper")) vnum = 381;
    else if (!str_cmp(arg2,"collar"   ) && !str_cmp(arg,"copper")) vnum = 382;
    else if (!str_cmp(arg2,"plate"    ) && !str_cmp(arg,"copper")) vnum = 383;
    else if (!str_cmp(arg2,"helmet"   ) && !str_cmp(arg,"copper")) vnum = 384;
    else if (!str_cmp(arg2,"leggings" ) && !str_cmp(arg,"copper")) vnum = 385;
    else if (!str_cmp(arg2,"boots"    ) && !str_cmp(arg,"copper")) vnum = 386;
    else if (!str_cmp(arg2,"bracer"   ) && !str_cmp(arg,"copper")) vnum = 387;
    else if (!str_cmp(arg2,"gauntlets") && !str_cmp(arg,"copper")) vnum = 388;
    else if (!str_cmp(arg2,"sleeves"  ) && !str_cmp(arg,"copper")) vnum = 389;
    else if (!str_cmp(arg2,"cape"     ) && !str_cmp(arg,"copper")) vnum = 390;
    else if (!str_cmp(arg2,"belt"     ) && !str_cmp(arg,"copper")) vnum = 391;
    else if (!str_cmp(arg2,"visor"    ) && !str_cmp(arg,"copper")) vnum = 392;
    else if (!str_cmp(arg2,"ring"     ) && !str_cmp(arg,"silver")) vnum = 393;
    else if (!str_cmp(arg2,"collar"   ) && !str_cmp(arg,"silver")) vnum = 394;
    else if (!str_cmp(arg2,"plate"    ) && !str_cmp(arg,"silver")) vnum = 395;
    else if (!str_cmp(arg2,"helmet"   ) && !str_cmp(arg,"silver")) vnum = 396;
    else if (!str_cmp(arg2,"leggings" ) && !str_cmp(arg,"silver")) vnum = 397;
    else if (!str_cmp(arg2,"boots"    ) && !str_cmp(arg,"silver")) vnum = 398;
    else if (!str_cmp(arg2,"gauntlets") && !str_cmp(arg,"silver")) vnum = 399;
    else if (!str_cmp(arg2,"sleeves"  ) && !str_cmp(arg,"silver")) vnum = 400;
    else if (!str_cmp(arg2,"cape"     ) && !str_cmp(arg,"silver")) vnum = 401;
    else if (!str_cmp(arg2,"belt"     ) && !str_cmp(arg,"silver")) vnum = 402;
    else if (!str_cmp(arg2,"bracer"   ) && !str_cmp(arg,"silver")) vnum = 403;
    else if (!str_cmp(arg2,"visor"    ) && !str_cmp(arg,"silver")) vnum = 404;
    else if (!str_cmp(arg2,"ring"     ) && !str_cmp(arg,"gold")) vnum = 405;
    else if (!str_cmp(arg2,"collar"   ) && !str_cmp(arg,"gold")) vnum = 406;
    else if (!str_cmp(arg2,"plate"    ) && !str_cmp(arg,"gold")) vnum = 407;
    else if (!str_cmp(arg2,"helmet"   ) && !str_cmp(arg,"gold")) vnum = 408;
    else if (!str_cmp(arg2,"leggings" ) && !str_cmp(arg,"gold")) vnum = 409;
    else if (!str_cmp(arg2,"boots"    ) && !str_cmp(arg,"gold")) vnum = 410;
    else if (!str_cmp(arg2,"gauntlets") && !str_cmp(arg,"gold")) vnum = 411;
    else if (!str_cmp(arg2,"sleeves"  ) && !str_cmp(arg,"gold")) vnum = 412;
    else if (!str_cmp(arg2,"cape"     ) && !str_cmp(arg,"gold")) vnum = 413;
    else if (!str_cmp(arg2,"belt"     ) && !str_cmp(arg,"gold")) vnum = 414;
    else if (!str_cmp(arg2,"bracer"   ) && !str_cmp(arg,"gold")) vnum = 415;
    else if (!str_cmp(arg2,"visor"    ) && !str_cmp(arg,"gold")) vnum = 416;
    else if (!str_cmp(arg2,"ring"     ) && !str_cmp(arg,"platinum")) vnum = 417;
    else if (!str_cmp(arg2,"collar"   ) && !str_cmp(arg,"platinum")) vnum = 418;
    else if (!str_cmp(arg2,"plate"    ) && !str_cmp(arg,"platinum")) vnum = 419;
    else if (!str_cmp(arg2,"helmet"   ) && !str_cmp(arg,"platinum")) vnum = 420;
    else if (!str_cmp(arg2,"leggings" ) && !str_cmp(arg,"platinum")) vnum = 421;
    else if (!str_cmp(arg2,"boots"    ) && !str_cmp(arg,"platinum")) vnum = 422;
    else if (!str_cmp(arg2,"gauntlets") && !str_cmp(arg,"platinum")) vnum = 423;
    else if (!str_cmp(arg2,"sleeves"  ) && !str_cmp(arg,"platinum")) vnum = 424;
    else if (!str_cmp(arg2,"cape"     ) && !str_cmp(arg,"platinum")) vnum = 425;
    else if (!str_cmp(arg2,"belt"     ) && !str_cmp(arg,"platinum")) vnum = 426;
    else if (!str_cmp(arg2,"bracer"   ) && !str_cmp(arg,"platinum")) vnum = 427;
    else if (!str_cmp(arg2,"visor"    ) && !str_cmp(arg,"platinum")) vnum = 428;

    else
    {
	send_to_char("Please specify which piece of chimerical armor you wish to make: Ring Collar\n\rPlate Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor.\n\r",ch);
	return;
    }
    if ( (ch->pcdata->quest < 100)
         && !str_cmp(arg,"copper"))
    {
	send_to_char("It costs 100 quest points to create a piece of copper chimerical
armour.\n\r",ch);
	return;
    }
    else if ( (ch->pcdata->quest < 250)
         && !str_cmp(arg,"silver"))
    {
     send_to_char("It costs 250 quest points to create a piece of silver chimerical armour.\n\r",ch);
     return;
    }
    else if ( (ch->pcdata->quest < 750)
         && !str_cmp(arg,"gold"))
    {
     send_to_char("It costs 750 quest points to create a piece of gold chimerical armour.\n\r",ch);
     return;
    }
    else if ( (ch->pcdata->quest < 1500)
         && !str_cmp(arg,"platinum"))
    {
     send_to_char("It costs 1500 quest points to create a piece of platinum chimerical armour.\n\r",ch);
     return;
    }
/*    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL) */
    if ( vnum == 0 || (obj = create_object(get_obj_index(vnum)  ,50 )) == NULL)

    {
	send_to_char("Missing object, please inform Nephesch.\n\r",ch);
	return;
    }
    if ( !str_cmp(arg,"copper") )
    {
    ch->pcdata->quest -= 100;
    obj->points += 100;
    }
    else if ( !str_cmp(arg,"silver") )
    {
    ch->pcdata->quest -= 250;
    obj->points += 250;
    }
    else if ( !str_cmp(arg,"gold") )
    {
    ch->pcdata->quest -= 750;
    obj->points += 750;
    }
    else if ( !str_cmp(arg,"platinum") )
    {
    ch->pcdata->quest -= 1500;
    obj->points += 1500;
    }
/*    obj = create_object(pObjIndex, 50); */
    SET_BIT(obj->quest, QUEST_RELIC);
    obj_to_char(obj, ch);
    act("$p forms in your hands in a spray of glamour.",ch,obj,NULL,TO_CHAR);
    act("$p forms in $n's hands in a spray of glamour.",ch,obj,NULL,TO_ROOM);
    return;
}


/* bugged */
/* void do_protocol(CHAR_DATA *ch, char *argument)
{
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *rch;

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_SOVE] < 1)
    {
	send_to_char("you need level 1 in Sovereign to use Protocol.\n\r",ch);
	return;
    }
    if (ch->in_room == NULL) {
		send_to_char("Nothing happens.\n\r",ch);
		return;
	}

    if (IS_MORE(ch, MORE_NEUTRAL)) {
		REMOVE_BIT(ch->more, MORE_NEUTRAL);
		REMOVE_BIT(ch->in_room->room_flags, ROOM_SAFE);
		act("$n stops making the time-out sign.", ch, NULL, NULL, TO_ROOM);
		send_to_char("The peaceful harmony in the air gradually fades away to nothing.\n\r", ch);
		act("The peaceful harmony in the air gradually fades away to nothing.",ch,NULL,NULL,TO_ROOM);
		ch->pcdata->obeah = 30;
		return;
	}

    if (IS_SET(ch->in_room->room_flags, ROOM_SAFE)) {
		send_to_char("The room is already under protocol.\n\r", ch);
		return;
	}

    SET_BIT(ch->in_room->room_flags, ROOM_SAFE);
    SET_BIT(ch->more, MORE_NEUTRAL);
    act("$n makes the time-out sign.", ch, NULL, NULL, TO_ROOM);
    send_to_char("You make the time-out sign.\n\r", ch);
    ch->pcdata->obeah = 30;
    send_to_char("The air is filled with peaceful harmony.\n\r", ch);
    act("The air is filled with peaceful harmony.",ch,NULL,NULL,TO_ROOM);
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    if (rch->fighting != NULL)
        stop_fighting(rch, TRUE);
    }
    return;
} */
/* slightly bugged, fix soon */
/* void do_willow(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_PRIM] < 1)
    {
	send_to_char("you need level 1 in Primal to use Willow Wisp.\n\r",ch);
	return;
    }

    
    if (arg2[0] == '\0') {
        send_to_char("Transport who what?\n\r", ch);
        return;
    }
    if ((victim = get_char_world(ch, arg2)) == NULL) {
		send_to_char("They aren't here.\n\r", ch);
		return;
	}
    if (victim == ch) {
		send_to_char("You cannot send things to yourself!\n\r", ch);
		return;
	}
    if (victim->in_room == NULL || victim->in_room == ch->in_room) {
		send_to_char("But they're right here!\n\r", ch);
		return;
	}
    if ((obj = get_obj_carry(ch, arg1)) == NULL) {
		send_to_char("You do not have that item.\n\r", ch);
		return;
	}
    if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_TRANSPORT)) {
		send_to_char("You are unable to transport anything to them.\n\r", ch);
		if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN)) {
			guardian_message(victim);
		act("$n just failed to send $p to you via a will-o-wisp.",ch,obj,victim,TO_VICT);
		}
		return;
	}

	// NO OTHER CHECKS AFTER THIS POINT
	if (check_giveto_mob(ch, victim, obj)) return;

    send_to_char("A will-o-wisp blinks into existance and lands on your shoulder.\n\r", ch);
    act("A will-o-wisp blinks into existance and lands on $n's shoulder.",ch,NULL,NULL,TO_ROOM);
    act("You give $p to the will-o-wisp.",ch,obj,NULL,TO_CHAR);
    act("$n gives $p to the will-o-wisp.",ch,obj,NULL,TO_ROOM);
    send_to_char("The will-o-wisp giggles and jumps through a rift in reality.\n\r", ch);
    act("The will-o-wisp giggles and jumps through a rift in reality.",ch,NULL,NULL,TO_ROOM);

    obj_from_char(obj);
    obj_to_char(obj,victim);

    send_to_char("A rift in reality appears and a will-o-wisp flies out landing on your shoulder.\n\r", victim);
    act("A rift in reality appears and a will-o-wisp flies out landing on $n's shoulder.",victim,NULL,NULL,TO_ROOM);
    act("You take $p from the will-o-wisp.",victim,obj,NULL,TO_CHAR);
    act("$n takes $p from the will-o-wisp.",victim,obj,NULL,TO_ROOM);
    send_to_char("The will-o-wisp kisses your cheek and blinks out of existance.\n\r", victim);
    act("The will-o-wisp kisses $n's cheek and blinks out of existance.",victim,NULL,NULL,TO_ROOM);

    do_autosave(ch,"");
    do_autosave(victim,"");
    if (!IS_NPC(victim) && ch != victim && IS_MORE(victim, MORE_GUARDIAN)) {
		guardian_message(victim);
		act("$n just sent $p to you via will-o-wisp.",ch,obj,victim,TO_VICT);
	}
    return;
} */
void do_faecharm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;
    char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_POOKA))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
    if ( ch == victim )
    {
  send_to_char( "You cannot charm yourself.\n\r", ch );
	return;
    }
 
    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
   send_to_char( "You cannot charm an ethereal person.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;
sprintf(buf,"A look of concentration crosses your face.");
act(buf,ch,NULL,NULL,TO_CHAR);
sprintf(buf,"A look of concentration crosses over $n's face.\n\r");
act(buf,ch,NULL,victim,TO_ROOM);
 
    if ( ( sn = skill_lookup( "charm" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = 240;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}
void do_geasa( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_SOVE] < 5)
    {
	send_to_char("you need level 5 in Sovereign to use Geasa.\n\r",ch);
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
	sprintf(buf,"You release %s.\n\r",familiar->short_descr);
	send_to_char( buf, ch );
	familiar->wizard = NULL;
	ch->pcdata->familiar = NULL;
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to command using Geasa?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "What an intelligent idea!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to command them.\n\r", ch );
	return;
    }

    if (victim->level > 350)
    {
	send_to_char( "They are too powerful.\n\r", ch );
	return;
    } 
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You take over $N's mind using the art of Geasa.",ch,NULL,victim,TO_CHAR);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
    return;
}
/* these last three commands are fucked a bit, fix later 
void do_phantom( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;

    argument = one_argument(argument, arg1);

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[FAE_LEGE] < 5)
    {
	send_to_char("you need level 5 in Legerdemain to use Phantom Shadows.\n\r",ch);
	return;
    }
 
    if (arg1[0] == '\0') {
        send_to_char("You can create a wyrm or a phoenix.\n\r", ch);
        return;
    }

    if ( ch->pcdata->followers > 3 )
    {
      stc("Nothing happens..\n\r",ch);
      return;
    }
    if (str_cmp(arg1,"wyrm"))
    {
    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->short_descr = str_dup("A massive elder wyrm");
    victim->name = str_dup("wyrm");
    victim->long_descr = str_dup("A massive elder wyrm is here coiling up in readiness. ");
    victim->level = 349;
    victim->hit = 15000;
    victim->max_hit = 15000;
    victim->hitroll = 250;
    victim->damroll = 250;
    victim->armor = -1000;
    SET_BIT(victim->act, ACT_NOEXP);
    
    act("You twirl around in a circle.\nThe ground itself begins to crack open.\n  Suddenly,  A massive elder wyrm breaks through and swerves in front of you.",ch,NULL,NULL,TO_CHAR);
    act("$n twirls about in a circle.\nThe ground itself begins to crack open.\n  Suddenly, A massive elder wyrm breaks through and swerves in front on $n!",ch,NULL,NULL,TO_ROOM);
    char_to_room( victim, ch->in_room );

    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    return;
    }

    if (str_cmp(arg1,"phoenix"))
    {
    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->short_descr = str_dup("A magnificent phoenix");
    victim->name = str_dup("wyrm");
    victim->long_descr = str_dup("A magnificent phoenix hovers in the air waiting. ");
    victim->level = 349;
    victim->hit = 20000;
    victim->max_hit = 20000;
    victim->hitroll = 250;
    victim->damroll = 250;
    victim->armor = -2000;
    SET_BIT(victim->act, ACT_NOEXP);
    
    act("You twist and contort your body in a frenzied dance.\nThe sky itself begins to split open.\n  Suddenly,  A magnificent phoenix breaks through and lands before you.",ch,NULL,NULL,TO_CHAR);
    act("$n twists and contorts $s body in a frenzied dance.\nThe sky itself begins to split open.\n  Suddenly, A magnificent phoenix breaks through and lands in front on $n!",ch,NULL,NULL,TO_ROOM);
    char_to_room( victim, ch->in_room );

    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    return;
    }
}
void do_dreaming( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

	if (has_timer(ch)) return;

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_EARTHMELD) )
    {
            REMOVE_BIT(ch->affected_by, AFF_SHIFT);
            REMOVE_BIT(ch->extra, EXTRA_EARTHMELD);
            REMOVE_BIT(ch->act, PLR_WIZINVIS);
           if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
            free_string(ch->morph);
            ch->morph = str_dup("");
            ch->level = 3;
            ch->trust = 3; 
           send_to_char( "You flicker and fade into existance.\n\r", ch );
            sprintf(buf, "%s flickers and fades into existance",ch->name);
            act(buf,ch,NULL,NULL,TO_ROOM);

            return;

    }
    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + 
     ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5] + 
     ch->loc_hp[6]) == 0 && ch->hit == ch->max_hit)
    {
    return;
    }
    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    SET_BIT(ch->affected_by, AFF_SHIFT);
    SET_BIT(ch->extra, EXTRA_EARTHMELD);
    SET_BIT(ch->act, PLR_WIZINVIS);
    free_string(ch->morph);
    ch->morph = str_dup("Someone");
    ch->level = 4;
    ch->trust = 4;
    send_to_char( "You fade away into the Dreaming's warm embrace.\n\r", ch );
    sprintf(buf, "%s fades away into the Dreaming's warm embrace.",ch->name);
    if ((check = ch->loc_hp[6]) > 0)
    {
    oldcheck = 0;
    sn = skill_lookup("clot");
    while (oldcheck != (check = ch->loc_hp[6]))
    {
        oldcheck = check;
        (*skill_table[sn].spell_fun) (sn,ch->level,ch);
    }
    }
    if ((check = (ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
    ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5])) > 0)
    {
    oldcheck = 0;
    while (oldcheck != (check = ch->loc_hp[0] + ch->loc_hp[1] + 
        ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + 
        ch->loc_hp[5]))
    {
        oldcheck = check;
        reg_mend(ch);
    }
    }
    if (ch->hit < ch->max_hit)
    {
    ch->hit = ch->max_hit;
    ch->agg = 0;
    send_to_char("The Dreaming has completely healed you!\n\r",ch);
    update_pos(ch);
    }
    return;
}
void do_court( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( IS_NPC(ch) ) return;
    if (!IS_CLASS(ch, CLASS_FAE) ||
ch->pcdata->stats[UNI_GEN] != 2) )     { 	stc("Huh?\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax is: court <target> <prince/enchant>\n\r", ch );
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

    if (!IS_CLASS(ch, CLASS_FAE) 
    {
	send_to_char( "But they are not a changeling!\n\r", ch );
	return;
    }

    if ( str_cmp(victim->clan,ch->clan) && str_cmp(arg2,"induct") )
    {
	send_to_char( "You can only grant your favour to someone in your court.\n\r", ch );
	return;
    }

    if ( ch->pcdata->stats[UNI_GEN] >= victim->pcdata->stats[UNI_GEN] )
    {
	send_to_char( "You can only grant your favour to someone of a lower rank.\n\r", ch );
	return;
    }

    if ( !str_cmp(arg2,"prince") && ch->pcdata->stats[UNI_GEN] == 2)
    {
	act("You remove $N's prince privilages!",ch,NULL,victim,TO_CHAR);
	act("$n removes $N's prince privilages!",ch,NULL,victim,TO_NOTVICT);
	act("$n removes your prince privilages!",ch,NULL,victim,TO_VICT);
	act("You make $N a prince!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N a prince!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you a prince!",ch,NULL,victim,TO_VICT);
	return;
    }
    else if ( !str_cmp(arg2,"enchant") && (ch->pcdata->stats[UNI_GEN] == 2))
    {
	act("You remove $N's permission to enchant a mortal!",ch,NULL,victim,TO_CHAR);
	act("$n has removed $N's permission to enchant a mortal!",ch,NULL,victim,TO_NOTVICT);
	act("$n has remove your permission to enchant a mortal!",ch,NULL,victim,TO_VICT);
	act("You grant $N permission to enchant a mortal!",ch,NULL,victim,TO_CHAR);
	act("$n has granted $N permission enchant a mortal!",ch,NULL,victim,TO_NOTVICT);
	act("$n has granted you permission enchant a mortal!",ch,NULL,victim,TO_VICT);
	return;
    }
    else if ( !str_cmp(arg2,"outcast") && victim->pcdata->stats[UNI_GEN] > 2 && 
	ch->pcdata->stats[UNI_GEN] == 2)
    {
	act("You make $N an Outcast!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N an Outcast!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you an Outcast!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( "" );
	return;
    }
    else if ( !str_cmp(arg2,"outcast") && victim->pcdata->stats[UNI_GEN] > 2 )
    {
	act("You make $N an Outcast!",ch,NULL,victim,TO_CHAR);
	act("$n has made $N an Outcast!",ch,NULL,victim,TO_NOTVICT);
	act("$n has made you an Outcast!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( "" );
	return;
    }
    else if ( !str_cmp(arg2,"induct") && victim->pcdata->stats[UNI_GEN] > 2 && 
	ch->pcdata->stats[UNI_GEN] == 2 && strlen(victim->clan) < 2)
    {

	if (IS_SET(victim->special, SPC_REBEL))
	{
	    send_to_char("You cannot induct a Rebel!\n\r",ch);
	    return;
	}

	act("You induct $N into your court!",ch,NULL,victim,TO_CHAR);
	act("$n inducts $N into $s court!",ch,NULL,victim,TO_NOTVICT);
	act("$n inducts you into $s court!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
	return;
    }
    else if ( !str_cmp(arg2,"induct") && victim->pcdata->stats[UNI_GEN] > 2 &&
	strlen(victim->clan) < 2)
    {
	if (IS_SET(victim->special, SPC_REBEL))
	{
	    send_to_char("You cannot induct a Rebel!\n\r",ch);
	    return;
	}
	act("You induct $N into your court!",ch,NULL,victim,TO_CHAR);
	act("$n inducts $N into $s court!",ch,NULL,victim,TO_NOTVICT);
	act("$n inducts you into $s court!",ch,NULL,victim,TO_VICT);
	free_string(victim->clan);
	victim->clan = str_dup( ch->clan );
	return;
    }
    else if ( !str_cmp(arg2,"accept") &&
	(ch->pcdata->stats[UNI_GEN] == 2 || IS_SET(ch->special,SPC_FAE_PRINCE)))
    {
	if ( victim->pcdata->rank > AGE_CHILDLING)
	{ send_to_char("But they are not a Childling!\n\r",ch); return; }
	act("You accept $N into the court!",ch,NULL,victim,TO_CHAR);
	act("$n has accepted $N into $s court!",ch,NULL,victim,TO_NOTVICT);
	act("$n accepted you into $s court!",ch,NULL,victim,TO_VICT);
	victim->pcdata->rank = AGE_WILDER;
	return;
    }
    else send_to_char( "You are unable to grant that sort of favour.\n\r", ch );
    return;
}
*/
void do_quicksilver( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    

    if (!IS_CLASS(ch, CLASS_FAE) )
    {
	stc("Huh?\n\r",ch);
        return;
    }
	if (ch->pcdata->powers[FAE_WAYF] < 1 )
    {
	send_to_char("You need level 1 Wayfare first.\n\r",ch);
	return;
    }

    
    if (!IS_EXTRA(ch, DEM_SPEED))
    	{
	    send_to_char("You start moving faster than the eye can follow.\n\r",ch);
	    act("$n starts moving faster than the eye can follow.",ch,NULL,NULL,TO_ROOM);
          SET_BIT(ch->extra, DEM_SPEED);
	    return;
    	}
     if (IS_EXTRA(ch, DEM_SPEED))
    {
	REMOVE_BIT(ch->extra, DEM_SPEED);
    	send_to_char("You slow down.\n\r",ch);
	act("$n slows down.",ch,NULL,NULL,TO_ROOM);
	return;
    }
}
void do_poisonweapon(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

	
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_NOCKER))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to poison?\n\r", ch);
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


    if (IS_WEAP(obj, WEAPON_POISON))
    {
	act("$p is already coated with poison.", ch, obj, NULL,
TO_CHAR);
	return;
    }

    act("You dip your hands in some liquid and rub it along $p,\n\r coating it with a sickly venom.",
ch, obj, NULL, TO_CHAR);
    act("$n dips $m hands in some liquid and rubs it along $p,\n\r coating it with a sickly venom.", ch,
obj, NULL, TO_ROOM);

    WAIT_STATE(ch, 8);
    SET_BIT(obj->weapflags, WEAPON_POISON);
    obj->value[1] += ch->pcdata->stats[UNI_GEN];
    obj->value[2] += ch->pcdata->stats[UNI_GEN];
}
void do_freezeweapon(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

	
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_NOCKER))
    {
	send_to_char("Huh?\n\r", ch );
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
void do_flameweapon(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

	
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_NOCKER))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    if (argument[0] == '\0')
    {
	send_to_char("Which item do you wish to enflame?\n\r", ch);
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
/* Eshu's cover(hide) and sap */
void do_cover( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_ESHU))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
	
    if ( IS_AFFECTED(ch, AFF_HIDE) )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You appear from the shadows.\n\r", ch );
    }
    else
    {
	act( "$n disappears into the shadows.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You disappear into the shadows.\n\r", ch );
	SET_BIT(ch->affected_by, AFF_HIDE);
    }
    return;
}
void do_sap( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if (IS_NPC(ch)) return;
	
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_ESHU))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
	
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Sap who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(ch, AFF_HIDE) )
    {
	send_to_char( "You cannot knockout someone if you are not in cover.\n\r", ch);
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Not on yourself!\n\r", ch );
	return;
    }
	
    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "Not while fighting!\n\r", ch );
	return;
    }
    
    if ( victim->in_room == ch->in_room )
	{
	act("You jump from the shadows and smack $N against the back\n\r their head, sending them down.",ch,NULL,victim,TO_CHAR);
	act("$n jumps from the shadows and smacks you upside the head\n\r with a heavy sap\n\rYou feel dizzy and stunned.",ch,NULL,victim,TO_VICT);
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	victim->position = POS_STUNNED;
	WAIT_STATE(ch, 24);
	return;
	}
	
	return;
}
/* sluagh's shadowshade */
void do_shadowshade( CHAR_DATA *ch, char *argument )
{
char buf[MAX_STRING_LENGTH];
ROOM_INDEX_DATA *inroom;   
inroom= ch->in_room;
 
if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SLUAGH))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }


SET_BIT(inroom->room_flags, ROOM_DARK);
sprintf(buf,"A look of concentration passes over %s's face.",
ch->name);
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char("A look of concentration passes over your face.\n\r",ch);
sprintf(buf,"A complete darkness fills the room.\n\r");
act(buf,ch,NULL,NULL,TO_ROOM);
send_to_char(buf,ch);
return;
}
void do_showcourt( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char lord[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (ch->pcdata->stats[UNI_GEN] < 3)
		strcpy(lord,ch->name);
	else strcpy(lord,ch->lord);
    sprintf( buf, "The changelings of %s:\n\r", lord );
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [ Hits ] [ Mana ] [ Move ] [  Exp  ]
\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_FAE)) 
	    continue;
	if ( !str_cmp(ch->lord,lord) || !str_cmp(ch->name,lord))
	{
	    sprintf( buf,
	    "[%-16s] [%-6ld] [%-6ld] [%-6ld] [ %-9d ]\n\r",
		capitalize( gch->name ),
		gch->hit,gch->mana,gch->move,
		gch->exp);
		send_to_char( buf, ch );
	}
    }
    return;
}
void do_faesteed( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;

if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SIDHE))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }


    if ( ch->pcdata->followers > 3 )
    {
      stc("Nothing happens..\n\r",ch);
      return;
    }

    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->short_descr = str_dup("A massive chimerical horse");
    victim->name = str_dup("horse");
    victim->long_descr = str_dup("A massive chimerical horse glowing with glamour stands here. ");
    victim->level = 500;
    victim->hit = 15000;
    victim->max_hit = 15000;
    victim->hitroll = 250;
    victim->damroll = 250;
    victim->armor = -1500;
    SET_BIT(victim->act, ACT_NOEXP);
    SET_BIT(victim->act, ACT_MOUNT);
    
    act("You whistle loudly.\nYou hear a loud trotting off in the distance.\n  Suddenly a massive glowing horse jumps in from nowhere and stands before you.",ch,NULL,NULL,TO_CHAR);
    act("$n whistles loudly.\nYou hear a loud trotting off in the distance.\n  Suddenly a massive glowing horse jumps in from nowhere and stands before $n!",ch,NULL,NULL,TO_ROOM);
    char_to_room( victim, ch->in_room );

    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    return;
}
void do_teethclench( CHAR_DATA *ch, char *argument )
{
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_REDCAP))
    {
	send_to_char("Only those of Kith Redcap can do this?\n\r", ch );
	return;
    }

  if ( IS_SET(ch->garou1, GAROU_CLENCHED) )
  {
    stc("You relax your jaw.\n\r",ch);
    REMOVE_BIT(ch->garou1, GAROU_CLENCHED);
    return;
  }

  if ( !IS_GAR1(ch, GAROU_CLENCHED) )
  {
    stc("You prepare to clench your jaw on your opponents.\n\r",ch);
    SET_BIT(ch->garou1, GAROU_CLENCHED);
    return;
  }
  return;
}

void do_silentwalk( CHAR_DATA *ch, char *argument )
{
    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_ESHU))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    send_to_char( "Your footsteps stop making any sound.\n\r", ch );

    if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	{
	SET_BIT(ch->affected_by, AFF_HIDE);
	return;
	}
    return;
}
void do_entrap( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_SLUAGH))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot entrap yourself.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You cannot entrap an ethereal person.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.25;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}
void do_redclaws(CHAR_DATA *ch, char *argument) {

		if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_REDCAP))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

	if (IS_VAMPAFF(ch, VAM_CLAWS)) {

	send_to_char("You remove the blood stained claws from your wrists.\n\r", ch );
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
	return;}

	send_to_char("You attach blood stained claws to your wrists.\n\r", ch );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);

	return;
}
/* Changeling weapon command for Nockers */
void do_craftweapon( CHAR_DATA *ch, char *argument )
{
/*    OBJ_INDEX_DATA *pObjIndex; */
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_FAE))
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }
    if (!IS_SET(ch->pcdata->kith, KITH_NOCKER))
    {
	send_to_char("yer no Nocker... bugger off?\n\r", ch );
	return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Command: craftweapon <color> <type>.\n\r", ch);
    return;
    }

    if (arg2[0] == '\0')
    {
	send_to_char("Please specify which type of chimerical weapon you wish to make:\n\rLongsword Axe Flail Dagger Spear Claw Mace.\n\r",ch);
	return;
    }

    if      (!str_cmp(arg2,"longsword" ) && !str_cmp(arg,"copper")) vnum = 429;
    else if (!str_cmp(arg2,"axe"       ) && !str_cmp(arg,"copper")) vnum = 430;
    else if (!str_cmp(arg2,"flail"     ) && !str_cmp(arg,"copper")) vnum = 431;
    else if (!str_cmp(arg2,"dagger"    ) && !str_cmp(arg,"copper")) vnum = 432;
    else if (!str_cmp(arg2,"spear"     ) && !str_cmp(arg,"copper")) vnum = 433;
    else if (!str_cmp(arg2,"claw"      ) && !str_cmp(arg,"copper")) vnum = 434;
    else if (!str_cmp(arg2,"mace"      ) && !str_cmp(arg,"copper")) vnum = 435;
    else if (!str_cmp(arg2,"longsword" ) && !str_cmp(arg,"silver")) vnum = 436;
    else if (!str_cmp(arg2,"axe"       ) && !str_cmp(arg,"silver")) vnum = 437;
    else if (!str_cmp(arg2,"flail"     ) && !str_cmp(arg,"silver")) vnum = 438;
    else if (!str_cmp(arg2,"dagger"    ) && !str_cmp(arg,"silver")) vnum = 439;
    else if (!str_cmp(arg2,"spear"     ) && !str_cmp(arg,"silver")) vnum = 440;
    else if (!str_cmp(arg2,"claw"      ) && !str_cmp(arg,"silver")) vnum = 441;
    else if (!str_cmp(arg2,"mace"      ) && !str_cmp(arg,"silver")) vnum = 442;
    else if (!str_cmp(arg2,"longsword" ) && !str_cmp(arg,"gold")) vnum = 443;
    else if (!str_cmp(arg2,"axe"       ) && !str_cmp(arg,"gold")) vnum = 444;
    else if (!str_cmp(arg2,"flail"     ) && !str_cmp(arg,"gold")) vnum = 445;
    else if (!str_cmp(arg2,"dagger"    ) && !str_cmp(arg,"gold")) vnum = 446;
    else if (!str_cmp(arg2,"spear"     ) && !str_cmp(arg,"gold")) vnum = 447;
    else if (!str_cmp(arg2,"claw"      ) && !str_cmp(arg,"gold")) vnum = 448;
    else if (!str_cmp(arg2,"mace"      ) && !str_cmp(arg,"gold")) vnum = 449;
    else if (!str_cmp(arg2,"longsword" ) && !str_cmp(arg,"platinum")) vnum = 450;
    else if (!str_cmp(arg2,"axe"       ) && !str_cmp(arg,"platinum")) vnum = 451;
    else if (!str_cmp(arg2,"flail"     ) && !str_cmp(arg,"platinum")) vnum = 452;
    else if (!str_cmp(arg2,"dagger"    ) && !str_cmp(arg,"platinum")) vnum = 453;
    else if (!str_cmp(arg2,"spear"     ) && !str_cmp(arg,"platinum")) vnum = 454;
    else if (!str_cmp(arg2,"claw"      ) && !str_cmp(arg,"platinum")) vnum = 455;
    else if (!str_cmp(arg2,"mace"      ) && !str_cmp(arg,"platinum")) vnum = 456;
    else
    {
	send_to_char("Please specify which type of chimerical weapon you wish to make:\n\rLongsword Axe Flail Dagger Spear Claw Mace.\n\r",ch);
	return;
    }
    if ( (ch->pcdata->quest < 100)
         && !str_cmp(arg,"copper"))
    {
	send_to_char("It costs 100 quest points to create a copper chimerical
weapon.\n\r",ch);
	return;
    }
    else if ( (ch->pcdata->quest < 250)
         && !str_cmp(arg,"silver"))
    {
     send_to_char("It costs 250 quest points to create a silver chimerical weapon.\n\r",ch);
     return;
    }
    else if ( (ch->pcdata->quest < 750)
         && !str_cmp(arg,"gold"))
    {
     send_to_char("It costs 750 quest points to create a gold chimerical weapon.\n\r",ch);
     return;
    }
    else if ( (ch->pcdata->quest < 1500)
         && !str_cmp(arg,"platinum"))
    {
     send_to_char("It costs 1500 quest points to create a platinum chimerical weapon.\n\r",ch);
     return;
    }
/*    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL) */
    if ( vnum == 0 || (obj = create_object(get_obj_index(vnum)  ,50 )) == NULL)

    {
	send_to_char("Missing object, please inform Nephesch.\n\r",ch);
	return;
    }
    if ( !str_cmp(arg,"copper") )
    {
    ch->pcdata->quest -= 100;
    obj->points += 100;
    }
    else if ( !str_cmp(arg,"silver") )
    {
    ch->pcdata->quest -= 250;
    obj->points += 250;
    }
    else if ( !str_cmp(arg,"gold") )
    {
    ch->pcdata->quest -= 750;
    obj->points += 750;
    }
    else if ( !str_cmp(arg,"platinum") )
    {
    ch->pcdata->quest -= 1500;
    obj->points += 1500;
    }
/*    obj = create_object(pObjIndex, 50); */
    SET_BIT(obj->quest, QUEST_RELIC);
    SET_BIT(obj->value[0], 18000); 
    obj_to_char(obj, ch);
    act("$p forms in your hands in an explosion of glamour.",ch,obj,NULL,TO_CHAR);
    act("$p forms in $n's hands in an explosion of glamour.",ch,obj,NULL,TO_ROOM);
    return;
}

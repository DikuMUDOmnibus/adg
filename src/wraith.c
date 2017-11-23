/****************************************************
 * All contents in this file created by Nephesch.   *
 * Except Skinride,Oubliette, courtesy of Paithan   *
 * However, all material is free for use as long as *
 * you give me some credit. As far as I know this   *
 * is the most accurate coding of the White Wolf    *
 * Wraith class. Have fun everyone.     ~           *
 ****************************************************/

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

void argos_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_ARGO] == 0 )
  {
    stc("      Argos: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_ARGO] >= 1 )
    stc("      Argos:  Enshroud",ch);
  if ( ch->pcdata->powers[WRA_ARGO] >= 2 )
    stc(" Phantomwings",ch);
  if ( ch->pcdata->powers[WRA_ARGO] >= 3 )
    stc(" Flickering",ch);
  if ( ch->pcdata->powers[WRA_ARGO] >= 4 )
    stc(" Jump",ch);
  if ( ch->pcdata->powers[WRA_ARGO] >= 5 )
    stc(" Oubliette",ch);
  stc("\n\r",ch);
  return;
} 
void castigate_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_CAST] == 0 )
  {
    stc("      Castigate: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_CAST] >= 1 )
    stc("      Castigate:  Soulsight",ch);
  if ( ch->pcdata->powers[WRA_CAST] >= 2 )
    stc(" Coax",ch);
  if ( ch->pcdata->powers[WRA_CAST] >= 3 )
    stc(" Secrets",ch);
  if ( ch->pcdata->powers[WRA_CAST] >= 4 )
    stc(" Purify",ch);
  if ( ch->pcdata->powers[WRA_CAST] >= 5 )
    stc(" Defiance",ch);
  stc("\n\r",ch);
  return;
}
void embody_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_EMBO] == 0 )
  {
    stc("      Embody: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_EMBO] >= 1 )
    stc("      Embody:  Whispers",ch);
  if ( ch->pcdata->powers[WRA_EMBO] >= 2 )
    stc(" Phantommask",ch);
  if ( ch->pcdata->powers[WRA_EMBO] >= 3 )
    stc(" none yet",ch);
  if ( ch->pcdata->powers[WRA_EMBO] >= 4 )
    stc(" Statue",ch);
  if ( ch->pcdata->powers[WRA_EMBO] >= 5 )
    stc(" Materialize",ch);
  stc("\n\r",ch);
  return;
}
void inhabit_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_INHA] == 0 )
  {
    stc("      Inhabit: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_INHA] >= 1 )
    stc("      Inhabit:  Empower",ch);
  if ( ch->pcdata->powers[WRA_INHA] >= 2 )
    stc(" Surge",ch);
  if ( ch->pcdata->powers[WRA_INHA] >= 3 )
    stc(" Objectride",ch);
  if ( ch->pcdata->powers[WRA_INHA] >= 4 )
    stc(" Gremlinize",ch);
  if ( ch->pcdata->powers[WRA_INHA] >= 5 )
    stc(" Itemjump",ch);
  stc("\n\r",ch);
  return;
}
void keening_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_KEEN] == 0 )
  {
    stc("      Keening: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_KEEN] >= 1 )
    stc("      Keening:  Dirge",ch);
  if ( ch->pcdata->powers[WRA_KEEN] >= 2 )
    stc(" Ballad",ch);
  if ( ch->pcdata->powers[WRA_KEEN] >= 3 )
    stc(" Muse",ch);
  if ( ch->pcdata->powers[WRA_KEEN] >= 4 )
    stc(" Crescendo",ch);
  if ( ch->pcdata->powers[WRA_KEEN] >= 5 )
    stc(" Requiem",ch);
  stc("\n\r",ch);
  return;
}
void moliate_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_MOLI] == 0 )
  {
    stc("      Moliate: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_MOLI] >= 1 )
    stc("      Moliate:  Imitate",ch);
  if ( ch->pcdata->powers[WRA_MOLI] >= 2 )
    stc(" Sculpt",ch);
  if ( ch->pcdata->powers[WRA_MOLI] >= 3 )
    stc(" Martialry",ch);
  if ( ch->pcdata->powers[WRA_MOLI] >= 4 )
    stc(" Rend",ch);
  if ( ch->pcdata->powers[WRA_MOLI] >= 5 )
    stc(" Bodyshape",ch);
  stc("\n\r",ch);
  return;
}
void outrage_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_OUTR] == 0 )
  {
    stc("      Outrage: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_OUTR] >= 1 )
    stc("      Outrage:  Pinging",ch);
  if ( ch->pcdata->powers[WRA_OUTR] >= 2 )
    stc(" Wraithgrasp",ch);
  if ( ch->pcdata->powers[WRA_OUTR] >= 3 )
    stc(" Stonehand",ch);
  if ( ch->pcdata->powers[WRA_OUTR] >= 4 )
    stc(" Pyro",ch);
  if ( ch->pcdata->powers[WRA_OUTR] >= 5 )
    stc(" Obliviate",ch);
  stc("\n\r",ch);
  return;
}
void pandemonium_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_PAND] == 0 )
  {
    stc("      Pandemonium: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_PAND] >= 1 )
    stc("      Pandemonium:  Weirdness",ch);
  if ( ch->pcdata->powers[WRA_PAND] >= 2 )
    stc(" Befuddlement",ch);
  if ( ch->pcdata->powers[WRA_PAND] >= 3 )
    stc(" Ether",ch);
  if ( ch->pcdata->powers[WRA_PAND] >= 4 )
    stc(" Foulhumor",ch);
  if ( ch->pcdata->powers[WRA_PAND] >= 5 )
    stc(" Tempus",ch);
  stc("\n\r",ch);
  return;
}
void phantasm_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_PHAN] == 0 )
  {
    stc("      Phantasm: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_PHAN] >= 1 )
    stc("      Phantasm:  Elysia *n*",ch);
  if ( ch->pcdata->powers[WRA_PHAN] >= 2 )
    stc(" Lucidity *n*",ch);
  if ( ch->pcdata->powers[WRA_PHAN] >= 3 )
    stc(" Dreams",ch);
  if ( ch->pcdata->powers[WRA_PHAN] >= 4 )
    stc(" Phantasmagoria",ch);
  if ( ch->pcdata->powers[WRA_PHAN] >= 5 )
    stc(" Agon",ch);
  stc("\n\r",ch);
  return;
}
void puppetry_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_PUPP] == 0 )
  {
    stc("      Puppetry: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_PUPP] >= 1 )
    stc("      Puppetry:  Skinride",ch);
  if ( ch->pcdata->powers[WRA_PUPP] >= 2 )
    stc(" Sudden",ch);
  if ( ch->pcdata->powers[WRA_PUPP] >= 3 )
    stc(" Mastervoice",ch);
  if ( ch->pcdata->powers[WRA_PUPP] >= 4 )
    stc(" Rein",ch);
  if ( ch->pcdata->powers[WRA_PUPP] >= 5 )
    stc(" Obliterate",ch);
  stc("\n\r",ch);
  return;
}
void usury_arcanoi( CHAR_DATA *ch )
{
  if (IS_NPC(ch) ) return;

  if ( ch->pcdata->powers[WRA_USUR] == 0 )
  {
    stc("      Usury: None.\n\r",ch);
    return;
  }

  if ( ch->pcdata->powers[WRA_USUR] >= 1 )
    stc("      Usury:  Transfering",ch);
  if ( ch->pcdata->powers[WRA_USUR] >= 2 )
    stc(" Charitable",ch);
  if ( ch->pcdata->powers[WRA_USUR] >= 3 )
    stc(" Withdrawl",ch);
  if ( ch->pcdata->powers[WRA_USUR] >= 4 )
    stc(" Exchange",ch);
  if ( ch->pcdata->powers[WRA_USUR] >= 5 )
    stc(" Investment",ch);
  stc("\n\r",ch);
  return;
}
void do_wraithpowers( CHAR_DATA *ch, char *argument )
{


  if ( IS_NPC(ch) ) return;

  if (IS_CLASS(ch, CLASS_WRAITH) )
  {
    stc("*------------------------**| Wraith Powers |**---------------------*\n\r",ch);
     argos_arcanoi(ch);
    castigate_arcanoi(ch);
    embody_arcanoi(ch);
    inhabit_arcanoi(ch);
    keening_arcanoi(ch);
    moliate_arcanoi(ch);
    outrage_arcanoi(ch);
    pandemonium_arcanoi(ch);
    phantasm_arcanoi(ch);
    puppetry_arcanoi(ch);
    usury_arcanoi(ch);
    divide2_to_char(ch);
    return;
  }
  else
   return;

  return;
}
/* To increase powers */

void do_arcanoi( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
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
        if (!str_cmp(arg1,"argos"))
	{
            send_to_char("Argos: The arcanoi of movement.\n\r",ch);
            if (ch->pcdata->powers[WRA_ARGO] < 1) 
                send_to_char("You have none of the Argos arcanoi.\n\r",ch);
            if (ch->pcdata->powers[WRA_ARGO] > 0) 
                send_to_char("Enshroud: have the ability to disappear.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"castigate"))
	{
            send_to_char("Castigate: The arcanoi of control.\n\r",ch);
           return;
	}
        else if (!str_cmp(arg1,"embody"))
	{
            send_to_char("Embody: The arcanoi of entering material plane.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"inhabit"))
        {
            send_to_char("Inhabit: The arcanoi of relations with non-living objects.\n\r",ch);
            return;
        }
	else if (!str_cmp(arg1,"keening"))
	{
            send_to_char("Keening: The arcanoi of sight beyond sight.\n\r",ch);
            return;
        }

        else if (!str_cmp(arg1,"moliate"))
	{
            send_to_char("Moliate: The arcanoi of flesh-shaping.\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"outrage"))
	{
            send_to_char("Outrage: The arcanoi of destruction and material plane interaction.\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"pandemonium"))
	{
            send_to_char("Pandemonium: The arcanoi of haunting and weather control.\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"phantasm"))
	{
            send_to_char("Phantasm: The arcanoi of dreams.\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"puppetry"))
	{
            send_to_char("Puppetry: The arcanoi of possesion.\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"usury"))
	{
            send_to_char("Usury: The arcanoi of healing and exchange.\n\r",ch);
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

             if (!str_cmp(arg1,"argos"       )) {improve = WRA_ARGO;
max=maxlevel;}
        else if (!str_cmp(arg1,"castigate"          )) {improve = WRA_CAST;
max=maxlevel;}
        else if (!str_cmp(arg1,"embody"        )) {improve = WRA_EMBO;
max=maxlevel;}
        else if (!str_cmp(arg1,"inhabit"      )) {improve = WRA_INHA;
max=maxlevel;}
	else if (!str_cmp(arg1,"keening"        )) {improve = WRA_KEEN;
max=maxlevel;}
        else if (!str_cmp(arg1,"moliate"       )) {improve = WRA_MOLI;
max=maxlevel;}
        else if (!str_cmp(arg1,"outrage"       )) {improve = WRA_OUTR;
max=maxlevel;}
        else if (!str_cmp(arg1,"pandemonium"       )) {improve = WRA_PAND;
max=maxlevel;}
        else if (!str_cmp(arg1,"phantasm"       )) {improve = WRA_PHAN;
max=maxlevel;}
        else if (!str_cmp(arg1,"puppetry"       )) {improve = WRA_PUPP;
max=maxlevel;}
        else if (!str_cmp(arg1,"usury"       )) {improve = WRA_USUR;
max=maxlevel;}
        else
	{										
            send_to_char("You can improve: Argos, Castigate, Embody, Inhabit, Keening, Moliate,\n\r Outrage, Pandemonium, Phantasm, Puppetry, Usury.\n\r",ch);
	    return;
	}
        cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max && ch->pcdata->rank
==AGE_METHUSELAH)
	{
            sprintf(buf,"You have already gained all the powers of the %s
arcanoi.\n\r", arg1);
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
arcanoi.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s arcanoi.\n\r",
arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a art, type: arcanoi <arcanoi name> improve.\n\r",ch);
    return;
}
/*
 *Argos arcanoi
 *1. Enshroud
 *2. Phantomwings
 *3. Flickering
 *4. Jump
 *5. Oubliette
 */
void do_enshroud( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[WRA_ARGO] < 1)
    {
	send_to_char("you need level 1 in Argos to use Enshroud.\n\r",ch);
	return;
}

/*
    if ( !IS_SET(ch->newbits, WRA_INVIS) )
    {
	send_to_char( "You enshroud your corpus with shadows.\n\r", ch );
	act("$n jumps into the shadows and they seem to swallow $m  whole.",ch,NULL,NULL,TO_ROOM);
	SET_BIT(ch->newbits,WRA_INVIS);
    }
    else
    {
	REMOVE_BIT(ch->newbits,WRA_INVIS);

	send_to_char( "You burst from the shadows.\n\r", ch );
	act("The shadows bulge as $n rips through.",ch,NULL,NULL,TO_ROOM);
    }
*/
    return;
}
void do_phantomwings( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[WRA_ARGO] < 2)
	{
           send_to_char("You must obtain at least level 2 in Argos to use Phantom Wings.\n\r",ch);
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
	if (IS_DEMAFF(ch,DEM_UNFOLDED) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",ch);
	    act("$n's wings fold up behind $s back.", ch, NULL, NULL, TO_ROOM);
	    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
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
void do_flickering( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_WRAITH) )
    {
	if (ch->pcdata->powers[WRA_ARGO] < 3)
	{
           send_to_char("You must obtain at least level 3 in Argos to use Flickering.\n\r",ch);
	   return;
	}

    }
    else
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_WRAITH) && !str_cmp(arg, "tempest"))
    {
    send_to_char("You jump into a hole in the nether.\n\r",ch);
    act("$n jumps into a sinkhole.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, get_room_index(ROOM_VNUM_HELL));
    do_look(ch,"");
    send_to_char("You fall through the Tempest!\n\r",ch);
    act("$n falls in from nowhere.",ch,NULL,NULL,TO_ROOM);
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

    if (!IS_CLASS(victim, CLASS_WRAITH) )
    {
	send_to_char("They are not a Wraith.\n\r",ch);
	return;
    }   

    if ( victim == ch ) {
		send_to_char( "Nothing happens.\n\r", ch);
		return; }

    if ( victim->level != LEVEL_AVATAR)
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }
    if (victim->in_room == NULL)
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    send_to_char("You jump through a hole in the nether.\n\r",ch);
    act("$n leaps through a hole in the nether.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You exit the neter.\n\r",ch);
    act("$n appears through a rip in the nether.",ch,NULL,NULL,TO_ROOM);
    return;
}
void do_jump(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
 
    one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->pcdata->powers[WRA_ARGO] < 4)
    {
    send_to_char("You require level 4 in Argos to use Portal Jump.\n\r",ch);
    return;
    }
    if ( arg[0] == '\0')
    {
        send_to_char( "Who do you wish to create a portal to?\n\r", ch );
	return;
    }
 
    if ( (victim = get_char_world( ch, arg ) ) == NULL )
    {
    send_to_char("They arent here.\n\r",ch);
    return;
    }
 
 
    if ( (victim == ch) 
    ||   victim->in_room == NULL
//    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
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
 
 
    obj = create_object( get_obj_index( 32345 ), 0 );
//    obj = create_object( get_obj_index( OBJ_VNUM_GATE ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, ch->in_room );
 
    obj = create_object( get_obj_index( 32345 ), 0 );
//    obj = create_object( get_obj_index( OBJ_VNUM_GATE ), 0 );
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = 5;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, victim->in_room );
    act( "A look of concentration passes over $n's face.", ch, NULL,NULL,
TO_ROOM );
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    act( "$p appears in front of $n in a blast of putrid smoke.", ch, obj, NULL,
TO_ROOM );
    act( "$p appears in front of you in a blast of putrid smoke.", ch, obj,
NULL, TO_CHAR );
    act( "$p appears in front of $n in a blast of putrid smoke.", victim, obj,
NULL, TO_ROOM );
    act( "$p appears in front of you in a blast of putrid smoke.", ch, obj,
victim, TO_VICT );
    return;
}
/* Oubliette isn't coded yet */
/* It is now! */
void do_oubliette(CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MSL];

    if(!IS_CLASS(ch,CLASS_WRAITH)){do_huh(ch,"");return;}
    if (ch->pcdata->powers[WRA_ARGO] <5) 
    {
      stc("You need level 5 in Argos to use Oubliette!\n\r",ch);
      return;
    }

    if(ch->in_room->vnum != ROOM_VNUM_HELL)
	{stc("You must be in the Tempst!\n\r",ch);
	 return;
	}

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	  stc("They are not here.\n\r",ch);
	  return;
	}

    if(!IS_NPC(victim)){stc("Not on players!\n\r",ch);return;}

	char_from_room(ch);
	char_to_room(ch,victim->in_room);
	stc("You step out of the shadow in front of your victim.\n\r",ch);
	return;
    }

/*
 *Castigate arcanoi
 *1. Soulsight
 *2. Coax
 *3. Secrets
 *4. Purify
 *5. Defiance
 */
void do_soulsight( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

	if (ch->pcdata->powers[WRA_CAST] < 1 )
    {
	send_to_char("You need level 1 in Castigate to use Soulsight.\n\r",ch);
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
    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED)||
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
void do_coax( CHAR_DATA *ch, char *argument )
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
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->pcdata->powers[WRA_CAST] < 2 )
    {
        send_to_char( "You must obtain at least level 2 in Castigate to use Coax.\n\r", ch );
	return;
    }
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Coax whom to do what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you coax yourself??\n\r", ch );
	return;
    }

  if (IS_CLASS(ch, CLASS_WRAITH) )
  {
    if (ch->pcdata->powers[WRA_CAST] > 3)
	awe = 75;
    else if (ch->pcdata->powers[WRA_CAST] > 4)
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
  
  if ( ch->pcdata->powers[WRA_CAST] > 2 )
  {
    sprintf( buffy, "%s %s", arg2, argument );
    if (IS_NPC(victim))
    	sprintf( buf, "I just know %s wants to %s",victim->short_descr,buffy);
    else if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH))
    	sprintf( buf, "I just know %s wants to %s", victim->morph,buffy);
    else
    	sprintf( buf, "I just know %s wants to %s", victim->name,buffy);
    do_say(ch,buf);
  }
  else
  {
    if (IS_NPC(victim))
        sprintf( buf, "I just know %s wants to %s", victim->short_descr, arg2 );
    else if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH))
        sprintf( buf, "I just know %s wants to %s", victim->morph, arg2 );
    else
        sprintf( buf, "I just know %s wants to %s", victim->name, arg2 );
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
    strcpy(buf,"Your wish is my command.");
    do_say(victim,buf);
    if ( ch->pcdata->powers[WRA_CAST] > 2 )
      interpret( victim, buffy );
    else interpret( victim, arg2 );
    return;
}
/* Secrets not in */
/* It is now! */
void do_secrets(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char buf[MSL];
  char arg[MSL];

  if(!IS_CLASS(ch,CLASS_WRAITH)){do_huh(ch,"");return;}

  if(ch->pcdata->powers[WRA_CAST] < 3) { 
   stc("You must have level 3 Castigate to use Secrets.\n\r",ch); 
   return;
  }

  if( (victim=get_char_room(ch,arg)) == NULL)
    {stc("They are not here.\n\r",ch);return;}
  sprintf(buf,"You concentrate on %s.\n\r",victim->name);
  stc(buf,ch);
  sprintf(buf,"%s concentrates on you.\n\r",ch->name);
  stc(buf,victim);


  do_berserk(victim,"");
  return;
}

void do_purify( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg);

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->pcdata->powers[WRA_CAST] < 4 )
    {
        send_to_char( "You must obtain at least level 4 in Castigate to use Dark Secrets.\n\r", ch );
	return;
    }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to calm?\n\r",ch);
    return;
  }
  
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }
 
  if ( number_range(1,4) != 3 )
  {    
      stc("You sing a blissful and soothing song.\n\r",ch);
      act("$n starts singing a blissful and soothing song.",ch,NULL,victim,TO_VICT);
      stc("Nothing seems to happen..\n\r",ch);
      WAIT_STATE(ch,12);
      return;
  }

 
  if ( IS_CLASS(victim, CLASS_VAMPIRE) )
  {
    if ( victim->pcdata->stats[UNI_RAGE] < 1 )
    {
      stc("They aren't even raged!\n\r",ch);
      return;
    }
    else
    {
      stc("You sing a blissful and soothing song.\n\r",ch);
      act("$n starts singing a blissful and soothing song.",ch,NULL,victim,TO_VICT);
      do_calm(victim,"");
      return;
    }
    WAIT_STATE(ch, 16);
    return;
  }
  else if ( IS_CLASS(victim, CLASS_WEREWOLF) )
  {
    if ( victim->pcdata->stats[UNI_RAGE] < 100 )
    {
	stc("They aren't even raged!\n\r",ch);
	return;
    }
    else
    {
      stc("You sing a blissful and soothing song.\n\r",ch);
      act("$n starts singing a blissful and soothing song.",ch,NULL,victim,TO_VICT);
      do_unwerewolf(victim,"");
      return;
    }
    WAIT_STATE(ch, 16);
  }
  else
   return;
  return;
}
/* Defiance not in... gonna take a while to code */

/* Embody arcanoi
 *1. Whisper
 *2. Phantommask
 *3. Nothing yet
 *4. Statue
 *5. Materialize
 */
/* whispers not in, like Ventrilloquate so it'll be easy */
void do_phantommask( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
	if (ch->pcdata->powers[WRA_EMBO] < 2  )
    {
	send_to_char("You need level 2 in Embody to use Phantom Mask.\n\r",ch);
	return;
    }

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
void do_statue( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    
     if ( IS_NPC(ch) )
	return;
 
	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

    if (!IS_CLASS(ch, CLASS_WRAITH)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }

    if (ch->pcdata->powers[WRA_EMBO] < 4)
    {
       send_to_char("You must obtain at least level 4 in Embody to use Statue.\n\r", ch);
       return;
    }
 
save_char_obj(ch);

  if ((obj = create_object(get_obj_index( 32346),60)) == NULL)
    {
        send_to_char( "Error - Please inform Nephesch.\n\r", ch);
	return;
    }

    if ( IS_EXTRA(ch, EXTRA_PLASMA) )
    {
      ch->pcdata->obj_vnum = 0;
      REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
      REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
      REMOVE_BIT(ch->extra, EXTRA_PLASMA);
      ch->pcdata->chobj = NULL;
      obj->chobj = NULL;
      free_string(ch->morph);
      ch->morph = str_dup("");
      act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
      act("Your reform your corpus.",ch,obj,NULL,TO_CHAR);
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
    SET_BIT(ch->extra, EXTRA_PLASMA);
    free_string(ch->morph);
    ch->morph = str_dup("a stone statue");
    obj_to_room(obj,ch->in_room);
    return;
}
void do_materialize( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch)) return;
    
	if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}

    if ( !IS_CLASS(ch, CLASS_WRAITH))
    {
	stc("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->powers[WRA_EMBO] < 5)
    {
       send_to_char("You must obtain at least level 5 in Embody to use Materialize.\n\r", ch);
       return;
    }

    	if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	{
//	    send_to_char("You fade into the plane of shadows.\n\r",ch);
//	    act("The shadows flicker and swallow up$n.",ch,NULL,NULL,TO_ROOM);
//	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
//	    do_look(ch,"auto");
	    stc("This power only lets you get into the skinlands.\n\r",ch);
	    return;
    	}
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You force yourself to appear in the skinlands.\n\r",ch);
	act("$n fades into existance.",ch,NULL,NULL,TO_ROOM);
	do_look(ch,"auto");
	ch->fight_timer = 10;
	return;
}


/*
 *Inhabit arcanoi
 *1. Empower - need to figure it out before coding
 *2. Surge - need an idea for it
 *3. Objectride
 *4. Gremlinize
 *5. Itemjump
 */
/* Empower not in */
/* Surge not in */
void do_objectride( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
   
    argument = one_argument( argument, arg );
   
    if ( IS_NPC(ch) ) return;
  
    if ( !IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_INHA] < 3 )
    {
      stc("You must obtain level 3 in Inhabit to use Object Ride.\n\r",ch);
      return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
      stc("You cannot find that object.\n\r",ch);
      return;
    }
    if(!IS_AFFECTED(ch,AFF_SHADOWPLANE))
    {
	stc("You must do this from the astral plane.\n\r",ch);
	return;
    }

    if ( obj->in_room == NULL )
    {
      stc("You cannot locate that object.\n\r",ch);
      return;
    }

    if ( obj->carried_by != NULL )
    {
      stc("That item is being carried by someone!\n\r",ch);
      return;
    }

    if ( obj->in_room->vnum == ch->in_room->vnum )
    {
      stc("You're already there!\n\r",ch);
      return;
    }

    act("$n steps into a nihil and vanishes.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, get_room_index( obj->in_room->vnum ));
    act("You step through a nihil and appear before $p.",ch,obj,NULL,TO_CHAR);
    act("$n steps out of a nihil in front of $p.",ch,obj,NULL,TO_ROOM);
    return;
}
/*
void do_gremlinize( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  bool perm = FALSE;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);

  if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_INHA] < 4 )
    {
      stc("You must obtain level 3 in Inhabit to use Gremlinize.\n\r",ch);
      return;
    }

  if ( arg1[0] == '\0' )
  {
    stc("What object do you want to attempt to gate to?\n\r",ch);
    return;
  }

  if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
  {
    stc("You concentrate deeply on foreign planes, but can find no such object.\n\r",ch);
    return;
  }

  if ( !str_cmp(arg2, "grab" ) )
  {
   if ( IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, ITEM_EQUEST)
	|| IS_SET(obj->quest, QUEST_RELIC) )
   {
     act("You reach through a rift in space and try to pull $p through, but a mystical force rips it from your hands.",ch,obj,NULL,TO_CHAR);
     return;
   }

   if ( obj->carried_by != NULL )
   {
     act("You reach through a rift in space and try to pull $p through, but someone snatches it away from you.",ch,obj,NULL,TO_CHAR);
     return;
   }
  }
  else
  {
   if ( IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, ITEM_EQUEST)
        || IS_SET(obj->quest, QUEST_RELIC) )
   {
     act("You attempt to create a rift in space to $p, but it is unlocatable.",ch,obj,NULL,TO_CHAR);
     return;
   }
   
   if ( obj->carried_by != NULL )
   {
     act("You attempt to create a rift in space to $p, but it seems to be possessed by another.",ch,obj,NULL,TO_CHAR);
     return;
   }

  }

  if ( !str_cmp( arg2, "permanent" ) )
  {
    if ( ch->practice < 5 )
    {
      stc("You require five units of Primal energy to support a permanent rift.\n\r",ch);
      return;
    }

    perm = TRUE;
    ch->practice -= 5;
  }


  formgate( ch, ch->in_room->vnum, obj->in_room->vnum, perm );

  if ( !str_cmp(arg2, "grab" ) )
  {
    obj_from_room( obj );
    obj_to_char( obj, ch );
    act("You reach through a rift in space and pull $p through the tear.",ch,obj,NULL,TO_CHAR);
    act("$n's hand plunges through a small tear in space and reappears holding $p.",ch,obj,NULL,TO_ROOM);
  }
  else
  {
    act("You create a rent in the Tapestry to $p.",ch,obj,NULL,TO_CHAR);
    act("$n creates a small rent in reality.",ch,obj,NULL,TO_ROOM);
  }

  return;

}
void formgate( CHAR_DATA *ch, int inroom, int toroom, bool perm )
{
  OBJ_DATA *in_gate;
  OBJ_DATA *to_gate;
  ROOM_INDEX_DATA *pIn;
  ROOM_INDEX_DATA *pTo;

  pIn = get_room_index( inroom );
  pTo = get_room_index( toroom );

  in_gate = create_object( get_obj_index( OBJ_VNUM_WGATE ), 0 );
  obj_to_room( in_gate, pIn );
  in_gate->value[0] = toroom;
  in_gate->value[1] = MAGE_INROOM_GATE;
  in_gate->value[3] = inroom;
  if ( !perm )
  in_gate->timer = ch->pcdata->powers[WRA_INHA];
  in_gate->item_type = ITEM_WGATE;

  to_gate = create_object( get_obj_index( OBJ_VNUM_WGATE ), 0 );
  obj_to_room( to_gate, pTo );
  to_gate->value[0] = inroom;
  to_gate->value[1] = MAGE_TOROOM_GATE;
  to_gate->value[3] = toroom;
  if ( !perm )
  to_gate->timer = ch->pcdata->powers[WRA_INHA];
  to_gate->item_type = ITEM_WGATE;

  return;
}
*/
void do_itemjump( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];
    char arg1[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument( argument, arg1 );

  if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_INHA] < 5 )
    {
      stc("You must obtain level 5 in Inhabit to use Item Jump.\n\r",ch);
      return;
    }
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: itemjump <object>\n\r", ch );
	return;
    }

	if(!str_cmp(arg1,"return") && IS_SET(ch->extra,EXTRA_OSWITCH)){
	do_humanform(ch,"");
		return;}
/*    else if (IS_SET(ch->extra, EXTRA_OSWITCH) && !str_cmp(arg1,
"return"))
    {
    ch->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup("");
    act("A cloud of putrid smoke pours out of $p and forms into $n.",ch,obj,NULL,TO_ROOM);
    act("Your spirit floats out of $p and reforms its corpus.",ch,obj,NULL,TO_CHAR);
    do_release(ch,"self);
  return;
    }*/
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
	send_to_char( "You cannot do this while polymorphed.\n\r", ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if (obj->chobj != NULL)
    {
	send_to_char( "That item already has someone in it.\n\r", ch);
	return;
    }
    if (!IS_SET(ch->extra, EXTRA_OSWITCH))
    {
    do_oclone(ch,obj->name);
    send_to_char("Ok.\n\r",ch);
/*    act("$n transforms into a cloud of putrid smoke and pours into $p.",ch,obj,NULL,TO_ROOM);
    act("You transform into a cloud of putrid smoke into $p.",ch,obj,NULL,TO_CHAR);
    ch->pcdata->obj_vnum = obj->pIndexData->vnum;
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);*/

    sprintf(buf,"self %s",obj->name);
    do_bind(ch,buf);
    return;
    }
    else if (IS_SET(ch->extra, EXTRA_OSWITCH) && !str_cmp(arg1, "return"))
    {
    send_to_char( "Syntax: itemjump return    to return to your body.\n\r",ch);
    return;
    }
}
/*
 *Keening arcanoi
 *1. Dirge
 *2. Ballad
 *3. Muse
 *4. Crescendo
 *5. Requiem
 */
void do_dirge( CHAR_DATA *ch, char *argument )
{
  if ( IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if  (ch->pcdata->powers[WRA_KEEN] < 1)
    {
        send_to_char("You must obtain at least level 1 in Keening to use Dirge.\n\r",ch);
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
void do_ballad( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->pcdata->powers[WRA_KEEN] < 2 )
    {
      stc("You must obtain level 2 in Keening to use Ballad.\n\r",ch);
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
/* not going to test yet, looks badly buggy
void do_muse(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *familiar;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[WRA_KEEN] < 3)
    {
	send_to_char("you need level 3 in Keening to use Muse.\n\r",ch);
	return;
    }

    if ((familiar = ch->pcdata->familiar) != NULL) 
    {
    if (IS_EXTRA(ch, EXTRA_NO_ACTION))
    {
        extract_char(familiar, TRUE);
        send_to_char("Your astral projection vanishes.\n\r", ch);
        REMOVE_BIT(ch->extra, EXTRA_NO_ACTION);
        REMOVE_BIT(ch->more, MORE_PROJECTION);
    }
    else send_to_char("Nothing happens.\n\r", ch);
    return;
    }

    if (ch->pcdata->followers > 4)
    {
    send_to_char("Nothing happens.\n\r",ch);
    return;
    }

    ch->pcdata->followers++;
    victim=create_mobile(get_mob_index(MOB_VNUM_ASTRAL));
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->level = 50;
    victim->hit = ch->hit;
    victim->max_hit = ch->max_hit;
    victim->mana = ch->mana;
    victim->max_mana = ch->max_mana;
    victim->move = ch->move;
    victim->max_move = ch->max_move;
    victim->hitroll = char_hitroll(ch);
    victim->damroll = char_damroll(ch);
    victim->armor = char_ac(ch);

    free_string(victim->name);
    victim->name = str_dup(ch->name);
    free_string(victim->short_descr);
    victim->short_descr = str_dup(ch->name);
    sprintf(buf,"%s is here.\n\r",ch->name);
    free_string(victim->long_descr);
    victim->long_descr = str_dup(buf);
    char_to_room(victim,ch->in_room);

    send_to_char("You astrally project your spirit into the room.\n\r",ch);
    act("$n closes $s eyes and a look of concentration crosses $s face.",ch,NULL,NULL,TO_ROOM);
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    SET_BIT(victim->more, MORE_SPIRIT);
    SET_BIT(victim->vampaff, VAM_SPIRITEYES);
    SET_BIT(ch->extra, EXTRA_NO_ACTION);
    SET_BIT(ch->more, MORE_PROJECTION);
    return;
} */
void do_crescendo( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->pcdata->powers[WRA_KEEN] < 4)
    {
	send_to_char("you need level 4 in Keening to use Crescendo.\n\r",ch);
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
void do_requiem( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
 
    argument = one_argument(argument,arg);
 
    if ( IS_NPC(ch) ) return;
    if (!IS_CLASS(ch,CLASS_WRAITH))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->pcdata->powers[WRA_KEEN] < 5)
    {
    send_to_char("You require level 5 in Keening to use Requiem.\n\r",ch);
    return;
    }
 
    if (arg[0] == '\0')
    {
    send_to_char("Whom?\n\r",ch);
    return;
    }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They arent here.\n\r", ch );
	return;
    }
 
    if ( ch == victim )
    {
        send_to_char( "You cannot do this to yourself!\n\r", ch );
	return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_SAFE))
    {
    send_to_char("You cannot attack them here.\n\r",ch);
    return;
    }
 if (is_safe(ch,victim)== TRUE) return;
    if ( victim->hit < victim->max_hit )
    {
        send_to_char( "They are hurt and alert.\n\r", ch );
	return;
    }
 
 
    act("You let loose a scream that rattles the fabric of reality at N.",ch,NULL,victim,TO_CHAR);
    act("$n lets loose a scream that rattles the fabric of reality in your face.",ch,NULL,victim,TO_VICT);
    act("$n lets loose a scream that rattles the fabric of reality at 
$N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, 12 );
    if ( number_percent( ) < 50 )
    	dam = number_range(1,4);
    else
    {
    	dam = 0;
        send_to_char("You failed.\n\r",ch);
	return;
    }
 
    dam += char_damroll(ch);
    if (dam == 0) dam = 1;
    if ( !IS_AWAKE(victim) )
	dam *= 5;
 
    if ( dam <= 0 )
	dam = 1;
    victim->hit -= dam;
 
    act("You clutch your head in agony!",victim,NULL,NULL,TO_CHAR);
    act("$n clutches his head in agony!",victim,NULL,NULL,TO_ROOM);
    victim->position = POS_STUNNED;
    if (IS_NPC(victim) && victim->hit < 0) 
    { 
    raw_kill(victim,ch);
    return;
    }
    if (!IS_NPC(victim) && victim->hit < -2) update_pos(ch);
    return;
}
/*
 *Moliate arcanoi
 *1. Imitate
 *2. Sculpt
 *3. Martialry
 *4. Rend
 *5. Bodyshape
 */
/*
void do_imitate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (ch->pcdata->powers[WRA_MOLI] < 1  )
    {
      send_to_char("You must obtain at least level 1 in Moliate to use Imitate.\n\r",ch);
	return;
    }

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
	send_to_char( "You can only use Imitate on Avatars or
lower.\n\r", ch );
	return;
    }


    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) &&
!IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
	sprintf(buf,"Your corpus twists and reshapes as %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"%s's corpus twists and reshapes as
%s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
    	sprintf(buf,"Your corpus twists and reshapes as
%s.",victim->name);
    	act(buf,ch,NULL,victim,TO_CHAR);
    	sprintf(buf,"%s's corpus twists and reshapes as
%s.",ch->morph,victim->name);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
    	sprintf(buf,"%s's corpus twists and reshapes as you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
    	ch->morph = str_dup( victim->name );
	return;
    }
    sprintf(buf,"Your corpus twists and reshapes as %s.",victim->name);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s's corpus twists and reforms as
%s.",ch->name,victim->name);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"%s's corpus twists and reforms as you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->name );
    return;
}
*/
/* seems to have a prev declare STC and do_bonemod
void do_sculpt( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->pcdata->powers[WRA_MOLI] < 2)
    {
	send_to_char("You need level 2 in Moliate to sculpt.\n\r",ch);
	return;
    }
 
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
 
    if ( !IS_NPC(victim) && ch != victim)
    {
	send_to_char( "Not on Players.\n\r", ch );
	return;
    }
 
 
 
    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) &&
!IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
        sprintf(buf,"Your corpus molds and transforms into %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s's corpus molds and transforms into
%s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
        sprintf(buf,"Your corpus molds and transforms into a clone of
%s.",victim->short_descr);
    	act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s's corpus molds and transforms into a clone of
%s.",ch->morph,victim->short_descr);
    	act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"%s's corpus mols and transforms into a clone of
you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT);
    	free_string( ch->morph );
        ch->morph = str_dup( victim->short_descr );
	return;
    }
    sprintf(buf,"Your corpus molds and transforms into a clone of
%s.",victim->short_descr);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"%s's corpus molds and transforms into a clone of
%s.",ch->name,victim->short_descr);
    act(buf,ch,NULL,victim,TO_NOTVICT);
    sprintf(buf,"%s's corpus molds and transforms into a clone of
you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->short_descr );
    return;
}
*/
/*
void do_martialry( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );


	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_MOLI] < 3)
	{
	   send_to_char("You need level 3 in Moliate to use Martialry.\n\r",ch);
	   return;
	}

    if ( arg1[0] == '\0')
    {
	send_to_char( "Syntax: martialry <modification>\n\r",	ch );
	send_to_char( "Horns, Head, Exoskeleton, Tail, Thirdarm, Fourtharm\n\r",ch);
        return;
    }
    if (!IS_CLASS(victim, CLASS_WRAITH) )
    {
	send_to_char("They are not a changeling.\n\r",ch);
	return;
    }   
*/
    /*
     * Set something.
     */
/* 
   if ( !str_cmp( arg1, "horns" ) )
    {
	
 	if (!IS_VAMPAFF(ch, VAM_HORNS))
	 {
	  SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
	  send_to_char("You pull a set of horns out of your
head.\n\r",ch);
	  act("$n pulls a set of horns from his
head!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
       }
	
	if (IS_VAMPAFF(ch, VAM_HORNS))
	 {
	  REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
	  send_to_char("You push your horns back into your head.\n\r",ch);
	  act("$n pushes $n's horns back into $n
head.\n\r",ch,NULL,NULL,TO_ROOM); 
	  return;

	 }
    }

    if ( !str_cmp( arg1, "thirdarm" ) )
    {
	
 	if (!IS_SET(ch->newbits, THIRD_HAND))
	 {
	  SET_BIT(ch->newbits, THIRD_HAND);
	  send_to_char("You pull an extra arm out of your side.\n\r",ch);
	  act("$n pulls a third arm from $s
side!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
       }
	
	if (IS_SET(ch->newbits, THIRD_HAND))
	 {
	  REMOVE_BIT(ch->newbits, THIRD_HAND);
	  send_to_char("You push your extra arm back into your side.\n\r",ch);
	  act("$n pushes $s extra arm into $s
side.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;

	 }
    }    
    if ( !str_cmp( arg1, "fourtharm" ) )
    {
	
 	if (!IS_SET(ch->newbits, FOURTH_HAND))
	 {
	  SET_BIT(ch->newbits, FOURTH_HAND);
	  send_to_char("You pull an extra arm out of your side.\n\r",ch);
	  act("$n pulls a third arm from $s
side!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
       }
	
	if (IS_SET(ch->newbits, FOURTH_HAND))
	 {
	  REMOVE_BIT(ch->newbits, FOURTH_HAND);
	  send_to_char("You push your extra arm back into your side.\n\r",ch);
	  act("$n pushes $s extra arm into $s
side.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;

	 }
    }
     if ( !str_cmp( arg1, "exoskeleton" ) )
    {
	
 	if (!IS_VAMPAFF(ch, VAM_EXOSKELETON))
	 {
	  SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_EXOSKELETON);
	  send_to_char("Your skin is covered by a hard exoskeleton.\n\r",ch);
	  act("$n's skin is covered by a hard exoskeleton!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
       }
	
	if (IS_VAMPAFF(ch, VAM_EXOSKELETON))
	 {
	  REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_EXOSKELETON);
	  send_to_char("Your exoskeleton slowly disappears under your
skin.\n\r",ch);
	  act("$n's hard exoskeleton disappears under $s
skin.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;

	 }
    }    

     if ( !str_cmp( arg1, "tail" ) )
    {
	
 	if (!IS_VAMPAFF(ch, VAM_TAIL))
	 {
	  SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_TAIL);
	  send_to_char("Your spine extends out into a long, pointed
tail.\n\r",ch);
	  act("$n's spine extends to form a long pointed
tail!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
       }
	
	if (IS_VAMPAFF(ch, VAM_TAIL))
	 {
	  REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_TAIL);
	  send_to_char("Your tail slowly retracts into your
spine.\n\r",ch);
	  act("$n's tail shrinks and vanishes into $s
spine.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;

	 }
    }    

     if ( !str_cmp( arg1, "head" ) )
    {
	
 	if (!IS_VAMPAFF(ch, VAM_HEAD))
	 {
	  SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HEAD);
	  send_to_char("Your head transforms into that of a fierce
lion.\n\r",ch);	  
	  act("$n's head transforms into that of a fierce
lion!\n\r",ch,NULL,NULL,TO_ROOM);
	  return;
       }
	
	if (IS_VAMPAFF(ch, VAM_HEAD))
	 {
	  REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HEAD);
	  send_to_char("Your head slowly resumes it's normal
form.\n\r",ch);
	  act("$n's head resumes its normal
form.\n\r",ch,NULL,NULL,TO_ROOM);
	  return;

	 }
}    

*/


    /*
     * Generate usage message.
     */
/*
    do_bonemod( ch, "" );
    return;
}
 */
void do_rend (CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];

    argument = one_argument (argument, arg);
        
    
    if (IS_NPC(ch)) return;
	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_MOLI] < 4)
	{
	   send_to_char("You need level 4 in Moliate to use Rend.\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
    send_to_char("Who do you wish to rend?\n\r",ch);
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
    sprintf(buf,"%s grabs ahold and",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char("You grab ahold and\n\r",ch);
    if (IS_ARM_R(victim,LOST_ARM) && IS_ARM_L(victim,LOST_ARM) &&
        IS_LEG_R(victim,LOST_LEG) && IS_LEG_L(victim,LOST_LEG))
        {
        send_to_char("All of their limbs are severed already!\n\r",ch);
        return;
        }
       if (number_percent ( ) < 45 )
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
        "rips %s's arm off!",victim->name);
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
        "rips %s's arm off!",victim->name);
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
        "rips %s's leg off!",victim->name);
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
        "rips %s's leg off!",victim->name);
        act(buf,ch,NULL,NULL,TO_ROOM);
        send_to_char(buf,ch);
        return;
        }
        }
send_to_char("You fail.\n\r",ch);
 
return;
}
/* wraith zulo */
void do_bodyshape( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_MOLI] < 5)
	{
	   send_to_char("You need level 5 in Moliate to use Bodyshape.\n\r",ch);
	   return;
	}


    if (IS_POLYAFF(ch, POLY_ZULO))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULO);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into your corpus.", ch, NULL, NULL, TO_CHAR );
        act( "$n's shrinks back into their corpus.", ch, NULL, NULL, TO_ROOM
);
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 200;
        ch->hitroll = ch->hitroll - 200; 
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
send_to_char("You cant bodyshape when changed.\n\r",ch);
return;
}
  
  
        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        act( "You transform your corpus into a half-corporeal state.", ch, NULL, NULL, TO_CHAR );
        act( "$n's stretches and twists into a massive shimmering form.", ch, NULL,
NULL, TO_ROOM );
        
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->polyaff, POLY_ZULO);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf, "%s A massive poltergeist", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->damroll = ch->damroll + 200;
        ch->hitroll = ch->hitroll + 200;
        return;
    }
/*
 *Outrage!!! arcanoi hee hee, the next best thing is Puppetry
 *1. Ping
 *2. Wraithgrasp
 *3. Stonehand
 *4. Pyro
 *5. Obliviate!!! muhahaha fuck I'm sick -Nephesch
 */
void do_pinging( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    CHAR_DATA       *mount;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *pexit;
    EXIT_DATA       *pexit_rev;
    char            buf       [MAX_INPUT_LENGTH];
    char            direction [MAX_INPUT_LENGTH];
    char            arg1      [MAX_INPUT_LENGTH];
    char            arg2      [MAX_INPUT_LENGTH];
    int             door;
    int             rev_dir;
    int             dam;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_OUTR] < 1)
	{
	   send_to_char("You need level 1 in Outrage to use Ping!\n\r",ch);
	   return;
	}

    if ( arg1[0] == '\0' )
    {
	send_to_char("Who do you wish to ping?\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char("They are not here.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you ping yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
    {
	send_to_char("But they have someone on their back!\n\r",ch);
	return;
    }
    else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	send_to_char("But they are riding!\n\r",ch);
	return;
    }

    if ( !is_garou(ch) || ( is_garou(ch) && ch->gifts[UKTENA] < 5 ) )
    {
      if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
      {
  	  send_to_char("You are unable to get their feet of the ground.\n\r",ch);
	  return;
      }
    }

    if ( !is_garou(ch) || ( is_garou(ch) && ch->gifts[UKTENA] < 5 ) )
    {
      if ( IS_NPC(victim) && victim->level > 900 )
      {
	  send_to_char("You are unable to get their feet of the ground.\n\r",ch);
	  return;
      }
    }

    if ( (victim->hit < victim->max_hit)
	 || ( victim->position == POS_FIGHTING && victim->fighting != ch) )
    {
	act( "$N is hurt and suspicious, and you are unable to affect $M.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }


    rev_dir = 0;

    if ( arg2[0] == '\0' )
	door = number_range(0,3);
    else
    {
    	if      ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "north" ) )
	    door = 0;
    	else if ( !str_cmp( arg2, "e" ) || !str_cmp( arg2, "east" ) )
	    door = 1;
    	else if ( !str_cmp( arg2, "s" ) || !str_cmp( arg2, "south" ) )
	    door = 2;
    	else if ( !str_cmp( arg2, "w" ) || !str_cmp( arg2, "west" ) )
	    door = 3;
    	else
    	{
	    send_to_char("You can only ping people north, south, east or west.\n\r", ch);
	    return;
    	}
    }

    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}

    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
	if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	{
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	act("$n jumps in from nowhere and pokes $N in the chest.",ch,NULL,NULL,TO_ROOM);
	return;
	}
	sprintf(buf,"$N goes flying and slams into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You send $N crashing into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n sends you crashing into the %s wall.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
	dam = number_range(ch->level, (ch->level * 4));
	victim->hit = victim->hit - dam;
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	if (victim->position == POS_DEAD) {raw_kill(victim,ch);return;}
	return;
    }

    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) && 
	!IS_AFFECTED(victim,AFF_PASS_DOOR ) &&
	!IS_AFFECTED(victim,AFF_ETHEREAL  ) )
    {
	if (IS_SET(pexit->exit_info, EX_LOCKED))
	    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	if (IS_SET(pexit->exit_info, EX_CLOSED))
	    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	{
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	act("$n jumps in from nowhere and pokes $N in the chest.",ch,NULL,NULL,TO_ROOM);
	return;
	}
	sprintf(buf,"$n sends $N crashing through the %s.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You send $N crashing through the %s.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n sends you %s, smashing you through the %s.", direction,pexit->keyword);
	act(buf,ch,NULL,victim,TO_VICT);
	sprintf(buf,"There is a loud crash as $n smashes through the $d.");
	act(buf,victim,NULL,pexit->keyword,TO_ROOM);

	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir] ) != NULL
	&&   pexit_rev->to_room == ch->in_room
	&&   pexit_rev->keyword != NULL )
	{
	    if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
		REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
		REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    if (door == 0) sprintf(direction,"south");
	    if (door == 1) sprintf(direction,"west");
	    if (door == 2) sprintf(direction,"north");
	    if (door == 3) sprintf(direction,"east");
	    char_from_room(victim);
	    char_to_room(victim,to_room);
	    sprintf(buf,"$n comes smashing in through the %s $d.", direction);
	    act(buf,victim,NULL,pexit->keyword,TO_ROOM);
	    dam = number_range(ch->level, (ch->level * 6));
	    victim->hit = victim->hit - dam;
	    update_pos(victim);
    	    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	    if (victim->position == POS_DEAD){raw_kill(victim,ch);return;}
	}
    }
    else
    {
	sprintf(buf,"$n sends $N flying %s.", direction);
	act(buf,ch,NULL,victim,TO_NOTVICT);
	sprintf(buf,"You send $N flying %s.", direction);
	act(buf,ch,NULL,victim,TO_CHAR);
	sprintf(buf,"$n sends you flying %s.", direction);
	act(buf,ch,NULL,victim,TO_VICT);
	if (door == 0) sprintf(direction,"south");
	if (door == 1) sprintf(direction,"west");
	if (door == 2) sprintf(direction,"north");
	if (door == 3) sprintf(direction,"east");
	char_from_room(victim);
	char_to_room(victim,to_room);
	sprintf(buf,"$n comes flying in from the %s.", direction);
	act(buf,victim,NULL,NULL,TO_ROOM);
	dam = number_range(ch->level, (ch->level * 2));
	victim->hit = victim->hit - dam;
	update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
	if (victim->position == POS_DEAD) {raw_kill(victim,ch);return;}
    }
    return;
}
void do_wraithgrasp (CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument (argument, arg);
        
    
    if (IS_NPC(ch)) return;
	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_OUTR] < 2)
	{
	   send_to_char("You need level 2 in Outrage to use Wraithgrasp.\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
    send_to_char("Who do you wish to grasp?\n\r",ch);
    return;
    }
    victim = get_char_room(ch, arg);
    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
     send_to_char("They arent here.\n\r",ch);
     return;
    }
    if (!IS_IMMUNE(victim,IMM_VAMPIRE) && IS_NPC(ch))
    {
	send_to_char( "You cannot grasp an unwilling person.\n\r", ch );
	return;
    }
    if (IS_CLASS(victim, CLASS_WRAITH))
    {
	send_to_char( "You are unable to affect your brothers in death.\n\r", ch );
	return;
    }
    	if (!IS_AFFECTED(victim, AFF_SHADOWPLANE))
    	{
	    send_to_char("You reach into the material plane and pull $N through.\n\r",ch);
	    act("$n reaches between the planes and pulls $N into the realm of shadows.",ch,NULL,victim,TO_ROOM);
          act("$n reaches through the planes and pulls you into the shadowlands.",ch,NULL,victim,TO_VICT);
	    SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
	    do_look(victim,"auto");
	    return;
    	}
    	REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You push $N through the barrier into the material plane.\n\r",ch);
	act("$n's hands appear shoving $N into the material world.",ch,NULL,victim,TO_ROOM);
      act("$n shoves you incredibly hard forcing you into the material world.",ch,NULL,victim,TO_VICT);
	do_look(victim,"auto");
	return;
}
void do_stonehand( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_OUTR] < 3)
	{
	   send_to_char("You need level 3 in Outrage to use Stonehand Punch.\n\r",ch);
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
  if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE))
  {
    stc("they're too insubstantial.\n\r",ch);
    return;
  }

  if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
  {
    stc("they're too insubstantial.\n\r",ch);
    return;
  }

  if ( victim->hit < 1000 )
  {
    stc("They are almost dead already.  It wouldn't be any fun.\n\r",ch);
    return;
  }

  dam = number_range((ch->pcdata->powers[WRA_OUTR] * 150),(ch->pcdata->powers[WRA_OUTR] *175));
  act("You conjure a massive fist to strike $N.",ch,NULL,victim,TO_CHAR);
  act("$n conjures a massive fist from the earth to strike you.",ch,NULL,victim,TO_VICT);
  mage_damage(ch,victim,dam,"stonehand punch",MAGEDAM_OTHER);
  WAIT_STATE(ch, 16);
  return;
}
void do_pyro(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;

	
	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_OUTR] < 4)
	{
	   send_to_char("You need level 4 in Outrage to use Pyrokinetic weapon.\n\r",ch);
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
void do_obliviate( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

	if (!IS_CLASS(ch, CLASS_WRAITH))
	{
	   send_to_char("Huh?\n\r",ch);
	   return;
	}
	if (ch->pcdata->powers[WRA_OUTR] < 5)
	{
	   send_to_char("You need level 5 in Outrage to use Obliviate!.\n\r",ch);
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

  dam = number_range((ch->pcdata->powers[WRA_OUTR] * 100),(ch->pcdata->powers[WRA_OUTR] *125));
  act("You Obliviate $N with a glowing fist.",ch,NULL,victim,TO_CHAR);
  act("$n punches you with a massive glowing fist.",ch,NULL,victim,TO_VICT);
  mage_damage(ch,victim,dam,"Obliviate",MAGEDAM_OTHER);
  WAIT_STATE(ch, 6);
  return;
}
/*
 *Pandemonium arcanoi
 *1. Weirdness
 *2. Befuddlement
 *3. Ether
 *4. Foulhumor
 *5. Tempus
 */
void do_weirdness( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PAND] < 1 )
    {
      stc("You must obtain level 1 Pandemonium to use Weirdness.\n\r",ch);
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
	act( "You twist your corpus and in a flash of putrid smoke you split in two.", ch, NULL, victim, TO_CHAR );
	act( "$n twists $s corpus and in a flash of putrid smoke, splits in two.", ch, NULL, victim,
TO_ROOM );
	return;

}
void do_befuddlement( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;
    char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PAND] < 2 )
    {
      stc("You must obtain level 2 Pandemonium to use Befuddlement.\n\r",ch);
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
void do_ether( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);

  if ( IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PAND] < 3 )
    {
      stc("You must obtain level 3 Pandemonium to use Dark Ether.\n\r",ch);
      return;
    }

  if ( arg[0] == '\0' )
  {
    stc("How do you want to change the weather?\n     Syntax: ether <rain,lightning,clear>\n\r",ch);
    return;
  }

  WAIT_STATE(ch, 16);
  if ( !str_cmp(arg, "rain" ) )
  {
    stc("You wave your hands over your head.\n\r",ch);
    stc("The skies blacken and rain begins to fall.\n\r",ch);
    weather_info.sky = SKY_RAINING;
  }
  else if ( !str_cmp(arg, "lightning") )
  {
    stc("You strike your hands together.\n\r",ch);
    stc("The skies blacken.\n\r",ch);
    stc("Lightning streaks across the sky.\n\r",ch);
    weather_info.sky = SKY_LIGHTNING;
  }
  else if ( !str_cmp(arg, "clear") )
  {
    stc("You push your hands out from your chest.\n\r",ch);
    stc("The skies slowly clear up.\n\r",ch);
    weather_info.sky = SKY_CLOUDLESS;
  }
  else
  {
    stc("Invalid option.\n   Syntax: ether <rain,lightning,clear>\n\r",ch);
    return;
  }

  return;
}
/* Foulhumor needs to stun before fight and then do damage, swarm type */

void do_tempus( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument(argument, arg);

  if ( IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PAND] < 5 )
    {
      stc("You must obtain level 5 Pandemonium to use Tempus Fugit.\n\r",ch);
      return;
    }

  if ( arg[0] == '\0' )
  {
    stc("Whom do you wish to Call Lightning upon?\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room(ch,arg) ) == NULL )
  {
    stc("You were unable to locate your victim.\n\r",ch);
    return;
  }
 
    if ( victim->level < 3 )
    {
      stc("They must be an avatar!\n\r",ch);
      return;
    }

  if ( weather_info.sky != SKY_LIGHTNING )
  {
    stc("There is no lightning in the sky to call!\n\r",ch);
    return;
  }

  WAIT_STATE(ch, 16);
  dam = number_range( (ch->pcdata->powers[WRA_PAND]*150),(ch->pcdata->powers[WRA_PAND]*200) );
  if ( !IS_NPC(ch) )
    dam -= number_range( 100,150 );

  mage_damage(ch, victim, dam, "lightning blast", MAGEDAM_ELECTRIC);  
  return;
}
/*
 *Phantasm arcanoi
 *1. Elysia *none yet*
 *2. Lucidity *none yet*
 *3. Dreams
 *4. Phantasmagoria
 *5. Agon -If I can do what I want with this ooooh, nasty
 */
/* Elysia nothing figured out yet */
/* Lucidity nothing figured out yet */
void do_dreams( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if (IS_NPC(ch)) return;
	
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PHAN] < 3 )
    {
      stc("You must obtain level 3 Phantasm to use Dream Eater.\n\r",ch);
      return;
    }
	
    if ( arg1[0] == '\0' )
    {
	send_to_char( "put who to sleep?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
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
	act("You throw sand into $N's eyes, putting them to sleep.",ch,NULL,victim,TO_CHAR);
	act("$n throws sand in your eyes.  You feel sleepy.",ch,NULL,victim,TO_VICT);
	victim->position = POS_SLEEPING;
	WAIT_STATE(ch, 24);
	return;
	}
	
	return;
}
void do_phantasmagoria( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;

  if ( IS_NPC(ch) ) return;
 
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PHAN] < 4 )
    {
      stc("You must obtain level 4 Phantasm to use Phantasmagoria.\n\r",ch);
      return;
    }
  
  if ( ch->pcdata->followers > 5 )
  {
    stc("Nothing seems to happen.\n\r",ch);
    return;
  }

  stc("You disappear and in a flash of putrid smoke you return cackling.\n\r",ch);
  act("$n disappears and in a flash of putrid smoke they return cackling.",ch,NULL,NULL,TO_ROOM);

    ch->pcdata->followers++;
    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->name = str_dup("monster");
    victim->short_descr = str_dup("A hideous monster");
    victim->long_descr = str_dup("A multi-limbed monstrosity towers above you.");
    victim->level = 200;
    victim->hit = 5000;
    victim->max_hit = 5000;
    victim->hitroll = 50;
    victim->damroll = 50;
    victim->armor = 300;
    SET_BIT(victim->act, ACT_NOEXP);
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act("$N materializes next to $n.",ch,NULL,victim,TO_ROOM);
    return;
}
void do_agon( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  int dam;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch) ) return;
 
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_PHAN] < 5 )
    {
      stc("You must obtain level 5 Phantasm to use Agon.\n\r",ch);
      return;
    }
  if ( arg[0] == '\0' )
  {
      stc("Whom do you wish to use this on?\n\r",ch);
      return;
  }
    if (ch == victim)
    {
	send_to_char( "You can't do this to yourself.\n\r", ch);
	return;
    }
  if ( ( victim = get_char_room(ch,arg) ) == NULL )
  {
    stc("They aren't here.\n\r",ch);
    return;
  }
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	{
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	act("$n comes flying out of a shadow.",ch,NULL,NULL,TO_ROOM);
	return;
	}
  else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE))
	{
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	act("$n comes flying out of a shadow.",ch,NULL,NULL,TO_ROOM);
	return;
	}
  dam = number_range((ch->pcdata->powers[WRA_PHAN] * 75),(ch->pcdata->powers[WRA_PHAN] *100));
  act("You punch $N straight between planes.",ch,NULL,victim,TO_CHAR);
  act("$n punches you so hard it sends you crashing into another plane.",ch,NULL,victim,TO_VICT);
  mage_damage(ch,victim,dam,"Agon",MAGEDAM_OTHER);
  if (!IS_AFFECTED(victim, AFF_SHADOWPLANE))
	{
	SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
	return;
	}
  else if (IS_AFFECTED(victim, AFF_SHADOWPLANE))
	{
	REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
	return;
	}
  WAIT_STATE(ch, 16);
  return;
}

/*
 *Puppetry arcanoi MUHAHAHAHA!!
 *1. Skinride
 *2. Sudden
 *3. Mastervoice
 *4. Rein
 *5. Obliterate    LXS can bite me - Nephesch
 */

void do_skinride(CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;  
  one_argument(argument,arg);
  one_argument(argument,arg2);

  if((!IS_CLASS(ch,CLASS_WRAITH)|| !IS_HERO(ch)) )
  {do_huh(ch,"");return;}
    if ( ch->pcdata->powers[WRA_PUPP] < 1 )
    {
      stc("You must obtain level 1 Puppetry to use Skinride.\n\r",ch);
      return;
    }   

    if(IS_SET(ch->newbits,WRA_SKINRIDE))
	{if(!str_cmp(arg2,"stop")){
	do_follow(ch,"self");
	REMOVE_BIT(ch->newbits,WRA_SKINRIDE);
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
            if ( d->snoop_by == ch->desc )
                d->snoop_by = NULL;
        }                
	stc("You leave the body of your host behind.\n\r",ch);
	return;}
	 else {stc("You are already skinriding someone!",ch);}
	}


  if(arg[0] == '\0')
  {
    stc("Who do you wish to Skinride?\n\r",ch);
    return;
  }



    if ( ( victim = get_char_room( ch, arg ) ) == NULL )  
    {
      stc("They are not here.\n\r",ch);
      return;
    }

    if (IS_CLASS(victim,CLASS_WRAITH) || IS_IMMORTAL(victim)
	|| IS_NPC(victim))
	{
		stc("You cannot Skinride them!\n\r",ch);
		return;
	}

//    victim->unveil = ch;
   victim->desc->snoop_by = ch->desc;        
   SET_BIT(ch->newbits,WRA_SKINRIDE);
   stc("You enter their body.\n\r",ch);
//    add_follower(ch,victim);
//	do_follow(ch,victim->name);
}


/*void do_skinride( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA     *d;   
    one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }


    if ( ch->pcdata->powers[WRA_PUPP] < 1 )
    {
      stc("You must obtain level 1 Puppetry to use Skinride.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Skinride whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

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
	stop_skinride( ch,"" );
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }
    
    if ( ch->master != NULL )
	stop_skinride( ch,"" );

    add_skinride( ch, victim );
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
    SET_BIT(ch->act, PLR_WIZINVIS);
    return;
}
void add_skinride( CHAR_DATA *ch, char *argument )
{
char arg[MSL];
argument = one_argument(argument,arg);

    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = arg;
    ch->leader        = NULL;

    if ( can_see( master, ch ) ){
    act( "You are now skinriding $N.",  ch, NULL, master, TO_CHAR );
    SET_BIT(ch->newbits,WRA_SKINRIDE);}
    return;
}



void stop_skinride( CHAR_DATA *ch )
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
    act( "You stop skinriding $N.", ch, NULL, ch->master, TO_CHAR    );

    ch->master = NULL;
    ch->leader = NULL;
    return;
}
*/
/* Sudden not in */
/* Mastersvoice not in */
/* Rein not in */
/* Obliterate not in, all 5 going to take a wh8ile but be well worth it */
/*
 *Usury arcanoi
 *1. Transfering
 *2. Charitable
 *3. Withdrawl
 *4. Exchange
 *5. Investment
 */
/* getting bored so Transfer isn't in */
/*
void do_charitable( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MIL];
  
  argument = one_argument( argument, arg );
  
    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
      stc("Huh?\n\r",ch);
      return;
    }

    if ( ch->pcdata->powers[WRA_USUR] < 2 )
    {
      stc("You must obtain level 2 Usury to use Charitable Trust.\n\r",ch);
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

  if ( victim == ch )
  {
    stc("You cannot use this power on yourself!\n\r",ch);
    return;
  }

  if ( victim->hit >= victim->max_hit )
  {
    stc("They are already at full health!\n\r",ch);
    return;
  }
  ch->hit - (ch->pcdata->powers[WRA_USUR] * 1000);
  act("You suck the injury out of $N and into yourself.",ch,NULL,victim,TO_CHAR);
  act("$n lays $s hands on you, and the pain drains away and into $m.",ch,NULL,victim,TO_VICT);
  act("$n lays $s hands on $N, and $N's wounds begin to close as $n begins developing them.",ch,NULL,victim,TO_NOTVICT);
  victim->hit += (ch->pcdata->powers[WRA_USUR] * 1000);
  WAIT_STATE(ch, 12);
  return;
} */
/* Withdrawl not in */
/* Exchange not in */
/* Investment not in - finish these later */
void do_embracesoul( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
/*
    if (!IS_SET(ch->special, SPC_WRAITHPRINCE) && IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char("You haven't been granted permission to embrace the souls of others.\n\r",ch);
	return;
    }

    if (!IS_SET(ch->special, SPC_WRAITHSOUL) && IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char("You haven't been granted embrace the souls of others.\n\r",ch);
	return;
    }
*/
    if ( arg[0] == '\0' )
    {
	send_to_char( "Embrace whom?\n\r", ch );
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
	send_to_char( "You cannot embrace yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_WRAITH))
    {
	send_to_char( "They are already a restless dead.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only enchant avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "They are already dead!\n\r", ch );
	return;
    }
    
	if (IS_CLASS(victim, CLASS_MONK) )
	{
	send_to_char("Their soul is too devoted to leave.\n\r", ch);
	return;
	}

	if (IS_CLASS(victim, CLASS_MAGE) )
	{
	send_to_char("Their avatars prevent them from leaving.\n\r", ch);
	return;
	}

	if (IS_CLASS(victim, CLASS_NINJA) )
	{
	send_to_char("They are too disciplined to leave their guild.\n\r", ch);
	return;
	}

    if (IS_CLASS(victim, CLASS_MOOGLE))
    {
    send_to_char( "Not on moogles!\n\r", ch);
    return;
    }
	
    if (IS_CLASS(victim, CLASS_PALADIN))
    {
    send_to_char( "Their soul is devoted to god.\n\r", ch);
    return;
    }

	if (IS_CLASS(victim, CLASS_DROW)) {
		send_to_char("Not on a drow!\n\r", ch );
		return; }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "Their rage keeps them from joining you!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "They have no soul!\n\r", ch );
	return;
    }


    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "But they are truly immortal.\n\r", ch );
	return;
    }

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot embrace the soul of an unwilling person.\n\r", ch );
	return;
    }

    if (victim->exp < 500000)
    {
	send_to_char("They cannot afford the 500000 exp required to leave their life.\n\r",ch);
	return;
    }

    victim->exp -= 500000;

    act("You embrace $N's soul and form them a corpus.", ch, NULL, victim, TO_CHAR);
    act("$n embraces $N's soul and forms them a corpus.", ch, NULL, victim, TO_NOTVICT);
    act("$n embraces your soul and forms you a corpus.", ch, NULL, victim, TO_VICT);
    send_to_char( "You are now a Wraith, welcome to the ranks of the restless dead!\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->class    = CLASS_WRAITH;
    SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
    SET_BIT(victim->affected_by, AFF_SHADOWSIGHT);
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

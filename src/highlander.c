/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stfeldt, Tom Madsen, and Katja Nyboe.   *
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

void set_fighting args(( CHAR_DATA *ch, CHAR_DATA *victim));

void    lsword                  args( ( CHAR_DATA *ch) );
void    ssword                  args( ( CHAR_DATA *ch) );
void    fsword                  args( ( CHAR_DATA *ch, char *argument) );
void    dsword                  args( ( CHAR_DATA *ch, char *argument) );

/* Coded by Kyrlin */
void do_forge( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int vnum = 29695;
    int weaptype = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_HIGHLANDER(ch))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
    send_to_char( "#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
    send_to_char( "#e=- Syntax#w: forge <modifier> <weapon>#e                                         -=#n\n\r",ch);
    send_to_char( "#e=-                                                                           -=#n\n\r",ch);
    send_to_char( "#e=- Modifiers      Qps            Special            Requirements             -=#n\n\r",ch);
    send_to_char( "#e=- #gGleaming       #c100            #wSilver             None#e                     -=#n\n\r",ch);
    send_to_char( "#e=- #gSharpened      #c250            #wSilver|Sharp       #gStatus #w[#c 5#w]#e              -=#n\n\r",ch);
    send_to_char( "#e=- #gQuickened      #c500            #wSilver|Sharp|Shock #gStatus #w[#c10#w]#e              -=#n\n\r",ch);
    send_to_char( "#e=-                                                                           -=#n\n\r",ch);
    send_to_char( "#e=- Weapon         Type                                                       -=#n\n\r",ch);
    send_to_char( "#e=- #gClaymore       #wSlash#e                                                      -=#n\n\r",ch);
    send_to_char( "#e=- #gKatana         #wSlice#e                                                      -=#n\n\r",ch);
    send_to_char( "#e=- #gKatar          #wStab#e                                                       -=#n\n\r",ch);
    send_to_char( "#e=- #gRapier         #wPierce#e                                                     -=#n\n\r",ch);
    send_to_char( "#e=- #gSickle         #wClaw#e                                                       -=#n\n\r",ch);
    send_to_char( "#e=-                                                                           -=#n\n\r",ch);
    send_to_char( "#e=-                   All weapons cost #c100#e Primal to forge.                   -=#n\n\r",ch);
    send_to_char( "#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
        return;
    }

    if (str_cmp(arg1,"gleaming") && str_cmp(arg1,"sharpened") && str_cmp(arg1,"quickened") )
    {
        do_forge( ch, "" );
        return;
    }
    if      (!str_cmp(arg2,"katana"     )) weaptype = 1;
    else if (!str_cmp(arg2,"katar"      )) weaptype = 2;
    else if (!str_cmp(arg2,"claymore"   )) weaptype = 3;
    else if (!str_cmp(arg2,"sickle"     )) weaptype = 5;
    else if (!str_cmp(arg2,"rapier"     )) weaptype = 11;
    else
    {
        do_forge( ch, "" );
        return;
    }

    if ( ch->practice < 100)
    {
        send_to_char("It costs 100 points of primal to forge a weapon.\n\r",ch);
        return;
    }

    else if (!str_cmp(arg1,"gleaming") && ch->pcdata->quest < 100)
    {
        send_to_char("You need 100 quest points to forge a Gleaming weapon.\n\r", ch);
        return;
    }

    else if (!str_cmp(arg1,"sharpened") && (ch->pcdata->quest < 250 || ch->race < 5) )
    {
        send_to_char("You need 250 quest points and 5 status to forge a Sharpened weapon.\n\r", ch);
        return;
    }

    else if (!str_cmp(arg1,"quickened") && (ch->pcdata->quest < 500 || ch->race < 10) )
    {
        send_to_char("You need 500 quest points and 10 status to forge a Quickened weapon.\n\r", ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Tracker.\n\r",ch);
        return;
    }
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    ch->practice   -= 100;
    if (!str_cmp(arg1,"gleaming"))
    {obj->spectype = SITEM_SILVER;ch->pcdata->quest -= 100;obj->points += 100;}
    if (!str_cmp(arg1,"sharpened"))
    {obj->spectype = SITEM_SILVER;obj->value[0] = 18000;obj->value[1] = 30;obj->value[2] = 50;ch->pcdata->quest -= 250;obj->points += 250;}
    if (!str_cmp(arg1,"quickened"))
    {obj->spectype = SITEM_SILVER;obj->value[0] = 18048;obj->value[1] = 40;obj->value[2] = 50;ch->pcdata->quest -= 500;obj->points += 500;}
    obj->value[3] = weaptype;

    sprintf( buf, obj->name, arg1, arg2 );
    free_string( obj->name );
    obj->name = str_dup( buf );

    sprintf( buf, obj->short_descr, arg1, arg2 );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );

    sprintf( buf, obj->description, arg1, arg2 );
    free_string( obj->description );
    obj->description = str_dup( buf );

    free_string( obj->questmaker );
    obj->questmaker = str_dup( ch->name );

    free_string( obj->questowner );
    obj->questowner = str_dup( ch->name );

    SET_BIT( obj->spectype, SITEM_HIGHLANDER );

    act("You forge $p.",ch,obj,NULL,TO_CHAR);
    act("$n forges $p.",ch,obj,NULL,TO_ROOM);
    return;
}

/* Read Aura and Scry abilites, given at status levels 5 and 10.
 * Added by Nightstar 30-12-99.
 */

void do_sense( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    else if (!IS_CLASS(ch, CLASS_HIGHLANDER) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_HIGHLANDER) && ch->race < 10 )
    {
      send_to_char("You are not powerful enough to do that yet.\n\r", ch);
      return;
    }
    
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

    if (!IS_CLASS(victim, CLASS_HIGHLANDER) )
    {
	send_to_char( "They aren't a highlander.\n\r", ch );
	return;
    }
	
    if ( IS_CLASS(victim, CLASS_HIGHLANDER) )
    {
      act("\"$n senses your whereabouts.\"",ch,NULL,victim,TO_VICT);
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
  return;

}

void do_stare( CHAR_DATA *ch, char *argument )
{
     if (IS_NPC(ch)) return;
     if (!IS_CLASS(ch, CLASS_HIGHLANDER))
     {
        send_to_char("Huh?\n\r",ch);
        return;
     }

     if (ch->race <=9)
     {
 send_to_char(
"You are not powerful enough to stare down opponents.\n\r",ch);
        return;
      }

if (!IS_SET(ch->newbits, STARE_DOWN))
    {
     send_to_char(
"You lock eyes with opponents, intent to fight to the death.\n\r",ch);
    SET_BIT(ch->newbits, STARE_DOWN);
    return;
    }

    else if (IS_SET(ch->newbits, STARE_DOWN))

     {
      send_to_char(
"You turn your gaze aside, no longer bent on duels to the death.\n\r",ch);
   REMOVE_BIT(ch->newbits, STARE_DOWN);
   return;
     }
  return;
}

void do_sixthsense( CHAR_DATA *ch, char *argument )
{   
    if (IS_NPC(ch) ) return;

    if (!IS_HIGHLANDER(ch) )
    {
        send_to_char( "Huh?\n\r", ch );
        return;
    }

    if (ch->race <=14)
    {
        send_to_char( "You are not yet powerful enough to awaken your Sixthsense.\n\r", ch );
        return;
    }

    if ( !IS_SET(ch->flag2, AFF2_SIXTHSENSE) )
    {
        send_to_char( "Your sixthsense kicks in.\n\r", ch );
        SET_BIT(ch->flag2, AFF2_SIXTHSENSE);
        return;
    }
    else
    {
        send_to_char( "You stop paying attention to your Sixthsense.\n\r", ch );
        REMOVE_BIT(ch->flag2, AFF2_SIXTHSENSE);
        return;
    }
    return;
}

void do_blitzattack (CHAR_DATA *ch, char *argument )
{
        //Blitz Attack channels a buncha of the swords together, but is exhausting.
      CHAR_DATA *victim;
        OBJ_DATA *katana;
        OBJ_DATA *katana2;
//      char buf[MAX_STRING_LENGTH];
//      int dmg;
//      int heal;
        int theattacks[5];
        int theattacks2[5];
        int tempvar;
        int storemana;
        bool doubled = FALSE;
        int cost = 12000;
        
        if (IS_NPC(ch)) return;

    if (argument[0] == '\0')
    {
        stc("Who do you want to use blitz on?\n\r", ch);
        return;
    }
 
    if ((victim = get_char_room(ch, argument)) == NULL)
    {
        if (ch->fighting == NULL) {
        send_to_char( "They aren't here.\n\r", ch );
        return;}
        else victim = ch->fighting;
    }


    if (is_safe(ch, victim)) return;
 
    if (ch == victim)
    {
        stc("You can't use blitz on yourself.\n\r", ch);
        return;
    }


        
        katana = get_eq_char(ch, WEAR_WIELD);
        katana2 = get_eq_char(ch, WEAR_HOLD);

        for (tempvar = 0; tempvar < 5; tempvar++)
        {
                theattacks[tempvar] = number_range(1,4);
                theattacks2[tempvar] = number_range(1,4);
        }

        if ((katana2 == NULL) || (katana2->pIndexData->vnum != 29695))
                doubled = FALSE;
        else
                doubled = TRUE;

        if (ch->mana < (cost * 2))
                doubled = FALSE;
        if (ch->move < cost)
                doubled = FALSE;

        if (doubled)            cost *= 2;

        if (ch->mana < cost)
        {
                send_to_char("You have not the 12000 mana to use this.\n\r",ch);
                return;
        }

        if (ch->move < (cost / 2))
        {
                send_to_char("You do not have the 6000 move to use this.\n\r",ch);
                return;
        }

        if ((katana == NULL) || (katana->pIndexData->vnum != 29695))
        {
                send_to_char("Without a proper katana the energies involved will destroy your weapon.\n\r",ch);
                return;
        }
        ch->mana -= 12000;
        ch->move -= 6000;

        send_to_char("You channel mystical energy into your katana.\n\r",ch);
        act("$n channels mystical energy into $s katana.\n\r",ch, NULL, NULL, TO_ROOM);
        send_to_char("Your katana starts to glow as you launch yourself at your opponent.\n\r",ch);
	set_fighting(ch, victim);

        storemana = ch->mana;   
        ch->mana = 10000;

        for (tempvar = 0; tempvar < 5; tempvar++)
        {
                switch (theattacks[tempvar])
                {
                         case 0:
                         case 1: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                         case 2: {      lsword(ch);       break;  }
                         case 3: {      ssword( ch);      break;  }
                         case 4: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                        default: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }

                }
        }
        for (tempvar = 0; tempvar < 5; tempvar++)
        {
                switch (theattacks2[tempvar])
                {
                         case 0:
                         case 1: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                         case 2: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                         case 3: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                         case 4: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                        default: {      multi_hit( ch, victim, TYPE_UNDEFINED );       break;  }
                }
        }
/*
        for (tempvar = 0; tempvar < 5; tempvar++)
        {
                switch (theattacks[tempvar])
                {
                         case 0:
                         case 1: {      do_sword(ch,"lightning");       break;  }
                         case 2: {      do_sword(ch,"fire");            break;  }
                         case 3: {      do_sword(ch,"ice");             break;  }
                         case 4:
                        default: {      do_sword(ch,"drain");           break;  }
                }
        }
        for (tempvar = 0; tempvar < 5; tempvar++)
        {
                switch (theattacks2[tempvar])
                {
                         case 0:
                         case 1: {      do_sword(ch,"lightning");       break;  }
                         case 2: {      do_sword(ch,"fire");            break;  }
                         case 3: {      do_sword(ch,"ice");             break;  }
                         case 4:
                        default: {      do_sword(ch,"drain");           break;  }
                }
        }
*/
        ch->mana = storemana;
        if (doubled)
   {
                send_to_char("Your exhausted and as you watch, one of your katanas melts into a puddle.\n\r",ch);
                obj_from_char( katana2 );
                obj_to_room( katana2, ch->in_room );
                act("$n's $p slides to the floor and as you watch $p starts to melt into a puddle.",ch, katana2, NULL, TO_ROOM);
                extract_obj( katana2 );
        }
        else
                send_to_char("Your exhausted and let your katana sag a little as you gasp for breath.\n\r",ch);

        return;
}

void lsword(CHAR_DATA *ch)
{
        CHAR_DATA *victim;
        OBJ_DATA *katana;
        char *vs;
        char buf[MAX_STRING_LENGTH];
        int dmg;


    if (IS_NPC(ch)) return;

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }


        katana = get_eq_char(ch, WEAR_WIELD);
                
        if (ch->mana < 2500)
        {
                send_to_char("You have not the 2500 mystical power to use this.\n\r",ch);
                return;
        }
         
        if ((katana == NULL) || (katana->pIndexData->vnum != 29695))
        {
                send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
                return;
        }
        ch->mana -= 2500;
        send_to_char("You channel mystical energy into your katana.\n\r",ch);
        do_say(ch,"#b#cL#CI#cG#CH#cT#CN#cI#CN#cG SWORD!#n");
        send_to_char("A humongous bolt of lightning blasts from your katana.\n\r",ch);
        act("A humongous bolt of lightning blasts from $n's katana!", ch, NULL, NULL, TO_ROOM);

        if (number_range(0,100) < 16)
        {
                act("The lightning blast slams into the ground and is absorbed.", ch, NULL, NULL, TO_ROOM);
                return;   
        }
	dmg = number_range(1000,3000);


      if ( dmg ==   0 ) { vs = "#B miss#n";}
    else if ( dmg <=  50 ) { vs = "#B lightly#n";}
    else if ( dmg <= 100 ) { vs = "";}
    else if ( dmg <= 250 ) { vs = "#B hard#n";}
    else if ( dmg <= 500 ) { vs = "#G very hard#n";}
    else if ( dmg <= 750 ) { vs = "#G extremely hard#n";}
    else if ( dmg <= 1000 ){ vs = "#G incredibly hard#n";}
    else if ( dmg <= 1250 ){ vs = "#R unbelievably hard#n";}
    else if ( dmg <= 1500 ){ vs = "#R unpleasantly hard#n";}
    else if ( dmg > 1500 ){ vs = "#R unpleasantly hard#n";}

        sprintf(buf,"Your lightning strikes $N%s![%d]",vs,dmg);
        ADD_COLOUR(ch,buf,L_CYAN);
        act2(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n's ligtning strikes $N%s!",vs);
        ADD_COLOUR(ch,buf,L_CYAN);
        act2(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$n's lightning strikes you%s![%d]",vs,dmg);
        ADD_COLOUR(ch,buf,L_CYAN);
        act2(buf,ch,NULL,victim,TO_VICT);
//        hurt_person(ch,victim,dmg,TRUE);
	 damage(ch,victim,dmg,1 + TYPE_ADD_AGG);

        if (victim->position > POS_STUNNED)
        victim->position = POS_STUNNED;
        return;
}

void ssword (CHAR_DATA *ch)
{
        CHAR_DATA *victim;
        OBJ_DATA *katana;
        char *vs;
        char buf[MAX_STRING_LENGTH];
        int dmg;


    if (IS_NPC(ch)) return;
  if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch ); 
        return;
    }
        katana = get_eq_char(ch, WEAR_WIELD);
                
        if (ch->mana < 2500)
        {
                send_to_char("You have not the 2500 mystical power to use this.\n\r",ch);
                return;
        }
         
        if ((katana == NULL) || (katana->pIndexData->vnum != 29695))
        {
                send_to_char("Without a proper katana the energy will destroy your weapon.\n\r",ch);
                return;
        }
        ch->mana -= 2500;
        send_to_char("You channel mystical energy into your katana.\n\r",ch);
        do_say(ch,"Soul sword!");
        send_to_char("A bolt of light shoots out from your katana.\n\r",ch);
        act("A bolt of light shoots from $n's katana!", ch, NULL, NULL, TO_ROOM);

        if (number_range(0,100) < 16)
        {
                act("Your lightning blast flies past $N and is absorbed by the ground.", ch, NULL, victim, TO_CHAR);
                act("The lightning blast slams into the ground and is absorbed.", ch, NULL, NULL, TO_ROOM);
                return;   
        }
	dmg = number_range(1000,3000);
        act("$N screams as $E is ripped apart by the dark energy. #R[#y#b MORTAL #n#R]#n", ch, NULL, victim, TO_CHAR);
        act("$n looks on as you start screaming while dark energy rips you apart from the inside out.", ch, NULL, victim, TO_VICT);
        send_to_char("You crumple to the ground.\n\r", victim);
        act("$n looks on as $N starts screaming.  Weakened $N crumples to the ground.", ch, NULL, victim, TO_NOTVICT);
        WAIT_STATE(ch, (PULSE_VIOLENCE * 5));
        victim->hit = -10;
	 damage(ch,victim,dmg,1 + TYPE_ADD_AGG);
//        hurt_person(ch, victim, dmg, TRUE);
        return;


}

void do_nsword(CHAR_DATA *ch, char *argument)
{
        char arg[MAX_STRING_LENGTH];
        bool lit = FALSE;
        bool fire = FALSE;
        bool ice = FALSE;
        bool drain = FALSE;
        bool soul = FALSE;
        
        argument = one_argument(argument, arg);
        
        if (IS_NPC(ch)) return;
        
        if (!IS_CLASS(ch, CLASS_HIGHLANDER))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }               
        
        if (!str_cmp(arg, "lightning"))                 lit = TRUE;
        if (!str_cmp(arg, "fire"))                      fire = TRUE;
        if (!str_cmp(arg, "ice"))                       ice = TRUE;
        if (!str_cmp(arg, "drain"))                     drain = TRUE;
        if (!str_cmp(arg, "soul"))                      soul = TRUE;
         
      
        if ((!lit) && (!fire) && (!ice) && (!drain) && (!soul))
        {
                send_to_char("That is not a technique.\n\r",ch);
                return;
        }
        
        if (lit)
	{
                        lsword(ch);
                        return;
        }
        if (soul)
	{
                        ssword(ch);
                        return;
        }
                
}


                
                 
                         
                   

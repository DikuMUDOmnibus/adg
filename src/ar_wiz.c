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
#include <unistd.h>
#include "player.h"

/* This file is primarily for wiz commands tho it may stray a little as
 * I put it together.
 */
/*
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
*/
void do_toggle( CHAR_DATA *ch, char *argument)
{
    char      buf [MAX_STRING_LENGTH];
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char      arg3 [MAX_INPUT_LENGTH];
    int time;
    int timez;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char("#wSyntax:#n toggle <dxp,dtr,dqp,chs,all> <on/off> <mins>.\n\r", ch );
        return;
    }


//    time = is_number (arg3) ? atoi (arg3) : 60;
    time = is_number(arg3) ? atoi(arg3) : 60;
    timez = time * 60 * PULSE_PER_SECOND;

    if ( arg3[0] == '\0' )
    { timez = -1; }

    if (!str_cmp(arg1,"all")) {
      if (!str_cmp(arg2,"on")) {
        dxp_info.dxp = 1;
        dtr_info.dtr = 1;
        chao_info.chs = 1;
        dqp_info.dqp = 1;
        dxp_info.dxpt = timez;
        dtr_info.dtrt = timez;
        chao_info.chst = timez;
        dqp_info.dqpt = timez;
        sprintf(buf,"{W%s {wturns on {BEVERYTHING {wfor{B %.2f %s{w!{x",ch->name,
		  dxp_info.dxpt >= 240 ? (float) dxp_info.dxpt / 60 /
		  PULSE_PER_SECOND : (float) dxp_info.dxpt / PULSE_PER_SECOND,
		  dxp_info.dxpt >= 240 ? "minute(s)" : "second(s)"); do_info(ch,buf); }
      else if (!str_cmp(arg2,"off")) {
        dxp_info.dxp = 0;
        dtr_info.dtr = 0;
        chao_info.chs = 0;
        dqp_info.dqp = 0;
        dxp_info.dxpt = -1;
        dtr_info.dtrt = -1;
        chao_info.chst = -1;
        dqp_info.dqpt = -1;
        sprintf(buf,"{W%s {Rturns off everything!{x",ch->name); do_info(ch,buf); }
       return;
   }

    if (!str_cmp(arg1,"dxp")) {
      if (!str_cmp(arg2,"on")) {
        dxp_info.dxp = 1;
        dxp_info.dxpt = timez;
        sprintf(buf,"{W%s {wturns on {BDOUBLE EXP{w!{x",ch->name); do_info(ch,buf); 
       do_info(NULL,dxp_sprintf(FALSE,TRUE));}
      else if (!str_cmp(arg2,"off")) {
        dxp_info.dxp = 0;
        dxp_info.dxpt = -1;
        sprintf(buf,"{W%s {Rturns off double exp!{x",ch->name); do_info(ch,buf); }
       return;
   }
   else if (!str_cmp(arg1,"dtr")) {
      if (!str_cmp(arg2,"on")) {
        dtr_info.dtr = 1;  
        dtr_info.dtrt = timez;
        sprintf(buf,"{W%s {wturns on {YDOUBLE TRAINING{w!{X",ch->name); do_info(ch,buf);
       do_info(NULL,dtr_sprintf(FALSE,TRUE)); }
      else if (!str_cmp(arg2,"off")) {
        dtr_info.dtr = 0;
        dtr_info.dtrt = -1;  
        sprintf(buf,"{W%s {Rturns off double training!{x",ch->name); do_info(ch,buf); }
       return;
   }
   else if (!str_cmp(arg1,"dqp")) {
      if (!str_cmp(arg2,"on")) {
        dqp_info.dqp = 1;
        dqp_info.dqpt = timez;
        sprintf(buf,"{W%s {wturns on {CDOUBLE QP{w!{x",ch->name); do_info(ch,buf);
       do_info(NULL,dqp_sprintf(FALSE,TRUE)); }
      else if (!str_cmp(arg2,"off")) {
        dqp_info.dqp = 0;
        dqp_info.dqpt = -1;
        sprintf(buf,"{W%s {Rturns off double quest points!{x",ch->name); do_info(ch,buf); }
       return;
   }
   else if (!str_cmp(arg1,"chs")) {
      if (!str_cmp(arg2,"on")) {
        chao_info.chs = 1;
        chao_info.chst = timez;
        sprintf(buf,"{W%s {wturns on #.C#.H#.A#.O#.S{w!{x",ch->name); do_info(ch,buf);
       do_info(NULL,chs_sprintf(FALSE,TRUE)); }
      else if (!str_cmp(arg2,"off")) {
        chao_info.chs = 0;
        chao_info.chst = -1;
        sprintf(buf,"{W%s {wturns off {Rchaos{w!{x",ch->name); do_info(ch,buf); }
       return;
   }
   else
   { stc("Not a valid toggle. Try again.\n\r",ch); return;}
  return;
}
void do_class( CHAR_DATA *ch, char *argument )
{
/* Changed by Toaster to add Ninja/Monk 19/01/00
 * Changed by Kyrlin to fix class syntax and to add obeah to class 'none' 31/01/00
 * Changed by Kyrlin to add haloofsun to class 'none' 01/02/00
 * Changed by Kyrlin to add in colour 03/02/00
 * Changed by Kyrlin to set demons correctly 03/02/00
 * Changed by Kyrlin to tweek demon settings 04/02/00
 * Changed by Kyrlin to set vamp lord to "Caine", set ww lord to "Gaia",
 * set demon lord to "lucifer", set drow lord to "lloth", all classed
 * vamps & ww's are 3rd gen, added return messages to setter. 04/02/00
 * Changed by Kyrlin to fix classing messages. 11/02/00
 */
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char("#wSyntax:#n class <char> <class>.\n\r", ch );
        send_to_char("#wClasses:#n\n\r", ch );
        send_to_char("None, ", ch);
        send_to_char("Abomination, ", ch);
/*        send_to_char("Angel, ", ch); */
        send_to_char("Demon, Dchamp, Dlord, ", ch);
        send_to_char("Highlander, ", ch);
        send_to_char("Hunter, \n\r", ch);
        send_to_char("#PPmage#n, #rRmage#n, #bBmage#n, #gGmage#n, #yYmage#n, ", ch);
        send_to_char("Shakrin, ",ch);
        send_to_char("Vampire, ", ch);
        send_to_char("Werewolf,\n\r", ch);
        send_to_char("Wraith, ", ch);
        send_to_char("Ninja, ", ch);
        send_to_char("Drow, ", ch);
        send_to_char("Monk.\n\r", ch);
        send_to_char("Fae.\n\r", ch);
	return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    if((!IS_DEMON(victim) || !IS_MAGE(victim)) &&  victim->class != 0 &&
	str_cmp(arg2, "none"))
    {
	stc("You must clear their class first.\n\r", ch);
	stc("Class <victim> none.\n\r", ch);
	return;
    }
         if ( !str_cmp( arg2, "none"       ) ) 
    {
    /* Used to make sure the person has enough "stuff" to change */
	victim->pcdata->condition[COND_THIRST] = 10000;
	victim->move = 10000;
	victim->mana = 10000;

	victim->pcdata->powers[WPOWER_BEAR]   = 4;
        victim->pcdata->powers[WPOWER_LYNX]   = 4;
	victim->pcdata->powers[WPOWER_BOAR]   = 4;
	victim->pcdata->powers[WPOWER_OWL]    = 4;
	victim->pcdata->powers[WPOWER_SPIDER] = 4; 
	victim->pcdata->powers[WPOWER_WOLF]   = 4;
	victim->pcdata->powers[WPOWER_HAWK]   = 4;
	victim->pcdata->powers[WPOWER_MANTIS] = 4;
	victim->pcdata->powers[WPOWER_TIGER]  = 4;
	victim->pcdata->powers[WPOWER_GAIA]   = 4;

	if ( IS_CLASS(victim, CLASS_WEREWOLF)
	     || IS_CLASS(victim, CLASS_VAMPIRE) ) do_unwerewolf(victim,"");
	if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
        victim->pcdata->stats[UNI_FORM0] = 0;
	free_string( victim->clan ); 
/* All classes in general */   
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
	send_to_char("Your fangs slide back into your gums.\n\r",victim);
	act("$n's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	    send_to_char("Your claws slide back under your nails.\n\r",victim);
	    act("$N's claws slide back under $s nails.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
	}
	else if (IS_CLASS(victim, CLASS_NINJA) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
 	{
	 	send_to_char("You shove your iron claws up your sleeves\n\r",victim);
	    act("$N shoves $S iron claws up $e sleeves.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    else if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
    {
    	send_to_char("Your talons slide back into your fingers.\n\r",victim);
	    act("$N's talons slide back into $s fingers.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, AFF_SHADOWSIGHT);
    }
    
    if (IS_ITEMAFF(victim,ITEMA_SUNSHIELD) )
    {
        send_to_char("Your Halo of the Sun gradually fades away.\n\r",victim);
        REMOVE_BIT(victim->itemaffect, ITEMA_SUNSHIELD);
    }

    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
    
    if (IS_VAMPAFF(victim,VAM_DISGUISED) )
    {
     free_string( victim->morph );
     victim->morph = str_dup( victim->name );
     send_to_char("You transform into yourself.\n\r", victim);
     REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
	 send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(ch,"auto");
    }

    if(!IS_IMMORTAL(victim))
    {	
    	if(IS_SET(victim->act, PLR_WIZINVIS) )
    	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You slowly fade into existence.\n\r", victim );
    	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 REMOVE_BIT(victim->act, PLR_DROWSIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}
   }
	
/* Demon Stuff */
    if (IS_DEMAFF(victim,DEM_HORNS) && IS_CLASS(victim, CLASS_DEMON) )
    {
	 send_to_char("Your horns slide back into your head.\n\r",victim);
	 act("$N's horns slide back into $s head.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	}
	
	if (IS_DEMAFF(victim,DEM_HOOVES) && IS_CLASS(victim, CLASS_DEMON) )
    {
	 send_to_char("Your hooves transform into feet.\n\r",victim);
	 act("$N's hooves transform back into $s feet.", ch, NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    }
/* Changed by Toaster 11/02/00 to remove extra arms after 'class self
none'
   if (IS_DEMAFF(victim,DEM_GRAFT) && IS_CLASS(victim, CLASS_DEMON) ) 
    {
        send_to_char("You loose 2 arms.\n\r",victim);
        act("$N's extra arms fall away. $s looks shocked.", ch, NULL, victim, TO_ROOM);
        REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_GRAFT); 
    }
*/
    if (IS_DEMAFF(victim,DEM_WINGS) && IS_CLASS(victim, CLASS_DEMON) )
    {
	if (IS_DEMAFF(victim,DEM_UNFOLDED) && IS_CLASS(victim, CLASS_DEMON) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",victim);
	    act("$N's wings fold up behind $s back.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
	send_to_char("Your wings slide into your back.\n\r",victim);
	act("$N's wings slide into $s back.", ch, NULL, victim, TO_ROOM);
	REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    }
    
	if ( IS_EXTRA(victim, EXTRA_OSWITCH) ) 
	{
	 do_humanform(victim,"");
	}
	
	REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH); 
	REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	REMOVE_BIT(victim->immune, IMM_SHIELDED); 
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
      REMOVE_BIT(victim->affected_by, AFF_SHADOWSIGHT);   
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_PROTEAN);  
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_VICISSITUDE);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CELERITY);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FORTITUDE);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_POTENCE);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_OBTENEBRATION);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SERPENTIS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_AUSPEX);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DOMINATE);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_PRESENCE);
      REMOVE_BIT(victim->newbits, NEW_OBEAH);
      REMOVE_BIT(victim->polyaff, POLY_ZULO);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DISGUISED);  
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
      REMOVE_BIT(victim->polyaff, POLY_BAT);
      REMOVE_BIT(victim->polyaff, POLY_MIST);
      REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
      REMOVE_BIT(victim->extra, EXTRA_FAKE_CON);         
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_MORTAL);
      REMOVE_BIT(victim->immune, IMM_SHIELDED);
      REMOVE_BIT(victim->polyaff, POLY_SERPENT);
      REMOVE_BIT(victim->special, SPC_WOLFMAN);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
      REMOVE_BIT(victim->newbits, NEW_NOPAIN);
      REMOVE_BIT(ch->newbits, NEW_NEUTRAL);
      REMOVE_BIT(victim->special, SPC_DEMON_LORD);

	REMOVE_BIT(victim->warp, WARP_CBODY);
      REMOVE_BIT(victim->warp, WARP_SBODY);
	REMOVE_BIT(victim->warp, WARP_STRONGARMS);
	REMOVE_BIT(victim->warp, WARP_STRONGLEGS);
	REMOVE_BIT(victim->warp, WARP_VENOMTONG);
	REMOVE_BIT(victim->warp, WARP_SPIKETAIL);
	REMOVE_BIT(victim->warp, WARP_BADBREATH);
	REMOVE_BIT(victim->warp, WARP_QUICKNESS);
	REMOVE_BIT(victim->warp, WARP_STAMINA);
	REMOVE_BIT(victim->warp, WARP_HUNT);
	REMOVE_BIT(victim->warp, WARP_DAMCAP);
	REMOVE_BIT(victim->warp, WARP_TERROR);
	REMOVE_BIT(victim->warp, WARP_REGEN);
	REMOVE_BIT(victim->warp, WARP_STEED);
	REMOVE_BIT(victim->warp, WARP_WEAPON);
	REMOVE_BIT(victim->warp, WARP_INFIRMITY);
	REMOVE_BIT(victim->warp, WARP_GBODY);
	REMOVE_BIT(victim->warp, WARP_SCARED);
	REMOVE_BIT(victim->warp, WARP_MAGMA);
	REMOVE_BIT(victim->warp, WARP_WEAK);
	REMOVE_BIT(victim->warp, WARP_SLOW);
	REMOVE_BIT(victim->warp, WARP_VULNER);
	REMOVE_BIT(victim->warp, WARP_SHARDS);
	REMOVE_BIT(victim->warp, WARP_WINGS);
	REMOVE_BIT(victim->warp, WARP_CLUMSY);
	REMOVE_BIT(victim->warp, WARP_STUPID);
	REMOVE_BIT(victim->warp, WARP_SPOON);
	REMOVE_BIT(victim->warp, WARP_FORK);
	REMOVE_BIT(victim->warp, WARP_KNIFE);

      REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
      REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_TAIL);
      REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
      REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
      REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
      REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
      REMOVE_BIT(victim->extra, EXTRA_OSWITCH);
      REMOVE_BIT(victim->newbits, NEW_DROWHATE);
      REMOVE_BIT(victim->polyaff, POLY_SPIDERFORM);
      REMOVE_BIT(victim->newbits, NEW_DARKNESS);
      REMOVE_BIT(victim->monkstuff, MONK_DEATH);
      REMOVE_BIT(victim->monkstuff, MONK_HEAL);
      REMOVE_BIT(victim->garou1, WOLF_COCOON);
      REMOVE_BIT(victim->newbits, NEW_QUILLS);
      REMOVE_BIT(victim->garou1, WOLF_RAZORCLAWS);
      REMOVE_BIT(victim->garou1, WW_GRANITE);
      REMOVE_BIT(victim->garou1, GAROU_CLENCHED);
      REMOVE_BIT(victim->garou1, GAROU_AVENGER);
      REMOVE_BIT( victim->affected_by, AFF_STEELSHIELD );
      REMOVE_BIT(victim->newbits, NEW_MONKFLAME);
      REMOVE_BIT(victim->newbits, NEW_MONKADAM);
      REMOVE_BIT(victim->newbits, NEW_POWER);
//      REMOVE_BIT(victim->act, PLR_WIZINVIS);
      REMOVE_BIT(victim->extra, EXTRA_STANCE);                             
      REMOVE_BIT(victim->act, OLD_PLR_DEMON);
      REMOVE_BIT(victim->act, OLD_PLR_CHAMPION);
      REMOVE_BIT(victim->extra, OLD_EXTRA_SIRE);
      REMOVE_BIT( victim->extra, OLD_EXTRA_PRINCE );
      REMOVE_BIT(victim->extra, OLD_EXTRA_PRINCE);
      REMOVE_BIT(victim->act, OLD_PLR_WEREWOLF);
      REMOVE_BIT(victim->act, OLD_PLR_WOLFMAN);
      REMOVE_BIT(victim->extra, OLD_EXTRA_SIRE);
      REMOVE_BIT( victim->extra, OLD_EXTRA_PRINCE );
      REMOVE_BIT( victim->extra, OLD_EXTRA_ANARCH );
      REMOVE_BIT( victim->extra, OLD_EXTRA_INCONNU );
      REMOVE_BIT(victim->act, OLD_PLR_VAMPIRE);
      REMOVE_BIT(victim->extra, OLD_EXTRA_HIGHLANDER);
      REMOVE_BIT( victim->extra, OLD_EXTRA_PRINCE );
      REMOVE_BIT( victim->extra, OLD_EXTRA_ANARCH );
      REMOVE_BIT( victim->extra, OLD_EXTRA_INCONNU );
      REMOVE_BIT(victim->act, OLD_PLR_VAMPIRE);
      REMOVE_BIT(victim->extra, EXTRA_POTENCY);
      REMOVE_BIT(victim->newbits, NEW_TIDE);
      REMOVE_BIT(victim->flag2, AFF2_SPIRITGUARD);
      REMOVE_BIT(victim->flag2, AFF2_IMP);
      REMOVE_BIT(victim->flag2, AFF2_SIXTHSENSE);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_HORNS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_WINGS);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_EXOSKELETON);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_TAIL);
      REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_HEAD);
      REMOVE_BIT(victim->flag2, VAMP_OBJMASK);          
      REMOVE_BIT(victim->extra, EXTRA_AWE);   
      REMOVE_BIT(victim->extra,EXTRA_DRAGON);
      REMOVE_BIT(victim->extra, EXTRA_PLASMA);
/*Added by Leon for Highlander Stare*/
      REMOVE_BIT(victim->newbits, STARE_DOWN);
      REMOVE_BIT(victim->newbits, NEW_COIL);
      REMOVE_BIT(victim->affected_by, AFF_SHIFT);
      REMOVE_BIT(victim->extra, EXTRA_EARTHMELD);
      REMOVE_BIT(victim->extra, EXTRA_ASH);
      REMOVE_BIT(victim->affected_by, AFF_SHADOWSIGHT);
      REMOVE_BIT(victim->mflags, MAGE_POLYAPPEAR);
      REMOVE_BIT(victim->mflags, MAGE_TELEKINETICS);
      REMOVE_BIT(victim->mflags, MFLAGS_DIMMAK);
      REMOVE_BIT(victim->mflags, MAGE_BETTERBODY);   
      REMOVE_BIT(victim->mflags, MAGE_SHAPED); 
	
	REMOVE_BIT(victim->special, SPC_CHAMPION );
	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
      REMOVE_BIT(victim->special, SPC_SIRE);
      REMOVE_BIT(victim->special, SPC_PRINCE);
      REMOVE_BIT(victim->special, SPC_DEMON_LORD);
      REMOVE_BIT(victim->special, SPC_DROW_WAR );
      REMOVE_BIT(victim->special, SPC_DROW_MAG );
      REMOVE_BIT(victim->special, SPC_DROW_CLE );
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
      REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	REMOVE_BIT(ch->newbits, NEW_POWER);
	victim->warpcount = 0;
	victim->monkstuff = 0;
	victim->monkcrap = 0;
      victim->spheres[MCOR] = 0;			
      victim->monkab[SPIRIT] = 0;
      victim->monkab[BODY] = 0; 
      victim->monkab[COMBAT] = 0;
      victim->monkab[AWARE] = 0;
      victim->spheres[MLIF] = 0;			
      victim->spheres[MPRI] = 0;			
      victim->spheres[MENT] = 0;			
      victim->spheres[MMIN] = 0;			
      victim->spheres[MSPI] = 0;			
      victim->spheres[MFOR] = 0;			
      victim->spheres[MMAT] = 0;			
      victim->spheres[MTIM] = 0;

	victim->pcdata->powers[DPOWER_FLAGS] = 0;
  	victim->pcdata->stats[DEMON_TOTAL] = 0;
	victim->pcdata->stats[DEMON_CURRENT] = 0;
	victim->pcdata->stats[DEMON_MAGIC] = 0;
	victim->pcdata->powers[WPOWER_BEAR]   = 0;
        victim->pcdata->powers[WPOWER_LYNX]   = 0;
	victim->pcdata->powers[WPOWER_BOAR]   = 0;
	victim->pcdata->powers[WPOWER_OWL]    = 0;
	victim->pcdata->powers[WPOWER_SPIDER] = 0; 
	victim->pcdata->powers[WPOWER_WOLF]   = 0;
	victim->pcdata->powers[WPOWER_HAWK]   = 0;
	victim->pcdata->powers[WPOWER_MANTIS] = 0;
	victim->pcdata->powers[WPOWER_TIGER] = 0;
	victim->pcdata->powers[WPOWER_GAIA] = 0;
  	victim->pcdata->powers[VAM_ANIM]   = 0;
      victim->gifts[STARGAZERS] = 0; 
      victim->gifts[HOMID] = 0;      	
      victim->gifts[METIS] = 0;     	
      victim->gifts[LUPUS] = 0; 
      victim->gifts[RAGABASH] = 0; 	
      victim->gifts[THEURGE] = 0; 		
      victim->gifts[PHILODOX] = 0; 	
      victim->gifts[GALLIARD] = 0; 	
      victim->gifts[AHROUN] = 0; 		
      victim->gifts[BLACKFURIES] = 0; 	
      victim->gifts[BONEGNAWERS] = 0; 	
      victim->gifts[CHILDREN] = 0; 	
      victim->gifts[CHILDRENOFGAIA] = 0;   
      victim->gifts[FIANNA] = 0; 		
      victim->gifts[GETOFFENRIS] = 0; 	
      victim->gifts[GLASSWALKERS] = 0; 	
      victim->gifts[REDTALONS] = 0; 	
      victim->gifts[SHADOWLORDS] = 0; 	
      victim->gifts[SILENTSTRIDERS] = 0; 	
      victim->gifts[SILVERFANGS] = 0; 	
      victim->gifts[UKTENA] = 0; 		
      victim->gifts[WENDIGO] = 0; 		
victim->pcdata->powersde[VAM_OBTE]   = 0;
victim->pcdata->powersde[VAM_THAU]   = 0;
victim->pcdata->powersde[VAM_OBFU]   = 0;
victim->pcdata->powersde[VAM_SERP]   = 0;
victim->pcdata->powersde[VAM_CHIM]   = 0;
victim->pcdata->powersde[VAM_NECR]   = 0;
victim->pcdata->powersde[VAM_CELE]   = 0;
victim->pcdata->powersde[VAM_PRES]   = 0;
victim->pcdata->powersde[VAM_AUSP]   = 0;
victim->pcdata->powersde[VAM_POTE]   = 0;
victim->pcdata->powersde[VAM_VICI]   = 0;
victim->pcdata->powersde[VAM_OBEA]   = 0;
victim->pcdata->powersde[VAM_FORT]   = 0;
victim->pcdata->powersde[VAM_QUIE]   = 0;
victim->pcdata->powersde[VAM_DOMI]   = 0;
victim->pcdata->powersde[VAM_PROT]   = 0;
victim->pcdata->powersde[VAM_DAIM]   = 0;
victim->pcdata->powersde[VAM_MELP]   = 0;
victim->pcdata->powersde[VAM_THAN]   = 0;

        victim->pcdata->powerss[NPOWER_SORA]     = 0;
  	victim->pcdata->powerss[NPOWER_CHIKYU]   = 0;
	victim->pcdata->powerss[NPOWER_NINGENNO] = 0;

	victim->pcdata->condition[COND_THIRST] = 0;
	victim->move = victim->max_move;
	victim->mana = victim->max_mana;
	victim->hit = victim->max_hit;
	victim->pcdata->rank = 0;
	
	victim->pcdata->stats[UNI_RAGE] = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->beast = 15;

        victim->pcdata->principles[0] = 0;  
        victim->pcdata->principles[1] = 0; 
        victim->pcdata->principles[2] = 0; 
        victim->pcdata->principles[3] = 0; 
        victim->pcdata->principles[4] = 0; 
        victim->pcdata->principles[5] = 0; 
        victim->pcdata->principles[6] = 0;  
        victim->pcdata->principles[7] = 0; 
        victim->pcdata->principles[8] = 0;
        victim->pcdata->principles[9] = 0;
        victim->pcdata->principles[10] = 0;
        victim->pcdata->principles[11] = 0;
        victim->pcdata->principles[12] = 0;
        victim->pcdata->principles[13] = 0;
        victim->pcdata->principles[14] = 0;
        victim->pcdata->principles[15] = 0;
        victim->pcdata->principles[16] = 0;
        victim->pcdata->principles[17] = 0;
        victim->pcdata->principles[18] = 0;
        victim->pcdata->principles[19] = 0;

    
    victim->class = 0;
    if ( !IS_IMMORTAL(victim) )
    {
       victim->trust = 0;
       victim->level = 2;
    }
    clear_stats(victim);
    if (victim == ch)
    {
        send_to_char("You are classless now.\n\r", victim);
    }
    else
    {
        send_to_char("They are classless now.\n\r", ch);
        send_to_char("You are classless now.\n\r", victim);
    }
    }
    else if ( !str_cmp( arg2, "demon"      ) ) 
    {
        if(IS_CLASS(victim, CLASS_DEMON))
        {
            if(IS_SET(victim->special, SPC_DEMON_LORD))
            {
                send_to_char("They must be a Demon Champion first.\n\r", ch);
                return;
            }
            else if(IS_SET(victim->special, SPC_CHAMPION))
            {
                REMOVE_BIT(victim->special, SPC_CHAMPION);
	    if( victim == ch)
	    {
                send_to_char("You are now a Demon.\n\r", victim);
	    }

             else send_to_char("They have been reset to Demon.\n\r",ch);
                return;
            }
            else
            {
                send_to_char("They are already a Demon.\n\r", ch);
                return;
            }
        }
        else
        {
            victim->class = 1;
            if (!IS_IMMORTAL(victim) )
                victim->level = 3;
            victim->lord=str_dup("Lucifer");
            if( victim == ch)
            {   
                send_to_char("You are now a Demon.\n\r", victim);
            }               
            else
            {
                send_to_char("They are now a Demon.\n\r",ch);
                send_to_char("You are now a Demon.\n\r", victim);
            }
        }
    }
    else if ( !str_cmp( arg2, "dchamp"      ) ) 
    { 
        if(IS_SET(victim->special, SPC_CHAMPION))
        {
            send_to_char("They are already a Demon Champion.\n\r", ch);
            return;
        }
        else if(!IS_CLASS(victim, CLASS_DEMON))
        {
            send_to_char("They must be a Demon first.\n\r", ch);
            return;
        }
        else if(IS_SET(victim->special, SPC_DEMON_LORD))
        {
            REMOVE_BIT(victim->special, SPC_DEMON_LORD);
            SET_BIT( victim->special, SPC_CHAMPION );
            send_to_char("They have been reset to Demon Champion.\n\r", ch);
            send_to_char("You are now a Demon Champion.\n\r", victim);
            return;
        }
        else
        { 
            SET_BIT( victim->special, SPC_CHAMPION );
            if( victim == ch)
            {   
                send_to_char("You are now a Demon Champion.\n\r", victim);
            }               
            else
            {
                send_to_char("They are now a Demon Champion.\n\r",ch);
                send_to_char("You are now a Demon Champion.\n\r", victim);
            }
        }
    }
    else if ( !str_cmp( arg2, "dlord"      ) ) 
    {
        if(IS_SET(victim->special, SPC_DEMON_LORD))
        {
            send_to_char("They are already a Demon Lord.\n\r", ch);
            return;
        }
        else if(!IS_CLASS(victim, CLASS_DEMON))
        {
            send_to_char("They must be a Demon first.\n\r", ch);
            return;
        }
        else if(!IS_SET(victim->special, SPC_CHAMPION))
        {
            send_to_char("They must be a Demon Champion first.\n\r", ch);
            return;
        }
        else
        {
            SET_BIT( victim->special, SPC_DEMON_LORD );
            REMOVE_BIT(victim->special, SPC_CHAMPION);
            if( victim == ch)
            {   
                send_to_char("You are now a Demon Lord.\n\r", victim);
            }               
            else
            {
                send_to_char("They are now a Demon Lord.\n\r",ch);
                send_to_char("You are now a Demon Lord.\n\r", victim);
            }
        }
     }
/*
    else if ( !str_cmp( arg2, "angel"         ) )
    {
        victim->class = CLASS_ANGEL;
        if( victim == ch)
        {
            send_to_char("You are now a Angel.\n\r", victim);
        }
        else
        {
            send_to_char("They are now an Angel.\n\r",ch);
	      send_to_char("You are now an Angel.\n\r", victim);
        }
    }
*/
    else if ( !str_cmp( arg2, "dragon"         ) )
    {
        victim->class = CLASS_DRAGON;
        if( victim == ch)
        {
            send_to_char("You are now a Dragon.\n\r", victim);
        }   
        else
        {
            send_to_char("They are now an Dragon.\n\r",ch);
              send_to_char("You are now an Dragon.\n\r", victim);
        }
    }      

 else if ( !str_cmp( arg2, "ninja"      ) )
    {   
     victim->class = 128;

/* Reset ninja powers */

	victim->pcdata->powerss[NPOWER_SORA] = 0;
	victim->pcdata->powerss[NPOWER_CHIKYU] = 0;
	victim->pcdata->powerss[NPOWER_NINGENNO] = 0;
	victim->pcdata->powerss[NINJA_KI] = 0;

/* Reset the new ninja stuff */

        victim->pcdata->principles[0] = 0;
        victim->pcdata->principles[1] = 0;
        victim->pcdata->principles[2] = 0;
        victim->pcdata->principles[3] = 0;
        victim->pcdata->principles[4] = 0;
        victim->pcdata->principles[5] = 0;
        victim->pcdata->principles[6] = 0;
        victim->pcdata->principles[7] = 0;
        victim->pcdata->principles[8] = 0;
        victim->pcdata->principles[9] = 0;
        victim->pcdata->principles[10] = 0;
        victim->pcdata->principles[11] = 0;
        victim->pcdata->principles[12] = 0;
        victim->pcdata->principles[13] = 0;
        victim->pcdata->principles[14] = 0;
        victim->pcdata->principles[15] = 0;
        victim->pcdata->principles[16] = 0;
        victim->pcdata->principles[17] = 0;
        victim->pcdata->principles[18] = 0;   
        victim->pcdata->principles[19] = 0;


    if ( victim->trust > 6 )
     {
       victim->trust = victim->trust;
       victim->level = victim->level;
     }
     else
     {
     victim->trust = 6;
     victim->level = 6;
     }
     send_to_char("You are now a ninja!\n\r", victim);

    }

 else if ( !str_cmp( arg2, "drow"      ) )
    {   
     victim->class = 32;
    if ( victim->trust > 6 )
     {
       victim->trust = victim->trust;
       victim->level = victim->level;
     }
     else
     {
     victim->trust = 6;
     victim->level = 6;
     }
     send_to_char("You are now a drow!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "monk"      ) )
    {
     victim->class = 512;
    if ( victim->trust > 6 )
     {
       victim->trust = victim->trust;
       victim->level = victim->level;
     }
     else
     {
     victim->trust = 6;
     victim->level = 6;
     }
     send_to_char("You are now a monk!\n\r", victim);
    }
    
    else if ( !str_cmp( arg2, "wraith"         ) )
    {
        victim->class = CLASS_WRAITH;
        if( victim == ch)
        {
            send_to_char("You are now a Wraith.\n\r", victim);
        }
        else
        {
            send_to_char("They are now a Wraith.\n\r",ch);
            send_to_char("You are now a Wraith.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "fae"         ) )
    {
        victim->class = CLASS_FAE;
        if( victim == ch)
        {
            send_to_char("You are now a Fae.\n\r", victim);
        }
        else
        {
            send_to_char("They are now a Fae.\n\r",ch);
            send_to_char("You are now a Fae.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "deathknight"         ) )
    {
        victim->class = CLASS_DEATHKNIGHT;
        if( victim == ch)
        {
            send_to_char("You are now a Death Knight\n\r", victim);
        }
        else
        {
            send_to_char("They are now a Death knight.\n\r",ch);
            send_to_char("You are now a Death Knight.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "shakrin" 	) )
    {
        victim->class = CLASS_SHAKRIN;
        if( victim == ch)
        {   
            send_to_char("You are now a Shakrin.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a Shakrin.\n\r",ch);
            send_to_char("You are now a Shakrin.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "highlander" ) ) 
    {
        victim->class = 16;
        if(!IS_IMMORTAL(victim) )
            victim->level = 3;
        if( victim == ch)
        {   
            send_to_char("You are now a Highlander.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a Highlander.\n\r",ch);
            send_to_char("You are now a Highlander.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "hunter" ) )
    {
      victim->class = CLASS_HUNTER;
      if(!IS_IMMORTAL(victim) )
        victim->level = 3;
      if( victim == ch)
      {
        stc("You are now a Hunter.\n\r", victim);
      }
      else
      {
        stc("They are now a Hunter.\n\r", ch);
        stc("You are now a Hunter.\n\r", victim);
      }
    }

    else if ( !str_cmp( arg2, "pmage"     ) )
    {	
        victim->class = 2;
        if ( victim->trust > 6 )
        {
            victim->trust = victim->trust;
            victim->level = victim->level;
        }
        else
        {
            victim->trust = 4;
            victim->level = 4;
        }
        victim->pcdata->powersm[MPOWER_RUNE0] = 0;
        victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
        victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
        victim->pcdata->powersm[MPOWER_RUNE3] = 15;    
        if( victim == ch)
        {   
            send_to_char("You are now a #PPurple#n Mage.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a #PPurple#n Mage.\n\r",ch);
            send_to_char("You are now a #PPurple#n Mage.\n\r", victim);
        }
    }
    else if ( !str_cmp( arg2, "rmage"     ) )
    {
        victim->class = 2;
        if ( victim->trust > 6 )
        {
            victim->trust = victim->trust;
            victim->level = victim->level;
        }
        else
        {
            victim->trust = 4;
            victim->level = 4;
        }
        victim->pcdata->powersm[MPOWER_RUNE0] = 1;
        victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
        victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
        victim->pcdata->powersm[MPOWER_RUNE3] = 15;    
        if( victim == ch)
        {   
            send_to_char("You are now a #rRed#n Mage.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a #rRed#n Mage.\n\r",ch);
            send_to_char("You are now a #rRed#n Mage.\n\r", victim);
        }
    }
    else if ( !str_cmp( arg2, "bmage"     ) )
    {
        victim->class = 2;
        if ( victim->trust > 6 )
        {
            victim->trust = victim->trust;
            victim->level = victim->level;
        }
        else
        {
            victim->trust = 4;
            victim->level = 4;
        }
        victim->pcdata->powersm[MPOWER_RUNE0] = 2;
        victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
        victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
        victim->pcdata->powersm[MPOWER_RUNE3] = 15;
        if( victim == ch)
        {   
            send_to_char("You are now a #bBlue#n Mage.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a #bBlue#n Mage.\n\r",ch);
            send_to_char("You are now a #bBlue#n Mage.\n\r", victim);
        }
    }
    else if ( !str_cmp( arg2, "gmage"     ) )
    {
        victim->class = 2;
        if ( victim->trust > 6 )
        {
            victim->trust = victim->trust;
            victim->level = victim->level;
        }
        else
        {
            victim->trust = 4;
            victim->level = 4;
        }
        victim->pcdata->powersm[MPOWER_RUNE0] = 3;
        victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
        victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
        victim->pcdata->powersm[MPOWER_RUNE3] = 15;
        if( victim == ch)
        {   
            send_to_char("You are now a #gGreen#n Mage.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a #gGreen#n Mage.\n\r",ch);
            send_to_char("You are now a #gGreen#n Mage.\n\r", victim);
        }
    }
    else if ( !str_cmp( arg2, "ymage"     ) )
    {
        victim->class = 2;
        if ( victim->trust > 6 )
        {
            victim->trust = victim->trust;
            victim->level = victim->level;
        }
        else
        {
            victim->trust = 4;
            victim->level = 4;
        }
        victim->pcdata->powersm[MPOWER_RUNE0] = 4;
        victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
        victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
        victim->pcdata->powersm[MPOWER_RUNE3] = 15;    
        if( victim == ch)
        {   
            send_to_char("You are now a #yYellow#n Mage.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a #yYellow#n Mage.\n\r",ch);
            send_to_char("You are now a #yYellow#n Mage.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "vampire"    ) ) 
    {
        victim->class = 8;
        if(!IS_IMMORTAL(victim) )
            victim->level = 3;
        victim->pcdata->stats[UNI_GEN] = 3;
        victim->lord=str_dup("Caine");
        if( victim == ch)
        {   
            send_to_char("You are now a Vampire.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now a Vampire.\n\r",ch);
            send_to_char("You are now a Vampire.\n\r", victim);
        }
    }

    else if ( !str_cmp( arg2, "werewolf"   ) ) 
    {
        victim->class = 4;
        if(!IS_IMMORTAL(victim) )
            victim->level = 3;
        victim->pcdata->stats[UNI_GEN] = 3;
        victim->lord=str_dup("Gaia");
        send_to_char("They are now a werewolf.\n\r", victim);
        send_to_char("You are now a werewolf.\n\r", victim);
    }

    else if ( !str_cmp( arg2, "abomination" ))
    {
        victim->class = 3;
        if (!IS_IMMORTAL(victim) )
            victim->level = 3;
        if( victim == ch)
        {   
            send_to_char("You are now an Abomination.\n\r", victim);
        }               
        else
        {
            send_to_char("They are now an Abomination.\n\r",ch);
            send_to_char("You are now an Abomination.\n\r", victim);
        }
    }  

    else
    {
        send_to_char("#wSyntax:#n class <char> <class>.\n\r", ch );
        send_to_char("#wClasses:#n\n\r", ch );
        send_to_char("None, ", ch);
        send_to_char("Abomination, ", ch);
/*        send_to_char("Angel, ", ch); */
        send_to_char("Demon, Dchamp, Dlord, ", ch);
        send_to_char("Highlander, ", ch);
        send_to_char("Hunter, \n\r", ch);
        send_to_char("#PPmage#n, #rRmage#n, #bBmage#n, #gGmage#n, #yYmage#n, ", ch);
        send_to_char("Shakrin, ",ch);
        send_to_char("Vampire, ", ch);
        send_to_char("Werewolf,\n\r", ch);
        send_to_char("Wraith, ", ch);
        send_to_char("Ninja, ", ch);
        send_to_char("Monk, ", ch);
        send_to_char("Drow.\n\r", ch);
	return;
    }
    send_to_char("Class Set.\n\r",ch);
    return;
}

void do_selfclass( CHAR_DATA *ch, char *argument )
{
//    CHAR_DATA *ch;
    char      arg1 [MAX_INPUT_LENGTH];
//    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
//    argument = one_argument( argument, arg2 );
   
  
    if ( IS_NPC(ch) ) return;

    if ( arg1[0] == '\0' )
    {
        send_to_char("#wSyntax:#n selfclass <class>.\n\r", ch );
        send_to_char("Demon, Highlander, Vampire, Werewolf, Ninja, Drow, Dragon\n\r", ch);
        send_to_char("Mage colors: #PPmage#n, #rRmage#n, #bBmage#n, #gGmage#n, #yYmage#n\n\r", ch);
        return;
    }
    if( ch->class != 0 )
    {
        stc("You're already classed, moron!\n\r", ch);
        return;
    }

    else if ( !str_cmp( arg1, "demon"      ) )
    {
            ch->class = CLASS_DEMON;
            if (!IS_IMMORTAL(ch) )
                ch->level = 3;
            ch->lord=str_dup("Tracker");
                send_to_char("You are now a Demon.\n\r", ch);
    }

    else if ( !str_cmp( arg1, "dragon"      ) )
    {
            ch->class = CLASS_DRAGON;
            if (!IS_IMMORTAL(ch) )
                ch->level = 3;
                send_to_char("You are now a Dragon.\n\r", ch);
    }

 else if ( !str_cmp( arg1, "ninja"      ) )
    {
     ch->class = CLASS_NINJA;
             
/* Reset ninja powers */
      
        ch->pcdata->powerss[NPOWER_SORA] = 0;
        ch->pcdata->powerss[NPOWER_CHIKYU] = 0;
        ch->pcdata->powerss[NPOWER_NINGENNO] = 0;
        ch->pcdata->powerss[NINJA_KI] = 200;
        
/* Reset the new ninja stuff */
            
        ch->pcdata->principles[0] = 0;
        ch->pcdata->principles[1] = 0;
        ch->pcdata->principles[2] = 0;
        ch->pcdata->principles[3] = 0;
        ch->pcdata->principles[4] = 0;
        ch->pcdata->principles[5] = 0;
        ch->pcdata->principles[6] = 0;
        ch->pcdata->principles[7] = 0;
        ch->pcdata->principles[8] = 0;
        ch->pcdata->principles[9] = 0; 
        ch->pcdata->principles[10] = 0;
        ch->pcdata->principles[11] = 0;
        ch->pcdata->principles[12] = 0;
        ch->pcdata->principles[13] = 0;
        ch->pcdata->principles[14] = 0;
        ch->pcdata->principles[15] = 0;
        ch->pcdata->principles[16] = 0;
        ch->pcdata->principles[17] = 0;
        ch->pcdata->principles[18] = 0;  
        ch->pcdata->principles[19] = 0;

     send_to_char("You are now a ninja!\n\r", ch);
        
    }
        
 else if ( !str_cmp( arg1, "drow"      ) ) 
    {
     ch->class = CLASS_DROW;
     send_to_char("You are now a drow!\n\r", ch);
    } 
/*
    else if ( !str_cmp( arg1, "monk"      ) )
    {
     ch->class = CLASS_MONK;
     send_to_char("You are now a monk!\n\r", ch);
    }       */
     
    else if ( !str_cmp( arg1, "highlander" ) || !str_cmp(arg1, "kensai") )
    {
        ch->class = CLASS_HIGHLANDER;
        if(!IS_IMMORTAL(ch) )
            ch->level = 3;
            send_to_char("You are now a Kensai.\n\r", ch);
    }
    else if ( !str_cmp( arg1, "pmage"     ) )
    {
        ch->class = CLASS_MAGE;
            ch->trust = 4;
            ch->level = 4;
        ch->pcdata->powersm[MPOWER_RUNE0] = 0;
        ch->pcdata->powersm[MPOWER_RUNE1] = 2047;
        ch->pcdata->powersm[MPOWER_RUNE2] = 1023;
        ch->pcdata->powersm[MPOWER_RUNE3] = 15;
            send_to_char("You are now a #PPurple#n Mage.\n\r", ch);
    }
    else if ( !str_cmp( arg1, "rmage"     ) )
    {
        ch->class = 2;
            ch->trust = 4;
            ch->level = 4;
        ch->pcdata->powersm[MPOWER_RUNE0] = 1;
        ch->pcdata->powersm[MPOWER_RUNE1] = 2047;
        ch->pcdata->powersm[MPOWER_RUNE2] = 1023;
        ch->pcdata->powersm[MPOWER_RUNE3] = 15;
            send_to_char("You are now a #rRed#n Mage.\n\r", ch);   
    }
    else if ( !str_cmp( arg1, "bmage"     ) )
    {
        ch->class = 2;
            ch->trust = 4;
            ch->level = 4;
        ch->pcdata->powersm[MPOWER_RUNE0] = 2;
        ch->pcdata->powersm[MPOWER_RUNE1] = 2047;
        ch->pcdata->powersm[MPOWER_RUNE2] = 1023;
        ch->pcdata->powersm[MPOWER_RUNE3] = 15;
            send_to_char("You are now a #bBlue#n Mage.\n\r", ch);
    }
    else if ( !str_cmp( arg1, "gmage"     ) )
    {
        ch->class = 2;
            ch->trust = 4;
            ch->level = 4;
        ch->pcdata->powersm[MPOWER_RUNE0] = 3;
        ch->pcdata->powersm[MPOWER_RUNE1] = 2047;
        ch->pcdata->powersm[MPOWER_RUNE2] = 1023;
        ch->pcdata->powersm[MPOWER_RUNE3] = 15;
            send_to_char("You are now a #gGreen#n Mage.\n\r", ch);
    }
    else if ( !str_cmp( arg1, "ymage"     ) )
    {
        ch->class = 2;
            ch->trust = 4;
            ch->level = 4;
        ch->pcdata->powersm[MPOWER_RUNE0] = 4;
        ch->pcdata->powersm[MPOWER_RUNE1] = 2047;
        ch->pcdata->powersm[MPOWER_RUNE2] = 1023;
        ch->pcdata->powersm[MPOWER_RUNE3] = 15;
            send_to_char("You are now a #yYellow#n Mage.\n\r", ch);
    }
            
    else if ( !str_cmp( arg1, "vampire"    ) )
    {
        ch->class = 8;
        if(!IS_IMMORTAL(ch) )
            ch->level = 3;
        ch->pcdata->stats[UNI_GEN] = 3;
        ch->lord=str_dup("Tracker");
            send_to_char("You are now a Vampire.\n\r", ch);
    }
            
    else if ( !str_cmp( arg1, "werewolf"   ) )
    {
        ch->class = 4;
        if(!IS_IMMORTAL(ch) )
            ch->level = 3;
        ch->pcdata->stats[UNI_GEN] = 3;
        ch->lord=str_dup("Tracker");
        send_to_char("You are now a werewolf.\n\r", ch);
    }
    else
    {
        send_to_char("#wSyntax:#n selfclass <class>.\n\r", ch );
        send_to_char("Demon, Highlander, Vampire, Werewolf, Ninja, Drow, Dragon\n\r", ch);
        send_to_char("Mage colors: #PPmage#n, #rRmage#n, #bBmage#n, #gGmage#n, #yYmage#n\n\r", ch);
        return;
    }
    return; 
}

void do_doublexp( CHAR_DATA *ch, char *argument )
{
/*Changed Toaster 19/03/00 so 5 players need to be on for doublexp and
doublexp off when it is off says so etc*/
// Changed Tracker 02/13/03 Toaster's an idiot
//    char arg[MIL];
      char buf[MSL];
      DESCRIPTOR_DATA *d;
      int count;

    count = 0;
    for (d = descriptor_list; d != NULL; d = d->next)
        {
        CHAR_DATA *wch;
        if (d->connected != CON_PLAYING) continue;
        wch = (d->original != NULL) ? d->original : d->character;
        if ( IS_NPC(wch) ) continue;
        if ( IS_IMMORTAL(wch) ) continue;
	count++;
	}

//    argument = one_argument( argument, arg );

    if (!ch->desc || *argument == '\0' )
    {
            send_to_char("Syntax: doublexp <on|off>.\n\r",ch);
            return;
     }

    if ( str_cmp(argument, "on")== 0 && (dxp_info.dxp == 1) )
    {
	send_to_char("Double Exp is already on.\n\r",ch);
	return;
    }

    if ( str_cmp(argument, "on")== 0 && (dxp_info.dxp == 0) )
    {	     
	dxp_info.dxp = 1;
        sprintf( buf, "%s activates DOUBLE Exp.\n\r",ch->name);
        do_info( ch, buf );
	  send_to_char("Double Exp is now on!\n\r",ch);
        return;
    }
    
    if (str_cmp(argument, "off")== 0 && (dxp_info.dxp == 0) )
    {
	send_to_char("Double Exp is already off.\n\r", ch);
	return;
    }

    else if (str_cmp(argument, "off")== 0 && (dxp_info.dxp == 1) )
    {
	dxp_info.dxp = 0;
        sprintf(buf, "DOUBLE Exp is no longer in affect!\n\r");
        do_info( ch, buf );
        send_to_char( "Double Exp is now OFF!\n\r", ch );
        return;
    }
    else
	{
	send_to_char("The Commands are ON or OFF.\n\r", ch);
	return;
	}
}

void do_chaos( CHAR_DATA *ch, char *argument )
{
/*Changed Toaster 19/03/00 so 5 players need to be on for doublexp and
doublexp off when it is off says so etc*/
//    char arg[MIL];
      char buf[MSL];
      DESCRIPTOR_DATA *d;
      int count;

    count = 0;
    for (d = descriptor_list; d != NULL; d = d->next)
        {
        CHAR_DATA *wch;
        if (d->connected != CON_PLAYING) continue;
        wch = (d->original != NULL) ? d->original : d->character;
        if ( IS_NPC(wch) ) continue;
        if ( IS_IMMORTAL(wch) ) continue;
	count++;
	}

//    argument = one_argument( argument, arg );

    if (!ch->desc || *argument == '\0' )
    {
            send_to_char("Syntax: Chaos <on|off>.\n\r",ch);
            return;
     }

    if ( str_cmp(argument, "on")== 0 && (chao_info.chs == 1) )
    {
	send_to_char("Chaos is already on.\n\r",ch);
	return;
    }

    if ( str_cmp(argument, "on")== 0 && (chao_info.chs == 0) )
    {	     
	chao_info.chs = 1;
        sprintf( buf, "%s unleashes chaotic forces.\n\r",ch->name);
        do_info( ch, buf );
	  send_to_char("Chaos is now on!\n\r",ch);
        return;
    }
    
    if (str_cmp(argument, "off")== 0 && (chao_info.chs == 0) )
    {
	send_to_char("Chaos is already off.\n\r", ch);
	return;
    }

    else if (str_cmp(argument, "off")== 0 && (chao_info.chs == 1) )
    {
	chao_info.chs = 0;
        sprintf(buf, "Chaos is no longer in affect!\n\r");
        do_info( ch, buf );
        send_to_char( "Chaos is now OFF!\n\r", ch );
        return;
    }
    else
	{
	send_to_char("The Commands are ON or OFF.\n\r", ch);
	return;
	}
}


void do_prep(CHAR_DATA *ch,char *argument)
{
  char buf[MSL];
  CHAR_DATA *vch;

  sprintf(buf,"#ePrepare for a #g%s#e!#n",argument);
  do_info(ch,buf);
  for(vch=char_list;vch!=NULL;vch=vch->next)
  {
    if(IS_NPC(vch)||vch->desc==NULL)
    continue;
 
    do_peace(vch,"");
    do_wake(vch,"");
    do_restore(ch,vch->name);
    do_call(vch,"all");
    do_save(vch,"");
  }
}

/*
void do_prep(CHAR_DATA *ch, char *argument ) 
{
	char buf[MSL];
        char                arg1[MAX_STRING_LENGTH]; 
	argument = one_argument( argument, arg1 );

	sprintf(buf,"#RPrepare for a #G%s#R!#n",arg1);
	do_info(ch,buf);
	do_peace(ch,"all");
	do_forceauto(ch,"wake");
      do_restore(ch,"all");
      do_forceauto(ch,"call all");
      call_all(ch);
      do_asave(ch, "changed"); *- autosave changed areas*
      do_forceauto(ch,"save");
      do_autosave(ch,"");
	return;
}
*/
void do_tscatter(CHAR_DATA *ch,char *argument)
{
  int room = 0;
  char buf[MSL];
  int i;
  char arg[MSL];
  ROOM_INDEX_DATA *location;  
  OBJ_DATA *obj;
  int value;
  argument=one_argument(argument,arg);

  if(!is_number(arg)||!arg[0])
  {
    stc("Syntax is tscatter <Number of Tokens>\n\r",ch);
    return;
  }
    sprintf(buf,"%s throws %d tokens around the mud. \n\r",ch->name, atoi(arg));
    do_info(ch,buf);
  for(i=0;i<atoi(arg);i++)
  {
    location = NULL;
    room = 0;
    while((location=get_room_index(room))==NULL)
    {(int)room=number_range(0,32000);}
    value=number_range(25,150);
    
    obj = create_object( get_obj_index(OBJ_VNUM_PROTOPLASM), value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = obj->value[0]*1000;
    obj->item_type = ITEM_QUEST;
    
    obj_to_room(obj,location);
    free_string( obj->name );
    obj->name = str_dup( "quest token" );
    sprintf(buf,"a %d point quest token",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d point quest token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
  }
}

void do_users( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *vch;
    CHAR_DATA *gch;
    DESCRIPTOR_DATA *d;
    DESCRIPTOR_DATA *d2;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int count;
    char *	st;
    char	s[100];
    char	idle[10];
    bool found = FALSE;
    bool found2 = FALSE;

    count	= 0;
    buf[0]	= '\0';
    buf2[0]	= '\0';

    send_to_char( "#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r", ch );

    strcat( buf2, "\n\r#w[#eNum Connected_State Login@ Idl#w]#e Player Name  Host#n\n\r" );
    strcat( buf2, "#e-------------------------------------------------------------------------------#n\n\r");

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->character != NULL && can_see( ch, d->character ) )
        {
            switch( d->connected )
            {
                case CON_PLAYING:              st = "    PLAYING    "; break;
                case CON_GET_NAME:             st = "   Get Name    "; break;
                case CON_GET_OLD_PASSWORD:     st = "Get Old Passwd "; break;
                case CON_CONFIRM_NEW_NAME:     st = " Confirm Name  "; break;
                case CON_GET_NEW_PASSWORD:     st = "Get New Passwd "; break;
                case CON_CONFIRM_NEW_PASSWORD: st = "Confirm Passwd "; break;
                case CON_GET_NEW_SEX:          st = "  Get New Sex  "; break;
                case CON_GET_NEW_ANSI:         st = "  Get New Ansi "; break;
                case CON_READ_MOTD:            st = " Reading MOTD  "; break;
                case CON_GET_NEW_CLASS:        st = " Picking Class "; break;
                case CON_NOT_PLAYING:          st = " *Not* Playing "; break;
                case CON_NOTE_TO:
                case CON_NOTE_SUBJECT:
                case CON_NOTE_EXPIRE:
                case CON_NOTE_TEXT:
                case CON_NOTE_FINISH:
                                               st = " Writing Note  "; break;
                default:                       st = "   !UNKNOWN!   "; break;
            }
	    count++;
         /* Format "login" value... */
           vch = d->original ? d->original : d->character;
           strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );
           
           if ( vch->timer > 0 ) 
              sprintf( idle, "%-2d", vch->timer );
           else
              sprintf( idle, "  " );
              
           sprintf( buf, "#0[#c%3d {w%s %7s {c%2s#0]{W %-12s {w%-32.32s#n\n\r",
              d->descriptor,
              st,
              s,
              idle,
              ( d->original ) ? d->original->name
                              : ( d->character )  ? d->character->name
                                                  : "(None!)",
              d->character->lasthost );
           
           strcat( buf2, buf );
           
        }  
    }
              
    sprintf( buf, "\n\r{c%d{w user%s.#n\n\r", count, count == 1 ? "" : "s" );
    strcat( buf2, buf );
    send_to_char( buf2, ch );
    send_to_char( "\n\r", ch );

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    if (d->connected != CON_PLAYING) continue;  
    for ( d2 = d->next; d2 != NULL; d2 = d2->next )
    {
      if (!str_cmp(d->host, d2->host))
      {
        if (d2->connected != CON_PLAYING) continue;
        if (d2->character == NULL || d->character == NULL) continue;
        found = TRUE;
        sprintf(buf, "{c%s {wand {c%s {ware Multiplaying.\n\r",d2->character->name, d->character->name);
        send_to_char(buf,ch);
      }
    }
  }
  if (!found) send_to_char("{cNo one is Multiplaying.#n\n\r",ch);

    send_to_char( "\n\r", ch );

  for (gch = char_list; gch != NULL; gch = gch->next)
  {
    if (IS_NPC(gch) || gch->desc) continue;
    found2 = TRUE;
    sprintf(buf, "{cName{D: {w%12s. {D({RRoom{D:{w #c%5d{D)#n\n\r", gch->name, gch->in_room == NULL ? : gch->in_room->vnum);
    send_to_char(buf,ch);
  }
  if(!found2) send_to_char("{wNo Linkdead Players found.#n\n\r",ch);

    send_to_char( "#e=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r", ch );
    return;
}

bool in_war(CHAR_DATA *ch)
{
  if (!ch->in_room) return FALSE;

  if (ch->in_room->vnum >= 50 && ch->in_room->vnum <= 67) return TRUE;
  return FALSE;
}


/* Now part of do_users 
void do_linkdead(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;

  for (gch = char_list; gch != NULL; gch = gch->next)
  {
    if (IS_NPC(gch) || gch->desc) continue;
    found = TRUE;
    sprintf(buf, "Name: %12s. (Room: %5d)\n\r", gch->name, gch->in_room == NULL ? : gch->in_room->vnum);
    send_to_char(buf,ch);
  }
  if(!found) send_to_char("No Linkdead Players found\n\r",ch);
  return;
}
*/

void do_relevel( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch)) return;

 /*Implementors*/
    if (!str_cmp(ch->name,"Tracker") || !str_cmp(ch->name,"Mystical") || !str_cmp(ch->name,"Cirin") || !str_cmp(ch->name, "Tijer"))
    {
	ch->level	= MAX_LEVEL;
	ch->trust	= MAX_LEVEL;
	send_to_char("Resetting to Implementor.\n\r",ch);
        ch->max_hit	= 60000;
	ch->hit	= ch->max_hit;
	ch->max_mana = 60000;
	ch->mana	= ch->max_mana;
	ch->max_move = 60000;
	ch->move	= ch->max_move;
        ch->practice = 10000;
        ch->pcdata->quest = 150000;
        send_to_char("Primal and Quest Points set to max.\n\r",ch);
        ch->pcdata->security = 1;
        send_to_char("Security group 1.\n\r",ch);
	SET_BIT(ch->extra, EXTRA_TRUSTED);
	SET_BIT(ch->extra, EXTRA_NOTE_TRUST);
	send_to_char("You are quest and note trusted.\n\r",ch);
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("Holy light mode on.\n\r", ch );
	send_to_char("Done.\n\r",ch);
    }
    else
	send_to_char("Huh?\n\r",ch);
    return;
}

void do_tick(CHAR_DATA *ch, char *argument)
{
    send_to_char("TICK! all ticks updated.\n\r",ch);
//    weather_update();
//    char_update();
//    obj_update();
    area_update();
//    room_update();
    update_pos(ch);
}

void do_cset( CHAR_DATA *ch, char *argument )
{
/* Coded by Kyrlin 02/12/00 */
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char arg4 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;

    sprintf(buf,"%s: Cset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    strcpy(arg4, argument);
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
        divide_to_char(ch);
stc( "#wSyntax:#n cset <victim> <area> <field> <value>\n\r",  ch );
stc( "\n\r",                                                  ch );
stc( "#wArea being one of:#n\n\r",                            ch );
stc( "  demon, drow, misc, rune\n\r",                         ch );
stc( "\n\r",						              ch );
stc( "#wField being one of:#n\n\r",			              ch );
stc( "  Demon: champion, lord\n\r",                           ch );
stc( "  Drow:  Warrior, Mage, Cleric.\n\r",                   ch );
stc( "  Misc:  sire, prince\n\r",                             ch );
stc( "  Rune:  #PAuram#n, #rIgnem#n, #bAquam#n, #gTerram#n, #yMentem#n\n\r",      ch );
stc( "  \n\r",						              ch );
        divide_to_char(ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_GODLESS) && ch->level < NO_GODLESS)
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    if ( !str_cmp( arg2, "demon") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's.\n\r", ch );
            return;
        }

        if (arg3 == '\0')
        {
            stc( "cset <victim> demon <setting>.\n\r", ch );
            return;         
        }

        if ( !IS_DEMON(victim) )
        {
            stc( "Demons only please.\n\r", ch );
            return;
        }

        if (IS_JUDGE(ch)) 
        {
            if (!str_cmp( arg3, "champion") )
            {   
                if ( IS_SET(victim->special, SPC_CHAMPION) )   
                {
                    stc("You remove their champion flag.\n\r",ch);
                    stc("You are no longer a demon champion!\n\r",victim);
                    REMOVE_BIT(victim->special, SPC_CHAMPION);
                }
                else
                {
                    stc("You add a champion flag to them.\n\r",ch);
                    stc("You are now a demon champion!\n\r",victim);
                    SET_BIT(victim->special, SPC_CHAMPION);
                }
            return;
            }
            if (!str_cmp( arg3, "lord" ) )
            {   
                if ( IS_SET(victim->special, SPC_DEMON_LORD) )   
                {
                    stc("You remove their demon lord flag.\n\r",ch);
                    stc("You are no longer a demon lord!\n\r",victim);
                    REMOVE_BIT(victim->special, SPC_DEMON_LORD);
                }
                else
                {
                    stc("You add a demon lord flag to them.\n\r",ch);
                    stc("You are now a demon lord!\n\r",victim);
                    SET_BIT(victim->special, SPC_DEMON_LORD);
                }
            return;
            }
        else
            send_to_char("No such demon setting exists.\n\r",ch);
            return;
        }
    }

    if ( !str_cmp( arg2, "drow") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's.\n\r", ch );
            return;
        }

        if (arg3 == '\0')
        {
            stc( "cset <victim> drow <setting>.\n\r", ch );
            return;         
        }

        if ( !IS_DROW(victim) )
        {
            stc( "Drow only please.\n\r", ch );
            return;
        }

        if (IS_JUDGE(ch)) 
        {
            if (!str_cmp( arg3, "warrior") )
            {   
                if ( IS_SET(victim->special, SPC_DROW_WAR) )   
                {
                    stc("You remove their warrior status.\n\r",ch);
                    stc("You are no longer a drow warrior!\n\r",victim);
                    REMOVE_BIT(victim->special, SPC_DROW_WAR);
                }
                else
                {
                    stc("You add a warrior flag to them.\n\r",ch);
                    stc("You are now a drow warrior!\n\r",victim);
                    SET_BIT(victim->special, SPC_DROW_WAR);
                }
            return;
            }
            if (!str_cmp( arg3, "mage" ) )
            {   
                if ( IS_SET(victim->special, SPC_DROW_MAG) )   
                {
                    stc("You remove their mage status.\n\r",ch);
                    stc("You are no longer a drow mage!\n\r",victim);
                    REMOVE_BIT(victim->special, SPC_DROW_MAG);
                }
                else
                {
                    stc("You add a drow mage flag to them.\n\r",ch);
                    stc("You are now a drow mage!\n\r",victim);
                    SET_BIT(victim->special, SPC_DROW_MAG);
                }
            return;
            }
            if (!str_cmp( arg3, "cleric" ) )
            {   
                if ( IS_SET(victim->special, SPC_DROW_CLE) )   
                {
                    stc("You remove their cleric status.\n\r",ch);
                    stc("You are no longer a drow cleric!\n\r",victim);
                    REMOVE_BIT(victim->special, SPC_DROW_CLE);
                }
                else
                {
                    stc("You add a drow cleric flag to them.\n\r",ch);
                    stc("You are now a drow cleric!\n\r",victim);
                    SET_BIT(victim->special, SPC_DROW_CLE);
                }
            return;
            }
        else
            send_to_char("No such demon setting exists.\n\r",ch);
            return;
        }
    }

    if ( !str_cmp( arg2, "misc") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's.\n\r", ch );
            return;
        }

        if (arg3 == '\0')
        {
            stc( "cset <victim> misc <setting>.\n\r", ch );
            return;         
        }

        if (!str_cmp( arg3, "sire") )
        {   
            if ( IS_SET(victim->special, SPC_SIRE) )   
            {
                stc("You remove their sire flag.\n\r",ch);
                stc("You are no longer a sire!\n\r",victim);
                REMOVE_BIT(victim->special, SPC_SIRE);
            }
            else
            {
                stc("You add a sire flag to them.\n\r",ch);
                stc("You are now a sire!\n\r",victim);
                SET_BIT(victim->special, SPC_SIRE);
            }
            return;
        }
        if (!str_cmp( arg3, "prince") )
        {   
            if ( IS_SET(victim->special, SPC_PRINCE) )   
            {
                stc("You remove their prince flag.\n\r",ch);
                stc("You are no longer a prince!\n\r",victim);
                REMOVE_BIT(victim->special, SPC_PRINCE);
            }
            else
            {
                stc("You add a prince flag to them.\n\r",ch);
                stc("You are now a prince!\n\r",victim);
                SET_BIT(victim->special, SPC_PRINCE);
            }
            return;
        }
    else
        send_to_char("No such demon setting exists.\n\r",ch);
        return;
    }

    if ( !str_cmp( arg2, "rune") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's.\n\r", ch );
            return;
        }

        if (arg3 == '\0')
        {
            stc( "cset <victim> rune <type>.\n\r", ch );
            return;         
        }
        if (!IS_MAGE(victim) )
        {
            stc( "Warlocks only please.\n\r", ch );
            return;
        }
        
        if (IS_JUDGE(ch)) 
        {
            if (!str_cmp( arg3, "auram") )
            {   
                victim->pcdata->powersm[MPOWER_RUNE0] = 0;
                victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
                victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
                victim->pcdata->powersm[MPOWER_RUNE3] = 15;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "ignem" ) )
            {   
                victim->pcdata->powersm[MPOWER_RUNE0] = 1;
                victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
                victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
                victim->pcdata->powersm[MPOWER_RUNE3] = 15;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "aquam" ) )
            {   
                victim->pcdata->powersm[MPOWER_RUNE0] = 2;
                victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
                victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
                victim->pcdata->powersm[MPOWER_RUNE3] = 15;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "terram" ) )
            {   
                victim->pcdata->powersm[MPOWER_RUNE0] = 3;
                victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
                victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
                victim->pcdata->powersm[MPOWER_RUNE3] = 15;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "mentem" ) )
            {   
                victim->pcdata->powersm[MPOWER_RUNE0] = 4;
                victim->pcdata->powersm[MPOWER_RUNE1] = 2047;
                victim->pcdata->powersm[MPOWER_RUNE2] = 1023;
                victim->pcdata->powersm[MPOWER_RUNE3] = 15;
                stc("Ok.\n\r",ch);
                return;
            }
        else
            send_to_char("No such rune type exists.\n\r",ch);
            return;
        }
    }

    do_cset( ch, "" );
    return;
}

int getMight(CHAR_DATA *ch)
{
  AFFECT_DATA *paf;
  OBJ_DATA *obj;
  int objhps = 0;
  int might, temp, i;

  for (i = 0; i < MAX_WEAR; i++)
  {
    if ((obj = get_eq_char(ch, i)) == NULL) continue;
    for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
    {
      if (paf->location == APPLY_HIT) 
        objhps += paf->modifier;
    }
    for (paf = obj->affected; paf; paf = paf->next)
    {
      if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
    }
  }
  
//  might = (ch->max_hit - (spellhps + objhps))/100;
    might = ch->max_hit/100;
    might += ch->max_move/100;
  
  for (i = 0; i < 5; i++)  might += UMIN(2, ch->spl[i]/100);
  for (i = 0; i < 13; i++) might += UMIN(4, ch->wpn[i]/50);
  for (i = 1; i < 16; i++) might += UMIN(4, ch->stance[i]/50);


  if (might >= 150)
  {
    if ((temp = get_ratio(ch)) >= 500) might += UMIN(100, temp / 50);  // 10 points for each pkpower.
  }


  return might;
}

/*
 * New system to replace status, called fair fight, it measures the
 * difference between two players, giving them points for their
 * stances, powers, and stats. If they are within each others range,
 * the call will return TRUE, if not FALSE. Call for fair_fight when
 * you need to see if a fight is fair (ie. decapping).
 */
bool fair_fight(CHAR_DATA *ch, CHAR_DATA *victim)
{

  if (ch->race == 0 && victim->race >= 1) return FALSE;
  else if( ((victim->race > 1 && victim->race <= 9) && (ch->race > 19))) return FALSE;
  else if ((victim->race <= 19) && (ch->race > 29)) return FALSE;
  else if ((victim->race <= 29) && (ch->race > 39)) return FALSE;
  else if ((victim->race <= 39) && (ch->race > 49)) return FALSE;
  else if ((victim->race <= 49) && (ch->race > 59)) return FALSE;
  else if ((victim->race <= 59) && (ch->race > 69)) return FALSE;
  else if ((victim->race <= 69) && (ch->race > 79)) return FALSE;
  else if ((victim->race == 0 && ch->race > 0)) return TRUE;

    if ( (( get_age(victim) - 17) * 2 ) < 10 ) return FALSE;

  /*
   * They passed the test, FIGHT children.
   */
  return TRUE;
}

int get_ratio(CHAR_DATA *ch)
{
  long ratio;

  if (IS_NPC(ch)) return 0;
  if ((ch->pkill + ch->pdeath) == 0) ratio = 0; // to avoid divide by zero.
  else if (ch->pkill > ch->pdeath)
    ratio = ch->pkill * 100 * ((ch->pkill * ch->pkill) - (ch->pdeath * ch->pdeath))/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  else if (ch->pkill > 0)
    ratio = (-100) * (ch->pdeath - ch->pkill) / ch->pkill;
  else
    ratio = (-100) * ch->pdeath;
  return (int) ratio;
}


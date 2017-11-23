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
#include <string.h>
#include <time.h>
#include "merc.h"
#include "warlock.h"



/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void    moon_update     args((void));
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void    ww_update       args( ( void ) );
void    embrace_update	args( ( void ) );
void    regen_limb	args( ( CHAR_DATA *ch ) );
void    player_update	args( ( void ) );

void update_ninja                               args ((CHAR_DATA *ch));
void update_werewolf                            args ((CHAR_DATA *ch));
void update_demon                               args ((CHAR_DATA *ch));
void update_drow                                args ((CHAR_DATA *ch));
void update_highlander                          args ((CHAR_DATA *ch));
void update_mage                                args ((CHAR_DATA *ch));
void update_vampire                             args ((CHAR_DATA *ch));
void update_monk                                args ((CHAR_DATA *ch));
void update_item_regen                          args ((CHAR_DATA *ch));


void hint_update( void )
{
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
    
    for(ch=char_list;ch!=NULL;ch=ch_next)
    {
      ch_next = ch->next;
 
//    if ( (( get_age(gch) - 17) * 2 ) < 5 ) 
      if(!IS_NPC(ch) && ((get_age(ch) - 17) * 2 < 5) && !IS_SET(ch->deaf, CHANNEL_HINT))
      {
        send_to_char("\n\r#w[#rHINT#w] ",ch);
        switch(number_range(0,11))
        {
        default: send_to_char(" No, we are -NOT- looking for immortals.", ch); break;
        case 0:  send_to_char(" Don't bother prac'in spells/skills while mortal, when you avatar they will all be set to 100.",ch); break;
        case 1:  send_to_char(" We do offer multiclassing here, see 'help multiclassing'",ch); break;
        case 2:  send_to_char(" To become a class, get 5 mob kills, save, train avatar, and selfclass.",ch);break;
        case 3:  send_to_char(" If you find a bug that crashes the mud, please report it to Tracker.",ch); break;
        case 4:  send_to_char(" You may train hp/mana/move all, a faster way of training stats.",ch); break;
        case 5:  send_to_char(" Use consider before combat to judge your chances in a fight",ch); break;
        case 6:  send_to_char(" The statcap here is 300K",ch); break;
        case 7:  send_to_char(" Don't eat yellow snow.",ch); break;
        case 8:  send_to_char(" Never trust a turtle.",ch); break;
        case 9:  send_to_char(" For information on Chaos, Double XP, Double Train, and Double QP, see 'help double'",ch); break;
        case 10: send_to_char(" This mud offers Kingdoms, type 'kinglist' or 'help kingdom' for information.",ch); break;
        case 11: send_to_char(" You will recieve Newbie Experience from your first 5 hrs of gametime", ch);break;
        }
        send_to_char("\n\r",ch);
      }
   }
   return;
}   

void gain_exp( CHAR_DATA *ch, int gain )
{
    CHAR_DATA *mount = NULL;
    CHAR_DATA *master = NULL;

    if ( IS_NPC(ch) && (mount = ch->mount) != NULL && !IS_NPC(mount))
    {
	if ( (master = ch->master) == NULL || master != mount )
	    mount->exp += gain;
    }

	/*if ( (( get_age(ch) - 17) * 2 ) < 5 )
      gain *= 3; NEWBIEEXP
*/

    if ( !IS_NPC(ch) && !IS_IMMORTAL(ch) )
	ch->exp += gain;
    return;
}


/*
 * Regeneration stuff.
 */

// New regen - Tracker
            
void player_update(void)
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
                
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
        ch_next = ch->next;
                
        if (IS_NPC(ch) || ch == NULL
            || ch->in_room == NULL)
        {
           continue;
        }
        if (ch->hit > 0
           && (ch->position == POS_MORTAL
           || ch->position == POS_INCAP))
                update_pos(ch);
                
        if (ch->hit > 0 && ch->position == POS_STUNNED && number_range(1,10) < 4)
                update_pos(ch);
                    
                    
        if (ch->hit < ch->max_hit
           || ch->move < ch->max_move
           || ch->mana < ch->max_mana)
           {
                            
                if (ch->in_room->vnum == 29502 && ch->move < ch->max_move)
                        ch->move += 8000;
                if (ch->in_room->vnum == 29501 && ch->mana < ch->max_mana)
                        ch->mana += 8000;
               if (ch->in_room->vnum == 29500 && ch->hit < ch->max_hit)
                        ch->hit += 8000;     

                if (ch->position == POS_MORTAL && number_range(1,10) < 4)
                        stc("Blood stops flowing from your wounds.\n\r",ch);
                if (ch->position == POS_STUNNED && number_range(1,10) < 4)
                        stc("Your bones move back into their proper places.\n\r",ch);
                if (ch->position == POS_INCAP && number_range(1,10) < 4)
                        stc("Your wounds slowly knit themselves back together.\n\r",ch);
                     
                switch (ch->position)
                {
                case POS_MORTAL:{
                ch->hit += number_range(1,3);
                break;}
           
                case POS_INCAP:{
                ch->hit += number_range(1,3);
		update_pos(ch);
                break;}
                
                case POS_STUNNED:{
                ch->hit++;
                break;}
           
                case POS_SLEEPING:{   
                        ch->hit  += number_range(1300,1800);
                        ch->mana += number_range(1300,1800);
                        ch->move += number_range(1300,1800);
                        break;}
                
                case POS_MEDITATING:{
                        ch->hit  += number_range(1200,1300);
                        ch->mana += number_range(1500,1900);
                        ch->move += number_range(1200,1300);
                        break;}
                        
                case POS_RESTING:{
                        ch->hit  += number_range(1120,1300);
                        ch->mana += number_range(1120,1300);
                        ch->move += number_range(1120,1300);
                        break;}
                
                case POS_STANDING:{
                        ch->hit  += number_range(450,520);
                        ch->mana += number_range(450,520);
                        ch->move += number_range(450,520);
                break;}
                
                default: break;
                }
           }    
        if (ch->hit  > ch->max_hit ) ch->hit  = ch->max_hit;
        if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
        if (ch->move > ch->max_move) ch->move = ch->max_move;

        if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NINJAKI] < 300 )
        {
            if ( ch->position == POS_MEDITATING )
            {
               ch->pcdata->powerss[NINJAKI] += number_range( 50, 70 );
               if ( ch->pcdata->powerss[NINJAKI] > 300 )
                   ch->pcdata->powerss[NINJAKI] = 300;
            }
        }

		//if (IS_EXTRA(ch, EXTRA_PREGNANT) && (yy > 0 || (dad > 0 && mm >= 9))) SET_BIT(ch->extra, EXTRA_LABOUR);
                        
    }           
    return;
}

/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int conamount;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
/*
if (IS_NPC(ch) && POS_MORTAL)
{
gain = number_range( 1, 2 );
} 
*/
   else
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE)) return 0;
	gain = number_range( 300, 500 );

	if ((conamount = (get_curr_con(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: gain *= conamount * 0.5;	break;
		case POS_SLEEPING:   gain *= conamount;		break;
		case POS_RESTING:    gain *= conamount * 0.5;	break;
	    }
	}

	if ( ch->pcdata->condition[COND_FULL]   == 0 && !IS_HERO(ch) )
	    gain *= 0.5;

	if ( ch->pcdata->condition[COND_THIRST] == 0 && !IS_HERO(ch) )
	    gain *= 0.5;

    }


    if ( IS_AFFECTED(ch, AFF_POISON) || IS_AFFECTED(ch, AFF_FLAMING))
	gain *= 0.25;

    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int intamount;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE)) return 0;
	gain = number_range( 10, 20 );

	if ((intamount = (get_curr_int(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: gain *= intamount * 10.0;	        break;
		case POS_SLEEPING:   gain *= intamount;			break;
		case POS_RESTING:    gain *= intamount * 0.5;		break;
	    }
	}

	if ( !IS_HERO(ch) && ch->pcdata->condition[COND_THIRST] == 0 )
	    gain *= 0.5;

    }

    if ( IS_AFFECTED( ch, AFF_POISON ) || IS_AFFECTED(ch, AFF_FLAMING) )
	gain *= 0.25;

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;
    int dexamount;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	if (IS_CLASS(ch, CLASS_VAMPIRE)) return 0;
	gain = number_range( 10, 20 );

	if ((dexamount = (get_curr_dex(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: gain *= dexamount * (ch->level * 10);	break;
		case POS_SLEEPING:   gain *= dexamount;		break;
		case POS_RESTING:    gain *= dexamount * 0.5;	break;
	    }
	}

	if ( !IS_HERO(ch) && ch->pcdata->condition[COND_THIRST] == 0 )
	    gain *= 0.5;
    }

    if ( IS_AFFECTED(ch, AFF_POISON) || IS_AFFECTED(ch, AFF_FLAMING) )
	gain *= 0.25;

    return UMIN(gain, ch->max_move - ch->move);
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( value == 0 || IS_NPC(ch) )
	return;

    if (!IS_NPC(ch) && IS_HERO(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) 
	&& iCond != COND_DRUNK)
	return;

    condition				= ch->pcdata->condition[iCond];
    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) )
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );
    else
	{
               int chargen = ch->pcdata->stats[UNI_GEN];
                int maxblood;
                if (chargen > 12)
                        chargen = 12;
                maxblood = (13 - chargen) * 100;
     
                ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, maxblood );
        }
/*
	if ( ch->pcdata->stats[UNI_GEN] == 2 )
		ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 1000 );
	else if ( ch->pcdata->stats[UNI_GEN] == 3 )
		ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 666 );
	else if ( ch->pcdata->stats[UNI_GEN] == 4 )
		ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 500 );
	else if ( ch->pcdata->stats[UNI_GEN] >= 5 )
		ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 400 );
	else if ( ch->pcdata->stats[UNI_GEN] == 6 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 333 ); 
else if ( ch->pcdata->stats[UNI_GEN] == 7 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 285 );
else if ( ch->pcdata->stats[UNI_GEN] == 8 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 250 );
else if ( ch->pcdata->stats[UNI_GEN] == 9 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 222 );
else if ( ch->pcdata->stats[UNI_GEN] == 10 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 200 );
else if ( ch->pcdata->stats[UNI_GEN] == 11 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 181 );
else if ( ch->pcdata->stats[UNI_GEN] == 12 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 166 );
else if ( ch->pcdata->stats[UNI_GEN] == 13 )
  ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 153 );	

}
*/
    if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	    {
		send_to_char( "You are REALLY hungry.\n\r",  ch );
		act( "You hear $n's stomach rumbling.", ch, NULL, NULL, TO_ROOM );
	    }
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You are REALLY thirsty.\n\r", ch );
/*
	    else if (ch->hit > 0)
	    {
		send_to_char( "You are DYING from lack of blood!\n\r", ch );
		act( "$n gets a hungry look in $s eyes.", ch, NULL, NULL, TO_ROOM );
		ch->hit = ch->hit - number_range(2,5);
		if (number_percent() <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
*/
        if(IS_AFFECTED(ch, AFF_SHADOWPLANE))
        { 
            do_shadowplane(ch, "");
        }

	if(IS_SET(ch->polyaff, POLY_NEWZULO) )
	{	
	    do_zuloform(ch, "");
	}
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }
    else if ( ch->pcdata->condition[iCond] < 10 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You feel hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
		send_to_char( "You feel thirsty.\n\r", ch );
	    else
	    {
		send_to_char( "You crave blood.\n\r", ch );
		if (number_range(1,1000) <= ch->beast && ch->beast > 0) vamp_rage(ch);
		if (number_percent() > (ch->pcdata->condition[COND_THIRST]+75)
		    && !IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	    }
	    break;
	}
    }

    return;
}





/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( ch->in_room == NULL ) continue;

	if ( ch->hunting != NULL && ch->hunting != '\0' && 
	    strlen(ch->hunting) > 1 )
	{
	    check_hunt( ch );
	    continue;
	}

	if ( !IS_NPC(ch) )
	{
/*
	    if ((ch->level == 12) && (fLogAll == FALSE)  )
            {
            	fLogAll = TRUE;
            }
*/

	    if (ch->pcdata->condition[COND_DRUNK] > 10 && number_range(1,10) == 1)
	    {
		send_to_char("You hiccup loudly.\n\r",ch);
		act("$n hiccups.",ch,NULL,NULL,TO_ROOM);
	    }
	    if (ch->pcdata->stage[0] > 0 || ch->pcdata->stage[2] > 0)
	    {
		CHAR_DATA *vch;
		if (ch->pcdata->stage[1] > 0 && ch->pcdata->stage[2] >= 225)
		{
		    ch->pcdata->stage[2] += 1;
		    if ( ( vch = ch->pcdata->partner ) != NULL &&
			!IS_NPC(vch) && vch->pcdata->partner == ch &&
			((vch->pcdata->stage[2] >= 200 && vch->sex == SEX_FEMALE) ||
			(ch->pcdata->stage[2] >= 200 && ch->sex == SEX_FEMALE)))
		    {
			if (ch->in_room != vch->in_room) continue;
			if (vch->pcdata->stage[2] >= 225 &&
			    ch->pcdata->stage[2] >= 225 &&
			    vch->pcdata->stage[2] < 240 &&
			    ch->pcdata->stage[2] < 240)
			{
			    ch->pcdata->stage[2] = 240;
			    vch->pcdata->stage[2] = 240;
			}
			if (ch->sex == SEX_MALE && vch->pcdata->stage[2] >= 240)
			{
			    act("You thrust deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_CHAR);
			    act("$n thrusts deeply between your warm, damp thighs.",ch,NULL,vch,TO_VICT);
			    act("$n thrusts deeply between $N's warm, damp thighs.",ch,NULL,vch,TO_NOTVICT);
			    if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
				ch->pcdata->stage[2] = vch->pcdata->stage[2];
			}
			else if (ch->sex == SEX_FEMALE && vch->pcdata->stage[2] >= 240)
			{
			    act("You squeeze your legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_CHAR);
			    act("$n squeezes $s legs tightly around you, moaning loudly.",ch,NULL,vch,TO_VICT);
			    act("$n squeezes $s legs tightly around $N, moaning loudly.",ch,NULL,vch,TO_NOTVICT);
			    if (vch->pcdata->stage[2] > ch->pcdata->stage[2])
				ch->pcdata->stage[2] = vch->pcdata->stage[2];
			}
		    }
		    if (ch->pcdata->stage[2] >= 250)
		    {
			if ( ( vch = ch->pcdata->partner ) != NULL &&
			    !IS_NPC(vch) && vch->pcdata->partner == ch &&
			    ch->in_room == vch->in_room)
			{
			    vch->pcdata->stage[2] = 250;
			    if (ch->sex == SEX_MALE)
			    {
				stage_update(ch,vch,2);
				stage_update(vch,ch,2);
			    }
			    else
			    {
				stage_update(vch,ch,2);
				stage_update(ch,vch,2);
			    }
			    ch->pcdata->stage[0] = 0;
			    vch->pcdata->stage[0] = 0;
			    if (!IS_EXTRA(ch, EXTRA_EXP))
			    {
				send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",ch);
				SET_BIT(ch->extra, EXTRA_EXP);
				ch->exp += 100000;
			    }
			    if (!IS_EXTRA(vch, EXTRA_EXP))
			    {
				send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",vch);
				SET_BIT(vch->extra, EXTRA_EXP);
				vch->exp += 100000;
			    }
			}
		    }
		}
		else
		{
		    if (ch->pcdata->stage[0] > 0 && ch->pcdata->stage[2] < 1 &&
			ch->position != POS_RESTING) 
		    {
			if (ch->pcdata->stage[0] > 1)
			    ch->pcdata->stage[0] -= 1;
			else
			    ch->pcdata->stage[0] = 0;
		    }
		    else if (ch->pcdata->stage[2]>0 && ch->pcdata->stage[0] < 1)
		    {
			if (ch->pcdata->stage[2] > 10)
			    ch->pcdata->stage[2] -= 10;
			else
			    ch->pcdata->stage[2] = 0;
			if (ch->sex == SEX_MALE && ch->pcdata->stage[2] == 0)
			    send_to_char("You feel fully recovered.\n\r",ch);
		    }
		}
	    }
/* Hunter HIDE - Was Ninja Michi
		if (!IS_NPC(ch) && IS_NINJA(ch) && IS_HERO(ch))
	    {
 Changed Toaster 19/04/00 Ninja hide now costs 1 ki per tick, also ninja
regen costs 5 per tick with fight timer
                if ( IS_AFFECTED(ch, AFF_HIDE) ) ch->move -= 10;
                if(ch->move <= 0)
                {
                 ch->move = 0;
                 if ( IS_AFFECTED(ch, AFF_HIDE) )
                 {
                 REMOVE_BIT(ch->affected_by, AFF_HIDE);
                 act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM );
                 send_to_char( "You appear from the shadows.\n\r", ch );
                 }
                }

		if ( ch->position == POS_FIGHTING && ch->pcdata->stats[UNI_RAGE] > 0 
		    && ch->pcdata->stats[UNI_RAGE] < 200 && !IS_ITEMAFF(ch, ITEMA_RAGER) )
		    ch->pcdata->stats[UNI_RAGE] += 1;
		else if ( ch->position == POS_MEDITATING && ch->pcdata->stats[UNI_RAGE] > 0
		 	&& ch->pcdata->stats[UNI_RAGE] < 200 && !IS_ITEMAFF(ch, ITEMA_RAGER) )
		 	ch->pcdata->stats[UNI_RAGE] -= 20;
		else if (ch->pcdata->stats[UNI_RAGE] > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
		    ch->pcdata->stats[UNI_RAGE] -= 1;
		if (ch->pcdata->stats[UNI_RAGE] < 1) continue;

		if(ch->pcdata->stats[UNI_RAGE] > 0)
		{
		if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana ||
                    ch->move < ch->max_move )
		if( ch->fight_timer > 0 )
	        {
		  if(ch->pcdata->powerss[NINJAKI] < 0)
			{
			 ch->pcdata->powerss[NINJAKI] = 0;
			 ch->pcdata->stats[UNI_RAGE] = 0;
			}
		 ch->pcdata->powerss[NINJAKI] -= 5;
                 werewolf_regen(ch);

		}
                if (ch->loc_hp[6] > 0)
                {
                    int sn = skill_lookup( "clot" );
                    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
                }
                else
                {
                    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
                        ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
                    reg_mend(ch);
                }       

	   }
	   }	

	    if (!IS_NPC(ch) && IS_MAGE(ch) && IS_HERO(ch))
	    {
   	    if ( (ch->position == POS_MEDITATING && 
		ch->spheres[MLIF] >= 3 && ch->spheres[MPRI] >= 3) || (ch->spheres[MLIF] == 5 && ch->spheres[MPRI] > 2) )
	    {
	     if ( ch->hit < ch->max_hit ||
                     ch->mana < ch->max_mana ||
                     ch->move < ch->max_move )
             if( ch->quint[BODY] >= 1 )
	     {
		 ch->quint[BODY]-- ;
	         if( ch->quint[BODY] == 0 ) ch->quint[AVATAR]-- ;
	         if( ch->quint[AVATAR] <= 0)
		 {
		   send_to_char("You have no quintessence so don't regen.\n\r",ch);
		   return;
		 }
	      return;
	      }

		    werewolf_regen(ch);
                if (ch->loc_hp[6] > 0)
                {
                    int sn = skill_lookup( "clot" );
                    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
                }
                else 
                {
                    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
                        ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
                    reg_mend(ch);
                }
	          
	    }
	    }
*/
	    if(IS_HERO(ch))
	    {
	    if (IS_ITEMAFF(ch, ITEMA_REGENERATE)) update_item_regen(ch); 
 	    if (IS_CLASS(ch, CLASS_MONK)) update_monk(ch);
 	    if (IS_CLASS(ch, CLASS_HIGHLANDER)) update_highlander(ch);
 	    if (IS_CLASS(ch, CLASS_NINJA)) update_ninja(ch);
 	    if (IS_CLASS(ch, CLASS_MAGE)) update_mage(ch);
 	    if (IS_CLASS(ch, CLASS_VAMPIRE)) update_vampire(ch);
 	    if (IS_CLASS(ch, CLASS_WEREWOLF)) update_werewolf(ch);
 	    if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp,WARP_REGEN)) update_demon(ch);
            if (IS_CLASS(ch,CLASS_DROW) && IS_SET(ch->pcdata->powersd[1],DPOWER_REGEN))
	    update_drow(ch);
	    if (IS_DEMON(ch) && IS_HERO(ch) && ch->in_room != NULL && ch->in_room->vnum == 30000)
	    {
		update_demon(ch);
	    }

            }
/*	    if (!IS_NPC(ch) && IS_VAMPIRE(ch) && IS_HERO(ch))
	    {
		if ( ch->position == POS_FIGHTING && ch->pcdata->stats[UNI_RAGE] > 0 
		    && ch->pcdata->stats[UNI_RAGE] < 25 && !IS_ITEMAFF(ch, ITEMA_RAGER))
		    ch->pcdata->stats[UNI_RAGE] += 1;
		else if (ch->pcdata->stats[UNI_RAGE] > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
                {
		        if (number_percent() <= ch->beast)
                        beast_rage(ch);
                    else
                        ch->pcdata->stats[UNI_RAGE]--;
                    if (ch->pcdata->stats[UNI_RAGE] < 1)  
                        send_to_char("You feel the beast release it's hold on you.\n\r",ch);
                }

		if (ch->pcdata->stats[UNI_RAGE] < 1) continue;
		if ( ch->hit < ch->max_hit ||
		     ch->mana < ch->max_mana || 
		     ch->move < ch->max_move )
		    werewolf_regen(ch);
		if (ch->loc_hp[6] > 0)
		{
		    int sn = skill_lookup( "clot" );
		    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
		}
		else
		{
		    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
			ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
		    reg_mend(ch);
		}
	    }
	
	    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) && IS_HERO(ch))
	    {
	      if ( ch->position != POS_FIGHTING )
	      {
		if ( ch->chi[CURRENT] > 0 )
	        {
		    if ( number_range( 1,4 ) == 2 )
		    {
			ch->chi[CURRENT]--;
			stc("You feel more relaxed.\n\r",ch);
			act("$n looks more relaxed.",ch,NULL,NULL,TO_ROOM);
		    }
		}
	      }
	    }

else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && (IS_HERO(ch) || (IS_CLASS(ch, CLASS_DEMON) && (IS_SET(ch->warp, WARP_REGEN))))) 
	    {
		if (ch->position == POS_FIGHTING && (!IS_ITEMAFF(ch,
ITEMA_RAGER) || (IS_CLASS(ch, CLASS_DEMON) && (IS_SET(ch->warp,
WARP_REGEN))))) 
		{
		    if ( ch->gifts[AHROUN] >= 5 &&
(ch->pcdata->stats[UNI_RAGE] < 350  || (IS_CLASS(ch, CLASS_DEMON) &&
(IS_SET(ch->warp, WARP_REGEN))))) 
		        ch->pcdata->stats[UNI_RAGE] += number_range(2,3);
	            if ( ch->gifts[SHADOWLORDS] >= 5 &&
(ch->pcdata->stats[UNI_RAGE] <= 350  || (IS_CLASS(ch, CLASS_DEMON) &&
(IS_SET(ch->warp, WARP_REGEN))))) 
			ch->pcdata->stats[UNI_RAGE] += number_range(2,4);
		    if (ch->pcdata->stats[UNI_RAGE] < 300)
			ch->pcdata->stats[UNI_RAGE] += number_range(5,10);
		    if (!IS_SET(ch->special, SPC_WOLFMAN) && 
			(ch->pcdata->stats[UNI_RAGE] >= 100 ||
(IS_CLASS(ch, CLASS_DEMON) && (IS_SET(ch->warp, WARP_REGEN))))) 
			do_werewolf(ch,"");
		}
		else if (ch->pcdata->stats[UNI_RAGE] > 0 &&
(!IS_ITEMAFF(ch, ITEMA_RAGER) || (IS_CLASS(ch, CLASS_DEMON) &&
(IS_SET(ch->warp, WARP_REGEN))))) 
		{
		    ch->pcdata->stats[UNI_RAGE] -= 1;
		    if (ch->pcdata->stats[UNI_RAGE] < 100)
do_unwerewolf(ch,"");
		}
		if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
		    ch->move < ch->max_move )
 		    werewolf_regen(ch);

	    }	    

             else if (IS_CLASS(ch,CLASS_DROW) && IS_SET(ch->pcdata->powersd[1],DPOWER_REGEN))
             {

		 if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana  || ch->move < ch->max_move )
                    werewolf_regen(ch);
                if (ch->loc_hp[6] > 0)
                {   
                    int sn = skill_lookup( "clot" );
                    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);  
                }
                else
                {
                    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
                        ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
                    reg_mend(ch);
                }
             }
*/
/*
		if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
		    werewolf_regen(ch);
		if (ch->hit > 0)
		{
		    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
			ch->move < ch->max_move )
			werewolf_regen(ch);
		    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
			ch->move < ch->max_move )
			werewolf_regen(ch);
		    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
			ch->move < ch->max_move )
			werewolf_regen(ch);
		    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
			ch->move < ch->max_move )
			werewolf_regen(ch);
		}
		if (ch->loc_hp[6] > 0)
		{
		    int sn = skill_lookup( "clot" );
		    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
		}
		else
		{
		    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
			ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
		    reg_mend(ch);
		}
	    }

	    else if (IS_ITEMAFF(ch, ITEMA_REGENERATE) || (!IS_NPC(ch) && 
		(IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_MOOGLE))))
	    {
		if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
		    ch->move < ch->max_move )
		    werewolf_regen(ch);
		if (ch->loc_hp[6] > 0)
		{
		    int sn = skill_lookup( "clot" );
		    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
		}
		else
		{
		    if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
			ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
		    reg_mend(ch);
		}
	    }
*/
	    continue;
	}

	if ( IS_AFFECTED(ch, AFF_CHARM) ) continue;

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	    if (ch == NULL) continue;
	}

	/* That's all for sleeping / busy monster */
	if ( ch->position != POS_STANDING )
	    continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 2 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && obj->cost > max )
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n picks $p up.", ch, obj_best, NULL, TO_ROOM );
		act( "You pick $p up.", ch, obj_best, NULL, TO_CHAR );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL)
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)
	&& ( ch->hunting == NULL || strlen(ch->hunting) < 2 )
	&& ( (!IS_SET(ch->act, ACT_STAY_AREA) && ch->level < 900)
	||   pexit->to_room->area == ch->in_room->area ) )
	{
	    move_char( ch, door );
	}

	/* Flee */
	if ( ch->hit < ch->max_hit / 2
	&& ( door = number_bits( 3 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_AFFECTED(ch, AFF_WEBBED)
	&&   ch->level < 900
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) )
	{
	    CHAR_DATA *rch;
	    bool found;

	    found = FALSE;
	    for ( rch  = pexit->to_room->people;
		  rch != NULL;
		  rch  = rch->next_in_room )
	    {
		if ( !IS_NPC(rch) )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if ( !found )
		move_char( ch, door );
	}

    }

    return;
}



/*
 * Update the moonphases
 */
void moon_update(void) {
	switch (time_info.day) {
		case 1: switch (time_info.hour) {
			case 12: weather_info.moonphase = MOON_NEW;
			break;
		} break;
		case 2: weather_info.moonphase = MOON_NEW; break;
		case 3: weather_info.moonphase = MOON_NEW; break;
		case 4: weather_info.moonphase = MOON_NEW; break;
		case 5: weather_info.moonphase = MOON_NEW; break;
		case 6: weather_info.moonphase = MOON_NEW; break;
		case 7: weather_info.moonphase = MOON_NEW; break;
		case 8: weather_info.moonphase = MOON_NEW; break;
		case 9: weather_info.moonphase = MOON_NEW; break;
		case 10: switch (time_info.hour) {
			case 12: weather_info.moonphase = MOON_CRESCENT;
			break;
		} break;
		case 11: weather_info.moonphase = MOON_CRESCENT; break;
		case 12: weather_info.moonphase = MOON_CRESCENT; break;
		case 13: weather_info.moonphase = MOON_CRESCENT; break;
		case 14: weather_info.moonphase = MOON_CRESCENT; break;
		case 15: weather_info.moonphase = MOON_CRESCENT; break;
		case 16: switch (time_info.hour) {
			case 12: weather_info.moonphase = MOON_HALF;
			break;
		} break;
		case 17: switch (time_info.hour) {
			case 12: weather_info.moonphase = MOON_GIBBOUS;
			break;
		} break;
		case 18: weather_info.moonphase = MOON_GIBBOUS; break;
		case 19: weather_info.moonphase = MOON_GIBBOUS; break;
		case 20: weather_info.moonphase = MOON_GIBBOUS; break;
		case 21: weather_info.moonphase = MOON_GIBBOUS; break;
		case 22: weather_info.moonphase = MOON_GIBBOUS; break;
		case 23: weather_info.moonphase = MOON_GIBBOUS; break;
		case 24: weather_info.moonphase = MOON_GIBBOUS; break;
		case 25: weather_info.moonphase = MOON_GIBBOUS; break;
		case 26: weather_info.moonphase = MOON_GIBBOUS; break;
		case 27: weather_info.moonphase = MOON_GIBBOUS; break;
		case 28: weather_info.moonphase = MOON_GIBBOUS; break;
		case 29: weather_info.moonphase = MOON_GIBBOUS; break;
		case 30: weather_info.moonphase = MOON_GIBBOUS; break;
		case 31: weather_info.moonphase = MOON_GIBBOUS; break;
		case 32: switch (time_info.hour) {
			case 12: weather_info.moonphase = MOON_FULL;
			break;
		} break;
		case 33: switch (time_info.hour) {
			case 12: weather_info.moonphase = MOON_WANING;
			break;
		} break;
		case 34: weather_info.moonphase = MOON_WANING; break; 
		case 35: weather_info.moonphase = MOON_WANING; break; 
	}
}



/*
 * Update the weather.
 */
void weather_update(void)
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch = NULL;
     int diff;
    bool char_up;
    bool mes_all = FALSE;

    strcpy(buf,"Bug - Please inform KaVir.\n\r");

/*    for (d = descriptor_list; d != NULL; d = d->next) {
		if (d->connected == CON_PLAYING
		&& (ch = d->character) != NULL
		&&  !IS_NPC(ch)
		&&  !IS_EXTRA(ch, EXTRA_OSWITCH)
		&&  !IS_HEAD(ch, LOST_HEAD)) {
			}*/

    if (IS_SET(world_affects, WORLD_ECLIPSE)) {
		strcpy(buf,"The sun emerges from behind the moon.\n\r");
		REMOVE_BIT(world_affects, WORLD_ECLIPSE);
		mes_all = TRUE;
	} else if (IS_SET(world_affects, WORLD_FOG) && number_range(1,5) == 1) {
		strcpy(buf,"The fog disperses.\n\r");
		REMOVE_BIT(world_affects, WORLD_FOG);
		mes_all = TRUE;
	} else if (IS_SET(world_affects, WORLD_RAIN) && number_range(1,5) == 1) {
		strcpy(buf,"It is no longer raining quite so heavily.\n\r");
		REMOVE_BIT(world_affects, WORLD_RAIN);
		mes_all = TRUE;
	}
    if (mes_all) 
{

		for (d = descriptor_list; d != NULL; d = d->next) {
			if (d->connected == CON_PLAYING
			&& (ch = d->character) != NULL
			&&  !IS_NPC(ch)
			&&  !IS_EXTRA(ch, EXTRA_OSWITCH)
			&&  !IS_HEAD(ch, LOST_HEAD)
			&&  ch->in_room != NULL
			&&  ch->in_room->sector_type != SECT_INSIDE
			&&  IS_OUTSIDE(ch))
			send_to_char(buf,ch);
		}
	}
    buf[0] = '\0';
    switch (++time_info.hour) {
		case  4:
		weather_info.moonlight = MOON_SET;
		strcat(buf, "The moon slowly slides below the horizon.\n\r");	
		break;

		case  5:
		weather_info.moonlight = MOON_DOWN;
		weather_info.time_of_day = MORNING;
		for (d = descriptor_list; d != NULL; d = d->next) {
			if (d->connected == CON_PLAYING
			&& (ch = d->character) != NULL
			&&  !IS_NPC(ch)
			&&  !IS_EXTRA(ch, EXTRA_OSWITCH)
			&&  !IS_HEAD(ch, LOST_HEAD)
			&& IS_CLASS(ch, CLASS_VAMPIRE))
				send_to_char("You feel weary as the night begins to come to an end.\n\r",ch);
		}
		break;

		case  6:
		weather_info.sunlight = SUN_RISE;
		strcat(buf, "As eastern horizon begins brightening, it banishes the evil of the night.\n\r");
		break;
	        

		case 10:
		weather_info.time_of_day = DAY;
		strcat(buf, "As the sun rises, a new day commences.\n\r");
		break;

		case 12:
		strcat(buf, "You hear chimes in the distance announce the midday.\n\r");
		break;

		case 17:
		weather_info.time_of_day = EVENING;
		strcat(buf, "The sun begins its descent towards the western horizon, the evil of night starts to awake.\n\r");
		break;	

		case 19:
		weather_info.sunlight = SUN_SET;
		strcat(buf, "The sun slowly disappears in the west, the evil of night has fully awakended, there is not a living soul to be seen.\n\r");
		break;

		case 20:
		weather_info.sunlight = SUN_DARK;
		strcat(buf, "The night has begun.\n\r");
		break;

		case 21:
		weather_info.moonlight = MOON_RISE;
		weather_info.time_of_day = NIGHT;
		if (weather_info.moonphase == MOON_CRESCENT)
			strcat(buf, "A crescent moon slowly rises above the horizon.\n\r");
		if (weather_info.moonphase == MOON_HALF)
			strcat(buf, "You see the moon, half tonight, slowly rising from the horizon.\n\r");
		if (weather_info.moonphase == MOON_GIBBOUS)
			strcat(buf, "The gibbous moon ascends, starting its nocturnal journey.\n\r");
		if (weather_info.moonphase == MOON_FULL)
			strcat(buf, "The full moon slowly ascends from its cradle, preparing to journey across the night sky.\n\r");
		if (weather_info.moonphase == MOON_WANING)
			strcat(buf, "The quickly waning moon slowly climbs to its nightly throne.\n\r");
		if (weather_info.moonphase == MOON_NEW)
			strcat(buf, "Hardly visible, the moon still ascends the horizon.\n\r");
		break;

		case 22: 
		weather_info.moonlight = MOON_UP;
		break;

		case 23:
		strcat(buf, "The moon starts its slow journey across the night sky. Stars glinting like tears trailing behind it.\n\r");
		break;

		case 24:

		time_info.hour = 0;
		time_info.day++;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    char_up = FALSE;
	    if ( d->connected == CON_PLAYING
	    &&   (ch = d->character) != NULL
	    &&  !IS_NPC(ch))
	    {
                if (ch->fighting == NULL && !IS_SET(ch->newbits, NEW_NATURAL)
		&& ch->monkab[SPIRIT] >= 2 )
                {
                    send_to_char("Your natural harmony returns.\n\r", ch);
                    SET_BIT(ch->newbits, NEW_NATURAL);
                }

		send_to_char( "You hear a clock in the distance strike midnight.\n\r",ch);
	 	if (IS_SET(ch->flag2, AFF2_ROT) && !IS_SET(ch->warp, WARP_INFIRMITY))
                {
                        send_to_char("Your flesh feels better.\n\r", ch);
                        REMOVE_BIT(ch->flag2, AFF2_ROT);
                        
                }
if ( IS_SET(ch->garou1, GAROU_PAWS) )
{
stc("You are no longer affected by paws of the newborn cub.\n\r",ch);
REMOVE_BIT(ch->garou1, GAROU_PAWS);
}
 if(IS_SET(ch->garou1,WOLF_QUICKSAND))
 {
   stc("You lose your concentration on the land around you.\n\r",ch);
   do_quicksand(ch,"");
 }

 if (IS_SET(ch->newbits,  NEW_VISCERAL))
                {
                        send_to_char("You feel the venom leave your body.\n\r", ch);
                        REMOVE_BIT(ch->newbits, NEW_VISCERAL);

                }
if (IS_SET(ch->newbits, NEW_VIOLATION))
{
stc("Your emotions return to normal.\n\r", ch);
REMOVE_BIT(ch->newbits, NEW_VIOLATION);
}

if (IS_SET(ch->flag2, AFF2_BALOR))
{
stc("You no longer feel the pain of Balor's Gaze.\n\r", ch);
REMOVE_BIT(ch->flag2, AFF2_BALOR);
}
                if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
                {
                send_to_char("The silence leaves the room.\n\r",ch);
                act("The silence leaves the room.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_SILENCE);
                
                }

                if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING))
                {
                send_to_char("The flames in the room die down.\n\r",ch);
                act("The flames in the room die down.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->in_room->room_flags,ROOM_FLAMING);
                }	
/* Changed Toaster 29/04/00 Vampires now full regen midnight and no
fight_timer*/	
	if ( IS_VAMPIRE(ch) && ch->fight_timer <= 0 )
		{
		    if (ch->hit < ch->max_hit)
			{ ch->hit = ch->max_hit; char_up = TRUE; }
		    if (ch->mana < ch->max_mana)
			{ ch->mana = ch->max_mana; char_up = TRUE; }
		    if (ch->move < ch->max_move)
			{ ch->move = ch->max_move; char_up = TRUE; }
		    if (char_up) send_to_char( "You feel the strength of the kindred flow through your veins!\n\r", ch );
		    ch->position = POS_STANDING;
		}
		    if (IS_EXTRA(ch, EXTRA_POTENCY))
		    { 
			send_to_char("You feel your blood potency fade away.\n\r", ch);	
			ch->pcdata->stats[UNI_GEN]++;
			REMOVE_BIT(ch->extra, EXTRA_POTENCY);
		    }

                    if (IS_SET(ch->newbits, NEW_TIDE))
 	            {
 		     REMOVE_BIT(ch->newbits, NEW_TIDE);
                     send_to_char("The tide of vitae leaves you.\n\r", ch);
		    }	    

}
	}
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }



    /*
     * Weather change.
     */
    if (time_info.month >= 9 && time_info.month <= 16)
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch (weather_info.sky)
    {
    default: 
	bug("Weather_update: bad sky %d.", weather_info.sky);
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if (weather_info.mmhg <  990
	|| (weather_info.mmhg < 1010 && number_bits(2) == 0))
	{
	    strcat(buf, "The sky is getting cloudy.\n\r");
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if (weather_info.mmhg <  970
	|| (weather_info.mmhg <  990 && number_bits(2) == 0))
	{
	    strcat(buf, "It starts to rain.\n\r");
	    weather_info.sky = SKY_RAINING;
	}

	if (weather_info.mmhg > 1030 && number_bits(2) == 0)
	{
	    strcat(buf, "The clouds disappear.\n\r");
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if (IS_SET(world_affects, WORLD_RAIN))
	{
	    if (weather_info.mmhg <  970)
		weather_info.mmhg += 10;
	    else if (weather_info.mmhg > 1010)
		weather_info.mmhg -= 10;
	    strcat(buf, "The rain pours down heavily from the sky.\n\r");
	    break;
	}
	if (weather_info.mmhg <  970 && number_bits(2) == 0)
	{
	    strcat(buf, "Lightning flashes in the sky.\n\r");
	    weather_info.sky = SKY_LIGHTNING;
	}

	if (weather_info.mmhg > 1030
	|| (weather_info.mmhg > 1010 && number_bits(2) == 0))
	{
	    strcat(buf, "The rain stopped.\n\r");
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if (weather_info.mmhg > 1010
	|| (weather_info.mmhg >  990 && number_bits(2) == 0))
	{
	    strcat(buf, "The lightning has stopped.\n\r");
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if (buf[0] != '\0')
    {
	for (d = descriptor_list; d != NULL; d = d->next)
	{
/* Changed by Tracker, damn annoying messages 03/13/03 */
//	    if (d->connected == CON_PLAYING
//	    &&   IS_OUTSIDE(d->character)
//	    &&   IS_AWAKE(d->character))
//		send_to_char(buf, d->character);
	}
    }

    return;
}

/*
void embrace_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *ch;
    CHAR_DATA       *victim;
    char buf[MAX_STRING_LENGTH];
    for ( d = descriptor_list; d != NULL; d = d->next )
      {
      if ((ch = d->character) == NULL
        || IS_NPC(ch)
        || ch->embracing==NULL)
	{
            continue;
      }

	if (!IS_CLASS(ch,CLASS_VAMPIRE))
	  stop_embrace(ch,NULL);

	victim=ch->embracing;

	if (get_char_world(ch,victim->name) ==NULL)
	{stop_embrace(ch,NULL);continue;}

	if (ch->in_room != victim->in_room)
	  stop_embrace(ch,victim);

      if (IS_NPC(victim) )
      {
	  ch->pcdata->condition[COND_THIRST] += number_range(40,50);
        act("$N falls to the ground lifeless.",ch,NULL,victim,TO_CHAR);
        act("$N falls to the ground lifeless.",ch,NULL,victim,TO_ROOM);
        stop_embrace(ch,victim);
        raw_kill(victim, ch);
        return;
      }

	if (victim->pcdata->condition[COND_THIRST] < 0)
	  victim->pcdata->condition[COND_THIRST] = 0;

        ch->pcdata->condition[COND_THIRST] += number_range(30,40);
        sprintf(buf,"%s shudders in ecstacy as he drinks blood from %s's neck.\n\r",ch->name,victim->name);
	  act(buf,ch,NULL,NULL,TO_ROOM);
	  sprintf(buf,"You shudder in ecstacy as you drink blood from %s's neck.\n\r",victim->name);
	  send_to_char(buf,ch);
	  sprintf(buf,"You feel some of your life slip away as %s drinks from your neck.\n\r",ch->name);
	  send_to_char(buf,victim);
	  ch->pcdata->condition[COND_THIRST] += number_range(35,40);
	  victim->pcdata->condition[COND_THIRST] -= number_range(40,42);
	  if (ch->pcdata->condition[COND_THIRST] > 2000/ch->pcdata->stats[UNI_GEN])
	{ch->pcdata->condition[COND_THIRST]=2000/ch->pcdata->stats[UNI_GEN];
	  send_to_char("Your bloodlust is sated.\n\r",ch);}
	  if (victim->pcdata->condition[COND_THIRST] < 0)
	    victim->pcdata->condition[COND_THIRST]=0;

	  if (victim->pcdata->condition[COND_THIRST] ==0)
	  {
	    sprintf(buf,"You have been diablerized!\n\r");
	    send_to_char(buf,victim);
	    raw_kill(victim, ch);
	    sprintf(buf,"%s's body falls lifeless to the ground.\n\r",victim->name);
	    act(buf,ch,NULL,NULL,TO_ROOM);
	    send_to_char(buf,ch);
    	    stop_embrace(ch,victim);
  	    if (victim->pcdata->stats[UNI_GEN] < ch->pcdata->stats[UNI_GEN] && ch->pcdata->stats[UNI_GEN] > 3)
  	    {
		sprintf(buf,"%s has been diablerized by %s.",victim->name,ch->name);
  	      ch->pcdata->stats[UNI_GEN] -= 1;
  		victim->pcdata->stats[UNI_GEN] += 1;
  		victim->pcdata->condition[COND_THIRST]=0;
  		do_info(ch,buf);
	    }
  	else
  	{
  	  sprintf(buf,"%s has been diablerized by %s for no generation.", victim->name,ch->name);
  	  victim->pcdata->condition[COND_THIRST]=0;
  	  do_info(ch,buf);
  	}

}
continue; 
 
 
 
      }
    return;
}
*/


/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{   
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    bool is_obj;
    bool drop_out = FALSE;
    time_t save_time;
    int i;

    save_time	= current_time;
    ch_save	= NULL;
    ch_quit	= NULL;
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	ch_next = ch->next;

	if (!IS_NPC(ch) && (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)))
	    is_obj = TRUE;
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	    {is_obj = TRUE;SET_BIT(ch->extra, EXTRA_OSWITCH);}
	else
	    is_obj = FALSE;



	/*
	 * Find dude with oldest save time.
	 */
	if ( !IS_NPC(ch)
	&& ( ch->desc == NULL || ch->desc->connected == CON_PLAYING )
	&&   ch->level >= 2
	&&   ch->save_time < save_time )
	{
	    ch_save	= ch;
	    save_time	= ch->save_time;
	}


    if (!IS_HERO(ch) && ch->hit < 1 && ch->position != POS_FIGHTING)
		{
		    char_from_room(ch);
		    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
		    act("The healer makes a few gestures and $n appears in the room.",ch,NULL,NULL,TO_ROOM);
		    ch->hit = 1;
		    update_pos(ch);
		    send_to_char("You feel a little better.\n\r",ch);
		    act("The healer says 'Are you ok $n?  Lucky I was here!'.",ch,NULL,NULL,TO_ROOM);
		    act("The healer says 'Are you ok $n?  Lucky I was here!'.",ch,NULL,NULL,TO_CHAR);
		}

	if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_SHAKRIN)) ch->exp = ch->exp + 1;

        if ( is_garou(ch) && ch->gifts[UKTENA] >= 2 && !IS_AFFECTED(ch, AFF_FLYING) )
	  SET_BIT(ch->affected_by, AFF_FLYING);

	if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[HARA_KIRI] > 0 )
	  ch->pcdata->powerss[HARA_KIRI]--;

       if ( IS_SET(ch->garou1, GAROU_WHELP2) )
       {
	 if ( number_range( 1, 3 ) == 2 )
         {
	   REMOVE_BIT(ch->garou1, GAROU_WHELP2);
	   ch->max_hit += 1500;
           stc("Your body feels slightly stronger.\n\r",ch);
	   SET_BIT(ch->garou1, GAROU_WHELP1);
         }
       }
       if ( IS_SET(ch->garou1, GAROU_WHELP1) )
       {
         if (number_range(1,3) == 2 )
         {
	   REMOVE_BIT(ch->garou1, GAROU_WHELP1);
	   ch->max_hit += 1500;
           stc("You feel the Whelp's Curse leave your body.\n\r",ch);
         }
       }	

        if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF)
          && ch->gnosis[GMAXIMUM] > 0 && ch->gnosis[GMAXIMUM]
           > ch->gnosis[GCURRENT] )
        {
          if ( ch->position == POS_SLEEPING )
            ch->gnosis[GCURRENT] += number_range(1,10);
          else if ( ch->position == POS_RESTING )
            ch->gnosis[GCURRENT] += number_range(1,9);
          else if ( ch->position == POS_FIGHTING )
            ch->gnosis[GCURRENT] += 0;
          else
            ch->gnosis[GCURRENT] += number_range( 1, 8 );
	}
	if (ch->gnosis[GCURRENT] >  ch->gnosis[GMAXIMUM]) 
	{
	    (ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM]);
	}
/*
        if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NINJAKI] < 300 )
        {
            if ( ch->position == POS_MEDITATING && !IS_AFFECTED(ch,AFF_HIDE) )
            {
               ch->pcdata->powerss[NINJAKI] += number_range( 50, 70 );
	       if ( ch->pcdata->powerss[NINJAKI] > 300 )
	  	   ch->pcdata->powerss[NINJAKI] = 300;
            }
        }*/

	if ( IS_SET(ch->mflags, MAGE_EMBRACED2) )
        {
          stc("The earth surrounding you starts to slowly crack.\n\r",ch);
	  SET_BIT(ch->mflags, MAGE_EMBRACED1);
          REMOVE_BIT(ch->mflags, MAGE_EMBRACED2);
        }
        else if ( IS_SET(ch->mflags, MAGE_EMBRACED1) )
        {
          stc("The earth surrounding you falls away.\n\r",ch);
          REMOVE_BIT(ch->mflags, MAGE_EMBRACED1);
        }

        if ( ch->quint[BODY] < (ch->spheres[MPRI]) )
          ch->quint[BODY]++;
        if ( ch->quint[AVATAR] < (ch->spheres[MPRI]) )
          ch->quint[AVATAR]++;
        

	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast > 0
	  && ch->pcdata->condition[COND_THIRST] <= 15 )
	{
	  act("You bare your fangs and scream in rage from lack of blood.",ch,NULL,NULL,TO_CHAR);
	  act("$n bares $s fangs and screams in rage.",ch,NULL,NULL,TO_ROOM);
          beast_rage(ch);
	  do_beastlike(ch,"");
	}

	  /* Add tick messages here..
	      if ( TIMER(ch, TIMER_WHATEVER) == 1 )
	        stc("Your whatever timer has worn off.\n\r",ch);
 	  */
	  if ( !IS_NPC(ch) )
          {
/* --> */
	  }

	if ( !IS_NPC(ch) )
	{
	  for ( i = 0; i < MAX_TIMER; i++ )
	    if ( ch->pcdata->tick_timer[i] > 0 ) ch->pcdata->tick_timer[i]--;
	}	

	if ( ch->position > POS_STUNNED && !is_obj)
	{
	    if ( ch->hit  < ch->max_hit )
		ch->hit  += hit_gain(ch);

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	}

	if ( ch->position == POS_STUNNED && !is_obj)
	{
            ch->hit = ch->hit + number_range(2,4);
	    update_pos( ch );
/*
            if (ch->position > POS_STUNNED)
            {
                act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM );
                act( "You clamber back to your feet.", ch, NULL, NULL, TO_CHAR );
            }
*/
	}

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL && !is_obj)
	{
	    OBJ_DATA *obj;
	    int blood;

	    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    || ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 ) )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    --ch->in_room->light;
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	    }

	    if ( ++ch->timer >= 12)
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		}
	    }

	    if ( ch->timer > 30 ) ch_quit = ch;

	    gain_condition( ch, COND_DRUNK,  -1 );
	    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	    {
	    	gain_condition( ch, COND_FULL, -1 );
	    	gain_condition( ch, COND_THIRST, -1 );
	    }
	    else
	    {
                if (ch->pcdata->condition[COND_THIRST] > 0) ch->pcdata->condition[COND_THIRST]--;
                if (ch->hit > 0 && ch->pcdata->condition[COND_THIRST] < 1)
                {
                    send_to_char( "You are DYING from lack of blood!\n\r", ch );
                    act( "$n gets a hungry look in $s eyes.", ch, NULL, NULL, TO_ROOM );
                    if (ch->hit > 0) ch->hit -= number_range(2,5);
                    update_pos(ch);
                    if (number_percent() <= ch->beast && ch->beast > 0) vamp_rage(ch);
                    if (!IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
                }
                else if (ch->hit > 0 && ch->pcdata->condition[COND_THIRST] <= 20)
                {
                    send_to_char( "You crave blood.\n\r", ch );
                    if (number_range(1,1000) <= ch->beast && ch->beast > 0) vamp_rage(ch);
                    if (number_percent() > (ch->pcdata->condition[COND_THIRST]+75)
                        && !IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
                }   
		blood = -1;

		if (IS_POLYAFF(ch, POLY_NEWZULO)) blood -= number_range(30,40);
		if (ch->beast > 0)
		{
		    if (IS_VAMPAFF(ch, VAM_CLAWS)) blood -= number_range(1,3);
		    if (IS_VAMPAFF(ch, VAM_FANGS)) blood -= 1;
		    if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) blood -= 1;
		    if (IS_VAMPAFF(ch, AFF_SHADOWSIGHT)) blood -= number_range(1,3);
		    if (IS_SET(ch->act, PLR_HOLYLIGHT)) blood -= number_range(1,5);
		    if (IS_VAMPAFF(ch, VAM_DISGUISED)) blood -= number_range(5,10);
		    if (IS_VAMPAFF(ch, VAM_CHANGED)) blood -= number_range(5,10);
		    if (IS_VAMPAFF(ch, IMM_SHIELDED)) blood -= number_range(1,3);
		    if (IS_POLYAFF(ch, POLY_SERPENT)) blood -= number_range(1,3);
/* Changed Toaster 03/05/00 Being i Shadowplane now costs vampires upkeep */
		    if(IS_AFFECTED(ch, AFF_SHADOWPLANE)) blood -= number_range(5,10);
		    if (ch->beast == 100) blood *= 2;
		}
		gain_condition( ch, COND_THIRST, blood );

	    }


	}

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
		paf->duration--;
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off && !is_obj)
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
			if (IS_SET(ch->in_room->room_flags,
ROOM_TOTAL_DARKNESS)) REMOVE_BIT(ch->in_room->room_flags,
ROOM_TOTAL_DARKNESS);
		    }
		}
	  
		affect_remove( ch, paf );
	    }
	}

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */
	if ( ch->loc_hp[6] > 0 && !is_obj && ch->in_room != NULL )
	{
	    int dam = 0;
	    int minhit = 0;
	    if (!IS_NPC(ch)) minhit = -11;
	    if (IS_BLEEDING(ch,BLEEDING_HEAD) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's neck.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your neck.\n\r", ch );
		dam += number_range(20,50);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_THROAT) && (ch->hit-dam) > minhit )
	    {
		act( "Blood pours from the slash in $n's throat.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "Blood pours from the slash in your throat.\n\r", ch );
		dam += number_range(10,20);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_HERO(ch)) { ch->hit = ch->hit - dam;
		if (ch->hit < 1) ch->hit = 1; }
	    else ch->hit = ch->hit - dam;
	    update_pos(ch);
	    ch->in_room->blood += dam;
	    if (ch->in_room->blood > 1000) ch->in_room->blood = 1000;
	    if (ch->hit <=-11 || (IS_NPC(ch) && ch->hit < 1))
	    {


		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}
	
if (IS_SET(ch->newbits, NEW_VISCERAL) && !is_obj && !drop_out)
{
	 int dam;
         if (IS_NPC(ch)) break;
         act( "$n winces in pain.", ch, NULL, NULL, TO_ROOM);
         send_to_char( "You wince in pain as venom spreads throughout your system.\n\r", ch);
         dam = number_range(250,500);
	 ch->hit = ch->hit - dam;
         update_pos(ch);
         if (ch->hit < -10)
         {
            do_killperson(ch,ch->name);
            drop_out = TRUE;
         }
        }
	if (IS_SET(ch->flag2,AFF2_ROT) && !is_obj && !drop_out)
	{
	 int dam;
	 if (IS_NPC(ch)) break;
         act( "$n's flesh shrivels and tears.", ch, NULL, NULL, TO_ROOM);
	 send_to_char( "Your flesh shrivels and tears.\n\r", ch);
         dam = number_range(250,500);
         ch->hit = ch->hit - dam;
         update_pos(ch);
         if (ch->hit < -10)
         {
            do_killperson(ch,ch->name);
            drop_out = TRUE;
         }
        }


 if (IS_SET(ch->flag2,AFF2_BALOR) && !is_obj && !drop_out) 
     {
         int dam;
         if (IS_NPC(ch)) break;
         act( "$n scream in pain as his body is torn apart from the inside out!", ch, NULL, NULL, TO_ROOM);
         send_to_char( "You scream in pain as your body is torn apart from the inside out.\n\r", ch);
         dam = number_range(250,500);
         ch->hit = ch->hit - dam;
         update_pos(ch);
         if (ch->hit < -10)
         {
            do_killperson(ch,ch->name);
            drop_out = TRUE;
         }
        }


	if ( IS_AFFECTED(ch, AFF_FLAMING) && !is_obj && !drop_out && ch->in_room != NULL )
	{
	    int dam;
	    if (!IS_NPC(ch) && IS_HERO(ch)) break;
	    if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_HEAT) &&
		!IS_CLASS(ch, CLASS_VAMPIRE)) break;
	    act( "$n's flesh burns and crisps.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "Your flesh burns and crisps.\n\r", ch );
	    dam = number_range(10,20);
	    if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_HEAT)) dam /= 2;
	    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE)) dam *= 2;
	    ch->hit = ch->hit - dam;
	    update_pos(ch);
	    if (ch->hit <=-11)
	    {
		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}
	else if ( IS_CLASS(ch, CLASS_VAMPIRE) && (!IS_AFFECTED(ch,AFF_SHADOWPLANE)) &&
	    (!IS_NPC(ch) && !IS_IMMUNE(ch,IMM_SUNLIGHT)) && ch->in_room != NULL &&
	    (!ch->in_room->sector_type == SECT_INSIDE) && !is_obj &&
	    (!room_is_dark(ch->in_room)) && (weather_info.sunlight != SUN_DARK) )
	{
	    act( "$n's flesh smolders in the sunlight!", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "Your flesh smolders in the sunlight!\n\r", ch );
	    /* This one's to keep Zarkas quiet ;) */
	    if (IS_POLYAFF(ch, POLY_SERPENT))
	    	ch->hit = ch->hit - number_range(2,4);
	    else
	    	ch->hit = ch->hit - number_range(5,10);
	    update_pos(ch);
	    if (ch->hit <=-11)
	    {
		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}
	else if ( IS_AFFECTED(ch, AFF_POISON) && !is_obj && !drop_out )
	{
	    act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You shiver and suffer.\n\r", ch );
	    damage( ch, ch, 2, gsn_poison );
	}
	else if ( !IS_NPC( ch ) && ch->paradox[1] > 0 )
	{
	    if ( ch->paradox[1] > 2999 /* lower to 2 to add paradox - Vic */ ) paradox( ch );
	    else if ( ch->paradox[2] == 0 && ch->paradox[1] > 0 )
	    {
		ch->paradox[1] --;
		ch->paradox[2] = PARADOX_TICK;
	    }
	    else ch->paradox[3] --;
	}

	else if ( ch->position == POS_INCAP && !is_obj && !drop_out )
	{
	    if (ch->level > 0)
                ch->hit = ch->hit + number_range(2,4);
	    else
                ch->hit = ch->hit - number_range(1,2);
	    update_pos( ch );
            if (ch->position > POS_INCAP)
            {
                act( "$n's wounds stop bleeding and seal up.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "Your wounds stop bleeding and seal up.\n\r", ch );
            }
            if (ch->position > POS_STUNNED)
            {
                act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You clamber back to your feet.\n\r", ch );
            }
	}

	else if ( ch->position == POS_MORTAL && !is_obj && !drop_out )
	{
	    drop_out = FALSE;
	    if (ch->level > 0)//right here also
                ch->hit = ch->hit + number_range(2,4);
//recommented by sage
	    else
	    {
                ch->hit = ch->hit - number_range(1,2);
		if (!IS_NPC(ch) && (ch->hit <=-11))
		    do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	    if (!drop_out)
	    {
	    	update_pos( ch );
// lala
            	if (ch->position == POS_INCAP)
            	{
                    act( "$n's wounds begin to close, and $s bones pop back into place.", ch, NULL, NULL, TO_ROOM );
                    send_to_char( "Your wounds begin to close, and your bones pop back into place.\n\r", ch );
            	}
	    }
	}
	else if ( ch->position == POS_DEAD && !is_obj && !drop_out )
	{
	   update_pos(ch);
	   if (!IS_NPC(ch))
		do_killperson(ch,ch->name);
	}
	drop_out = FALSE;
    }
/*

//new regen shit hope it works? SAGE
	else if ( ch->position == POS_INCAP && !is_obj && !drop_out )
	{
	    if (IS_HERO(ch))
                ch->hit = ch->hit + number_range(2,4);
	    else
                ch->hit = ch->hit - number_range(1,2);
	    update_pos( ch );
            if (ch->position > POS_INCAP)
            {
                act( "$n's wounds stop bleeding and seal up.", ch, NULL,
NULL, TO_ROOM );
                send_to_char( "Your wounds stop bleeding and seal up.\n\r", ch );
            }
            if (ch->position > POS_STUNNED)
            {
                act( "$n clambers back to $s feet.", ch, NULL, NULL,
TO_ROOM );
                send_to_char( "You clamber back to your feet.\n\r", ch );
            }
	}
	else if ( ch->position == POS_MORTAL && !is_obj && !drop_out )
	{
	    drop_out = FALSE;
	    if (IS_HERO(ch))
                ch->hit = ch->hit + number_range(2,4);
	    else
	    {
                ch->hit = ch->hit - number_range(1,2);
		if ((!IS_NPC(ch) && ch->hit<-10) || (IS_NPC(ch) &&
ch->hit<0))
		{
		    ch->fatal = 16;
		killperson(ch,ch);	    
	//do_killperson(ch,ch);
		    continue;
		}
	    }
	    if (!drop_out)
	    {
	    	update_pos( ch );
            	if (ch->position == POS_INCAP)
            	{
                    act( "$n's wounds begin to close, and $s bones pop back into place.", ch, NULL, NULL, TO_ROOM );
                    send_to_char( "Your wounds begin to close, and your bones pop back into place.\n\r", ch );
            	}
	    }
	}
	else if ( ch->position == POS_DEAD && !is_obj && !drop_out )
	{
	    if (!IS_NPC(ch)) 
	    {
//		ch->form = 16;
		killperson(ch,ch);
		//do_killperson(ch,ch);
		continue;
	    }
	}
	drop_out = FALSE;
    }
2*/
    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    if ( ch_save != NULL || ch_quit != NULL )
    {
	for ( ch = char_list; ch != NULL; ch = ch_next )
	{
	    ch_next = ch->next;
	    if ( ch == ch_save )
		save_char_obj( ch );
	    if ( ch == ch_quit )
		do_quit( ch, "" );
	}
    }

    return;
}


void quest( void )
{
    static const sh_int rvnum[] =
    {
         109,  2210, 19192, 2396, 2429, 1477, 7900,  601,
        9103, 25008,   931, 9326, 9236, 2103, 2350
    };
    static const sh_int ovnum[] =
    {
	200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
	210, 211
    };
    static const sh_int mvnum[] =
    {
	200, 201
    };
    OBJ_DATA *obj;
    CHAR_DATA *ch;
    int rrandom;
    int orandom;
    int mrandom;

    rrandom = number_range(0,14);
    orandom = number_range(0,11);
    mrandom = number_range(0,1);
    obj = create_object(get_obj_index(ovnum[orandom]),100);
    ch = create_mobile(get_mob_index(mvnum[mrandom]));
    obj_to_char(obj,ch);
    char_to_room(ch,get_room_index(rvnum[rrandom]));
    SET_BIT(ch->special,SPC_NOFIND);
    SET_BIT(obj->quest,ITEM_EQUEST);
    obj->cost = 15;

    return;
}


void obj_update( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
//    char buf[MSL];
    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;

	if ( obj->timer <= 0 || --obj->timer > 0 ){
/*	if((obj->item_type == ITEM_CORPSE_NPC 
	  || obj->item_type == ITEM_CORPSE_PC) && obj->in_obj == 0 && obj!=NULL)
//	alter_corpse(obj);*/
	
	    continue;}

	switch ( obj->item_type )
	{
	default:              message = "$p vanishes.";         	break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         	break;
	case ITEM_CORPSE_NPC: message = "$p decays into dust.";		break;
/*
	case ITEM_CORPSE_PC:  sprintf(buf,"%s is wholly consumed by maggots.",
				obj->name);
				message = buf;	break;
*/	case ITEM_CORPSE_PC:  message = "$p decays into dust.";		break;
	case ITEM_FOOD:       message = "$p decomposes.";		break;
	case ITEM_TRASH:      message = "$p crumbles into dust.";	break;
	case ITEM_EGG:        message = "$p cracks open.";		break;
	case ITEM_WEAPON:     message = "The poison on $p melts through it.";		break;
	 case ITEM_WALL:       message = "$p flows back into the ground.";break;
	}

	if ( obj->carried_by != NULL )
	{
	    act( message, obj->carried_by, obj, NULL, TO_CHAR );
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    act( message, rch, obj, NULL, TO_ROOM );
	    act( message, rch, obj, NULL, TO_CHAR );
	}

	/* If the item is an egg, we need to create a mob and shell!
	 * KaVir
	 */
	if ( obj->item_type == ITEM_EGG )
	{
	    CHAR_DATA      *creature;
	    OBJ_DATA       *egg;
	    if ( get_mob_index( obj->value[0] ) != NULL )
	    {
		if (obj->carried_by != NULL && obj->carried_by->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->carried_by->in_room);
		}
		else if (obj->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->in_room);
		}
		else
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,get_room_index(ROOM_VNUM_HELL));
/*
		    obj->timer = 1;
		    continue;
*/
		}
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, creature->in_room );
	    	act( "$n clambers out of $p.", creature, obj, NULL, TO_ROOM );
	    }
	    else if (obj->in_room != NULL)
	    {
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, obj->in_room );
	    }
	}
	if (obj != NULL) extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't want the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim = NULL;

    OBJ_DATA *obj = NULL;
    OBJ_DATA *chobj = NULL;
    ROOM_INDEX_DATA *objroom = NULL;
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&& ( obj = ch->pcdata->chobj ) != NULL )
	{
	    if (obj->in_room != NULL)
	    	objroom = obj->in_room;
	    else if (obj->in_obj != NULL)
	    	objroom = get_room_index(ROOM_VNUM_IN_OBJECT);
	    else if (obj->carried_by != NULL)
	    {
		if (obj->carried_by != ch && obj->carried_by->in_room != NULL)
		    objroom = obj->carried_by->in_room;
		else continue;
	    }
	    else continue;
	    if (ch->in_room != objroom && objroom != NULL)
	    {
	    	char_from_room(ch);
	    	char_to_room(ch,objroom);
		do_look(ch,"auto");
	    }
	}
	else if ( d->connected == CON_PLAYING
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&&   (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0) )
	{
	    if (ch->pcdata->obj_vnum != 0)
	    {
		bind_char(ch);
		continue;
	    }
	    if (IS_HEAD(ch,LOST_HEAD))
	    {
	    	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	    	send_to_char("You are able to regain a body.\n\r",ch);
	    	ch->position = POS_RESTING;
	    	ch->hit = 1;
	    } else {
	    	send_to_char("You return to your body.\n\r",ch);
		REMOVE_BIT(ch->extra,EXTRA_OSWITCH);}
	    REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	    if ( ( chobj = ch->pcdata->chobj ) != NULL )
		chobj->chobj = NULL;
	    ch->pcdata->chobj = NULL;
	    do_look(ch,"auto");
	}
	continue;
    }

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;
	if ( IS_NPC(wch)
	|| ( wch->desc != NULL && wch->desc->connected != CON_PLAYING )
	||   wch->position <= POS_STUNNED
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->pcdata == NULL
	|| ( ( chobj = wch->pcdata->chobj ) != NULL )
	||   wch->in_room == NULL )
	    continue;

	if ( (IS_CLASS(wch, CLASS_WEREWOLF) && IS_HERO(wch)) ||
	    IS_ITEMAFF(wch, ITEMA_STALKER) )
	{
	    if ( wch->hunting != NULL && wch->hunting != '\0' && 
		strlen(wch->hunting) > 1 && wch->pcdata->powers[WPOWER_LYNX] > 1)
	    {
		ROOM_INDEX_DATA *old_room = wch->in_room;
		check_hunt( wch );
		if (wch->in_room == old_room)
		{
		    free_string(wch->hunting);
		    wch->hunting = str_dup( "" );
		    continue;
		}
		check_hunt( wch );
		if (wch->in_room == old_room)
		{
		    free_string(wch->hunting);
		    wch->hunting = str_dup( "" );
		}
		continue;
	    }
	}

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)
	    ||   no_attack(ch, wch)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	||   IS_SET(wch->in_room->room_flags,ROOM_SAFE)

	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch ) )
		continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   !no_attack(ch, vch)
		&&   vch->pcdata != NULL
		&& ( ( chobj = vch->pcdata->chobj ) == NULL )
		&&   vch->level < LEVEL_IMMORTAL
		&&   vch->position > POS_STUNNED
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch ) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
	    {
		continue;
	    }
	    if ( is_garou(victim) && victim->gifts[PHILODOX] >= 2 )
	        continue;

            if (IS_EXTRA(victim, EXTRA_AWE)) continue;

	    multi_hit( ch, victim, TYPE_UNDEFINED );
	}
    }

    return;
}


/*added by sage for embrace*/

void embrace_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *ch;
    CHAR_DATA       *victim;
    int blpr;  /* variable to check for amout of blood sucked. Shakti */
    char buf[MAX_STRING_LENGTH];
               int chargen;
                int maxblood;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
      
      if ( ( ch = d->character) == NULL )
	continue;

    if ( IS_CLASS(ch, CLASS_MONK) && ch->fighting == NULL )
        ch->monkblock = 0;
    
      if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK)
          && ch->focus[MAXIMUM] > 0 && ch->focus[MAXIMUM]
           > ch->focus[CURRENT] )
        {
          if ( ch->position == POS_SLEEPING )
            ch->focus[CURRENT] += number_range(1, 10);
          else if ( ch->position == POS_MEDITATING )
            ch->focus[CURRENT] += number_range( 4, 5 );
          else if ( ch->position == POS_RESTING )
            ch->focus[CURRENT] += number_range(1,9);
          else if ( ch->position == POS_FIGHTING )
            ch->focus[CURRENT] += 0;
          else
            ch->focus[CURRENT] += number_range( 1, 8 );
        }
      if ( IS_CLASS(ch, CLASS_MONK) && ch->focus[CURRENT] > ch->focus[MAXIMUM])    
         ch->focus[CURRENT] = ch->focus[MAXIMUM];
 
      if ( IS_NPC(ch) || ch->embracing == NULL )
        continue;

	if (!IS_CLASS(ch,CLASS_VAMPIRE))
	  stop_embrace(ch,NULL);

	victim=ch->embracing;


	if (IS_NPC(victim) )
	{
		if (victim->level > 75 )
		{
			stop_embrace(ch,victim);
			send_to_char("You cannot embrace someone so powerful!\n\r",ch);
			return;
		}
		
            if (victim->in_room != ch->in_room)
            {
                stc("Your victim is no longer in the room.\n\r", ch);
            stop_embrace(ch,victim);
                return;
            }
	
		(blpr =number_range(30,40));
		victim->practice -=blpr;
		ch->pcdata->condition[COND_THIRST] +=blpr;
		
		sprintf(buf, "%s shudders in ecstacy as he drinks blood from %s's neck.\n\r", ch->name,victim->short_descr);
	    act(buf,ch,NULL,NULL,TO_ROOM);
	    sprintf(buf, "You shudder in ecstacy as you drink blood from %s's neck.\n\r",victim->short_descr);
	    send_to_char(buf,ch);
/*	    
		if ( ch->pcdata->condition[COND_THIRST] >
		    (2000 / ch->pcdata->stats[UNI_GEN]) )

		{
		  stc("Your blood thirst is sated.\n\r",ch);
		  ch->pcdata->condition[COND_THIRST] = (2000/ch->pcdata->stats[UNI_GEN]);
                }
*/
               chargen = ch->pcdata->stats[UNI_GEN];
                if (chargen > 12)
                        chargen = 12;
                maxblood = (13 - chargen) * 100;
                if (ch->pcdata->condition[COND_THIRST] >= maxblood)
                        send_to_char( "Your blood thirst is sated.\n\r", ch);
        
                ch->pcdata->condition[COND_THIRST] = maxblood;
                    
		if (victim->practice < 0)
		{
			victim->practice = 0;
		}

		if (victim->practice == 0)
		
		{
	     raw_kill(victim, ch);
	     sprintf(buf,"%s's body falls lifless to the ground!.\n\r",victim->short_descr);
	     act(buf,ch,NULL,NULL,TO_ROOM);
	     send_to_char(buf,ch);
	     stop_embrace(ch,victim);
		 return;
		}
	 
		continue;
	}



	if (get_char_world(ch,victim->name) == NULL)
	  {stop_embrace(ch,NULL);
return;
//continue;
}
	if (ch->in_room != victim->in_room)
	  stop_embrace(ch,victim);
	if (victim->pcdata->condition[COND_THIRST] < 0)
	  victim->pcdata->condition[COND_THIRST] = 0;
       
	ch->pcdata->condition[COND_THIRST] += number_range(30,40);
    sprintf(buf, "%s shudders in ecstacy as he drinks blood from %s's neck.\n\r", ch->name,victim->name);
	act(buf,ch,NULL,NULL,TO_ROOM);
	sprintf(buf, "You shudder in ecstacy as you drink blood from %s's neck.\n\r",victim->name);
	send_to_char(buf,ch);
	sprintf(buf, "You feel some of your life slip away as %s drinks from your neck.\n\r", ch->name);
	send_to_char(buf,victim);
	ch->pcdata->condition[COND_THIRST] += number_range(35,40);
	victim->pcdata->condition[COND_THIRST] -= number_range(40,42);
	if (ch->pcdata->condition[COND_THIRST] > 2000/ch->pcdata->stats[UNI_GEN]);

{ch->pcdata->condition[COND_THIRST]=2000/ch->pcdata->stats[UNI_GEN];
	send_to_char("Your bloodlust is sated.\n\r",ch);}
	if (victim->pcdata->condition[COND_THIRST] < 0)
	  victim->pcdata->condition[COND_THIRST]=0;
	if (victim->pcdata->condition[COND_THIRST] ==0)
	{
	  sprintf(buf,"You have been diablerized!\n\r");
	  send_to_char(buf,victim);
	  raw_kill(victim, ch);
	  sprintf(buf,"%s's body falls lifeless to the ground.\n\r",victim->name);
	  act(buf,ch,NULL,NULL,TO_ROOM);
	  send_to_char(buf,ch);
	  stop_embrace(ch,victim);
     	  if (victim->pcdata->stats[UNI_GEN] < ch->pcdata->stats[UNI_GEN] && ch->pcdata->stats[UNI_GEN] > 3)
  	  {
	    sprintf(buf,"%s has been diablerized by %s.",victim->name,ch->name);
	    ch->pcdata->stats[UNI_GEN] -= 1;
	    victim->pcdata->stats[UNI_GEN] += 1;
	    victim->pcdata->condition[COND_THIRST] = 0;
	    do_info(ch,buf);
	  }
	  else
  	  {
	    sprintf(buf,"%s has been diablerized by %s for no generation.", victim->name,ch->name);
	    victim->pcdata->condition[COND_THIRST] = 0;
	    do_info(ch,buf);
	  }
    }
    continue; 
  }
  return;
}





void ww_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *victim;
//    float            dam = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
      {
      if (!IS_PLAYING(d) || (victim = d->character) == NULL
        || IS_NPC(victim) || IS_IMMORTAL(victim)
	|| victim->in_room == NULL || victim->pcdata->chobj != NULL
        || IS_CLASS(victim,CLASS_WEREWOLF))
	{
            continue;
        }
/*
      if ( !IS_SET( d->character->in_room->room_flags, ROOM_BLADE_BARRIER ) )
        continue;

      act( "The scattered blades on the ground fly up into the air ripping into you.", d->character, NULL, NULL, TO_CHAR );
      act( "The scattered blades on the ground fly up into the air ripping into $n.", d->character, NULL, NULL, TO_ROOM );

      act( "The blades drop to the ground inert.", d->character, NULL, NULL, TO_CHAR );
      act( "The blades drop to the ground inert.", d->character, NULL, NULL, TO_ROOM );

      dam = number_range( 7, 14 );
      dam = dam / 100;
      dam = d->character->hit * dam;
      if ( dam < 100 ) dam = 100;
      d->character->hit = d->character->hit - dam;
      if ( d->character->hit < -10 ) d->character->hit = -10;
      update_pos( victim );
*/
      }

    return;
}


/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
/*
void handle_counter(CHAR_DATA *ch)
{
char buf[MSL];
//if(!IS_IMMORTAL(ch))return;
  switch(ch->countdown[1])
  {
	case SHDW:

	case CPOR:
	  if(ch->countdown[0]>0){
	  sprintf(buf,"%d until Copyover.",ch->countdown[0]--);
	  do_info(ch,buf);break;}
	  else
	  do_copyover(ch,"");
	  break;
	case RSTR:
	  if(ch->countdown[0]>0){
	  sprintf(buf,"%d until Restore.",ch->countdown[0]--);
	  do_info(ch,buf);break;}
	  else do_restore(ch,"all");
	  break;
	default:
	  ch->countdown[0]=0;ch->countdown[1]=0;return;

	}

}
*/
void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int     pulse_ww;

    UpdateWar();

    if ( --pulse_ww       <= 0 )
    {
        pulse_ww        = PULSE_WW;
        ww_update       ( );
    }

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2);
//	imc_connect_to("subliminal_chaos");
	area_update	( );
    }
/*
    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }
*/
       if ( --pulse_mobile   <= (PULSE_MOBILE>>1) )
    {
	if ( pulse_mobile   <= 0 )
	{
	    pulse_mobile	= PULSE_MOBILE;
	    mobile_update	( );
	    player_update	( );
	    obj_update		( );
	}

else if ( pulse_mobile   == (PULSE_MOBILE>>1) )
	    obj_update		( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );

   if (war_info.wart == 0)
   {
            war_info.wart = 14400;
//            do_startwar(NULL,"1 50 30");
do_info(NULL,"{wA {RWar{w will be starting in {R30{w seconds. Type '#wWarstats{w' for more info, or '#wJoinwar{w' to join.{x");
//    WarAnnounce(buf);    
    mal_war.timer= 30;
    mal_war.type= 1;
    mal_war.reward= 50;
    mal_war.stage=1;
   }
   if (war_info.wart >= 1)
   {
         war_info.wart -= number_range(1,5);
   }
   if (dxp_info.dxpt == 0)
   {
            dxp_info.dxp = 0;  
            dxp_info.dxpt = -1;
            do_info(NULL,"{BDouble EXP {wis now {YOFF{w!{x");
   }
   if (dxp_info.dxpt >= 1)
   {
         dxp_info.dxpt -= 5;
   }
   if (dqp_info.dqpt == 0)
   {
            dqp_info.dqp = 0;
            dqp_info.dqpt = -1;
            do_info(NULL,"{CDouble QP{w is now {YOFF{w!{x");
   }  
   if (dqp_info.dqpt >= 1)
   {
         dqp_info.dqpt -= 5;
   }

   if (dtr_info.dtrt == 0)
   {
            dtr_info.dtr = 0;
            dtr_info.dtrt = -1;
            do_info(NULL,"Double Training is now OFF!");
   }  
   if (dtr_info.dtrt >= 1)
   {
         dtr_info.dtrt -= 5;
   }

   if (chao_info.chst == 0)
   {
            chao_info.chs = 0;
            chao_info.chst = -1;
            do_info(NULL,"CHAOS is now OFF!");
   }  
   if (chao_info.chst >= 1)
   {
         chao_info.chst -= 5;
   }
        aggr_update( );
	embrace_update();
    }

    if ( --pulse_point    <= 0 )
    {
	pulse_point     = number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2);
	weather_update	( );
	hint_update ( );
	moon_update	( );
	char_update	( );
	obj_update	( );
    }

    aggr_update( );
//    tail_chain( );
    return;
}

void update_vampire(CHAR_DATA *ch)
{
    if ( ch->position == POS_FIGHTING && ch->pcdata->stats[UNI_RAGE] > 0
         && ch->pcdata->stats[UNI_RAGE] < 25 && !IS_ITEMAFF(ch, ITEMA_RAGER))
             ch->pcdata->stats[UNI_RAGE] += 1;
    else if (ch->pcdata->stats[UNI_RAGE] > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
         {
             if (number_percent() <= ch->beast) beast_rage(ch);
              else
                ch->pcdata->stats[UNI_RAGE]--;
             if (ch->pcdata->stats[UNI_RAGE] < 1)
                        send_to_char("You feel the beast release it's hold on you.\n\r",ch);
         }

    if (ch->pcdata->stats[UNI_RAGE] < 1) return;

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
      regen_limb(ch);
      return;
}
void update_werewolf(CHAR_DATA *ch)
{
  if (!IS_ITEMAFF(ch, ITEMA_RAGER))
  {
    if (ch->position == POS_FIGHTING)
    {
      if ( ch->gifts[AHROUN] >= 5 && ch->pcdata->stats[UNI_RAGE] < 350)
          ch->pcdata->stats[UNI_RAGE] += number_range(2,3);
      if ( ch->gifts[SHADOWLORDS] >= 5 && ch->pcdata->stats[UNI_RAGE] <= 350)
          ch->pcdata->stats[UNI_RAGE] += number_range(2,4);
      if (ch->pcdata->stats[UNI_RAGE] < 300)
          ch->pcdata->stats[UNI_RAGE] += number_range(5,10);
      if (!IS_SET(ch->special, SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] >= 100)
                        do_werewolf(ch,"");
     }
     else if (ch->pcdata->stats[UNI_RAGE] > 0)
     {
         ch->pcdata->stats[UNI_RAGE] -= 1;
         if (ch->pcdata->stats[UNI_RAGE] < 100)
	     do_unwerewolf(ch,"");
    }
  }
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
     regen_limb(ch);
      return;
}
void update_demon(CHAR_DATA *ch)
{
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
      regen_limb(ch);
      return;
}
void update_mage(CHAR_DATA *ch)
{
   	    if ( (ch->position == POS_MEDITATING && 
		ch->spheres[MLIF] >= 3 && ch->spheres[MPRI] >= 3) || (ch->spheres[MLIF] == 5 && ch->spheres[MPRI] > 2) )
	    {

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
      regen_limb(ch);
            }
      return;
}
void update_highlander(CHAR_DATA *ch)
{
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
      regen_limb(ch);
      return;
}

void update_monk(CHAR_DATA *ch)
{
 if ( ch->position != POS_FIGHTING )
              {
                if ( ch->chi[CURRENT] > 0 )   
                {
                    if ( number_range( 1,4 ) == 2 )
                    {
                        ch->chi[CURRENT]--;
                        stc("You feel more relaxed.\n\r",ch);
                        act("$n looks more relaxed.",ch,NULL,NULL,TO_ROOM);
                    }
                }
              }

    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
      regen_limb(ch);
      return;
}

void update_ninja(CHAR_DATA *ch)
{
    if ( IS_AFFECTED(ch, AFF_HIDE) )
	ch->pcdata->powerss[NINJAKI] -= 1;
    if(ch->pcdata->powerss[NINJAKI] < 0)
    {
	ch->pcdata->powerss[NINJAKI] = 0;
        if ( IS_AFFECTED(ch, AFF_HIDE) )
        {
        REMOVE_BIT(ch->affected_by, AFF_HIDE);
        act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "You appear from the shadows.\n\r", ch );
        }
    }

    if ( ch->position == POS_FIGHTING && ch->pcdata->stats[UNI_RAGE] > 0 
	&& ch->pcdata->stats[UNI_RAGE] < 300 && !IS_ITEMAFF(ch, ITEMA_RAGER))
	    ch->pcdata->stats[UNI_RAGE] += 1;
	else if ( ch->position == POS_MEDITATING && ch->pcdata->stats[UNI_RAGE] > 0
	&& ch->pcdata->stats[UNI_RAGE] < 300 && !IS_ITEMAFF(ch, ITEMA_RAGER) )
	    ch->pcdata->stats[UNI_RAGE] -= 20;
	else if (ch->pcdata->stats[UNI_RAGE] > 0 && !IS_ITEMAFF(ch, ITEMA_RAGER))
	    ch->pcdata->stats[UNI_RAGE] -= 1;
	if (ch->pcdata->stats[UNI_RAGE] < 1) return;

        if (ch->pcdata->stats[UNI_RAGE] > 0 && (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move))
        werewolf_regen(ch);
	regen_limb(ch);
      
    return;
}
void update_drow(CHAR_DATA *ch)
{
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
      werewolf_regen(ch);
      regen_limb(ch);
      return;
}

void update_item_regen(CHAR_DATA *ch)
{
  if (ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)
  werewolf_regen(ch);
  regen_limb(ch);
  return;
}

    

void regen_limb(CHAR_DATA *ch)
{
  if (ch->loc_hp[6] > 0)
  {
    int sn = skill_lookup( "clot" );
    (*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
  }
  else if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] + ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
    reg_mend(ch);
  return;
}


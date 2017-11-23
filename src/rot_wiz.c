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



void do_pset( CHAR_DATA *ch, char *argument )
{
/* Re-worked by Kyrlin 02/12/00 */
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char arg4 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;

    sprintf(buf,"%s: Pset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    strcpy(arg4, argument);
    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
        divide_to_char(ch);
stc( "#wSyntax:#n pset <victim> <area> <field> <value>\n\r",  ch );
stc( "\n\r",                                                  ch );
stc( "#wArea being one of:#n\n\r",                            ch );
stc( "  quest quest+ quest- beast aura dps status\n\r",       ch );
stc( "  weapon, immune, spell, stance, special\n\r",	        ch );
stc( "  legend bounty fighttimer agg\n\r",                    ch );
stc( "\n\r",						              ch );
stc( "#wField being one of:#n\n\r",			              ch );
stc( "  Immune: slash stab smash animal misc charm\n\r",      ch ); 
stc( "  Immune: heat cold acid summon voodoo \n\r",	        ch );
stc( "  Immune: hurl backstab shielded kick disarm\n\r",      ch );
stc( "  Immune: steal sleep drain sunlight, all\n\r",         ch );
stc( "  Spell:  #Pauram#n, #rignem#n, #baquam#n, #gterram#n, #ymentem#n, all\n\r", ch );
stc( "  Stance: bull, crab, crane, mongoose, viper\n\r",      ch );
stc( "  Stance: dragon, mantis, monkey, swallow, tiger\n\r",  ch );
stc( "  Stance: bear, cobra, cougar, eagle, rabbit, all\n\r", ch );
stc( "  Weapon: slice stab slash whip claw blast   \n\r",     ch );
stc( "  Weapon: pound crush grep bite pierce suck, all\n\r",  ch );
stc( "  \n\r",						              ch );
stc( "#wSpecial being:#n\n\r",                                ch );
stc( "  ftalk, testchar\n\r",                                 ch );
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

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */
    if (!str_cmp(arg2,"class2"))
   {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's!\n\r", ch );
            return;
        }
         if (!str_cmp(arg3,"none"))
        { victim->class_2 = 0; stc("Your second class has been reset.\n\r",victim); stc("Ok\n\r",ch); return; }
        if (victim->class_2 != 0) { stc("You must clear their second class first.\n\r",ch); return; }

        if (!str_cmp(arg3,"demon"))
        { 
            victim->class_2 = CLASS_DEMON;
            victim->lord=str_dup("Tracker");
                send_to_char("They are now a Demon.\n\r",ch);
                send_to_char("You are now a Demon.\n\r", victim);
        }
        else if (!str_cmp(arg3,"mage"))
        {
            victim->class_2 = CLASS_MAGE;
                send_to_char("They are now a Mage.\n\r",ch);
                send_to_char("You are now a Mage.\n\r", victim);
        }
        else if (!str_cmp(arg3,"werewolf"))
        {
            victim->class_2 = CLASS_WEREWOLF;
                send_to_char("They are now a Werewolf.\n\r",ch);
                send_to_char("You are now a Werewolf.\n\r", victim);
        }
        else if (!str_cmp(arg3,"ninja"))
        {
            victim->class_2 = CLASS_NINJA;
        victim->pcdata->powerss[NPOWER_SORA] = 0;
        victim->pcdata->powerss[NPOWER_CHIKYU] = 0;
        victim->pcdata->powerss[NPOWER_NINGENNO] = 0;  
                send_to_char("They are now a Ninja.\n\r",ch);
                send_to_char("You are now a Ninja.\n\r", victim);
        }
        else if (!str_cmp(arg3,"monk"))
        {
            victim->class_2 = CLASS_MONK;
                send_to_char("They are now a Monk.\n\r",ch);
                send_to_char("You are now a Monk.\n\r", victim);
        }
        else if (!str_cmp(arg3,"vampire"))
        {
            victim->class_2 = CLASS_VAMPIRE;
                send_to_char("They are now a Vampire.\n\r",ch);
                send_to_char("You are now a Vampire.\n\r", victim);
        }
        else if (!str_cmp(arg3,"Drow"))
        {
            victim->class_2 = CLASS_DROW;
            victim->lord=str_dup("Tracker");
                send_to_char("They are now a Drow.\n\r",ch);
                send_to_char("You are now a Drow.\n\r", victim);
        }
        else if (!str_cmp(arg3,"kensai"))
        {
            victim->class_2 = CLASS_HIGHLANDER;
                send_to_char("They are now a Kensai.\n\r",ch);
                send_to_char("You are now a Kensai.\n\r", victim);
        }
        else { stc("Invalid class.\n\r",ch); return; }
       return;
    }

    if (!str_cmp(arg2,"class3"))
   {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's!\n\r", ch );
            return;
        }
         if (!str_cmp(arg3,"none"))
        { victim->class_3 = 0; stc("Your second class has been reset.\n\r",victim); stc("Ok\n\r",ch); return; }
        if (victim->class_3 != 0) { stc("You must clear their second class first.\n\r",ch); return; }
        
        if (!str_cmp(arg3,"demon"))
        {
            victim->class_3 = CLASS_DEMON;
            victim->lord=str_dup("Tracker");
                send_to_char("They are now a Demon.\n\r",ch);
                send_to_char("You are now a Demon.\n\r", victim);
        }
        else if (!str_cmp(arg3,"mage"))
        {
            victim->class_3 = CLASS_MAGE;
                send_to_char("They are now a Mage.\n\r",ch);
                send_to_char("You are now a Mage.\n\r", victim);
        }
        else if (!str_cmp(arg3,"werewolf"))
        {
            victim->class_3 = CLASS_WEREWOLF;
                send_to_char("They are now a Werewolf.\n\r",ch);
                send_to_char("You are now a Werewolf.\n\r", victim);
        }
        else if (!str_cmp(arg3,"ninja"))
        {
            victim->class_3 = CLASS_NINJA;
        victim->pcdata->powerss[NPOWER_SORA] = 0;
        victim->pcdata->powerss[NPOWER_CHIKYU] = 0;
        victim->pcdata->powerss[NPOWER_NINGENNO] = 0;
                send_to_char("They are now a Ninja.\n\r",ch);
                send_to_char("You are now a Ninja.\n\r", victim);
        }
        else if (!str_cmp(arg3,"monk"))
        {
            victim->class_3 = CLASS_MONK;
                send_to_char("They are now a Monk.\n\r",ch);
                send_to_char("You are now a Monk.\n\r", victim);
        }
        else if (!str_cmp(arg3,"vampire"))
        {
            victim->class_3 = CLASS_VAMPIRE;
                send_to_char("They are now a Vampire.\n\r",ch);
                send_to_char("You are now a Vampire.\n\r", victim);
        }
        else if (!str_cmp(arg3,"Drow"))
        {
            victim->class_3 = CLASS_DROW;
            victim->lord=str_dup("Tracker");
                send_to_char("They are now a Drow.\n\r",ch);
                send_to_char("You are now a Drow.\n\r", victim);
        }
        else if (!str_cmp(arg3,"kensai"))
        {
            victim->class_3 = CLASS_HIGHLANDER;
                send_to_char("They are now a Kensai.\n\r",ch);
                send_to_char("You are now a Kensai.\n\r", victim);
        }
        else if (!str_cmp(arg3,"dragon"))
        {
            victim->class_3 = CLASS_DRAGON;
                send_to_char("They are now a Dragon.\n\r",ch);
                send_to_char("You are now a Dragon.\n\r", victim);
        }
        else { stc("Invalid class.\n\r",ch); return; }
       return;
      }

    if ( !str_cmp( arg2, "special") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's!\n\r", ch );   
            return;
        }

        if (!str_cmp( arg3, "ftalk") )
        {   
            if ( IS_SET(victim->newbits, NEW_FTALK) )   
            {
                stc( "You remove them from the FTALK channel.\n\r",ch);
                stc("You are no longer on the FTALK channel!\n\r",victim);
                REMOVE_BIT(victim->newbits, NEW_FTALK);
            }
            else
            {
                stc("You add them to the FTALK channel.\n\r",ch);
                stc("You have been added to the FTALK channel.\n\rSyntax is: ftalk <message>\n\r",victim);
                SET_BIT(victim->newbits, NEW_FTALK);
            }
        return;
        }

        if (!str_cmp( arg3, "testchar") )
        {   
            if ( IS_SET(victim->newbits, TEST_CHAR) )   
            {
                stc("You remove their test char status.\n\r",ch);
                stc("You are no longer a test char!\n\r",victim);
                victim->race = 0;
                victim->pcdata->stats[UNI_GEN] = 3;
                set_title(victim, "the mortal");
                REMOVE_BIT(victim->newbits, TEST_CHAR);
                if ( IS_SET(victim->act, PLR_LOG) )
                    REMOVE_BIT(victim->act, PLR_LOG);
            }
            else
            {
                stc("You make them into a test char.\n\r",ch);
                stc("You have been set as a test char.\n\r",victim);
                victim->race = 100;
                victim->pcdata->stats[UNI_GEN] = 1;
                set_title(victim, "the TEST CHARACTER");
                SET_BIT(victim->newbits, TEST_CHAR);
                if ( !IS_SET(victim->act, PLR_LOG) )
                    SET_BIT(victim->act, PLR_LOG);
            }
        return;
        }
    }
	
    if ( !str_cmp( arg2, "spell") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's.\n\r", ch );
            return;
        }

        if (arg3 == '\0')
        {
            stc( "pset <victim> spell <type> <value>.\n\r", ch );
            return;         
        }

        argument = one_argument( argument, arg4 );
        value = is_number( arg4 ) ? atoi( arg4 ) : -1;

        if ( value < 1 || value > 240 )
        {
            stc( "Spell range is 1 to 240.\n\r", ch );
            return;
        }

        if (IS_JUDGE(ch)) 
        {
            if (!str_cmp( arg3, "auram") )
            {   
                victim->spl[PURPLE_MAGIC] = value;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "ignem" ) )
            {   
                victim->spl[RED_MAGIC] = value;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "aquam" ) )
            {   
                victim->spl[BLUE_MAGIC] = value;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "terram" ) )
            {   
                victim->spl[GREEN_MAGIC] = value;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "mentem" ) )
            {   
                victim->spl[YELLOW_MAGIC] = value;
                stc("Ok.\n\r",ch);
                return;
            }
            if (!str_cmp(arg3, "all"))
            {
                victim->spl[PURPLE_MAGIC] = value;
                victim->spl[RED_MAGIC]    = value;
                victim->spl[BLUE_MAGIC]   = value;
                victim->spl[GREEN_MAGIC]  = value;
                victim->spl[YELLOW_MAGIC] = value;
                stc("Ok.\n\r",ch);
                return;
            }
        else
            send_to_char("No such spell type exists.\n\r",ch);
            return;
        }
    }

    if ( !str_cmp( arg2, "stance") )
    {
        if ( IS_NPC(victim) )
        {
            stc( "Not on NPC's.\n\r", ch );
            return;
        }

        if (arg3 == '\0')
        {
            stc( "pset <victim> stance <stance> <value>.\n\r", ch );
            return;         
        }

        argument = one_argument( argument, arg4 );
        value = is_number( arg4 ) ? atoi( arg4 ) : -1;

        if ( value < 0 || value > 200 )
        {
            stc( "Stance range is 0 to 200.\n\r", ch );
            return;
        }

        if (IS_JUDGE(ch)) 
        {
            if ( !str_cmp( arg3, "bull" ) )
            {
                victim->stance[STANCE_BULL] = value;
		    stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "crab" ) )
            {
                victim->stance[STANCE_CRAB] = value;
		    stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "crane" ) )
            {
                victim->stance[STANCE_CRANE] = value;
		    stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "mongoose" ) )
            {
                victim->stance[STANCE_MONGOOSE] = value;
		    stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "viper" ) )
            {
                victim->stance[STANCE_VIPER] = value;
		    stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "dragon" ) )
            {
                victim->stance[STANCE_DRAGON] = value;
                victim->stance[STANCE_CRAB]   = 200;
                victim->stance[STANCE_BULL]   = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "mantis" ) )
            {
                victim->stance[STANCE_MANTIS] = value;
                victim->stance[STANCE_CRANE]  = 200;
                victim->stance[STANCE_VIPER]  = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "monkey" ) )
            {
                victim->stance[STANCE_MONKEY] = value;
                victim->stance[STANCE_MONGOOSE]  = 200;
                victim->stance[STANCE_CRANE]  = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "swallow" ) )
            {
                victim->stance[STANCE_SWALLOW] = value;
                victim->stance[STANCE_CRAB]  = 200;
                victim->stance[STANCE_MONGOOSE]  = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "tiger" ) )
            {
                victim->stance[STANCE_TIGER] = value;
                victim->stance[STANCE_BULL]  = 200;
                victim->stance[STANCE_VIPER] = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "bear" ) )
            {
                victim->stance[STANCE_BEAR] = value;
                victim->stance[STANCE_SWALLOW]  = 200;
                    victim->stance[STANCE_CRAB]  = 200;
                    victim->stance[STANCE_MONGOOSE]  = 200;
                victim->stance[STANCE_DRAGON]  = 200;
                    victim->stance[STANCE_BULL]   = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "cobra" ) )
            {
                victim->stance[STANCE_COBRA] = value;
                victim->stance[STANCE_TIGER]  = 200;
                    victim->stance[STANCE_BULL]  = 200;
                    victim->stance[STANCE_VIPER] = 200;
                victim->stance[STANCE_DRAGON]  = 200;
                    victim->stance[STANCE_CRAB]   = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "cougar" ) )
            {
                victim->stance[STANCE_COUGAR] = value;
                victim->stance[STANCE_MANTIS]  = 200;
                    victim->stance[STANCE_CRANE]  = 200;
                    victim->stance[STANCE_VIPER]  = 200;
                victim->stance[STANCE_DRAGON]  = 200;
                    victim->stance[STANCE_CRAB]   = 200;
                    victim->stance[STANCE_BULL]   = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "eagle" ) )
            {
                victim->stance[STANCE_EAGLE] = value;
                victim->stance[STANCE_SWALLOW]  = 200;
                    victim->stance[STANCE_CRAB]  = 200;
                    victim->stance[STANCE_MONGOOSE]  = 200;
                victim->stance[STANCE_TIGER]  = 200;
                    victim->stance[STANCE_BULL]  = 200;
                    victim->stance[STANCE_VIPER] = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if ( !str_cmp( arg3, "rabbit" ) )
            {
                victim->stance[STANCE_RABBIT] = value;
                victim->stance[STANCE_SWALLOW]  = 200;
                    victim->stance[STANCE_CRAB]  = 200;
                    victim->stance[STANCE_MONGOOSE]  = 200;
                victim->stance[STANCE_MONKEY]  = 200;
                    victim->stance[STANCE_CRANE]  = 200;
                stc("Ok.\n\r",ch);
                return;
            }
            if (!str_cmp(arg3, "all"))
            {
		    victim->stance[STANCE_BULL]    = value;
		    victim->stance[STANCE_CRAB]    = value;
		    victim->stance[STANCE_CRANE]   = value;
		    victim->stance[STANCE_MONGOOSE]= value;
		    victim->stance[STANCE_VIPER]   = value;
		    victim->stance[STANCE_DRAGON]  = value;
		    victim->stance[STANCE_MANTIS]  = value;
		    victim->stance[STANCE_MONKEY]  = value;
		    victim->stance[STANCE_SWALLOW] = value;
		    victim->stance[STANCE_TIGER]   = value;
		    victim->stance[STANCE_BEAR]    = value;
		    victim->stance[STANCE_COBRA]   = value;
		    victim->stance[STANCE_COUGAR]  = value;
		    victim->stance[STANCE_EAGLE]   = value;
		    victim->stance[STANCE_RABBIT]  = value;
                stc("Ok.\n\r",ch);
                return;
            }
        else
            send_to_char("No such stance exists.\n\r",ch);
            return;
        }
    }

    if ( !str_cmp( arg2, "beast" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Beast range is 0 to 100.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->beast = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }    

    if ( !str_cmp( arg2, "bounty") )
    {
        if ( IS_NPC(victim) )
        {
            send_to_char("Not on NPC's!\n\r", ch );
            return;
        }
        if ( value < 0 || value > 2000 )
        {
            send_to_char("Bounty range is 0 to 2000.\n\r", ch);
            return;
        }
	if( value <= 0)
	{
	sprintf(buf,"%s no longer has a bounty.\n\r", victim->name);
	do_info(ch, buf);
	victim->pcdata->bounty = 0;
	return;
	}
	else
        victim->pcdata->bounty = value;
	sprintf(buf, "%s is now worth %d questpoints.", victim->name,victim->pcdata->bounty );
	do_info(ch,buf);
        send_to_char("OK.\n\r", ch);
        return;
    }

  if ( !str_cmp( arg2, "agg" ) )
    {
        if( IS_NPC(victim) )
        {
            send_to_char("Not on npcs.\n\r",ch);
            return;
        }
     if ( value < 0 || value > 110 )
        {
            send_to_char( "Agg damage from 0 to 110 pls.\n\r", ch );
            return;
        }

        victim->agg = value;
        return;
    }

 if ( !str_cmp( arg2, "fighttimer" ) )
    {
	if( IS_NPC(victim) )
	{
	    send_to_char("Not on npcs.\n\r",ch);
	    return;
	}
     if ( value < 1 || value > 30 )
        {
            send_to_char( "Fight timers from 1 to 30 pls.\n\r", ch );
            return;
        }
	
	ch->fight_timer = value;
	return;
    }

    if ( !str_cmp( arg2, "aura" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}
	 
	if ( value < 1 || value > 200000 )
	{
	    send_to_char( "Aura points range is 1 to 200000.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		ch->pcdata->stats[PALADIN_CURRENT] = value;
		ch->pcdata->stats[PALADIN_TOTAL] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "dps" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value > 20000000 )
	{
	    send_to_char( "Demon points range is 1 to 20000000.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->stats[DEMON_CURRENT] = value;
		victim->pcdata->stats[DEMON_TOTAL] = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "legend" ) )
    {
        if ( IS_NPC(victim) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }
        
        if ( value < 0 || value > 9 )
        {
            send_to_char( "Legend range is 0 to 9.\n\r", ch );
            return;
        }       
        
        if (IS_JUDGE(ch)) {
                victim->lstatus = value;
                send_to_char("Ok.\n\r",ch);}
        else 
                send_to_char( "Sorry, no can do...\n\r", ch );
        return;
    }


    if ( !str_cmp( arg2, "status" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Status range is 0 to 100.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->race = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "quest" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value > 150000 )
	{
	    send_to_char( "Quest range is 1 to 150000.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->quest = value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "quest+" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 1 || value + victim->pcdata->quest > 15000 )
	{
	    send_to_char( "Quest range is 1 to 15000.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->quest += value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "quest-" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( victim->pcdata->quest - value < 1 || value > 15000 )
	{
	    send_to_char( "Quest range is 1 to 15000.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) {
		victim->pcdata->quest -= value;
		send_to_char("Ok.\n\r",ch);}
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "immune" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

        if (arg3 == '\0')
        {
	    send_to_char( "pset <victim> immune <immunity>.\n\r", ch );
	    return;         
        }

	if (IS_JUDGE(ch)) 
        {
                if (!str_cmp(arg3, "voodoo"))
                {
                  if (IS_SET(victim->immune, IMM_VOODOO))
                  {
		    REMOVE_BIT(victim->immune, IMM_VOODOO);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_VOODOO);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "slash"))
                {
                  if (IS_SET(victim->immune, IMM_SLASH))
                  {
		    REMOVE_BIT(victim->immune, IMM_SLASH);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SLASH);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "stab"))
                {
                  if (IS_SET(victim->immune, IMM_STAB))
                  {
		    REMOVE_BIT(victim->immune, IMM_STAB);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_STAB);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "smash"))
                {
                  if (IS_SET(victim->immune, IMM_SMASH))
                  {
		    REMOVE_BIT(victim->immune, IMM_SMASH);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SMASH);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "anmial"))
                {
                  if (IS_SET(victim->immune, IMM_ANIMAL))
                  {
		    REMOVE_BIT(victim->immune, IMM_ANIMAL);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_ANIMAL);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "misc"))
                {
                  if (IS_SET(victim->immune, IMM_MISC))
                  {
		    REMOVE_BIT(victim->immune, IMM_MISC);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_MISC);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "charm"))
                {
                  if (IS_SET(victim->immune, IMM_CHARM))
                  {
		    REMOVE_BIT(victim->immune, IMM_CHARM);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_CHARM);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }
                if (!str_cmp(arg3, "heat"))
                {
                  if (IS_SET(victim->immune, IMM_HEAT))
                  {
		    REMOVE_BIT(victim->immune, IMM_HEAT);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_HEAT);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }
                if (!str_cmp(arg3, "cold"))
                {
                  if (IS_SET(victim->immune, IMM_COLD))
                  {
		    REMOVE_BIT(victim->immune, IMM_COLD);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_COLD);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "lightning"))
                {
                  if (IS_SET(victim->immune, IMM_LIGHTNING))
                  {
		    REMOVE_BIT(victim->immune, IMM_LIGHTNING);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_LIGHTNING);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "acid"))
                {
                  if (IS_SET(victim->immune, IMM_ACID))
                  {
		    REMOVE_BIT(victim->immune, IMM_ACID);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_ACID);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "shield"))
                {
                  if (IS_SET(victim->immune, IMM_SHIELDED))
                  {
		    REMOVE_BIT(victim->immune, IMM_SHIELDED);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SHIELDED);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "hurl"))
                {
                  if (IS_SET(victim->immune, IMM_HURL))
                  {
		    REMOVE_BIT(victim->immune, IMM_HURL);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_HURL);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "backstab"))
                {
                  if (IS_SET(victim->immune, IMM_BACKSTAB))
                  {
		    REMOVE_BIT(victim->immune, IMM_BACKSTAB);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_BACKSTAB);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }
		
                if (!str_cmp(arg3, "kick"))
                {
                  if (IS_SET(victim->immune, IMM_KICK))
                  {
		    REMOVE_BIT(victim->immune, IMM_KICK);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_KICK);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "disarm"))
                {
                  if (IS_SET(victim->immune, IMM_DISARM))
                  {
		    REMOVE_BIT(victim->immune, IMM_DISARM);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_DISARM);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "steal"))
                {
                  if (IS_SET(victim->immune, IMM_STEAL))
                  {
		    REMOVE_BIT(victim->immune, IMM_STEAL);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_STEAL);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "sleep"))
                {
                  if (IS_SET(victim->immune, IMM_SLEEP))
                  {
		    REMOVE_BIT(victim->immune, IMM_SLEEP);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SLEEP);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "sunlight"))
                {
                  if (IS_SET(victim->immune, IMM_SUNLIGHT))
                  {
		    REMOVE_BIT(victim->immune, IMM_SUNLIGHT);
		    send_to_char("Ok Immunity Removed.\n\r",ch);
                    return;                    
                  }
                  else
                  {
		    SET_BIT(victim->immune, IMM_SUNLIGHT);
		    send_to_char("Ok Immunity Added.\n\r",ch);
                    return;
                  }
                }

                if (!str_cmp(arg3, "all"))
                {
		    SET_BIT(victim->immune, IMM_DRAIN);
		    SET_BIT(victim->immune, IMM_VOODOO);
		    SET_BIT(victim->immune, IMM_SLASH);
		    SET_BIT(victim->immune, IMM_STAB);
		    SET_BIT(victim->immune, IMM_SMASH);
		    SET_BIT(victim->immune, IMM_ANIMAL);
		    SET_BIT(victim->immune, IMM_MISC);
		    SET_BIT(victim->immune, IMM_CHARM);
		    SET_BIT(victim->immune, IMM_HEAT);
		    SET_BIT(victim->immune, IMM_COLD);
		    SET_BIT(victim->immune, IMM_LIGHTNING);
		    SET_BIT(victim->immune, IMM_ACID);
		    SET_BIT(victim->immune, IMM_HURL);
		    SET_BIT(victim->immune, IMM_BACKSTAB);
		    SET_BIT(victim->immune, IMM_KICK);
		    SET_BIT(victim->immune, IMM_DISARM);
		    SET_BIT(victim->immune, IMM_STEAL);
		    SET_BIT(victim->immune, IMM_SLEEP);
		    send_to_char("Ok, All Standard Immunities Added.\n\r",ch);
                    return;
                }

                send_to_char("No such immunity exists.\n\r",ch);
                return;
        }
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "weapon" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

        argument = one_argument( argument, arg4 );
        /*
         * Snarf the value (which need not be numeric).
         */
        value = is_number( arg4 ) ? atoi( arg4 ) : -1;

	if ( value < 0 || value > 1000 )
	{
	    send_to_char( "Weapon skill range is 0 to 1000.\n\r", ch );
	    return;
	}

	if (IS_JUDGE(ch)) 
        {
                if (!str_cmp(arg3, "unarmed"))
                {
		  victim->wpn[0] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "slice"))
                {
		  victim->wpn[1] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "stab"))
                {
		  victim->wpn[2] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "slash"))
                {
		  victim->wpn[3] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "whip"))
                {
		  victim->wpn[4] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "claw"))
                {
		  victim->wpn[5] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "blast"))
                {
		  victim->wpn[6] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "pound"))
                {
		  victim->wpn[7] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "crush"))
                {
		  victim->wpn[8] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "grep"))
                {
		  victim->wpn[9] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "bite"))
                {
		  victim->wpn[10] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "pierce"))
                {
		  victim->wpn[11] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "suck"))
                {
		  victim->wpn[12] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }

                if (!str_cmp(arg3, "all"))
                {
		  victim->wpn[0] = value;
		  victim->wpn[1] = value;
		  victim->wpn[2] = value;
		  victim->wpn[3] = value;
		  victim->wpn[4] = value;
		  victim->wpn[5] = value;
		  victim->wpn[6] = value;
		  victim->wpn[7] = value;
		  victim->wpn[8] = value;
		  victim->wpn[8] = value;
		  victim->wpn[9] = value;
		  victim->wpn[10] = value;
                  victim->wpn[11] = value;
		  victim->wpn[12] = value;
		  send_to_char("Ok.\n\r",ch);
                  return;
                }
                send_to_char("No such weapon skill exists.\n\r",ch);
                return;
        }
	else
		send_to_char( "Sorry, no can do...\n\r", ch );
	return;
    }

    /*
     * Generate usage message.
     */
    do_pset( ch, "" );
    return;
}


void do_paradox( CHAR_DATA *ch, char *argument)  {

	char arg[MAX_INPUT_LENGTH];

	CHAR_DATA *victim;

        DESCRIPTOR_DATA *d;        

        argument = one_argument(argument, arg);        



	if (arg[0] == '\0') {

		send_to_char("Paradox whom?\n\r", ch );

		return;}



    if ( get_trust(ch) >=  MAX_LEVEL - 2 && !str_cmp(arg,"all"))

    {

        for (d = descriptor_list; d != NULL; d = d->next)

        {

	    victim = d->character;



	    if (victim == NULL || IS_NPC(victim))

		continue;

                

          

            { paradox(victim); }

        }

    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )

    {

        send_to_char( "They aren't here.\n\r", ch );

        return;

    }

	paradox(victim);

	send_to_char("Done!\n\r", ch );

}





void paradox( CHAR_DATA *ch )

{

    char            buf[MAX_STRING_LENGTH]; 



    send_to_char("The sins of your past strike back!\n\r",  ch);

    send_to_char("The paradox has come for your soul!\n\r",  ch);

    sprintf( buf, "#r%s#e is struck by a #pP#Pa#rr#RA#pd#PO#rx#n.\n\r", ch->name);

    do_info(ch, buf);

    ch->paradox[1] = 0;

    ch->paradox[2] = 0;



/*  Chris please sort this out.  KaVir. */



    ch->hit     =  -10;

    update_pos( ch );

    do_escape(ch, "");

    SET_BIT(ch->extra, TIED_UP);

    SET_BIT(ch->extra, GAGGED);

    SET_BIT(ch->extra, BLINDFOLDED);



/*  Commented out, as it's a very pointless piece of code.



    if (ch->paradox[0] <= 15)

    {

    	ch->hit     =  -10;

    	update_pos( ch );

    	do_escape(ch, "");

    	SET_BIT(ch->extra, TIED_UP);

    	SET_BIT(ch->extra, GAGGED);

    	SET_BIT(ch->extra, BLINDFOLDED);

    }

    else if (ch->paradox[0] <= 55)

    {

    	ch->hit     =  -10;

    	update_pos( ch );

    	do_escape(ch, "");

    	SET_BIT(ch->extra, TIED_UP);

    	SET_BIT(ch->extra, GAGGED);

    	SET_BIT(ch->extra, BLINDFOLDED);

    }

    else if (ch->paradox[0] <= 99)

    {

    	ch->hit     =  -10;

    	update_pos( ch );

    	do_escape(ch, "");

    	SET_BIT(ch->extra, TIED_UP);

    	SET_BIT(ch->extra, GAGGED);

    	SET_BIT(ch->extra, BLINDFOLDED);

    }

    else if (ch->paradox[0] >= 100)

    {

    	ch->hit     =  -10;

    	update_pos( ch );

    	do_escape(ch, "");

    	SET_BIT(ch->extra, TIED_UP);

    	SET_BIT(ch->extra, GAGGED);

    	SET_BIT(ch->extra, BLINDFOLDED);

    }

*/

    return;

}


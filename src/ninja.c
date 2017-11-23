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

/* Local Functions */

int check_skill( int princ, int princ_level, CHAR_DATA *ch); 


/* New Ninja Stuff */

void do_newprinciples( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!str_cmp(ch->name, "Tracker"));
    else
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
 

    if (!IS_CLASS(ch, CLASS_NINJA))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

/*    DELAY(ch, 8); */
 
    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        sprintf(buf,"Principles: Ku (%d), Chi (%d), Sui (%d), Ka (%d) and Fu (%d).\n\r",
        ch->pcdata->principles[NINJA_KU], ch->pcdata->principles[NINJA_CHI],
        ch->pcdata->principles[NINJA_SUI], ch->pcdata->principles[NINJA_KA],
        ch->pcdata->principles[NINJA_FU]);
        send_to_char(buf,ch);
        return;
     }

    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"ku"))
        {
	    send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=[**] NINJA PRINCIPLES [**]=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
            send_to_char("Ku - The void.  The source.  The wellspring.  The self.\n\r\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] < 1)
                send_to_char("You have no knowledge of the principle of Ku.\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] > 0)
                send_to_char("Darting retreat      |   Retreat       |\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] > 1)
                send_to_char("Still the body       |   Still         |\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] > 2)
                send_to_char("Sidestepping defence |   Sidestep      |\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] > 3)
                send_to_char("Unbalance foe        |   Unbalance     |\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] > 4)
                send_to_char("The shadow warrior   |   Shadowwarrior |\n\r", ch);
            if (ch->pcdata->principles[NINJA_KU] > 5)
                send_to_char("Illuminate the void  |   Illuminate    |\n\r",ch);
            if (ch->pcdata->principles[NINJA_KU] > 6)
                send_to_char("Hornet sting         |   Sting         |\n\r",ch);
	    send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
                return; 
        }

        if (!str_cmp(arg1,"chi"))
        {
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=[**] NINJA PRINCIPLES [**]=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
            send_to_char("Chi - The earth.\n\r\n\r",ch);
            if (ch->pcdata->principles[NINJA_CHI] < 1)
                send_to_char("You have no knowledge of the principle of Chi.\n\r",ch);
            if (ch->pcdata->principles[NINJA_CHI] > 0)
                send_to_char("                  |             |\n\r",ch);                                   
            if (ch->pcdata->principles[NINJA_CHI] > 1)   
                send_to_char("                  |             |\n\r",ch);                               
            if (ch->pcdata->principles[NINJA_CHI] > 2)   
                send_to_char("                  |             |\n\r",ch);                             
            if (ch->pcdata->principles[NINJA_CHI] > 3)   
                send_to_char("                  |             |\n\r",ch);                    
            if (ch->pcdata->principles[NINJA_CHI] > 4)       
                send_to_char("                  |             |\n\r", ch);    
            if (ch->pcdata->principles[NINJA_CHI] > 5)       
                send_to_char("                  |             |\n\r",ch);                                             
            if (ch->pcdata->principles[NINJA_CHI] > 6)   
                send_to_char("                  |             |\n\r",ch);                                           
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
                return;
        }

        if (!str_cmp(arg1,"sui"))
        {
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=[**] NINJA PRINCIPLES [**]=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
            send_to_char("Sui - The water.\n\r\n\r",ch);
            if (ch->pcdata->principles[NINJA_SUI] < 1)
                send_to_char("You have no knowledge of the principle of Sui.\n\r",ch);
            if (ch->pcdata->principles[NINJA_SUI] > 0)  
                send_to_char("                  |             |\n\r",ch);                               
            if (ch->pcdata->principles[NINJA_SUI] > 1)  
                send_to_char("                  |             |\n\r",ch);                             
            if (ch->pcdata->principles[NINJA_SUI] > 2)  
                send_to_char("                  |             |\n\r",ch);                    
            if (ch->pcdata->principles[NINJA_SUI] > 3)      
                send_to_char("                  |             |\n\r",ch);     
            if (ch->pcdata->principles[NINJA_SUI] > 4)      
                send_to_char("                  |             |\n\r", ch);                                            
            if (ch->pcdata->principles[NINJA_SUI] > 5)
                send_to_char("                  |             |\n\r",ch);
            if (ch->pcdata->principles[NINJA_SUI] > 6)
                send_to_char("                  |             |\n\r",ch);
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
                return;
        }

        if (!str_cmp(arg1,"ka"))
        {
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=[**] NINJA PRINCIPLES [**]=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
            send_to_char("Ka - The fire.\n\r\n\r",ch);
            if (ch->pcdata->principles[NINJA_KA] < 1)  
                send_to_char("You have no knowledge of the principle of Ka.\n\r",ch);
            if (ch->pcdata->principles[NINJA_KA] > 0)  
                send_to_char("                  |             |\n\r",ch);                             
            if (ch->pcdata->principles[NINJA_KA] > 1)  
                send_to_char("                  |             |\n\r",ch);                    
            if (ch->pcdata->principles[NINJA_KA] > 2)      
                send_to_char("                  |             |\n\r",ch);     
            if (ch->pcdata->principles[NINJA_KA] > 3)      
                send_to_char("                  |             |\n\r",ch);                                             
            if (ch->pcdata->principles[NINJA_KA] > 4)
                send_to_char("                  |             |\n\r", ch);
            if (ch->pcdata->principles[NINJA_KA] > 5)
                send_to_char("                  |             |\n\r",ch);
            if (ch->pcdata->principles[NINJA_KA] > 6)
                send_to_char("                  |             |\n\r",ch);
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
                return;
        }

        if (!str_cmp(arg1,"fu"))
        {
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=[**] NINJA PRINCIPLES [**]=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
            send_to_char("Fu - The wind.\n\r\n\r",ch);
            if (ch->pcdata->principles[NINJA_FU] < 1)  
                send_to_char("You have no knowledge of the principle of Fu.\n\r",ch);
            if (ch->pcdata->principles[NINJA_FU] > 0)  
                send_to_char("                  |             |\n\r",ch);                             
            if (ch->pcdata->principles[NINJA_FU] > 1)  
                send_to_char("                  |             |\n\r",ch);                    
            if (ch->pcdata->principles[NINJA_FU] > 2)      
                send_to_char("                  |             |\n\r",ch);     
            if (ch->pcdata->principles[NINJA_FU] > 3)      
                send_to_char("                  |             |\n\r",ch);                                             
            if (ch->pcdata->principles[NINJA_FU] > 4)
                send_to_char("                  |             |\n\r", ch);
            if (ch->pcdata->principles[NINJA_FU] > 5)
                send_to_char("                  |             |\n\r",ch);
            if (ch->pcdata->principles[NINJA_FU] > 6)
                send_to_char("                  |             |\n\r",ch);
            send_to_char("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\r",ch);
                return;
        }

        sprintf(buf,"Principles: Ku (%d), Chi (%d), Sui (%d), Ka (%d) and Fu (%d).\n\r",
        ch->pcdata->principles[NINJA_KU], ch->pcdata->principles[NINJA_CHI],
        ch->pcdata->principles[NINJA_SUI], ch->pcdata->principles[NINJA_KA],
        ch->pcdata->principles[NINJA_FU]);
        send_to_char(buf,ch);
        return;
        }

    if (!str_cmp(arg2,"improve"))
    { 
        int improve;
        int cost;
        int max;
            
             if (!str_cmp(arg1,"ku"   )) {improve = NINJA_KU;   max=10;}
        else if (!str_cmp(arg1,"chi"  )) {improve = NINJA_CHI;  max=10;}
        else if (!str_cmp(arg1,"sui"  )) {improve = NINJA_SUI;  max=10;}
	else if (!str_cmp(arg1,"ka"   )) {improve = NINJA_KA;   max=10;}          
	else if (!str_cmp(arg1,"fu"   )) {improve = NINJA_FU;   max=10;}          
        else
        {
            send_to_char("Principles: Ku, Chi, Sui, Ka and Fu.\n\r",ch);
            return;
        }

        cost = (ch->pcdata->principles[improve]+1) * 10;
        arg1[0] = UPPER(arg1[0]);

        if ( ch->pcdata->principles[improve] >= max )
        {
            sprintf(buf,"You have already gained all the powers of the %s principle.\n\r", arg1);
            send_to_char(buf,ch);
            return;
        }

        if ( cost > ch->practice )
        { 
            sprintf(buf,"It costs you %d primal to improve your %s principle.\n\r", cost, arg1);
            send_to_char(buf,ch);
            return;
        }

        ch->pcdata->principles[improve] += 1;
        ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s principle.\n\r", arg1);
        send_to_char(buf,ch);
    }

    else send_to_char("To improve a principle, type: Principle <principle type> improve.\n\r",ch);
        return;

}

/* Ninja Knifewind thingie - Pissed Simian 30/09/2001 */
/*
void do_knifewind( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    int dam;
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
    char buf  [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg1 );
    dam = ch->pcdata->principles[NINJA_FU];


    if( IS_NPC( ch ))
       return;

    if (!str_cmp(ch->name, "Tracker"));
    else
    {   
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if( !IS_NINJA( ch ))
    {
        send_to_char( "Huh?\n\r", ch );
        return;
        }

    if( ch->pcdata->principles[NINJA_FU] < 1 )
    {
        send_to_char( "You have not learned the Fu principle to the first level./n/r", ch );
        return;
        }

    if( arg1[0] == '\0' )
    {
        send_to_char( "Use the knifewind on who?\n\r", ch ); 
        return;
        }
 
    if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
        }
    
    if ( ch == victim )
    {
        send_to_char( "Not on yourself!\n\r", ch );
        return;
        }

    WAIT_STATE( ch, 8 );

    if( IS_CLASS( victim, CLASS_NINJA ))
    {
        dam = ( dam / 2 );
        if( dam < 1 )
            dam = 1;
        }

    if( number_range(1,10) > 9)
    {
        if( IS_CLASS(victim, CLASS_HIGHLANDER ))
        {
            sprintf(buf,"You slice your hand through the air aiming towards %s.",victim->name);
            send_to_char(buf,ch);
            sprintf(buf,"%s lunges at you with his hand, stopping several inches in front of your chest.",ch->name);
            send_to_char(buf,victim);
            act("#PYou shape the air itself into a knife and send it lashing out in front of you.#n",ch, NULL, victim, TO_CHAR);

            sprintf(buf,"You feel an invisible force cut into you with ");
            ADD_COLOUR(victim,buf,L_RED);
            act2(buf,ch,NULL,victim,TO_VICT);          

            damage = ((damage*10)+(number_range(50,100)/(ch->pcdata->principles[NINJA_CHI]));

            if( ch->pcdata->principles[NINJA_KA] > 7)
	    {
                dam = dam + ((ch->pcdata->principles[NINJA_KA]*5) + (number_range(25,50)/(ch->pcdata->principles[NINJA_SUI]/2)));
	    }
            if(dam < 1)
            dam = 1;
            damage(ch,victim,dam,1);

            if(damage < 50)
            {
                sprintf(buf,"laughable");
                ADD_COLOUR(victim,buf,D_GREEN);
                act2(buf,ch,NULL,victim,TO_VICT);
                }
            else if(damage < 100)
            {
                sprintf("noticable #rpower./n/r",victim);
            else if(damage < 250)
                send_to_char("#Rpainful #rpower./n/r",victim);
            else if(damage < 500)
                send_to_char("#Bagonising #rpower./n/r",victim);
            else
                send_to_char("#Csoul-rending #rpower./n/r"victim);
            send_to_char("/rThe blade of air hits with /r",ch);
            if(damage(<50)
                send_to_char("#Gweak/r force./n/r",ch);
            else if(damage < 100)
                send_to_char("#Yaverage/r force./n/r",ch);
            else if(damage < 250)
                send_to_char("#Rsatisfying/r force./n/r",ch);
            else if(damage < 500)
                send_to_char("#Bripping/r force./n/r".ch);
            else
                send_to_char("#Cflesh-stripping/r force./n/r",ch);
            return;
            }
        sprintf(buf,"/n/rYou chop your hand downwards through the air and towards %s./n/r",victim->name);
        send_to_char(buf,ch);
        sprintf(buf,"/n/r%s lashes out towards you with his hand but stops a few inches in front of 
you./n/r,ch->name);
        send_to_char(buf,victim);
        send_to_char("/rYou feel a stinging pain as if you had been lashed by a #cfreezing/r wind./n/r",victim);
        send_to_char("/rYou shape the air into a sharp blade and launch it out in front of you./n/r",ch);
        damage = ((damage*8)+(number_range(40,80)/(ch->pcdata(principles[NINJA_CHI]));
        if(damage < 1)
            damage = 1;
        ;
        damage(ch,victim,damage,1);
        }

}
*/
// Ninja "Darting Retreat" - Simian - 10/6/2001

void do_retreat( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH]; 
    int attempt;
    int chance;


    if (!str_cmp(ch->name, "Tracker"));
    else
    {   
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if (!IS_CLASS(ch, CLASS_NINJA))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->principles[NINJA_KU] < 1 )
    {
        send_to_char("You have not learned the Ku principle to 1.\n\r", ch);
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ch->position == POS_FIGHTING )
	    ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if( ch->move <= 0 )
    {
        send_to_char("You are too exhausted to move.\n\r",ch);
        return;
    }

    chance = number_percent() - ch->pcdata->principles[NINJA_KU];

    if(IS_AFFECTED(ch,AFF_WEBBED))
    {
	if( chance > 20 )
        {
            send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	    return;
        }
        else
        {
            send_to_char("You slip out of the webbing holding you.\n\r",ch);
            sprintf(buf,"%s slips out of the webbing holding them.\n\r",ch->name);
            send_to_char(buf,victim);
        }
    }
    
    if(IS_CLASS(victim,CLASS_WEREWOLF) && IS_SET(victim->garou1,GAROU_CLENCHED) )
    {
        if( chance > 20 )
        {
            sprintf(buf,"%s's clenched teeth prevents you from fleeing.\n\r",victim->name);
            send_to_char(buf,ch);
            return;
        }	
        else
        {
            sprintf(buf,"You avoid %s as they try to clench you in their teeth.\n\r",victim->name);
            send_to_char(buf, ch);
            sprintf(buf,"%s slips from between your clenched teeth.\n\r",ch->name);
            send_to_char(buf,victim);
        }
    }

    if(IS_CLASS(victim,CLASS_VAMPIRE) && IS_SET(victim->newbits,NEW_COIL) )
    {
        if( chance > 20 )
        {
            sprintf(buf,"Not with %s coiled around you!\n\r",victim->name);
            send_to_char(buf,ch);
            act("$n can't escape with $N coiled around them.",ch,NULL,victim,TO_ROOM);
            return;
        }
        else
        {
            sprintf(buf,"You break free from %s's coils which were holding you.\n\r",victim->name);
            send_to_char(buf, ch);
            sprintf(buf,"%s breaks free from your coils.\n\r",ch->name);
            send_to_char(buf,victim);
        }
    }

    if(IS_AFFECTED(ch,AFF2_TENDRILS))
    {
        if( chance > 20 )
	{
	    send_to_char("You are unable to move while tangled in tendrils.\n\r",ch);
	    return;
        }
        else
        {
            send_to_char("You break free from the tendrils which had entangled you.\n\r",ch);
            sprintf(buf,"%s breaks free from the tendrils entangling them.\n\r",ch->name);
            send_to_char(buf,victim);
        }
    }

	if ( !IS_NPC(ch) )
	    send_to_char( "\nYou launch yourself backwards suprising your foe.\n\n\r", ch );

	act( "\n$n jumps back suddenly and tries to retreat.", ch, NULL, NULL, TO_ROOM );

    was_in = ch->in_room;
    for ( attempt = 0; attempt < 6; attempt++ )
    {
	EXIT_DATA *pexit;
	int door;

	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
	    continue;

	move_char( ch, door );
	if ( ( now_in = ch->in_room ) == was_in )
	    continue;

	ch->in_room = was_in;
	act( "\n$n has retreated from combat.", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;

	if ( !IS_NPC(ch) )
	    send_to_char( "\nYou succesfully retreat from combat.\n\r", ch );

        if ( IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
          REMOVE_BIT(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	if ( IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->newbits, NEW_DARKNESS) )
 	{
	    stc("You leave your darkness in a panic!\n\r",ch);
	    REMOVE_BIT(victim->newbits, NEW_DARKNESS);
	    act("$N leaves $S darkness in a panic!",ch,NULL,victim,TO_NOTVICT);
	}
	stop_fighting( ch, TRUE );
	return;
    }

    send_to_char( "You were unable to retreat.\n\r", ch );
    WAIT_STATE(ch, 12);
    return;
}


/* Ninja 'Scroll' Commmand - Simian 22/07/2001
 * Expanded to show target and attack plan - Simian 23/07/2001
 * Added defines, added pcdata variables - Simian 24/07/2001
 * Still to do:
 * expand for defences and preps
 */

void do_scroll( CHAR_DATA *ch, char *argument ) {

/* Variable declaration */

    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];

 /* Variable initialisation */

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    victim = get_char_world(ch,arg1);

 /* Body of the function */

    if (!str_cmp(ch->name, "Tracker"));
    else
    {   
    send_to_char("Huh?\n\r",ch);
    return;
    }

    if(IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_NINJA))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    // If no argument was passed we check what is on the scroll
    if(arg1[0] == '\0' && arg2[0] == '\0' && arg3[0] == '\0') {
        send_to_char("Ninja's Assassination Scroll\n\r",ch);

        if(ch->pcdata->ns_target == NULL) {
            send_to_char("You haven't selected a target for your assassination yet.\n\r",ch);
            send_to_char("Type 'target <victim>' to select that enemy.\n\r",ch);
            }
        else {
            sprintf(buf,"You are planning to assassinate %s.\r",ch->pcdata->ns_target);
            send_to_char(buf,ch);
            // Check if the target is online
            if(get_char_world(ch,ch->pcdata->ns_target) == NULL) {
                send_to_char(" but you cannot find them at the moment.\n\r",ch);
                }
            else {
                send_to_char(" and can strike your victim when you are ready.\n\r",ch);
                }
            }
        // If none of the details are filled in on the scroll
        if(ch->pcdata->ns_plan[NS_INIT] == 0 &&
           ch->pcdata->ns_plan[NS_ATTACK1] == 0 &&
           ch->pcdata->ns_plan[NS_ATTACK2] == 0 &&
           ch->pcdata->ns_plan[NS_ATTACK3] == 0 &&
           ch->pcdata->ns_plan[NS_DEFEND1] == 0 &&
           ch->pcdata->ns_plan[NS_DEFEND2] == 0 &&
           ch->pcdata->ns_plan[NS_DEFEND3] == 0 &&
           ch->pcdata->ns_plan[NS_PREP1] == 0 &&
           ch->pcdata->ns_plan[NS_PREP2] == 0 &&
           ch->pcdata->ns_plan[NS_PREP3] == 0)
            {
            // Tell the ninja they haven't planned it yet
            send_to_char("You haven't decided on any of the details of the assassination attempt.\n\r",ch);
            send_to_char("Type 'scroll plan' to plan your tactics.\n\r",ch);
            return;
            }

        // Some of the details are completed so we start to describe...
        // ** How the ninja is going to start their attack **

        if(ch->pcdata->ns_plan[NS_INIT] == 0) {
            send_to_char("You haven't planned how to start your attack",ch);
            }
        else {
            send_to_char("You will start the assassination ",ch);
            switch(ch->pcdata->ns_plan[NS_INIT]) {
                case NS_KILL     : send_to_char("by attacking normally",ch); break;
                case NS_BACKSTAB : send_to_char("with a backstab",ch); break;
                case NS_STING    : send_to_char("with the Hornet Sting",ch); break;
                default          : send_to_char("rather unimpressively as your plan appears to be barfed (note imms about this bug)",ch);
            }

        //** How the ninja is going to continue their attack

        // If all these are 0 then they haven't decided
        if(ch->pcdata->ns_plan[NS_ATTACK1] == 0 &&
           ch->pcdata->ns_plan[NS_ATTACK2] == 0 &&
           ch->pcdata->ns_plan[NS_ATTACK3] == 0)
            {
            // Wording changes depending on if the ninja has set their initial attack
            if(ch->pcdata->ns_plan[NS_INIT] == 0) {
                send_to_char(" or how to follow it through.",ch);
                }
            else {
                send_to_char(" but you haven't decided how to continue the attack.",ch);
                }
            }
        // They have decided at least 1 of the three attacks
        else {
            if(ch->pcdata->ns_plan[NS_INIT] == 0) {
                send_to_char(" yet, but you plan to fight ",ch);
                }
            else {
                send_to_char(" and you plan to fight ",ch);
                }

            // Check if the first attack is set
            if(ch->pcdata->ns_plan[NS_ATTACK1] == 0) {
                ;
                // Do nothing as one of the other attack fields is set
                }
            else {
                // The first attack is...
                switch(ch->pcdata->ns_plan[NS_ATTACK1]) {
                    case NS_UNBALANCE : send_to_char("by unbalancing your foe",ch); break;
                    default           : send_to_char("like a ballerina (no honestly!) so you'd better note imms about this bug",ch);
                    }
                }

            // Check if the second attack is set
            if(ch->pcdata->ns_plan[NS_ATTACK2] == 0) {
                ;
                // Do nothing as one of the other attack fields is set
                }
            else {
                // The second attack is set so we check if we sent anything for the first attack
                if(ch->pcdata->ns_plan[NS_ATTACK1] == 0) {
                    ;
                    // Nothing was sent before so no need for a comma etc.
                    }
                // Something was sent for the first attack but what we link with depends on if there is a third attack
                else if(ch->pcdata->ns_plan[NS_ATTACK3] == 0) {
                    // attack3 isn't set so we need to say 'and'
                    send_to_char(" and ",ch);
                    }
                else {
                    // attack3 is set so its a comma
                    send_to_char(", ",ch);
                    }
                // The second attack is...
                switch(ch->pcdata->ns_plan[NS_ATTACK2]) {
                    case NS_UNBALANCE : send_to_char("by unbalancing your foe",ch); break;
                    default           : send_to_char("like a ballerina (no honestly!) so you'd better note imms about this bug",ch); 
                    }
                }

            // Check if the third attack is set
            if(ch->pcdata->ns_plan[NS_ATTACK3] == 0) {
                // Its not set so we do nothing
                ;
                }
            else {
                // Was something sent for the 1st or 2nd attacks
                if(ch->pcdata->ns_plan[NS_ATTACK2] == 0) {
                    // Nothing was sent for the 2nd attack so check the 1st
                    if(ch->pcdata->ns_plan[NS_ATTACK1] == 0) {
                        // The third attack is the only one set so nothing has been sent before and we can do nothing
                        ;
                        }
                    // The first attack was set but not the second so we need to say 'and'
                    else {
                        send_to_char(" and ",ch);
                        }
                    }
                // The second attack was set so we are saying 'and' without checking the first ('cos we don't have to)
                else {
                    send_to_char(" and ",ch);
                    }

                // The 3rd attack is...
                switch(ch->pcdata->ns_plan[NS_ATTACK3]) {
                    case NS_UNBALANCE : send_to_char("by unbalancing your foe",ch); break;
                    default           : send_to_char("like a ballerina (no honestly!) so you'd better note imms about this bug",ch);
                    }
                }
            // We've finished writing about the attacks so we close the line.
            send_to_char(".\n\r",ch);
            }

        // ** How is the ninja going to defend themself **

        // If all these are 0 then they haven't decided
        if(ch->pcdata->ns_plan[NS_DEFEND1] == 0 &&
           ch->pcdata->ns_plan[NS_DEFEND2] == 0 &&
           ch->pcdata->ns_plan[NS_DEFEND3] == 0)
            {
            send_to_char("You haven't decided how you will defend yourself.\n\r",ch);
            }
        // They have decided at least 1 of the three attacks
        else {
            send_to_char("You plan to defend yourself ",ch);

            // Check if the first defence is set
            if(ch->pcdata->ns_plan[NS_DEFEND1] == 0) {
                ;
                // Do nothing as one of the other defence fields is set
                }
            else {
                // The first attack is...
                switch(ch->pcdata->ns_plan[NS_DEFEND1]) {
                    case NS_SIDESTEP  : send_to_char("with the sidestepping defence",ch); break;
                    default           : send_to_char("by asking everyone nicely if they'll stop hitting you.  alternatively you could note imms about this bug",ch);
                    }
                }

            // Check if the second defence is set
            if(ch->pcdata->ns_plan[NS_DEFEND2] == 0) {
                ;
                // Do nothing as one of the other defence fields is set
                }
            else {
                // The second defence is set so we check if we sent anything for the first defence
                if(ch->pcdata->ns_plan[NS_DEFEND1] == 0) {
                    ;
                    // Nothing was sent before so no need for a comma etc.
                    }
                // Something was sent for the first defence but what we link with depends on if there is a third defence
                else if(ch->pcdata->ns_plan[NS_DEFEND3] == 0) {
                    // defend3 isn't set so we need to say 'and'
                    send_to_char(" and ",ch);
                    }
                else {
                    // defend3 is set so its a comma
                    send_to_char(", ",ch);
                    }
                // The second defence is...
                switch(ch->pcdata->ns_plan[NS_DEFEND2]) {
                    case NS_SIDESTEP  : send_to_char("with the sidestepping defence",ch); break;
                    default           : send_to_char("by asking everyone nicely if they'll stop hitting you.  alternatively you could note imms about this bug",ch);
                    }
                }

            // Check if the third defence is set
            if(ch->pcdata->ns_plan[NS_DEFEND3] == 0) {
                // Its not set so we do nothing
                ;
                }
            else {
                // Was something sent for the 1st or 2nd defences
                if(ch->pcdata->ns_plan[NS_DEFEND2] == 0) {
                    // Nothing was sent for the 2nd defence so check the 1st
                    if(ch->pcdata->ns_plan[NS_DEFEND1] == 0) {
                        // The third defence is the only one set so nothing has been sent before and we can do nothing
                        ;
                        }
                    // The first defence was set but not the second so we need to say 'and'
                    else {
                        send_to_char(" and ",ch);
                        }
                    }
                // The second defence was set so we are saying 'and' without checking the first ('cos we don't have to)
                else {
                    send_to_char(" and ",ch);
                    }

                // The 3rd defence is...
                switch(ch->pcdata->ns_plan[NS_DEFEND3]) {
                    case NS_SIDESTEP  : send_to_char("with the sidestepping defence",ch); break;
                    default           : send_to_char("by asking everyone nicely if they'll stop hitting you.  alternatively you could note imms about this bug",ch);
                    }
                }
            // We've finished writing about the defences so we close the line.
            send_to_char(".\n\r",ch);
            }

        // ** How is the ninja going to prepare themselves

        // If all these are 0 then they haven't decided
        if(ch->pcdata->ns_plan[NS_PREP1] == 0 &&
           ch->pcdata->ns_plan[NS_PREP2] == 0 &&
           ch->pcdata->ns_plan[NS_PREP3] == 0)
            {
            send_to_char("You have yet to prepare yourself for the attack.\n\r",ch);
            }
        // They have decided at least 1 of the three preps
        else {
            send_to_char("You have prepared yourself ",ch);

            // Check if the first prep is set
            if(ch->pcdata->ns_plan[NS_PREP1] == 0) {
                ;
                // Do nothing as one of the other prep fields is set
                }
            else {
                // The first prep is...
                switch(ch->pcdata->ns_plan[NS_PREP1]) {
                    case NS_POISON    : send_to_char("by poisoning your weapons",ch); break;
                    default           : send_to_char("with a lovely cup of tea followed by a flower arranging course, alternatively you could note imms about this bug",ch);
                    }
                }

            // Check if the second prep is set
            if(ch->pcdata->ns_plan[NS_PREP2] == 0) {
                ;
                // Do nothing as one of the other prep fields is set
                }
            else {
                // The second prep is set so we check if we sent anything for the first prep
                if(ch->pcdata->ns_plan[NS_PREP1] == 0) {
                    ;
                    // Nothing was sent before so no need for a comma etc.
                    }
                // Something was sent for the first prep but what we link with depends on if there is a third prep
                else if(ch->pcdata->ns_plan[NS_PREP3] == 0) {
                    // prep3 isn't set so we need to say 'and'
                    send_to_char(" and ",ch);
                    }
                else {
                    // prep3 is set so its a comma
                    send_to_char(", ",ch);
                    }
                // The second prep is...
                switch(ch->pcdata->ns_plan[NS_PREP2]) {
                    case NS_POISON  : send_to_char("by poisoning your weapons",ch); break;
                    default           : send_to_char("with a lovely cup of tea followed by a flower arranging course, alternatively you could note imms about this bug",ch);
                    }
                }

            // Check if the third prep is set
            if(ch->pcdata->ns_plan[NS_PREP3] == 0) {
                // Its not set so we do nothing
                ;
                }
            else {
                // Was something sent for the 1st or 2nd preps
                if(ch->pcdata->ns_plan[NS_PREP2] == 0) {
                    // Nothing was sent for the 2nd prep so check the 1st
                    if(ch->pcdata->ns_plan[NS_PREP1] == 0) {
                        // The third prep is the only one set so nothing has been sent before and we can do nothing
                        ;
                        }
                    // The first prep was set but not the second so we need to say 'and'
                    else {
                        send_to_char(" and ",ch);
                        }
                    }
                // The second prep was set so we are saying 'and' without checking the first ('cos we don't have to)
                else {
                    send_to_char(" and ",ch);
                    }

                // The 3rd prep is...
                switch(ch->pcdata->ns_plan[NS_PREP3]) {
                    case NS_POISON  : send_to_char("by poisoning your weapons",ch); break;
                    default           : send_to_char("with a lovely cup of tea followed by a flower arranging course, alternatively you could note imms about this bug",ch);
                    }
                }
            // We've finished writing about the preps so we close the line.
            send_to_char(".\n\r",ch);
            }


        }

    // If arguments were passed

    }

send_to_char("=-=-=-=-=-=-=-",ch);

return;

}

/* Coded by Kyrlin */
void do_ninjagear( CHAR_DATA *ch, char *argument )
{ 
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
 
    if (!IS_NINJA( ch ) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Command: ninjagear <item>.\n\r", ch);
        send_to_char("Items:   Tanto, Ninjanto, Mask.\n\r", ch);
        return;
    }

    if      (!str_cmp( arg, "tanto" ) )    vnum = 29687;
    else if (!str_cmp( arg, "ninjanto" ) ) vnum = 29688;
    else if (!str_cmp( arg, "mask" ) )     vnum = 29689;
    else
    {
        send_to_char("Please specify which item you wish to make: Tanto, Ninjanto, Mask.\n\r",ch);
	return;
    }

    if ( ch->practice < 100)
    {
        send_to_char("It costs 100 points of primal to create an item.\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Tracker.\n\r",ch);
        return;
    }

    ch->practice   -= 100;
    obj = create_object(pObjIndex, 50);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(ch->name);
    obj_to_char(obj, ch);
    act("You reach into the shadows and pull out $p.",ch,obj,NULL,TO_CHAR);
    act("$n reaches into the shadows and pulls out $p.",ch,obj,NULL,TO_ROOM);    
    return;
}

void do_principles( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Principles: Sora (%d), Chikyu (%d), Ningenno (%d).\n\r",
	ch->pcdata->powerss[NPOWER_SORA], ch->pcdata->powerss[NPOWER_CHIKYU],
	ch->pcdata->powerss[NPOWER_NINGENNO]);
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"sora"))
	{
	    send_to_char("Sora - The finding, observing, and locating principle.\n\r\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_SORA] < 1) 
		send_to_char("You have none of the Sora principles.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_SORA] > 0) 
		send_to_char("Mitsukeru -Locate- The scrying power to find enemies.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_SORA] > 1) 
		send_to_char("Koryou -Consider- The read aura power, learn about your enemies.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_SORA] > 2) 
		send_to_char("Kakusu -Hidden- Enhanced Hide.  Only other ninjas can see you.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_SORA] > 3) 
		send_to_char("Uro-Uro -Silent Walk- You leave no footprints behind.\n\r",ch);
		if (ch->pcdata->powerss[NPOWER_SORA] > 4)
		send_to_char("Kanzuite -Aware- The truesight power.\n\r", ch);
		if (ch->pcdata->powerss[NPOWER_SORA] > 5)
		send_to_char("Bomuzite -Sleep Gas- By mixing an potion, you can put everyone in a room to sleep.\n\r", ch);
        if (ch->pcdata->powerss[NPOWER_SORA] > 6)
		send_to_char("Mizotamouyo -Shadowwalk- You are able to walk the paths of darkness to reach another's
location.\r\n", ch);
		return;
	}
	else if (!str_cmp(arg1,"chikyu"))
	{
	    send_to_char("Chikyu - Preperation for battle.\n\r\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_CHIKYU] < 1) 
		send_to_char("You have none of the Chikyu principles.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_CHIKYU] > 0) 
		send_to_char("Tsuyoku -Strength- Toughness.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_CHIKYU] > 1) 
		send_to_char("Songai -Damage- Enhanced damage.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_CHIKYU] > 2) 
		send_to_char("Isogu -Haste- Adds two extra attacks.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_CHIKYU] > 3) 
		send_to_char("Tsuiseki -Hunt- Fast hunting.\n\r",ch);
		if (ch->pcdata->powerss[NPOWER_CHIKYU] > 4)
		send_to_char("Sakeru -Sonic Speed- Dodge more attacks.\n\r", ch);
	    if (ch->pcdata->powerss[NPOWER_CHIKYU] > 5)
		send_to_char("HaraKiri -Blood Power- Hurt yourself to gain power.\n\r", ch );
        if (ch->pcdata->powerss[NPOWER_CHIKYU] > 6)
		send_to_char("Orimasu -Mending- Regenerate your being through willpower alone.\n\r", ch );
	if (ch->pcdata->powerss[NPOWER_CHIKYU] > 7)
		send_to_char("Gunaya - Entrap your foes with a spiritual net.\n\r", ch);
		return;
	}
	else if (!str_cmp(arg1,"ningenno"))
	{
	    send_to_char("Ningenno - The battle, attacking and getting away.\n\r\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_NINGENNO] < 1) 
		send_to_char("You have none of the Ningenno principles.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_NINGENNO] > 0) 
		send_to_char("Tsume -Claw- IronClaw always worn on wrist to aid in hand-to-hand.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_NINGENNO] > 1) 
		send_to_char("Hakunetsu -First Strike- You get super backstabs.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_NINGENNO] > 2) 
		send_to_char("Mienaku -Vanish- Never fail flee.\n\r",ch);
	    if (ch->pcdata->powerss[NPOWER_NINGENNO] > 3) 
		send_to_char("Shiroken -Throwing Star- Added attack per round, like headbutt for demons.\n\r",ch);
		if (ch->pcdata->powerss[NPOWER_NINGENNO] > 4)
		send_to_char("Dokuyaku -Poison- Adds poisons to the Shiroken\n\r", ch);
	    if (ch->pcdata->powerss[NPOWER_NINGENNO] > 5)
    	send_to_char("Circle - circle behind your opponent and hit him during battle\n\r", ch );
		if (ch->pcdata->powerss[NPOWER_NINGENNO] > 6)
		send_to_char("Trip - Trip your opponent and send him sprawling to the ground.\n\r", ch );

	    return;
	}
	sprintf(buf,"Principles: Sora (%d), Chikyu (%d), Ningenno (%d).\n\r",
	ch->pcdata->powerss[NPOWER_SORA], ch->pcdata->powerss[NPOWER_CHIKYU],
	ch->pcdata->powerss[NPOWER_NINGENNO]);
	send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"sora"   )) {improve = NPOWER_SORA;   max=7;}
	else if (!str_cmp(arg1,"chikyu"   )) {improve = NPOWER_CHIKYU;   max=8;}
	else if (!str_cmp(arg1,"ningenno"   )) {improve = NPOWER_NINGENNO; max=7;}
	else
	{
	    send_to_char("Principles: Sora, Chikyu, Ningenno.\n\r",ch);
	    return;
	}							
	cost = (ch->pcdata->powerss[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powerss[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the powers of the %s principle.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your %s principle.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powerss[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You improve your ability in the %s principle.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a principle, type: Principle <principle type> improve.\n\r",ch);
	return;
}

void do_michi(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

  if ( IS_CLASS(ch, CLASS_NINJA) )
  {
	
	if ( ch->pcdata->stats[UNI_RAGE] >= 50 )
	{
	    send_to_char("But you are already in the state of Michi.\n\r",ch);
	    return;
	}
	if ( ch->pcdata->powerss[NINJAKI] < 100 ) 
	{
		send_to_char("But you do not have enough Ki to perform the michi.\n\r", ch);
		return;
	} 
	
	send_to_char("You are gifted positive energy while performing the michi.\n\r",ch);
	act("$n is gifted positives energy while performing the michi.",ch,NULL,NULL,TO_ROOM);
	ch->pcdata->stats[UNI_RAGE] += 200;
	  ch->pcdata->powerss[NINJAKI] -= 100;
	WAIT_STATE(ch,12);
	return;
  }
	else
    send_to_char("But you are already in the state of Michi.\n\r",ch);
    return;
}

void do_discipline( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    bool      can_sire = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
	
    if (!IS_NINJA(ch) || (ch->pcdata->stats[UNI_GEN] < 1) )
    {
        send_to_char("Huh?\n\r", ch );
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
	send_to_char("You are not able to discipline new ninjas.\n\r",ch);
	return;
    }

    if (!str_cmp(ch->clan,"") && ch->pcdata->stats[UNI_GEN] == 2)
    {
	send_to_char( "First you must become Jonin of a Family.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Discipline whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They arent here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPCs.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot discipline yourself.\n\r", ch );
	return;
    }

    if (IS_NINJA(victim) )
    {
	send_to_char( "They are already disciplined.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only teach avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to discipline vampires!\n\r", ch );
	return;
    }

    if (victim->class != 0)
    {
        send_to_char("They already have a profession.\n\r", ch );
        return;
    }

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot discipline an unwilling person.\n\r", ch );
	return;
    }

    if( victim->max_hit < 1500 )
    {
        send_to_char("They do not have 1500 hitpoints.\n\r", ch);
        return;
    }
        
    if (ch->exp < 100000)
    {
	send_to_char("You cannot afford the 100000 exp required to discipline them.\n\r",ch);
	return;
    }

    if (victim->exp < 100000)
    {
	send_to_char("They cannot afford the 100000 exp required to be disciplined from you.\n\r",ch);
	return;
    }

    ch->exp -= 100000;
    victim->exp -= 100000;

    act("You discipline $N the ways of the ninja.", ch, NULL, victim, TO_CHAR);
    act("$n disciplines $N the ways of the ninja.", ch, NULL, victim, TO_NOTVICT);
    act("$n disciplines you the way of the ninja.", ch, NULL, victim, TO_VICT);
    victim->class    = CLASS_NINJA;
    send_to_char( "You are now a ninja.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->pcdata->stats[UNI_GEN] = ch->pcdata->stats[UNI_GEN] + 1;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_kakusu( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}


	
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_SORA] < 3 )
	{
	  send_to_char("You have not learned the Sora principle to 3.\n\r", ch);
	  return;
}
if (ch->fight_timer >0) {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;}	



	if ( ch->pcdata->powerss[NINJAKI] < 5 )
	{
		send_to_char("You dont have 5 ki to activate your power.\n\r", ch);
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
/*Changed Toaster Kak costs 5 to put on but slowly drains ki was 30*/
	ch->pcdata->powerss[NINJAKI] -= 5;
    }
    return;
}

void do_orimasu(CHAR_DATA *ch, char *argument)
{
	if(IS_NPC(ch)) return;
	if(!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_CHIKYU] < 7)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if( ch->pcdata->powerss[NINJAKI] < 15 )
	{
		send_to_char("You do not have the required mental energies to mend your being.\r\n", ch);
		return;
	}
	else if( ch->position == POS_FIGHTING )
	{
		send_to_char("You cannot mend your being with such distractions!\r\n", ch);
		return;
	}

	send_to_char("You take a deep breath and attempt to gain mastery over your injuries.\r\n", ch);
	act( "$n takes a deep breath and begins to concentrate.", ch, NULL, NULL, TO_ROOM );
	if( number_range(1, 5) != 1 )
	{
		send_to_char("You mend some of your injuries by force of will alone.", ch);
		ch->hit  += (int)(ch->max_hit  * (double)((double)1/(double)(number_range(8,12))));
		ch->mana += (int)(ch->max_mana * (double)((double)1/(double)(number_range(12,16))));
		ch->move += (int)(ch->max_move * (double)((double)1/(double)(number_range(12,16))));

		if( ch->hit > ch->max_hit ) ch->hit = ch->max_hit;
		if( ch->mana > ch->max_mana ) ch->mana = ch->max_mana;
		if( ch->move > ch->max_move ) ch->move = ch->max_move;
		
		ch->pcdata->powerss[NINJAKI] -= 15;
		act( "$n's wounds slowly begin to close before your eyes!", ch, NULL, NULL, TO_ROOM );
		WAIT_STATE(ch, 12);
		return;
	}
	else
	{
		send_to_char("You were unable to gain mastery over your mortal being.", ch);
		ch->pcdata->powerss[NINJAKI] -= 5;
		WAIT_STATE(ch, 12);
		return;
	}
}

void do_kanzuite( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_SORA] < 5 )
	{
	  send_to_char("You have not learned the Sora principle to 5.\n\r", ch);
	  return;
	}
	
	else if ( ch->pcdata->powerss[NINJAKI] < 30 )  
	{
		send_to_char("You don't have 30 Ki to increase your awareness.\n\r", ch);
		return;
	} 

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "You're senses return to normal.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "You're senses increase into incredible proportions.\n\r", ch );
	 ch->pcdata->powerss[NINJAKI] -= 30; 
    }

    return;
}

void do_mienaku( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;


	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powerss[NPOWER_NINGENNO] < 3 )
	{
	send_to_char("You have not learned the Ningenno principle to 3.\n\r", ch);
	return;
	}
	else if (!IS_CLASS(ch, CLASS_NINJA) )
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}
	
	if ( IS_NPC(ch) ) return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ch->position == POS_FIGHTING )
	    ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }
	if ( ch->pcdata->powerss[NINJAKI] < 20 )  
	{
	send_to_char("You don't have enough Ki points to flee.\n\r", ch);
	return;
	} 
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
  
    was_in = ch->in_room;
    
{
	EXIT_DATA *pexit;
	int door;

	send_to_char("You move to vanish from combat!\n\r", ch );

for ( attempt =  0; attempt < 6; attempt++ ) 
	{
	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
	      continue;


	move_char( ch, door );
	if ( ( now_in = ch->in_room ) == was_in )
	      continue;

/* Use escape instead of flee so people know it's the ninja power */
	ch->in_room = was_in;
	act( "$n has escaped!", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;
	
	if ( !IS_NPC(ch) )
	    send_to_char( "You escape from combat!\n\r", ch ); 

/*	if (victim->in_room == ch->in_room)
    {
    	SET_BIT(victim->extra,BLINDFOLDED);
    	act("You start to move at super speeds and blind $N.",ch,NULL,victim,TO_CHAR);
    	act("$n starts to move at super speeds and blinds $N.",ch,NULL,victim,TO_ROOM);
    	act("$n starts to move at super speeds and blinds you.",ch,NULL,victim,TO_VICT);
    	
    	return;
    }
  */  
	  ch->pcdata->powerss[NINJAKI] -= 20; 
	stop_fighting( ch, TRUE );
	return;
   }
   
}
    return;
}

void do_ningentra( CHAR_DATA *ch, char *argument )
{
	sh_int rvnum;
	CHAR_DATA *victim;

	rvnum = -1;
	if (!IS_CLASS(ch, CLASS_NINJA))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	if (ch->pcdata->stats[UNI_GEN] != 2)
	{
		send_to_char("Only the ninja leaders have knowledge of this power.\n\r", ch);
		return;
	}
	/* Define room numbers here per clan */
	if (!str_cmp(ch->clan, "Shadow")) rvnum = 31920;

	if (rvnum == -1) 
	{
		send_to_char("Your clan HQ has not been recognised...\n\r", ch);
		send_to_char("Please request Hunter to add the HQ to the list.\n\r", ch);
		return;
	}
	if (ch->in_room->vnum != rvnum)
	{
		send_to_char("You can only use this in your HQ.\n\r", ch);
		return;
	}
	
	if ((victim = get_char_world(ch, argument)) == NULL)
	{
		send_to_char("You cannot find them.\n\r", ch);
		return;
	}
	
	if (str_cmp(victim->clan, ch->clan))
	{
		send_to_char("You can only call those of your own.\n\r", ch);
		return;
	}
	else
	if (victim->position == POS_FIGHTING)
	{
		send_to_char("Sorry, they are too busy right now.\n\r", ch);
		return;
	}
	else
	{
		char_from_room( victim );
		char_to_room( victim, ch->in_room);
		send_to_char( "Ok, they are in the HQ.\n\r", ch);
		send_to_char( "You have been put in your HQ by your master.\n\r", victim);
		send_to_char( "Type HOME HERE to make this your home.\n\r", victim);
	}
	return;
}	
	

void do_bomuzite( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    int sn;
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ( ch->pcdata->powerss[NPOWER_SORA] < 6 )
	{
	    send_to_char("You have not learned the Sora principle to 6.\n\r", ch);
	    return;
	}
	
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Bomb who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
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
    
	if ( ch->pcdata->powerss[NINJAKI] < 30 )  
	{
	send_to_char("You do not have enough Ki points.\n\r", ch);
	return;
	}     
	  
    if ( victim->in_room == ch->in_room )
	{
	act("You toss your bomb onto the floor and put $N to sleep.",ch,NULL,victim,TO_CHAR);
	act("$n tosses a bomb onto the floor.  You feel sleepy.",ch,NULL,victim,TO_VICT);
	victim->position = POS_SLEEPING;
	
	 sn = skill_lookup( "sleep" );
    af.type      = sn;
    af.duration  = ch->pcdata->powerss[NINJAKI]/300;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );
	  ch->pcdata->powerss[NINJAKI] -= 20; 
	WAIT_STATE(ch, 24);
	return;
	}
	
	return;
}


void do_tsume(CHAR_DATA *ch, char *argument) {

		if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_NINGENNO] < 1) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_VAMPAFF(ch, VAM_CLAWS)) {

	send_to_char("You remove the IronClaws from your wrists.\n\r", ch );
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
	return;}

	send_to_char("You attach IronClaws to your wrists.\n\r", ch );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);

	return;
}

void do_hara_kiri(CHAR_DATA *ch, char *argument) {

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_CHIKYU] < 6) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (ch->pcdata->powerss[HARA_KIRI] > 0) {
	send_to_char("You are already experiencing the power of HaraKiri.\n\r", ch );
	return;}

	if (ch->hit < ch->max_hit/10) {
	send_to_char("You are hurt too badly already.\n\r", ch );
	return;}

	ch->pcdata->powerss[HARA_KIRI] = ch->hit/500;
	if (ch->pcdata->powerss[HARA_KIRI] < 5)
	ch->pcdata->powerss[HARA_KIRI] = 5;	
	ch->hit = 1;
	ch->mana = 1;
	ch->move = 1;
	send_to_char("You cut your finger and bleed profusely.\n\r", ch );
	act("$n cuts his finger and obtains the power of HaraKiri.",ch,NULL,NULL,TO_ROOM);
	return;
}

void do_trip(CHAR_DATA *ch, char *argument) {

	char  arg[MAX_STRING_LENGTH];
	char  buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	
	argument = one_argument(argument, arg);
	
	if( IS_NPC(ch) ) return;

	if(!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_NINGENNO] < 7 ) {
		send_to_char("Huh?\n\r", ch);
		return;
	}

    
    if( (victim = ch->fighting) == NULL )
	{
		if( arg[0] == '\0' )
		{
			send_to_char("Trip whom?\r\n", ch);
			return;
		}
		if( (victim = get_char_room(ch, arg)) == NULL )
		{
			send_to_char("How can you trip someone who is not here?\r\n", ch);
			return;
		}
        else if( IS_NPC(victim) )
		{
			send_to_char("You cannot trip them!\r\n", ch);
			return;
		}
	}
   
	if( number_range(1,4) != 1 )
	{
		sprintf(buf, "You skillfully trip %s, and they go down!\r\n", victim->name);
		send_to_char(buf, ch);
		sprintf(buf, "%s makes an impressive fighting move, and you land on your butt!\r\n", ch->name);
		send_to_char(buf, victim);
		WAIT_STATE(ch, 10);
		WAIT_STATE(victim, 15);
		return;
	}
	else
	{
		send_to_char("You attempt an impressive fighting move, but instead land on your butt!\r\n", ch);
		sprintf(buf, "%s attempts to make an impressive fighting move, but lands on his butt!\r\n", ch->name);
	    send_to_char(buf, victim);
		WAIT_STATE(ch, 20);
		return;
    }
}


void do_mitsukeru( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_SORA] <1) {
	send_to_char("Huh?\n\r", ch );
	return;}
	if (arg[0] == '\0')
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
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
//    ch->fight_timer = 10;
    return;
}

void do_mizotamouyo( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char	  arg[MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg);

	if( IS_NPC(ch) ) return;
	if( !IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powerss[NPOWER_SORA] < 7 )
	{
		send_to_char("Huh?", ch);
		return;
	}

        if (ch->fight_timer > 0) 
	{
        send_to_char("Not until your fight timer expires.\n\r", ch );
	return;
	}

	if( arg[0] == '\0' )
	{
		send_to_char("Who do you wish to walk to via the paths of darkness?\r\n", ch);
		return;
	}
	
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   ch->in_room == NULL)    
    {
		send_to_char( "You attempt to walk through the shadows, but fail.\n\r", ch );
		return;
    }

    if ( victim->in_room == ch->in_room )
    {
		send_to_char( "But you are already there!\n\r", ch );
		return;
    }

    if ( IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) ) )
    {
		send_to_char( "You are unable to focus on their location.\n\r", ch );
		return;
    }

    send_to_char("You dissipate into the shadows.\n\r",ch);
    act("$n dissipates into the shadows, slowly vanishing from sight.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You coalesce from the darkness which surrounds you.\n\r",ch);
//    ch->pcdata->powers[NINJAKI] -= 15;
    ch->move -= (ch->max_move/10);
    if(ch->move < 0) ch->move = 0;
    WAIT_STATE(ch, 5);
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
	else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r",
victim->name);
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
   return;
   }


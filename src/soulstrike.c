/* Coded by Toaster for a status based attack, blah blah blah */

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


/* Local functions */
int     apply_damage     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );

int apply_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
      float reduct = 1;
      int MAX_DAM;

      MAX_DAM = 15000;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) && dam > 1  )
    {
    dam *= 0.5;
    }

    if(IS_CLASS(victim, CLASS_WEREWOLF) )

        {
            if ( IS_GAR1(victim, WOLF_COCOON) )
              reduct += .1;
            if ( victim->gifts[PHILODOX] >= 1 )
              reduct += .1;
            if ( victim->gifts[PHILODOX] >= 3 )
              reduct += .1;
            if ( victim->gifts[SHADOWLORDS] >= 2 )
              reduct += .1;
            if ( victim->gifts[SHADOWLORDS] >= 4 )
              reduct += .1;
        }
    if(IS_CLASS(victim, CLASS_VAMPIRE) )
    {

 	if ( victim->pcdata->powers[VAM_FORT] == 0 )
     		reduct += 0;    	
    	if ( victim->pcdata->powers[VAM_FORT] >= 1 )    	
    		reduct += .1;    	
    	if ( victim->pcdata->powers[VAM_FORT] >= 2 )    	
    		reduct += .1;    	
    	if ( victim->pcdata->powers[VAM_FORT] >= 3 )    	
    		reduct += .1;    	
    	if ( victim->pcdata->powers[VAM_FORT] >= 4 )    	
    		reduct += .1;    	
    	if ( victim->pcdata->powers[VAM_FORT] >= 5 )    	
    		reduct += .1;
    	
    }
    if ( IS_CLASS(victim, CLASS_MAGE) )   
    {
        if ( victim->spheres[MSPI] >= 1 )
        reduct += 1.5;
    }

    dam /= reduct;
    if (dam > MAX_DAM) dam = MAX_DAM;
    return dam;
}


void do_soulstrike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int dam;
 
    argument = one_argument( argument, arg1 );
 
    if ( IS_NPC(ch) )
    return;
/* 
    if (ch->race < 2)
    {
	send_to_char("You need at least 2 status before you can use this.\n\r",ch);
    	return;
    }
*/
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Whose Spirit do you wish to grip?\n\r", ch );
   	return;
    }
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
    	send_to_char( "They aren't here.\n\r", ch );
   	return;
    }
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't spiritgrip a fighting person.\n\r", ch );
        return;
    }
       
    if ( victim->hit < victim->max_hit )
    {
        act( "$N is hurt and suspicious ... you do it.",
            ch, NULL, victim, TO_CHAR );
        return;
    }

    if (is_safe(ch,victim) == TRUE) return;
    if (IS_NPC(victim))
    {
	send_to_char("You cannot use this on Mobs.\n\r", ch ); 
	return;
    }
//    if (!IS_NPC(victim)) dam= number_range( 200, (((ch->race) * 1000)/2) );
    dam = victim->max_hit;
    dam = apply_damage(ch, victim, dam);

    sprintf(buf,"Your plunge your hand into $Ns body and grip their spirit. [%d]",dam);
    ADD_COLOUR(ch,buf, L_RED);
    act(buf,ch,NULL,victim,TO_CHAR);
    sprintf(buf,"$n hand enters your body gripping your spirit. [%d]",dam);
    ADD_COLOUR(ch,buf, L_RED);
    act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"$n plunges thier hand into $Ns body and grips their spirit.");
    ADD_COLOUR(ch,buf, L_RED);
    act(buf,ch, NULL,victim,TO_NOTVICT);
    send_to_char("\n\r",ch);
    victim->hit -= dam;
    ch->move -= 100;
    WAIT_STATE(ch,12);
    return;
    }
/*
void do_idle(CHAR_DATA *ch, char *argument)
{

    CHAR_DATA *vch;
    DESCRIPTOR_DATA *d;
    char        buf[MAX_INPUT_LENGTH];
    int count;
    char * 	st;
    char	s[100];
    char	idle[10];

    count 	= 0;
    

    send_to_char("Players     Idle\n\r-----------------------------------------------------------\n\r",ch);
 for ( d = descriptor_list; d != NULL; d = d->next )
    {                                                                          
     count++;

}
*/

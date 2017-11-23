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

const char * prey_name [9] =
{
    "Demon", "Garou",
    "Highlander", "Kindred",
    "Warlock", "Blah",
    "Blah", "Blah",
    "Blah"
};


void do_edges( CHAR_DATA *ch, char *argument )
{
//  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
//  int disc, cost; 
//  cost = 0;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( !IS_HUNTER(ch) )
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( arg1[0] == '\0' )
  {
      divide_to_char(ch);
      stc("#P=-#n                                  #rIMBUED#n                                   #P-=#n\n\r",ch);
      stc("#P=-=-=-=-=-=-=-=-=-=-=-=-=#B[#G**#B]#w        EDGES        #B[#G**#B]#P=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
	stc("#P=-#n                                                                           #P-=#n\n\r",ch);
	stc("#P=-#n    #eMERCY                   VISION                   ZEAL#n                  #P-=#n\n\r",ch);
	stc("#P=-#n                                                                           #P-=#n\n\r",ch);
	stc("#P=-#n  #wInnocence  #B(#G     #B)#w       Visionary #B(#G     #B)#w       Defence   #B(#G     #B)#n       #P-=#n\n\r",ch);
	stc("#P=-#n  Martyrdom  (*****)                               Judgement (*****)       #P-=#n\n\r",ch);
	stc("#P=-#n  Redemption (*****)                               Vengeance (*****)       #P-=#n\n\r",ch);
	stc("#P=-#n                                                                           #P-=#n\n\r",ch);
      divide_to_char(ch);
      return;
  }
}

/*
void do_study( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int disc, cost; 
  cost = 0;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( !IS_HUNTER(ch) )
  {
    stc("Huh?\n\r",ch);
    return;
  }

  if ( arg1[0] == '\0' )
  {
      divide_to_char(ch);
      stc("#P=-#n                                  #rHUNTERS#n                                  #P-=#n\n\r",ch);
    	stc("#P=-=-=-=-=-=-=-=-=-=-=-=-=#B[#G**#B]#w     PROFESSIONS     #B[#G**#B]#P=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
	stc("#P=-#n                                                                           #P-=#n\n\r",ch);
	stc("#P=-#n                                                                           #P-=#n\n\r",ch);
	stc("#P=-=-=-=-=-=-=-=-=-=-=-=-=#B[#G**#B]#w        PREY         #B[#G**#B]#P=-=-=-=-=-=-=-=-=-=-=-=-=#n\n\r",ch);
	stc("#P=-#n                                                                           #P-=#n\n\r",ch);
sprintf(buf,"#P=-#n          Demon #B(#G%s%s%s%s%s#B)#n         Garou #B(#G%s%s%s%s%s#B)#n       Highlander #B(#G%s%s%s%s%s#B)#n     #P-=#n\n\r",
 	       ch->hprey[HUNTDEM] >= 1 ? "*" : " ",
             ch->hprey[HUNTDEM] >= 2 ? "*" : " ",
             ch->hprey[HUNTDEM] >= 3 ? "*" : " ",
             ch->hprey[HUNTDEM] >= 4 ? "*" : " ",
             ch->hprey[HUNTDEM] >= 5 ? "*" : " ",
             ch->hprey[HUNTWER] >= 1 ? "*" : " ",
             ch->hprey[HUNTWER] >= 2 ? "*" : " ",
             ch->hprey[HUNTWER] >= 3 ? "*" : " ",
             ch->hprey[HUNTWER] >= 4 ? "*" : " ", 
             ch->hprey[HUNTWER] >= 5 ? "*" : " ",  
             ch->hprey[HUNTHIG] >= 1 ? "*" : " ",
             ch->hprey[HUNTHIG] >= 2 ? "*" : " ",
             ch->hprey[HUNTHIG] >= 3 ? "*" : " ",
             ch->hprey[HUNTHIG] >= 4 ? "*" : " ", 
             ch->hprey[HUNTHIG] >= 5 ? "*" : " ");
        stc( buf, ch);
sprintf(buf,"#P=-#n        Kindred #B(#G%s%s%s%s%s#B)#n       Warlock #B(#G%s%s%s%s%s#B)#n                              #P-=#n\n\r",
             ch->hprey[HUNTVAM] >= 1 ? "*" : " ",
             ch->hprey[HUNTVAM] >= 2 ? "*" : " ",
             ch->hprey[HUNTVAM] >= 3 ? "*" : " ",
             ch->hprey[HUNTVAM] >= 4 ? "*" : " ", 
             ch->hprey[HUNTVAM] >= 5 ? "*" : " ",  
             ch->hprey[HUNTMAG] >= 1 ? "*" : " ",
             ch->hprey[HUNTMAG] >= 2 ? "*" : " ",
             ch->hprey[HUNTMAG] >= 3 ? "*" : " ",
             ch->hprey[HUNTMAG] >= 4 ? "*" : " ", 
             ch->hprey[HUNTMAG] >= 5 ? "*" : " ");	
        stc( buf, ch);
	  stc("#P=-#n                                                                           #P-=#n\n\r",ch);
      divide_to_char(ch);
        return;
  }

  if ( arg2[0] == '\0' )
  {
    if ( !str_prefix( arg1, "demon"  ) )
    {
	if ( ch->hprey[HUNTDEM] == 0 )
        {
	  stc("You have no knowledge of demons.\n\r",ch);
	  return;
	}

	if ( ch->hprey[HUNTDEM] >= 1 )
        {
	  divide_to_char(ch);
	  centre_text("Knowledge of Demons",ch);
	  divide_to_char(ch);
	  centre_text("What are Demons?",ch);
	}
	if ( ch->hprey[HUNTDEM] >= 2 )
          centre_text("What are Demons afraid off?",ch);
	if ( ch->hprey[HUNTDEM] >= 3 )
	  centre_text("Fuck this for a laugh.",ch);
	if ( ch->hprey[HUNTDEM] >= 4 )
	  centre_text("Yadda Yadda Yadda",ch);
	if ( ch->hprey[HUNTDEM] >= 5 )
	  centre_text("Fry Demons Fry!",ch);
	divide_to_char(ch);
	return;
    } 

    if ( !str_prefix( arg1, "garou" ) || !str_prefix(arg1, "werewolf") )
    {
	if ( ch->hprey[HUNTWER] == 0 )
	{
	  stc("You have no knowledge of the Garou.\n\r",ch);
	  return;
	}
	
	if ( ch->hprey[HUNTWER] >= 1 )
	{
	  centre_text("Knowledge of Poodles",ch);
	  divide_to_char(ch);
	  centre_text("Flea Infection",ch);
	}
	if ( ch->hprey[HUNTWER] >= 2 )
	  centre_text("Pissing on Lamp posts",ch);
	if ( ch->hprey[HUNTWER] >= 3 )
	  centre_text("Where to stick this lot.",ch);
	if ( ch->hprey[HUNTWER] >= 4 )
	  centre_text("Finish all theres yerself.",ch);
	if ( ch->hprey[HUNTWER] >= 5 )
	  centre_text("Kill that bastard poodle boy!",ch);
	divide_to_char(ch);
	return;
    }
  
    if ( !str_prefix( arg1, "highlander" ) )
    {
      if ( ch->hprey[HUNTHIG] == 0 )
      {
	stc("You have no knowledge of the immortals.\n\r",ch);
	return;
      }

      if ( ch->hprey[HUNTHIG] >= 1 )
      {
	divide_to_char(ch);
	centre_text("About sword wielding poofs.",ch);
	divide_to_char(ch);
	centre_text("Analyze your bottom",ch);
      }
      if ( ch->hprey[HUNTHIG] >= 2 )
        centre_text("Alter your trousers",ch);
      if ( ch->hprey[HUNTHIG] >= 3 )
	centre_text("Take the Weight",ch);
      if ( ch->hprey[HUNTHIG] >= 4 )
	centre_text("Enchanting sex toys",ch);
      if ( ch->hprey[HUNTHIG] >= 5 )
	centre_text("Hover about and wait to be paid",ch);
      divide_to_char(ch);
      return;
    }

    if ( !str_prefix( arg1, "kindred" ) || !str_prefix(arg1, "vampire") )
    {
      if ( ch->hprey[HUNTVAM] == 0 )
      {
	stc("You have no knowledge of the Kindred.\n\r",ch);
	return;
      }

      if ( ch->hprey[HUNTVAM] >= 1 )
      {
	divide_to_char(ch);
	centre_text("I like these",ch);
	divide_to_char(ch);
	centre_text("Have some blood",ch);
      }
      if ( ch->hprey[HUNTVAM] >= 2 )
     	centre_text("Blood Kiss",ch);
      if ( ch->hprey[HUNTVAM] >= 3 )
	centre_text("Awaken the Inanimate",ch);
      if ( ch->hprey[HUNTVAM] >= 4 )
	centre_text("Spirit Blast",ch);
      if ( ch->hprey[HUNTVAM] >= 5 )
	centre_text("Breach the Gauntlet",ch);
      divide_to_char(ch);
      return;
    }

  if ( !str_prefix( arg1, "warlock" ) || !str_prefix(arg1, "mage") )
    {
      if ( ch->hprey[HUNTMAG] == 0 )
      {
        stc("You have no knowledge of warlocks.\n\r",ch);
        return;
      }
      
      if ( ch->hprey[HUNTMAG] >= 1 )
      {
        divide_to_char(ch);
        centre_text("How to lift robes!",ch);
        divide_to_char(ch);
        centre_text("Chant chant chant",ch);
      }
      if ( ch->hprey[HUNTMAG] >= 2 )
        centre_text("I'm off for food",ch);
      if ( ch->hprey[HUNTMAG] >= 3 )
        centre_text("Lalala",ch);
      if ( ch->hprey[HUNTMAG] >= 4 )
        centre_text("This does fuck all",ch);
      if ( ch->hprey[HUNTMAG] >= 5 )
        centre_text("Laugh your ass off, what a waste",ch);
      divide_to_char(ch);
      return;
    }

      return;
    }      


  if ( !str_cmp( arg2, "learn" ) )
  {
         if ( !str_prefix( arg1, "demon" ) )
      disc = HUNTDEM;
    else if ( !str_prefix( arg1, "garou" ) )
      disc = HUNTWER;
    else if ( !str_prefix( arg1, "highlander" ) )
      disc = HUNTHIG;
    else if ( !str_prefix( arg1, "kindred" ) )
      disc = HUNTVAM;
    else if ( !str_prefix( arg1, "warlock" ) )
      disc = HUNTMAG;
    else
    {
      stc("You must select from the following: Demon, Garou, Highlander, Kindred, Warlock.\n\r",ch);
      return;
    }
  }
  else
  {
    do_study(ch,"");
    return;
  }

  cost = ( ( ch->hprey[disc] + 1 ) * 10 );

  if ( ch->hprey[disc] >= 5 )
  {
    sprintf(buf, "You have already learned everything about %s's.\n\r",prey_name[disc]);
    stc(buf,ch);
    return;
  }

  if ( ch->practice < cost )
  {
    sprintf(buf, "You need %d primal to obtain new understanding of %s's.\n\r", cost, prey_name[disc] );
    stc(buf,ch);
    return;
  }

  ch->practice -= cost;
  ch->hprey[disc]++;
  sprintf(buf, "Your knowledge of %s's increases.\n\r",prey_name[disc] );
  stc(buf,ch);
  return;
}
*/

void do_hunting( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );


    if(!IS_HUNTER(ch) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' )
    {
	send_to_char("What class do you wish to hunt?\n\r", ch);
	return;
    }
    if (!str_cmp(arg1,"Vampire") )
    {
        if(IS_SET(ch->special, SPC_WERE_HUN) )
        {
            stc("But you are already hunting Werewolfs!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_MAGE_HUN) )
        {
            stc("But you are already hunting Mages!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_DEMO_HUN) )
        {
            stc("But you are already hunting Demons!\n\r",ch);
            return;
        }
        else
	      stc("You have chosen to rid the world of Vampires.\n\r", ch);
    	      SET_BIT(ch->special, SPC_VAMP_HUN);
	      return;
    }
    else if (!str_cmp(arg1,"werewolf") )
    {
        if(IS_SET(ch->special, SPC_VAMP_HUN) )
        {
            stc("But you are already hunting Vampires!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_MAGE_HUN) )
        {
            stc("But you are already hunting Mages!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_DEMO_HUN) )
        {
            stc("But you are already hunting Demons!\n\r",ch);
            return;
        }
        else
            stc("You have chosen to rid the world of Werewolfs.\n\r", ch);
            SET_BIT(ch->special, SPC_WERE_HUN);
            return;
    }
    else if (!str_cmp(arg1,"mage") )
    {
        if(IS_SET(ch->special, SPC_WERE_HUN) )
        {
            stc("But you are already hunting Werewolfs!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_VAMP_HUN) )
        {
            stc("But you are already hunting Vampires!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_DEMO_HUN) )
        {
            stc("But you are already hunting Demons!\n\r",ch);
            return;
        }
        else
            stc("You have chosen to rid the world of Mages.\n\r", ch);
            SET_BIT(ch->special, SPC_MAGE_HUN);
            return;
    }
    else if (!str_cmp(arg1,"demon") )
    {
        if(IS_SET(ch->special, SPC_WERE_HUN) )
        {
            stc("But you are already hunting Werewolfs!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_MAGE_HUN) )
        {
            stc("But you are already hunting Mages!\n\r",ch);
            return;
        }
        if(IS_SET(ch->special, SPC_VAMP_HUN) )
        {
            stc("But you are already hunting Vampires!\n\r",ch);
            return;
        }
        else
            stc("You have chosen to rid the world of Demons.\n\r", ch);
            SET_BIT(ch->special, SPC_DEMO_HUN);
            return;
    }
    else if (!str_cmp(arg1,"none") )
    {
        send_to_char("You have cleared your Hunting list.\n\r",ch);  
        REMOVE_BIT(ch->special, SPC_DEMO_HUN);
        REMOVE_BIT(ch->special, SPC_VAMP_HUN);
        REMOVE_BIT(ch->special, SPC_WERE_HUN);
        REMOVE_BIT(ch->special, SPC_MAGE_HUN);
        return;
    }
    else
    {
        stc("You must select your prey from the following:\n\r",ch);
        stc("Demon, Werewolf, *****, Vampire, Mage.\n\r",ch);
      return;
    }
return;
}

/* Classing power*/
/*
void do_imbue( CHAR_DATA *ch, char *argument )
{
*/

void do_imbued( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *hch;

    if(!IS_HUNTER(ch)) return;

    stc("The following people are Imbued:\n\r",ch);
    stc("[   Name   ] [   Sect   ] [  Hunting  ]\n\r", ch);
    for (hch = char_list; hch != NULL; hch = hch->next )
    {
	if (IS_NPC(hch) ) continue;
      if (IS_IMMORTAL(hch) ) continue;
	if (!IS_HUNTER(hch) ) continue;
	{
	  sprintf( buf, 
	  "[%-10s]\n\r",
	  	capitalize (hch->name) );
	 send_to_char( buf, ch );
	}
    }
return;
}

void do_torture( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char arg1 [MAX_INPUT_LENGTH];
        
    argument = one_argument( argument, arg1 );
        
        
    if(!IS_HUNTER(ch) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char("Whom do you wish to torture?\n\r", ch);
        return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if (victim == ch)
    {
        send_to_char( "You cannot torture yourself.\n\r", ch );
        return;
    }

    if(!IS_EXTRA(victim, TIED_UP))
    { 
	send_to_char( "You can only torture somebody who is tied up.\n\r", ch);
	return;
    }

    act("You start to torture $N.",ch,NULL,victim,TO_CHAR);
    act("$n starts to torture $N.",ch,NULL,victim,TO_ROOM);
    send_to_char("You are being tortured!\n\r",victim);
    sprintf(buf,"%s has been tortured by %s.",victim->name,ch->name);
    do_info(ch,buf);
    return;

}

void do_ihide( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
    if (!IS_CLASS(ch, CLASS_HUNTER) )
    {
        stc("Huh?\n\r", ch);
        return;
    }

/*
    if (IS_CLASS(ch, CLASS_HUNTER) && ch->pcdata->powers[NPOWER_SORA] < 3 )
    {
    stc("You have not learned the Sora principle to 3.\n\r", ch);
    return;
    }
*/

    if (ch->fight_timer >0)
    {
        stc("Not until your fight timer expires.\n\r", ch );
        return;
    }	

	if ( ch->move < 50 )
	{
		send_to_char("You don't have 50 move to activate your power.\n\r", ch);
		return;
	} 

    if ( IS_AFFECTED(ch, AFF_HIDE) )
    {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM );
	stc( "You appear from the shadows.\n\r", ch );
    }

    else
    {
	act( "$n disappears into the shadows.", ch, NULL, NULL, TO_ROOM );
	stc( "You disappear into the shadows.\n\r", ch );
	SET_BIT(ch->affected_by, AFF_HIDE);
	ch->mana -= 50;
    }
    return;
}